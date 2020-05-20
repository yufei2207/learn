/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     pt_bd2_rdss.h
* Abstract： BD2 RDSS protocol define 
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
#ifndef H_PT_BD2_RDSS_H
#define H_PT_BD2_RDSS_H  

#include "../pt_define.h"

#ifdef  PT_ENABLE_BD2_RD

//////////////////////////////////////////////////////////////////////////
//interface define
//////////////////////////////////////////////////////////////////////////

#define BD2_RD_ISTYPE(TYPE)   (TYPE >= BD2_RD_ZERO && TYPE <= BD2_RD_LAST)

extern  int BD2_RD_DecodeHead(const uchar* buf, int len);
extern  int BD2_RD_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType);

extern  FUNC_ENCODER BD2_RD_GetEncoder(int nType);
extern  FUNC_DECODER BD2_RD_GetDecoder(int nType);

#ifdef WIN32
    extern void* BD2_RD_GetPackage(int nType);
#endif

//////////////////////////////////////////////////////////////////////////
//data struct define
//////////////////////////////////////////////////////////////////////////
typedef struct TRDBSI
{
	//响应波束号
	uchar nInboudBeam;
	//时差波束号
	uchar nTmDifBeam;
	//波束信号功率
	uchar nPower[10];
}TRDBSI;

//////////////////////////////////////////////////////////////////////////
//结构体说明：BSS,2代RDSS输入协议，设置响应波束和时差波束
//////////////////////////////////////////////////////////////////////////

typedef struct TRDBSS
{
	//响应波束（1-10）
	uchar nInboudBeam;
	//时差波束（1-10）
	uchar nTmDifBeam;
}TRDBSS;
//////////////////////////////////////////////////////////////////////////
//结构体说明：CXA,2代RDSS输入协议，查询本机通信信息
//////////////////////////////////////////////////////////////////////////

typedef struct TRDCXA
{
	//查询类别（0:定位查询,1:通信查询）
	uchar nQueryType;
	//查询方式（1-3）
	uchar nQueryMode;
	//用户地址（ID号）
	int nUserID;
}TRDCXA;
//////////////////////////////////////////////////////////////////////////
//结构体说明：DSA,2代RDSS输入协议，发送定时申请
//////////////////////////////////////////////////////////////////////////

typedef struct TRDDSA
{
	//用户地址（ID号）
	int nUserID;
	//定时方式（1/2）
	uchar nTimingMode;
	//有无位置信息指示（A/V）
	uchar cLocaFlag;
	//纬度llll.ll
	double db_Lat;
	//经度yyyyy.yy
	double db_Lon;
	//申请频度（秒；0为单次申请）
	float f_Freq;
	//单项零值
	float f_SingleZero;
	//附加零值（纳秒）
	float f_AddonZero;
}TRDDSA;
//////////////////////////////////////////////////////////////////////////
//结构体说明：DWA,2代RDSS输入协议，发送定位申请
//////////////////////////////////////////////////////////////////////////
typedef struct TRDDWA
{
	//用户地址（ID号）
	int nUserID;	
	//紧急定位（A:紧急定位,V:普通定位）
	uchar cUrgencyPos;
	//测高方式（0:有高程,1:无高程,2:测高1,3:测高2）
	uchar uc_MHMode;
	//高程指示（H:高空用户,L:普通用户）
	uchar cLocaType;
	//高程数据（米）
	double db_HeightData;
	//天线高（米）
	double db_AntHigh;
	//气压数据（帕）
	double db_AirPressure;
	//温度数据（度）
	double db_Temperature;
	//申请频度（秒,0:单次定位）
	double db_Freq;
}TRDDWA;
//////////////////////////////////////////////////////////////////////////
//结构体说明：DWR,2代RDSS输出协议，接收到的定位申请
//////////////////////////////////////////////////////////////////////////
typedef struct TRDDWR
{
	//定位信息类型（1-3）
	uchar nPosType;
	//用户地址（ID号）
	int nUserID;
	//定位时刻（UTC时间）
	uchar nHour;
	uchar nMinute;
	float f_Second;
	//纬度
	double db_Lat;
	//纬度方向（N/S）
	uchar cLatDir;
	//经度
	double db_Lon;
	//经度方向（E/W）
	uchar cLonDir;
	//大地高
	double db_Height;
	//大地高单位（M）
	uchar cHunit;
	//高程异常
	float f_Kesi;
	//高程异常单位（M）
	uchar cKesiUnit;
	//精度指示（0-1）
	uchar nPrecision;
	//紧急定位指示（A/V）
	uchar cUrgencyPos;
	//多值解指示（A/V）
	uchar cMutilpleValue;
	//高程类型指示（H/L）
	uchar cLocaType;
}TRDDWR;

//////////////////////////////////////////////////////////////////////////
//结构体说明：FKI,2代RDSS输出协议，用户设备输出的反馈信息
//////////////////////////////////////////////////////////////////////////
typedef struct TRDFKI
{
	//指令名称
	uchar cCmdName[4];
	//指令执行情况（Y/N）
	uchar cCmdExecFlag;
	//频度设置指示（Y/N）
	uchar cFreqSetFlag;
	//发射抑制指示（0-3）
	uchar nRestrainFlag;
	//等待时间
	uchar nWaitHour;
	uchar nWaitSecond;
}TRDFKI;

//////////////////////////////////////////////////////////////////////////
//结构体说明：GXM,2代RDSS双向协议，设置，读取返回用户设备管理
//								信息
//////////////////////////////////////////////////////////////////////////
typedef struct 
{
	//指令类型（1-3）
	uchar cCmdType;
	//管理信息
	uchar MngInfo[64];
}TRDGXM;

//////////////////////////////////////////////////////////////////////////
//结构体说明：ICA,2代RDSS输入协议，检测本用户设备加解密模块信息
//////////////////////////////////////////////////////////////////////////
typedef struct TRDICA
{
	//指令类型（0-1  0：本机  1：下属用户）
	uchar nCmdType;
	//下属用户信息帧号
	uchar nFrameNum;
}TRDICA;
//////////////////////////////////////////////////////////////////////////
//结构体说明：ICI,2代RDSS输出协议，输出本机加密模块信息
//////////////////////////////////////////////////////////////////////////
typedef struct TRDICI
{
	//用户地址（ID号）
	int nUserID;
	//序列号
	int nSerialNum;
	//通播地址
	int nBrdID;
	//用户特征值
	uchar nUserType;
	//服务频度（秒）
	float f_Freq;
	//通信等级（1-4）
	uchar nCommLevel;
	//加密标志（E/N）
	uchar cEncryptFlag;
	//下属用户数
	int nSubuserNum;
}TRDICI;

//////////////////////////////////////////////////////////////////////////
//结构体说明：ICZ,2代RDSS输出协议，指挥型用户设备输出下属用户
//								信息。除最后一条语句外，其余每条必须传满40个用户
//								信息。
//////////////////////////////////////////////////////////////////////////
typedef struct TRDICZ
{
	//总下属用户数
	int nSubuserNum;
	//下属用户ID号（一包协议最多有40个）
	int nSubuserID[40];
}TRDICZ;

//////////////////////////////////////////////////////////////////////////
//结构体说明：JMS,2代RDSS输入协议，设置用户设备实现无线电静默
//////////////////////////////////////////////////////////////////////////
typedef struct TRDJMS
{
	//无线电静默设置指示（E/N）
	uchar cJMFlag;
}TRDJMS;

//////////////////////////////////////////////////////////////////////////
//结构体说明：KLS,2代RDSS输入协议，发送口令识别指令
//////////////////////////////////////////////////////////////////////////
typedef struct TRDKLS
{
	//用户地址（ID号）
	int nUserID;
	//应答标志
	uchar cRequestFlag;
}TRDKLS;

//////////////////////////////////////////////////////////////////////////
//结构体说明：KLT,2代RDSS双向协议，指挥型用户设备输出下属用户发送
//								的口令识别内容，或者普通型用户设备响应指挥型用户设备
//								口令识别指令的信息
//////////////////////////////////////////////////////////////////////////
typedef struct TRDKLT
{
	//标识（P/Z P：普通型用户，Z：指挥型用户）
	uchar cUserType;
	//用户地址（ID号）
	int nUserID;
	//电文类型（0-2）
	uchar nInfoType;
	//电文内容
	int nInfolen;
	int nInfo[248];
}TRDKLT;

//////////////////////////////////////////////////////////////////////////
//结构体说明：LZM,2代RDSS双向协议，外设向用户设备设置零值或读取
//								设备零值申请，用户设备箱外设输出设备零值
//////////////////////////////////////////////////////////////////////////
typedef struct TRDLZM
{
	//管理模式（1-3）
	uchar nMode;
	//设备零值（纳秒）
	double dbZeroValue;
}TRDLZM;

//////////////////////////////////////////////////////////////////////////
//结构体说明：HZR,2代RDSS输出协议，用户设备进行通信回执查询后的
//								获得的回执信息
//////////////////////////////////////////////////////////////////////////
typedef struct TRDHZR_TIME
{
    int nHour;
    int nMinute;
}TRDHZR_TIME;

typedef struct TRDHZR
{
	//用户地址（ID号）
	int nUserID;
	//回执数（0-5 0：无回执）
	uchar nHZNum;
	//发信时间（时分）
    TRDHZR_TIME SendTime[5];
    //回执时间（时分）
    TRDHZR_TIME RespTime[5];
}TRDHZR;

//////////////////////////////////////////////////////////////////////////
//结构体说明：TXA,2代RDSS输入协议，用于设置用户设备发送通信申请
//////////////////////////////////////////////////////////////////////////
typedef struct TRDTXA
{
	//用户地址（ID号）
	int nUserID;
	//通信类别（0/1 0：特快 1：普通）
	uchar nCommMode;
	//传输方式（0-2 0：汉字 1：代码 2:混发 3:位置上报）
	uchar nInfoType;
	//通信电文内容
	int nInfolen;
	char Info[420];
}TRDTXA;

//////////////////////////////////////////////////////////////////////////
//结构体说明：TXR,2代RDSS输出协议，用户设备进行通信申请后获得的
//								通信信息
//////////////////////////////////////////////////////////////////////////
typedef struct TRDTXR
{
	//信息类别（1-5）
	uchar nMsgType;
	//用户地址（ID号）
	int nUserID;
    char szUserID[16];
	//电文形式（0：汉字  1：代码 2:混发 3:位置上报）
	uchar nInfoType;
	//发送时间
	uchar nSendHour;
	uchar nSendMin;
	//通信电文内容
	int nInfolen;
	uchar Info[420];
}TRDTXR;

//////////////////////////////////////////////////////////////////////////
//结构体说明：WAA,2代RDSS双向协议，用户设置用户设备发送位置报告
//								1申请，本语句不适用于指挥型用户设备监收的位置信息输出
//////////////////////////////////////////////////////////////////////////
typedef struct TRDWAA
{
	//信息类型（0-1  0：用户设备就收的位置报告  1：用户设备发送的位置报告）
	uchar nMsgType;
	//报告频度（秒  0：单次位置报告）
	float f_Freq;
	//用户地址（ID号）
	int nUserID;
	//位置报告时间
	uchar nReportHour;
	uchar nReportMin;
	float f_ReportSecond;
	//纬度
	float f_Lat;
	//纬度方向
	uchar cLatAspect;
	//经度
	float f_Lon;
	//经度方向
	uchar cLonAspect;
	//高程值
	float f_Height;
	//高程单位（米）
	uchar cHeightUnit;
}TRDWAA;

//////////////////////////////////////////////////////////////////////////
//结构体说明：WBA,2代RDSS输入协议，用户设置用户设备发送位置报告
//								2申请，该语句对应的输出语句为DWR
//////////////////////////////////////////////////////////////////////////
typedef struct TRDWBA
{
	//用户地址（ID号）
	int nUserID;
	//高程指示（H/L）
	uchar cHeightflag;
	//天线高（米）
	double f_AnteHeight;
	//报告频度（秒 0：单次位置报告）
	float f_Freq;
}TRDWBA;

//////////////////////////////////////////////////////////////////////////
//结构体说明：ZHS,2代RDSS输入协议，外设向用户设备发送自毁指令，
//								用于设置用户设备进行自毁
//////////////////////////////////////////////////////////////////////////
typedef struct TRDZHS
{
	//自毁指令
	uchar cSelfDestory[9];
}TRDZHS;

//////////////////////////////////////////////////////////////////////////
//结构体说明：RMO,2代RDSS输出协议，设定向己方输出或停止输出参数语句
//////////////////////////////////////////////////////////////////////////
typedef struct TRDRMO
{
	uchar uc_TagCommond[4];
	uchar uc_Mode;
	float f_Freq;
}TRDRMO;

#endif// end PT_ENABLE_BD2_RD define

#endif//end  H_PT_BD2_RDSS_H  define
