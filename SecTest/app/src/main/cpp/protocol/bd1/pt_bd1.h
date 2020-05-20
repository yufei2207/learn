/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     pt_bd1.h
* Abstract： BD1 protocol define
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
#ifndef H_PT_BD1_H
#define H_PT_BD1_H

#include "../pt_define.h"

#ifdef PT_ENABLE_BD1
//////////////////////////////////////////////////////////////////////////
//interface define
//////////////////////////////////////////////////////////////////////////
#define BD1_ISTYPE(TYPE)   (TYPE >= BD1_ZERO && TYPE <= BD1_LAST)
extern  int BD1_DecodeHead(const uchar* buf, int len);
extern  int BD1_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType);

extern  FUNC_ENCODER BD1_GetEncoder(int nType);
extern  FUNC_DECODER BD1_GetDecoder(int nType);

extern  int BD1_RecvData(TStackParam *pParam, uchar data);
extern  int BD1_RecvDataEx(TStackParam *pParam, uchar data);
extern  int BD1_EncodePackage(uchar *lpBuff, int nBuffLen, void *pPackage, 
                              int nMsgType, char bDevice);
extern  int BD1_DecodePackage(void *pPackage, int nMsgType, uchar *lpBuff, int nBuffLen);

#ifdef WIN32
    extern void* BD1_GetPackage(int nType);
#endif

//////////////////////////////////////////////////////////////////////////
//data struct define
//////////////////////////////////////////////////////////////////////////
typedef struct TBD1GLJC
{
    int nUserAddr;  //user address
    char nFrequency; //output frequency, unit:minute
}TBD1GLJC;

typedef enum TBD1MessageType
{
    TBDMT_POSITION      = 0,
    TBDMT_SENDMESSAGE   = 2,
    TBDMT_QUERYMESSAGE  = 3,
}TBD1MessageType;

typedef enum THeightTestMode
{
    THM_HEIGHT = 0,
    THM_NOTEST,
    THM_TEST1,
    THM_TEST2
}THeightTestMode;

typedef struct TBD1DWSQ
{
    int nUserAddr;  //user address
    char bEmergent; //1：yes ,0:no
    char nHeightTestMode; //THeightTestMode
    char nHeightFlag; //0:normal, 1:top

    short nHeight;        //uint:m
    unsigned short nAntennaHeight; //uint:m

    int  nAirPressure; //uint:0.1HP
    short  nTemperature;//uint:0.1℃

    short nFrequency; //output frequency, unit:second
}TBD1DWSQ;

typedef struct TCommMessage
{
    char  nType; //0：emergent ,1:normal
    char  nMode; //0:chinese, 1:code, 2:code and chinese  mix ,3:position
    char  bPassword; //0:communication,1:password
    short nDataLen;  //uint:byte
    uchar szData[420]; 
    char  bAnswer;
}TCommMessage;

typedef struct TQueryMessage
{
    char  nType; //0：position ,1:communication
    char  nMode; //nType = 0: 0:once, 1:twice;2:thrice 
                 //nType = 1: 0:by last time, 2:by useraddress;3:query receipt 
}TQueryMessage;

typedef struct TBD1TXSQ
{
    int  nUserAddr;  //user address
    char nType;    //TBD1MessageType
    int  nPeerAddr;

    union
    {
        TCommMessage  msgComm;//communication message
        TQueryMessage msgQuery;//query message
    }data;
}TBD1TXSQ;

typedef struct TBD1ICJC
{
    int  nUserAddr;  //user address
    char nFrameID;    //
}TBD1ICJC;

typedef struct TBD1XTZJ
{
    int  nUserAddr;  //user address
    short nFrequency; //output frequency, unit:second
}TBD1XTZJ;

//用户机至外设
typedef struct TBD1GLZK
{
    int  nUserAddr;   //user address
    uchar PowerState[6];    //power 0...5
}TBD1GLZK;

typedef struct TPosData
{
    char nDegree; 
    char nMinute;
    char nSecond;
    char nDotSecond; //uint:0.1s
}TPosData;

typedef struct TBD1DWXX
{
    int  nUserAddr;   //user address
    char nType;      //0：position ,1:query
    char bSecretKey; //0：no ,1:yes
    char nPrecision; //0：level-1 ,1:level-2
    char bEmergent;  //1：yes ,0:no
    char bMultiValue; //0：no ,1:yes
    char nHeightFlag; //0:normal, 1:top

    int  nQueryAddr;   //query address

    char nHour;
    char nMinute;
    char nSecond;
    char nDotSecond;  //uint:0.01s
    
    TPosData latPos;
    TPosData lonPos;

    short nHeight;      //uint:m
    short nHeightError; //uint:m
}TBD1DWXX;

typedef struct TBD1TXXX
{
    int  nUserAddr;   //user address
    char nMode;       //0:chinese, 1:code, 2:mix ,3:position
    char nType;      //0：position ,1:query
    char bSecretKey; //0：no ,1:yes

    int  nPeerAddr;   //sender address

    char nHour;       //start hour
    char nMinute;     //start minute

    short nDataLen;   //uint:byte
    uchar szData[420];     //
    char  nCRCFlag;   //0x00: CRC success; 0x01:CRC error
}TBD1TXXX;

typedef struct TBD1HZTime
{
	uchar nHour;
	uchar nMinute;
}TBD1HZTime;
#define BD1_MAX_NUM_HZ		5
typedef struct TBD1TXHZ
{
    int  nUserAddr;   //user address

	int  nRecvAddr;   //receiver address
    char nCount;      //回执数

	TBD1HZTime SendTime[BD1_MAX_NUM_HZ];
	TBD1HZTime HZTime[BD1_MAX_NUM_HZ];
}TBD1TXHZ;

typedef struct TBD1ICXX
{
    int  nUserAddr;   //user address
    char nFrameID;    //
//first frame
    int  nBroadcastID;   //broadcast address
    char nUserType;    //user type
    short nFrequency;    //user frequency, uint:s
    char nLevel;       //
    char bSecret;      //0：no ,1:yes
    unsigned short nSubUserNum;
//other frame    
    int  SubUserAddr[100];
}TBD1ICXX;

typedef struct TBD1FKXX
{
    int  nUserAddr;   //user address

    uchar nResult;       //feedback type
    uchar szResult[4]; //detail info
	int   nFreq;
}TBD1FKXX;

typedef struct TBD1ZJXX
{
    int  nUserAddr;   //user address
    char nICState; //ic state
    char nHardwareState; //hardware state
    char nBatteryState;  //battery state
    char nInboundState; //inbound state
    char PowerState[6]; //power state
}TBD1ZJXX;

typedef struct TBD1NULL
{
	int  nUserAddr;   //user address
}TBD1NULL;

typedef TBD1NULL  TBD1XHDQ;

typedef struct TBD1XHXX
{
    int  nUserAddr;   //user address
	int nSerialNum;
}TBD1XHXX;

typedef struct TBD1GXZX
{
    int nUserAddr;   	//user address
	uchar MngInfo[32];  //manage info
}TBD1GXZX;

typedef TBD1NULL  TBD1GXDQ;

typedef TBD1GXZX TBD1GLXX;

typedef struct TBD1JSZL
{
    int nUserAddr;   //user address

	uchar nType; //0:cmd;1:all
	uchar szCMD[4];  //cmd 
}TBD1JSZL;

typedef struct TBD1BSSZ
{
    int nUserAddr;   //user address

	uchar Channel[6]; 
	uchar nBeamRes;  //
}TBD1BSSZ;

#endif  //PT_ENABLE_BD1

#endif//H_PT_BD1_H













