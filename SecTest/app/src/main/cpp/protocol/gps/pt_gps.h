/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     pt_gps.h
* Abstract： gps protocol define
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
#ifndef H_PT_GPS_H
#define H_PT_GPS_H  

#include "../pt_define.h"

#ifdef  PT_ENABLE_GPS

//////////////////////////////////////////////////////////////////////////
//interface define
//////////////////////////////////////////////////////////////////////////

#define GPS_ISTYPE(TYPE)   (TYPE >= GPS_ZERO && TYPE <= GPS_LAST)

extern  int GPS_DecodeHead(const uchar* buf, int len);
extern  int GPS_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType);

extern  FUNC_ENCODER GPS_GetEncoder(int nType);
extern  FUNC_DECODER GPS_GetDecoder(int nType);


#ifdef WIN32
    extern void* GPS_GetPackage(int nType);
#endif  

//////////////////////////////////////////////////////////////////////////
//data struct define
//////////////////////////////////////////////////////////////////////////

typedef struct TGPSGGA
{
    uchar nHour;//时 hh
    uchar nMinute;//分 mm
    float fSecond;//秒ss.ss
    float fLat;//纬度
    uchar cLatDir;//纬度单位
    float fLon;//经度
    uchar cLonDir;//经度单位
    uchar cPosFlag;//定位有效标志(0:valid,1:invalid)
    uchar nSatNum;//视野内的卫星数
    float fPrecision;//水平精度(0.5~99.9)
    double dbAnteAltitude;//天线相对海平面高度(-9999.9~9999.9m)
    uchar cAltitudeunit;//单位
    double dbSphereAlti;//大地椭球面相对海平面高度(-9999.9~9999.9m)
    uchar cSphereunit;//单位
    int   nDifDataTerm;//查分数据龄期
    int   DifRefID;//差分站台ID号
    
}TGPSGGA;

typedef struct TGPSGSA
{
    uchar nMode2;//模式2 
    uchar nMode1;//模式1
    uchar nPRNCode[12];//Pseudo Random Noise，伪随机噪声码(01-32)
    float fPDOP;//位置精度因子(0.5-99.9)
    float fHDOP;//水平精度因子(0.5-99.9)
    float fVDOP;//垂直精度因子(0.5-99.9)
}TGPSGSA;

typedef struct TGPSGSV_SATINFO
{
    uchar nSatID;//卫星编号，01至32
    float fElevation;//卫星仰角，00至90度
    float fAzimuth;//方位角，000至359度。实际值
    float fCN;//信噪比（C/No），00至99dB；无表未接收到讯号
    
}TGPSGSV_SATINFO;

typedef struct TGPSGSV
{
    uchar nMsgNum;//总的GSV语句电文数
    uchar nCurMsgNum;//当前GSV 语句号
    uchar nSatNum;//可视卫星总数，00至12
    TGPSGSV_SATINFO SatInfo[4];    
}TGPSGSV;

typedef struct TGPSRMC
{
    uchar nUtcHour;//hh
    uchar nUtcMinute;//mm
    float fSecond;//ss.ss
    uchar PosFlag;//定位有效位
    float fLat;//纬度
    uchar cLatAspect;//纬度单位
    float fLon;//经度
    uchar cLonAspect;//经度单位
    float fSpeed;//地面速度(000.0~359.9度)
    float fCourse;//航向
    uchar nUtcDay;//日
    uchar nUtcMonth;//月
    uchar nUtcYear;//UTC年
    float fMVariation;// Magnetic Variation，磁偏角（000.0~180.0度，前导位数不足则补0）
    uchar cDeclination;//Declination，磁偏角方向，E（东）或W （西）
    uchar nMode;//模式指示
}TGPSRMC;

#endif// end PT_ENABLE_GPS define

#endif//end  H_PT_GPS_H  define
