/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name��     pt_jt808_device.c
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

#include "pt_jt808_device.h"

#ifdef  PT_ENABLE_JT808_DV

extern int PT_Decode_INT32(int *pVal, uchar *lpBuff, int nLen);
extern int PT_Encode_INT32(uchar *lpBuff, int nLen, int nVal);

extern int PT_Decode_SHORT(short *pVal, uchar *lpBuff, int nLen);
extern int PT_Encode_SHORT(uchar *lpBuff, int nLen, short nVal);
//--------------------------------------------------------------------------
//Э��ջ����뺯������
//--------------------------------------------------------------------------
int JT808_DV_DecodeCommon(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_DV_EncodeCommon(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DV_DecodeLinkCheck(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_DV_EncodeLinkCheck(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DV_DecodePowerCtrl(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_DV_EncodePowerCtrl(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DV_DecodeVersion(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_DV_EncodeVersion(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DV_DecodePosition(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_DV_EncodePosition(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DV_DecodeParam(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_DV_EncodeParam(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DV_DecodeICAuthen(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_DV_EncodeICAuthen(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DV_DecodeICReadResult(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_DV_EncodeICReadResult(uchar *lpBuff, int nBuffLen, void *pPackage);
//--------------------------------------------------------------------------
//Э��ջ��������
//--------------------------------------------------------------------------
/*
*�������ƣ�JT808_DV_MakeCRC
*����˵��������У��ֵ
*����˵����buf    �������ݻ���
*          len    �������ݳ���
*����ֵ��  У��ֵ
*/
unsigned char JT808_DV_MakeCRC(uchar *buf, int nLen)
{
    unsigned int nValue = 0;
    int i = 0;
    for (;i < nLen;i++) 
    {
        nValue += buf[i];
    }

    return (nValue & 0xFF);
}
/*
*�������ƣ�JT808_DV_Translate
*����˵��������У��ֵ
*����˵����buf    �������ݻ���
*          len    �������ݳ���
*����ֵ��  У��ֵ
*/
unsigned short JT808_DV_Translate(uchar *buf, int nLen, uchar bEncode, int bufsize)
{
    unsigned short nDataLen = 0;
    unsigned short i = 0;

    if (bEncode)//
    {
        uchar data[PT_MAX_BUFFER_LEN];
        uchar bChanged = 0;
        for (i = 0; i < nLen; i++) 
        {
            if (buf[i] == 0x7D)
            {
                data[nDataLen++] = 0x7D;
                data[nDataLen++] = 0x01;
                bChanged = 1;
            }
            else if (buf[i] == 0x7E)
            {
                data[nDataLen++] = 0x7D;
                data[nDataLen++] = 0x02;
                bChanged = 1;
            }
            else if (buf[i] == 0x0D)
            {
                data[nDataLen++] = 0x7D;
                data[nDataLen++] = 0x03;
                bChanged = 1;
            }
            else if (buf[i] == 0x0A)
            {
                data[nDataLen++] = 0x7D;
                data[nDataLen++] = 0x04;
                bChanged = 1;
            }
            else
            {
                data[nDataLen++] = buf[i];
            }
        }

        if (bChanged)
        {
            if (nDataLen <= bufsize)
                memcpy(buf, data, nDataLen);
            else 
                return -1;
        }
    }
    else//decode
    {
        for (i = 0; i < nLen - 1; i++) 
        {
            if (buf[i] == 0x7D && buf[i + 1] == 0x02)
            {
                buf[nDataLen++] = 0x7E;
                i++;
            }
            else if (buf[i] == 0x7D && buf[i + 1] == 0x01)
            {
                buf[nDataLen++] = 0x7D;
                i++;
            }
            else if (buf[i] == 0x7D && buf[i + 1] == 0x03)
            {
                buf[nDataLen++] = 0x0D;
                i++;
            }
            else if (buf[i] == 0x7D && buf[i + 1] == 0x04)
            {
                buf[nDataLen++] = 0x0A;
                i++;
            }
            else
            {
                buf[nDataLen++] = buf[i];
            }
        }

        if (i < nLen)
        {
            buf[nDataLen++] = buf[i];
        }
    }

    return nDataLen;
}
/*
*�������ƣ�JT808_DV_DecodeHead
*����˵����������Ϣ����
*����˵����buf    �������ݻ���
*          len    �������ݳ���
*����ֵ��  -1 ��ʾ��Ч����Ϣ���ͣ�������ʾ����������
*/
int JT808_DV_DecodeHead(const uchar* buf, int len)
{
    if (len >= 8 && (buf[0] == 0x7E))
    {
        switch (buf[7])
        {
        case 0x01:
            return JT808_DV_POWERON;
        case 0x02:
            return JT808_DV_LINKCHECK;
        case 0x03:
            return JT808_DV_POWERCTRL;
        case 0x04:
            return JT808_DV_VERSION;
        case 0x05:
            return JT808_DV_SELFCHECK;
        case 0x06:
            return JT808_DV_LIVEUPDATE;
        case 0x40:
            return JT808_DV_IC_AUTHENTICATE;
        case 0x41:
            return JT808_DV_IC_READRESULT;
        case 0x42:
            return JT808_DV_IC_PULLOUT;
        case 0x43:
            return JT808_DV_IC_READ;
        case 0x50:
            return JT808_DV_POSITION;
        case 0x51:
            return JT808_DV_PARAM;
        default:
            return -1;
        }
    }

	return -1;
}
/*
*�������ƣ�JT808_DV_EncodeHead
*����˵��������Э��ͷ
*����˵����lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          bDevice   �Ƿ��豸
*          nType     ��Ϣ����
*����ֵ��  -1 ��ʾʧ�ܣ�������ʾ�������ݳ���
*/
int JT808_DV_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType)
{
    if (nBuffLen < 10)
        return -1;

    if (JT808_DV_ISTYPE(nType))
    {
        lpBuff[0] = 0x7E;
        lpBuff[1] = 0;//checksum

        return 2;
    }

    return -1;
}

/*
*�������ƣ�JT808_DV_DecodeHead
*����˵����������Ϣ����
*����˵����buf    �������ݻ���
*          len    �������ݳ���
*����ֵ��  -1 ��ʾ��Ч����Ϣ���ͣ�������ʾ����������
*/
int JT808_DV_DecodeHeadEx(TJT808DVHead *head, uchar* lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;

    if (nBuffLen < 6)
        return -1;

    nRet = PT_Decode_SHORT(&(head->nVersion), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Decode_SHORT(&(head->nManufacturerID), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    head->nDeviceType = *ptr++;
    nBuffLen--;

    //msgtype
    *ptr++;

    return ptr - lpBuff;
}
/*
*�������ƣ�JT808_DV_EncodeHeadEx
*����˵��������Э��ͷ
*����˵����head    Э��ͷ
*          lpBuff  ���ݻ��泤��
*          nBuffLen   �Ƿ��豸
*����ֵ��  -1 ��ʾʧ�ܣ�������ʾ�������ݳ���
*/
int JT808_DV_EncodeHeadEx(TJT808DVHead *head, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, head->nVersion);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, head->nManufacturerID);
    ptr += nRet;
    nBuffLen -= nRet;

    *ptr++ = head->nDeviceType;
    nBuffLen--;

    switch (head->nMsgType)
    {
    case JT808_DV_POWERON:
        *ptr++ = 0x01;
        break;
    case JT808_DV_LINKCHECK:
        *ptr++ = 0x02;
        break;
    case JT808_DV_POWERCTRL:
        *ptr++ = 0x03;
        break;
    case JT808_DV_VERSION:
        *ptr++ = 0x04;
        break;
    case JT808_DV_SELFCHECK:
        *ptr++ = 0x05;
        break;
    case JT808_DV_LIVEUPDATE:
        *ptr++ = 0x06;
        break;
    case JT808_DV_IC_AUTHENTICATE:
        *ptr++ = 0x40;
        break;
    case JT808_DV_IC_READRESULT:
        *ptr++ = 0x41;
        break;
    case JT808_DV_IC_PULLOUT:
        *ptr++ = 0x42;
        break;
    case JT808_DV_IC_READ:
        *ptr++ = 0x43;
        break;
    case JT808_DV_POSITION:
        *ptr++ = 0x50;
        break;
    case JT808_DV_PARAM:
        *ptr++ = 0x51;
        break;
    default:
        return -1;
    }

    return ptr - lpBuff;
}
/*
*�������ƣ�JT808_DV_GetEncoder
*����˵����������Ϣ���ͻ�ȡ��Ӧ�ı��뺯��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�ޱ��뺯����������ʾ���뺯��ָ��
*/
FUNC_ENCODER JT808_DV_GetEncoder(int nType)
{
    switch (nType)
    {
    case JT808_DV_POWERON://�ϵ�ָʾ
        return JT808_DV_EncodeCommon;
    case JT808_DV_LINKCHECK://��·���
        return JT808_DV_EncodeLinkCheck;
    case JT808_DV_POWERCTRL://��Դ����
        return JT808_DV_EncodePowerCtrl;
    case JT808_DV_VERSION://����汾���
        return JT808_DV_EncodeVersion;
    case JT808_DV_IC_AUTHENTICATE:
        return JT808_DV_EncodeICAuthen;
    case JT808_DV_IC_READRESULT:
        return JT808_DV_EncodeICReadResult;
    case JT808_DV_IC_PULLOUT:
    case JT808_DV_IC_READ:
        return JT808_DV_EncodeCommon;
    case JT808_DV_POSITION://���趨λ����
        return JT808_DV_EncodePosition;
    case JT808_DV_PARAM:
        return JT808_DV_EncodeParam;
    default:
        return NULL;
    }
}
/*
*�������ƣ�JT808_DV_GetDecoder
*����˵����������Ϣ���ͻ�ȡ��Ӧ�Ľ��뺯��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�޽��뺯����������ʾ���뺯��ָ��
*/
FUNC_DECODER JT808_DV_GetDecoder(int nType)
{
    switch (nType)
    {
    case JT808_DV_POWERON://�ϵ�ָʾ
        return JT808_DV_DecodeCommon;
    case JT808_DV_LINKCHECK://��·���
        return JT808_DV_DecodeLinkCheck;
    case JT808_DV_POWERCTRL://��Դ����
        return JT808_DV_DecodePowerCtrl;
    case JT808_DV_VERSION://����汾���
        return JT808_DV_DecodeVersion;
    case JT808_DV_IC_AUTHENTICATE:
        return JT808_DV_DecodeICAuthen;
    case JT808_DV_IC_READRESULT:
        return JT808_DV_DecodeICReadResult;
    case JT808_DV_IC_PULLOUT:
    case JT808_DV_IC_READ:
        return JT808_DV_DecodeCommon;
    case JT808_DV_POSITION://���趨λ����
        return JT808_DV_DecodePosition;
    case JT808_DV_PARAM:
        return JT808_DV_DecodeParam;
    default:
        return NULL;
    }
}
/*
*�������ƣ�JT808_DV_RecvData
*����˵����������յ�������Э�����ݽṹ
*����˵����pParam Э�����ջָ��
*          data     ���յ�����
*����ֵ��  -1 ��ʾû���յ���֡���ݣ�������ʾ������֡���ݵ���Ϣ����
*/
int  JT808_DV_RecvData(TStackParam *pParam, uchar data)
{
    int nRet = RESULT_ERROR;
    if (pParam->nRecvLen == 0)
    {
        if (data == 0x7E)//�յ�֡ͷ
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

        if (data == 0x7E)
        {
            pParam->nFrameLen = JT808_DV_Translate(pParam->RecvBuff + 1, pParam->nRecvLen - 2, 0, pParam->nRecvBuffLen);
            pParam->nFrameLen += 2;
            pParam->RecvBuff[pParam->nFrameLen - 1] = 0x7E;

            pParam->nCRCValue = JT808_DV_MakeCRC(pParam->RecvBuff + 4, pParam->nFrameLen - 5);

            if (pParam->nCRCValue == pParam->RecvBuff[1])//У����ȷ
            {
                pParam->nMsgType = JT808_DV_DecodeHead(pParam->RecvBuff, pParam->nFrameLen);

                if (JT808_DV_ISTYPE(pParam->nMsgType))
                {
                    pParam->bRecvFrame = 1;
                    return pParam->nMsgType;
                }
            }
        }
        else if (pParam->nRecvLen >= pParam->nRecvBuffLen)
        {
            pParam->nRecvLen = 0;
        }
    }

    return RESULT_ERROR;
}
/*
*�������ƣ�JT808_DV_DecodePackage
*����˵�������������֡������Э�����ݽṹ
*����˵����pPackage  ���ݽṹָ��
*          lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          nMsgType  Э������
*����ֵ��  0 ��ʾ����ɹ���������ʾʧ��
*/
int JT808_DV_DecodePackage(void *pPackage, int nMsgType, uchar *lpBuff, int nBuffLen)
{
    unsigned char nTmp;
    TJT808DVHead* head = (TJT808DVHead *)pPackage;

    FUNC_DECODER fnDecoder = NULL;

    fnDecoder = JT808_DV_GetDecoder(nMsgType);
    if (fnDecoder == NULL)
        return RESULT_ERROR;

    if (nBuffLen >= 9)
    {
        nTmp = lpBuff[1];
        if (nTmp == JT808_DV_MakeCRC(lpBuff + 4, nBuffLen - 5))
        {
            head->nMsgType = nMsgType;
            nBuffLen -= 2;

            nTmp = JT808_DV_DecodeHeadEx(head, lpBuff + 2, nBuffLen);
            nBuffLen -= nTmp;

            return fnDecoder(pPackage, lpBuff + 2 + nTmp, nBuffLen - 1);
        }
    }

    return RESULT_ERROR;
}

/*
*�������ƣ�JT808_DV_EncodePackage
*����˵������������
*����˵����lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          pMsgData  Э�����ݽṹָ��
*          bDevice   �Ƿ��豸ջ���豸ջ����Э��ͷ����"BD"��1��ʾ��PCջ����"CC"��0��ʾ
*����ֵ��  -1 ��ʾʧ�ܣ�������ʾ���ݳ���
*/
int JT808_DV_EncodePackage(uchar *lpBuff, int nBuffLen, 
                       void *pPackage, int nMsgType, char bDevice)
{
    int nRet = RESULT_ERROR;
    int nBuffSize = nBuffLen;
    FUNC_ENCODER fnEncoder = NULL;
    TJT808DVHead *head = (TJT808DVHead *)pPackage;
    uchar *ptr = lpBuff;

    fnEncoder = JT808_DV_GetEncoder(nMsgType);
    if (fnEncoder == NULL)
        return RESULT_ERROR;

//encode head
    nRet = JT808_DV_EncodeHead(ptr, nBuffLen, bDevice, nMsgType);
    if (nRet == RESULT_ERROR)
    {
        return nRet;
    }
    else
    {
        ptr += nRet;
        nBuffLen -= nRet;
    }

    head->nMsgType = nMsgType;
    nRet = JT808_DV_EncodeHeadEx(head, ptr, nBuffLen);
    if (nRet == RESULT_ERROR)
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
//encode CRC ,frame tail
    if (nBuffLen < 1)
    {
        return RESULT_ERROR;
    }

    //CRC
    lpBuff[1] = JT808_DV_MakeCRC(lpBuff + 4, ptr - lpBuff - 4);

    nBuffLen = JT808_DV_Translate(lpBuff + 1, ptr - lpBuff - 1, 1, nBuffSize);
    nBuffLen += 1;

    lpBuff[nBuffLen++] = 0x7E;
    
    return nBuffLen;
}

#ifdef WIN32
/*
*�������ƣ�NW_GetPackage
*����˵����������Ϣ���ͻ�ȡ��Ӧ�����ݽṹ��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�޴��������ݽṹ����������ʾ���ݽṹ��ָ�룬�ṹ���ݰ�ʹ�����Ӧ���ͷſռ�
*/
void* JT808_DV_GetPackage(int nType)
{
    switch (nType)
    { 
    default:
        return NULL;
    }
}
#endif //WIN32
//--------------------------------------------------------------------------
//Э��ջ����뺯��ʵ��
//--------------------------------------------------------------------------
int JT808_DV_DecodeCommon(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    return 0;
}

int JT808_DV_EncodeCommon(uchar *lpBuff, int nBuffLen, void *pPackage)
{ 
    return 0;
}

int JT808_DV_DecodeLinkCheck(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    uchar *ptr = lpBuff;
    TJT808DVLinkCheck * dat = (TJT808DVLinkCheck *)pPackage;

    if (nBuffLen > 2)
    {
        dat->head.nDirect = 1;
        dat->nMinute = *ptr++;
        dat->nSecond = *ptr++;
    }
    else
    {
        dat->head.nDirect = 0;
        dat->nMinute = 0;
        dat->nSecond = 0;
    }

    return ptr - lpBuff;
}

int JT808_DV_EncodeLinkCheck(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    uchar *ptr = lpBuff;
    TJT808DVLinkCheck * dat = (TJT808DVLinkCheck *)pPackage;

    if (dat->head.nDirect )
    {
        *ptr++ = dat->nMinute;
        *ptr++ = dat->nSecond;
    }

    return ptr - lpBuff;
}

int JT808_DV_DecodePowerCtrl(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    uchar *ptr = lpBuff;
    TJT808DVPowerCtrl *dat = (TJT808DVPowerCtrl *)pPackage;

    dat->data = *ptr++;
    
    return ptr - lpBuff;
}

int JT808_DV_EncodePowerCtrl(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    uchar *ptr = lpBuff;
    TJT808DVPowerCtrl *dat = (TJT808DVPowerCtrl *)pPackage;

    *ptr++ = dat->data;

    return ptr - lpBuff;
}

int JT808_DV_DecodeVersion(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    uchar *ptr = lpBuff;
    TJT808DVVersion *dat = (TJT808DVVersion *)pPackage;

    dat->nVersion= *ptr++;

    return ptr - lpBuff;
}

int JT808_DV_EncodeVersion(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    uchar *ptr = lpBuff;
    TJT808DVVersion *dat = (TJT808DVVersion *)pPackage;

    *ptr++ = dat->nVersion;

    return ptr - lpBuff;
}

int JT808_DV_DecodePosition(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    uchar *ptr = lpBuff;
    TJT808DVPosition *dat = (TJT808DVPosition *)pPackage;

    dat->nDeviceID = *ptr++;
    nBuffLen--;

    if (nBuffLen >= sizeof(dat->data))
    {
        nBuffLen = sizeof(dat->data) - 1;
    }

    memcpy(dat->data, ptr, nBuffLen);
    dat->data[nBuffLen] = '\0';
    ptr += nBuffLen;

    return ptr - lpBuff;
}

int JT808_DV_EncodePosition(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    uchar *ptr = lpBuff;
    TJT808DVPosition *dat = (TJT808DVPosition *)pPackage;
    int nDataLen = strlen(dat->data);

    *ptr++ = dat->nDeviceID;

    memcpy(ptr, dat->data, nDataLen);
    ptr += nDataLen;

    return ptr - lpBuff;
}

int JT808_DV_DecodeParam(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808DVParam *dat = (TJT808DVParam *)pPackage;
    
    if (dat->head.nDeviceType == JT808_DV_NW_INTERPHONE)
    {
        dat->param.a.nDeviceID = *ptr++;
        dat->param.a.nDeviceType = *ptr++;
        nBuffLen -= 2;

        nRet = PT_Decode_SHORT(&(dat->param.a.nUploadFreq), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;
    }
    else if (dat->head.nDeviceType == JT808_DV_NW_TRANSPORT)
    {
        nRet = PT_Decode_INT32(&(dat->param.b.nUserID), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;

        nRet = PT_Decode_SHORT(&(dat->param.b.nUploadFreq), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;

        nRet = sizeof(dat->param.b.szMobile) - 1;
        if (nRet >= nBuffLen)
        {
            nRet = nBuffLen;
        }
        
        memcpy(dat->param.b.szMobile, ptr, nRet);
        ptr += nBuffLen;
        nBuffLen -= nBuffLen;
    }

    return ptr - lpBuff;
}

int JT808_DV_EncodeParam(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808DVParam *dat = (TJT808DVParam *)pPackage;

    if (dat->head.nDeviceType == JT808_DV_NW_INTERPHONE)
    {
        *ptr++ = dat->param.a.nDeviceID;
        *ptr++ = dat->param.a.nDeviceType;
        nBuffLen -= 2;

        nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->param.a.nUploadFreq);
        ptr += nRet;
        nBuffLen -= nRet;
    }
    else if (dat->head.nDeviceType == JT808_DV_NW_TRANSPORT)//һ���
    {
        nRet = PT_Encode_INT32(ptr, nBuffLen, dat->param.b.nUserID);
        ptr += nRet;
        nBuffLen -= nRet;

        nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->param.b.nUploadFreq);
        ptr += nRet;
        nBuffLen -= nRet;

        nRet = strlen(dat->param.b.szMobile);
        memcpy(ptr, dat->param.b.szMobile, nRet);
        ptr += nRet;
        nBuffLen -= nRet;
    }

    return ptr - lpBuff;
}

int JT808_DV_DecodeICAuthen(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    uchar *ptr = lpBuff;
    TJT808DVICAuthen *dat = (TJT808DVICAuthen *)pPackage;

    dat->nResult= *ptr++;
    nBuffLen--;
    
    dat->nDataLen = nBuffLen;
    if (nBuffLen> 0)
    {
        memcpy(dat->data, ptr, dat->nDataLen);
        
        ptr += dat->nDataLen;
    }

    return ptr - lpBuff;
}


int JT808_DV_EncodeICAuthen(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    uchar *ptr = lpBuff;
    TJT808DVICAuthen *dat = (TJT808DVICAuthen *)pPackage;

    *ptr++ = dat->nResult;
    nBuffLen--;
    
    if (dat->nDataLen > 0)
    {
        memcpy(ptr, dat->data, dat->nDataLen);
        
        ptr += dat->nDataLen;
    }

    return ptr - lpBuff;
}

int JT808_DV_DecodeICReadResult(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    unsigned char nTemp;
    uchar *ptr = lpBuff;
    TJT808DVICReadResult *dat = (TJT808DVICReadResult *)pPackage;

    if (nBuffLen == 0)//����
    {
        dat->head.nDirect = 1;
    }
    else
    {
        dat->head.nDirect = 0;
        
        dat->nResult= *ptr++;
        nBuffLen--;
        
        if (dat->nResult == 0)//�����ɹ�
        {
            nTemp = *ptr++;
            nBuffLen--;
            
            if (nTemp > 0)
            {
                memcpy(dat->szName, ptr, nTemp);
                ptr += nTemp;
                nBuffLen -= nTemp;
            }
            
            if (nBuffLen > 20)
            {
                nTemp = 20;
                memcpy(dat->szCertificateID, ptr, nTemp);
                ptr += nTemp;   
                nBuffLen -= nTemp;
            }
            
            nTemp = *ptr++;
            nBuffLen--;
            //��Ϊģ��
            nTemp = nBuffLen - 4;
            if (nTemp > 0)
            {
                memcpy(dat->szIssueDepartment, ptr, nTemp);
                dat->szIssueDepartment[nTemp] = '\0';
                ptr += nTemp;
                nBuffLen -= nTemp;
            }
            
            if (nBuffLen >= 4)
            {
                memcpy(dat->szUsefulLife, ptr, 4);
                dat->nYear = (*ptr >> 4) * 1000 + (*ptr & 0xF) * 100;
                ptr++;
                dat->nYear += (*ptr >> 4) * 10 + (*ptr & 0xF);
                ptr++;
                dat->nMonth = (*ptr >> 4) * 10 + (*ptr & 0xF);
                ptr++;
                dat->nDay = (*ptr >> 4) * 10 + (*ptr & 0xF);
            }
        }
    }
    return ptr - lpBuff;
}

int JT808_DV_EncodeICReadResult(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    uchar *ptr = lpBuff;
    unsigned char nTemp = 0;
    TJT808DVICReadResult *dat = (TJT808DVICReadResult *)pPackage;
    if (dat->head.nDirect == 0)//��������
    {
        *ptr++ = dat->nResult;
        
        if (dat->nResult == 0)//�����ɹ�
        {   //����
            nTemp = strlen(dat->szName) + 1;
            *ptr++ = nTemp;
            memcpy(ptr, dat->szName, nTemp);
            ptr += nTemp;
            //֤�����
            memcpy(ptr, dat->szCertificateID, 20);
            ptr += 20;
            //��֤����
            nTemp = strlen(dat->szIssueDepartment) + 1;
            *ptr++ = nTemp;
            memcpy(ptr, dat->szIssueDepartment, nTemp);
            ptr += nTemp;
            //��Ч��
            nTemp = dat->nYear / 100;
            *ptr++ = ((nTemp / 10) << 4 )| (nTemp % 10);
            
            nTemp = dat->nYear % 100;
            *ptr++ = ((nTemp / 10) << 4 )| (nTemp % 10);
            
            *ptr++ = ((dat->nMonth / 10) << 4 )| (dat->nMonth % 10);
            *ptr++ = ((dat->nDay / 10) << 4 )| (dat->nDay % 10);
        }
    }
    
    return ptr - lpBuff;
}
///////////////////////////////////////
#endif//end PT_ENABLE_JT808_DV
