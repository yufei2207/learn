/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name��     pt_gps.h
* Abstract�� gps protocol define
* 
* Version�� 1.0
* Author��  yfwang
* Date��    2012/03/19
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
    uchar nHour;//ʱ hh
    uchar nMinute;//�� mm
    float fSecond;//��ss.ss
    float fLat;//γ��
    uchar cLatDir;//γ�ȵ�λ
    float fLon;//����
    uchar cLonDir;//���ȵ�λ
    uchar cPosFlag;//��λ��Ч��־(0:valid,1:invalid)
    uchar nSatNum;//��Ұ�ڵ�������
    float fPrecision;//ˮƽ����(0.5~99.9)
    double dbAnteAltitude;//������Ժ�ƽ��߶�(-9999.9~9999.9m)
    uchar cAltitudeunit;//��λ
    double dbSphereAlti;//�����������Ժ�ƽ��߶�(-9999.9~9999.9m)
    uchar cSphereunit;//��λ
    int   nDifDataTerm;//�����������
    int   DifRefID;//���վ̨ID��
    
}TGPSGGA;

typedef struct TGPSGSA
{
    uchar nMode2;//ģʽ2 
    uchar nMode1;//ģʽ1
    uchar nPRNCode[12];//Pseudo Random Noise��α���������(01-32)
    float fPDOP;//λ�þ�������(0.5-99.9)
    float fHDOP;//ˮƽ��������(0.5-99.9)
    float fVDOP;//��ֱ��������(0.5-99.9)
}TGPSGSA;

typedef struct TGPSGSV_SATINFO
{
    uchar nSatID;//���Ǳ�ţ�01��32
    float fElevation;//�������ǣ�00��90��
    float fAzimuth;//��λ�ǣ�000��359�ȡ�ʵ��ֵ
    float fCN;//����ȣ�C/No����00��99dB���ޱ�δ���յ�Ѷ��
    
}TGPSGSV_SATINFO;

typedef struct TGPSGSV
{
    uchar nMsgNum;//�ܵ�GSV��������
    uchar nCurMsgNum;//��ǰGSV ����
    uchar nSatNum;//��������������00��12
    TGPSGSV_SATINFO SatInfo[4];    
}TGPSGSV;

typedef struct TGPSRMC
{
    uchar nUtcHour;//hh
    uchar nUtcMinute;//mm
    float fSecond;//ss.ss
    uchar PosFlag;//��λ��Чλ
    float fLat;//γ��
    uchar cLatAspect;//γ�ȵ�λ
    float fLon;//����
    uchar cLonAspect;//���ȵ�λ
    float fSpeed;//�����ٶ�(000.0~359.9��)
    float fCourse;//����
    uchar nUtcDay;//��
    uchar nUtcMonth;//��
    uchar nUtcYear;//UTC��
    float fMVariation;// Magnetic Variation����ƫ�ǣ�000.0~180.0�ȣ�ǰ��λ��������0��
    uchar cDeclination;//Declination����ƫ�Ƿ���E��������W ������
    uchar nMode;//ģʽָʾ
}TGPSRMC;

#endif// end PT_ENABLE_GPS define

#endif//end  H_PT_GPS_H  define
