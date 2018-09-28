#include <jni.h>
#include <string>
#include <android/log.h>



extern "C" {


#define LOG_TAG "FFmpeg"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,LOG_TAG,__VA_ARGS__)
#define LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)


static JavaVM *jvm = NULL;//java虚拟机
static jclass m_clazz = NULL;//当前类(面向java)
static JNIEnv *m_env = NULL;
#include "ffmpeg.h"
#include "android/native_window_jni.h"
static char amixStr[10] = "amix";
static char timeStr[10] = "time=";
static char amixing = 0;
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
            LOGI("%s", line);
            break;

        case AV_LOG_DEBUG:
            LOGD("TEST ==== %s", line);
            break;

        case AV_LOG_FATAL:
            LOGF("%s", line);
            break;

        case AV_LOG_WARNING:
            LOGW("%s", line);
            break;

        case AV_LOG_TRACE:
        case AV_LOG_ERROR:
        default:
            LOGE("%s", line);
    }
}


JNIEXPORT jint JNICALL
Java_com_sefon_zhang_androidforffmpeg_FFmpegCmd_execute(JNIEnv *env,jobject clazz, jobjectArray commands) {
    av_log_set_callback(my_logcat);
    env->GetJavaVM(&jvm);
    m_env = env;
    m_clazz = static_cast<jclass>(env->NewGlobalRef(clazz));
    int argc = env->GetArrayLength(commands);
    char *argv[argc];
    int i;
    for (i = 0; i < argc; i++) {
        jstring js = (jstring) env->GetObjectArrayElement(commands, i);
        argv[i] = (char *) env->GetStringUTFChars(js, 0);
    }
    return execute(argc, argv);
}
    void callProGressMethod(char *ret) {
        char *p = strstr(ret,amixStr);
        if (p!=NULL) {
            amixing = 1 ;
        }
        int ss=0;

        if(amixing == 1){
            char *q = strstr(ret, timeStr);
            if(q != NULL){
                //LOGE("遇到time=");
                char str[14] = {0};
                strncpy(str, q, 13);
                int h =(str[5]-'0')*10+(str[6]-'0');
                int m =(str[8]-'0')*10+(str[9]-'0');
                int s =(str[11]-'0')*10+(str[12]-'0');
                ss = s+m*60+h*60*60;
            }else{
                return;
            }
        }else{
            return;
        }
        if (m_clazz == NULL) {
            LOGE("---------------clazz isNULL---------------");
            return;
        }
        //获取方法ID (I)V指的是方法签名 通过javap -s -public FFmpegCmd 命令生成
        jmethodID methodID = (m_env)->GetStaticMethodID(m_env, m_clazz, "onProgress", "(I)V");
        if (methodID == NULL) {
            LOGE("---------------methodID isNULL---------------");
            return;
        }
        //调用该java方法
        (*m_env)->CallStaticVoidMethod(m_env, m_clazz, methodID,ss);


    }
}
