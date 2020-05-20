// Add.cpp : ���� DLL Ӧ�ó���ĵ���������
//

//#include "stdafx.h"

#include <stdio.h>
//#include <tchar.h>

#include "add.h"


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

int* add_return_point(int a, int b){
	printf("parameter1=%d\n", a);
	printf("parameter2=%d\n", b);

	int* pHe = new int[1];
	*pHe = a + b;
	printf("return=%d\n", *pHe);

	printf("point=%p\n", pHe);

	return pHe;
}