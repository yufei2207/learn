#ifndef __Add_H__
#define __Add_H__



typedef struct
{
	int x;
	int y;
}STR_POINT;

extern "C" int* add_return_point(int a, int b);

extern "C" int add(int a, int b);

extern "C" int addpoint(int *a, int *b);

extern "C" int addSTR(STR_POINT a);

extern "C" int addSTR_POINT(STR_POINT *a);

#endif 
