#include <jni.h>
#include <string>
#include "utils/LogUtils.h"
#include <thread>

#include <android/native_activity.h>

// 全局变量
JavaVM *g_vm = nullptr;

jobject global_mainActivity = nullptr;

void sweetSpace(JNIEnv *env, jobject thiz);


// 耗时任务
void backgroundTask(JNIEnv *env, jobject javaObject1) {
    LOGI(__func__)
    // 执行耗时的操作
    for (int i = 0; i < 3; ++i) {
        LOGI(" 执行耗时的操作 %d", i)
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

//    jclass mainActivity_jclass = env->GetObjectClass(javaObject1);
    jclass mainActivity_jclass = env->GetObjectClass(global_mainActivity);
    if (mainActivity_jclass == nullptr) {
        LOGE("  mainActivity jclass is NULL")
        return;
    }

    jmethodID enterSecond_jmethodID = env->GetMethodID(mainActivity_jclass, "enterSecond", "()V");
    env->CallVoidMethod(javaObject1, enterSecond_jmethodID);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_uvknc_subthreadwithjni_MainActivity_runTask(JNIEnv *env, jobject javaObject) {
    env->GetJavaVM(&g_vm);
    LOGI(__func__)
    global_mainActivity = env->NewGlobalRef(javaObject);
    // 在子线程中执行任务
    std::thread backgroundThread([env, javaObject]() {
        // 获取当前线程的 JNIEnv
        JNIEnv *threadEnv;
        if (g_vm->AttachCurrentThread(&threadEnv, nullptr) == JNI_OK) {

            // 在子线程中执行任务
            backgroundTask(threadEnv, global_mainActivity);
            // 释放当前线程的 JNIEnv
            g_vm->DetachCurrentThread();
        }
    });

    backgroundThread.detach();

}
extern "C"
JNIEXPORT void JNICALL
Java_com_uvknc_subthreadwithjni_MainActivity_enterB(JNIEnv *env, jobject javaObject1) {

    jclass newActivityClass = env->FindClass("com/uvknc/subthreadwithjni/SecondActivity");
    if (newActivityClass == nullptr) {
        LOGE("  newActivityClass is NULL")
        // 打印日志或进行其他错误处理
        return;
    }

    // 通知任务完成（可以使用回调或其他方式）
    jclass    javaClass            = env->GetObjectClass(javaObject1);
    jmethodID onTaskCompleteMethod = env->GetMethodID(javaClass, "onTaskComplete", "()V");
    env->CallVoidMethod(javaObject1, onTaskCompleteMethod);

    // 获取主线程的上下文
    jclass    mainActivityClass           = env->GetObjectClass(javaObject1);
    jmethodID getApplicationContextMethod = env->GetMethodID(mainActivityClass, "getApplicationContext",
                                                             "()Landroid/content/Context;");
    jobject   mainContextObj              = env->CallObjectMethod(javaObject1, getApplicationContextMethod);
    jobject   mainContext                 = env->NewGlobalRef(mainContextObj);

    // 获取 Intent 的类引用
    jclass intentClass = env->FindClass("android/content/Intent");

    // 创建 Intent
    jmethodID intentConstructor = env->GetMethodID(intentClass, "<init>", "(Landroid/content/Context;Ljava/lang/Class;)V");
    jobject   intent            = env->NewObject(intentClass, intentConstructor, mainContext, newActivityClass);

    // 启动 Activity
    jmethodID startActivityMethod = env->GetMethodID(mainActivityClass, "startActivity",
                                                     "(Landroid/content/Intent;)V");
    env->CallVoidMethod(javaObject1, startActivityMethod, intent);

    // 释放引用
    env->DeleteGlobalRef(mainContext);


}

static JavaVM *mCoreJavaVM = nullptr;
static JNIEnv *mJNIEnv     = nullptr;

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGD(__func__)
    mCoreJavaVM = vm;
    if (mCoreJavaVM->GetEnv((void **) &mJNIEnv, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }


    return JNI_VERSION_1_6;


}


void sweetSpace(JNIEnv *env, jobject thiz) {

    LOGD(__func__)

}



