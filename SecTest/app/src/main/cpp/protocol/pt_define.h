/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name��     protocol.h
* Abstract�� protocol interface
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

#ifndef H_PT_DEFINE_H
#define H_PT_DEFINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef NULL
    #define NULL    0
#endif

typedef unsigned char  uchar;

#include "pt_utility.h"

//result define 
#define	 RESULT_ERROR           -1
#define	 RESULT_VERIFY_ERROR    -2
#define	 RESULT_INVILID_TYPE    -3
#define	 RESULT_SUCCESS          0

typedef enum TStackType
{
   STACK_BD1        = 1 << 0, 
   STACK_BD2_RD     = 1 << 1, 
   STACK_BD2_RN     = 1 << 2, 
   STACK_BD2_SP     = 1 << 3, 
   STACK_BD2_EX     = 1 << 4, 
   STACK_NW         = 1 << 5,
   STACK_GPS        = 1 << 6,
   STACK_GT06       = 1 << 7,
   STACK_JT808      = 1 << 8,
   STACK_JT808_DV   = 1 << 9,
}TStackType;

typedef struct TStackParam
{
    int  nStackType; //Э��ջ����
    int  nRecvFlag;  //����״̬
    char bRecvFrame; //�Ƿ���յ�һ֡����

    short nFrameLen;//һ��Э���Լ�����RD����Э��֡����
    unsigned short nCRCValue;//У��
    //�Ƿ����У��
    uchar bCRC;
    //����buff
    uchar *RecvBuff;
    //���ջ��泤��
    int   nRecvBuffLen;
    //�����ַ�����
    int   nRecvLen;
    //Э������
    int   nMsgType;
    //���ݽ��պ���ָ��
    void *fnRecv;
}TStackParam;

//���뺯���ӿڶ���
typedef int (*FUNC_DECODER)(void *pPackage, uchar *lpBuff, int nBuffLen);
//���뺯���ӿڶ���
typedef int (*FUNC_ENCODER)(uchar *lpBuff, int nBuffLen, void* pPackage);

#include "pt_config.h"  //Э��ջ�����ļ�

#ifdef PT_ENABLE_BD1
    #include "./bd1/pt_bd1.h"
#endif 

#ifdef PT_ENABLE_BD2_RD
    #include "./bd2/pt_bd2_rdss.h"
#endif 

#ifdef PT_ENABLE_BD2_RN
    #include "./bd2/pt_bd2_rnss.h"
#endif 

#ifdef PT_ENABLE_BD2_EXPRESS
    #include "./bd2/pt_bd2_express.h"
#endif 
       
#ifdef PT_ENABLE_BD2_SPECIAL
    #include "./bd2/pt_bd2_special.h"
#endif 

#ifdef PT_ENABLE_NW
    #include "./nw/pt_nw_define.h"
#endif 

#ifdef PT_ENABLE_GPS
    #include "./gps/pt_gps.h"
#endif 

#ifdef PT_ENABLE_GT06
    #include "./gt06/pt_gt06.h"
#endif

#ifdef PT_ENABLE_JT808
    #include "./jt808/pt_jt808.h"
#endif

#ifdef PT_ENABLE_JT808_DV
    #include "./jt808/pt_jt808_device.h"
#endif

typedef enum TProtocolType
{
    INVALID_TYPE = -1,
    TY_NULL = 0,
#ifdef PT_ENABLE_BD1   //һ��Э�飬�������û�����Ϣ
    BD1_ZERO,
    BD1_GLJC = BD1_ZERO,//���ʼ��
    BD1_DWSQ,      //��λ����
    BD1_TXSQ,      //ͨ������
    BD1_HXSZ,      //��������
    BD1_DHSQ,      //��������
    BD1_DSSQ,      //��ʱ����
    BD1_SCSC,      //ʱ�����
    BD1_ZBZH,      //����ת��
    BD1_JSZL,      //����ָ��
    BD1_GBXX,      //�㲥��Ϣ
    BD1_GYXX,      //������Ϣ
    BD1_WMCS,      //�������
    BD1_CKSC,      //�������
    BD1_ICJC,      //IC���
    BD1_JJZH,      //�����Ի�
    BD1_BSSZ,      //��������
    BD1_GXZX,      //����ע��
    BD1_GXDQ,      //���Ŷ�ȡ
    BD1_XTZJ,      //ϵͳ�Լ� 
    BD1_LZSZ,      //��ֵ����
    BD1_LZDQ,      //��ֵ��ȡ
    BD1_SJSC,      //ʱ�����
    BD1_BBDQ,      //�汾��ȡ
    BD1_XHDQ,      //��Ŷ�ȡ
    BD1_GLZK,      //����״��//�û�����������Ϣ
    BD1_DWXX,      //��λ��Ϣ
    BD1_TXXX,      //ͨ����Ϣ
    BD1_TXHZ,      //ͨ�Ż�ִ
    BD1_KLSB,      //����ʶ��
    BD1_DHSC,      //�������
    BD1_DSJG,      //��ʱ���
    BD1_SCSJ,      //ʱ������
    BD1_ZBSC,      //�������
    BD1_GBSC,      //�㲥���
    BD1_GYSC,      //�������
    BD1_ICXX,      //IC��Ϣ
    BD1_ZHQR,      //�Ի�ȷ��
    BD1_ZJXX,      //�Լ���Ϣ
    BD1_LZXX,      //��ֵ��Ϣ
    BD1_SJXX,      //ʱ����Ϣ
    BD1_BBXX,      //�汾��Ϣ
    BD1_XHXX,      //�����Ϣ
    BD1_GLXX,      //������Ϣ
    BD1_FKXX,      //������Ϣ
    BD1_ILXX,      //I·��Ϣ
    BD1_QLXX,      //Q·��Ϣ
    BD1_RZCW,       //�Զ���Э��
    BD1_BJLZ,
    BD1_DQGK,      //��ȡ����
    BD1_BHXX,      //������Ϣ
    BD1_SSXX,      //��ʱ��Ϣ
    BD1_KZXX,      //
    BD1_DSXX,      
    BD1_SZXX,
    BD1_DSLX,
    BD1_BJWZ,
    BD1_FPGA,
    BD1_BDSZ,      //����������Ϣ
    BD1_LAST = BD1_BDSZ, //һ��Э�����
#endif//PT_ENABLE_BD1
#ifdef PT_ENABLE_BD2_RN   //����RNSSЭ��
    BD2_RN_ZERO,
    BD2_RN_AAM = BD2_RN_ZERO,//��·�㵽�ﱨ��
    BD2_RN_ALM,//������������
    BD2_RN_APL,//����Ա�������
    BD2_RN_BSD,//������ʶ��
    BD2_RN_BSQ,//������ʶ��
    BD2_RN_BSX,//������ʶ��
    BD2_RN_COM,//�����û��豸����
    BD2_RN_DHV,//�ٶ��ർ����Ϣ
    BD2_RN_GBS,//����GNSS���ǹ��ϼ��
    BD2_RN_GGA,//������λ����
    BD2_RN_GLL,//������궨λ��Ϣ
    BD2_RN_GLS,//�����û��豸λ�õȳ�ʼ����Ϣ
    BD2_RN_GSA,//�����û��豸����ģʽ��
    BD2_RN_GST,//����α�����ͳ������
    BD2_RN_GSV,//���������Ϣ
    BD2_RN_ICM,//����û��豸�мӽ���ģ����Կ��Ч����Ϣ
    BD2_RN_IHI,//�����û��豸����ٶȼ��ٶȵ���Ϣ
    BD2_RN_IHO,//�������������Ϣ
    BD2_RN_LPM,//�����豸������ʡ��ģʽ
    BD2_RN_MSS,//�����豸��ǰ��λ��ʽ
    BD2_RN_PMU,//���PRM��ʱЧ������Ϣ
    BD2_RN_RMC,//�����򵼺���������
    BD2_RN_TXT,//���ڴ�����ı�
    BD2_RN_VTG,//����������͵���
    BD2_RN_ZBS,//��������ϵ����
    BD2_RN_ZDA,//����UTCʱ�䡢���ںͱ���ʱ��
    BD2_RN_ZTI,//����豸��ǰ����״̬��Ϣ
    BD2_RN_LAST = BD2_RN_ZTI,
#endif //PT_ENABLE_BD2_RN
#ifdef PT_ENABLE_BD2_RD   //����RDSSЭ��
    BD2_RD_ZERO,
    BD2_RD_BSI = BD2_RD_ZERO,//���ղ���״̬��Ϣ
    BD2_RD_BSS,      //������Ӧ������ʱ���
    BD2_RD_CXA,      //���ò�ѯ����
    BD2_RD_DSA,      //���ö�ʱ����
    BD2_RD_DWA,      //���ö�λ����
    BD2_RD_DWR,      //��λ��Ϣ
    BD2_RD_FKI,      //�û��豸������Ϣ
    BD2_RD_GXM,      //������Ϣ���á���ѯ
    BD2_RD_ICA,      //���ü��ӽ���ģ��
    BD2_RD_ICI,      //�û��豸�����ӽ���ģ����Ϣ
    BD2_RD_ICZ,      //ָ�ӻ������û���Ϣ
    BD2_RD_JMS,      //���߾�Ĭ����
    BD2_RD_KLS,      //ָ�ӻ����Ϳ���ʶ��ָ��
    BD2_RD_KLT,      //����ʶ��Ӧ��
    BD2_RD_LZM,      //�û��豸��ֵ����
    BD2_RD_HZR,      //��ִ��Ϣ
    BD2_RD_TXA,      //����ͨѶ����
    BD2_RD_TXR,      //ͨѶ��Ϣ
    BD2_RD_WAA,      //���û����λ�ñ���1��λ������
    BD2_RD_WBA,      //����λ�ñ���2����
    BD2_RD_ZHS,      //�����Ի�
    BD2_RD_LAST = BD2_RD_ZHS,
#endif//PT_ENABLE_BD2_RD
#ifdef PT_ENABLE_BD2_EXPRESS    //ר�����
    BD2_EX_ZERO,
    BD2_EX_ECS = BD2_EX_ZERO,      //�������ԭʼ������Ϣ
    BD2_EX_ECT,      //ԭʼ������Ϣ
    BD2_EX_TCS,      //����ͨ��ǿ�Ƹ���
    BD2_EX_IDV,      //���ż����Ϣ
    BD2_EX_PRD,      //�����û��豸���α��۲�ֵ���ز���λ
    BD2_EX_PRO,      //ԭʼα��۲�ֵ���ز���λ
    BD2_EX_RIS,      //�豸��λ
    BD2_EX_RMO,      //�������
    BD2_EX_SCS,      //RDSS˫ͨ��ʱ������
	BD2_EX_TXM,		//ͨ������
	BD2_EX_WGM,		//��������
    BD2_EX_LAST = BD2_EX_WGM,
#endif// PT_ENABLE_BD2_EXPRESS
#ifdef PT_ENABLE_BD2_SPECIAL//�������
    BD2_SP_ZERO,
    BD2_SP_XNF = BD2_SP_ZERO,      //�û��豸���չߵ�������Ϣ��ʽ
    BD2_SP_XSD,      //��������û���λ��Ϣ 
    BD2_SP_XST,      //��������û�ͨѶ��Ϣ
    BD2_SP_LAST = BD2_SP_XST,
#endif// PT_ENABLE_BD2_SPECIAL
#ifdef PT_ENABLE_NW//ŵά�Զ���Э��
    NW_ZERO,
    NW_KJD = NW_ZERO, //��������ָ��
    NW_ICK,     //����״̬����
    NW_ICS,     //���п�����״̬
    NW_ICQ,     //��ȡ������Ϣ
    NW_ICH,     //��ǰ���Ļ�����Ϣ
    NW_JKD,     //�ӿ��л�
	NW_LGN,     //�豸��¼����
    NW_WZR,     //�豸���λ�����ͻ���
    NW_WZI,     //�豸�ϱ�λ����ƽ̨
	NW_WZQ,     //ƽ̨��ѯ�豸λ��
	NW_STA,     //�豸�ϱ�״̬
	NW_TXI,     //ƽ̨���豸����ͨ��
	NW_FKS,     //���ط������豸����Ӧ	 ����������һ���
    NW_FKD,     //�豸���������ص���Ӧ	 һ�������������
	NW_YDM,     //���ؿ���ָ�� �ر�\��ͨ�͵�  ����������һ���
	NW_TXS,     //�ͻ�������һ���ͨ�Ų���
    NW_HMM,     //���ӡ�ɾ��SOS����
    NW_HMQ,     //��ѯSOS����
	NW_DXQ,     //��ѯ�豸�洢����
	NW_DBG,     //�����豸������Ϣ
    NW_MSS,     //���ù���ģʽ
    NW_LAST = NW_MSS,//ŵά�Զ���Э�����
#endif//PT_ENABLE_NW
#ifdef PT_ENABLE_GPS//GPSЭ��
    GPS_ZERO,
    GPS_GGA = GPS_ZERO,//ȫ��λ����
    GPS_GSA,      //����PRN����
    GPS_GSV,      //����״̬��Ϣ
    GPS_RMC,      //���䶨λ����
    GPS_VTG,      //�����ٶ���Ϣ
    GPS_GLL,      //�����������
    GPS_ZDA,      //UTCʱ�������
    GPS_LAST = GPS_ZDA,
#endif//PT_ENABLE_GPS
#ifdef PT_ENABLE_GT06//GPSЭ��
    GT06_ZERO,
    GT06_LOGINREQ = GT06_ZERO,//��¼����
    GT06_LOGINRES,//��¼��Ӧ
    GT06_POSITION, //��λ
    GT06_STATEREQ,  //״̬��Ϣ����
    GT06_STATERES,  //״̬��Ϣ��Ӧ
    GT06_CMDREQ,  //����ָ��
    GT06_CMDRES,  //����ָ����Ӧ
    GT06_PHOTO,  //�ϴ�ͼƬ
    GT06_LAST = GT06_PHOTO,
#endif//PT_ENABLE_GPS
#ifdef PT_ENABLE_JT808
    JT808_ZERO,
    JT808_TERMINALRES = JT808_ZERO,//�ն�ͨ��Ӧ���
    JT808_PLATFORMRES,//ƽ̨ͨ��Ӧ���
    JT808_KEEPALIVE,//�ն�������
    JT808_REGISTERREQ,//�ն�ע��̩�
    JT808_REGISTERRES,//�ն�ע��Ӧ���
    JT808_DEREGISTER,//�ն�ע����
    JT808_LOGIN,//�ն˼�Ȩ��
    JT808_PARAMETET_SET,//�����ն˲����̩�
    JT808_PARAMETER_QUERYREQ,//��ѯ�ն˲�����
    JT808_PARAMETER_QUERYRES,//��ѯ�ն˲���Ӧ���
    JT808_TERMINALCTRL,//�ն˿��ơ�
    JT808_POSITION_REPORT,//λ����Ϣ�㱨�̩�
    JT808_POSITION_QUERYREQ,//λ����Ϣ��ѯ��
    JT808_POSITION_QUERYRES,//λ����Ϣ��ѯӦ���
    JT808_POSITION_TRACK,//��ʱλ�ø��١�
    JT808_TEXT_NOTICE,//�ı��·���
    JT808_EVENT_SET,//�¼����á�
    JT808_EVENT_REPORT,//�¼������
    JT808_QUESTION_REQ,//�����·���  
    JT808_QUESTION_RES,//����Ӧ���
    JT808_SERVICE_SET,//��Ϣ�㲥�˵����á�
    JT808_SERVICE_OPT,//��Ϣ�㲥/ȡ����
    JT808_SERVICE_INFO,//��Ϣ�����
    JT808_PHONECALLBACK,//�绰�ز���
    JT808_PHONEBOOK,//���õ绰����
    JT808_CAR_CTRLREQ,//�������ơ�
    JT808_CAR_CTRLRES,//��������Ӧ���
    JT808_REGION_CIRCLE_SET,//����Բ�������
    JT808_REGION_CIRCLE_REMOVE,//ɾ��Բ�������
    JT808_REGION_RECT_SET,//���þ��������
    JT808_REGION_RECT_REMOVE,//ɾ�����������
    JT808_REGION_POLYGON_SET,//���ö���������
    JT808_REGION_POLYGON_REMOVE,//ɾ������������
    JT808_ROUTE_SET,//����·�ߡ�
    JT808_ROUTE_REMOVE,//ɾ��·�ߡ�
    JT808_DRIVINGRECORDER_REQ,//��ʻ��¼�����ݲɼ������
    JT808_DRIVINGRECORDER_RES,//��ʻ��¼�������ϴ���
    JT808_DRIVINGRECORDER_PARAM,  //��ʻ��¼�ǲ����´������
    JT808_ORDER_REPORT,              //�����˵��ϱ���
    JT808_DRIVERINFO_REPORT,         //��ʻԱ�����Ϣ�ɼ��ϱ��̩�
    JT808_MULTIMEDIA_EVENT_REPORT,//��ý���¼���Ϣ�ϴ���
    JT808_MULTIMEDIA_DATA_UPLOADREQ,//��ý�������ϴ���
    JT808_MULTIMEDIA_DATA_UPLOADRES,//��ý�������ϴ�Ӧ���
    JT808_CAMERA_CTRL,//����ͷ����ָ�����
    JT808_MULTIMEDIA_DATA_QUERYREQ,//�洢��ý�����ݼ���
    JT808_MULTIMEDIA_DATA_QUERYRES,//�洢��ý�����ݼ���Ӧ��
    JT808_MULTIMEDIA_DATA_UPLOADCMD,//�洢��ý�������ϴ�����
    JT808_RECORDER_CTRL,//¼�����ʼ/ֹͣ��
    JT808_DATA_DOWNLOAD,//��������͸����
    JT808_DATA_UPLOAD,//��������͸����
    JT808_DATA_GZIP_UPLOAD,//����ѹ���ϱ���
    JT808_PLATFORM_RAS,//ƽ̨RAS��Կ��
    JT808_TERMINAL_RAS,//�ն�RAS��Կ��
    JT808_MULTIPOSITION,//���û�λ���ϱ�,�Զ���Э���
    JT808_SENSOR_REPORT,//�����������ϴ���
    JT808_SENSOR_QUERYREQ,//���������ݲ�ѯ�����
    JT808_SENSOR_QUERYRES,//���������ݲ�ѯ������
	JT808_SEND_AGAINREQ,//�����ְ������  VER_201301
	JT808_SPECIFY_PARAM_QUERYREQ,//��ѯָ���ն˲�����
	JT808_PROPERTY_QUERYREQ,//��ѯ�ն����ԡ�
	JT808_PROPERTY_QUERYRES,//��ѯ�ն�����Ӧ���
	JT808_UPDATE_PACKAGEREQ,//�·��ն���������
	JT808_UPDATE_PACKAGERES,//�ն��������֪ͨ��
	JT808_WARM_MANAGE,//�˹�ȷ�ϱ�����Ϣ��
	JT808_DRIVERINFO_REPORTREQ,//�ϱ���ʻԱ�����Ϣ�����
	JT808_POSDATA_UPLOAD,//��λ���������ϴ���
	JT808_CAN_DATA_UPLOAD,//CAN���������ϴ�
	JT808_CAMERA_CTRL_RES,//����ͷ������������Ӧ��
	JT808_ONE_MULTIMEDIA_DATA_QUERYREQ,//�����洢��ý�����ݼ����ϴ�����
    JT808_DEVICE_UNLOCK,//ʹ��ǰ����
	JT808_LAST,

#endif //PT_ENABLE_JT808
#ifdef PT_ENABLE_JT808_DV
    JT808_DV_ZERO,
    JT808_DV_POWERON = JT808_DV_ZERO,  //�ϵ�ָʾ��ͨ����Ϣ
    JT808_DV_LINKCHECK,//��·��⣬ͨ����Ϣ
    JT808_DV_POWERCTRL,//��Դ���ƣ�ͨ����Ϣ
    JT808_DV_VERSION,  //����汾��⣬ͨ����Ϣ
    JT808_DV_SELFCHECK, //�����Լ죬ͨ����Ϣ
    JT808_DV_LIVEUPDATE,//����̼�������ͨ����Ϣ
    JT808_DV_IC_AUTHENTICATE,  //IC��֤����/Ӧ��
    JT808_DV_IC_READRESULT, //IC��ȡ�������/Ӧ��
    JT808_DV_IC_PULLOUT, //IC�γ�֪ͨ/Ӧ��
    JT808_DV_IC_READ, //IC����������ȡ����/Ӧ��
    JT808_DV_POSITION, //���趨λ����,�Զ�����Ϣ
    JT808_DV_PARAM, //�������,�Զ�����Ϣ
    JT808_DV_LAST,
#endif //PT_ENABLE_JT808_DV
}TProtocolType;

/*
*�������ƣ�PT_InitLib
*����˵������ʼ��Э���
*����˵������
*����ֵ��  ��
*/
void  PT_InitLib(void);

/*
*�������ƣ�PT_Initialize
*����˵������ʼ��Э��ջ����
*����˵����nStackType Э��ջ����
*����ֵ��  -1 ��ʾЭ��ջ�����㣬������ʾջID
*/
int  PT_Initialize(int nStackType);
/*
*�������ƣ�PT_InitializeEx
*����˵������ʼ��Э��ջ����
*����˵����pParam Э�����ջָ��
*����ֵ��  -1 ��ʾʧ�ܣ������ɹ�
*/
int  PT_InitializeEx(TStackParam *pParam);

/*
*�������ƣ�PT_Uninitialize
*����˵�����ͷ�Э��ջ
*����˵����nStackID ջID
*����ֵ��  ��
*/
void PT_Uninitialize(int nStackID);
/*
*�������ƣ�PT_UninitializeEx
*����˵�����ͷ�Э��ջ
*����˵����pParam Э�����ջָ��
*����ֵ��  ��
*/
void PT_UninitializeEx(TStackParam *pParam);

/*
*�������ƣ�PT_GetRecvData
*����˵������ȡ���ջ��������
*����˵����lpBuff   ���ݻ���ָ��
*          nBuffLen ���ݻ��泤��
*          nStackID ջID
*����ֵ��  0 ��ʾ����ɹ���������ʾʧ��
*/
int PT_GetRecvData(uchar *lpBuff, int nBuffLen, int nStackID);

/*
*�������ƣ�PT_RecvData
*����˵����������յ�������Э�����ݽṹ
*����˵����nStackID ջID
*          data     ���յ�����
*����ֵ��  -1 ��ʾû���յ���֡���ݣ�������ʾ������֡���ݵ���Ϣ����
*/
int  PT_RecvData(int nStackID, uchar data);

/*
*�������ƣ�PT_RecvDataEx
*����˵����������յ�������Э�����ݽṹ
*����˵����pParam Э�����ջָ��
*          data     ���յ�����
*����ֵ��  -1 ��ʾû���յ���֡���ݣ�������ʾ������֡���ݵ���Ϣ����
*/
int  PT_RecvDataEx(TStackParam *pParam, uchar data);


/*
*�������ƣ�PT_DecodeRecvData
*����˵��������Э��ջ���յ�������Э�����ݽṹ
*����˵����pPackage �ṹ��ָ��
*          nStackID ջID
*����ֵ��  0 ��ʾ����ɹ���������ʾʧ��
*/
int PT_DecodeRecvData(void *pPackage, int nStackID);

/*
*�������ƣ�PT_DecodeRecvDataEx
*����˵��������Э��ջ���յ�������Э�����ݽṹ
*����˵����pPackage �ṹ��ָ��
*          pParam Э�����ջָ��
*����ֵ��  0 ��ʾ����ɹ���������ʾʧ��
*/
int PT_DecodeRecvDataEx(void *pPackage, TStackParam *pParam);

/*
*�������ƣ�PT_DecodeType
*����˵����������յ���֡���ݵ���Ϣ����
*����˵����buf    �������ݻ���
*          len    �������ݳ���
*          nStackType Э��ջ���� 
*����ֵ��  -1 ��ʾ��Ч����Ϣ���ͣ�������ʾ����������
*/
int PT_DecodeType(const uchar* lpBuff, int nBuffLen, int nStackType);

/*
*�������ƣ�PT_DecodePackage
*����˵�������������֡������Э�����ݽṹ
*����˵����pPackage  ���ݽṹָ��
*          lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          nStackType Э��ջ����
*����ֵ��  0 ��ʾ����ɹ���������ʾʧ��
*/
int PT_DecodePackage(void *pPackage, uchar *lpBuff, int nBuffLen, int nStackType);

/*
*�������ƣ�PT_EncodePackage
*����˵������������
*����˵����lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          pMsgData  Э�����ݽṹָ��
*          bDevice   �Ƿ��豸ջ���豸ջ����Э��ͷ����"BD"��1��ʾ��PCջ����"CC"��0��ʾ
*����ֵ��  -1 ��ʾʧ�ܣ�������ʾ���ݳ���
*/
int PT_EncodePackage(uchar *lpBuff, int nBuffLen, 
                       void *pPackage, int nType, char bDevice);

#ifdef WIN32
/*
*�������ƣ�PT_GetPackage
*����˵����������Ϣ���ͻ�ȡ��Ӧ�����ݽṹ��,�˺�����������WIN32ƽ̨
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�޴��������ݽṹ����������ʾ���ݽṹ��ָ�룬�ṹ���ݰ�ʹ�����Ӧʹ��free�ͷ�
*/
void* PT_GetPackage(int nType);

#endif // end WIN32 define

#endif //end  H_PT_DEFINE_H  define






















