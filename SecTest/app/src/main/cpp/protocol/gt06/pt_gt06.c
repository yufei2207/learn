/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     pt_gt06.c
* Abstract： GT06 protocol 
* 
* Version： 1.0
* Author：  yfwang
* Date：    2012/03/19
*
* history   :
*	     when            who    	          what:
*      -------------   --------------    -------------------
*      Mar 19, 2012   yfwang                create
*
*/

#include "pt_gt06.h"

#ifdef  PT_ENABLE_GT06
extern int PT_Decode_INT32(int *pVal, uchar *lpBuff, int nLen);
extern int PT_Encode_INT32(uchar *lpBuff, int nLen, int nVal);

extern int PT_Decode_SHORT(short *pVal, uchar *lpBuff, int nLen);
extern int PT_Encode_SHORT(uchar *lpBuff, int nLen, short nVal);
//--------------------------------------------------------------------------
//协议栈编解码函数定义
//--------------------------------------------------------------------------
int GT_DecodeLogin(void *pPackage, uchar *lpBuff, int nBuffLen);
int GT_EncodeLogin(uchar *lpBuff, int nDataLen, void *pPackage);

int GT_DecodePosition(void *pPackage, uchar *lpBuff, int nBuffLen);
int GT_EncodePosition(uchar *lpBuff, int nDataLen, void *pPackage);

int GT_DecodeState(void *pPackage, uchar *lpBuff, int nBuffLen);
int GT_EncodeState(uchar *lpBuff, int nBuffLen, void *pPackage);

int GT_DecodeRes(void *pPackage, uchar *lpBuff, int nBuffLen);
int GT_EncodeRes(uchar *lpBuff, int nBuffLen, void *pPackage);

int GT_DecodeCMD(void *pPackage, uchar *lpBuff, int nBuffLen);
int GT_EncodeCMD(uchar *lpBuff, int nBuffLen, void *pPackage);

int GT_DecodePhoto(void *pPackage, uchar *lpBuff, int nBuffLen);
int GT_EncodePhoto(uchar *lpBuff, int nBuffLen, void *pPackage);
//--------------------------------------------------------------------------
//协议栈基本函数
//--------------------------------------------------------------------------
//CRC table
static unsigned short GT06_CRC16_TABLE[] = 
{		  	
    0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
    0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
    0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
    0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
    0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
    0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
    0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
    0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
    0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
    0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
    0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
    0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
    0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
    0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
    0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
    0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
    0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
    0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
    0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
    0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
    0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
    0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
    0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
    0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
    0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
    0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
    0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
    0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
    0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
    0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
    0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
    0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};	

/*
*函数名称：GT06_MakeCRC
*函数说明：计算校验值
*参数说明：buf    接收数据缓存
*          len    接收数据长度
*返回值：  校验值
*/
unsigned short GT06_MakeCRC(uchar *buf, int nLen)
{
    unsigned short nValue = 0xffff;
    uchar i = 0;
    for (;i < nLen;i++) 
    {
        // 1.value 右移8位(相当于除以256)
        // 2.value与进来的数据进行异或运算后再与0xFF进行与运算
        //    得到一个索引index，然后查找CRC16_TABLE表相应索引的数据
        // 1和2得到的数据再进行异或运算。
        nValue = (nValue >> 8) ^ GT06_CRC16_TABLE[(nValue ^ buf[i]) & 0xff];			
    }

    return nValue;
}


/*
*函数名称：GT06_DecodeHead
*函数说明：解析消息类型
*参数说明：buf    接收数据缓存
*          len    接收数据长度
*返回值：  -1 表示无效的消息类型，其他表示解析的类型
*/
int GT06_DecodeHead(const uchar* buf, int len)
{
    int nFrameLen = 0;
    if (len >= 4 && (buf[0] == 'x' && buf[1] == 'x'))
    {
        nFrameLen = buf[2] + 5;
	    switch (buf[3])
        {
        case 0x01://login req or res
            if (len == 10)
                return GT06_LOGINRES;// server -> client
            else
                return GT06_LOGINREQ;//client -> server
        case 0x12://gps position
            return GT06_POSITION;//client -> server
        case 0x13://state
            if (len <= 10)
                return GT06_STATERES;// server -> client
            else
                return GT06_STATEREQ;//client -> server
        case 0x15://control response //client -> server
            return GT06_CMDRES;
        case 0x80://control command// server -> client
            return GT06_CMDREQ;
        case 0x20://photo conversation
            return GT06_PHOTO;
        default:
            return -1;
        }
    }

	return -1;
}
/*
*函数名称：GT06_EncodeHead
*函数说明：编码协议头
*参数说明：lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          bDevice   是否设备
*          nType     消息类型
*返回值：  -1 表示失败，其他表示编码数据长度
*/
int GT06_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType)
{
    uchar *pHead = NULL;
    int nLen = 0;

    if (nBuffLen < 4)
        return -1;

    if (GT06_ISTYPE(nType))
    {
        lpBuff[0] = 'x';
        lpBuff[1] = 'x';
        lpBuff[2] = 0;
        lpBuff[3] = 0x0;

        switch (nType)
        {
        case GT06_LOGINREQ://登录请求
        case GT06_LOGINRES://登录响应
            lpBuff[3] = 0x01;
            break;
        case GT06_POSITION://定位
            lpBuff[3] = 0x12;
            break;
        case GT06_STATEREQ://状态信息请求
        case GT06_STATERES://状态信息响应
            lpBuff[3] = 0x13;
            break;
        case GT06_CMDREQ://控制指令
            lpBuff[3] = 0x80;
            break;
        case GT06_CMDRES://控制指令响应
            lpBuff[3] = 0x15;
            break;
        case GT06_PHOTO:
            lpBuff[3] = 0x20;
        default:
            break;
        }

        return (lpBuff[3] ? 4 : -1);
    }

    return -1;
}
/*
*函数名称：GT06_GetEncoder
*函数说明：根据消息类型获取相应的编码函数
*参数说明：nType 消息类型
*返回值：  NULL 表示无编码函数，其他表示编码函数指针
*/
FUNC_ENCODER GT06_GetEncoder(int nType)
{
    switch (nType)
    {
    case GT06_LOGINREQ://登录请求
        return GT_EncodeLogin;
    case GT06_LOGINRES://登录响应
        return GT_EncodeRes;
    case GT06_POSITION://定位
        return GT_EncodePosition;
    case GT06_STATEREQ://状态信息请求
        return GT_EncodeState;
    case GT06_STATERES://状态信息响应
        return GT_EncodeRes;
    case GT06_CMDREQ://控制指令
        return GT_EncodeCMD;
    case GT06_CMDRES://控制指令响应
        return GT_EncodeCMD;
    case GT06_PHOTO:
        return GT_EncodePhoto;
    default:
        return NULL;
    }
}
/*
*函数名称：GT06_GetDecoder
*函数说明：根据消息类型获取相应的解码函数
*参数说明：nType 消息类型
*返回值：  NULL 表示无解码函数，其他表示解码函数指针
*/
FUNC_DECODER GT06_GetDecoder(int nType)
{
    switch (nType)
    {
    case GT06_LOGINREQ://登录请求
        return GT_DecodeLogin;
    case GT06_LOGINRES://登录响应
        return GT_DecodeRes;
    case GT06_POSITION://定位
        return GT_DecodePosition;
    case GT06_STATEREQ://状态信息请求
        return GT_DecodeState;
    case GT06_STATERES://状态信息响应
        return GT_DecodeRes;
    case GT06_CMDREQ://控制指令
        return GT_DecodeCMD;
    case GT06_CMDRES://控制指令响应
        return GT_DecodeCMD;
    case GT06_PHOTO:
        return GT_DecodePhoto;
    default:
        return NULL;
    }
}
/*
*函数名称：GT06_RecvData
*函数说明：解码接收的数据至协议数据结构
*参数说明：pParam 协议接收栈指针
*          data     接收的数据
*返回值：  -1 表示没有收到整帧数据，其他表示所接收帧数据的消息类型
*/
int  GT06_RecvData(TStackParam *pParam, uchar data)
{
    int nRet = RESULT_ERROR;
    if (pParam->nRecvLen == 0)
    {
        if (data == 'x')//收到帧头
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

        if (pParam->nRecvLen == 4)
        {
            pParam->nMsgType = GT06_DecodeHead(pParam->RecvBuff, pParam->nRecvLen);
            if (pParam->nMsgType == INVALID_TYPE)
            {
                pParam->nRecvLen = 0;
            }
        }
        else if (pParam->nRecvLen >= 7)
        {
            nRet = GT06_RecvDataEx(pParam, data);
            if (nRet == RESULT_ERROR)//
            {
                pParam->nRecvLen = 0;
            }
            else if (nRet > 0) //收到完整帧
            {
                pParam->nRecvLen = 0;
                return nRet;
            }
        }
    }

    return RESULT_ERROR;
}
/*
*函数名称：GT06_RecvDataEx
*函数说明：解码接收的数据至协议数据结构
*参数说明：pParam   栈
*          data     接收的数据
*返回值：  -1 表示没有收到整帧数据，其他表示所接收帧数据的消息类型
*/
int  GT06_RecvDataEx(TStackParam *pParam, uchar data)
{
    unsigned short nCRC ;
    if (pParam->nRecvLen == 7) 
    {
        pParam->nFrameLen = pParam->RecvBuff[2] + 5;//6=headlen(2)+datalen(1)+ptlen(1)+taillen(2)
        if (pParam->nFrameLen <= 7 || pParam->nFrameLen > pParam->nRecvBuffLen)//帧长无效
        {
            return RESULT_ERROR;
        }
        else
        {
            if (pParam->nFrameLen - 4 <= 7)
            {
                pParam->nCRCValue = GT06_MakeCRC(pParam->RecvBuff + 2, pParam->nFrameLen - 6);
                pParam->bCRC = 0;
            }
            else
            {
                pParam->nCRCValue = GT06_MakeCRC(pParam->RecvBuff + 2, 5);
            }
        }
    }
    else if (pParam->nRecvLen > 7 )
    {
        if (pParam->bCRC)
        {
            pParam->nCRCValue = (pParam->nCRCValue >> 8) ^ GT06_CRC16_TABLE[(pParam->nCRCValue ^ data) & 0xff];
        }

        if (pParam->nRecvLen == pParam->nFrameLen)//本帧接收完成
        {
            PT_Decode_SHORT(&nCRC, pParam->RecvBuff + pParam->nFrameLen - 4, 2);
            nCRC = ~nCRC;
            if (pParam->nCRCValue == nCRC)//校验正确
            {
                pParam->bRecvFrame = 1;
                return pParam->nMsgType;
            }
            else
            {
                return RESULT_ERROR;
            }
        }
        else if (pParam->nRecvLen == pParam->nFrameLen - 4)
        {
            pParam->bCRC = 0;
        }
    }
    
    return RESULT_SUCCESS;
}
/*
*函数名称：GT06_DecodePackage
*函数说明：解码接收整帧数据至协议数据结构
*参数说明：pPackage  数据结构指针
*          lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          nMsgType  协议类型
*返回值：  0 表示解码成功，其他表示失败
*/
int GT06_DecodePackage(void *pPackage, int nMsgType, uchar *lpBuff, int nBuffLen)
{
    unsigned short nCRC;
    int nFrameLen = 0;

    FUNC_DECODER fnDecoder = NULL;

    fnDecoder = GT06_GetDecoder(nMsgType);
    if (fnDecoder == NULL)
        return RESULT_ERROR;

    if (nBuffLen > 3 && GT06_ISTYPE(nMsgType))
    {
         nFrameLen = lpBuff[2] + 5; 
    }

    if (nFrameLen > 3 && nFrameLen <= nBuffLen)
    {
        PT_Decode_SHORT(&nCRC, lpBuff + nFrameLen - 4, 2);
        nCRC = ~nCRC;
        if (nCRC == GT06_MakeCRC(lpBuff + 2, nFrameLen - 6))
        {
            return fnDecoder(pPackage, lpBuff, nFrameLen);
        }
    }

    return RESULT_ERROR;
}

/*
*函数名称：GT06_EncodePackage
*函数说明：编码数据
*参数说明：lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          pMsgData  协议数据结构指针
*          bDevice   是否设备栈，设备栈二代协议头编码"BD"用1表示，PC栈编码"CC"用0表示
*返回值：  -1 表示失败，其他表示数据长度
*/
int GT06_EncodePackage(uchar *lpBuff, int nBuffLen, 
                       void *pPackage, int nMsgType, char bDevice)
{
    int nRet = RESULT_ERROR;
    FUNC_ENCODER fnEncoder = NULL;
    uchar *ptr = lpBuff;
    unsigned short nCRC;

    fnEncoder = GT06_GetEncoder(nMsgType);
    if (fnEncoder == NULL)
        return RESULT_ERROR;

//encode head
    nRet = GT06_EncodeHead(ptr, nBuffLen, bDevice, nMsgType);
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
//encode length, CRC ,frame tail
    if (nBuffLen < 4)
    {
        return RESULT_ERROR;
    }

    //length include frame tail char（0x0d0x0a）and crc(two bytes)
    lpBuff[2] =  (ptr - lpBuff) - 1;
    
    nCRC = GT06_MakeCRC(lpBuff + 2, lpBuff[2] - 1);
    nRet = PT_Encode_SHORT(ptr, nBuffLen, ~nCRC);
    ptr += nRet;
    nBuffLen -= nRet;

    *ptr++ = 0x0D;
    *ptr++ = 0x0A;
        
    
    return (ptr - lpBuff);
}

#ifdef WIN32
/*
*函数名称：NW_GetPackage
*函数说明：根据消息类型获取相应的数据结构包
*参数说明：nType 消息类型
*返回值：  NULL 表示无此类型数据结构包，其他表示数据结构包指针，结构数据包使用完后应当释放空间
*/
void* GT06_GetPackage(int nType)
{
    switch (nType)
    { 
    case GT06_LOGINREQ://登录请求
        return malloc(sizeof(TGTLOGIN));
    case GT06_LOGINRES://登录响应
        return malloc(sizeof(TGTLOGINRES));
    case GT06_POSITION://定位
        return malloc(sizeof(TGTPOSITON));
    case GT06_STATEREQ://状态信息请求
        return malloc(sizeof(TGTSTATEREQ));
    case GT06_STATERES://状态信息响应
        return malloc(sizeof(TGTSTATERES));
    case GT06_CMDREQ://控制指令
        return malloc(sizeof(TGTCMDREQ));
    case GT06_CMDRES://控制指令响应
        return malloc(sizeof(TGTCMDRES));
    case GT06_PHOTO:
        return malloc(sizeof(TGTPHOTO));
    default:
        return NULL;
    }
}
#endif //WIN32
//--------------------------------------------------------------------------
//协议栈编解码函数实现
//--------------------------------------------------------------------------
int GT_DecodeLogin(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr;
    TGTLOGIN *pLogin = (TGTLOGIN *)pPackage;

    ptr = lpBuff + 4;
    nBuffLen -= 4;

    pLogin->szDeviceID[0] = (*ptr++) + 0x30;

    pLogin->szDeviceID[1] = ((*ptr >> 4) & 0x0f) + 0x30;
    pLogin->szDeviceID[2] = ((*ptr++)  & 0x0f) + 0x30;

    pLogin->szDeviceID[3] = ((*ptr >> 4) & 0x0f) + 0x30;
    pLogin->szDeviceID[4] = ((*ptr++)  & 0x0f) + 0x30;

    pLogin->szDeviceID[5] = ((*ptr >> 4) & 0x0f) + 0x30;
    pLogin->szDeviceID[6] = ((*ptr++)  & 0x0f) + 0x30;

    pLogin->szDeviceID[7] = ((*ptr >> 4) & 0x0f) + 0x30;
    pLogin->szDeviceID[8] = ((*ptr++)  & 0x0f) + 0x30;

    pLogin->szDeviceID[9] = ((*ptr >> 4) & 0x0f) + 0x30;
    pLogin->szDeviceID[10] = ((*ptr++)  & 0x0f) + 0x30;

    pLogin->szDeviceID[11] = ((*ptr >> 4) & 0x0f) + 0x30;
    pLogin->szDeviceID[12] = ((*ptr++)  & 0x0f) + 0x30;

    pLogin->szDeviceID[13] = ((*ptr >> 4) & 0x0f) + 0x30;
    pLogin->szDeviceID[14] = ((*ptr++)  & 0x0f) + 0x30;

    pLogin->szDeviceID[15] = '\0';

    nRet = PT_Decode_SHORT(&(pLogin->nSeqNum), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}

int GT_EncodeLogin(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr;
    TGTLOGIN *pLogin = (TGTLOGIN *)pPackage;

    ptr = lpBuff;

    *ptr++ = pLogin->szDeviceID[0] - 0x30;
    *ptr++ = ((pLogin->szDeviceID[1] - 0x30) << 4) | ((pLogin->szDeviceID[2] - 0x30) & 0x0f);
    *ptr++ = ((pLogin->szDeviceID[3] - 0x30) << 4) | ((pLogin->szDeviceID[4] - 0x30) & 0x0f);
    *ptr++ = ((pLogin->szDeviceID[5] - 0x30) << 4) | ((pLogin->szDeviceID[6] - 0x30) & 0x0f);
    *ptr++ = ((pLogin->szDeviceID[7] - 0x30) << 4) | ((pLogin->szDeviceID[8] - 0x30) & 0x0f);
    *ptr++ = ((pLogin->szDeviceID[9] - 0x30) << 4) | ((pLogin->szDeviceID[10] - 0x30) & 0x0f);
    *ptr++ = ((pLogin->szDeviceID[11] - 0x30) << 4) |((pLogin->szDeviceID[12] - 0x30) & 0x0f);
    *ptr++ = ((pLogin->szDeviceID[13] - 0x30) << 4) |((pLogin->szDeviceID[14] - 0x30) & 0x0f);
    nBuffLen -= 8;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, pLogin->nSeqNum);
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}

int GT_DecodePosition(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr;
    unsigned int nTemp;
    char nGPSLen = 0;

    TGTPOSITON *pReq = (TGTPOSITON *)pPackage;

    ptr = lpBuff + 4;
    nBuffLen -= 4;

    pReq->nYear = (*ptr++) + 2000;
    pReq->nMonth = *ptr++;
    pReq->nDay = *ptr++;
    pReq->nHour = *ptr++;
    pReq->nMinute = *ptr++;
    pReq->nSecond = *ptr++;
    nBuffLen -= 6;

    nGPSLen = (*ptr >> 4) & 0x0F;
    pReq->nSatelliteNum = (*ptr++) & 0x0F;
    nBuffLen--;
    //lat
    nRet = PT_Decode_INT32(&nTemp, ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    pReq->dblLat = nTemp / 1800000.0;//60 * 30000

    //LONG
    nRet = PT_Decode_INT32(&nTemp, ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;
    
    pReq->dblLon = nTemp / 1800000.0;//60 * 30000

    //speed
    pReq->dblSpeed = *ptr++;
    nBuffLen--;
    //status
    pReq->bGPSDifferent = (*ptr >> 5) & 0x01; 
    pReq->bGPSPosition = (*ptr >> 4) & 0x01; 
    pReq->bWestLon = (*ptr >> 3) & 0x01; 
    pReq->bNorthLat = (*ptr >> 2) & 0x01; 

    //course
    pReq->nCourse = (*ptr & 0x03) << 8;
    ptr++;
    nBuffLen--;

    pReq->nCourse |= *ptr++;
    nBuffLen--;
    //reserved 8bytes
    ptr += 8;
    nBuffLen -= 8;

    nRet = PT_Decode_SHORT(&(pReq->nSeqNum), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}

int GT_EncodePosition(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr;
    double dblTemp;
    unsigned int nTemp;

    TGTPOSITON *pReq = (TGTPOSITON *)pPackage;
    
    ptr = lpBuff;

    *ptr++ = (pReq->nYear - 2000) & 0xFF;
    *ptr++ = pReq->nMonth;
    *ptr++ = pReq->nDay;
    *ptr++ = pReq->nHour;
    *ptr++ = pReq->nMinute;
    *ptr++ = pReq->nSecond;
    nBuffLen -= 6;
    //GPS info length is 0xC
    *ptr++ = ((0xC << 4) | (pReq->nSatelliteNum & 0xF));
    nBuffLen--;
    //lat
    dblTemp = pReq->dblLat * 1800000;//60 * 30000
    nTemp = (unsigned int)dblTemp;

    nRet = PT_Encode_INT32(ptr, nBuffLen, nTemp);
    ptr += nRet;
    nBuffLen -= nRet;

    //LONG
    dblTemp = pReq->dblLon * 1800000; //60 * 30000
    nTemp = (unsigned int)dblTemp;

    nRet = PT_Encode_INT32(ptr, nBuffLen, nTemp);
    ptr += nRet;
    nBuffLen -= nRet;

    //speed
    nTemp = (unsigned int)pReq->dblSpeed;
    *ptr++ = nTemp & 0xFF;
    nBuffLen--;
    //status
    *ptr = 0;
    if (pReq->bGPSDifferent)
    {
        *ptr |= (0x01 << 5); 
    }

    if (pReq->bGPSPosition)
    {
        *ptr |= (0x01 << 4)	;
    }

    if (pReq->bWestLon)//west long.
    {
        *ptr |= (0x01 << 3);
    }

    if (pReq->bNorthLat)// north lat.
    {
        *ptr |= (0x01 << 2)	;
    }
    //course
    nTemp = (unsigned int)pReq->nCourse;
    *ptr |= ((nTemp >> 8) & 0x03);

    ptr++;
    nBuffLen--;

    *ptr++ = (nTemp & 0xFF);
    nBuffLen--;
    //reserved 8bytes
    *((int *)ptr) = 0x0;
    ptr += 4;
    *((int *)ptr) = 0x0;
    ptr += 4;
    nBuffLen -= 8;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, pReq->nSeqNum);
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}

int GT_DecodeState(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr;
    TGTSTATEREQ *pReq = (TGTSTATEREQ *)pPackage;

    ptr = lpBuff + 4;
    nBuffLen -= 4;

    pReq->bOil = (*ptr >> 7) & 0x01;
    pReq->bGPSPos = (*ptr >> 6) & 0x01;
    pReq->nWarnFlag = (*ptr >> 3) & 0x07;
    pReq->bCharge = (*ptr >> 2) & 0x01;
    pReq->bACC = (*ptr >> 1) & 0x01;
    pReq->bProtected = (*ptr++) & 0x01;

    nBuffLen--;

    pReq->nPowerLevel = *ptr++;
    nBuffLen--;

    pReq->nGSMSignal = *ptr++;
    nBuffLen--;

    //扩展位
    *ptr++;
    *ptr++;

    nRet = PT_Decode_SHORT(&(pReq->nSeqNum), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}

int GT_EncodeState(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr;
    TGTSTATEREQ *pReq = (TGTSTATEREQ *)pPackage;

    ptr = lpBuff;

    *ptr = ((pReq->bOil & 0x01) << 7);
    *ptr |= ((pReq->bGPSPos & 0x01) << 6);
    *ptr |= ((pReq->nWarnFlag & 0x07) << 3);
    *ptr |= ((pReq->bCharge & 0x01) << 2);
    *ptr |= ((pReq->bACC & 0x01) << 1);
    *ptr++ |= (pReq->bProtected & 0x01);
    nBuffLen--;

    *ptr++ = pReq->nPowerLevel;
    nBuffLen--;

    *ptr++ = pReq->nGSMSignal;
    nBuffLen--;
//扩展位
    *ptr++ = 0;
    *ptr++ = 0;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, pReq->nSeqNum);
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}

int GT_DecodeRes(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr;
    TGTCOMMONRES *pRes = (TGTCOMMONRES *)pPackage;
    
    ptr = lpBuff + 4;
    nBuffLen -= 4;

    nRet = PT_Decode_SHORT(&(pRes->nSeqNum), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;
    
    return (ptr - lpBuff);
}

int GT_EncodeRes(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr;
    TGTCOMMONRES *pRes = (TGTCOMMONRES *)pPackage;

    ptr = lpBuff;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, pRes->nSeqNum);
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}

int GT_DecodeCMD(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr;
    TGTCMDREQ *pReq = (TGTCMDREQ *)pPackage;

    ptr = lpBuff + 4;
    nBuffLen -= 4;

    pReq->nCMDLen = (*ptr++) - 4;
    nBuffLen--;

    if (pReq->nCMDLen > sizeof(pReq->szCMD))
    {
        return -1;
    }

    nRet = PT_Decode_INT32(&(pReq->nServerFlag), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    memcpy(pReq->szCMD, ptr, pReq->nCMDLen);
    ptr += pReq->nCMDLen;
    nBuffLen -= pReq->nCMDLen;

    nRet = PT_Decode_SHORT(&pReq->nSeqNum, ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}

int GT_EncodeCMD(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr;
    TGTCMDREQ *pReq = (TGTCMDREQ *)pPackage;

    ptr = lpBuff;
    
    *ptr++ = pReq->nCMDLen + 4;
    nBuffLen--;
    
    nRet = PT_Encode_INT32(ptr, nBuffLen, pReq->nServerFlag);
    ptr += nRet;
    nBuffLen -= nRet;

    memcpy(ptr, pReq->szCMD, pReq->nCMDLen);
    ptr += pReq->nCMDLen;
    nBuffLen -= pReq->nCMDLen;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, pReq->nSeqNum);
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}

int GT_DecodePhoto(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr;
    TGTPHOTO *pReq = (TGTPHOTO *)pPackage;

    ptr = lpBuff + 4;
    nBuffLen -= 4;

    pReq->bFinished = *ptr++;
    nBuffLen -= 1;

    pReq->nDataLen = *ptr++;
    nBuffLen--;

    if (pReq->nDataLen > sizeof(pReq->data))
    {
        return -1;
    }

    memcpy(pReq->data, ptr, pReq->nDataLen);
    ptr += pReq->nDataLen;
    nBuffLen -= pReq->nDataLen;

    nRet = PT_Decode_SHORT(&pReq->nSeqNum, ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}

int GT_EncodePhoto(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr;
    TGTPHOTO *pReq = (TGTPHOTO *)pPackage;

    ptr = lpBuff;

    *ptr++ = pReq->bFinished;
    nBuffLen--;

    *ptr++ = pReq->nDataLen;
    nBuffLen--;

    memcpy(ptr, pReq->data, pReq->nDataLen);
    ptr += pReq->nDataLen;
    nBuffLen -= pReq->nDataLen;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, pReq->nSeqNum);
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}

///////////////////////////////////////
#endif//end PT_ENABLE_GT06










