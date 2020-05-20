/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name��     pt_bd2_rnss.h
* Abstract�� BD2 RNSS protocol define 
* 
* Version�� 1.0
* Author��  yfwang
* Date��    2012/03/19
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
    TRNTIME UtcTime;//��λʱ��
    double db_Speed;//�ٶ�
    double db_XSpeed;//X���ٶ�
    double db_YSpeed;//Y���ٶ�
    double db_ZSpeed;//Z���ٶ�
    double db_GroudSpeed;//����
    double db_MaxSpeed;//����ٶ�
    double db_AverageSpeed;//ƽ���ٶ�
    double db_AllAvgSpeed;//ȫ��ƽ���ٶ�
    double db_ValidSpeed;//��Ч�ٶ�
    uchar cSpeedUnit;//�ٶȵ�λ:K,km/h
}TRNDHV;

typedef struct TRNGBS
{
    TRNTIME UtcTime;//��λʱ��
    double db_LatError;//γ�ȵ�Ԥ�����
    double db_LonError;//���ȵ�Ԥ�����
    double db_HeigError;//�߶ȵ�Ԥ�����
    int  nAbnorPRNSat;//���п��ܵĹ�������PRN��
    float fAbnorSatLoss;//�����п��ܵĹ�������©����
    float fAbmorSatDev;//������ܵĹ������ǹ��Ƶ�ƫ��
    float fEstDev;//ƫ�ù���ı�׼ƫ��
}TRNGBS;

typedef struct TRNGGA
{
    TRNTIME UtcTime;//��λʱ��
    //γ��
    double db_Lat;
    //γ�ȷ���
    uchar cLatDir;
    //����
    double db_Lon;
    //���ȷ���
    uchar cLonDir;
    //״ָ̬ʾ
    uchar nPosStatus;
    //��Ұ�ڵ�������
    uchar nSatNum;
    //���߸�
    double db_Antheight;
    //���߸ߵ�λ
	uchar cAntHUnit;
	//�߳��쳣
	float f_Kesi;
	//�߳��쳣��λ��M��
	uchar cKesiUnit;
	//�����������
    int DifDataAge;
    //���վ̨ID��
    int DifID;
    //VDOPֵ
    float fVDOP;
}TRNGGA;

typedef struct TRNGLL
{
    //γ��
    double db_Lat;
    //γ�ȷ���
    uchar cLatDir;
    //����
    double db_Lon;
    //���ȷ���
    uchar cLonDir;
    //��λʱ��
    TRNTIME UtcTime;
    //��λ��Чλ
    uchar PosFlag; 
    //ģʽָʾ(0:�Զ�ģʽ,1:���ģʽ,2:����ģʽ,3:�ֶ�����ģʽ,4:ģ����ģʽ)
    int nPosMode;
}TRNGLL;

typedef struct TRNGLS

{
    //γ��
    double db_Lat;
    //γ�ȷ���
    uchar cLatDir;
    //����
    double db_Lon;
    //���ȷ���
    uchar cLonDir;
    //�߳�
    double db_Height;
    //�̵߳�λ
	uchar cHUnit;   
    uchar cPrecision;
    //��ǰUTCʱ��
    TRNDATE UtcDate;
	//��λʱ��
    TRNTIME UtcTime;
    //��ʼ�����(0:������Ч,1:�������,2:����洢��)
    uchar InitType;
}TRNGLS;

typedef struct TRNGSA
{
    uchar nMode;//ģʽָʾ(M:�ֶ�,A:�Զ�) 
    uchar nSelectMode;//ѡ��ģʽ,(1:��ʾ��λ������,2:2D,3:3D)
    uchar nPRNCode[12];//Pseudo Random Noise��α���������(01-32)
    float fPDOP;//λ�þ�������(0.5-99.9)
    float fHDOP;//ˮƽ��������(0.5-99.9)
    float fVDOP;//��ֱ��������(0.5-99.9)
    float fTDOP;
}TRNGSA;

typedef struct TRNGSV_SATINFO
{
    uchar nSatID;//���Ǳ�ţ�01��32
    float fElevation;//�������ǣ�00��90��
    float fAzimuth;//��λ�ǣ�000��359�ȡ�ʵ��ֵ
    float fCN;//����ȣ�C/No����00��99dB���ޱ�δ���յ�Ѷ��
    
}TRNGSV_SATINFO;

typedef struct TRNGSV
{
    uchar nMsgNum;//�ܵ�GSV��������
    uchar nCurMsgNum;//��ǰGSV ����
    uchar nSatNum;//��������������00��12
    TRNGSV_SATINFO SatInfo[4];    
}TRNGSV;

typedef struct TRNICM
{
    TRNDATE BeginDate;
    TRNDATE StopDate;
}TRNICM;

typedef struct TRNIHI
{
    TRNTIME UtcTime;//UTCʱ��
    double db_NorthSpeed;//�����ٶ�(m/s)
    double db_EastSpeed;//�����ٶ�(m/s)
    double db_UpSpeed;//�췽�ٶ�(m/s)
    double db_NorthAcc;//�������ٶ�(m2/s)
    double db_EastAcc;//�������ٶ�(m2/s)
    double db_UpAcc;//�췽���ٶ�(m2/s)
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
    uchar nMode;//ģʽָʾ(0:ʡ��ģʽ��1:����ģʽ)
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
    uchar PosFlag;//��λ��Чλ
    float fLat;//γ��
    uchar cLatDir;//γ�ȵ�λ
    float fLon;//����
    uchar cLonDir;//���ȵ�λ
    float fSpeed;//�����ٶ�(000.0~359.9��)
    float fCourse;//����
    TRNDATE UtcDate;//
    float fMVariation;// Magnetic Variation����ƫ�ǣ�000.0~180.0�ȣ�ǰ��λ��������0��
    uchar cDeclination;//Declination����ƫ�Ƿ���E��������W ������
    uchar nPosMode;//ģʽָʾ(A:������λ,D:���,E:����,N:,������Ч)
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
    double db_Compression;//����
    double db_Dx;
    double db_Dy;
    double db_Dz;
}TRNZBS;

typedef struct TRNZDA
{
    uchar nPosMode;//Posotion mode :RDSS/RNSS
    TRNTIME UtcTime;//
    TRNDATE UtcDate;//date
    uchar nTimeZone;//Time Zone(0- ��13)
    uchar nMinuDif;//minute differece (00-59)
    TRNTIME ReviseTime;//��ʱ����ֵʱ��
    double db_ReviseData;//����ֵ
    uchar nPrecission;//����ָʾ
    uchar cSatLockFlag;//�����ź�����״̬ Y/N
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
