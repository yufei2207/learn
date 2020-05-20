/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name��     pt_jtdevice.h
* Abstract�� JT/T808 device protocol 
* 
* Version�� 1.0
* Author��  yfwang
* Date��    2012/08/04
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
    JT808_DV_TERMINAL = 0x01,//�ն�
    JT808_DV_LED_DISPATCHER = 0x02,//������ʾ��
    JT808_DV_LED_NAVIGATION = 0x03,//������ʾ��
    JT808_DV_DETECTOR_OIL = 0x04,//���������
    JT808_DV_DETECTOR_ACCELERATION = 0x05,//���ٶȼ����
    JT808_DV_ALARM_BURGLAR = 0x06,//����������
    JT808_DV_EXTEND_INTERFACE = 0x07,//�ӿ���չ��
    JT808_DV_DETECTOR_WEIGHT = 0x08,//���ؼ����
    JT808_DV_DETECTOR_TRAFFIC = 0x09,//���������
    JT808_DV_SENSOR_UNIVERSAL = 0x0A,//ͨ�ô�����
    JT808_DV_IC = 0x0B,//IC������
    JT808_DV_NW_INTERPHONE = 0x10,//�Խ���
    JT808_DV_NW_TRANSPORT = 0x11,//���ݴ�����->һ���
}TJT808DeviceType;

typedef struct TJT808DVHead
{
    unsigned short nVersion;
    unsigned short nManufacturerID;
    unsigned char  nDeviceType;
    unsigned short nMsgType;
    unsigned char  nDirect;//0 ���У�1����
}TJT808DVHead;

typedef struct TJT808DVCommon
{
    TJT808DVHead head;
}TJT808DVCommon;

typedef TJT808DVCommon TJT808DVPowerOn;

typedef struct TJT808DVPowerCtrl
{
    TJT808DVHead head;

    uchar data;  //host->device, 0x00:�ӻ��˳�ʡ��ģʽ,0x01:�ӻ�����ʡ��ģʽ
                 //device->host��0x01��success��0x02:fail
}TJT808DVPowerCtrl;

typedef struct TJT808DVLinkCheck
{
    TJT808DVHead head;

    uchar bReq; //0:Ӧ��1:����
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
        }a; //�Խ�������
        
        struct 
        {
            unsigned int   nUserID;//server user ID
            unsigned short nUploadFreq;//
            unsigned char  szMobile[12];
        }b;//һ�������
    }param;
}TJT808DVParam;

typedef struct TJT808DVICAuthen
{
    TJT808DVHead head;

    unsigned char nResult;
    unsigned char nDataLen;//���ݳ���
    unsigned char data[64];//���У�24�ֽ�У������;����:64�ֽ���֤��Ϣ
}TJT808DVICAuthen;

typedef struct TJT808DVICReadResult
{
    TJT808DVHead head;

    unsigned char nResult;
    unsigned char  szName[24];//��ʻԱ����
    unsigned char  szCertificateID[48];//��ҵ�ʸ�֤���
    unsigned char  szIssueDepartment[128];//��֤��������
    unsigned char  szUsefulLife[4];//֤����Ч�� 
    unsigned short nYear;//֤����Ч��  YYYYMMDD
    unsigned char  nMonth;//֤����Ч��  YYYYMMDD
    unsigned char  nDay;//֤����Ч��  YYYYMMDD
}TJT808DVICReadResult;

//IC�γ�֪ͨ
typedef TJT808DVCommon TJT808DVICPullout;

//����������ȡIC
typedef TJT808DVCommon TJT808DVICRead;

#endif// end PT_ENABLE_JT808_DV

#endif//end  H_PT_JT808_DV_H













