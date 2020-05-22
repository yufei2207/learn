/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name��     protocol.c
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
#include "pt_config.h"
#include "pt_define.h"


typedef enum TDataType
{
    DATA_UNKNOWN    = -1,
#ifdef PT_ENABLE_BD1
    DATA_BD1,
#endif	 
#ifdef PT_ENABLE_BD2_RD 
    DATA_BD2_RD, 
#endif
#ifdef PT_ENABLE_BD2_RN 
    DATA_BD2_RN, 
#endif	 
#ifdef PT_ENABLE_BD2_SPECIAL 
    DATA_BD2_SP, 
#endif	 
#ifdef PT_ENABLE_BD2_EXPRESS 
    DATA_BD2_EX, 
#endif	 
#ifdef PT_ENABLE_NW 
    DATA_NW,
#endif	 
#ifdef PT_ENABLE_GPS 
    DATA_GPS,
#endif	 
#ifdef PT_ENABLE_GT06 
    DATA_GT06,
#endif	 
#ifdef PT_ENABLE_JT808 
    DATA_JT808,
#endif	 
#ifdef PT_ENABLE_JT808_DV 
    DATA_JT808_DV,
#endif	 
}TDataType;
//Э��ջ������

#if	PT_MAX_STACK_NUM > 0
uchar m_stackBuff[PT_MAX_STACK_NUM][PT_MAX_BUFFER_LEN];
TStackParam m_stackParam[PT_MAX_STACK_NUM];
#endif

typedef int (*FUNC_RECV)(TStackParam *pParam, uchar data);	
typedef int (*FUNC_DATA_RECV)(TStackParam *pParam, uchar data);
typedef int (*FUNC_HEAD_DECODER)(const uchar* buf, int len);
typedef int (*FUNC_HEAD_ENCODER)(uchar *lpBuff, int nBuffLen, char bDevice, int nType);
typedef int (*FUNC_PKG_ENCODER)(uchar *lpBuff, int nBuffLen, void *pPackage, 
                                int nMsgType, char bDevice);
typedef int (*FUNC_PKG_DECODER)(void *pPackage, int nMsgType, uchar *lpBuff, int nBuffLen);

int PT_RecvData_ASCII(TStackParam *pParam, uchar data);
int PT_RecvData_ASCII_Ex(TStackParam *pParam, uchar data);
int PT_DecodePackage_ASCII(void *pPackage, int nMsgType, uchar *lpBuff, int nBuffLen);
int PT_EncodePackage_ASCII(uchar *lpBuff, int nBuffLen, void *pPackage, int nMsgType, char bDevice);

int PT_RecvData_MIX(TStackParam *pParam, uchar data);



typedef struct TStackFunc
{
    FUNC_DATA_RECV    fnRecvData;
    FUNC_HEAD_ENCODER fnHeadEncoder;
    FUNC_HEAD_DECODER fnHeadDecoder;
    FUNC_PKG_ENCODER  fnPkgEncoder;
    FUNC_PKG_DECODER  fnPkgDecoder;
}TStackFunc;

#define MAX_STACKTYPE       10
TStackFunc  m_stackFunc[MAX_STACKTYPE] =
{
#ifdef PT_ENABLE_BD1
    BD1_RecvDataEx, BD1_EncodeHead, BD1_DecodeHead, BD1_EncodePackage, BD1_DecodePackage,
#endif
#ifdef PT_ENABLE_BD2_RD
    PT_RecvData_ASCII_Ex, BD2_RD_EncodeHead, BD2_RD_DecodeHead, PT_EncodePackage_ASCII, PT_DecodePackage_ASCII,
#endif
#ifdef PT_ENABLE_BD2_RN
    PT_RecvData_ASCII_Ex, BD2_RN_EncodeHead, BD2_RN_DecodeHead, PT_EncodePackage_ASCII, PT_DecodePackage_ASCII,
#endif
#ifdef PT_ENABLE_BD2_SPECIAL
    BD2_SP_RecvData, BD2_SP_EncodeHead, BD2_SP_DecodeHead, BD2_SP_EncodePackage, BD2_SP_DecodePackage,
#endif
#ifdef PT_ENABLE_BD2_EXPRESS
    PT_RecvData_ASCII_Ex, BD2_EX_EncodeHead, BD2_EX_DecodeHead, PT_EncodePackage_ASCII, PT_DecodePackage_ASCII,
#endif
#ifdef PT_ENABLE_NW
    PT_RecvData_ASCII_Ex, NW_EncodeHead, NW_DecodeHead, PT_EncodePackage_ASCII, PT_DecodePackage_ASCII,
#endif

#ifdef PT_ENABLE_GPS
    PT_RecvData_ASCII_Ex, GPS_EncodeHead, GPS_DecodeHead, PT_EncodePackage_ASCII, PT_DecodePackage_ASCII,
#endif
#ifdef PT_ENABLE_GT06
    GT06_RecvDataEx, GT06_EncodeHead, GT06_DecodeHead, GT06_EncodePackage, GT06_DecodePackage,
#endif
#ifdef PT_ENABLE_JT808
    JT808_RecvDataEx, JT808_EncodeHead, JT808_DecodeHead, JT808_EncodePackage, JT808_DecodePackage,
#endif
#ifdef PT_ENABLE_JT808_DV
    NULL, JT808_DV_EncodeHead, JT808_DV_DecodeHead, JT808_DV_EncodePackage, JT808_DV_DecodePackage,
#endif
};


#define PT_DECODEHEAD(NAME, MSGTYPE, BUFF, LEN, STACKTYPE) \
        {\
            if (STACKTYPE & STACK_ ## NAME)\
            {\
                MSGTYPE = NAME ## _DecodeHead(BUFF, LEN);\
                if (MSGTYPE != INVALID_TYPE)\
                {\
                    return MSGTYPE;\
                }\
            }\
        }

#define PT_ENCODEHEAD(NAME, BUFF, LEN, BDEVICE, TYPE) \
        {\
            if (NAME ## _ISTYPE(TYPE))\
            {\
                return NAME ## _EncodeHead(BUFF, LEN, BDEVICE, TYPE);\
            }\
        }

#define PT_GETENCODER(NAME, TYPE) \
        {\
            if (NAME ## _ISTYPE(TYPE))\
            {\
                return NAME ## _GetEncoder(TYPE);\
            }\
        }

#define PT_GETDECODER(NAME, TYPE) \
        {\
            if (NAME ## _ISTYPE(TYPE))\
            {\
                return NAME ## _GetDecoder(TYPE);\
            }\
        }

#define PT_GETPACKAGE(NAME, TYPE) \
{\
    if (NAME ## _ISTYPE(TYPE))\
    {\
        return NAME ## _GetPackage(TYPE);\
    }\
}

#define PT_GETDATATYPE(NAME, TYPE) \
{\
    if (NAME ## _ISTYPE(TYPE))\
    {\
        return DATA_ ## NAME;\
    }\
}
//--------------------------------------------------------------------------
//Э��ջ��������
//--------------------------------------------------------------------------
/*
*�������ƣ�PT_GetDataType
*����˵����������������
*����˵����nMsgType Э������
*����ֵ��  
*/
int PT_GetDataType(int nMsgType)
{
#ifdef PT_ENABLE_BD1        
    PT_GETDATATYPE(BD1, nMsgType);
#endif//PT_ENABLE_BD1

#ifdef PT_ENABLE_BD2_SPECIAL        
    PT_GETDATATYPE(BD2_SP, nMsgType);
#endif//PT_ENABLE_BD2_SPECIAL

#ifdef PT_ENABLE_BD2_EXPRESS 
    PT_GETDATATYPE(BD2_EX, nMsgType);
#endif//PT_ENABLE_BD2_EXPRESS

#ifdef PT_ENABLE_BD2_RN 
    PT_GETDATATYPE(BD2_RN, nMsgType);
#endif//PT_ENABLE_BD2_RN

#ifdef PT_ENABLE_BD2_RD 
    PT_GETDATATYPE(BD2_RD, nMsgType);
#endif//PT_ENABLE_BD2_RD

#ifdef PT_ENABLE_NW        
    PT_GETDATATYPE(NW, nMsgType);
#endif//PT_ENABLE_NW

#ifdef PT_ENABLE_GPS        
    PT_GETDATATYPE(GPS, nMsgType);
#endif//PT_ENABLE_GPS

#ifdef PT_ENABLE_GT06        
   PT_GETDATATYPE(GT06, nMsgType);
#endif//PT_ENABLE_GT06

#ifdef PT_ENABLE_JT808        
   PT_GETDATATYPE(JT808, nMsgType);
#endif//PT_ENABLE_JT808

#ifdef PT_ENABLE_JT808_DV        
   PT_GETDATATYPE(JT808_DV, nMsgType);
#endif//PT_ENABLE_JT808_DV

    return DATA_UNKNOWN;
}
/*
*�������ƣ�PT_DecodeHead
*����˵����������Ϣ����
*����˵����buf    �������ݻ���
*          len    �������ݳ���
*          nStackType Э��ջ����
*����ֵ��  -1 ��ʾ��Ч����Ϣ���ͣ�������ʾ����������
*/
int PT_DecodeHead(const uchar* buf, int len, int nStackType)
{
    int nType = INVALID_TYPE;

    if (nStackType != STACK_JT808_DV && (nStackType & STACK_JT808_DV))
    {//this protocol must solely work
        return INVALID_TYPE;
    }

#ifdef PT_ENABLE_GPS
    PT_DECODEHEAD(GPS, nType, buf, len, nStackType);
#endif

#ifdef PT_ENABLE_NW
    PT_DECODEHEAD(NW, nType, buf, len, nStackType);
#endif

#ifdef PT_ENABLE_BD2_RD
    PT_DECODEHEAD(BD2_RD, nType, buf, len, nStackType);
#endif

#ifdef PT_ENABLE_BD2_RN
    PT_DECODEHEAD(BD2_RN, nType, buf, len, nStackType);
#endif

#ifdef PT_ENABLE_BD2_SPECIAL
    PT_DECODEHEAD(BD2_SP, nType, buf, len, nStackType);
#endif

#ifdef PT_ENABLE_BD2_EXPRESS
    PT_DECODEHEAD(BD2_EX, nType, buf, len, nStackType);
#endif

#ifdef PT_ENABLE_BD1
    PT_DECODEHEAD(BD1, nType, buf, len, nStackType);
#endif

#ifdef PT_ENABLE_GT06
    PT_DECODEHEAD(GT06, nType, buf, len, nStackType);
#endif

#ifdef PT_ENABLE_JT808
    PT_DECODEHEAD(JT808, nType, buf, len, nStackType);
#endif

#ifdef PT_ENABLE_JT808_DV
    PT_DECODEHEAD(JT808_DV, nType, buf, len, nStackType);
#endif

	return INVALID_TYPE;
}
/*
*�������ƣ�PT_EncodeHead
*����˵��������Э��ͷ
*����˵����lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          bDevice   �Ƿ��豸
*          nType     ��Ϣ����
*����ֵ��  -1 ��ʾʧ�ܣ�������ʾ�������ݳ���
*/
int PT_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nMsgType)
{
    int nDataType = PT_GetDataType(nMsgType);
    if (nDataType !=  DATA_UNKNOWN)
    {
        if (m_stackFunc[nDataType].fnHeadEncoder)
        {
            return m_stackFunc[nDataType].fnHeadEncoder(lpBuff, nBuffLen, bDevice, nMsgType);
        }
    }

    return RESULT_INVILID_TYPE;
}
/*
*�������ƣ�PT_GetEncoder
*����˵����������Ϣ���ͻ�ȡ��Ӧ�ı��뺯��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�ޱ��뺯����������ʾ���뺯��ָ��
*/
FUNC_ENCODER PT_GetEncoder(int nMsgType)
{
#ifdef PT_ENABLE_GPS
    PT_GETENCODER(GPS, nMsgType);
#endif

#ifdef PT_ENABLE_NW
    PT_GETENCODER(NW, nMsgType);
#endif

#ifdef PT_ENABLE_BD2_RD
    PT_GETENCODER(BD2_RD, nMsgType);
#endif

#ifdef PT_ENABLE_BD2_RN
    PT_GETENCODER(BD2_RN, nMsgType);
#endif

#ifdef PT_ENABLE_BD2_EXPRESS
    PT_GETENCODER(BD2_EX, nMsgType);
#endif

    return NULL;
}
/*
*�������ƣ�PT_GetDecoder
*����˵����������Ϣ���ͻ�ȡ��Ӧ�Ľ��뺯��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�޽��뺯����������ʾ���뺯��ָ��
*/
FUNC_DECODER PT_GetDecoder(int nMsgType)
{
#ifdef PT_ENABLE_GPS
    PT_GETDECODER(GPS, nMsgType);
#endif

#ifdef PT_ENABLE_NW
    PT_GETDECODER(NW, nMsgType);
#endif

#ifdef PT_ENABLE_BD2_RD
    PT_GETDECODER(BD2_RD, nMsgType);
#endif

#ifdef PT_ENABLE_BD2_RN
    PT_GETDECODER(BD2_RN, nMsgType);
#endif

#ifdef PT_ENABLE_BD2_EXPRESS
    PT_GETDECODER(BD2_EX, nMsgType);
#endif

#ifdef PT_ENABLE_BD1
    PT_GETDECODER(BD1, nMsgType);
#endif

#ifdef PT_ENABLE_GT06
    PT_GETDECODER(GT06, nMsgType);
#endif

#ifdef PT_ENABLE_BD2_SPECIAL
    PT_GETDECODER(BD2_SP, nMsgType);
#endif

#ifdef PT_ENABLE_JT808
    PT_GETDECODER(JT808, nMsgType);
#endif

#ifdef PT_ENABLE_JT808_DV
    PT_GETDECODER(JT808_DV, nMsgType);
#endif

    return NULL;  
}
/*
*�������ƣ�PT_InitLib
*����˵������ʼ��Э��ջ
*����˵������
*����ֵ��  ��
*/
void  PT_InitLib(void)
{
#if PT_MAX_STACK_NUM > 0
	int i =0;
	for(; i < PT_MAX_STACK_NUM; i++)
	{
		m_stackParam[i].fnRecv = NULL;
	}
#endif
}
/*
*�������ƣ�PT_Initialize
*����˵������ʼ��Э��ջ����
*����˵����nStackType Э��ջ����
*����ֵ��  -1 ��ʾЭ��ջ�����㣬������ʾջID
*/
int  PT_Initialize(int nStackType)
{
#if PT_MAX_STACK_NUM > 0
	int i =0;
	for(; i < PT_MAX_STACK_NUM; i++)
	{
		if (m_stackParam[i].fnRecv == NULL)//�жϵ�ǰջ�Ƿ��ѷ���
		{
            m_stackParam[i].nStackType = nStackType;
            m_stackParam[i].RecvBuff = m_stackBuff[i];
            m_stackParam[i].nRecvBuffLen = PT_MAX_BUFFER_LEN;
            if (PT_InitializeEx(&m_stackParam[i]) == 0)
            {
			    return i;
            }
            else
            {
                return RESULT_ERROR;
            }
		}
	}
#endif
	
	return RESULT_ERROR;
}
/*
*�������ƣ�PT_InitializeEx
*����˵������ʼ��Э��ջ
*����˵����pParam Э�����ջָ��
*����ֵ��  -1 ��ʾЭ��ջ�����㣬������ʾջID
*/
int  PT_InitializeEx(TStackParam *pParam)
{
    if (pParam && pParam->nStackType > 0 
        && pParam->RecvBuff != NULL && pParam->nRecvBuffLen > 0)
    {
        if (pParam->nStackType == STACK_BD1)
        {
        #ifdef PT_ENABLE_BD1
            pParam->fnRecv = (void*)BD1_RecvData;
        #endif
        }
        else if (pParam->nStackType == STACK_GT06)
        {
        #ifdef PT_ENABLE_GT06
            pParam->fnRecv = (void*)GT06_RecvData;
        #endif
        }
        else if (pParam->nStackType == STACK_JT808)
        {
        #ifdef PT_ENABLE_JT808
            pParam->fnRecv = (void*)JT808_RecvData;
        #endif
        }
        else if (pParam->nStackType == STACK_JT808_DV)
        {
        #ifdef PT_ENABLE_JT808_DV
            pParam->fnRecv = (void*)JT808_DV_RecvData;
        #endif
        }
        else if (pParam->nStackType != STACK_JT808_DV && (pParam->nStackType & STACK_JT808_DV))
        {//this protocol must solely work
            pParam->fnRecv = NULL;
        }
        else if (!(    (pParam->nStackType & STACK_BD1) 
                    || (pParam->nStackType & STACK_GT06) 
                    || (pParam->nStackType & STACK_BD2_SP)
                    || (pParam->nStackType & STACK_JT808)
                   ))
        {
             pParam->fnRecv = (void*)PT_RecvData_ASCII;
        }
        else
        {
            pParam->fnRecv = (void*)PT_RecvData_MIX;
        }

        if (pParam->fnRecv != NULL)
        {
            pParam->nRecvFlag = DATA_UNKNOWN;
            pParam->bRecvFrame = 0;
            pParam->nFrameLen = 0;
            pParam->nRecvLen = 0;

            return 0;
        }
    }
	return RESULT_ERROR;
}
/*
*�������ƣ�PT_Uninitialize
*����˵�����ͷ�Э��ջ
*����˵����nStackID ջID
*����ֵ��  ��
*/
void PT_Uninitialize(int nStackID)
{
#if PT_MAX_STACK_NUM > 0
	if (nStackID >= 0 && nStackID < PT_MAX_STACK_NUM)
    {
        PT_UninitializeEx(&m_stackParam[nStackID]);
    }
#endif
}

/*
*�������ƣ�PT_UninitializeEx
*����˵�����ͷ�Э��ջ
*����˵����pParam Э�����ջָ��
*����ֵ��  ��
*/
void PT_UninitializeEx(TStackParam *pParam)
{
	if (pParam)
    {
        pParam->fnRecv = NULL;
        pParam->nRecvFlag = DATA_UNKNOWN;
        pParam->bRecvFrame = 0;
        pParam->nFrameLen = 0;
        pParam->nRecvLen = 0;
    }
}
/*
*�������ƣ�PT_GetRecvData
*����˵������ȡ���ջ��������
*����˵����lpBuff   ���ݻ���ָ��
*          nBuffLen ���ݻ��泤��
*          nStackID ջID
*����ֵ��  0 ��ʾ����ɹ���������ʾʧ��
*/
int PT_GetRecvData(uchar *lpBuff, int nBuffLen, int nStackID)
{
#if PT_MAX_STACK_NUM > 0
    if (nStackID < 0 && nStackID >= PT_MAX_STACK_NUM)//��Ч��ջID
        return RESULT_ERROR;

    if (m_stackParam[nStackID].bRecvFrame == 0)//δ���յ�������֡
        return RESULT_ERROR;

    if (nBuffLen >= m_stackParam[nStackID].nFrameLen)
    {
        memcpy(lpBuff, m_stackParam[nStackID].RecvBuff, m_stackParam[nStackID].nFrameLen);

        return m_stackParam[nStackID].nFrameLen;
    }
#endif   
    return RESULT_ERROR;
}
/*
*�������ƣ�BD1_RecvData
*����˵����������յ�������Э�����ݽṹ
*����˵����nStackID ջID
*          data     ���յ�����
*����ֵ��  -1 ��ʾû���յ���֡���ݣ�������ʾ������֡���ݵ���Ϣ����
*/
int  PT_RecvData_ASCII(TStackParam *pParam, uchar data)
{
    int nRet = RESULT_ERROR;
    if (pParam->nRecvLen == 0)
    {
        if (data == '$')//�յ�֡ͷ
        {
            pParam->RecvBuff[0] = data;
            
            pParam->nRecvLen = 1;//��ʼ���ݽ���
            pParam->bRecvFrame = 0;
            pParam->nCRCValue = 0;
            pParam->bCRC = 1;
            pParam->nFrameLen = 0;
            pParam->nMsgType = -1;
        }
    }
    else 
    {
        pParam->RecvBuff[pParam->nRecvLen++] = data;

        if (pParam->nRecvLen == 6)
        {
            pParam->nMsgType = PT_DecodeHead(pParam->RecvBuff, pParam->nRecvLen, pParam->nStackType);
            if (pParam->nMsgType == INVALID_TYPE)
            {
                pParam->nRecvLen = 0;
            }
        }
        else if (pParam->nRecvLen > 6)
        {
            nRet = PT_RecvData_ASCII_Ex(pParam, data);
            if (nRet == RESULT_ERROR)//
            {
                pParam->nRecvLen = 0;
            }
            else if (nRet > 0) //�յ�����֡
            {
                pParam->nRecvLen = 0;
                return nRet;
            }
        }
    }

    return RESULT_ERROR;
}
/*
*�������ƣ�PT_RecvData_ASCII_Ex
*����˵����������յ�������Э�����ݽṹ
*����˵����nStackID ջID
*          data     ���յ�����
*����ֵ��  -1 ��ʾû���յ���֡���ݣ�������ʾ������֡���ݵ���Ϣ����
*/
int  PT_RecvData_ASCII_Ex(TStackParam *pParam, uchar data)
{
    char szCRC[4];//��������У���ַ����Ƚ�
    char *pszCRC = NULL;

    //if (data == ',' && pParam->RecvBuff[pParam->nRecvLen - 2] == ',')
    //{
    //    pParam->RecvBuff[pParam->nRecvLen - 1] = ' ';//�������ż����ӿո���ں�������ʹ��sscanf����
    //    pParam->RecvBuff[pParam->nRecvLen++] = data;
    //    if (pParam->nRecvLen >= pParam->nRecvBuffLen)//�������
    //    {
    //        return RESULT_ERROR;
    //    }
    //}
    //else 
    if (data == '*')//ֹͣ����У��
    {
        pParam->bCRC = 0;
    }

    if (pParam->bCRC)
    {
        pParam->nCRCValue ^= data;
    }

    if (data == 0x0A && pParam->RecvBuff[pParam->nRecvLen - 2] == 0x0D)//��֡�������
    {
        pParam->nCRCValue ^= PT_MakeCRC(pParam->RecvBuff + 1, 5);
        //����У��
        sprintf(szCRC, "%02X", pParam->nCRCValue & 0xFF);
        pszCRC = &(pParam->RecvBuff[pParam->nRecvLen - 4]);
		pszCRC[0] = toupper(pszCRC[0]);
		pszCRC[1] = toupper(pszCRC[1]);
        if (strncmp(szCRC, pszCRC, 2) == 0)//У����ȷ
        {
            pParam->nFrameLen = pParam->nRecvLen;
            pParam->RecvBuff[pParam->nRecvLen] = 0;
            pParam->bRecvFrame = 1;

            return pParam->nMsgType;
        }
        else
        {
            return RESULT_ERROR;
        }
    }
    else if (pParam->nRecvLen >= pParam->nRecvBuffLen)//�������
    {
        return RESULT_ERROR;
    }

    return RESULT_SUCCESS;
}
/*
*�������ƣ�PT_RecvData_MIX
*����˵�������յĻ��Э������
*����˵����pParam   ջ����ָ��
*          data     ���յ�����
*����ֵ��  -1 ��ʾû���յ���֡���ݣ�������ʾ������֡���ݵ���Ϣ����
*/
int  PT_RecvData_MIX(TStackParam *pParam, uchar data)
{
    char *ptr = NULL;
    int nRet = RESULT_ERROR;

    pParam->RecvBuff[pParam->nRecvLen++] = data;
    if (pParam->nRecvFlag > DATA_UNKNOWN)
    {
        nRet = m_stackFunc[pParam->nRecvFlag].fnRecvData(pParam, data);
        if (nRet == RESULT_ERROR)//
        {
            pParam->nRecvFlag = DATA_UNKNOWN;
            pParam->nRecvLen = 0;
        }
        else if (nRet > 0) //�յ�����֡
        {
            pParam->nRecvFlag = DATA_UNKNOWN;
            pParam->nRecvLen = 0;
            return nRet;
        }
    }
    else if (pParam->nRecvLen >= 6)//���֡ͷ��һ��5�ֽڣ�����6�ֽ�
    {
        ptr = pParam->RecvBuff + pParam->nRecvLen - 6;
        pParam->nMsgType = PT_DecodeHead(ptr, 6, pParam->nStackType);
        if (pParam->nMsgType != INVALID_TYPE)
        {
            pParam->nRecvFlag = PT_GetDataType(pParam->nMsgType);
            if (pParam->nRecvFlag != DATA_UNKNOWN)
            {
                if (m_stackFunc[pParam->nRecvFlag].fnRecvData)//��ʼ���ݽ���
                {
                    pParam->bRecvFrame = 0;
                    pParam->nCRCValue = 0;
                    pParam->bCRC = 1;
                    pParam->nFrameLen = 0;

                    if (pParam->nRecvLen > 6)
                    {
                        memcpy(pParam->RecvBuff, ptr, 6);
                        pParam->nRecvLen = 6;
                    }
                }
            }
        }

        if (pParam->nRecvLen >= pParam->nRecvBuffLen)//�������
        {
            memcpy(pParam->RecvBuff, ptr, 5);
            pParam->nRecvLen = 5;
            pParam->nRecvFlag = DATA_UNKNOWN;//������֡����
        }
    }

    return RESULT_ERROR;
}
/*
*�������ƣ�PT_RecvData
*����˵����������յ�������Э�����ݽṹ
*����˵����nStackID ջID
*          data     ���յ�����
*����ֵ��  -1 ��ʾû���յ���֡���ݣ�������ʾ������֡���ݵ���Ϣ����
*/
int  PT_RecvData(int nStackID, uchar data)
{
	
#if PT_MAX_STACK_NUM > 0
    if (nStackID >= 0 && nStackID < PT_MAX_STACK_NUM)
    {
        return PT_RecvDataEx(&m_stackParam[nStackID], data);
    }
#endif
    
    return RESULT_ERROR;
}
/*
*�������ƣ�PT_RecvDataEx
*����˵������������
*����˵����pParam   ջ����ָ��
*          data     ���յ�����
*����ֵ��  -1 ��ʾû���յ���֡���ݣ�������ʾ������֡���ݵ���Ϣ����
*/
int  PT_RecvDataEx(TStackParam *pParam, uchar data)
{
    if (pParam && pParam->fnRecv)
    {
        return ((FUNC_RECV)pParam->fnRecv)(pParam, data);
    }

    return RESULT_ERROR;
}

/*
*�������ƣ�PT_DecodeRecvData
*����˵����������յ�������Э�����ݽṹ
*����˵����pPackage �ṹ��ָ��
*          nStackID ջID
*����ֵ��  0 ��ʾ����ɹ���������ʾʧ��
*/
int PT_DecodeRecvData(void *pPackage, int nStackID)
{
    int nRet = RESULT_ERROR;
#if PT_MAX_STACK_NUM > 0	
    if (nStackID >= 0 && nStackID < PT_MAX_STACK_NUM)
    {
        nRet = PT_DecodeRecvDataEx(pPackage, &m_stackParam[nStackID]);
    }
#endif
    return nRet;
}
/*
*�������ƣ�PT_DecodeRecvDataEx
*����˵��������Э��ջ���յ�������Э�����ݽṹ
*����˵����pPackage �ṹ��ָ��
*          nStackID ջID
*����ֵ��  0 ��ʾ����ɹ���������ʾʧ��
*/
int PT_DecodeRecvDataEx(void *pPackage, TStackParam *pParam)
{
    int nDataType;
    FUNC_DECODER fnDecoder = NULL;

    if (pParam->bRecvFrame == 0)//δ���յ�������֡
        return RESULT_ERROR;

    nDataType = PT_GetDataType(pParam->nMsgType);
    if (nDataType != DATA_UNKNOWN)
    {
        if (m_stackFunc[nDataType].fnPkgDecoder(pPackage, pParam->nMsgType, pParam->RecvBuff, pParam->nFrameLen) >= RESULT_SUCCESS)
        {
            return pParam->nMsgType;
        }
    }

    return RESULT_ERROR;
//     fnDecoder = PT_GetDecoder(pParam->nMsgType);
//     if ( fnDecoder)
//     {
//         return fnDecoder(pPackage, pParam->RecvBuff, pParam->nFrameLen);
//     }
    
    return RESULT_ERROR;
}

int PT_EncodeRecvData(void *pPackage, int nStackID)
{
    int nRet = RESULT_ERROR;
#if PT_MAX_STACK_NUM > 0	
    if (nStackID >= 0 && nStackID < PT_MAX_STACK_NUM)
    {
        nRet = PT_EncodeRecvDataEx(pPackage, &m_stackParam[nStackID]);
    }
#endif
    return nRet;
}

int PT_EncodeRecvDataEx(void *pPackage, TStackParam *pParam)
{
	FUNC_ENCODER fnEncoder = NULL;
	fnEncoder = PT_GetEncoder(pParam->nMsgType);
    if ( fnEncoder)
    {
        return fnEncoder(pPackage, pParam->nFrameLen, pParam->RecvBuff);
    }

    return -1;
}
/*
*�������ƣ�PT_DecodeType
*����˵����������յ���֡���ݵ���Ϣ����
*����˵����buf    �������ݻ���
*          len    �������ݳ���
*          nStackType Э��ջ����
*����ֵ��  -1 ��ʾ��Ч����Ϣ���ͣ�������ʾ����������
*/
int PT_DecodeType(const uchar* lpBuff, int nBuffLen, int nStackType)
{
    return PT_DecodeHead(lpBuff, nBuffLen, nStackType);
}
/*
*�������ƣ�PT_DecodePackage
*����˵�������������֡������Э�����ݽṹ
*����˵����pPackage  ���ݽṹָ��
*          lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          nStackType Э��ջ����
*����ֵ��  0 ��ʾ����ɹ���������ʾʧ��
*/
int PT_DecodePackage_ASCII(void *pPackage, int nMsgType, uchar *lpBuff, int nBuffLen)
{
    char *pCRC = NULL;
    uchar nCRC = 0;
    char szCRC[4];
    int nFrameLen = 0;

    FUNC_DECODER fnDecoder = NULL;
    fnDecoder = PT_GetDecoder(nMsgType);
    if (fnDecoder == NULL)
    {
        return RESULT_ERROR;
    }

    for (nFrameLen = 1; nFrameLen < nBuffLen; nFrameLen++)
    {
        if (lpBuff[nFrameLen] == '*')
        {
            pCRC = &lpBuff[nFrameLen];
        }
        else if (lpBuff[nFrameLen] == 0x0A)
        {
            break;
        }

        if (pCRC == NULL)
        {
            nCRC ^= lpBuff[nFrameLen];           
        }
    }

    if (lpBuff[nFrameLen - 1] == 0x0D && pCRC)
    {
        pCRC++;

        sprintf(szCRC, "%02X", nCRC & 0xFF);
		pCRC[0] = toupper(pCRC[0]);
		pCRC[1] = toupper(pCRC[1]);
        if (strncmp(szCRC, pCRC, 2) == 0)//У����ȷ
        {
            return fnDecoder(pPackage, lpBuff, nFrameLen);
        }
    }

    return RESULT_ERROR;
}
/*
*�������ƣ�PT_DecodePackage
*����˵�������������֡������Э�����ݽṹ
*����˵����pPackage  ���ݽṹָ��
*          lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          nStackType Э��ջ����
*����ֵ��  0 ��ʾ����ɹ���������ʾʧ��
*/
int PT_DecodePackage(void *pPackage, uchar *lpBuff, int nBuffLen, int nStackType)
 {
    int nDataType;
    int nMsgType = PT_DecodeHead(lpBuff, nBuffLen, nStackType);

    nDataType = PT_GetDataType(nMsgType);

    if (nDataType != DATA_UNKNOWN)
    {
        if (m_stackFunc[nDataType].fnPkgDecoder(pPackage, nMsgType, lpBuff, nBuffLen) >= RESULT_SUCCESS)
        {
            return nMsgType;
        }
    }

    return RESULT_ERROR;
}

/*
*�������ƣ�PT_EncodePackage_ASCII
*����˵������������
*����˵����lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          pMsgData  Э�����ݽṹָ��
*          bDevice   �Ƿ��豸ջ���豸ջ����Э��ͷ����"BD"��1��ʾ��PCջ����"CC"��0��ʾ
*����ֵ��  -1 ��ʾʧ�ܣ�������ʾ���ݳ���
*/
int PT_EncodePackage_ASCII(uchar *lpBuff, int nBuffLen, 
                           void *pPackage, int nMsgType, char bDevice)
{
    int nRet = RESULT_ERROR;
    FUNC_ENCODER fnEncoder = NULL;
    uchar *ptr = lpBuff;
    uchar nCRC;

    fnEncoder = PT_GetEncoder(nMsgType);
    if (fnEncoder == NULL)
        return RESULT_ERROR;
//encode head
    nRet = PT_EncodeHead(ptr, nBuffLen, bDevice, nMsgType);
    if ( nRet == RESULT_ERROR)
    {
        return nRet;
    }
    else
    {
        ptr += nRet;
        nBuffLen -= nRet;
    }
//encode body
    nRet = fnEncoder(ptr, nBuffLen, pPackage);
    if (nRet == RESULT_ERROR)
    {
        return nRet;
    }
    else
    {
        ptr += nRet;
        nBuffLen -= nRet;
    }
//encode CRC and tail
    if (nBuffLen < 5)
        return RESULT_ERROR;

    nCRC = PT_MakeCRC(lpBuff + 1, ptr - lpBuff - 1);//У�鲻����֡ͷ'$'

    nRet = sprintf(ptr, "*%02X\r\n", nCRC);
    ptr += nRet;
    
    return (ptr - lpBuff);
}
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
                       void *pPackage, int nMsgType, char bDevice)
{
    int nDataType = PT_GetDataType(nMsgType);
    if (nDataType != DATA_UNKNOWN)
    {
        if (m_stackFunc[nDataType].fnPkgEncoder)
        {
            return m_stackFunc[nDataType].fnPkgEncoder(lpBuff, nBuffLen, 
                pPackage, nMsgType, bDevice);
        }
    }
    return RESULT_ERROR;
}

#ifdef WIN32
/*
*�������ƣ�PT_GetPackage
*����˵����������Ϣ���ͻ�ȡ��Ӧ�����ݽṹ��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�޴��������ݽṹ����������ʾ���ݽṹ��ָ�룬�ṹ���ݰ�ʹ�����Ӧ���ͷſռ�
*/
void *PT_GetPackage(int nType)
{
#ifdef PT_ENABLE_BD1
    PT_GETPACKAGE(BD1, nType);
#endif//PT_ENABLE_BD1

#ifdef PT_ENABLE_BD2_RD
    PT_GETPACKAGE(BD2_RD, nType);
#endif//PT_ENABLE_BD2_RD

#ifdef PT_ENABLE_BD2_RN
    PT_GETPACKAGE(BD2_RN, nType);
#endif//PT_ENABLE_BD2_RN

#ifdef PT_ENABLE_BD2_EXPRESS
    PT_GETPACKAGE(BD2_EX, nType);
#endif//PT_ENABLE_BD2_EXPRESS

#ifdef PT_ENABLE_BD2_SPECIAL
    PT_GETPACKAGE(BD2_SP, nType);
#endif//PT_ENABLE_BD2_SPECIAL

#ifdef PT_ENABLE_NW
    PT_GETPACKAGE(NW, nType);
#endif//PT_ENABLE_NW
  
#ifdef PT_ENABLE_GPS
    PT_GETPACKAGE(GPS, nType);
#endif//PT_ENABLE_GPS

#ifdef PT_ENABLE_GT06
    PT_GETPACKAGE(GT06, nType);
#endif//PT_ENABLE_GT06

#ifdef PT_ENABLE_JT808
    PT_GETPACKAGE(JT808, nType);
#endif//PT_ENABLE_JT808

#ifdef PT_ENABLE_JT808_DV
    PT_GETPACKAGE(JT808_DV, nType);
#endif//PT_ENABLE_JT808_DV
    return NULL;
}
#endif//end WIN32 define
