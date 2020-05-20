/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     pt_nw_define.h
* Abstract： nw defined protocol 
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
#ifndef H_PT_NW_DEFINE_H
#define H_PT_NW_DEFINE_H

#include "../pt_define.h"

#ifdef  PT_ENABLE_NW

//////////////////////////////////////////////////////////////////////////
//interface define
//////////////////////////////////////////////////////////////////////////

#define NW_ISTYPE(TYPE)   (TYPE >= NW_ZERO && TYPE <= NW_LAST)

extern  int NW_DecodeHead(const uchar* buf, int len);
extern  int NW_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType);

extern  FUNC_ENCODER NW_GetEncoder(int nType);
extern  FUNC_DECODER NW_GetDecoder(int nType);

#ifdef WIN32
    extern void* NW_GetPackage(int nType);
#endif
//////////////////////////////////////////////////////////////////////////
//data struct define
//////////////////////////////////////////////////////////////////////////
typedef struct TNWICS
{
	int nCardState1;
	int nCardState2;
	int nCardState3;
	int nCardState4;
	int nCardState5;
	int nCardState6;
	int nCardState7;
	int nCardState8;
	int nCardState9;
	int nCardState10;
} TNWICS;

//////////////////////////////////////////////////////////////////////////
//结构体说明：KJD,自定义握手协议
//////////////////////////////////////////////////////////////////////////
typedef struct TNWKJD
{
	uchar uc_Hello[6];
} TNWKJD;

//////////////////////////////////////////////////////////////////////////
//结构体说明：ICK,自定义双向协议，描述向外设发送所有卡的状态
//////////////////////////////////////////////////////////////////////////
typedef struct TNWICK
{
	uchar uc_CommandType;
	uchar uc_SoltNum;
	uchar uc_WorkState;
} TNWICK;

//////////////////////////////////////////////////////////////////////////
//结构体说明：ICQ,自定义输入协议，读取卡信息
//////////////////////////////////////////////////////////////////////////
typedef struct TNWICQ
{
	uchar uc_CommandType;
	float f_DeviceZero;
	uchar uc_SerialNum[10];
	uchar uc_ManageInfo[65];
} TNWICQ;

//////////////////////////////////////////////////////////////////////////
//结构体说明：ICH,自定义输入协议，读取卡信息
//////////////////////////////////////////////////////////////////////////
typedef struct TNWICH
{
	//用户地址（ID号）
	uchar uc_UserID[8];
	//序列号
	uchar uc_SerialNum[9];
	//通播地址
	uchar uc_BroadcastNum[8];
	//用户特征值
	uchar uc_Eigenvalue;
	//服务频度（秒）
	float f_ServiceFreq;
	//通信等级（1-4）
	uchar uc_CommLevel;
	//加密标志（E/N）
	uchar uc_EncryptMark;
	//下属用户数
	float f_UnderlingCount;
} TNWICH;
//追踪器自定义协议
//设备登录服务
typedef struct TNWLGN
{
    uchar    szDeviceID[16];
    unsigned short nSeqNum; //sequence number
} TNWLGN;
//位置上报
typedef struct TNWWZR
{
	uchar nYear;  
    uchar nMonth;   
    uchar nDay;  
	uchar nHour;  
	uchar nMinute;
	uchar nSecond;   

	uchar nDeviceID;	//0：车载设备;其他：手台

	char   chLat;  // 'N' or 'S'
	double dblLat;
	
	char   chLong;  // 'N' or 'S'
	double dblLong;

	double dblHeight; //高度
    double dblSpeed; //航速
    double dblCourse;//航向

	unsigned int   nFlag; //0:正常,1:特定事件
}TNWWZR;

//位置批量上报
typedef struct TNWWZI
{
    unsigned short nSeqNum; //sequence number
    unsigned int   nServerNum; //0:正常,1:位置查询
    unsigned char  nDataLen;
    unsigned char  szData[210];//位置数据
}TNWWZI;

//平台查询设备位置
typedef struct TNWWZQ
{
	unsigned int nServerNum; //Server number
}TNWWZQ;

//状态上报
typedef struct TNWSTA
{
	uchar nDataType	 ;  //数据类型 0:客户端；1:GPRS

	uchar nWarnFlag  ;  //报警标志
    uchar bGPSPos    ;  //是否GPS已定位
	uchar bOil       ;  //是否油电接通 
	uchar bACC       ;  //是否ACC高   
    uchar bCharge    ;  //是否充电
    uchar nPowerLevel;     //电压等级
    uchar nGSMSignal;      //GSM信号强度
   	uchar nGPRSState;      //GPRS状态	

    unsigned short nSeqNum; //sequence number
}TNWSTA;

typedef struct TNWTXI
{
	unsigned int nServerNum; //Server number
    
	uchar nHour;  
	uchar nMinute;
    
    unsigned char szUserID[16];
    unsigned char nDataLen;
    unsigned char szData[420];
}TNWTXI;

typedef struct TNWFKS
{
    unsigned short nSeqNum; //sequence number

    int nType;    //
    int nRet;     //0:success, other :fail

    uchar szData[128]; //detail content
}TNWFKS;

typedef struct TNWFKD
{
    unsigned int nServerNum; //server flag
    unsigned short nSeqNum; //sequence number

    int nType;    //
    int nRet;     //0:success, other :fail
    uchar szData[128]; //detail content
}TNWFKD;

//设置通信信息
typedef struct TNWTXS
{
	uchar nCMDType; //指令类型	  1:设置；2:查询；3：查询输出
	
    int   nGWUserID;           //网关用户机ID

	uchar szGWAddr[32];		   //网关地址
	int   nGWPort;			   //网关端口

	unsigned short nFreq; //上传频度（秒）
} TNWTXS;

//油电管理
typedef struct TNWYDM
{
	uchar nDataType;  //数据类型 0:客户端；1:GPRS

	uchar nCMDType; //指令类型
	uchar nAction; //操作类型 0:关闭;1:接通

    unsigned int nServerNum; //server flag
}TNWYDM;

#define MAX_NW_PHONE_LEN   12
//号码管理查询
typedef struct TNWHMM
{
	uchar nDataType;  //数据类型 0:客户端；1:GPRS

    unsigned int nServerNum; //server flag

	uchar nCMDType; //指令类型	1:增加，2:删除
    uchar szPhoneNumber[MAX_NW_PHONE_LEN]; 
}TNWHMM;

//号码管理查询
typedef struct TNWHMQ
{
	uchar nDataType	 ;  //数据类型 0:客户端；1:GPRS
    
    uchar nCMDType; //指令类型	1:查询，2:返回
    
    unsigned int nServerNum; //server flag
	unsigned short nSeqNum; //seq number

    #define MAX_NW_PHONE_NUM   3
    
    uchar nCount;    //max = 3
    uchar PhoneList[MAX_NW_PHONE_NUM][MAX_NW_PHONE_LEN];     //0:success, other:fail
}TNWHMQ;

//查询存储短信
typedef struct TNWDXQ
{
    uchar nCount;    //短信条数
}TNWDXQ;

//输出调试信息
typedef struct TNWDBG
{
    uchar nType;	//调试类型     
}TNWDBG;

//工作模式设置
typedef struct TNWMSS
{
    uchar nCMDType; //指令类型	  1:设置；2:查询；3：查询输出
    uchar nMode;	//工作模式    0：正常工作 1:支持北斗对讲机     
}TNWMSS;

#endif// end PT_ENABLE_NW

#endif//end  H_PT_NW_DEFINE_H




