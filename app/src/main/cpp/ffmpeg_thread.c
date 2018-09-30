//
// Created by zhang on 2018/9/29.
//

#include <termios.h>
#include <jni.h>
#include "ffmpeg.h"
#include "ffmpeg_thread.h"

pthread_t ptid;
char **args = NULL;
int num = 0;
void *mcontext;
typedef struct tick_context {
    JavaVM *javaVM;
    jclass jniHelperClz;
    jobject jniHelperObj;
    jclass mainActivityClz;
    jobject mainActivityObj;

} TickContext;

static void (*ffmpeg_callback)(int ret);

void *thread(void *arg) {
    int result = execute(num, args);

    return ((void *) 0);
}

void *threadCallBack(void *context) {
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
                                             "onProgressCallBack",
                                             "(I)V");
    (*env)->CallVoidMethod(env, pctx->mainActivityObj, statusId);

    (*javaVM)->DetachCurrentThread(javaVM);
    return context;
}

int run_cmd_thread(int cmdnum, char **argv, void *context) {
    mcontext = context;
    num = cmdnum;
    args = argv;
    int temp = pthread_create(&ptid, NULL, thread, NULL);

    if (temp == 0)
        return 1;
    return 0;
}


void thread_exit(int ret) {
    if (ffmpeg_callback) {
        ffmpeg_callback(ret);
    }
    threadCallBack(mcontext);
    pthread_exit("thread_exit");
}