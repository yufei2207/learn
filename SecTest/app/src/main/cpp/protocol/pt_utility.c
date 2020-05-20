/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     pt_utility.c
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

#include "pt_define.h"

/*
*函数名称：PT_MakeCRC
*函数说明：计算校验和
*参数说明：buf 数据缓存
*          len 数据长度
*返回值：  检验和
*/
uchar PT_MakeCRC(uchar* buf, int len)
{
	uchar nCRC = buf[0];	//校验和
	int i = 0;		//循环次数

    for (i = 1; i < len; i++)
    {
        nCRC ^= buf[i];
    }

	return nCRC;	//返回校验和
}

int PT_Decode_INT32(int *pVal, uchar *lpBuff, int nLen)
{
    if (nLen < 4)
        return -1;

    *pVal = lpBuff[0];
    *pVal <<= 24;
    *pVal |= (lpBuff[1] << 16);
    *pVal |= (lpBuff[2] << 8);
    *pVal |= lpBuff[3];

    return 4;
}

int PT_Encode_INT32(uchar *lpBuff, int nLen, int nVal)
{
    if (nLen < 4)
        return -1;

    lpBuff[0] = (nVal >> 24) & 0xFF;
    lpBuff[1] = (nVal >> 16) & 0xFF;
    lpBuff[2] = (nVal >> 8) & 0xFF;
    lpBuff[3] = (nVal) & 0xFF;

    return 4;
}

int PT_Decode_INT24(int *pVal, uchar *lpBuff, int nLen)
{
    if (nLen < 3)
        return -1;

    *pVal = lpBuff[0];
    *pVal <<= 16;
    
    *pVal |= (lpBuff[1] << 8);
    *pVal |= lpBuff[2];

    return 3;
}

int PT_Encode_INT24(uchar *lpBuff, int nLen, int nVal)
{
    if (nLen < 3)
        return -1;

    lpBuff[0] = (nVal >> 16) & 0xFF;
    lpBuff[1] = (nVal >> 8) & 0xFF;
    lpBuff[2] = (nVal) & 0xFF;

    return 3;
}

int PT_Decode_SHORT(short *pVal, uchar *lpBuff, int nLen)
{
    if (nLen < 2)
        return -1;

    *pVal = lpBuff[0];
    *pVal <<= 8;

    *pVal |= lpBuff[1];

    return 2;
}

int PT_Encode_SHORT(uchar *lpBuff, int nLen, short nVal)
{
    if (nLen < 2)
        return -1;

    lpBuff[0] = (nVal >> 8) & 0xFF;
    lpBuff[1] = (nVal) & 0xFF;

    return 2;
}

/*
*
*
*char *pdest
*
*/
void PT_DecodeString(uchar *pDest, int nDestLen, uchar *pSrc)
{
    unsigned int nSrcLen = 0;
    
    nSrcLen = strlen(pSrc);
    if (nSrcLen > (nDestLen - 1))
    {
        nSrcLen = nDestLen - 1;
    }

    memcpy(pDest, pSrc, nSrcLen);
    pDest[nSrcLen] = '\0';
}


void PT_SplitString(TPackage *pPkg, char *pData, int nLen, char search)
{
	char *ptr = pData;
	pPkg->nItems = 0;

	while (ptr = strchr(pData, search))
	{
		*ptr = '\0';
		pPkg->pItem[pPkg->nItems++] = pData;

		pData = ptr + 1;			
		if ((pPkg->nItems + 1) == MAX_DATA_ITEM)
		{   
			break;
		}
	}

	pPkg->pItem[pPkg->nItems++] = pData;	
}

void PT_SplitStringEx(TPackage *pPkg, char *pData, int nLen, char *search)
{
	int i;
	char *ptr = pData;
	pPkg->nItems = 0;

	for (i = 0; i < nLen; i++, ptr++)
	{
		if (strchr(search, *ptr))
		{
			*ptr = '\0';
			pPkg->pItem[pPkg->nItems++] = pData;

			pData = ptr + 1;			
			if ((pPkg->nItems + 1) == MAX_DATA_ITEM)
			{
				break;
			}
		}
	}

	pPkg->pItem[pPkg->nItems++] = pData;	
}
void PT_SplitStringEx2(TPackage *pPkg, char *pData, int nLen, char *search,int nsearchcount)
{
	int i,searchtemp = 0;
	char *ptr = pData;
	pPkg->nItems = 0;

	if (nsearchcount >MAX_DATA_ITEM)
	{
		nsearchcount = MAX_DATA_ITEM;
	}
	for (i = 0; i < nLen; i++, ptr++)
	{
		if(searchtemp>=nsearchcount)
			break;
		if ((strchr(search, *ptr)))
		{
			*ptr = '\0';
			pPkg->pItem[pPkg->nItems++] = pData;

			pData = ptr + 1;			
			if ((pPkg->nItems + 1) == MAX_DATA_ITEM)
			{
				break;
			}
			searchtemp++;
		}
	}
	pPkg->pItem[pPkg->nItems++] = pData;

}
