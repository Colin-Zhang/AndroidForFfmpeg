/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <pthread.h>
#include <jni.h>
#include <android/log.h>

#include <malloc.h>
#include <libavutil/log.h>
#include "ffmpeg_thread.h"



#define LOG_TAG "FFmpeg"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,LOG_TAG,__VA_ARGS__)
#define LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)


// processing callback to handler class
typedef struct tick_context {
    JavaVM *javaVM;
    jclass jniHelperClz;
    jobject jniHelperObj;
    jclass mainActivityClz;
    jobject mainActivityObj;

} TickContext;
TickContext g_ctx;

void *threadExecuteInfoCallBack(void *context, jstring line) {
    TickContext *pctx = (TickContext *) context;
    JavaVM *javaVM = pctx->javaVM;
    JNIEnv *env;
    jint res = (*javaVM)->GetEnv(javaVM, (void **) &env, JNI_VERSION_1_6);
    if (res != JNI_OK) {
        res = (*javaVM)->AttachCurrentThread(javaVM, &env, NULL);
        if (JNI_OK != res) {

            return NULL;
        }
    }
    jmethodID statusId = (*env)->GetMethodID(env, pctx->jniHelperClz,
                                             "onProgressInfo",
                                             "(Ljava/lang/String;)V");

    (*env)->CallVoidMethod(env, pctx->mainActivityObj, statusId,(*env)->NewStringUTF(env,line));

    (*javaVM)->DetachCurrentThread(javaVM);
    return context;
}

void my_logcat(void *ptr, int level, const char *fmt, va_list vl) {
    va_list vl2;
    char line[1024];
    static int print_prefix = 1;
    va_copy(vl2, vl);
    av_log_format_line(ptr, level, fmt, vl2, line, sizeof(line), &print_prefix);
    va_end(vl2);
    switch (level) {
        case AV_LOG_VERBOSE:
            LOGV("%s", line);
            break;

        case AV_LOG_INFO:
            LOGI("Innnnn = %s", line);
            break;

        case AV_LOG_DEBUG:

            LOGE("Info ==== %s", line);
            break;

        case AV_LOG_FATAL:
            LOGF("Fatal == %s", line);
            break;

        case AV_LOG_WARNING:
            LOGW("Warning === %s", line);
            threadExecuteInfoCallBack(&g_ctx, line);
            break;

        case AV_LOG_TRACE:
        case AV_LOG_ERROR:
        default:
            threadExecuteInfoCallBack(&g_ctx, line);
            LOGD("default === %s", line);
    }
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, jobject jObjct) {

    JNIEnv *env;
    memset(&g_ctx, 0, sizeof(g_ctx));

    g_ctx.javaVM = vm;
    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR; // JNI version not supported.
    }

    jclass clz = (*env)->FindClass(env,
                                   "com/sefon/zhang/androidforffmpeg/FFmpegUtils");
    g_ctx.jniHelperClz = (*env)->NewGlobalRef(env, clz);

    jmethodID jniHelperCtor = (*env)->GetMethodID(env, g_ctx.jniHelperClz,
                                                  "<init>", "()V");
    jobject handler = (*env)->NewObject(env, g_ctx.jniHelperClz,
                                        jniHelperCtor);
    g_ctx.jniHelperObj = (*env)->NewGlobalRef(env, handler);

    g_ctx.mainActivityObj = NULL;
    return JNI_VERSION_1_6;
}




/*
 * Interface to Java side to start ticks, caller is from onResume()
 */
JNIEXPORT void JNICALL
Java_com_sefon_zhang_androidforffmpeg_FFmpegUtils_execute(JNIEnv *env, jobject instance,
                                                          jint cmdnum, jobjectArray commands) {
    pthread_attr_t threadAttr_;
    pthread_attr_init(&threadAttr_);
    pthread_attr_setdetachstate(&threadAttr_, PTHREAD_CREATE_DETACHED);
    av_log_set_callback(my_logcat);
    jclass clz = (*env)->GetObjectClass(env, instance);
    g_ctx.mainActivityClz = (*env)->NewGlobalRef(env, clz);
    g_ctx.mainActivityObj = (*env)->NewGlobalRef(env, instance);
    int i = 0;
    char **argv = NULL;//命令集 二维指针
    jstring *strr = NULL;
    if (commands != NULL) {
        argv = (char **) malloc(sizeof(char *) * cmdnum);
        strr = (jstring *) malloc(sizeof(jstring) * cmdnum);
        for (i = 0; i < cmdnum; ++i) {//转换
            strr[i] = (jstring) (*env)->GetObjectArrayElement(env, commands, i);
            argv[i] = (char *) (*env)->GetStringUTFChars(env, strr[i], 0);
        }
    }
    run_cmd_thread(cmdnum, argv, &g_ctx);
}

