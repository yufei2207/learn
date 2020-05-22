// Add.cpp : ���� DLL Ӧ�ó���ĵ���������
//

//#include "stdafx.h"

#include <stdio.h>
//#include <tchar.h>
#include <android/log.h>
#include "add.h"

//log定义
#define  LOG    "JNILOG" // 这个是自定义的LOG的TAG
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG,__VA_ARGS__) // 定义LOGD类型
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG,__VA_ARGS__) // 定义LOGI类型
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,LOG,__VA_ARGS__) // 定义LOGF类型

int add(int a, int b){
	printf("parameter1=%d\n", a);
	printf("parameter2=%d\n", b);
	printf("return=%d\n",a+b);

	return a + b;
}

int addpoint(int *a, int *b){
	printf("parameter1_point=%p\n", a);
	printf("parameter1=%d\n", *a);

	printf("parameter2_point=%p\n", b);
	printf("parameter2=%d\n", *b);
	printf("return=%d\n", *a + *b);
	return *a + *b;
}

int addSTR(STR_POINT a){
	return (a.x + a.y);
}

int addSTR_POINT(STR_POINT *a){
	printf("parameter_point=%p\n", a);

	printf("parameter.x=%d\n", a->x);
	printf("parameter.y=%d\n", a->y);
	printf("return=%d\n", a->x + a->y);

	return a->x + a->y;
}

