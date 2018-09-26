#include "ffmpeg_cmd_utils.h"
#include <string.h>
#include <jni.h>
#include <android/log.h>

#include "ffmpeg_thread.h"

static JavaVM *jvm = NULL;
//java虚拟机
static jclass m_clazz = NULL;//当前类(面向java)
#define LOG_TAG "System.out.c"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)


/**
 * 回调执行Java方法
 * 参看 Jni反射+Java反射
 */
void callJavaMethod(JNIEnv *env, jclass clazz,int ret) {
    if (clazz == NULL) {
        LOGD("---------------classID isNULL---------------");
        return;
    }
    //获取方法ID (I)V指的是方法签名 通过javap -s -public FFmpegCmd 命令生成
    LOGI("---------------class == ---------------");
    jmethodID methodID = (*env)->GetMethodID(env, clazz, "onExecuted", "(I)V");
    LOGI("---------------获取成功---------------");
    if (methodID == NULL) {
        LOGI("---------------methodID isNULL---------------");
        return;
    }
    LOGI("---------------调用成功---------------");
    //调用该java方法
    (*env)->CallStaticVoidMethod(env, clazz, methodID,ret);

}


/**
 * c语言-线程回调
 */
static void ffmpeg_callback(int ret) {
    LOGI("---------------调用3---------------");
    JNIEnv *env;
    //附加到当前线程从JVM中取出JNIEnv, C/C++从子线程中直接回到Java里的方法时  必须经过这个步骤
    (*jvm)->AttachCurrentThread(jvm, (JNIEnv **) (void **) &env, NULL);
    callJavaMethod(env, m_clazz,ret);
    LOGI("---------------找到回调---------------");
    //完毕-脱离当前线程
    (*jvm)->DetachCurrentThread(jvm);
}

/*
 * Class:     com_toshiba_ffmpeg_FFmpegCmd
 * Method:    exec
 * Signature: (I[Ljava/lang/String;)I
 */
JNIEXPORT jint

JNICALL Java_com_sefon_zhang_androidforffmpeg_FFmpegCmd_exec
        (JNIEnv *env, jobject clazz, jint cmdnum, jobjectArray cmdline) {
    //---------------------------------C语言 反射Java 相关----------------------------------------
    //在jni的c线程中不允许使用共用的env环境变量 但JavaVM在整个jvm中是共用的 可通过保存JavaVM指针,到时候再通过JavaVM指针取出JNIEnv *env;
    //ICS之前(你可把NDK sdk版本改成低于11) 可以直接写m_clazz = clazz;直接赋值,  然而ICS(sdk11) 后便改变了这一机制,在线程中回调java时 不能直接共用变量 必须使用NewGlobalRef创建全局对象
    //官方文档正在拼命的解释这一原因,参看:http://android-developers.blogspot.jp/2011/11/jni-local-reference-changes-in-ics.html
//    (*env)->GetJavaVM(env, &jvm);
    m_clazz = (*env)->GetObjectClass(env, clazz);
    //---------------------------------C语言 反射Java 相关----------------------------------------


    //---------------------------------java 数组转C语言数组----------------------------------------

    int i = 0;//满足NDK所需的C99标准
    char **argv = NULL;//命令集 二维指针
    jstring *strr = NULL;

    if (cmdline != NULL) {
        argv = (char **) malloc(sizeof(char *) * cmdnum);
        strr = (jstring *) malloc(sizeof(jstring) * cmdnum);

        for (i = 0; i < cmdnum; ++i) {//转换
            strr[i] = (jstring)(*env)->GetObjectArrayElement(env, cmdline, i);
            argv[i] = (char *) (*env)->GetStringUTFChars(env, strr[i], 0);
        }
    }
    //---------------------------------java 数组转C语言数组----------------------------------------
    //---------------------------------执行FFmpeg命令相关----------------------------------------
    //新建线程 执行ffmpeg 命令
    LOGI("---------------调用1---------------");
    ffmpeg_thread_run_cmd(cmdnum, argv);
    //注册ffmpeg命令执行完毕时的回调
    LOGI("---------------调用2---------------");
        ffmpeg_thread_callback(ffmpeg_callback);
    //---------------------------------执行FFmpeg命令相关----------------------------------------

    free(strr);
    return 0;
}
