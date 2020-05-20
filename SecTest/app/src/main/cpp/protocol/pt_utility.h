/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name��     pt_utility.h
* Abstract�� protocol utility
* 
* Version�� 1.0
* Author��  yfwang
* Date��    2012/03/19
*
* history   :
*	     when            who    	          what:
*      -------------   --------------    -------------------
*      Mar 19, 2012   yfwang                create
*
*/
#ifndef H_PT_UTILITY_H
#define H_PT_UTILITY_H

#include "pt_define.h"

/*
*�������ƣ�PT_MakeCRC
*����˵��������У���
*����˵����buf ���ݻ���
*          len ���ݳ���
*����ֵ��  �����
*/
extern uchar PT_MakeCRC(uchar* buf, int len);

/*
*�������ƣ�PT_Decode_INT32
*����˵��������32λ����
*����˵����pVal ��������ֵ
*					 buf ���ݻ���
*          len ���ݳ���
*����ֵ��  -1�����ݻ��治�㣬�����������ֽ���
*/
extern int PT_Decode_INT32(int *pVal, uchar *lpBuff, int nLen);
/*
*�������ƣ�PT_Encode_INT32
*����˵��������32λ����
*����˵����
*					 buf ���ݻ���
*          len ���ݳ���
*          nVal ��������ֵ
*����ֵ��  -1�����ݻ��治�㣬�����������ֽ���
*/
extern int PT_Encode_INT32(uchar *lpBuff, int nLen, int nVal);
/*
*�������ƣ�PT_Decode_INT24
*����˵��������24λ����
*����˵����pVal ��������ֵ
*					 buf ���ݻ���
*          len ���ݳ���
*����ֵ��  -1�����ݻ��治�㣬�����������ֽ���
*/
extern int PT_Decode_INT24(int *pVal, uchar *lpBuff, int nLen);

/*
*�������ƣ�PT_Encode_INT24
*����˵��������24λ����
*����˵����
*		   buf ���ݻ���
*          len ���ݳ���
*          nVal ��������ֵ
*����ֵ��  -1�����ݻ��治�㣬�����������ֽ���
*/
extern int PT_Encode_INT24(uchar *lpBuff, int nLen, int nVal);
/*
*�������ƣ�PT_Decode_SHORT
*����˵��������16λ����
*����˵����pVal ��������ֵ
*					 buf ���ݻ���
*          len ���ݳ���
*����ֵ��  -1�����ݻ��治�㣬�����������ֽ���
*/
extern int PT_Decode_SHORT(short *pVal, uchar *lpBuff, int nLen);
/*
*�������ƣ�PT_Encode_SHORT
*����˵��������16λ����
*����˵����
*					 buf ���ݻ���
*          len ���ݳ���
*          nVal ��������ֵ
*����ֵ��  -1�����ݻ��治�㣬�����������ֽ���
*/
extern int PT_Encode_SHORT(uchar *lpBuff, int nLen, short nVal);
/*
*�������ƣ�PT_DecodeString
*����˵���������ַ���
*����˵����
*		   pDest    Ŀ�����ݻ���
*          nDestLen Ŀ�����ݻ��泤��
*          pSrc     Դ���ݻ��� �С�\0��������
*����ֵ��  ��
*/
void PT_DecodeString(uchar *pDest, int nDestLen, uchar *pSrc);

#define MAX_DATA_ITEM   23
typedef struct TPackage
{
	char *pItem[MAX_DATA_ITEM];
	char  nItems;
}TPackage;

void PT_SplitString(TPackage *pPkg, char *pData, int nLen, char search);

void PT_SplitStringEx(TPackage *pPkg, char *pData, int nLen, char *search);
void PT_SplitStringEx2(TPackage *pPkg, char *pData, int nLen, char *search,int nsearchcount);
#endif//H_PT_UTILITY_H











