/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name��     pt_bd2_rdss.h
* Abstract�� BD2 RDSS protocol define 
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
	//��Ӧ������
	uchar nInboudBeam;
	//ʱ�����
	uchar nTmDifBeam;
	//�����źŹ���
	uchar nPower[10];
}TRDBSI;

//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����BSS,2��RDSS����Э�飬������Ӧ������ʱ���
//////////////////////////////////////////////////////////////////////////

typedef struct TRDBSS
{
	//��Ӧ������1-10��
	uchar nInboudBeam;
	//ʱ�����1-10��
	uchar nTmDifBeam;
}TRDBSS;
//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����CXA,2��RDSS����Э�飬��ѯ����ͨ����Ϣ
//////////////////////////////////////////////////////////////////////////

typedef struct TRDCXA
{
	//��ѯ���0:��λ��ѯ,1:ͨ�Ų�ѯ��
	uchar nQueryType;
	//��ѯ��ʽ��1-3��
	uchar nQueryMode;
	//�û���ַ��ID�ţ�
	int nUserID;
}TRDCXA;
//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����DSA,2��RDSS����Э�飬���Ͷ�ʱ����
//////////////////////////////////////////////////////////////////////////

typedef struct TRDDSA
{
	//�û���ַ��ID�ţ�
	int nUserID;
	//��ʱ��ʽ��1/2��
	uchar nTimingMode;
	//����λ����Ϣָʾ��A/V��
	uchar cLocaFlag;
	//γ��llll.ll
	double db_Lat;
	//����yyyyy.yy
	double db_Lon;
	//����Ƶ�ȣ��룻0Ϊ�������룩
	float f_Freq;
	//������ֵ
	float f_SingleZero;
	//������ֵ�����룩
	float f_AddonZero;
}TRDDSA;
//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����DWA,2��RDSS����Э�飬���Ͷ�λ����
//////////////////////////////////////////////////////////////////////////
typedef struct TRDDWA
{
	//�û���ַ��ID�ţ�
	int nUserID;	
	//������λ��A:������λ,V:��ͨ��λ��
	uchar cUrgencyPos;
	//��߷�ʽ��0:�и߳�,1:�޸߳�,2:���1,3:���2��
	uchar uc_MHMode;
	//�߳�ָʾ��H:�߿��û�,L:��ͨ�û���
	uchar cLocaType;
	//�߳����ݣ��ף�
	double db_HeightData;
	//���߸ߣ��ף�
	double db_AntHigh;
	//��ѹ���ݣ�����
	double db_AirPressure;
	//�¶����ݣ��ȣ�
	double db_Temperature;
	//����Ƶ�ȣ���,0:���ζ�λ��
	double db_Freq;
}TRDDWA;
//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����DWR,2��RDSS���Э�飬���յ��Ķ�λ����
//////////////////////////////////////////////////////////////////////////
typedef struct TRDDWR
{
	//��λ��Ϣ���ͣ�1-3��
	uchar nPosType;
	//�û���ַ��ID�ţ�
	int nUserID;
	//��λʱ�̣�UTCʱ�䣩
	uchar nHour;
	uchar nMinute;
	float f_Second;
	//γ��
	double db_Lat;
	//γ�ȷ���N/S��
	uchar cLatDir;
	//����
	double db_Lon;
	//���ȷ���E/W��
	uchar cLonDir;
	//��ظ�
	double db_Height;
	//��ظߵ�λ��M��
	uchar cHunit;
	//�߳��쳣
	float f_Kesi;
	//�߳��쳣��λ��M��
	uchar cKesiUnit;
	//����ָʾ��0-1��
	uchar nPrecision;
	//������λָʾ��A/V��
	uchar cUrgencyPos;
	//��ֵ��ָʾ��A/V��
	uchar cMutilpleValue;
	//�߳�����ָʾ��H/L��
	uchar cLocaType;
}TRDDWR;

//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����FKI,2��RDSS���Э�飬�û��豸����ķ�����Ϣ
//////////////////////////////////////////////////////////////////////////
typedef struct TRDFKI
{
	//ָ������
	uchar cCmdName[4];
	//ָ��ִ�������Y/N��
	uchar cCmdExecFlag;
	//Ƶ������ָʾ��Y/N��
	uchar cFreqSetFlag;
	//��������ָʾ��0-3��
	uchar nRestrainFlag;
	//�ȴ�ʱ��
	uchar nWaitHour;
	uchar nWaitSecond;
}TRDFKI;

//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����GXM,2��RDSS˫��Э�飬���ã���ȡ�����û��豸����
//								��Ϣ
//////////////////////////////////////////////////////////////////////////
typedef struct 
{
	//ָ�����ͣ�1-3��
	uchar cCmdType;
	//������Ϣ
	uchar MngInfo[64];
}TRDGXM;

//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����ICA,2��RDSS����Э�飬��Ȿ�û��豸�ӽ���ģ����Ϣ
//////////////////////////////////////////////////////////////////////////
typedef struct TRDICA
{
	//ָ�����ͣ�0-1  0������  1�������û���
	uchar nCmdType;
	//�����û���Ϣ֡��
	uchar nFrameNum;
}TRDICA;
//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����ICI,2��RDSS���Э�飬�����������ģ����Ϣ
//////////////////////////////////////////////////////////////////////////
typedef struct TRDICI
{
	//�û���ַ��ID�ţ�
	int nUserID;
	//���к�
	int nSerialNum;
	//ͨ����ַ
	int nBrdID;
	//�û�����ֵ
	uchar nUserType;
	//����Ƶ�ȣ��룩
	float f_Freq;
	//ͨ�ŵȼ���1-4��
	uchar nCommLevel;
	//���ܱ�־��E/N��
	uchar cEncryptFlag;
	//�����û���
	int nSubuserNum;
}TRDICI;

//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����ICZ,2��RDSS���Э�飬ָ�����û��豸��������û�
//								��Ϣ�������һ������⣬����ÿ�����봫��40���û�
//								��Ϣ��
//////////////////////////////////////////////////////////////////////////
typedef struct TRDICZ
{
	//�������û���
	int nSubuserNum;
	//�����û�ID�ţ�һ��Э�������40����
	int nSubuserID[40];
}TRDICZ;

//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����JMS,2��RDSS����Э�飬�����û��豸ʵ�����ߵ羲Ĭ
//////////////////////////////////////////////////////////////////////////
typedef struct TRDJMS
{
	//���ߵ羲Ĭ����ָʾ��E/N��
	uchar cJMFlag;
}TRDJMS;

//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����KLS,2��RDSS����Э�飬���Ϳ���ʶ��ָ��
//////////////////////////////////////////////////////////////////////////
typedef struct TRDKLS
{
	//�û���ַ��ID�ţ�
	int nUserID;
	//Ӧ���־
	uchar cRequestFlag;
}TRDKLS;

//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����KLT,2��RDSS˫��Э�飬ָ�����û��豸��������û�����
//								�Ŀ���ʶ�����ݣ�������ͨ���û��豸��Ӧָ�����û��豸
//								����ʶ��ָ�����Ϣ
//////////////////////////////////////////////////////////////////////////
typedef struct TRDKLT
{
	//��ʶ��P/Z P����ͨ���û���Z��ָ�����û���
	uchar cUserType;
	//�û���ַ��ID�ţ�
	int nUserID;
	//�������ͣ�0-2��
	uchar nInfoType;
	//��������
	int nInfolen;
	int nInfo[248];
}TRDKLT;

//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����LZM,2��RDSS˫��Э�飬�������û��豸������ֵ���ȡ
//								�豸��ֵ���룬�û��豸����������豸��ֵ
//////////////////////////////////////////////////////////////////////////
typedef struct TRDLZM
{
	//����ģʽ��1-3��
	uchar nMode;
	//�豸��ֵ�����룩
	double dbZeroValue;
}TRDLZM;

//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����HZR,2��RDSS���Э�飬�û��豸����ͨ�Ż�ִ��ѯ���
//								��õĻ�ִ��Ϣ
//////////////////////////////////////////////////////////////////////////
typedef struct TRDHZR_TIME
{
    int nHour;
    int nMinute;
}TRDHZR_TIME;

typedef struct TRDHZR
{
	//�û���ַ��ID�ţ�
	int nUserID;
	//��ִ����0-5 0���޻�ִ��
	uchar nHZNum;
	//����ʱ�䣨ʱ�֣�
    TRDHZR_TIME SendTime[5];
    //��ִʱ�䣨ʱ�֣�
    TRDHZR_TIME RespTime[5];
}TRDHZR;

//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����TXA,2��RDSS����Э�飬���������û��豸����ͨ������
//////////////////////////////////////////////////////////////////////////
typedef struct TRDTXA
{
	//�û���ַ��ID�ţ�
	int nUserID;
	//ͨ�����0/1 0���ؿ� 1����ͨ��
	uchar nCommMode;
	//���䷽ʽ��0-2 0������ 1������ 2:�췢 3:λ���ϱ���
	uchar nInfoType;
	//ͨ�ŵ�������
	int nInfolen;
	char Info[420];
}TRDTXA;

//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����TXR,2��RDSS���Э�飬�û��豸����ͨ��������õ�
//								ͨ����Ϣ
//////////////////////////////////////////////////////////////////////////
typedef struct TRDTXR
{
	//��Ϣ���1-5��
	uchar nMsgType;
	//�û���ַ��ID�ţ�
	int nUserID;
    char szUserID[16];
	//������ʽ��0������  1������ 2:�췢 3:λ���ϱ���
	uchar nInfoType;
	//����ʱ��
	uchar nSendHour;
	uchar nSendMin;
	//ͨ�ŵ�������
	int nInfolen;
	uchar Info[420];
}TRDTXR;

//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����WAA,2��RDSS˫��Э�飬�û������û��豸����λ�ñ���
//								1���룬����䲻������ָ�����û��豸���յ�λ����Ϣ���
//////////////////////////////////////////////////////////////////////////
typedef struct TRDWAA
{
	//��Ϣ���ͣ�0-1  0���û��豸���յ�λ�ñ���  1���û��豸���͵�λ�ñ��棩
	uchar nMsgType;
	//����Ƶ�ȣ���  0������λ�ñ��棩
	float f_Freq;
	//�û���ַ��ID�ţ�
	int nUserID;
	//λ�ñ���ʱ��
	uchar nReportHour;
	uchar nReportMin;
	float f_ReportSecond;
	//γ��
	float f_Lat;
	//γ�ȷ���
	uchar cLatAspect;
	//����
	float f_Lon;
	//���ȷ���
	uchar cLonAspect;
	//�߳�ֵ
	float f_Height;
	//�̵߳�λ���ף�
	uchar cHeightUnit;
}TRDWAA;

//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����WBA,2��RDSS����Э�飬�û������û��豸����λ�ñ���
//								2���룬������Ӧ��������ΪDWR
//////////////////////////////////////////////////////////////////////////
typedef struct TRDWBA
{
	//�û���ַ��ID�ţ�
	int nUserID;
	//�߳�ָʾ��H/L��
	uchar cHeightflag;
	//���߸ߣ��ף�
	double f_AnteHeight;
	//����Ƶ�ȣ��� 0������λ�ñ��棩
	float f_Freq;
}TRDWBA;

//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����ZHS,2��RDSS����Э�飬�������û��豸�����Ի�ָ�
//								���������û��豸�����Ի�
//////////////////////////////////////////////////////////////////////////
typedef struct TRDZHS
{
	//�Ի�ָ��
	uchar cSelfDestory[9];
}TRDZHS;

//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����RMO,2��RDSS���Э�飬�趨�򼺷������ֹͣ����������
//////////////////////////////////////////////////////////////////////////
typedef struct TRDRMO
{
	uchar uc_TagCommond[4];
	uchar uc_Mode;
	float f_Freq;
}TRDRMO;

#endif// end PT_ENABLE_BD2_RD define

#endif//end  H_PT_BD2_RDSS_H  define
