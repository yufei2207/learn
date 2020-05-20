/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     pt_bd2_rnss.h
* Abstract： BD2 RNSS protocol define 
* 
* Version： 1.0
* Author：  yfwang
* Date：    2012/03/19
*
* history   :
*	     when            who    	          what:
*      -------------   --------------    -------------------
*      Mar 19, 2012      yfwang                create
*      Mar 27, 2012      xlsong                add
*/
#ifndef H_PT_BD2_RNSS_H
#define H_PT_BD2_RNSS_H  

#include "../pt_define.h"

#ifdef  PT_ENABLE_BD2_RN

//////////////////////////////////////////////////////////////////////////
//interface define
//////////////////////////////////////////////////////////////////////////
#define BD2_RN_ISTYPE(TYPE)   (TYPE >= BD2_RN_ZERO && TYPE <= BD2_RN_LAST)

extern  int BD2_RN_DecodeHead(const uchar* buf, int len);
extern  int BD2_RN_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType);

extern  FUNC_ENCODER BD2_RN_GetEncoder(int nType);
extern  FUNC_DECODER BD2_RN_GetDecoder(int nType);

#ifdef WIN32
    extern void* BD2_RN_GetPackage(int nType);
#endif

//////////////////////////////////////////////////////////////////////////
//data struct define
//////////////////////////////////////////////////////////////////////////

typedef struct TRNCOM
{
    int nBaudRate;//baud rate(4800,9600,38400,115200...)
    int nDataBits;//data bits(8,7)
    int nStopBits;//stop bits(0,1)
    int nParity;  //parity (0-None,1-odd,2-Even)   
}TRNCOM;

typedef struct TRNTIME
{
    uchar nHour;
    uchar nMinute;
    uchar nSecond;
    uchar nMilSecond;
}TRNTIME;

typedef struct TRNDATE

{
    uchar nYear;
    uchar nMonth;
    uchar nDay;
}TRNDATE;

typedef struct TRNDHV
{
    TRNTIME UtcTime;//定位时间
    double db_Speed;//速度
    double db_XSpeed;//X轴速度
    double db_YSpeed;//Y轴速度
    double db_ZSpeed;//Z轴速度
    double db_GroudSpeed;//地速
    double db_MaxSpeed;//最大速度
    double db_AverageSpeed;//平均速度
    double db_AllAvgSpeed;//全程平均速度
    double db_ValidSpeed;//有效速度
    uchar cSpeedUnit;//速度单位:K,km/h
}TRNDHV;

typedef struct TRNGBS
{
    TRNTIME UtcTime;//定位时间
    double db_LatError;//纬度的预计误差
    double db_LonError;//经度的预计误差
    double db_HeigError;//高度的预计误差
    int  nAbnorPRNSat;//最有可能的故障卫星PRN号
    float fAbnorSatLoss;//对最有可能的故障卫星漏检率
    float fAbmorSatDev;//对最可能的故障卫星估计的偏差
    float fEstDev;//偏置估算的标准偏差
}TRNGBS;

typedef struct TRNGGA
{
    TRNTIME UtcTime;//定位时间
    //纬度
    double db_Lat;
    //纬度方向
    uchar cLatDir;
    //经度
    double db_Lon;
    //经度方向
    uchar cLonDir;
    //状态指示
    uchar nPosStatus;
    //视野内的卫星数
    uchar nSatNum;
    //天线高
    double db_Antheight;
    //天线高单位
	uchar cAntHUnit;
	//高程异常
	float f_Kesi;
	//高程异常单位（M）
	uchar cKesiUnit;
	//差分数据龄期
    int DifDataAge;
    //查分站台ID号
    int DifID;
    //VDOP值
    float fVDOP;
}TRNGGA;

typedef struct TRNGLL
{
    //纬度
    double db_Lat;
    //纬度方向
    uchar cLatDir;
    //经度
    double db_Lon;
    //经度方向
    uchar cLonDir;
    //定位时间
    TRNTIME UtcTime;
    //定位有效位
    uchar PosFlag; 
    //模式指示(0:自动模式,1:查分模式,2:估算模式,3:手动输入模式,4:模拟器模式)
    int nPosMode;
}TRNGLL;

typedef struct TRNGLS

{
    //纬度
    double db_Lat;
    //纬度方向
    uchar cLatDir;
    //经度
    double db_Lon;
    //经度方向
    uchar cLonDir;
    //高程
    double db_Height;
    //高程单位
	uchar cHUnit;   
    uchar cPrecision;
    //当前UTC时间
    TRNDATE UtcDate;
	//定位时间
    TRNTIME UtcTime;
    //初始化类别(0:数据有效,1:清除星历,2:清除存储器)
    uchar InitType;
}TRNGLS;

typedef struct TRNGSA
{
    uchar nMode;//模式指示(M:手动,A:自动) 
    uchar nSelectMode;//选用模式,(1:表示定位不可用,2:2D,3:3D)
    uchar nPRNCode[12];//Pseudo Random Noise，伪随机噪声码(01-32)
    float fPDOP;//位置精度因子(0.5-99.9)
    float fHDOP;//水平精度因子(0.5-99.9)
    float fVDOP;//垂直精度因子(0.5-99.9)
    float fTDOP;
}TRNGSA;

typedef struct TRNGSV_SATINFO
{
    uchar nSatID;//卫星编号，01至32
    float fElevation;//卫星仰角，00至90度
    float fAzimuth;//方位角，000至359度。实际值
    float fCN;//信噪比（C/No），00至99dB；无表未接收到讯号
    
}TRNGSV_SATINFO;

typedef struct TRNGSV
{
    uchar nMsgNum;//总的GSV语句电文数
    uchar nCurMsgNum;//当前GSV 语句号
    uchar nSatNum;//可视卫星总数，00至12
    TRNGSV_SATINFO SatInfo[4];    
}TRNGSV;

typedef struct TRNICM
{
    TRNDATE BeginDate;
    TRNDATE StopDate;
}TRNICM;

typedef struct TRNIHI
{
    TRNTIME UtcTime;//UTC时间
    double db_NorthSpeed;//北方速度(m/s)
    double db_EastSpeed;//东方速度(m/s)
    double db_UpSpeed;//天方速度(m/s)
    double db_NorthAcc;//北方加速度(m2/s)
    double db_EastAcc;//东方加速度(m2/s)
    double db_UpAcc;//天方加速度(m2/s)
}TRNIHI;

typedef struct TRNIHO
{
    uchar nMsgNum;//the total number of the IHO message
    uchar nCurMsgNum;//current numble of the IHO message
    TRNTIME UtcTime;//UTC time 
    int nSatPRN;//sat PRN number
    double db_SatX;//Sat coordinate  
    double db_SatY;//Sat coordinate
    double db_SatZ;//Sat coordinate
    double db_SatSpeedX;
    double db_SatSpeedY;
    double db_SatSpeedZ;
    double db_PD;//phonily distance
    double db_PDV;//phonily distance velocity
    double db_PDOffect;//phonily distance  offect
    double db_PDVOffect;
    
}TRNIHO;

typedef struct TRNLPM
{
    uchar nMode;//模式指示(0:省电模式，1:正常模式)
}TRNLPM;

typedef struct TRNMSS
{
    uchar cWorkMode;//work mode
    uchar nSelectMode;//Position mode/test mode
    uchar cFreq[3];//frequency
    uchar cTrc[3];//trace

}TRNMSS;

typedef struct TRNPMU
{
    uchar nFreq;
    TRNTIME Deadline;
    
}TRNPMU;

typedef struct TRNRMC
{
    TRNTIME UtcTime;//hh
    uchar PosFlag;//定位有效位
    float fLat;//纬度
    uchar cLatDir;//纬度单位
    float fLon;//经度
    uchar cLonDir;//经度单位
    float fSpeed;//地面速度(000.0~359.9度)
    float fCourse;//航向
    TRNDATE UtcDate;//
    float fMVariation;// Magnetic Variation，磁偏角（000.0~180.0度，前导位数不足则补0）
    uchar cDeclination;//Declination，磁偏角方向，E（东）或W （西）
    uchar nPosMode;//模式指示(A:自主定位,D:差分,E:估算,N:,数据无效)
}TRNRMC;

typedef struct TRNTXT
{
    uchar nMsgNum;//the total number of the TXT message
    uchar nCurMsgNum;//current numble of the TXT message    
    uchar nTXTFlag;
    uchar TXTInfo[249]; 
}TRNTXT;

typedef struct TRNZBS
{
    uchar CoorID;//coordinate ID:user-defined/BJ54/WgS84/CGS2000
    double db_aLine;
    double db_Compression;//扁率
    double db_Dx;
    double db_Dy;
    double db_Dz;
}TRNZBS;

typedef struct TRNZDA
{
    uchar nPosMode;//Posotion mode :RDSS/RNSS
    TRNTIME UtcTime;//
    TRNDATE UtcDate;//date
    uchar nTimeZone;//Time Zone(0- ±13)
    uchar nMinuDif;//minute differece (00-59)
    TRNTIME ReviseTime;//定时修正值时刻
    double db_ReviseData;//修正值
    uchar nPrecission;//精度指示
    uchar cSatLockFlag;//卫星信号锁定状态 Y/N
}TRNZDA;

typedef struct TRNZTI
{
    uchar nPRMStatus;//PRM status 0:normal,1:abnormal
    uchar nEncModeStatus;//encrypt/decrypt module status 0:normal,1:abnormal
    uchar nAntStatus;//antenna status 0:normal,1:abnormal
    uchar nChlStatus;//Channel status 0:normal,1:abnormal
    uchar nPowerOrign;//current source 0:Battery,1:external power source
    uchar nChargeStatus;//charge status 0:charging 1:be not charging
    float fPowerPersent;//dump Power
    uchar fRemainHour;//remaining time
    uchar fRemainMinu;
   
}TRNZTI;



#endif// end PT_ENABLE_BD2_RN define

#endif//end  H_PT_BD2_RNSS_H  define
