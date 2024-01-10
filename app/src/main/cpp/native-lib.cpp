#include <jni.h>
#include <string>
#include "utils/LogUtils.h"
#include <thread>

// 全局变量
JavaVM *g_vm = nullptr;


// 耗时任务
void backgroundTask(JNIEnv *env, jobject javaObject1) {

    LOGI(__func__)
    // 执行耗时的操作
    for (int i = 0; i < 10; ++i) {
        LOGI(" 执行耗时的操作 %d", i)
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // 更新 UI（如果需要）

    // 通知任务完成（可以使用回调或其他方式）
    jclass javaClass = env->GetObjectClass(javaObject1);
     jmethodID onTaskCompleteMethod = env->GetMethodID(javaClass, "onTaskComplete", "()V");
     env->CallVoidMethod(javaObject1, onTaskCompleteMethod);
}

jobject global_mainActivity = nullptr;

extern "C"
JNIEXPORT void JNICALL
Java_com_uvknc_subthreadwithjni_MainActivity_runTask(JNIEnv *env, jobject javaObject) {
    env->GetJavaVM(&g_vm);


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