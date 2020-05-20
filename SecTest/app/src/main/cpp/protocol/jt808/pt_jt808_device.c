/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     pt_jt808_device.c
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

#include "pt_jt808_device.h"

#ifdef  PT_ENABLE_JT808_DV

extern int PT_Decode_INT32(int *pVal, uchar *lpBuff, int nLen);
extern int PT_Encode_INT32(uchar *lpBuff, int nLen, int nVal);

extern int PT_Decode_SHORT(short *pVal, uchar *lpBuff, int nLen);
extern int PT_Encode_SHORT(uchar *lpBuff, int nLen, short nVal);
//--------------------------------------------------------------------------
//协议栈编解码函数定义
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
//协议栈基本函数
//--------------------------------------------------------------------------
/*
*函数名称：JT808_DV_MakeCRC
*函数说明：计算校验值
*参数说明：buf    接收数据缓存
*          len    接收数据长度
*返回值：  校验值
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
*函数名称：JT808_DV_Translate
*函数说明：计算校验值
*参数说明：buf    接收数据缓存
*          len    接收数据长度
*返回值：  校验值
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
*函数名称：JT808_DV_DecodeHead
*函数说明：解析消息类型
*参数说明：buf    接收数据缓存
*          len    接收数据长度
*返回值：  -1 表示无效的消息类型，其他表示解析的类型
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
*函数名称：JT808_DV_EncodeHead
*函数说明：编码协议头
*参数说明：lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          bDevice   是否设备
*          nType     消息类型
*返回值：  -1 表示失败，其他表示编码数据长度
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
*函数名称：JT808_DV_DecodeHead
*函数说明：解析消息类型
*参数说明：buf    接收数据缓存
*          len    接收数据长度
*返回值：  -1 表示无效的消息类型，其他表示解析的类型
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
*函数名称：JT808_DV_EncodeHeadEx
*函数说明：编码协议头
*参数说明：head    协议头
*          lpBuff  数据缓存长度
*          nBuffLen   是否设备
*返回值：  -1 表示失败，其他表示编码数据长度
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
*函数名称：JT808_DV_GetEncoder
*函数说明：根据消息类型获取相应的编码函数
*参数说明：nType 消息类型
*返回值：  NULL 表示无编码函数，其他表示编码函数指针
*/
FUNC_ENCODER JT808_DV_GetEncoder(int nType)
{
    switch (nType)
    {
    case JT808_DV_POWERON://上电指示
        return JT808_DV_EncodeCommon;
    case JT808_DV_LINKCHECK://链路检测
        return JT808_DV_EncodeLinkCheck;
    case JT808_DV_POWERCTRL://电源控制
        return JT808_DV_EncodePowerCtrl;
    case JT808_DV_VERSION://外设版本检测
        return JT808_DV_EncodeVersion;
    case JT808_DV_IC_AUTHENTICATE:
        return JT808_DV_EncodeICAuthen;
    case JT808_DV_IC_READRESULT:
        return JT808_DV_EncodeICReadResult;
    case JT808_DV_IC_PULLOUT:
    case JT808_DV_IC_READ:
        return JT808_DV_EncodeCommon;
    case JT808_DV_POSITION://外设定位数据
        return JT808_DV_EncodePosition;
    case JT808_DV_PARAM:
        return JT808_DV_EncodeParam;
    default:
        return NULL;
    }
}
/*
*函数名称：JT808_DV_GetDecoder
*函数说明：根据消息类型获取相应的解码函数
*参数说明：nType 消息类型
*返回值：  NULL 表示无解码函数，其他表示解码函数指针
*/
FUNC_DECODER JT808_DV_GetDecoder(int nType)
{
    switch (nType)
    {
    case JT808_DV_POWERON://上电指示
        return JT808_DV_DecodeCommon;
    case JT808_DV_LINKCHECK://链路检测
        return JT808_DV_DecodeLinkCheck;
    case JT808_DV_POWERCTRL://电源控制
        return JT808_DV_DecodePowerCtrl;
    case JT808_DV_VERSION://外设版本检测
        return JT808_DV_DecodeVersion;
    case JT808_DV_IC_AUTHENTICATE:
        return JT808_DV_DecodeICAuthen;
    case JT808_DV_IC_READRESULT:
        return JT808_DV_DecodeICReadResult;
    case JT808_DV_IC_PULLOUT:
    case JT808_DV_IC_READ:
        return JT808_DV_DecodeCommon;
    case JT808_DV_POSITION://外设定位数据
        return JT808_DV_DecodePosition;
    case JT808_DV_PARAM:
        return JT808_DV_DecodeParam;
    default:
        return NULL;
    }
}
/*
*函数名称：JT808_DV_RecvData
*函数说明：解码接收的数据至协议数据结构
*参数说明：pParam 协议接收栈指针
*          data     接收的数据
*返回值：  -1 表示没有收到整帧数据，其他表示所接收帧数据的消息类型
*/
int  JT808_DV_RecvData(TStackParam *pParam, uchar data)
{
    int nRet = RESULT_ERROR;
    if (pParam->nRecvLen == 0)
    {
        if (data == 0x7E)//收到帧头
        {
            pParam->RecvBuff[0] = data;
            
            pParam->nRecvLen = 1;//开始数据接收
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

            if (pParam->nCRCValue == pParam->RecvBuff[1])//校验正确
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
*函数名称：JT808_DV_DecodePackage
*函数说明：解码接收整帧数据至协议数据结构
*参数说明：pPackage  数据结构指针
*          lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          nMsgType  协议类型
*返回值：  0 表示解码成功，其他表示失败
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
*函数名称：JT808_DV_EncodePackage
*函数说明：编码数据
*参数说明：lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          pMsgData  协议数据结构指针
*          bDevice   是否设备栈，设备栈二代协议头编码"BD"用1表示，PC栈编码"CC"用0表示
*返回值：  -1 表示失败，其他表示数据长度
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
*函数名称：NW_GetPackage
*函数说明：根据消息类型获取相应的数据结构包
*参数说明：nType 消息类型
*返回值：  NULL 表示无此类型数据结构包，其他表示数据结构包指针，结构数据包使用完后应当释放空间
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
//协议栈编解码函数实现
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
    else if (dat->head.nDeviceType == JT808_DV_NW_TRANSPORT)//一体机
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

    if (nBuffLen == 0)//下行
    {
        dat->head.nDirect = 1;
    }
    else
    {
        dat->head.nDirect = 0;
        
        dat->nResult= *ptr++;
        nBuffLen--;
        
        if (dat->nResult == 0)//读卡成功
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
            //成为模块
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
    if (dat->head.nDirect == 0)//上行数据
    {
        *ptr++ = dat->nResult;
        
        if (dat->nResult == 0)//读卡成功
        {   //姓名
            nTemp = strlen(dat->szName) + 1;
            *ptr++ = nTemp;
            memcpy(ptr, dat->szName, nTemp);
            ptr += nTemp;
            //证件编号
            memcpy(ptr, dat->szCertificateID, 20);
            ptr += 20;
            //发证机构
            nTemp = strlen(dat->szIssueDepartment) + 1;
            *ptr++ = nTemp;
            memcpy(ptr, dat->szIssueDepartment, nTemp);
            ptr += nTemp;
            //有效期
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
