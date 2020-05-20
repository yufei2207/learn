#include <jni.h>
#include <string>
#include "add.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_sectest_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_sectest_MainActivity_addJNI(
        JNIEnv* env,
        jobject /* this */) {

    int he = add(1,2);
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
