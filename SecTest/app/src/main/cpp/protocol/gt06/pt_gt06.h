/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     pt_gt06.h
* Abstract： GT06 protocol 
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
#ifndef H_PT_GT06_H
#define H_PT_GT06_H

#include "../pt_define.h"

#ifdef  PT_ENABLE_GT06

//////////////////////////////////////////////////////////////////////////
//interface define
//////////////////////////////////////////////////////////////////////////

#define GT06_ISTYPE(TYPE)   (TYPE >= GT06_ZERO && TYPE <= GT06_LAST)
extern unsigned short GT06_MakeCRC(uchar *buf, int nLen);
extern  int GT06_DecodeHead(const uchar* buf, int len);
extern  int GT06_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType);

extern  FUNC_ENCODER GT06_GetEncoder(int nType);
extern  FUNC_DECODER GT06_GetDecoder(int nType);

extern  int GT06_RecvData(TStackParam *pParam, uchar data);
extern  int GT06_RecvDataEx(TStackParam *pParam, uchar data);
extern  int GT06_EncodePackage(uchar *lpBuff, int nBuffLen, void *pPackage, 
                               int nMsgType, char bDevice);
extern  int GT06_DecodePackage(void *pPackage, int nMsgType, uchar *lpBuff, int nBuffLen);

#ifdef WIN32
    extern void* GT06_GetPackage(int nType);
#endif
//////////////////////////////////////////////////////////////////////////
//data struct define
//////////////////////////////////////////////////////////////////////////
typedef struct TGTCOMMONRES
{
    unsigned short nSeqNum; //sequence number
} TGTCOMMONRES;

typedef struct TGTLOGIN
{
    uchar szDeviceID[16];
    unsigned short nSeqNum;//sequence number
} TGTLOGIN;

typedef TGTCOMMONRES  TGTLOGINRES;

typedef struct TGTPOSITON
{
    int    nYear;
    uchar  nMonth;
    uchar  nDay;
    uchar  nHour;
    uchar  nMinute;
    uchar  nSecond;

    uchar  nSatelliteNum;       //position Satellite number

    char   bNorthLat;   //1：north lat. ,0:sourth lat 
    double dblLat;//uint:degree
    char   bWestLon;  //1：west long. ,0:east long.  
    double dblLon;//uint:degree

    double dblSpeed;	//

    short  nCourse;	//course

    char   bGPSPosition; //1：GPS position ,0:no
    char   bGPSDifferent;  //1：GPS different position,0:no

    unsigned short nSeqNum;//sequence number
} TGTPOSITON;

typedef struct TGTSTATEREQ
{
    char bOil       ;  //是否油电接通
    char bGPSPos    ;  //是否GPS已定位
    char nWarnFlag  ;  //报警标志
    char bCharge    ;  //是否充电
    char bACC       ;  //是否ACC高
    char bProtected;   //是否设防

    char nPowerLevel;     //电压等级
    char nGSMSignal;      //GSM信号强度

    unsigned short nSeqNum;//sequence number
} TGTSTATEREQ;

typedef TGTCOMMONRES  TGTSTATERES;


typedef struct TGTCMDREQ
{
    uchar nCMDLen;      //command length
    uchar szCMD[256]; //command content
    unsigned int nServerFlag; //server flag 
    unsigned short nSeqNum; //sequence number
} TGTCMDREQ;

typedef TGTCMDREQ TGTCMDRES;

typedef struct TGTPHOTO
{
    uchar nDataLen;   //photo size
    uchar data[248]; //photo data
    unsigned bFinished;//
    unsigned short nSeqNum; //sequence number
} TGTPHOTO;

#endif// end PT_ENABLE_GT06

#endif//end  H_PT_GT06_H













