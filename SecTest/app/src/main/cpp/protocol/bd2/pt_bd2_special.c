//////////////////////////////////////////////////////////////////////////
//公    司：陕西诺维信息技术有限公司
//作	者：马永根
//创建日期：2012-2-10
//修改日期：2012-2-10
//文件说明：北斗二代RD协议标准C封装定义文件
//修改说明：创建协议结构，实现基本协议处理函数
//版本说明：V0.1
//
//////////////////////////////////////////////////////////////////////////

#include "pt_bd2_special.h"

#ifdef  PT_ENABLE_BD2_SPECIAL

//二代RD及诺维自定义协议头定义
const uchar BD2_SP_Head[][4] = {{0x55,0xAA,0x01},"XSD","XST"};
//--------------------------------------------------------------------------
//协议栈编解码函数定义
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
//协议栈基本函数
//--------------------------------------------------------------------------
/*
*函数名称：BD2_SP_DecodeHead
*函数说明：解析消息类型
*参数说明：buf    接收数据缓存
*          len    接收数据长度
*返回值：  -1 表示无效的消息类型，其他表示解析的类型
*/
int BD2_SP_DecodeHead(const uchar* buf, int len)
{
	int nCount;
	int i;

    if (len >= 6 && buf[0] == '$')
    {
        if (   (buf[1] == 'B' && buf[2] == 'D') 
            || (buf[1] == 'C' && buf[2] == 'C')
            || (buf[1] == 'G' && buf[2] == 'P')
            || (buf[1] == 'G' && buf[2] == 'N')
            || (buf[1] == 'G' && buf[2] == 'L')
            || (buf[1] == 'G' && buf[2] == 'A'))
        {
	        nCount = sizeof(BD2_SP_Head) / sizeof(BD2_SP_Head[0]);//计算二代协议个数
	        for(i = 0; i< nCount; i++)
	        {
		        if(strncmp(buf + 3, BD2_SP_Head[i], strlen(BD2_SP_Head[i])) == 0)	//协议头判断
		        {
			        return ( i + BD2_SP_ZERO);
		        }
	        }
        }
    }
    else if (len >= 3 && strncmp(buf, BD2_SP_Head[BD2_SP_XNF - BD2_SP_ZERO], 3) == 0)//用户设备接收惯导辅助信息格式
    {
        return BD2_SP_XNF;
    }

	return -1;
}
/*
*函数名称：BD2_SP_EncodeHead
*函数说明：编码协议头
*参数说明：lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          bDevice   是否设备
*          nType     消息类型
*返回值：  -1 表示失败，其他表示编码数据长度
*/
int BD2_SP_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType)
{
    uchar *pHead = NULL;
    int nLen = 0;

    if (nBuffLen < 7)
        return -1;

    if (nType == BD2_SP_XNF)//用户设备接收惯导辅助信息格式
    {
        pHead = (char *)&BD2_SP_Head[nType - BD2_SP_ZERO];
        nLen = strlen(pHead);
        if (nBuffLen >= nLen)
        {
            strcpy(lpBuff, pHead);
            return nLen;
        }
    }
    else if (BD2_SP_ISTYPE(nType))
    {
        lpBuff[0] = '$';
        if (bDevice)
        {
            lpBuff[1] = 'B';
            lpBuff[2] = 'D';
        }
        else
        {
            lpBuff[1] = 'C';
            lpBuff[2] = 'C';
        }
        nBuffLen -= 3;

        pHead = (char *)&BD2_SP_Head[nType - BD2_SP_ZERO];
        nLen = strlen(pHead);
        nLen++;

        if (nBuffLen >= nLen)
        {
            strcpy(lpBuff + 3, pHead);

            nLen += 3;
            lpBuff[nLen - 1] = ',';

            return nLen;
        }
    }

    return -1;
}
/*
*函数名称：BD2_SP_GetEncoder
*函数说明：根据消息类型获取相应的编码函数
*参数说明：nType 消息类型
*返回值：  NULL 表示无编码函数，其他表示编码函数指针
*/
FUNC_ENCODER BD2_SP_GetEncoder(int nType)
{
    switch (nType)
    {
    case BD2_SP_XNF://用户设备接收惯导辅助信息格式
    case BD2_SP_XSD://输出下属用户定位信息 
    case BD2_SP_XST://输出下属用户通讯信息
    default:
        return NULL;
    }
}
/*
*函数名称：BD2_SP_GetDecoder
*函数说明：根据消息类型获取相应的解码函数
*参数说明：nType 消息类型
*返回值：  NULL 表示无解码函数，其他表示解码函数指针
*/
FUNC_DECODER BD2_SP_GetDecoder(int nType)
{
    switch (nType)
    {
    case BD2_SP_XNF://用户设备接收惯导辅助信息格式
    case BD2_SP_XSD://输出下属用户定位信息 
    case BD2_SP_XST://输出下属用户通讯信息
    default:
        return NULL;
    }
}
/*
*函数名称：BD2_SP_RecvData
*函数说明：解码接收的数据至协议数据结构
*参数说明：pParam   栈
*          data     接收的数据
*返回值：  -1 表示没有收到整帧数据，其他表示所接收帧数据的消息类型
*/
int  BD2_SP_RecvData(TStackParam *pParam, uchar data)
{
    if (pParam->nRecvLen == 8)
    {
        if (pParam->nMsgType == BD2_SP_XNF)
        {
            pParam->nFrameLen = pParam->RecvBuff[3] + 4 + 1;
        }
        else
        {
            PT_Decode_SHORT(&(pParam->nFrameLen), &(pParam->RecvBuff[6]), 2);
        }
        
        if (pParam->nFrameLen <= 8 || pParam->nFrameLen > pParam->nRecvBuffLen)//帧长无效,结束本帧接收
        {
            return RESULT_ERROR;
        }
    }
    else if (pParam->nFrameLen > 8)
    {
        if (pParam->nRecvLen == pParam->nFrameLen)//本帧接收完成
        {
            pParam->nCRCValue ^= PT_MakeCRC(pParam->RecvBuff, 8);

            if ((pParam->nCRCValue & 0xFF) == data)//校验正确
            {
                pParam->bRecvFrame = 1;
                return pParam->nMsgType;
            }
            else
            {
                return RESULT_ERROR;
            }
        }
        else 
        {
            pParam->nCRCValue ^= data;
        }
    }

    return RESULT_SUCCESS;
}
/*
*函数名称：BD2_SP_EncodePackage
*函数说明：编码数据
*参数说明：lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          pMsgData  协议数据结构指针
*          bDevice   是否设备栈，设备栈二代协议头编码"BD"用1表示，PC栈编码"CC"用0表示
*返回值：  -1 表示失败，其他表示数据长度
*/
int BD2_SP_EncodePackage(uchar *lpBuff, int nBuffLen, 
                       void *pPackage, int nMsgType, char bDevice)
{
    int nRet = RESULT_ERROR;
    int nHeadLen = 0;
    FUNC_ENCODER fnEncoder = NULL;
    uchar *ptr = lpBuff;

    fnEncoder = BD2_SP_GetEncoder(nMsgType);
    if (fnEncoder == NULL)
        return RESULT_ERROR;

//encode head
    nRet = BD2_SP_EncodeHead(ptr, nBuffLen, bDevice, nMsgType);
    if ( nRet == RESULT_ERROR)
    {
        return nRet;
    }
    else
    {
        nHeadLen = nRet;
        ptr += nRet;
        nBuffLen -= nRet;
    }

    if (nMsgType == BD2_SP_XSD || nMsgType == BD2_SP_XST)
    {//二代RD特殊协议长度占用两字节
        ptr += 2; 
        nBuffLen -= 2;
    }
    else if (nMsgType == BD2_SP_XNF)
    {//用户设备接收惯导辅助信息格式 帧长占用1字节
        ptr++; 
        nBuffLen--;
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
 //encode length and CRC
    if (nBuffLen < 1)
    {
        return RESULT_ERROR;
    }
    //length
    if (nMsgType == BD2_SP_XNF)
    {
        lpBuff[nHeadLen] =  ptr - lpBuff  - 4;//长度不包括帧头及校验位
    }
    else
    {
        PT_Encode_SHORT(&lpBuff[nHeadLen], 2, (unsigned short)(ptr - lpBuff + 1));
     }
     //CRC
    *ptr = PT_MakeCRC(lpBuff, ptr - lpBuff);//校验包含帧头'$'
    ptr++;
        
    return (ptr - lpBuff);
}
/*
*函数名称：BD2_SP_DecodePackage
*函数说明：解码接收整帧数据至协议数据结构
*参数说明：pPackage  数据结构指针
*          lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          nMsgType  协议类型
*返回值：  0 表示解码成功，其他表示失败
*/
int BD2_SP_DecodePackage(void *pPackage, int nMsgType, uchar *lpBuff, int nBuffLen)
{
    char nCRC;
    unsigned short nFrameLen = 0;

    FUNC_DECODER fnDecoder = NULL;

    fnDecoder = BD2_SP_GetDecoder(nMsgType);
    if (fnDecoder == NULL)
    {
        return RESULT_ERROR;
    }

    if (nBuffLen > 3 && nMsgType == BD2_SP_XNF)
    {
        nFrameLen = lpBuff[3] + 4 + 1;
    }
    else if (nBuffLen > 7 && (nMsgType == BD2_SP_XSD || nMsgType == BD2_SP_XST))
    {
        PT_Decode_SHORT(&nFrameLen, &lpBuff[6], 2);
    }

    if (nFrameLen > 7 && nFrameLen <= nBuffLen)
    {
        nCRC = PT_MakeCRC(lpBuff, nFrameLen - 1);
        if (nCRC != lpBuff[nFrameLen - 1])//校验不正确
        {
            return RESULT_ERROR;
        }
    }
    else
    {
        return RESULT_ERROR;
    }
   
    return fnDecoder(pPackage, lpBuff, nFrameLen);
}

#ifdef WIN32
/*
*函数名称：BD2_SP_GetPackage
*函数说明：根据消息类型获取相应的数据结构包
*参数说明：nType 消息类型
*返回值：  NULL 表示无此类型数据结构包，其他表示数据结构包指针，结构数据包使用完后应当释放空间
*/
void* BD2_SP_GetPackage(int nType)
{
    switch (nType)
    {
    case BD2_SP_XNF://用户设备接收惯导辅助信息格式
    case BD2_SP_XSD://输出下属用户定位信息 
    case BD2_SP_XST://输出下属用户通讯信息
    default:
        return NULL;
    }
}
#endif //WIN32
//--------------------------------------------------------------------------
//协议栈编解码函数
//--------------------------------------------------------------------------





#endif //end PT_ENABLE_BD2_SPECIAL define