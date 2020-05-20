/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     pt_utility.h
* Abstract： protocol utility
* 
* Version： 1.0
* Author：  yfwang
* Date：    2012/03/19
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
*函数名称：PT_MakeCRC
*函数说明：计算校验和
*参数说明：buf 数据缓存
*          len 数据长度
*返回值：  检验和
*/
extern uchar PT_MakeCRC(uchar* buf, int len);

/*
*函数名称：PT_Decode_INT32
*函数说明：解码32位数据
*参数说明：pVal 解码数据值
*					 buf 数据缓存
*          len 数据长度
*返回值：  -1：数据缓存不足，其他：解码字节数
*/
extern int PT_Decode_INT32(int *pVal, uchar *lpBuff, int nLen);
/*
*函数名称：PT_Encode_INT32
*函数说明：编码32位数据
*参数说明：
*					 buf 数据缓存
*          len 数据长度
*          nVal 编码数据值
*返回值：  -1：数据缓存不足，其他：解码字节数
*/
extern int PT_Encode_INT32(uchar *lpBuff, int nLen, int nVal);
/*
*函数名称：PT_Decode_INT24
*函数说明：解码24位数据
*参数说明：pVal 解码数据值
*					 buf 数据缓存
*          len 数据长度
*返回值：  -1：数据缓存不足，其他：解码字节数
*/
extern int PT_Decode_INT24(int *pVal, uchar *lpBuff, int nLen);

/*
*函数名称：PT_Encode_INT24
*函数说明：编码24位数据
*参数说明：
*		   buf 数据缓存
*          len 数据长度
*          nVal 编码数据值
*返回值：  -1：数据缓存不足，其他：解码字节数
*/
extern int PT_Encode_INT24(uchar *lpBuff, int nLen, int nVal);
/*
*函数名称：PT_Decode_SHORT
*函数说明：解码16位数据
*参数说明：pVal 解码数据值
*					 buf 数据缓存
*          len 数据长度
*返回值：  -1：数据缓存不足，其他：解码字节数
*/
extern int PT_Decode_SHORT(short *pVal, uchar *lpBuff, int nLen);
/*
*函数名称：PT_Encode_SHORT
*函数说明：编码16位数据
*参数说明：
*					 buf 数据缓存
*          len 数据长度
*          nVal 编码数据值
*返回值：  -1：数据缓存不足，其他：解码字节数
*/
extern int PT_Encode_SHORT(uchar *lpBuff, int nLen, short nVal);
/*
*函数名称：PT_DecodeString
*函数说明：解码字符串
*参数说明：
*		   pDest    目标数据缓存
*          nDestLen 目标数据缓存长度
*          pSrc     源数据缓存 有‘\0’结束符
*返回值：  无
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











