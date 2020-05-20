/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     pt_jtdevice.h
* Abstract： JT/T808 device protocol 
* 
* Version： 1.0
* Author：  yfwang
* Date：    2012/08/04
*
* history   :
*	     when            who    	          what:
*      -------------   --------------    -------------------
*      Aug 04, 2012   yfwang                create
*
*/
#ifndef H_PT_JT808_DV_H
#define H_PT_JT808_DV_H

#include "../pt_define.h"

#ifdef  PT_ENABLE_JT808_DV

//////////////////////////////////////////////////////////////////////////
//interface define
//////////////////////////////////////////////////////////////////////////

#define JT808_DV_ISTYPE(TYPE)   (TYPE >= JT808_DV_ZERO && TYPE < JT808_DV_LAST)
extern unsigned char JT808_DV_MakeCRC(uchar *buf, int nLen);
extern  unsigned short JT808_DV_Translate(uchar *buf, int nLen, uchar bEncode, int bufsize);

extern  int JT808_DV_DecodeHead(const uchar* buf, int len);
extern  int JT808_DV_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType);

extern  FUNC_ENCODER JT808_DV_GetEncoder(int nType);
extern  FUNC_DECODER JT808_DV_GetDecoder(int nType);

extern  int JT808_DV_RecvData(TStackParam *pParam, uchar data);
extern  int JT808_DV_RecvDataEx(TStackParam *pParam, uchar data);
extern  int JT808_DV_EncodePackage(uchar *lpBuff, int nBuffLen, void *pPackage, 
                               int nMsgType, char bDevice);
extern  int JT808_DV_DecodePackage(void *pPackage, int nMsgType, uchar *lpBuff, int nBuffLen);

#ifdef WIN32
    extern void* JT808_DV_GetPackage(int nType);
#endif
//////////////////////////////////////////////////////////////////////////
//data struct define
//////////////////////////////////////////////////////////////////////////
typedef enum TJT808DeviceType
{
    JT808_DV_TERMINAL = 0x01,//终端
    JT808_DV_LED_DISPATCHER = 0x02,//调度显示屏
    JT808_DV_LED_NAVIGATION = 0x03,//导航显示屏
    JT808_DV_DETECTOR_OIL = 0x04,//油量检测器
    JT808_DV_DETECTOR_ACCELERATION = 0x05,//加速度检测器
    JT808_DV_ALARM_BURGLAR = 0x06,//防盗报警器
    JT808_DV_EXTEND_INTERFACE = 0x07,//接口扩展器
    JT808_DV_DETECTOR_WEIGHT = 0x08,//载重检测器
    JT808_DV_DETECTOR_TRAFFIC = 0x09,//客流检测器
    JT808_DV_SENSOR_UNIVERSAL = 0x0A,//通用传感器
    JT808_DV_IC = 0x0B,//IC读卡器
    JT808_DV_NW_INTERPHONE = 0x10,//对讲机
    JT808_DV_NW_TRANSPORT = 0x11,//数据传输器->一体机
}TJT808DeviceType;

typedef struct TJT808DVHead
{
    unsigned short nVersion;
    unsigned short nManufacturerID;
    unsigned char  nDeviceType;
    unsigned short nMsgType;
    unsigned char  nDirect;//0 上行；1下行
}TJT808DVHead;

typedef struct TJT808DVCommon
{
    TJT808DVHead head;
}TJT808DVCommon;

typedef TJT808DVCommon TJT808DVPowerOn;

typedef struct TJT808DVPowerCtrl
{
    TJT808DVHead head;

    uchar data;  //host->device, 0x00:从机退出省电模式,0x01:从机进入省电模式
                 //device->host，0x01：success，0x02:fail
}TJT808DVPowerCtrl;

typedef struct TJT808DVLinkCheck
{
    TJT808DVHead head;

    uchar bReq; //0:应答；1:请求
    uchar nMinute;
    uchar nSecond;
}TJT808DVLinkCheck;

typedef struct TJT808DVVersion
{
    TJT808DVHead head;

    uchar nVersion;
}TJT808DVVersion;

typedef struct TJT808DVPosition
{
    TJT808DVHead head;

    uchar nDeviceID;
    uchar data[256];//RMC or GGA or Other
}TJT808DVPosition;

typedef struct TJT808DVParam
{
    TJT808DVHead head;
    
    union
    {
        struct 
        {
            uchar nDeviceID;
            uchar nDeviceType;
            unsigned short nUploadFreq;
        }a; //对讲机参数
        
        struct 
        {
            unsigned int   nUserID;//server user ID
            unsigned short nUploadFreq;//
            unsigned char  szMobile[12];
        }b;//一体机参数
    }param;
}TJT808DVParam;

typedef struct TJT808DVICAuthen
{
    TJT808DVHead head;

    unsigned char nResult;
    unsigned char nDataLen;//数据长度
    unsigned char data[64];//下行：24字节校验数据;上行:64字节认证信息
}TJT808DVICAuthen;

typedef struct TJT808DVICReadResult
{
    TJT808DVHead head;

    unsigned char nResult;
    unsigned char  szName[24];//驾驶员姓名
    unsigned char  szCertificateID[48];//从业资格证编号
    unsigned char  szIssueDepartment[128];//发证机构名称
    unsigned char  szUsefulLife[4];//证件有效期 
    unsigned short nYear;//证件有效期  YYYYMMDD
    unsigned char  nMonth;//证件有效期  YYYYMMDD
    unsigned char  nDay;//证件有效期  YYYYMMDD
}TJT808DVICReadResult;

//IC拔出通知
typedef TJT808DVCommon TJT808DVICPullout;

//主动触发读取IC
typedef TJT808DVCommon TJT808DVICRead;

#endif// end PT_ENABLE_JT808_DV

#endif//end  H_PT_JT808_DV_H













