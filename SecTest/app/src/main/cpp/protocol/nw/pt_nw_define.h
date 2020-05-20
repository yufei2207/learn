/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name��     pt_nw_define.h
* Abstract�� nw defined protocol 
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
//�ṹ��˵����KJD,�Զ�������Э��
//////////////////////////////////////////////////////////////////////////
typedef struct TNWKJD
{
	uchar uc_Hello[6];
} TNWKJD;

//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����ICK,�Զ���˫��Э�飬���������跢�����п���״̬
//////////////////////////////////////////////////////////////////////////
typedef struct TNWICK
{
	uchar uc_CommandType;
	uchar uc_SoltNum;
	uchar uc_WorkState;
} TNWICK;

//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����ICQ,�Զ�������Э�飬��ȡ����Ϣ
//////////////////////////////////////////////////////////////////////////
typedef struct TNWICQ
{
	uchar uc_CommandType;
	float f_DeviceZero;
	uchar uc_SerialNum[10];
	uchar uc_ManageInfo[65];
} TNWICQ;

//////////////////////////////////////////////////////////////////////////
//�ṹ��˵����ICH,�Զ�������Э�飬��ȡ����Ϣ
//////////////////////////////////////////////////////////////////////////
typedef struct TNWICH
{
	//�û���ַ��ID�ţ�
	uchar uc_UserID[8];
	//���к�
	uchar uc_SerialNum[9];
	//ͨ����ַ
	uchar uc_BroadcastNum[8];
	//�û�����ֵ
	uchar uc_Eigenvalue;
	//����Ƶ�ȣ��룩
	float f_ServiceFreq;
	//ͨ�ŵȼ���1-4��
	uchar uc_CommLevel;
	//���ܱ�־��E/N��
	uchar uc_EncryptMark;
	//�����û���
	float f_UnderlingCount;
} TNWICH;
//׷�����Զ���Э��
//�豸��¼����
typedef struct TNWLGN
{
    uchar    szDeviceID[16];
    unsigned short nSeqNum; //sequence number
} TNWLGN;
//λ���ϱ�
typedef struct TNWWZR
{
	uchar nYear;  
    uchar nMonth;   
    uchar nDay;  
	uchar nHour;  
	uchar nMinute;
	uchar nSecond;   

	uchar nDeviceID;	//0�������豸;��������̨

	char   chLat;  // 'N' or 'S'
	double dblLat;
	
	char   chLong;  // 'N' or 'S'
	double dblLong;

	double dblHeight; //�߶�
    double dblSpeed; //����
    double dblCourse;//����

	unsigned int   nFlag; //0:����,1:�ض��¼�
}TNWWZR;

//λ�������ϱ�
typedef struct TNWWZI
{
    unsigned short nSeqNum; //sequence number
    unsigned int   nServerNum; //0:����,1:λ�ò�ѯ
    unsigned char  nDataLen;
    unsigned char  szData[210];//λ������
}TNWWZI;

//ƽ̨��ѯ�豸λ��
typedef struct TNWWZQ
{
	unsigned int nServerNum; //Server number
}TNWWZQ;

//״̬�ϱ�
typedef struct TNWSTA
{
	uchar nDataType	 ;  //�������� 0:�ͻ��ˣ�1:GPRS

	uchar nWarnFlag  ;  //������־
    uchar bGPSPos    ;  //�Ƿ�GPS�Ѷ�λ
	uchar bOil       ;  //�Ƿ��͵��ͨ 
	uchar bACC       ;  //�Ƿ�ACC��   
    uchar bCharge    ;  //�Ƿ���
    uchar nPowerLevel;     //��ѹ�ȼ�
    uchar nGSMSignal;      //GSM�ź�ǿ��
   	uchar nGPRSState;      //GPRS״̬	

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

//����ͨ����Ϣ
typedef struct TNWTXS
{
	uchar nCMDType; //ָ������	  1:���ã�2:��ѯ��3����ѯ���
	
    int   nGWUserID;           //�����û���ID

	uchar szGWAddr[32];		   //���ص�ַ
	int   nGWPort;			   //���ض˿�

	unsigned short nFreq; //�ϴ�Ƶ�ȣ��룩
} TNWTXS;

//�͵����
typedef struct TNWYDM
{
	uchar nDataType;  //�������� 0:�ͻ��ˣ�1:GPRS

	uchar nCMDType; //ָ������
	uchar nAction; //�������� 0:�ر�;1:��ͨ

    unsigned int nServerNum; //server flag
}TNWYDM;

#define MAX_NW_PHONE_LEN   12
//��������ѯ
typedef struct TNWHMM
{
	uchar nDataType;  //�������� 0:�ͻ��ˣ�1:GPRS

    unsigned int nServerNum; //server flag

	uchar nCMDType; //ָ������	1:���ӣ�2:ɾ��
    uchar szPhoneNumber[MAX_NW_PHONE_LEN]; 
}TNWHMM;

//��������ѯ
typedef struct TNWHMQ
{
	uchar nDataType	 ;  //�������� 0:�ͻ��ˣ�1:GPRS
    
    uchar nCMDType; //ָ������	1:��ѯ��2:����
    
    unsigned int nServerNum; //server flag
	unsigned short nSeqNum; //seq number

    #define MAX_NW_PHONE_NUM   3
    
    uchar nCount;    //max = 3
    uchar PhoneList[MAX_NW_PHONE_NUM][MAX_NW_PHONE_LEN];     //0:success, other:fail
}TNWHMQ;

//��ѯ�洢����
typedef struct TNWDXQ
{
    uchar nCount;    //��������
}TNWDXQ;

//���������Ϣ
typedef struct TNWDBG
{
    uchar nType;	//��������     
}TNWDBG;

//����ģʽ����
typedef struct TNWMSS
{
    uchar nCMDType; //ָ������	  1:���ã�2:��ѯ��3����ѯ���
    uchar nMode;	//����ģʽ    0���������� 1:֧�ֱ����Խ���     
}TNWMSS;

#endif// end PT_ENABLE_NW

#endif//end  H_PT_NW_DEFINE_H




