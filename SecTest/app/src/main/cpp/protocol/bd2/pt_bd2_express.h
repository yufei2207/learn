/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name£º     pt_bd2_express.h
* Abstract£º BD2 express protocol define
* 
* Version£º 1.0
* Author£º  yfwang
* Date£º    2012/03/19
*
* history   :
*	     when            who    	          what:
*      -------------   --------------    -------------------
*      Mar 19, 2012   yfwang                create
*
*/
#ifndef H_PT_BD2_EXPRESS_H
#define H_PT_BD2_EXPRESS_H  

#include "../pt_define.h"

#ifdef  PT_ENABLE_BD2_EXPRESS

//////////////////////////////////////////////////////////////////////////
//interface define
//////////////////////////////////////////////////////////////////////////

#define BD2_EX_ISTYPE(TYPE)   (TYPE >= BD2_EX_ZERO && TYPE <= BD2_EX_LAST)

extern  int BD2_EX_DecodeHead(const uchar* buf, int len);
extern  int BD2_EX_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType);

extern  FUNC_ENCODER BD2_EX_GetEncoder(int nType);
extern  FUNC_DECODER BD2_EX_GetDecoder(int nType);

#ifdef WIN32
    extern void *BD2_EX_GetPackage(int nType);
#endif  

//////////////////////////////////////////////////////////////////////////
//data struct define
//////////////////////////////////////////////////////////////////////////

typedef struct TBD2RMO
{
	char szCMD[4];//
	char nMode;//1:close;2:open;3:close all;4:open all
	char nFreq;//
}TBD2RMO;

typedef struct TBD2TXM
{
	uchar nCMD;//1:set;2:query;3:response
    uchar nMode;//0:auto;1:BD1;2:GPRS
}TBD2TXM;

typedef struct TBD2WGM
{
	uchar nCMD;//1:set;2:query;3:response

	int   nUserID;
	uchar szAddr[32];
    int   nPort;//
	uchar szDeviceID[16];
}TBD2WGM;

#endif// end PT_ENABLE_BD2_EXPRESS define

#endif//end  H_PT_BD2_EXPRESS_H  define