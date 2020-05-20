/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name£º     pt_bd2_special.h
* Abstract£º BD2 special protocol define
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
#ifndef H_PT_BD2_SPECIAL_H
#define H_PT_BD2_SPECIAL_H  

#include "../pt_define.h"

#ifdef  PT_ENABLE_BD2_SPECIAL

//////////////////////////////////////////////////////////////////////////
//interface define
//////////////////////////////////////////////////////////////////////////
#define BD2_SP_ISTYPE(TYPE)   (TYPE >= BD2_SP_ZERO && TYPE <= BD2_SP_LAST)

extern  int BD2_SP_DecodeHead(const uchar* buf, int len);
extern  int BD2_SP_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType);

extern  FUNC_ENCODER BD2_SP_GetEncoder(int nType);
extern  FUNC_DECODER BD2_SP_GetDecoder(int nType);

extern  int BD2_SP_RecvData(TStackParam *pParam, uchar data);

extern  int BD2_SP_EncodePackage(uchar *lpBuff, int nBuffLen, void *pPackage, 
                                 int nMsgType, char bDevice);
extern  int BD2_SP_DecodePackage(void *pPackage, int nMsgType, uchar *lpBuff, int nBuffLen);

#ifdef WIN32
    extern void *BD2_SP_GetPackage(int nType);
#endif  

//////////////////////////////////////////////////////////////////////////
//data struct define
//////////////////////////////////////////////////////////////////////////





#endif// end PT_ENABLE_BD2_SPECIAL define

#endif//end  H_PT_BD2_SPECIAL_H  define