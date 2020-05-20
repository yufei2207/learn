#ifndef __Add_H__
#define __Add_H__



typedef struct
{
	int x;
	int y;
}STR_POINT;

int* add_return_point(int a, int b);

int add(int a, int b);

int addpoint(int *a, int *b);

int addSTR(STR_POINT a);

int addSTR_POINT(STR_POINT *a);

#endif 
