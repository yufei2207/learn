/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name��     pt_jt808.h
* Abstract�� JT/T808 protocol 
* 
* Version�� 1.0
* Author��  yfwang
* Date��    2012/07/26
*
* history   :
*	     when            who    	          what:
*      -------------   --------------    -------------------
*      Jul 26, 2012   yfwang                create
*
*/
#ifndef H_PT_JT808_H
#define H_PT_JT808_H

#include "../pt_define.h"

//	//

#define BCD2BIN(a)	(((((a)>>4) & 0x0F)*10) + ((a) & 0x0F))

#define BIN2BCD(a)	((((a)/10)<<4) + ((a)%10))

#ifdef  PT_ENABLE_JT808
//////////////////////////////////////////////////////////////////////////
//interface define
//////////////////////////////////////////////////////////////////////////

#define JT808_ISTYPE(TYPE)   (TYPE >= JT808_ZERO && TYPE < JT808_LAST)
extern unsigned char JT808_MakeCRC(uchar *buf, int nLen);

extern  int JT808_Translate(uchar *buf, int nLen, uchar bEncode, int bufsize);

extern  int JT808_DecodeHead(const uchar* buf, int len);
extern  int JT808_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType);

extern  FUNC_ENCODER JT808_GetEncoder(int nType);
extern  FUNC_DECODER JT808_GetDecoder(int nType);

extern  int JT808_RecvData(TStackParam *pParam, uchar data);
extern  int JT808_RecvDataEx(TStackParam *pParam, uchar data);
extern  int JT808_EncodePackage(uchar *lpBuff, int nBuffLen, void *pPackage, 
                               int nMsgType, char bDevice);
extern  int JT808_DecodePackage(void *pPackage, int nMsgType, uchar *lpBuff, int nBuffLen);

#ifdef WIN32
    extern void* JT808_GetPackage(int nType);
#endif
//////////////////////////////////////////////////////////////////////////
//data struct define
//////////////////////////////////////////////////////////////////////////
typedef enum TJT808DEFINE
{  
    //��������
    JT808_DEF_MAX_NUM_PARAMETER    = 0x50, //�ն˲����������ֵ     20130121 modify by xlsong
    JT808_DEF_MAX_NUM_PHONEITEM    = 10, //�绰�����������
    JT808_DEF_MAX_NUM_USERPOSITION = 10, //λ���ϱ�����û���
    JT808_DEF_MAX_NUM_EVENT        = 10, //�¼����������
    JT808_DEF_MAX_NUM_KEY          = 10, //����������
    JT808_DEF_MAX_NUM_SERVICE      = 10, //��Ϣ���������
    JT808_DEF_MAX_NUM_REGION       = 24, //���������
    JT808_DEF_MAX_NUM_POLYGON_POINT= 20, //�����������󶥵���
    JT808_DEF_MAX_NUM_ROUTE_CORNER = 20, //·�����յ���
    JT808_DEF_MAX_NUM_INDEX_MEDIA  = 29, //��ý�����ݼ��������
    JT808_DEF_MAX_NUM_SENSOR_TYRE  = 32, //�����̥��������
    JT808_DEF_MAX_NUM_SENSOR_TEMP  = 2,  //����¶ȴ�������
    JT808_DEF_MAX_NUM_SENSOR_OIL   = 2,  //�����ѹ��������
    JT808_DEF_MAX_NUM_SENSOR_POSI  = 2,  //���λ�ƴ�������
    JT808_DEF_MAX_NUM_MEDIAID      = 128,//����ϱ�ý��ID��
    //Ӧ��
    JT808_DEF_RESULT_SUCCESS      = 0,//�ɹ�/ȷ��
    JT808_DEF_RESULT_FAIL         = 1,//ʧ��
    JT808_DEF_RESULT_ERROR        = 2,//��Ϣ����
    JT808_DEF_RESULT_NOTSUPPORT   = 3,//��֧��
	JT808_DEF_RESULT_CONFIRM_ALARM= 4,//ȷ�ϱ���
    //Э������
    JT808_DEF_PROTOCOL_GSM       = 0,//����GSM��JT808Э��
    JT808_DEF_PROTOCOL_BD        = 1,//���ڱ�����JT808Э��
    //�ն˲�������
    JT808_DEF_PARAM_FIRST       = 0x0001,//������ʼ
    JT808_DEF_PARAM_KEEPALIVE   = 0x0001,//�ն��������ͼ������λΪ��S
    JT808_DEF_PARAM_TCP_TIMEOUT = 0x0002,//TCP��ϢӦ��ʱʱ�䣬��λΪ��S
    JT808_DEF_PARAM_TCP_RESEND  = 0x0003,//TCP��Ϣ�ش�����
    JT808_DEF_PARAM_UDP_TIMEOUT = 0x0004,//UDP��ϢӦ��ʱʱ�䣬��λΪ��S
    JT808_DEF_PARAM_UDP_RESEND  = 0x0005,//UDP��Ϣ�ش�����
    JT808_DEF_PARAM_SMS_TIMEOUT = 0x0006,//SMS��ϢӦ��ʱʱ�䣬��λΪ��S
    JT808_DEF_PARAM_SMS_RESEND  = 0x0007,//SMS��Ϣ�ش�����
    JT808_DEF_PARAM_BD_TIMEOUT  = 0x0008,//������ϢӦ��ʱʱ�䣬��λΪ��S
    JT808_DEF_PARAM_BD_RESEND   = 0x0009,//������Ϣ�ش�����
    JT808_DEF_PARAM_MASTER_APN  = 0x0010,//��������APN������ͨѶ���ŷ��ʵ�.
                                         //��������ʽΪCDMA����ô�ΪPPP���ź���
    JT808_DEF_PARAM_MASTER_USERNAME= 0x0011,//������������ͨѶ�����û���
    JT808_DEF_PARAM_MASTER_PASSWORD= 0x0012,//������������ͨѶ��������
    JT808_DEF_PARAM_MASTER_ADDRESS = 0x0013,//��������IP��ַ��������
    JT808_DEF_PARAM_SLAVE_APN      = 0x0014,//��������APN������ͨѶ���ŷ��ʵ�.
                                             //��������ʽΪCDMA����ô�ΪPPP���ź���
    JT808_DEF_PARAM_SLAVE_USERNAME = 0x0015,//������������ͨѶ�����û���
    JT808_DEF_PARAM_SLAVE_PASSWORD = 0x0016,//������������ͨѶ��������
    JT808_DEF_PARAM_SLAVE_ADDRESS  = 0x0017,//��������IP��ַ��������
    JT808_DEF_PARAM_TCP_PORT       = 0x0018,//������TCP�˿�
    JT808_DEF_PARAM_UDP_PORT       = 0x0019,//������UDP�˿�
	JT808_DEF_IC_MASTER_ADDRESS    = 0x1A,//��·����֤ IC ����֤�������� IP ��ַ������------201301��Э��   by xlsong
	JT808_DEF_IC_TCP_PORT          = 0x1B,//��·����֤ IC ����֤�������� TCP �˿�------201301��Э��        by xlsong
	JT808_DEF_IC_UDP_PORT          = 0x1C,//��·����֤ IC ����֤�������� UDP �˿�------201301��Э��		 by xlsong
	JT808_DEF_IC_SLAVE_ADDRESS    = 0x1D,//��·����֤ IC ����֤���ݷ����� IP ��ַ���������˿�ͬ��������------201301s��Э�� by xlsong

    JT808_DEF_PARAM_MASTER_BDID    = 0x001E,//�������������û���IC������ 
    JT808_DEF_PARAM_SLAVE_BDID     = 0x001F,//�������������û���IC������ 
		
    JT808_DEF_PARAM_REPORT_STRATEGY    = 0x0020,//λ�û㱨���ԡ�
                                            //0����ʱ�㱨��1������㱨��2����ʱ�Ͷ���㱨
    JT808_DEF_PARAM_REPORT_MODE    = 0x0021,//λ�û㱨������0������Acc״̬��
                                            //1�����ݵ�¼״̬��Acc״̬�����жϵ�¼״̬������¼�ٸ���Acc״̬
    JT808_DEF_PARAM_INTERVAL_NODRIVER= 0x0022,//��ʻԱδ��¼�㱨ʱ��������λΪ��S��>0
    JT808_DEF_PARAM_INTERVAL_RESTING = 0x0027,//����ʱ�㱨ʱ��������λΪ��S��>0
    JT808_DEF_PARAM_INTERVAL_ALARM     = 0x0028,//��������ʱ�㱨ʱ��������λΪ��S��>0 
    JT808_DEF_PARAM_INTERVAL_DEFAULT = 0x0029,//ȱʡʱ��㱨�������λΪ��S��>0
    JT808_DEF_PARAM_DISTANCE_DEFAULT = 0x002C,//ȱʡ����㱨�������λΪ��m,>0
    JT808_DEF_PARAM_DISTANCE_NODRIVER= 0x002D,//��ʻԱδ��¼�㱨����������λΪ��m��>0
    JT808_DEF_PARAM_DISTANCE_RESTING = 0x002E,//����ʱ�㱨����������λΪ��m��>0 
    JT808_DEF_PARAM_DISTANCE_ALARM   = 0x002F,//��������ʱ�㱨����������λΪ��m��>0  
    JT808_DEF_PARAM_TURNINGANGLE     = 0x0030,//�յ㲹���Ƕȣ�<180��
	JT808_DEF_PARAM_ELEC_FENCE_RADII = 0x0031,//����Χ���뾶���Ƿ�λ����ֵ������λΪ�� ---��Э��
    JT808_DEF_PARAM_PHONE_PLATFORM = 0x0040,//���ƽ̨�绰����
    JT808_DEF_PARAM_PHONE_RESET    = 0x0041,//��λ�绰���롣���ô˺���绰�����ն˵绰���ն˸�
    JT808_DEF_PARAM_PHONE_RESTORE  = 0x0042,//�ָ��������õ绰����
    JT808_DEF_PARAM_PHONE_SMS      = 0x0043,//���ƽ̨SMS�绰����
    JT808_DEF_PARAM_PHONE_SMS_SOS  = 0x0044,//�����ն�SMS�ı���������
    JT808_DEF_PARAM_ACCEPT_MODE    = 0x0045,//�ն˵绰�������ԡ�0����������
                                            //1 Acc ONʱ�Զ�������OFFʱ�ֶ�����
    JT808_DEF_PARAM_TALK_LIMIT_ONCE= 0x0046,//ÿ��ͨ���ʱ�䡣��λΪ��S��
                                            //0 Ϊ������ͨ����0xFFFFFFFFΪ������
    JT808_DEF_PARAM_TALK_LIMIT_MONTH= 0x0047,//�����ͨ��ʱ�䡣��λΪ��S��
                                            //0 Ϊ������ͨ����0xFFFFFFFFΪ������
    JT808_DEF_PARAM_PHONE_MONITOR  = 0x0048,//�����绰����
    JT808_DEF_PARAM_PHONE_PRIVATESMS= 0x0049,//���ƽ̨��Ȩ���ź���
    JT808_DEF_PARAM_WARN_FLAG_IGNORE= 0x0050,//���������֣���λ����Ϣ�㱨��Ϣ�еı�����ʶ
                                            //���Ӧ����ӦλΪ1����Ӧλ����������
    JT808_DEF_PARAM_WARN_FLAG_SMS   = 0x0051,//���������ı�SMS���أ���λ����Ϣ�㱨��Ϣ��
                                            //�ı�����־���Ӧ����ӦλΪ1����Ӧ����ʱ�����ı�SMS
    JT808_DEF_PARAM_WARN_FLAG_CAMERA= 0x0052,//�������㿪�أ���λ����Ϣ�㱨��Ϣ�еı�����־���
                                            //Ӧ����ӦλΪ1����Ӧ����ʱ����ͷ����
    JT808_DEF_PARAM_WARN_FLAG_SAVE  = 0x0053,//��������洢��־����λ����Ϣ�㱨��Ϣ�еı�����־
                                               //���Ӧ����ӦλΪ1����Ӧ����ʱ�������Ƭ���д洢��
                                               //����ʵʱ�ϴ�
	JT808_DEF_PARAM_KEY_FLAG       = 0x0054,//�ؼ���־����λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ��//20130121 add by xlsong
											//��ӦλΪ 1 ����Ӧ����Ϊ�ؼ�����
    JT808_DEF_PARAM_MAX_SPEED      = 0x0055,//����ٶȣ���λΪǧ��/Сʱ��Km/h��
    JT808_DEF_PARAM_MAX_OVERSPEED  = 0x0056,//���ٳ���ʱ�䣬��λΪ��S
    JT808_DEF_PARAM_MAX_DRIVING_ONCE= 0x0057,//������ʻʱ�����ޣ���λΪ��S
    JT808_DEF_PARAM_MAX_DRIVING_DAY= 0x0058,//�����ۼƼ�ʻʱ�����ޣ���λΪ��S
    JT808_DEF_PARAM_MIN_REST       = 0x0059,//��С��Ϣʱ�䣬��λΪ��S
    JT808_DEF_PARAM_MAX_REST       = 0x005A,//�ͣ��ʱ�䣬��λΪ��S
	JT808_DEF_PARAM_DIF_OVERSPEED  = 0x005B,//���ٱ���Ԥ����ֵ����λΪ 1/10Km/h ---Ver201301
	JT808_DEF_PARAM_DIF_TIREDRIVING= 0x005C,//ƣ�ͼ�ʻԤ����ֵ����λΪ�루s����>0 ---Ver201301

	JT808_DEF_PARAM_ALARM_HIT      = 0x005D,//��ײ�����������ã� ---Ver201301
											//b7-b0�� ��ײʱ�䣬��λ 4ms�� 
											//b15-b8����ײ���ٶȣ���λ 0.1g�����÷�Χ�ڣ�0-79 ֮�䣬Ĭ��Ϊ10��
    JT808_DEF_PARAM_ALARM_TURNON    = 0x005E,//�෭�����������ã� ---Ver201301
											//�෭�Ƕȣ���λ 1 �ȣ�Ĭ��Ϊ 30 �ȡ�
	JT808_DEF_PARAM_TIMING_CTRL     = 0x0064,//��ʱ���տ��ƣ� ---Ver201301
	JT808_DEF_PARAM_FIXEDDIS_CTRL   = 0x0065,//�������տ���		---Ver201301
	JT808_DEF_PARAM_MEDIA_QUALITY  = 0x0070,//ͼ��/��Ƶ������1~10��1Ϊ���
    JT808_DEF_PARAM_MEDIA_BRIGHTENESS= 0x0071,//���ȡ�0~255
    JT808_DEF_PARAM_MEDIA_CONTRAST  = 0x0072,//�Աȶȡ�0~127 
    JT808_DEF_PARAM_MEDIA_SATURATION= 0x0073,//���Ͷȡ�0~127
    JT808_DEF_PARAM_MEDIA_CHROMA    = 0x0074,//ɫ�ȡ�0~255 
    JT808_DEF_PARAM_MILEAGE         = 0x0080,//������̱������1/10Km
    JT808_DEF_PARAM_PROVINCEID      = 0x0081,//�������ڵ�ʡ��ID
    JT808_DEF_PARAM_CITYID          = 0x0082,//�������ڵ�����ID 
    JT808_DEF_PARAM_LICENSEPLATE    = 0x0083,//���ƺ���
    JT808_DEF_PARAM_COLOR           = 0x0084,//������ɫ������JT/T415��2006��5.4.12�Ĺ涨 
	JT808_DEF_PARAM_GNSS_POSMODE    =0x0090,//GNSS ��λģʽ���������£� ---Ver201301
											//bit0��0������ GPS ��λ�� 1������ GPS ��λ�� 
											//bit1��0�����ñ�����λ�� 1�����ñ�����λ�� 
											//bit2��0������ GLONASS ��λ�� 1������ GLONASS ��λ�� 
											//bit3��0������ Galileo ��λ�� 1������ Galileo ��λ��
	JT808_DEF_PARAM_GNSS_BOUTERAYE    = 0x0091,//GNSS ������   ---Ver201301
	JT808_DEF_PARAM_GNSS_OUTOUT_HZ    = 0x0092,//GNSS ģ����ϸ��λ�������Ƶ��  ---Ver201301
	JT808_DEF_PARAM_GNSS_COLLECT_HZ   = 0x0093,//GNSS ģ����ϸ��λ���ݲɼ�Ƶ��  ---Ver201301
	JT808_DEF_PARAM_GNSS_UPLOAD_MODE  = 0x0094,//GNSS ģ����ϸ��λ�����ϴ���ʽ  ---Ver201301
	JT808_DEF_PARAM_GNSS_UPLOAD_SET   = 0x0095,//GNSS ģ����ϸ��λ�����ϴ�����  ---Ver201301
	JT808_DEF_PARAM_CAN_CHL1_COLLECT  = 0x0100,//CAN ����ͨ�� 1 �ɼ�ʱ����(ms)  ---Ver201301
	JT808_DEF_PARAM_CAN_CHL1_UPLOAD   = 0x0101,//CAN ����ͨ�� 1 �ϴ�ʱ����(ms) ---Ver201301
	JT808_DEF_PARAM_CAN_CHL2_COLLECT  = 0x0102,//CAN ����ͨ�� 2 �ɼ�ʱ����(ms) ---Ver201301
	JT808_DEF_PARAM_CAN_CHL2_UPLOAD   = 0x0103,//CAN ����ͨ�� 2 �ϴ�ʱ����(ms) ---Ver201301
	JT808_DEF_PARAM_CAN_ID_SET_FIRST  = 0x0110,//CAN ���� ID �����ɼ����� ---Ver201301
    JT808_DEF_PARAM_CAN_ID_SET_LAST   = 0x01FF,//CAN ���� ID �����ɼ����� ---Ver201301

    JT808_DEF_PARAM_POSI_STANDARD     = 0xF011,//λ�Ʊ�����׼
	JT808_DEF_PARAM_TEMP_MIN		  = 0xF012,	//�¶ȱ�������
	JT808_DEF_PARAM_TEMP_MAX		  = 0xF013,	//�¶ȱ�������
	JT808_DEF_PARAM_TYRE_STANDARD     = 0xF014,	//̥ѹ��׼

    JT808_DEF_PARAM_CAMERA_NUMBER     = 0xF100,//����ͷ��װ��
    JT808_DEF_PARAM_OILBOX_LENGTH     = 0xF101,//���䳤��
    JT808_DEF_PARAM_OILBOX_WIDTH      = 0xF102,//������
    JT808_DEF_PARAM_OILBOX_HEIGHT     = 0xF103,//����߶�
    JT808_DEF_PARAM_OILSENSOR_HEIGHT  = 0xF104,//��ѹ�������߶�
    JT808_DEF_PARAM_OILSENSOR_WARNVALUE  = 0xF105,//��ѹ�쳣�仯��ֵ

    JT808_DEF_PARAM_TYRE_TYPE         = 0xF111,//̥ѹ���������� �� 0:�ޣ�1:ΰӯ;2:ά��������;3:ά��������
    JT808_DEF_PARAM_TYRE_ID_FIRST     = 0xF112,//�����ǰ����̥ѹ������ID�� 0����ʾδ��װ��������������ID
    JT808_DEF_PARAM_TYRE_ID_LAST      = JT808_DEF_PARAM_TYRE_ID_FIRST + JT808_DEF_MAX_NUM_SENSOR_TYRE,//�ұ������̥ѹ������ID�� 0����ʾδ��װ��������������ID
    JT808_DEF_PARAM_LAST              = JT808_DEF_PARAM_TYRE_ID_LAST,	//��������

	
	//GNSSģ������
	JT808_DEF_GPS_SERVICE = 1<<0,//֧�� GPS ��λ
	JT808_DEF_BD_SERVICE  = 1<<1,//֧�� BD ��λ
	JT808_DEF_GLONASS_SERVICE = 1<<2,//֧�� GLONASS ��λ
	JT808_DEF_GALILEO_SERVICE = 1<<3,//֧�� Galileo ��λ

	//ͨ��ģ������
	JT808_DEF_GPRS_SERVICE     = 1<<0,//֧��GPRSͨ��
	JT808_DEF_CDMA_SERVICE     = 1<<1,//֧��CDMAͨ��
	JT808_DEF_TDSCDMA_SERVICE  = 1<<2,//֧��TD-SCDMAͨ��
	JT808_DEF_WCDMA_SERVICE    = 1<<3,//֧��WCDMAͨ��
	JT808_DEF_CDMA2000_SERVICE = 1<<4,//֧��CDMA2000ͨ��
	JT808_DEF_TD_LTE_SERVICE   = 1<<5,//֧��TD-LTEͨ��
	JT808_DEF_OHTER_SERVICE    = 1<<7,//֧������ͨ�ŷ�ʽ

	//�ն˿���ָ��
    JT808_DEF_TERMINAL_UPDATE     = 1,//��������  TUpdateInfo
    JT808_DEF_TERMINAL_REDIRECT   = 2,//�����ն�����ָ��������  TPlatform
    JT808_DEF_TERMINAL_SHUTDOWN   = 3,//�ն˹ػ�
    JT808_DEF_TERMINAL_RESET      = 4,//�ն˸�λ
    JT808_DEF_TERMINAL_RESTORE    = 5,//�ն˻ָ���������
    JT808_DEF_TERMINAL_DISCONNECT = 6,//�ر�����ͨѶ
    JT808_DEF_TERMINAL_MUTE       = 7,//�ر���������ͨѶ
    //λ����Ϣ����
    JT808_DEF_STATE_ACC           = 1 << 0,  //ACC ��0���أ�1����
    JT808_DEF_STATE_POSITION      = 1 << 1,  //��λ��0��δ��λ��1���Ѷ�λ
    JT808_DEF_STATE_SOUTH         = 1 << 2,  //��γ, 0����γ��1����γ
    JT808_DEF_STATE_WEST          = 1 << 3,  //������0��������1������
    JT808_DEF_STATE_WORKING       = 1 << 4,  //��Ӫ״̬�� 0��ͣ�ˣ�1����Ӫ
    JT808_DEF_STATE_ENCRYPT       = 1 << 5,  //��γ�ȼ��ܣ�0:δ���ܣ�1:�Ѽ���
	JT808_DEF_STATE_CARRY_EMPTY   = 0, //�ػ����ճ�
	JT808_DEF_STATE_CARRY_HALF    = 1 << 8, //�ػ�������
	JT808_DEF_STATE_CARRY_RESERVED= 2 << 8, //�ػ�������
	JT808_DEF_STATE_CARRY_FULL    = 3 << 8, //�ػ�������
    JT808_DEF_STATE_OIL           = 1 << 10, //��·,0���أ�1����
    JT808_DEF_STATE_POWER         = 1 << 11, //��·,0���أ�1����
    JT808_DEF_STATE_LOCK          = 1 << 12, //0�����Ž�����1�����ż���
	JT808_DEF_STATE_DOOR1         = 1 << 13, //0���� 1 �أ�1���� 1 ����ǰ�ţ�//��Э��
	JT808_DEF_STATE_DOOR2         = 1 << 14, //0���� 2 �أ�1���� 2 �������ţ�//��Э�� 
	JT808_DEF_STATE_DOOR3         = 1 << 15, //0���� 3 �أ�1���� 3 �������ţ�//��Э��
	JT808_DEF_STATE_DOOR4         = 1 << 16, //0���� 4 �أ�1���� 4 ������ʻϯ�ţ�//��Э��
	JT808_DEF_STATE_DOOR5         = 1 << 17, //0���� 5 �أ�1���� 5 �����Զ��壩  //��Э��
	JT808_DEF_STATE_GPS           = 1 << 18, //0��δʹ�� GPS ���ǽ��ж�λ��1��ʹ�� GPS ���ǽ��ж�λ//��Э��  
	JT808_DEF_STATE_BD            = 1 << 19, //0��δʹ�ñ������ǽ��ж�λ��1��ʹ�ñ������ǽ��ж�λ//��Э��
	JT808_DEF_STATE_GLONASS       = 1 << 20, //0��δʹ�� GLONASS ���ǽ��ж�λ��1��ʹ�� GLONASS ���ǽ��ж�λ //��Э��
	JT808_DEF_STATE_GALILEO       = 1 << 21, //0��δʹ�� Galileo ���ǽ��ж�λ��1��ʹ�� Galileo ���ǽ��ж�λ  //��Э��

    JT808_DEF_WARN_SOS        = 1 << 0, //���������������������غ󴥷����յ�Ӧ�������
    JT808_DEF_WARN_OVERSPEED  = 1 << 1, //���ٱ�������־λά���������������
    JT808_DEF_WARN_TIRED      = 1 << 2, //ƣ�ͼ�ʻ����־λά���������������
    JT808_DEF_WARN_URGEN      = 1 << 3, //Σ��Ԥ������־λά���������������
    JT808_DEF_WARN_GNSS_1     = 1 << 4, //GNSSģ�鷢�����ϣ���־λά���������������
    JT808_DEF_WARN_GNSS_2     = 1 << 5, //GNSS����δ�ӻ򱻼��ϣ���־λά���������������
    JT808_DEF_WARN_GNSS_3     = 1 << 6, //GNSS���߶�·����־λά���������������
    JT808_DEF_WARN_POWER_LACK = 1 << 7, //�ն�����ԴǷѹ����־λά���������������
    JT808_DEF_WARN_POWER_OFF  = 1 << 8, //�ն�����Դ���磬��־λά���������������
    JT808_DEF_WARN_LCD        = 1 << 9, //�ն�LCD������ʾ�����ϣ���־λά���������������
    JT808_DEF_WARN_TTS        = 1 << 10, //TTSģ����ϣ���־λά���������������
    JT808_DEF_WARN_CAMERA     = 1 << 11, //����ͷ���ϣ���־λά���������������
	JT808_DEF_WARN_IC_MODULE  = 1 << 12, //1����·����֤ IC ��ģ�����
	JT808_DEF_WARN_EARLY_OVERSPEED = 1 << 13, //1������Ԥ�� 
	JT808_DEF_WARN_EARLY_TIRED     = 1 << 14, //1��ƣ�ͼ�ʻԤ��
    JT808_DEF_WARN_GSM             = 1 << 15, //GSMģ����ϣ���־λά���������������
    JT808_DEF_WARN_BD              = 1 << 16, //����ģ����ϣ���־λά���������������
    JT808_DEF_WARN_OVERTIME_DRIVE  = 1 << 18, //�����ۼƼ�ʻ��ʱ����־λά���������������
    JT808_DEF_WARN_OVERTIME_PARK   = 1 << 19, //��ʱͣ������־λά���������������
    JT808_DEF_WARN_AREA       = 1 << 20, //���������յ�Ӧ�������
    JT808_DEF_WARN_ROUTE      = 1 << 21, //����·�ߣ��յ�Ӧ�������
    JT808_DEF_WARN_DRIVING_TIME = 1 << 22, //·����ʻʱ�䲻��/�������յ�Ӧ�������
    JT808_DEF_WARN_ROUTE_OFFSET = 1 << 23, //·��ƫ�뱨������־λά���������������
    JT808_DEF_WARN_VSS        = 1 << 24, //����Vss���ϣ���־λά���������������
    JT808_DEF_WARN_OIL        = 1 << 25, //���������쳣����־λά���������������
    JT808_DEF_WARN_THEFT      = 1 << 26, //������������־λά���������������
    JT808_DEF_WARN_LIGHT      = 1 << 27, //�����Ƿ�����յ�Ӧ�������
    JT808_DEF_WARN_MOVE       = 1 << 28, //�����Ƿ���λ���յ�Ӧ�������
    JT808_DEF_WARN_HIT        = 1 << 29, //��ײԤ�� ����־λά��������������� //��Э��
	JT808_DEF_WARN_TURNON     = 1 << 30, //�෭Ԥ�� ��־ά���������������   //��Э��
	JT808_DEF_WARN_OPENDOOR   = 1 << 31, //�Ƿ����ű������ն�δ��������ʱ�����жϷǷ����ţ� //��Э��
	
	//��չ�ź�״̬λ
	JT808_DEF_SIGNAL_DIPPED_LIGHT    = 1<<0,//������ź�
	JT808_DEF_SIGNAL_HIGH_BEAM       = 1<<1,//Զ����ź�
	JT808_DEF_SIGNAL_RIGHT_TURNSIGNAL= 1<<2,//��ת����ź�
	JT808_DEF_SIGNAL_LEFT_TURNSIGNAL = 1<<3,//��ת����ź�
	JT808_DEF_SIGNAL_BRAKING         = 1<<4,//�ƶ��ź�
	JT808_DEF_SIGNAL_REVERSE         = 1<<5,//�����ź�
	JT808_DEF_SIGNAL_FOGLIGHT        = 1<<6,//����ź�
	JT808_DEF_SIGNAL_CLEARRANCE      = 1<<7,//ʾ����
	JT808_DEF_SIGNAL_HORN            = 1<<8,//�����ź�
	JT808_DEF_SIGNAL_AIRCONDITION    = 1<<9,//�յ�״̬
	JT808_DEF_SIGNAL_NEUTRAL         = 1<<10,//�յ��ź� 
	JT808_DEF_SIGNAL_RETARDER        = 1<<11,//����������
	JT808_DEF_SIGNAL_ABS             = 1<<12,//ABS ����
	JT808_DEF_SIGNAL_HEATER          = 1<<13,//����������
	JT808_DEF_SIGNAL_CLUTCH          = 1<<14,//�����״̬
	//IO״̬
	JT808_DEF_IO_DEEP_SLEEP = 1<<0,//�������״̬
	JT808_DEF_IO_SLEEP      = 1<<1,//����״̬
	//������Ϣ
    JT808_DEF_INFO_MILEAGE    = 1 << 1,    //���
    JT808_DEF_INFO_OIL        = 1 << 2,    //����
    JT808_DEF_INFO_SPEED      = 1 << 3,    //��ʻ��¼�ǻ�ȡ���ٶ�
    JT808_DEF_INFO_OVERSPEED  = 1 << 4,    //���ٱ���������Ϣ
    JT808_DEF_INFO_AREA_ROUTE = 1 << 5,    //��������/·�߱���������Ϣ
    JT808_DEF_INFO_DRIVING_TIME= 1 << 6,   //·����ʻʱ�䲻��/��������������Ϣ
	JT808_DEF_INFO_EVENTID     = 1<<8,//��Ҫ�˹�ȷ�ϱ����¼��� ID
	JT808_DEF_INFO_CARSIGNAL   = 1<<9,//��չ�����ź�״̬λ
	JT808_DEF_INFO_IOSTATUS    = 1<<10,//IO״̬λ
	JT808_DEF_INFO_AD          = 1<<11,//ģ����
	JT808_DEF_INFO_WLSIGNAL    = 1<<12,//����ͨ�������ź�ǿ��
	JT808_DEF_INFO_GNSSSTARS   = 1<<13,//GNSS ��λ������
	JT808_DEF_INFO_ADDDATALEN  = 1<<14,//�����Զ�����Ϣ����
    //�Զ��帽����Ϣ
    JT808_DEF_INFO1_OIL        = 1 << 0,    //����
    JT808_DEF_INFO1_TYRE       = 1 << 1,    //̥ѹ
    JT808_DEF_INFO1_TEMP       = 1 << 2,    //�¶�
    JT808_DEF_INFO1_POSI       = 1 << 3,    //λ��
    JT808_DEF_INFO1_CARRY      = 1 << 4,    //�ػ�״̬
    JT808_DEF_INFO1_SERVERFLAG = 1 << 5,    //��������ʶ��Ϣ,�Զ��帽����Ϣ

    //�ı��·���־
    JT808_DEF_TEXT_URGENCY     = 1 << 0,//����
    JT808_DEF_TEXT_LED         = 1 << 2,//�ն���ʾ����ʾ
    JT808_DEF_TEXT_TTS         = 1 << 3,//�ն�TTS����
    JT808_DEF_TEXT_AD          = 1 << 4,//�������ʾ
    JT808_DEF_TEXT_CAN         = 1 << 5,//CNA������Ϣ

    //�¼���������
    JT808_DEF_EVENT_REMOVEALL  = 0,//ɾ���ն����������¼���������󲻴�����ֽ�
    JT808_DEF_EVENT_UPDATE     = 1,//�����¼�
    JT808_DEF_EVENT_APPEND     = 2,//׷���¼�
    JT808_DEF_EVENT_MODIFY     = 3,//�޸��¼�
    JT808_DEF_EVENT_REMOVE     = 4,// ɾ���ض������¼�,֮���¼�����������¼�����

    //�㲥��������
    JT808_DEF_SERVICE_REMOVEALL  = 0,//ɾ���ն����������¼���������󲻴�����ֽ�
    JT808_DEF_SERVICE_UPDATE     = 1,//���²˵�
    JT808_DEF_SERVICE_APPEND     = 2,//׷�Ӳ˵�
    JT808_DEF_SERVICE_MODIFY     = 3,//�޸Ĳ˵�

    //��������ָ��
    JT808_DEF_CAR_CTRL_LOCK      = 1 << 0, //0 ���Ž�����1 ���ż���
    JT808_DEF_CAR_CTRL_YD        = 1 << 7, //0 �ָ��͵磻1 �ж��͵磬�Զ���

    //�绰����������
    JT808_DEF_PHONEBOOK_REMOVEALL  = 0,//ɾ���ն������д洢����ϵ�ˣ�������󲻴�����ֽ�
    JT808_DEF_PHONEBOOK_UPDATE     = 1,//��ʾ���µ绰��(ɾ���ն������е�ȫ����ϵ�˲�׷����Ϣ���е���ϵ��)
    JT808_DEF_PHONEBOOK_APPEND     = 2,//׷�ӵ绰��
    JT808_DEF_PHONEBOOK_MODIFY     = 3,//�޸ĵ绰��

    //������������
    JT808_DEF_REGION_UPDATE      = 0,//��������
    JT808_DEF_REGION_APPEND      = 1,//׷������
    JT808_DEF_REGION_MODIFY      = 2,//�޸�����
    //��������
    JT808_DEF_REGION_LIMIT_TIME    = 1 << 0,//����ʱ��
    JT808_DEF_REGION_LIMIT_SPEED   = 1 << 1,//����
    JT808_DEF_REGION_ENTER_DRIVER  = 1 << 2,//�����򱨾�����ʻԱ
    JT808_DEF_REGION_ENTER_PLATFORM= 1 << 3,//�����򱨾���ƽ̨
    JT808_DEF_REGION_LEAVE_DRIVER  = 1 << 4,//�����򱨾�����ʻԱ
    JT808_DEF_REGION_LEAVE_PLATFORM= 1 << 5,//�����򱨾���ƽ̨
    JT808_DEF_REGION_SOUTH_LAT     = 1 << 6,//0 ��γ��1 ��γ
    JT808_DEF_REGION_WEST_LONG     = 1 << 7,//0 ������1 ����
	JT808_DEF_REGION_OPENDOOR_SET  = 1 << 8,//0�������ţ�1����ֹ����
	JT808_DEF_REGION_ENTER_COMM    = 1 << 14,//0����������ͨ��ģ�飻1��������ر�ͨ��ģ��
	JT808_DEF_REGION_ENTER_GNSSPOS = 1 << 15,//0�������򲻲ɼ� GNSS ��ϸ��λ���ݣ�1��������ɼ� GNSS ��ϸ��λ����

    JT808_DEF_ROAD_LIMIT_TIME    = 1 << 0,//����ʱ��
    JT808_DEF_ROAD_LIMIT_SPEED   = 1 << 1,//����
    JT808_DEF_ROAD_SOUTH_LAT     = 1 << 2,//�����򱨾�����ʻԱ
    JT808_DEF_ROAD_WEST_LONG     = 1 << 3,//�����򱨾���ƽ̨

	//IC ����ȡ���
	JT808_DEF_READIC_SECCESS      = 0x00,//IC �������ɹ�    Ver201301
	JT808_DEF_READIC_SECRETKEY_ERR= 0x01,//����ʧ�ܣ�ԭ��Ϊ��Ƭ��Կ��֤δͨ��
	JT808_DEF_READIC_CARD_LOCKED  = 0x02,//����ʧ�ܣ�ԭ��Ϊ��Ƭ�ѱ�������
	JT808_DEF_READIC_CARD_DISPEARD= 0x03,//����ʧ�ܣ�ԭ��Ϊ��Ƭ���γ�
	JT808_DEF_READIC_CRC_ERR      = 0x04,//����ʧ�ܣ�ԭ��Ϊ����У�����
	
	//λ����������
	JT808_DEF_REPORT_NORMAL = 0,//����λ�������㱨
	JT808_DEF_REPORT_REPLACE = 1,//ä������
	
	//CAN ��־
	JT808_DEF_CAN_AVERAGE = 1<<0,//���ݲɼ���ʽ 0��ԭʼ���ݣ�1���ɼ������ƽ��ֵ��
	JT808_DEF_CAN_EXT_FRAME = 1<<1,//֡���� 0����׼֡��1����չ֡��


     //��ý���������Ͷ���
    JT808_DEF_MEDIA_TYPE_PICTURE = 0,//ý������,ͼ��
    JT808_DEF_MEDIA_TYPE_AUDIO   = 1,//ý������,��Ƶ
    JT808_DEF_MEDIA_TYPE_VIDEO   = 2,//ý������,��Ƶ
   
    JT808_DEF_MEDIA_CODE_JPEG    = 0,//ý�����
    JT808_DEF_MEDIA_CODE_TIF     = 1,//ý�����
    JT808_DEF_MEDIA_CODE_MP3     = 2,//ý�����
    JT808_DEF_MEDIA_CODE_WAV     = 3,//ý�����
    JT808_DEF_MEDIA_CODE_WMV     = 4,//ý�����
    
    
    JT808_DEF_PHOTO_SIZE_320_240 = 1,//�ֱ���
    JT808_DEF_PHOTO_SIZE_640_480 = 2,//�ֱ���
    JT808_DEF_PHOTO_SIZE_800_600 = 3,//�ֱ���
    JT808_DEF_PHOTO_SIZE_1280_768 = 4,//�ֱ���
    JT808_DEF_PHOTO_SIZE_176_144 = 5,//�ֱ���
    JT808_DEF_PHOTO_SIZE_352_288 = 6,//�ֱ���
    JT808_DEF_PHOTO_SIZE_704_288 = 7,//�ֱ���
    JT808_DEF_PHOTO_SIZE_704_576 = 8,//�ֱ���

    JT808_DEF_MEDIA_EVENT_PLATFORM = 0,//�¼������,ƽ̨�·�ָ��
    JT808_DEF_MEDIA_EVENT_TERMLY = 1,//�¼������,��ʱ
    JT808_DEF_MEDIA_EVENT_HEIST  = 2,//�¼������,���ٱ���
    JT808_DEF_MEDIA_EVENT_HIT    = 3,//�¼������,��ײ�෭
    JT808_DEF_MEDIA_EVENT_DOOR_OPEN= 4,//�¼������,����
    JT808_DEF_MEDIA_EVENT_DOOR_CLOSE= 5,//�¼������,����
    JT808_DEF_MEDIA_EVENT_DOOR_SPEED= 6,//�¼������,�����ɿ���أ�ʱ�ٴӣ�20 ���ﵽ���� 20 ����
    JT808_DEF_MEDIA_EVENT_DISTANCE= 7,//�¼������,����
    JT808_DEF_MEDIA_EVENT_OTHER  = 8,//�¼������,����
    
		
	JT808_DEF_OIL_STATE_NORMAL   = 0,//��������
    JT808_DEF_OIL_STATE_LACK	 = 1,//��������
    JT808_DEF_OIL_STATE_EXCEPTION= 2,//�����쳣
		
    JT808_DEF_TPRE_STATE_SIGNAL_NORMAL  = 0x80,  //��̥�ź�����
    JT808_DEF_TPRE_STATE_SIGNAL_LOST    = 0x81,   //��̥�źŶ�ʧ
    JT808_DEF_TPRE_STATE_PRESSURE_NORMAL= 0x00,   //��̥ѹ������
    JT808_DEF_TPRE_STATE_PRESSURE_LOW   = 0x02,   //��̥��ѹ
    JT808_DEF_TPRE_STATE_PRESSURE_HIGH  = 0x04,   //��̥��ѹ
    JT808_DEF_TPRE_STATE_TEMP_NORMAL    = 0x00,   //��̥�¶�����
    JT808_DEF_TPRE_STATE_TEMP_LOW       = 0x08,   //��̥����
    JT808_DEF_TPRE_STATE_TEMP_HIGH      = 0x10,   //��̥����
    JT808_DEF_TPRE_STATE_AIRLEAK_NORMAL = 0x00,   //��̥��©������
    JT808_DEF_TPRE_STATE_AIRLEAK_LOW    = 0x20,   //��̥����©��
    JT808_DEF_TPRE_STATE_AIRLEAK_HIGH   = 0x40,   //��̥����©��

	JT808_DEF_TEMP_STATE_NORMAL   = 0,//�¶�����
    JT808_DEF_TEMP_STATE_HIGH	  = 1,//�¶ȹ���
    JT808_DEF_TEMP_STATE_LOW	  = 2,//�¶ȹ���
    
	//����͸������  ---Ver201301
	JT808_DEF_DATA_GNSS = 0x00,//GNSS ģ����ϸ��λ����
	JT808_DEF_DATA_IC   = 0x0B,//��·����֤ IC ����Ϣ�ϴ���ϢΪ 64Byte���´���ϢΪ
							   //24Byte����·����֤ IC ����֤͸����ʱʱ��Ϊ 30s����ʱ�󣬲��ط�
	JT808_DEF_DATA_COM1 = 0x41,//���� 1 ͸����Ϣ 
	JT808_DEF_DATA_COM2 = 0x42,//���� 2 ͸����Ϣ 
										 //0xF0-0xFF  �û��Զ���͸����Ϣ
}TJT808DEFINE;


typedef struct TJT808Head
{
    unsigned char  nProtocol;//Э������ 0��GSM�� 1������
    unsigned short nID;  //��ϢID
    unsigned char  bMultiPacket;//�Ƿ���
    unsigned char  nEncryptType;//��������
    unsigned char  szSystemID[12];//12λϵͳ���
    unsigned short nSeqNum;//��ˮ��
    unsigned short nPacketCount;//�������
    unsigned short nPacketSeq;//������
}TJT808Head;

//ͨ������
typedef struct TJT808COMMONREQ
{
    TJT808Head head;
    //nodata
}TJT808COMMONREQ;

//ͨ��Ӧ��
typedef struct TJT808COMMONRES
{
    TJT808Head head;

    unsigned short nSeqNum; //sequence number
    unsigned short nResID;//Response ID
    unsigned char  nResult;//���0,�ɹ�/ȷ�ϣ�1,ʧ�ܣ�2,��Ϣ����3,��֧��
} TJT808COMMONRES;

//ƽ̨ͨ��Ӧ��
typedef TJT808COMMONRES TJT808PlatformRes;

//�ն�ͨ��Ӧ��
typedef TJT808COMMONRES TJT808TerminalRes;

//�ն�����
typedef TJT808COMMONREQ TJT808KeepAlive;

//�ն�ע��
typedef struct TJT808REGISTERREQ
{
    TJT808Head head;

    unsigned short nProvID;//ʡ��ID 
    unsigned short nCityID;//������ID 
    unsigned char  szManID[6] ;//������ID
#ifdef PT_JT808_VER_201301
	unsigned char  szModel[20] ;//�ն��ͺ�  Ver201301   xlsong
#else
    unsigned char  szModel[8] ;//�ն��ͺ�
#endif
    unsigned char  szTerminalID[8] ;//�ն�ID
    unsigned char  nColor ;//������ɫ
    unsigned char  szLicensePlate[32];//����
}TJT808REGISTERREQ;

//�ն�ע��Ӧ��
typedef struct TJT808REGISTERRES
{
    TJT808Head head;

    unsigned short nSeqNum;// Ӧ����ˮ��
    unsigned char nResult;//���
    unsigned char szSystemID[13];//�������
    unsigned char szAuthcode[32];//��Ȩ��
}TJT808REGISTERRES;

//�ն�ע��
typedef TJT808COMMONREQ TJT808Deregister;

//�ն˼�Ȩ
typedef struct TJT808LOGIN
{
    TJT808Head head;

    unsigned char szAuthcode[32];//��Ȩ��
}TJT808LOGIN;

//�ն˲�������
typedef struct TJT808CanParam
{
    unsigned int  nInterval;   //��ʾ�� ID �ɼ�ʱ����(ms)��0 ��ʾ���ɼ�
    unsigned char nChannel;   //��ʾ CAN ͨ���ţ�0��CAN1��1��CAN2
    unsigned char nFrameType; //��ʾ֡���ͣ�0����׼֡��1����չ֡
    unsigned char nMode;      //��ʾ���ݲɼ���ʽ��0��ԭʼ���ݣ�1���ɼ�����ļ���ֵ
    unsigned int  nCanID;     //��ʾ CAN ���� ID
    unsigned char bValid;
}TJT808CanParam;
        
typedef  struct TJT808ParameterItem
{
    unsigned int  nType;    //��������
    //����ֵ
    union
    {
        unsigned int  nValue;       //��ֵ����
        unsigned char szValue[32];  //�ַ�������
        TJT808CanParam canValue;
    }param;                         
}TJT808ParameterItem;
//�����ն˲���
typedef struct TJT808ParameterSet
{
    TJT808Head head;

    unsigned char nCount;//��������
    TJT808ParameterItem params[JT808_DEF_MAX_NUM_PARAMETER];//�������б�
}TJT808ParameterSet;

//��ѯ�ն˲���
typedef struct TJT808ParameterQueryReq
{
	TJT808Head head;
	unsigned int ncount;//������ĸ���
	unsigned int nType[JT808_DEF_MAX_NUM_PARAMETER];//�������б�
}TJT808ParameterQueryReq;

//typedef TJT808COMMONREQ TJT808ParameterQueryReq; 
//��ѯ�ն˲���Ӧ��
typedef struct TJT808ParameterQueryRes
{
    TJT808Head head;

    unsigned int  nSeqNum;//Ӧ����ˮ��
    unsigned char nCount; //Ӧ���������
    TJT808ParameterItem params[JT808_DEF_MAX_NUM_PARAMETER];//�������б�
}TJT808ParameterQueryRes;

//�ն�������Ϣ����
typedef struct TJT808ConnInfo
{
    unsigned char  szAPN[32];//���ŵ�����
    unsigned char  szUser[32];//�����û���
    unsigned char  szPassword[32];//��������
    unsigned char  szAddress[64];//��������ַ��IP��������
    unsigned short nTCPPort;//������TCP�˿�
    unsigned short nUDPPort;//������UDP�˿�
    unsigned int   nBDID;//�û���IC����
}TJT808ConnInfo;
//�ն˿���
typedef struct TJT808TerminalCtrl
{
    TJT808Head head;
    unsigned char nCMD;//������
                       //1:��������  TUpdateInfo
                       //2:�����ն�����ָ��������  TPlatform
                       //3:�ն˹ػ�
                       //4:�ն˸�λ
                       //5:�ն˻ָ���������
                       //6:�ر�����ͨѶ
                       //7:�ر���������ͨѶ
    union
    {
        struct TUpdateInfo
        {
            unsigned char szURL[64];//URL ��ַ��

            TJT808ConnInfo conninfo;

            unsigned char  ManID[6];//������ID
            unsigned char  szHardwareVersion[16];//Ӳ���汾
            unsigned char  szSoftwareVersion[16];//����汾
            unsigned short nTimeLimit;//���ӵ�ָ��������ʱ��
        }updateinfo;
        
        struct TPlatform
        {
            unsigned char  nCtrl;//���ӿ���,
                                //0:�л���ָ�����ƽ̨������,
                                //1:�л���ԭȱʡ���ƽ̨�����������ָ�����״̬
            unsigned char  szAuthcode[32];//��Ȩ��

            TJT808ConnInfo conninfo;

            unsigned short nTimeLimit;//���ӵ�ָ��������ʱ��
        }platform;
    }cmdinfo;
    
}TJT808TerminalCtrl;

typedef struct TJT808DateTime
{
    unsigned short nYear;//��
    unsigned char  nMonth;//��
    unsigned char  nDay;//��
    unsigned char  nHour;//ʱ
    unsigned char  nMinute;//��
    unsigned char  nSecond;//��
}TJT808DateTime;

typedef struct JT808TyreSensor//̥ѹ������
{
    unsigned char nIndex;//��λ��1:��1,2����1���������Դ�����
    short nPressure;//kpa
    short nTemp;    //���϶ȡ�
    unsigned char nState;
}JT808TyreSensor;

typedef struct TJT808PosiState
{
    //λ�û�����Ϣ
    unsigned int nWarnFlag;//�澯��־
    unsigned int nState;//״̬
    double dblLatitude;//γ��;
    double dblLongitude;//����
    unsigned short nHigh;//���θ߶� m
    double dblSpeed;//�ٶ� Km/h 
    unsigned short nDeriction;//����

    unsigned short nYear;//��
    unsigned char  nMonth;//��
    unsigned char  nDay;//��
    unsigned char  nHour;//ʱ
    unsigned char  nMinute;//��
    unsigned char  nSecond;//��
}TJT808PosiState;

typedef struct TJT808PositionInfo
{
    //λ�û�����Ϣ
    unsigned int nWarnFlag;//�澯��־
    unsigned int nState;//״̬
    double dblLatitude;//γ��;
    double dblLongitude;//����
    unsigned short nHigh;//���θ߶� m
    double dblSpeed;//�ٶ� Km/h 
    unsigned short nDeriction;//����

    unsigned short nYear;//��
    unsigned char  nMonth;//��
    unsigned char  nDay;//��
    unsigned char  nHour;//ʱ
    unsigned char  nMinute;//��
    unsigned char  nSecond;//��
    //������Ϣ
    struct 
    {
		unsigned int  nFlag;//0:������Ϣ���������и�����Ϣ
        unsigned int   nMileage;//���0x01 1/10km
        unsigned short nOil;    //����0x02 1/10L
        unsigned short nSpeed;  //��¼���ٶ� 0x03 1/10km/h
		unsigned short nEventID;//0x04��Ҫ�˹�ȷ�ϱ����¼���ID��WORD���� 1 ��ʼ���� ---Ver201301
        //0x11 ���ٱ���������Ϣ
        struct TOverspeedInfo
        {
            unsigned char nPositionType;//λ������
            unsigned int nAreaID;//����ID
        } overspeedinfo;

        //0x12 ��������/·�߱���������Ϣ
        struct TAreaInfo
        {
            unsigned char nPositionType;//λ������
            unsigned int nAreaID;//����ID
            unsigned char nDeriction;//���� 0:����1:��
        } areainfo;

        //0x13 ·����ʻʱ�䲻��/��������������Ϣ
        struct TDrivingInfo
        {
            unsigned int   nAreaID;//����
            unsigned short nTime;//·����ʻʱ��
            unsigned char  nResult;//��� 0:���㣻1:����
        } drivinginfo;
		
		unsigned int   nCarSignalStatus; //0x25 ��չ�����ź�״̬λ
		unsigned short nIoStatus;//0x2A IO״̬λ
		unsigned int   nAnalogData;//0x2B ģ������bit0-15��AD0��bit16-31��AD1
		unsigned char  nWLSignal;//0x30 ����ͨ�������ź�ǿ��
		unsigned char  nGnssStars;//0x31 GNSS ��λ������
		unsigned int   nAddDataLen;//�����Զ�����Ϣ����
    }addininfo;
    
    //�Զ�����Ϣ
    struct
    {
        unsigned int   nFlag;//0:������Ϣ���������и�����Ϣ

        unsigned char  nOilCount;//������������
        unsigned short oils[JT808_DEF_MAX_NUM_SENSOR_OIL];//���� %
        unsigned char  oilsstate[JT808_DEF_MAX_NUM_SENSOR_OIL];//����״̬

        unsigned char  nTempCount;//�¶ȴ�������
        unsigned short temps[JT808_DEF_MAX_NUM_SENSOR_TEMP];//���϶ȡ�
        unsigned char  tempsstate[JT808_DEF_MAX_NUM_SENSOR_TEMP];//�¶�״̬

        unsigned char  nTyreCount;//̥ѹ��������
        JT808TyreSensor tyres[JT808_DEF_MAX_NUM_SENSOR_TYRE];

        unsigned char  nPosiCount;//λ�ƴ�������
        unsigned short positions[JT808_DEF_MAX_NUM_SENSOR_POSI];//λ��
        unsigned char  posistate[JT808_DEF_MAX_NUM_SENSOR_POSI];//λ��״̬

        unsigned char  nCarryState;		//�ػ�״̬ 

        unsigned int   nServerFlag;//0xFF��������ʶ��Ϣ �Զ��帽����Ϣ
    }userdef;
}TJT808PositionInfo;
//λ����Ϣ�㱨
typedef struct TJT808PositionReport
{
    TJT808Head head;
    TJT808PositionInfo position;
}TJT808PositionReport;

//λ����Ϣ��ѯ
typedef TJT808COMMONREQ TJT808PositionQueryReq;

//λ����Ϣ��ѯӦ��
typedef struct TJT808PositionQueryRes
{
    TJT808Head head;

    unsigned short nSeqNum;// Ӧ����ˮ��

    TJT808PositionInfo position;
}TJT808PositionQueryRes;

//��ʱλ�ø���
typedef struct TJT808PositionTrack
{
    TJT808Head head;

    unsigned short nInterval;//ʱ����,unit:s, 0:ֹͣ����,�޺����ֶ�
    unsigned int   nTimeLimit;//λ�ø�����Ч��,unit:s,
                              //�ն��ڽ��յ�λ�ø��ٿ�����Ϣ��
                              //����Ч�ڽ�ֹʱ��֮ǰ��������Ϣ�е�ʱ��������λ�û㱨
}TJT808PositionTrack;

//�ı��·�
typedef struct TJT808TextNotice
{
    TJT808Head head;

    unsigned char nFlag;//�ı���Ϣ��־λ�������
    unsigned char szText[256];//�ı���Ϣ
}TJT808TextNotice;

//�¼�����
typedef struct TJT808Event
{
    unsigned char nEventID;//�¼�ID,���ն�����ͬID���¼����򱻸���
    unsigned char szEvent[24];//�¼����ݣ���GBK����
}TJT808Event;

typedef struct TJT808EventSet
{
    TJT808Head head;

    unsigned char nType;//��������
    unsigned char nCount;//��������
    TJT808Event events[JT808_DEF_MAX_NUM_EVENT];//�¼����б�
}TJT808EventSet;

//�¼�����
typedef struct TJT808EventReport
{
    TJT808Head head;

    unsigned char nEventID;//�¼�ID
}TJT808EventReport;

//�����·�
typedef struct TJT808Key
{
    unsigned char nKeyID;//��ID
	unsigned short nKeyLen;//�������ֶ��ֽڳ���    ȷ���𰸵���󳤶�   ---by xlsong
	unsigned char szKey[256];//�����ݣ���GBK����
}TJT808Key;

typedef struct TJT808QuestionReq
{
    TJT808Head head;

    unsigned char nFlag;//�����·���־λ����
    
    unsigned char szQuestion[256];//�����ı�����GBK����
    unsigned char nKeyCount;//������
    TJT808Key keys[JT808_DEF_MAX_NUM_KEY];//��ѡ���б�
}TJT808QuestionReq;

//����Ӧ��
typedef struct TJT808QuestionRes
{
    TJT808Head head;

    unsigned short nSeqNum;//Ӧ����ˮ��
    unsigned char  nKeyID;//��ID
}TJT808QuestionRes;

//��Ϣ�㲥�˵�����
typedef struct TJT808Service
{
    unsigned char nServiceID;//��Ϣ����
    unsigned char szService[24];//��Ϣ����
}TJT808Service;

typedef struct TJT808ServiceSet
{
    TJT808Head head;

    unsigned char nType;//��������,0:ɾ���ն�ȫ����Ϣ��;1:���²˵���2׷�Ӳ˵���3�޸Ĳ˵�
    unsigned char nCount;//��Ϣ������
    TJT808Service services[JT808_DEF_MAX_NUM_SERVICE];//��Ϣ���б�
}TJT808ServiceSet;

//��Ϣ�㲥/ȡ��
typedef struct TJT808ServiceOpt
{
    TJT808Head head;

    unsigned char nType;//��Ϣ����
    unsigned char nFlag;//0 ȡ����1�㲥
}TJT808ServiceOpt;

//��Ϣ����
typedef struct TJT808ServiceInfo
{
    TJT808Head head;

    unsigned char nType;//��Ϣ����
    unsigned char szService[256];//��Ϣ����
}TJT808ServiceInfo;

//�绰�ز�
typedef struct TJT808PhoneCallback
{
    TJT808Head head;

    unsigned char nFlag;//��־ ,0:��ͨͨ����1:����
    unsigned char szPhone[24];//�غ��绰����
}TJT808PhoneCallback;

//���õ绰��
typedef struct TJT808PhoneItem
{
    unsigned char nFlag;//��־,1: ���룻2: ������3: ����/����
    unsigned char szPhone[24];
    unsigned char szName[24];
}TJT808PhoneItem;

typedef struct TJT808PhoneBook
{
    TJT808Head head;

    unsigned char nType;//�������� 
                        //0 ɾ���ն������д洢����ϵ��
                        //1 ���µ绰��(ɾ���ն������е�ȫ����ϵ�˲�׷����Ϣ���е���ϵ��)
                        //2 ׷�ӵ绰��
                        //3 �޸ĵ绰��(����ϵ��Ϊ����)
    
    unsigned char nCount;//��ϵ������
    TJT808PhoneItem book[JT808_DEF_MAX_NUM_PHONEITEM];//�绰����ϵ����
}TJT808PhoneBook;

//��������
typedef struct TJT808CarCtrlReq
{
    TJT808Head head;

#ifdef PT_JT808_VER_ITS //ITSƽ̨����ΪShort
    unsigned short nCMD;//ITS �ͻ���,��ָ��Ϊ16bit ,
                        //�ұ���λ��һ��ȫΪ0��
                        //ʹ��ʱ��ֻȡ��Чλbit0    ---by xlsong   
#else
    unsigned char nCMD; //���Ʊ�־, bit0-0:���Ž�����1:���ż���  
                        //bit1~6����    
                        //bit7-0:�ָ��͵磻1:���͵磬�Զ���
#endif
	unsigned int  nServerFlag;//��������ʶ��Ϣ �Զ���
}TJT808CarCtrlReq;

//��������Ӧ��
typedef TJT808PositionQueryRes TJT808CarCtrlRes;

//�������������������
typedef struct TJT808Point
{
    double dblLatitude;//γ��;
    double dblLongitude;//����
}TJT808Point;
//Բ��������������������
typedef struct TJT808Circle
{
    unsigned int   nRegionID;//����ID
    unsigned short nFlag;//��������
    TJT808Point    ptCenter;//Բ��
    unsigned int   nRadius;//�뾶,��λΪ��m
    TJT808DateTime tmStart;//��ʼʱ��,����������0λΪ0��û�и��ֶ�
    TJT808DateTime tmEnd;//����ʱ��,����������0λΪ0��û�и��ֶ�
    unsigned short nMaxSpeed;//����ٶ�,Km/h������������1λΪ0��û�и��ֶ�
    unsigned char  nTimeLimit;//���ٳ���ʱ��,��λΪ��s������������λ1Ϊ0��û�и��ֶ�
}TJT808Circle;
//����Բ������
typedef struct TJT808RegionCircleSet
{
    TJT808Head head;

    unsigned char nType;     //�������� 0:��������1:׷������2:�޸�����
    unsigned char nRegionCount;//��������
    TJT808Circle  Regions[JT808_DEF_MAX_NUM_REGION];//Բ�����������������
}TJT808RegionCircleSet;

//ɾ��Բ������
typedef struct TJT808RegionRemove
{
    TJT808Head head;

    unsigned char nRegionCount;//��������,������125��������125�������ö�����Ϣ��0Ϊɾ������Բ������
    unsigned int  Regions[JT808_DEF_MAX_NUM_REGION];//Բ�����������ID
}TJT808RegionRemove;

typedef TJT808RegionRemove TJT808RegionCircleRemove;
//������������������
typedef struct TJT808Rect
{
    unsigned int   nRegionID;//����ID
    unsigned short nFlag;//��������
    TJT808Point    ptLeftTop;//���ϵ�
    TJT808Point    ptRightBottom;//���µ�
    TJT808DateTime tmStart;//��ʼʱ��,����������0λΪ0��û�и��ֶ�
    TJT808DateTime tmEnd;//����ʱ��,����������0λΪ0��û�и��ֶ�
    unsigned short nMaxSpeed;//����ٶ�,Km/h������������1λΪ0��û�и��ֶ�
    unsigned char  nTimeLimit;//���ٳ���ʱ��,��λΪ��s������������λ1Ϊ0��û�и��ֶ�
}TJT808Rect;
//���þ�������
typedef struct TJT808RegionRectSet
{
    TJT808Head head;

    unsigned char nType;       //�������� 0:��������1:׷������2:�޸�����
    unsigned char nRegionCount;//��������
    TJT808Rect  Regions[JT808_DEF_MAX_NUM_REGION];//�������������������
}TJT808RegionRectSet;

//ɾ����������
typedef struct TJT808RegionRemove TJT808RegionRectRemove;


//���ö��������
typedef struct TJT808RegionPolygonSet
{
    TJT808Head head;

    unsigned int   nRegionID;//����ID
    unsigned short nFlag;//��������
    TJT808DateTime tmStart;//��ʼʱ��,����������0λΪ0��û�и��ֶ�
    TJT808DateTime tmEnd;//����ʱ��,����������0λΪ0��û�и��ֶ�
    unsigned short nMaxSpeed;//����ٶ�,Km/h������������1λΪ0��û�и��ֶ�
    unsigned char  nTimeLimit;//���ٳ���ʱ��,��λΪ��s������������λ1Ϊ0��û�и��ֶ�

    unsigned char nPointCount;//���򶥵�����
    TJT808Point  Points[JT808_DEF_MAX_NUM_POLYGON_POINT];//���������Ķ�����
}TJT808RegionPolygonSet;

//ɾ�����������
typedef struct TJT808RegionRemove TJT808RegionPolygonRemove;

//·�߹յ�������
typedef struct TJT808Corner
{
    unsigned int   nCornerID;//�յ�ID
    unsigned int   nRoadID;//·��ID
    TJT808Point    ptCorner;//�յ�
    unsigned char  nRoadWidth;//·�ο��
    unsigned char  nFlag;//·������,��λΪ��m��·��Ϊ�ùյ㵽��һ�յ�
    unsigned short nMaxTime;//·����ʻ������ֵ,��λΪ��s����·������0λΪ0��û�и��ֶ�
    unsigned short nMinTime;//·����ʻ���㷧ֵ,��λΪ��s,����������0λΪ0��û�и��ֶ�
    unsigned short nMaxSpeed;//·������ٶ�,Km/h������������1λΪ0��û�и��ֶ�
    unsigned char  nTimeLimit;//���ٳ���ʱ��,��λΪ��s������������λ1Ϊ0��û�и��ֶ�
}TJT808Corner;

//����·��
typedef struct TJT808RouteSet
{
    TJT808Head head;

    unsigned int   nRouteID;//·��ID,�ظ����������
    unsigned short nFlag;//·������
    TJT808DateTime tmStart;//��ʼʱ��,��·������0λΪ0��û�и��ֶ�
    TJT808DateTime tmEnd;//����ʱ��,��·������0λΪ0��û�и��ֶ�
   
    unsigned short nCornerCount;//·���ܹյ���
    TJT808Corner  Corners[JT808_DEF_MAX_NUM_ROUTE_CORNER];//·�߹յ�������
}TJT808RouteSet;

//ɾ��·��
typedef struct TJT808RegionRemove TJT808RouteRemove;

//��ʻ��¼�����ݲɼ�����
typedef struct TJT808DriveingRecorderReq
{
    TJT808Head head;

    unsigned char nType;    //������,�б��GB/T19056�����Ҫ��
    unsigned int  nDataLen; //���ݿ鳤��
    unsigned char data[256];//����
}TJT808DriveingRecorderReq;

//��ʻ��¼�ǲɼ������ϴ�
typedef struct TJT808DriveingRecorderRes
{
    TJT808Head head;

    unsigned short nSeqNum;// Ӧ����ˮ��
    unsigned char  nType;    //������,�б��GB/T19056�����Ҫ��

    unsigned short nDataLen; //���ݿ鳤��
    unsigned char  data[1008];//����
}TJT808DriveingRecorderRes;

//��ʻ��¼�ǲ����´�����
typedef struct TJT808DriveingRecorderParam
{
    TJT808Head head;

    unsigned char nType;    //������,�б��GB/T19056�����Ҫ��
    unsigned int  nDataLen; //���ݿ鳤��
    unsigned char data[256];//����
}TJT808DriveingRecorderParam;

//�����˵��ϱ�
typedef struct TJT808OrderReport
{
    TJT808Head head;

    unsigned int  nDataLen;//�����˵�����
    unsigned char data[256];//�����˵�����
}TJT808OrderReport;

//��ʻԱ�����Ϣ�ɼ��ϱ�
typedef struct TJT808DriverInfoReport
{
    TJT808Head head;
	unsigned char  nStatus;//0x01����ҵ�ʸ�֤ IC ������,0x02����ҵ�ʸ�֤ IC ���γ�---Ver201301 add
	TJT808DateTime time;//ʱ�� YY-MM-DD-hh-mm-ss   ---Ver201301 add
	unsigned char  nICStatus;//IC ����ȡ��� ---Ver201301 add
    unsigned char  szName[24];//��ʻԱ����
    unsigned char  szIDCard[24];//��ʻԱ���֤����  ---Ver201301 delete this item    by xlsong
    unsigned char  szCertificateID[48];//��ҵ�ʸ�֤���
    unsigned char  szIssueDepartment[128];//��֤��������
	unsigned char  szUsefulLife[4];//֤����Ч��  YYYYMMDD  ---Ver201301 add
}TJT808DriverInfoReport;

//��ý���¼���Ϣ�ϴ�
typedef struct TJT808MediaEventReport
{
    TJT808Head head;

    unsigned int   nMediaID;//��ý��ID
    unsigned char  nMediaType;//0ͼ��1��Ƶ��2��Ƶ
    unsigned char  nCodeType;//0JPEG��1TIF��2MP3��3WAV��4WMV����������
    unsigned char  nEventType;//0ƽ̨�·�ָ�1��ʱ������2���ٱ���������3��ײ�෭������������������
    unsigned char  nChannel;//ͨ��ID
}TJT808MediaEventReport;

//��ý�������ϴ�
typedef struct TJT808MediaDataUploadReq
{
    TJT808Head head;
    
    unsigned int   nMediaID;//��ý��ID
    unsigned char  nMediaType;//0ͼ��1��Ƶ��2��Ƶ
    unsigned char  nCodeType;//0JPEG��1TIF��2MP3��3WAV��4WMV����������
    unsigned char  nEventType;//0ƽ̨�·�ָ�1��ʱ������2���ٱ���������3��ײ�෭������������������
    unsigned char  nChannel;//ͨ��ID
	TJT808PosiState position;//��ý�����ݵ�λ�û�����Ϣ����   ---Ver201301
    unsigned short nDataLen;
    unsigned char  data[1024];//��ý�����ݰ�
}TJT808MediaDataUploadReq;

//��ý�������ϴ�Ӧ��
typedef struct TJT808MediaDataUploadRes
{
    TJT808Head head;

    unsigned int   nMediaID;
    unsigned char  nResendCount;//�ش�����
    unsigned short  ResendIDList[255];//�ش����ݰ�ID�б� 
}TJT808MediaDataUploadRes;

//����ͷ��������ָ�� 
typedef struct TJT808CameralCtrl
{
    TJT808Head head;
	
	unsigned char  	nChannel;//ͨ��ID  (����ITS��Э��:bit0-channel1,bit1-channel2...)
	unsigned short  nType;   //����ָ�� 0:��ʾֹͣ���� 1:��ʼ���� 0xFFFF:��ʾ¼��;������ʾ���մ���
	//unsigned short  nPhototime;//��������   modify by xlsong
	unsigned short	nInterval;//������/¼��ʱ�� ��λ:s; 0:��ʾ����Сʱ�������ջ�һֱ¼��
	unsigned char  	nSaveFlag;//�����־ 0:��ʾʵʱ�ϴ�;1:��ʾ����
	
	unsigned char    nDistinguishability;//�ֱ���
    unsigned char  	nPictureQuality;//ͼ��/��Ƶ���� 1~10 1:��ʾ������ʧ��С 10:ѹ�������
	unsigned char  	nBrightness;//���� 0~255
	unsigned char  	nContrast;//�Աȶ� 0~127
	unsigned char  	nSaturation;//���Ͷ� 0~127
	unsigned char  	nChroma;//ɫ�� 0~255
	
}TJT808CameralCtrl;


//�洢��ý�����ݼ���
typedef struct TJT808MediaIndexReq
{
    TJT808Head head;
	
    unsigned char nMedia;//��ý������ 0:ͼ��; 1:��Ƶ; 2:��Ƶ
    unsigned char nChannel;//ͨ��ID   0:��ʾ������ý�����͵�����ͨ��
    unsigned char nEventType;////�¼������ 0:ƽ̨�·�ָ�� 1:��ʱ���� 2:���ٱ������� 3:��ײ�෭������������������

    TJT808DateTime  tmStart;//��ʼʱ�� YY-MM-DD-HH-MM-SS
    TJT808DateTime  tmEnd;//����ʱ��  YY-MM-DD-HH-MM-SS
	
}TJT808MediaIndexReq;

//��ý������������
typedef struct TJT808MediaIndex
{
	unsigned int nMediaID;//��ý��ID  20130124��Э������  ---- Ver201301 add
    unsigned char nMedia;//��ý������ 0:ͼ��; 1:��Ƶ; 2:��Ƶ
    unsigned char nChannel;//ͨ��ID   0:��ʾ������ý�����͵�����ͨ��
    unsigned char nEventType;//�¼������ 0:ƽ̨�·�ָ�� 1:��ʱ���� 2:���ٱ������� 3:��ײ�෭������������������
    TJT808PosiState position;//λ����Ϣ�㱨
}TJT808MediaIndex;
	
//�洢��ý�����ݼ���Ӧ�� 
typedef struct TJT808MediaIndexRes
{
    TJT808Head head;

	unsigned short nSeqNum;//Ӧ����ˮ��
	
	unsigned short nIndexcount;//��ý�����ݼ��������� 2013-01-11
    TJT808MediaIndex IndexData[JT808_DEF_MAX_NUM_INDEX_MEDIA];//��ý����������� 2013-01-11
	
}TJT808MediaIndexRes;

//�洢��ý�������ϴ����� 
typedef struct TJT808MediaDataUpload
{
    TJT808Head head;

	unsigned char nMedia;//��ý������ 0:ͼ��; 1:��Ƶ; 2:��Ƶ
    unsigned char nChannel;//ͨ��ID   0:��ʾ������ý�����͵�����ͨ��
    unsigned char nEventType;//�¼������ 0:ƽ̨�·�ָ�� 1:��ʱ���� 2:���ٱ������� 3:��ײ�෭������������������
    
    TJT808DateTime  tmStart;//��ʼʱ�� YY-MM-DD-HH-MM-SS
    TJT808DateTime  tmEnd;//����ʱ��   YY-MM-DD-HH-MM-SS
	unsigned char   nDelFlag;//ɾ����־
}TJT808MediaDataUpload;

//¼�����ʼ/ֹͣ
typedef struct TJT808RecorderCtrl
{
    TJT808Head head;

    unsigned char  nCMD;    //0:ֹͣ��1����ʼ
    unsigned short nTimeLimit; //ʱ�䳤�� 0����ʾһֱ����
    unsigned char  nSaveFlag;//0:�ϴ���1�����汾��
    unsigned char  nSample;//¼�������� 0:8K��1:11K��2: 23K��3: 32K����������
}TJT808RecorderCtrl;

//����͸��
typedef struct TJT808DataLoad
{
    TJT808Head head;

    unsigned char  nDataType;//��������
    unsigned int nDataLen; //���ݳ���   by xlsong
    unsigned char  data[420];//��������
}TJT808DataLoad;

//��������͸��
typedef TJT808DataLoad  TJT808DataDownload;

//��������͸��
typedef TJT808DataLoad  TJT808DataUpload;

//����ѹ���ϱ�
typedef TJT808DataLoad  TJT808DataGzipLoad;

//RSA��Կ
typedef struct TJT808Ras
{
    TJT808Head head;

    unsigned int   nRasE;//RSA��Կ{e,n}�е�e
    unsigned short nRasNLen; //RSA��Կ{e,n}�е�n����
    unsigned char  RasN[128];//RSA��Կ{e,n}�е�n
}TJT808Ras;

//ƽ̨RAS��Կ
typedef TJT808Ras  TJT808PlatFormRas;

//�ն�RAS��Կ
typedef TJT808Ras  TJT808TerminalRas;

//�Զ���Э��
//���û�λ���ϱ�
typedef struct TSimplePosition
{
    unsigned char nUserID;//�豸���
    double dblLatitude;//γ��;
    double dblLongitude;//����
    double dblSpeed;//�ٶ� Km/h 
    unsigned short nDeriction;//����
}TSimplePosition;

typedef struct TJT808MultiPosition
{
    TJT808Head head;

    unsigned short nYear;//��
    unsigned char  nMonth;//��
    unsigned char  nDay;//��
    unsigned char  nHour;//ʱ
    unsigned char  nMinute;//��
    unsigned char  nSecond;//��

    unsigned char  nCount;//�û���
    TSimplePosition position[JT808_DEF_MAX_NUM_USERPOSITION];
}TJT808MultiPosition;

//�����������ϴ�

typedef struct TJT808SensorReport
{
    TJT808Head head;

    unsigned char  nOilCount;//������������
    unsigned short oils[JT808_DEF_MAX_NUM_SENSOR_OIL];//���� %
    unsigned char  oilsstate[JT808_DEF_MAX_NUM_SENSOR_OIL];//����״̬
		
    unsigned char  nTempCount;//�¶ȴ�������
    unsigned short temps[JT808_DEF_MAX_NUM_SENSOR_TEMP];//���϶ȡ�
    unsigned char  tempsstate[JT808_DEF_MAX_NUM_SENSOR_TEMP];//�¶�״̬
	
    unsigned char  nTyreCount;//̥ѹ��������
    JT808TyreSensor tyres[JT808_DEF_MAX_NUM_SENSOR_TYRE];
	
    unsigned char  nPosiCount;//λ�ƴ�������
    unsigned short positions[JT808_DEF_MAX_NUM_SENSOR_POSI];//λ��

	unsigned char  nCarryState;		//�ػ�״̬ 
    unsigned int   nWarnFlag;//����״̬
    unsigned int   nState;//����״̬
}TJT808SensorReport;
//���������ݲ�ѯ����
typedef struct TJT808SensorQueryReq
{
    TJT808Head head;
    
    unsigned char  szDevice[16];//�豸���
}TJT808SensorQueryReq;

//���������ݲ�ѯ����
typedef struct TJT808SensorQueryRes
{
    TJT808Head head;
    unsigned short nSeqNum;// Ӧ����ˮ��
    unsigned char  nResult;// Ӧ����
    unsigned char  nCarryState;//�ػ�״̬
    unsigned int   nWarnFlag;//����״̬
    unsigned int   nState;//����״̬
    unsigned char  szOil[24];//��������
    unsigned char  szTemp[24];//�¶ȴ���������
    unsigned char  szTyre[128];//̥ѹ����
    unsigned char  szPosi[32];//λ�ƴ���������
}TJT808SensorQueryRes;
//------Ver201301 Ver201301---
typedef struct TJT808SendAgainReq
{
	TJT808Head head;
	unsigned short nSeqNum;// Ӧ����ˮ��
	unsigned char nCount;//�ش�������
	unsigned short nIDlist[128];//�ش� ID �б�
}TJT808SendAgainReq;

//��ѯ�ն�����Ӧ��  Ver201301
typedef struct TJT808PropertyQueryRes
{
	TJT808Head head;
	unsigned short nType;//�ն�����
	unsigned char szManID[6];//������ID
	unsigned char szModel[20] ;//�ն��ͺ�
	unsigned char szTerminalID[8] ;//�ն�ID
	unsigned char szSIMID[20];//�ն� SIM �� ICCID
	unsigned char szHWVerNum[8];//�ն�Ӳ���汾��
	unsigned char szFWVerNum[8];// �ն˹̼��汾��
	unsigned char nGNSSProp;//GNSS ģ������
	unsigned char nCommProp;//ͨ��ģ������
}TJT808PropertyQueryRes;

//�·��ն�������  Ver301301
typedef struct TJT808UpdatePackageReq
{
	TJT808Head head;
	unsigned char nType;// ��������[0���ն�,12:��·����֤ IC ��������,52:�������Ƕ�λģ��]
	unsigned char szManID[6]; //������ID
	unsigned char szVerNum[8];//�汾��
    unsigned int  nPackageLen;//�������ݰ��ܳ���
	unsigned int  nDataLen;   //�������ݳ���
	unsigned char data[1004]; //��������  1023-18 = 1005
}TJT808UpdatePackageReq;

//�ն��������֪ͨ  Ver301301
typedef struct TJT808UpdatePackageRes
{
	TJT808Head head;
	unsigned char nUpdateType;// ��������
	unsigned char nResult;//������� 0���ɹ���1��ʧ�ܣ�2��ȡ��
}TJT808UpdatePackageRes;

//�˹�ȷ�ϱ�����Ϣ   Ver201301
typedef struct TJT808WarmManage
{
	TJT808Head head;
	unsigned short nSeqNum;//������Ϣ��ˮ��  ���˹�ȷ�ϵı�����Ϣ��ˮ�ţ�0 ��ʾ�ñ�������������Ϣ
	unsigned int   nWarmType;//�˹�ȷ�ϱ�������
}TJT808WarmManage;

//��λ���������ϴ�  Ver201301
typedef struct TJT808PosDataUpload
{
	TJT808Head head;
	unsigned short nTotalCount;//����������
	unsigned char  nPosType;//λ����������
    
#ifdef JT808_PLATFORM
    unsigned short nDataCount;//�������������
	TJT808PositionInfo posdata[20];//λ�û㱨��Ϣ��
#else//�豸
    unsigned short nDataLen;
    unsigned char  data[1000];
#endif    
}TJT808PosDataUpload;

//CAN���������ϴ�  Ver201301
typedef struct TJT808CanDataUpload
{
	TJT808Head head;
    
	unsigned short nTotalCount;
	unsigned char  nHour;//ʱ   CAN �������ݽ���ʱ��
	unsigned char  nMinute;//��
	unsigned char  nSecond;//��
	unsigned char  nmSecond;//����
    
    unsigned short nDataCount;//���������
	TJT808CanParam CanParam[128];//CAN ���������� 
    unsigned char  CanData[128][8];
}TJT808CanDataUpload;

//����ͷ������������Ӧ�� Ver201301
typedef struct TJT808CameraCtrlRes
{
	TJT808Head head;
	unsigned short nSeqNum;// Ӧ����ˮ��
	unsigned char nResult;//���
	unsigned short nMediaCount;//����ɹ��Ķ�ý����� 
	unsigned int   MediaIDList[JT808_DEF_MAX_NUM_MEDIAID];//��ý�� ID �б�
}TJT808CameraCtrlRes;

//�����洢��ý�����ݼ����ϴ����� Ver201301
typedef struct TJT808OneMediaDataQueryReq
{
	TJT808Head head;
	unsigned int nMediaID;//��ý�� ID
	unsigned char   nDelFlag;//ɾ����־
}TJT808OneMediaDataQueryReq;

//�豸ʹ��ǰ���� Ver201301
typedef struct TJT808DeviceUnlockReq
{
	TJT808Head head;
    
    unsigned char bEncrypt;
    unsigned int  nEncryptKey;
             char szUserMobile[12];
    
    unsigned short nProvID;//ʡ��ID
    unsigned short nCity;//������ID
    
	unsigned char  szManufacturer[6];//������ID
    unsigned char  szModel[20];//�ն��ͺ�
    unsigned char  szDeviceID[8];//�ն�ID
    unsigned char  nColor;//������ɫ
    unsigned char  szLicenseplate[10];//���ƺ�
    unsigned char  szVIN[20];//����VIN
}TJT808DeviceUnlockReq;

//--------------------------------
#endif// end PT_ENABLE_JT808

#endif//end  H_PT_JT808_H

