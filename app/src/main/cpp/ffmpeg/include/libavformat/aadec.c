/*
 * Audible AA demuxer
 * Copyright (c) 2015 Vesselin Bontchev
 *
 * Header parsing is borrowed from https://github.com/jteeuwen/audible project.
 * Copyright (c) 2001-2014, Jim Teeuwen
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "avformat.h"
#include "internal.h"
#include "libavutil/dict.h"
#include "libavutil/intreadwrite.h"
#include "libavutil/tea.h"
#include "libavutil/opt.h"

#define AA_MAGIC 1469084982 /* this identifies an audible .aa file */
#define MAX_CODEC_SECOND_SIZE 3982
#define MAX_TOC_ENTRIES 16
#define MAX_DICTIONARY_ENTRIES 128
#define TEA_BLOCK_SIZE 8

typedef struct AADemuxContext {
    AVClass *class;
    uint8_t *aa_fixed_key;
    int aa_fixed_key_len;
    int codec_second_size;
    int current_codec_second_size;
    int chapter_idx;
    struct AVTEA *tea_ctx;
    uint8_t file_key[16];
    int64_t current_chapter_size;
} AADemuxContext;

static int get_second_size(char *codec_name)
{
    int result = -1;

    if (!strcmp(codec_name, "mp332")) {
        result = 3982;
    } else if (!strcmp(codec_name, "acelp16")) {
        result = 2000;
    } else if (!strcmp(codec_name, "acelp85")) {
        result = 1045;
    }

    return result;
}

static int aa_read_header(AVFormatContext *s)
{
    int i, j, idx, largest_idx = -1;
    uint32_t nkey, nval, toc_size, npairs, header_seed = 0, start;
    char key[128], val[128], codec_name[64] = {0};
    uint8_t output[24], dst[8], src[8];
    int64_t largest_size = -1, current_size = -1;
    struct toc_entry {
        uint32_t offset;
        uint32_t size;
    } TOC[MAX_TOC_ENTRIES];
    uint32_t header_key_part[4];
    uint8_t header_key[16] = {0};
    AADemuxContext *c = s->priv_data;
    AVIOContext *pb = s->pb;
    AVStream *st;

    /* parse .aa header */
    avio_skip(pb, 4); // file size
    avio_skip(pb, 4); // magic string
    toc_size = avio_rb32(pb); // TOC size
    avio_skip(pb, 4); // unidentified integer
    if (toc_size > MAX_TOC_ENTRIES)
        return AVERROR_INVALIDDATA;
    for (i = 0; i < toc_size; i++) { // read TOC
        avio_skip(pb, 4); // TOC entry index
        TOC[i].offset = avio_rb32(pb); // block offset
        TOC[i].size = avio_rb32(pb); // block size
    }
    avio_skip(pb, 24); // header termination block (ignored)
    npairs = avio_rb32(pb); // read dictionary entries
    if (npairs > MAX_DICTIONARY_ENTRIES)
        return AVERROR_INVALIDDATA;
    for (i = 0; i < npairs; i++) {
        memset(val, 0, sizeof(val));
        memset(key, 0, sizeof(key));
        avio_skip(pb, 1); // unidentified integer
        nkey = avio_rb32(pb); // key string length
        nval = avio_rb32(pb); // value string length
        avio_get_str(pb, nkey, key, sizeof(key));
        avio_get_str(pb, nval, val, sizeof(val));
        if (!strcmp(key, "codec")) {
            av_log(s, AV_LOG_DEBUG, "Codec is <%s>\n", val);
            strncpy(codec_name, val, sizeof(codec_name) - 1);
        } else if (!strcmp(key, "HeaderSeed")) {
            av_log(s, AV_LOG_DEBUG, "HeaderSeed is <%s>\n", val);
            header_seed = atoi(val);
        } else if (!strcmp(key, "HeaderKey")) { // this looks like "1234567890 1234567890 1234567890 1234567890"
            av_log(s, AV_LOG_DEBUG, "HeaderKey is <%s>\n", val);
            sscanf(val, "%"SCNu32"%"SCNu32"%"SCNu32"%"SCNu32,
                   &header_key_part[0], &header_key_part[1], &header_key_part[2], &header_key_part[3]);
            for (idx = 0; idx < 4; idx++) {
                AV_WB32(&header_key[idx * 4], header_key_part[idx]); // convert each part to BE!
            }
            av_log(s, AV_LOG_DEBUG, "Processed HeaderKey is ");
            for (i = 0; i < 16; i++)
                av_log(s, AV_LOG_DEBUG, "%02x", header_key[i]);
            av_log(s, AV_LOG_DEBUG, "\n");
        } else {
            av_dict_set(&s->metadata, key, val, 0);
        }
    }

    /* verify fixed key */
    if (c->aa_fixed_key_len != 16) {
        av_log(s, AV_LOG_ERROR, "aa_fixed_key value needs to be 16 bytes!\n");
        return AVERROR(EINVAL);
    }

    /* verify codec */
    if ((c->codec_second_size = get_second_size(codec_name)) == -1) {
        av_log(s, AV_LOG_ERROR, "unknown codec <%s>!\n", codec_name);
        return AVERROR(EINVAL);
    }

    /* decryption key derivation */
    c->tea_ctx = av_tea_alloc();
    if (!c->tea_ctx)
        return AVERROR(ENOMEM);
    av_tea_init(c->tea_ctx, c->aa_fixed_key, 16);
    output[0] = output[1] = 0; // purely for padding purposes
    memcpy(output + 2, header_key, 16);
    idx = 0;
    for (i = 0; i < 3; i++) { // TEA CBC with weird mixed endianness
        AV_WB32(src, header_seed);
        AV_WB32(src + 4, header_seed + 1);
        header_seed += 2;
        av_tea_crypt(c->tea_ctx, dst, src, 1, NULL, 0); // TEA ECB encrypt
        for (j = 0; j < TEA_BLOCK_SIZE && idx < 18; j+=1, idx+=1) {
            output[idx] = output[idx] ^ dst[j];
        }
    }
    memcpy(c->file_key, output + 2, 16); // skip first 2 bytes of output
    av_log(s, AV_LOG_DEBUG, "File key is ");
    for (i = 0; i < 16; i++)
        av_log(s, AV_LOG_DEBUG, "%02x", c->file_key[i]);
    av_log(s, AV_LOG_DEBUG, "\n");

    /* decoder setup */
    st = avformat_new_stream(s, NULL);
    if (!st) {
        av_freep(&c->tea_ctx);
        return AVERROR(ENOMEM);
    }
    st->codecpar->codec_type = AVMEDIA_TYPE_AUDIO;
    if (!strcmp(codec_name, "mp332")) {
        st->codecpar->codec_id = AV_CODEC_ID_MP3;
        st->codecpar->sample_rate = 22050;
        st->need_parsing = AVSTREAM_PARSE_FULL_RAW;
        st->start_time = 0;
    } else if (!strcmp(codec_name, "acelp85")) {
        st->codecpar->codec_id = AV_CODEC_ID_SIPR;
        st->codecpar->block_align = 19;
        st->codecpar->channels = 1;
        st->codecpar->sample_rate = 8500;
        st->need_parsing = AVSTREAM_PARSE_FULL_RAW;
    } else if (!strcmp(codec_name, "acelp16")) {
        st->codecpar->codec_id = AV_CODEC_ID_SIPR;
        st->codecpar->block_align = 20;
        st->codecpar->channels = 1;
        st->codecpar->sample_rate = 16000;
        st->need_parsing = AVSTREAM_PARSE_FULL_RAW;
    }

    /* determine, and jump to audio start offset */
    for (i = 1; i < toc_size; i++) { // skip the first entry!
        current_size = TOC[i].size;
        if (current_size > largest_size) {
            largest_idx = i;
            largest_size = current_size;
        }
    }
    start = TOC[largest_idx].offset;
    avio_seek(pb, start, SEEK_SET);
    c->current_chapter_size = 0;

    return 0;
}

static int aa_read_packet(AVFormatContext *s, AVPacket *pkt)
{
    uint8_t dst[TEA_BLOCK_SIZE];
    uint8_t src[TEA_BLOCK_SIZE];
    int i;
    int trailing_bytes;
    int blocks;
    uint8_t buf[MAX_CODEC_SECOND_SIZE * 2];
    int written = 0;
    int ret;
    AADemuxContext *c = s->priv_data;

    // are we at the start of a chapter?
    if (c->current_chapter_size == 0) {
        c->current_chapter_size = avio_rb32(s->pb);
        if (c->current_chapter_size == 0) {
            return AVERROR_EOF;
        }
        av_log(s, AV_LOG_DEBUG, "Chapter %d (%" PRId64 " bytes)\n", c->chapter_idx, c->current_chapter_size);
        c->chapter_idx = c->chapter_idx + 1;
        avio_skip(s->pb, 4); // data start offset
        c->current_codec_second_size = c->codec_second_size;
    }

    // is this the last block in this chapter?
    if (c->current_chapter_size / c->current_codec_second_size == 0) {
        c->current_codec_second_size = c->current_chapter_size % c->current_codec_second_size;
    }

    // decrypt c->current_codec_second_size bytes
    blocks = c->current_codec_second_size / TEA_BLOCK_SIZE;
    for (i = 0; i < blocks; i++) {
        avio_read(s->pb, src, TEA_BLOCK_SIZE);
        av_tea_init(c->tea_ctx, c->file_key, 16);
        av_tea_crypt(c->tea_ctx, dst, src, 1, NULL, 1);
        memcpy(buf + written, dst, TEA_BLOCK_SIZE);
        written = written + TEA_BLOCK_SIZE;
    }
    trailing_bytes = c->current_codec_second_size % TEA_BLOCK_SIZE;
    if (trailing_bytes != 0) { // trailing bytes are left unencrypted!
        avio_read(s->pb, src, trailing_bytes);
        memcpy(buf + written, src, trailing_bytes);
        written = written + trailing_bytes;
    }

    // update state
    c->current_chapter_size = c->current_chapter_size - c->current_codec_second_size;
    if (c->current_chapter_size <= 0)
        c->current_chapter_size = 0;

    ret = av_new_packet(pkt, written);
    if (ret < 0)
        return ret;
    memcpy(pkt->data, buf, written);

    return 0;
}

static int aa_probe(AVProbeData *p)
{
    uint8_t *buf = p->buf;

    // first 4 bytes are file size, next 4 bytes are the magic
    if (AV_RB32(buf+4) != AA_MAGIC)
        return 0;

    return AVPROBE_SCORE_MAX / 2;
}

static int aa_read_close(AVFormatContext *s)
{
    AADemuxContext *c = s->priv_data;

    av_freep(&c->tea_ctx);

    return 0;
}

#define OFFSET(x) offsetof(AADemuxContext, x)
static const AVOption aa_options[] = {
    { "aa_fixed_key", // extracted from libAAX_SDK.so and AAXSDKWin.dll files!
        "Fixed key used for handling Audible AA files", OFFSET(aa_fixed_key),
        AV_OPT_TYPE_BINARY, {.str="77214d4b196a87cd520045fd2a51d673"},
        .flags = AV_OPT_FLAG_DECODING_PARAM },
    { NULL },
};

static const AVClass aa_class = {
    .class_name = "aa",
    .item_name  = av_default_item_name,
    .option     = aa_options,
    .version    = LIBAVUTIL_VERSION_INT,
};

AVInputFormat ff_aa_demuxer = {
    .name           = "aa",
    .long_name      = NULL_IF_CONFIG_SMALL("Audible AA format files"),
    .priv_class     = &aa_class,
    .priv_data_size = sizeof(AADemuxContext),
    .extensions     = "aa",
    .read_probe     = aa_probe,
    .read_header    = aa_read_header,
    .read_packet    = aa_read_packet,
    .read_close     = aa_read_close,
    .flags          = AVFMT_GENERIC_INDEX,
};
