//
// Created by zhang on 2018/9/26.
//

#ifndef ANDROIDFORFFMPEG_FFMPEG_CMD_UTILS_H
#define ANDROIDFORFFMPEG_FFMPEG_CMD_UTILS_H

#endif //ANDROIDFORFFMPEG_FFMPEG_CMD_UTILS_H
#include <string.h>
#include <jni.h>

#include <jni.h>
/* Header for class com_toshiba_ffmpeg_FFmpegCmd */

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_toshiba_ffmpeg_FFmpegCmd
 * Method:    exec
 * Signature: (I[Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_toshiba_ffmpeg_FFmpegCmd_exec
        (JNIEnv *, jclass, jint, jobjectArray);

#ifdef __cplusplus
}

#endif

