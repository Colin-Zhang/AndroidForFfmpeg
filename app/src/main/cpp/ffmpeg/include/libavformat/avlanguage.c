/*
 * Cyril Comparon, Larbi Joubala, Resonate-MP4 2009
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "avlanguage.h"
#include "libavutil/avstring.h"
#include "libavutil/common.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct LangEntry {
    const char str[4];
    uint16_t next_equivalent;
} LangEntry;

static const uint16_t lang_table_counts[] = { 484, 20, 184 };
static const uint16_t lang_table_offsets[] = { 0, 484, 504 };

static const LangEntry lang_table[] = {
    /*----- AV_LANG_ISO639_2_BIBL entries (484) -----*/
    /*0000*/ { "aar",  504 },
    /*0001*/ { "abk",  505 },
    /*0002*/ { "ace",    2 },
    /*0003*/ { "ach",    3 },
    /*0004*/ { "ada",    4 },
    /*0005*/ { "ady",    5 },
    /*0006*/ { "afa",    6 },
    /*0007*/ { "afh",    7 },
    /*0008*/ { "afr",  507 },
    /*0009*/ { "ain",    9 },
    /*0010*/ { "aka",  508 },
    /*0011*/ { "akk",   11 },
    /*0012*/ { "alb",  502 },
    /*0013*/ { "ale",   13 },
    /*0014*/ { "alg",   14 },
    /*0015*/ { "alt",   15 },
    /*0016*/ { "amh",  509 },
    /*0017*/ { "ang",   17 },
    /*0018*/ { "anp",   18 },
    /*0019*/ { "apa",   19 },
    /*0020*/ { "ara",  511 },
    /*0021*/ { "arc",   21 },
    /*0022*/ { "arg",  510 },
    /*0023*/ { "arm",  492 },
    /*0024*/ { "arn",   24 },
    /*0025*/ { "arp",   25 },
    /*0026*/ { "art",   26 },
    /*0027*/ { "arw",   27 },
    /*0028*/ { "asm",  512 },
    /*0029*/ { "ast",   29 },
    /*0030*/ { "ath",   30 },
    /*0031*/ { "aus",   31 },
    /*0032*/ { "ava",  513 },
    /*0033*/ { "ave",  506 },
    /*0034*/ { "awa",   34 },
    /*0035*/ { "aym",  514 },
    /*0036*/ { "aze",  515 },
    /*0037*/ { "bad",   37 },
    /*0038*/ { "bai",   38 },
    /*0039*/ { "bak",  516 },
    /*0040*/ { "bal",   40 },
    /*0041*/ { "bam",  521 },
    /*0042*/ { "ban",   42 },
    /*0043*/ { "baq",  489 },
    /*0044*/ { "bas",   44 },
    /*0045*/ { "bat",   45 },
    /*0046*/ { "bej",   46 },
    /*0047*/ { "bel",  517 },
    /*0048*/ { "bem",   48 },
    /*0049*/ { "ben",  522 },
    /*0050*/ { "ber",   50 },
    /*0051*/ { "bho",   51 },
    /*0052*/ { "bih",  519 },
    /*0053*/ { "bik",   53 },
    /*0054*/ { "bin",   54 },
    /*0055*/ { "bis",  520 },
    /*0056*/ { "bla",   56 },
    /*0057*/ { "bnt",   57 },
    /*0058*/ { "bos",  525 },
    /*0059*/ { "bra",   59 },
    /*0060*/ { "bre",  524 },
    /*0061*/ { "btk",   61 },
    /*0062*/ { "bua",   62 },
    /*0063*/ { "bug",   63 },
    /*0064*/ { "bul",  518 },
    /*0065*/ { "bur",  498 },
    /*0066*/ { "byn",   66 },
    /*0067*/ { "cad",   67 },
    /*0068*/ { "cai",   68 },
    /*0069*/ { "car",   69 },
    /*0070*/ { "cat",  526 },
    /*0071*/ { "cau",   71 },
    /*0072*/ { "ceb",   72 },
    /*0073*/ { "cel",   73 },
    /*0074*/ { "cha",  528 },
    /*0075*/ { "chb",   75 },
    /*0076*/ { "che",  527 },
    /*0077*/ { "chg",   77 },
    /*0078*/ { "chi",  503 },
    /*0079*/ { "chk",   79 },
    /*0080*/ { "chm",   80 },
    /*0081*/ { "chn",   81 },
    /*0082*/ { "cho",   82 },
    /*0083*/ { "chp",   83 },
    /*0084*/ { "chr",   84 },
    /*0085*/ { "chu",  532 },
    /*0086*/ { "chv",  533 },
    /*0087*/ { "chy",   87 },
    /*0088*/ { "cmc",   88 },
    /*0089*/ { "cop",   89 },
    /*0090*/ { "cor",  593 },
    /*0091*/ { "cos",  529 },
    /*0092*/ { "cpe",   92 },
    /*0093*/ { "cpf",   93 },
    /*0094*/ { "cpp",   94 },
    /*0095*/ { "cre",  530 },
    /*0096*/ { "crh",   96 },
    /*0097*/ { "crp",   97 },
    /*0098*/ { "csb",   98 },
    /*0099*/ { "cus",   99 },
    /*0100*/ { "cze",  485 },
    /*0101*/ { "dak",  101 },
    /*0102*/ { "dan",  535 },
    /*0103*/ { "dar",  103 },
    /*0104*/ { "day",  104 },
    /*0105*/ { "del",  105 },
    /*0106*/ { "den",  106 },
    /*0107*/ { "dgr",  107 },
    /*0108*/ { "din",  108 },
    /*0109*/ { "div",  537 },
    /*0110*/ { "doi",  110 },
    /*0111*/ { "dra",  111 },
    /*0112*/ { "dsb",  112 },
    /*0113*/ { "dua",  113 },
    /*0114*/ { "dum",  114 },
    /*0115*/ { "dut",  499 },
    /*0116*/ { "dyu",  116 },
    /*0117*/ { "dzo",  538 },
    /*0118*/ { "efi",  118 },
    /*0119*/ { "egy",  119 },
    /*0120*/ { "eka",  120 },
    /*0121*/ { "elx",  121 },
    /*0122*/ { "eng",  541 },
    /*0123*/ { "enm",  123 },
    /*0124*/ { "epo",  542 },
    /*0125*/ { "est",  544 },
    /*0126*/ { "ewe",  539 },
    /*0127*/ { "ewo",  127 },
    /*0128*/ { "fan",  128 },
    /*0129*/ { "fao",  550 },
    /*0130*/ { "fat",  130 },
    /*0131*/ { "fij",  549 },
    /*0132*/ { "fil",  132 },
    /*0133*/ { "fin",  548 },
    /*0134*/ { "fiu",  134 },
    /*0135*/ { "fon",  135 },
    /*0136*/ { "fre",  491 },
    /*0137*/ { "frm",  137 },
    /*0138*/ { "fro",  138 },
    /*0139*/ { "frr",  139 },
    /*0140*/ { "frs",  140 },
    /*0141*/ { "fry",  552 },
    /*0142*/ { "ful",  547 },
    /*0143*/ { "fur",  143 },
    /*0144*/ { "gaa",  144 },
    /*0145*/ { "gay",  145 },
    /*0146*/ { "gba",  146 },
    /*0147*/ { "gem",  147 },
    /*0148*/ { "geo",  494 },
    /*0149*/ { "ger",  487 },
    /*0150*/ { "gez",  150 },
    /*0151*/ { "gil",  151 },
    /*0152*/ { "gla",  554 },
    /*0153*/ { "gle",  553 },
    /*0154*/ { "glg",  555 },
    /*0155*/ { "glv",  558 },
    /*0156*/ { "gmh",  156 },
    /*0157*/ { "goh",  157 },
    /*0158*/ { "gon",  158 },
    /*0159*/ { "gor",  159 },
    /*0160*/ { "got",  160 },
    /*0161*/ { "grb",  161 },
    /*0162*/ { "grc",  162 },
    /*0163*/ { "gre",  488 },
    /*0164*/ { "grn",  556 },
    /*0165*/ { "gsw",  165 },
    /*0166*/ { "guj",  557 },
    /*0167*/ { "gwi",  167 },
    /*0168*/ { "hai",  168 },
    /*0169*/ { "hat",  564 },
    /*0170*/ { "hau",  559 },
    /*0171*/ { "haw",  171 },
    /*0172*/ { "heb",  560 },
    /*0173*/ { "her",  567 },
    /*0174*/ { "hil",  174 },
    /*0175*/ { "him",  175 },
    /*0176*/ { "hin",  561 },
    /*0177*/ { "hit",  177 },
    /*0178*/ { "hmn",  178 },
    /*0179*/ { "hmo",  562 },
    /*0180*/ { "hrv",  563 },
    /*0181*/ { "hsb",  181 },
    /*0182*/ { "hun",  565 },
    /*0183*/ { "hup",  183 },
    /*0184*/ { "iba",  184 },
    /*0185*/ { "ibo",  571 },
    /*0186*/ { "ice",  493 },
    /*0187*/ { "ido",  574 },
    /*0188*/ { "iii",  572 },
    /*0189*/ { "ijo",  189 },
    /*0190*/ { "iku",  577 },
    /*0191*/ { "ile",  570 },
    /*0192*/ { "ilo",  192 },
    /*0193*/ { "ina",  568 },
    /*0194*/ { "inc",  194 },
    /*0195*/ { "ind",  569 },
    /*0196*/ { "ine",  196 },
    /*0197*/ { "inh",  197 },
    /*0198*/ { "ipk",  573 },
    /*0199*/ { "ira",  199 },
    /*0200*/ { "iro",  200 },
    /*0201*/ { "ita",  576 },
    /*0202*/ { "jav",  579 },
    /*0203*/ { "jbo",  203 },
    /*0204*/ { "jpn",  578 },
    /*0205*/ { "jpr",  205 },
    /*0206*/ { "jrb",  206 },
    /*0207*/ { "kaa",  207 },
    /*0208*/ { "kab",  208 },
    /*0209*/ { "kac",  209 },
    /*0210*/ { "kal",  585 },
    /*0211*/ { "kam",  211 },
    /*0212*/ { "kan",  587 },
    /*0213*/ { "kar",  213 },
    /*0214*/ { "kas",  590 },
    /*0215*/ { "kau",  589 },
    /*0216*/ { "kaw",  216 },
    /*0217*/ { "kaz",  584 },
    /*0218*/ { "kbd",  218 },
    /*0219*/ { "kha",  219 },
    /*0220*/ { "khi",  220 },
    /*0221*/ { "khm",  586 },
    /*0222*/ { "kho",  222 },
    /*0223*/ { "kik",  582 },
    /*0224*/ { "kin",  640 },
    /*0225*/ { "kir",  594 },
    /*0226*/ { "kmb",  226 },
    /*0227*/ { "kok",  227 },
    /*0228*/ { "kom",  592 },
    /*0229*/ { "kon",  581 },
    /*0230*/ { "kor",  588 },
    /*0231*/ { "kos",  231 },
    /*0232*/ { "kpe",  232 },
    /*0233*/ { "krc",  233 },
    /*0234*/ { "krl",  234 },
    /*0235*/ { "kro",  235 },
    /*0236*/ { "kru",  236 },
    /*0237*/ { "kua",  583 },
    /*0238*/ { "kum",  238 },
    /*0239*/ { "kur",  591 },
    /*0240*/ { "kut",  240 },
    /*0241*/ { "lad",  241 },
    /*0242*/ { "lah",  242 },
    /*0243*/ { "lam",  243 },
    /*0244*/ { "lao",  600 },
    /*0245*/ { "lat",  595 },
    /*0246*/ { "lav",  603 },
    /*0247*/ { "lez",  247 },
    /*0248*/ { "lim",  598 },
    /*0249*/ { "lin",  599 },
    /*0250*/ { "lit",  601 },
    /*0251*/ { "lol",  251 },
    /*0252*/ { "loz",  252 },
    /*0253*/ { "ltz",  596 },
    /*0254*/ { "lua",  254 },
    /*0255*/ { "lub",  602 },
    /*0256*/ { "lug",  597 },
    /*0257*/ { "lui",  257 },
    /*0258*/ { "lun",  258 },
    /*0259*/ { "luo",  259 },
    /*0260*/ { "lus",  260 },
    /*0261*/ { "mac",  495 },
    /*0262*/ { "mad",  262 },
    /*0263*/ { "mag",  263 },
    /*0264*/ { "mah",  605 },
    /*0265*/ { "mai",  265 },
    /*0266*/ { "mak",  266 },
    /*0267*/ { "mal",  608 },
    /*0268*/ { "man",  268 },
    /*0269*/ { "mao",  496 },
    /*0270*/ { "map",  270 },
    /*0271*/ { "mar",  610 },
    /*0272*/ { "mas",  272 },
    /*0273*/ { "may",  497 },
    /*0274*/ { "mdf",  274 },
    /*0275*/ { "mdr",  275 },
    /*0276*/ { "men",  276 },
    /*0277*/ { "mga",  277 },
    /*0278*/ { "mic",  278 },
    /*0279*/ { "min",  279 },
    /*0280*/ { "mis",  280 },
    /*0281*/ { "mkh",  281 },
    /*0282*/ { "mlg",  604 },
    /*0283*/ { "mlt",  612 },
    /*0284*/ { "mnc",  284 },
    /*0285*/ { "mni",  285 },
    /*0286*/ { "mno",  286 },
    /*0287*/ { "moh",  287 },
    /*0288*/ { "mon",  609 },
    /*0289*/ { "mos",  289 },
    /*0290*/ { "mul",  290 },
    /*0291*/ { "mun",  291 },
    /*0292*/ { "mus",  292 },
    /*0293*/ { "mwl",  293 },
    /*0294*/ { "mwr",  294 },
    /*0295*/ { "myn",  295 },
    /*0296*/ { "myv",  296 },
    /*0297*/ { "nah",  297 },
    /*0298*/ { "nai",  298 },
    /*0299*/ { "nap",  299 },
    /*0300*/ { "nau",  614 },
    /*0301*/ { "nav",  623 },
    /*0302*/ { "nbl",  622 },
    /*0303*/ { "nde",  616 },
    /*0304*/ { "ndo",  618 },
    /*0305*/ { "nds",  305 },
    /*0306*/ { "nep",  617 },
    /*0307*/ { "new",  307 },
    /*0308*/ { "nia",  308 },
    /*0309*/ { "nic",  309 },
    /*0310*/ { "niu",  310 },
    /*0311*/ { "nno",  620 },
    /*0312*/ { "nob",  615 },
    /*0313*/ { "nog",  313 },
    /*0314*/ { "non",  314 },
    /*0315*/ { "nor",  621 },
    /*0316*/ { "nqo",  316 },
    /*0317*/ { "nso",  317 },
    /*0318*/ { "nub",  318 },
    /*0319*/ { "nwc",  319 },
    /*0320*/ { "nya",  624 },
    /*0321*/ { "nym",  321 },
    /*0322*/ { "nyn",  322 },
    /*0323*/ { "nyo",  323 },
    /*0324*/ { "nzi",  324 },
    /*0325*/ { "oci",  625 },
    /*0326*/ { "oji",  626 },
    /*0327*/ { "ori",  628 },
    /*0328*/ { "orm",  627 },
    /*0329*/ { "osa",  329 },
    /*0330*/ { "oss",  629 },
    /*0331*/ { "ota",  331 },
    /*0332*/ { "oto",  332 },
    /*0333*/ { "paa",  333 },
    /*0334*/ { "pag",  334 },
    /*0335*/ { "pal",  335 },
    /*0336*/ { "pam",  336 },
    /*0337*/ { "pan",  630 },
    /*0338*/ { "pap",  338 },
    /*0339*/ { "pau",  339 },
    /*0340*/ { "peo",  340 },
    /*0341*/ { "per",  490 },
    /*0342*/ { "phi",  342 },
    /*0343*/ { "phn",  343 },
    /*0344*/ { "pli",  631 },
    /*0345*/ { "pol",  632 },
    /*0346*/ { "pon",  346 },
    /*0347*/ { "por",  634 },
    /*0348*/ { "pra",  348 },
    /*0349*/ { "pro",  349 },
    /*0350*/ { "pus",  633 },
    /*0351*/ { "que",  635 },
    /*0352*/ { "raj",  352 },
    /*0353*/ { "rap",  353 },
    /*0354*/ { "rar",  354 },
    /*0355*/ { "roa",  355 },
    /*0356*/ { "roh",  636 },
    /*0357*/ { "rom",  357 },
    /*0358*/ { "rum",  500 },
    /*0359*/ { "run",  637 },
    /*0360*/ { "rup",  360 },
    /*0361*/ { "rus",  639 },
    /*0362*/ { "sad",  362 },
    /*0363*/ { "sag",  645 },
    /*0364*/ { "sah",  364 },
    /*0365*/ { "sai",  365 },
    /*0366*/ { "sal",  366 },
    /*0367*/ { "sam",  367 },
    /*0368*/ { "san",  641 },
    /*0369*/ { "sas",  369 },
    /*0370*/ { "sat",  370 },
    /*0371*/ { "scn",  371 },
    /*0372*/ { "sco",  372 },
    /*0373*/ { "sel",  373 },
    /*0374*/ { "sem",  374 },
    /*0375*/ { "sga",  375 },
    /*0376*/ { "sgn",  376 },
    /*0377*/ { "shn",  377 },
    /*0378*/ { "sid",  378 },
    /*0379*/ { "sin",  646 },
    /*0380*/ { "sio",  380 },
    /*0381*/ { "sit",  381 },
    /*0382*/ { "sla",  382 },
    /*0383*/ { "slo",  501 },
    /*0384*/ { "slv",  648 },
    /*0385*/ { "sma",  385 },
    /*0386*/ { "sme",  644 },
    /*0387*/ { "smi",  387 },
    /*0388*/ { "smj",  388 },
    /*0389*/ { "smn",  389 },
    /*0390*/ { "smo",  649 },
    /*0391*/ { "sms",  391 },
    /*0392*/ { "sna",  650 },
    /*0393*/ { "snd",  643 },
    /*0394*/ { "snk",  394 },
    /*0395*/ { "sog",  395 },
    /*0396*/ { "som",  651 },
    /*0397*/ { "son",  397 },
    /*0398*/ { "sot",  655 },
    /*0399*/ { "spa",  543 },
    /*0400*/ { "srd",  642 },
    /*0401*/ { "srn",  401 },
    /*0402*/ { "srp",  653 },
    /*0403*/ { "srr",  403 },
    /*0404*/ { "ssa",  404 },
    /*0405*/ { "ssw",  654 },
    /*0406*/ { "suk",  406 },
    /*0407*/ { "sun",  656 },
    /*0408*/ { "sus",  408 },
    /*0409*/ { "sux",  409 },
    /*0410*/ { "swa",  658 },
    /*0411*/ { "swe",  657 },
    /*0412*/ { "syc",  412 },
    /*0413*/ { "syr",  413 },
    /*0414*/ { "tah",  672 },
    /*0415*/ { "tai",  415 },
    /*0416*/ { "tam",  659 },
    /*0417*/ { "tat",  670 },
    /*0418*/ { "tel",  660 },
    /*0419*/ { "tem",  419 },
    /*0420*/ { "ter",  420 },
    /*0421*/ { "tet",  421 },
    /*0422*/ { "tgk",  661 },
    /*0423*/ { "tgl",  665 },
    /*0424*/ { "tha",  662 },
    /*0425*/ { "tib",  484 },
    /*0426*/ { "tig",  426 },
    /*0427*/ { "tir",  663 },
    /*0428*/ { "tiv",  428 },
    /*0429*/ { "tkl",  429 },
    /*0430*/ { "tlh",  430 },
    /*0431*/ { "tli",  431 },
    /*0432*/ { "tmh",  432 },
    /*0433*/ { "tog",  433 },
    /*0434*/ { "ton",  667 },
    /*0435*/ { "tpi",  435 },
    /*0436*/ { "tsi",  436 },
    /*0437*/ { "tsn",  666 },
    /*0438*/ { "tso",  669 },
    /*0439*/ { "tuk",  664 },
    /*0440*/ { "tum",  440 },
    /*0441*/ { "tup",  441 },
    /*0442*/ { "tur",  668 },
    /*0443*/ { "tut",  443 },
    /*0444*/ { "tvl",  444 },
    /*0445*/ { "twi",  671 },
    /*0446*/ { "tyv",  446 },
    /*0447*/ { "udm",  447 },
    /*0448*/ { "uga",  448 },
    /*0449*/ { "uig",  673 },
    /*0450*/ { "ukr",  674 },
    /*0451*/ { "umb",  451 },
    /*0452*/ { "und",  452 },
    /*0453*/ { "urd",  675 },
    /*0454*/ { "uzb",  676 },
    /*0455*/ { "vai",  455 },
    /*0456*/ { "ven",  677 },
    /*0457*/ { "vie",  678 },
    /*0458*/ { "vol",  679 },
    /*0459*/ { "vot",  459 },
    /*0460*/ { "wak",  460 },
    /*0461*/ { "wal",  461 },
    /*0462*/ { "war",  462 },
    /*0463*/ { "was",  463 },
    /*0464*/ { "wel",  486 },
    /*0465*/ { "wen",  465 },
    /*0466*/ { "wln",  680 },
    /*0467*/ { "wol",  681 },
    /*0468*/ { "xal",  468 },
    /*0469*/ { "xho",  682 },
    /*0470*/ { "yao",  470 },
    /*0471*/ { "yap",  471 },
    /*0472*/ { "yid",  683 },
    /*0473*/ { "yor",  684 },
    /*0474*/ { "ypk",  474 },
    /*0475*/ { "zap",  475 },
    /*0476*/ { "zbl",  476 },
    /*0477*/ { "zen",  477 },
    /*0478*/ { "zha",  685 },
    /*0479*/ { "znd",  479 },
    /*0480*/ { "zul",  687 },
    /*0481*/ { "zun",  481 },
    /*0482*/ { "zxx",  482 },
    /*0483*/ { "zza",  483 },
    /*----- AV_LANG_ISO639_2_TERM entries (20) -----*/
    /*0484*/ { "bod",  523 },
    /*0485*/ { "ces",  531 },
    /*0486*/ { "cym",  534 },
    /*0487*/ { "deu",  536 },
    /*0488*/ { "ell",  540 },
    /*0489*/ { "eus",  545 },
    /*0490*/ { "fas",  546 },
    /*0491*/ { "fra",  551 },
    /*0492*/ { "hye",  566 },
    /*0493*/ { "isl",  575 },
    /*0494*/ { "kat",  580 },
    /*0495*/ { "mkd",  607 },
    /*0496*/ { "mri",  606 },
    /*0497*/ { "msa",  611 },
    /*0498*/ { "mya",  613 },
    /*0499*/ { "nld",  619 },
    /*0500*/ { "ron",  638 },
    /*0501*/ { "slk",  647 },
    /*0502*/ { "sqi",  652 },
    /*0503*/ { "zho",  686 },
    /*----- AV_LANG_ISO639_1 entries (184) -----*/
    /*0504*/ { "aa" ,    0 },
    /*0505*/ { "ab" ,    1 },
    /*0506*/ { "ae" ,   33 },
    /*0507*/ { "af" ,    8 },
    /*0508*/ { "ak" ,   10 },
    /*0509*/ { "am" ,   16 },
    /*0510*/ { "an" ,   22 },
    /*0511*/ { "ar" ,   20 },
    /*0512*/ { "as" ,   28 },
    /*0513*/ { "av" ,   32 },
    /*0514*/ { "ay" ,   35 },
    /*0515*/ { "az" ,   36 },
    /*0516*/ { "ba" ,   39 },
    /*0517*/ { "be" ,   47 },
    /*0518*/ { "bg" ,   64 },
    /*0519*/ { "bh" ,   52 },
    /*0520*/ { "bi" ,   55 },
    /*0521*/ { "bm" ,   41 },
    /*0522*/ { "bn" ,   49 },
    /*0523*/ { "bo" ,  425 },
    /*0524*/ { "br" ,   60 },
    /*0525*/ { "bs" ,   58 },
    /*0526*/ { "ca" ,   70 },
    /*0527*/ { "ce" ,   76 },
    /*0528*/ { "ch" ,   74 },
    /*0529*/ { "co" ,   91 },
    /*0530*/ { "cr" ,   95 },
    /*0531*/ { "cs" ,  100 },
    /*0532*/ { "cu" ,   85 },
    /*0533*/ { "cv" ,   86 },
    /*0534*/ { "cy" ,  464 },
    /*0535*/ { "da" ,  102 },
    /*0536*/ { "de" ,  149 },
    /*0537*/ { "dv" ,  109 },
    /*0538*/ { "dz" ,  117 },
    /*0539*/ { "ee" ,  126 },
    /*0540*/ { "el" ,  163 },
    /*0541*/ { "en" ,  122 },
    /*0542*/ { "eo" ,  124 },
    /*0543*/ { "es" ,  399 },
    /*0544*/ { "et" ,  125 },
    /*0545*/ { "eu" ,   43 },
    /*0546*/ { "fa" ,  341 },
    /*0547*/ { "ff" ,  142 },
    /*0548*/ { "fi" ,  133 },
    /*0549*/ { "fj" ,  131 },
    /*0550*/ { "fo" ,  129 },
    /*0551*/ { "fr" ,  136 },
    /*0552*/ { "fy" ,  141 },
    /*0553*/ { "ga" ,  153 },
    /*0554*/ { "gd" ,  152 },
    /*0555*/ { "gl" ,  154 },
    /*0556*/ { "gn" ,  164 },
    /*0557*/ { "gu" ,  166 },
    /*0558*/ { "gv" ,  155 },
    /*0559*/ { "ha" ,  170 },
    /*0560*/ { "he" ,  172 },
    /*0561*/ { "hi" ,  176 },
    /*0562*/ { "ho" ,  179 },
    /*0563*/ { "hr" ,  180 },
    /*0564*/ { "ht" ,  169 },
    /*0565*/ { "hu" ,  182 },
    /*0566*/ { "hy" ,   23 },
    /*0567*/ { "hz" ,  173 },
    /*0568*/ { "ia" ,  193 },
    /*0569*/ { "id" ,  195 },
    /*0570*/ { "ie" ,  191 },
    /*0571*/ { "ig" ,  185 },
    /*0572*/ { "ii" ,  188 },
    /*0573*/ { "ik" ,  198 },
    /*0574*/ { "io" ,  187 },
    /*0575*/ { "is" ,  186 },
    /*0576*/ { "it" ,  201 },
    /*0577*/ { "iu" ,  190 },
    /*0578*/ { "ja" ,  204 },
    /*0579*/ { "jv" ,  202 },
    /*0580*/ { "ka" ,  148 },
    /*0581*/ { "kg" ,  229 },
    /*0582*/ { "ki" ,  223 },
    /*0583*/ { "kj" ,  237 },
    /*0584*/ { "kk" ,  217 },
    /*0585*/ { "kl" ,  210 },
    /*0586*/ { "km" ,  221 },
    /*0587*/ { "kn" ,  212 },
    /*0588*/ { "ko" ,  230 },
    /*0589*/ { "kr" ,  215 },
    /*0590*/ { "ks" ,  214 },
    /*0591*/ { "ku" ,  239 },
    /*0592*/ { "kv" ,  228 },
    /*0593*/ { "kw" ,   90 },
    /*0594*/ { "ky" ,  225 },
    /*0595*/ { "la" ,  245 },
    /*0596*/ { "lb" ,  253 },
    /*0597*/ { "lg" ,  256 },
    /*0598*/ { "li" ,  248 },
    /*0599*/ { "ln" ,  249 },
    /*0600*/ { "lo" ,  244 },
    /*0601*/ { "lt" ,  250 },
    /*0602*/ { "lu" ,  255 },
    /*0603*/ { "lv" ,  246 },
    /*0604*/ { "mg" ,  282 },
    /*0605*/ { "mh" ,  264 },
    /*0606*/ { "mi" ,  269 },
    /*0607*/ { "mk" ,  261 },
    /*0608*/ { "ml" ,  267 },
    /*0609*/ { "mn" ,  288 },
    /*0610*/ { "mr" ,  271 },
    /*0611*/ { "ms" ,  273 },
    /*0612*/ { "mt" ,  283 },
    /*0613*/ { "my" ,   65 },
    /*0614*/ { "na" ,  300 },
    /*0615*/ { "nb" ,  312 },
    /*0616*/ { "nd" ,  303 },
    /*0617*/ { "ne" ,  306 },
    /*0618*/ { "ng" ,  304 },
    /*0619*/ { "nl" ,  115 },
    /*0620*/ { "nn" ,  311 },
    /*0621*/ { "no" ,  315 },
    /*0622*/ { "nr" ,  302 },
    /*0623*/ { "nv" ,  301 },
    /*0624*/ { "ny" ,  320 },
    /*0625*/ { "oc" ,  325 },
    /*0626*/ { "oj" ,  326 },
    /*0627*/ { "om" ,  328 },
    /*0628*/ { "or" ,  327 },
    /*0629*/ { "os" ,  330 },
    /*0630*/ { "pa" ,  337 },
    /*0631*/ { "pi" ,  344 },
    /*0632*/ { "pl" ,  345 },
    /*0633*/ { "ps" ,  350 },
    /*0634*/ { "pt" ,  347 },
    /*0635*/ { "qu" ,  351 },
    /*0636*/ { "rm" ,  356 },
    /*0637*/ { "rn" ,  359 },
    /*0638*/ { "ro" ,  358 },
    /*0639*/ { "ru" ,  361 },
    /*0640*/ { "rw" ,  224 },
    /*0641*/ { "sa" ,  368 },
    /*0642*/ { "sc" ,  400 },
    /*0643*/ { "sd" ,  393 },
    /*0644*/ { "se" ,  386 },
    /*0645*/ { "sg" ,  363 },
    /*0646*/ { "si" ,  379 },
    /*0647*/ { "sk" ,  383 },
    /*0648*/ { "sl" ,  384 },
    /*0649*/ { "sm" ,  390 },
    /*0650*/ { "sn" ,  392 },
    /*0651*/ { "so" ,  396 },
    /*0652*/ { "sq" ,   12 },
    /*0653*/ { "sr" ,  402 },
    /*0654*/ { "ss" ,  405 },
    /*0655*/ { "st" ,  398 },
    /*0656*/ { "su" ,  407 },
    /*0657*/ { "sv" ,  411 },
    /*0658*/ { "sw" ,  410 },
    /*0659*/ { "ta" ,  416 },
    /*0660*/ { "te" ,  418 },
    /*0661*/ { "tg" ,  422 },
    /*0662*/ { "th" ,  424 },
    /*0663*/ { "ti" ,  427 },
    /*0664*/ { "tk" ,  439 },
    /*0665*/ { "tl" ,  423 },
    /*0666*/ { "tn" ,  437 },
    /*0667*/ { "to" ,  434 },
    /*0668*/ { "tr" ,  442 },
    /*0669*/ { "ts" ,  438 },
    /*0670*/ { "tt" ,  417 },
    /*0671*/ { "tw" ,  445 },
    /*0672*/ { "ty" ,  414 },
    /*0673*/ { "ug" ,  449 },
    /*0674*/ { "uk" ,  450 },
    /*0675*/ { "ur" ,  453 },
    /*0676*/ { "uz" ,  454 },
    /*0677*/ { "ve" ,  456 },
    /*0678*/ { "vi" ,  457 },
    /*0679*/ { "vo" ,  458 },
    /*0680*/ { "wa" ,  466 },
    /*0681*/ { "wo" ,  467 },
    /*0682*/ { "xh" ,  469 },
    /*0683*/ { "yi" ,  472 },
    /*0684*/ { "yo" ,  473 },
    /*0685*/ { "za" ,  478 },
    /*0686*/ { "zh" ,   78 },
    /*0687*/ { "zu" ,  480 },
    { "", 0 }
};

static int lang_table_compare(const void *lhs, const void *rhs)
{
    return strcmp(lhs, ((const LangEntry *)rhs)->str);
}

const char *ff_convert_lang_to(const char *lang, enum AVLangCodespace target_codespace)
{
    int i;
    const LangEntry *entry = NULL;
    const int NB_CODESPACES = FF_ARRAY_ELEMS(lang_table_counts);

    if (target_codespace >= NB_CODESPACES)
        return NULL;

    for (i=0; !entry && i<NB_CODESPACES; i++)
        entry = bsearch(lang,
                        lang_table + lang_table_offsets[i],
                        lang_table_counts[i],
                        sizeof(LangEntry),
                        lang_table_compare);
    if (!entry)
        return NULL;

    for (i=0; i<NB_CODESPACES; i++)
        if (entry >= lang_table + lang_table_offsets[target_codespace] &&
            entry < lang_table + lang_table_offsets[target_codespace] + lang_table_counts[target_codespace])
            return entry->str;
        else
            entry = lang_table + entry->next_equivalent;

    if (target_codespace == AV_LANG_ISO639_2_TERM)
        return ff_convert_lang_to(lang, AV_LANG_ISO639_2_BIBL);

    return NULL;
}

#if LIBAVFORMAT_VERSION_MAJOR < 58
const char *av_convert_lang_to(const char *lang, enum AVLangCodespace target_codespace)
{
    return ff_convert_lang_to(lang, target_codespace);
}
#endif
