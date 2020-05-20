/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     pt_nw_define.c
* Abstract： nw defined protocol 
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

#include "pt_nw_define.h"

#ifdef  PT_ENABLE_NW
//二代RD及诺维自定义协议头定义
const uchar NW_Head[][4]={ "KJD", "ICK", "ICS", "ICQ", "ICH", "JKD", //验卡平台
						   "LGN", "WZR", "WZI", "WZQ", "STA", "TXI", //追踪器诺维自定义
                           "FKS", "FKD", "YDM", "TXS", "HMM", "HMQ", 
                           "DXQ", "DBG", "MSS"};
//--------------------------------------------------------------------------
//协议栈编解码函数定义
//--------------------------------------------------------------------------
int NW_DecodeKjd(void *pPackage, uchar *lpBuff, int nDataLen);
int NW_EncodeKjd(uchar *lpBuff, int nDataLen, void *pPackage);

int NW_DecodeIch(void *pPackage, uchar *lpBuff, int nDataLen);

int NW_DecodeIcs(void *pPackage, uchar *lpBuff, int nDataLen);

int NW_DecodeIck(void *pPackage, uchar *lpBuff, int nDataLen);
int NW_EncodeIck(uchar *lpBuff, int nBuffLen, void *pPackage);

int NW_EncodeIcq(uchar *lpBuff, int nBuffLen, void *pPackage);

int NW_DecodeLGN(void *pPackage, uchar *lpBuff, int nBuffLen);
int NW_EncodeLGN(uchar *lpBuff, int nBuffLen, void *pPackage);

int NW_DecodeWZR(void *pPackage, uchar *lpBuff, int nBuffLen);
int NW_EncodeWZR(uchar *lpBuff, int nBuffLen, void *pPackage);

int NW_DecodeWZI(void *pPackage, uchar *lpBuff, int nBuffLen);
int NW_EncodeWZI(uchar *lpBuff, int nBuffLen, void *pPackage);

int NW_DecodeWZQ(void *pPackage, uchar *lpBuff, int nBuffLen);
int NW_EncodeWZQ(uchar *lpBuff, int nBuffLen, void *pPackage);

int NW_DecodeSTA(void *pPackage, uchar *lpBuff, int nBuffLen);
int NW_EncodeSTA(uchar *lpBuff, int nBuffLen, void *pPackage);

int NW_DecodeTXI(void *pPackage, uchar *lpBuff, int nBuffLen);
int NW_EncodeTXI(uchar *lpBuff, int nBuffLen, void *pPackage);

int NW_DecodeFKS(void *pPackage, uchar *lpBuff, int nBuffLen);
int NW_EncodeFKS(uchar *lpBuff, int nBuffLen, void *pPackage);

int NW_DecodeFKD(void *pPackage, uchar *lpBuff, int nBuffLen);
int NW_EncodeFKD(uchar *lpBuff, int nBuffLen, void *pPackage);

int NW_DecodeTXS(void *pPackage, uchar *lpBuff, int nDataLen);
int NW_EncodeTXS(uchar *lpBuff, int nBuffLen, void *pPackage);

int NW_DecodeYDM(void *pPackage, uchar *lpBuff, int nBuffLen);
int NW_EncodeYDM(uchar *lpBuff, int nBuffLen, void *pPackage);

int NW_DecodeHMM(void *pPackage, uchar *lpBuff, int nBuffLen);
int NW_EncodeHMM(uchar *lpBuff, int nBuffLen, void *pPackage);

int NW_DecodeHMQ(void *pPackage, uchar *lpBuff, int nBuffLen);
int NW_EncodeHMQ(uchar *lpBuff, int nBuffLen, void *pPackage);

int NW_DecodeDXQ(void *pPackage, uchar *lpBuff, int nBuffLen);
int NW_EncodeDXQ(uchar *lpBuff, int nBuffLen, void *pPackage);

int NW_DecodeDBG(void *pPackage, uchar *lpBuff, int nBuffLen);
int NW_EncodeDBG(uchar *lpBuff, int nBuffLen, void *pPackage);

int NW_DecodeMSS(void *pPackage, uchar *lpBuff, int nBuffLen);
int NW_EncodeMSS(uchar *lpBuff, int nBuffLen, void *pPackage);

//--------------------------------------------------------------------------

//协议栈基本函数
//--------------------------------------------------------------------------
/*
*函数名称：NW_DecodeHeadEx
*函数说明：解析消息类型
*参数说明：buf    接收数据缓存
*          len    接收数据长度
*返回值：  -1 表示无效的消息类型，其他表示解析的类型
*/
int NW_DecodeHeadEx(const uchar* buf, int len)
{
	int nCount;
	int i;
    nCount = sizeof(NW_Head) / sizeof(NW_Head[0]);//计算二代协议个数
    for(i = 0; i< nCount; i++)
    {
	    if(strncmp(buf + 3, NW_Head[i], strlen(NW_Head[i])) == 0)	//协议头判断
	    {
		    return ( i + NW_ZERO);
	    }
    }

	return -1;
}
/*
*函数名称：NW_DecodeHead
*函数说明：解析消息类型
*参数说明：buf    接收数据缓存
*          len    接收数据长度
*返回值：  -1 表示无效的消息类型，其他表示解析的类型
*/
int NW_DecodeHead(const uchar* buf, int len)
{
//检测是否二代协议头
    if (len >= 6 && (buf[0] == '$' && buf[1] == 'N' && buf[2] == 'W'))
    {
        return NW_DecodeHeadEx(buf, len);
    }

	return -1;
}
/*
*函数名称：NW_EncodeHead
*函数说明：编码协议头
*参数说明：lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          bDevice   是否设备
*          nType     消息类型
*返回值：  -1 表示失败，其他表示编码数据长度
*/
int NW_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType)
{
    uchar *pHead = NULL;
    int nLen = 0;

    if (nBuffLen < 7)
        return -1;

    if (NW_ISTYPE(nType))
    {
        lpBuff[0] = '$';
        lpBuff[1] = 'N';
        lpBuff[2] = 'W';
        nBuffLen -= 3;

        pHead = (uchar *)&NW_Head[nType - NW_ZERO];
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
*函数名称：NW_GetEncoder
*函数说明：根据消息类型获取相应的编码函数
*参数说明：nType 消息类型
*返回值：  NULL 表示无编码函数，其他表示编码函数指针
*/
FUNC_ENCODER NW_GetEncoder(int nType)
{
    switch (nType)
    {
    case NW_KJD://开机握手指令
		return NW_EncodeKjd;
    case NW_ICK://卡座状态控制
		return NW_EncodeIck;
    case NW_ICS://所有卡座的状态
    case NW_ICQ://读取卡内信息
		return NW_EncodeIcq;
    case NW_ICH://当前卡的基本信息
    case NW_JKD://接口切换
		return NULL;
	case NW_LGN://设备登录网关
		return NW_EncodeLGN;
    case NW_WZR://设备输出位置至客户端
        return NW_EncodeWZR;
    case NW_WZI://设备上报位置至平台
        return NW_EncodeWZI;
	case NW_WZQ://平台查询设备位置
		return NW_EncodeWZQ;
	case NW_STA://设备上报状态
		return NW_EncodeSTA;
	case NW_TXI://平台与设备短信通信
		return NW_EncodeTXI;
	case NW_FKS://网关发送至设备的响应	 服务器发给一体机
		return NW_EncodeFKS;
    case NW_FKD://设备发送至网关的响应	 一体机发给服务器
		return NW_EncodeFKD;
	case NW_YDM://网关控制指令 关闭\接通油电  服务器发给一体机
		return NW_EncodeYDM;
	case NW_TXS://客户端设置一体机通信参数
		return NW_EncodeTXS;
    case NW_HMM://增加、删除SOS号码
		return NW_EncodeHMM;
    case NW_HMQ://查询SOS号码
		return NW_EncodeHMQ;
	case NW_DXQ://查询设备存储短信
		return NW_EncodeDXQ;
	case NW_DBG://管理设备调试信息
		return NW_EncodeDBG;
    case NW_MSS://模式设置
		return NW_EncodeMSS;
    default:
        return NULL;
    }
}
/*
*函数名称：NW_GetDecoder
*函数说明：根据消息类型获取相应的解码函数
*参数说明：nType 消息类型
*返回值：  NULL 表示无解码函数，其他表示解码函数指针
*/
FUNC_DECODER NW_GetDecoder(int nType)
{
    switch (nType)
    {
    case NW_KJD://开机握手指令
		return NW_DecodeKjd;
	case NW_ICK://卡座状态控制
		return NW_DecodeIck;
    case NW_ICS://所有卡座的状态
		return NW_DecodeIcs;
    case NW_ICQ://读取卡内信息
    case NW_ICH://当前卡的基本信息
		return NW_DecodeIch;
    case NW_JKD://接口切换
		return NULL;
	case NW_LGN://追踪器登录网关
		return NW_DecodeLGN;
	case NW_WZR://设备输出位置至客户端
		return NW_DecodeWZR;
    case NW_WZI://设备上报位置至平台
        return NW_DecodeWZI;
	case NW_WZQ://平台查询设备位置
		return NW_DecodeWZQ;
	case NW_STA://追踪器上报状态
		return NW_DecodeSTA;
	case NW_TXI://平台与设备短信通信
		return NW_DecodeTXI;
	case NW_FKS://网关发送至设备的响应	 服务器发给一体机
		return NW_DecodeFKS;
    case NW_FKD://设备发送至网关的响应	 一体机发给服务器
		return NW_DecodeFKD;
	case NW_YDM://网关控制指令 关闭\接通油电  服务器发给一体机
		return NW_DecodeYDM;
	case NW_TXS://客户端设置一体机通信参数
		return NW_DecodeTXS;
    case NW_HMM://增加、删除SOS号码
		return NW_DecodeHMM;
    case NW_HMQ://查询SOS号码
		return NW_DecodeHMQ;
	case NW_DXQ://查询设备存储短信
		return NW_DecodeDXQ;
	case NW_DBG://管理设备调试信息
		return NW_DecodeDBG;
    case NW_MSS://模式设置
		return NW_DecodeMSS;
    default:
        return NULL;
    }
}

#ifdef WIN32
/*
*函数名称：NW_GetPackage
*函数说明：根据消息类型获取相应的数据结构包
*参数说明：nType 消息类型
*返回值：  NULL 表示无此类型数据结构包，其他表示数据结构包指针，结构数据包使用完后应当释放空间
*/
void* NW_GetPackage(int nType)
{
    switch (nType)
    {
    case NW_KJD://开机握手指令
		return malloc(sizeof(TNWKJD));
    case NW_ICK://卡座状态控制
		return malloc(sizeof(TNWICK));
    case NW_ICS://所有卡座的状态
		return malloc(sizeof(TNWICS));
	case NW_ICQ://读取卡内信息
		return malloc(sizeof(TNWICQ));
    case NW_ICH://当前卡的基本信息
		return malloc(sizeof(TNWICH));
    case NW_JKD://接口切换
		return NULL;
    default:
        return NULL;
    }
}
#endif //WIN32
//--------------------------------------------------------------------------
//协议栈编解码函数实现
//--------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////
//函数说明：开机握手指令解码函数
//参数说明：协议结构体，需要解码的buff
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int NW_DecodeKjd(void *pPackage, uchar *lpBuff, int nDataLen)
{
    TNWKJD* pKjd = (TNWKJD*)pPackage;
    sscanf(lpBuff,"%*[^,],%[^*]",pKjd->uc_Hello);
    return RESULT_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
//函数说明：开机握手指令编码函数
//参数说明：协议结构体，编码后将放入buff中
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int NW_EncodeKjd(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TNWKJD* pKjd = (TNWKJD*)pPackage;

    nRet = sprintf(lpBuff,"%s",pKjd->uc_Hello);
    return nRet;
}

//////////////////////////////////////////////////////////////////////////
//函数说明：协议解码
//参数说明：协议结构体，需要解码的buff
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int NW_DecodeIch(void *pPackage, uchar *lpBuff, int nDataLen)
{
    TNWICH* pIch = (TNWICH*)pPackage;
    memset(pIch,0,sizeof(TNWICH));
    sscanf(lpBuff,"%*[^,],%[^,d],%[^,d],%[^,d],%[^,d],%[^,d],%[^,d],%[^,d],%[^*d]",
        pIch->uc_UserID,pIch->uc_SerialNum,&pIch->uc_BroadcastNum,&pIch->uc_Eigenvalue,&pIch->f_ServiceFreq,
        &pIch->uc_CommLevel,&pIch->uc_EncryptMark,&pIch->f_UnderlingCount);
    return RESULT_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
//函数说明：协议解码
//参数说明：协议结构体，需要解码的buff
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int NW_DecodeIcs(void *pPackage, uchar *lpBuff, int nDataLen)
{
    TNWICS* pIcs = (TNWICS*)pPackage;
    memset(pIcs,0,sizeof(TNWICS));
    sscanf(lpBuff,"%*[^,],%[^,d],%[^,d],%[^,d],%[^,d],%[^,d],%[^,d],%[^,d],%[^,d],%[^,d],%[^*d]",
        &pIcs->nCardState1,&pIcs->nCardState2,&pIcs->nCardState3,&pIcs->nCardState4,&pIcs->nCardState5,
        &pIcs->nCardState6,&pIcs->nCardState7,&pIcs->nCardState8,&pIcs->nCardState9,&pIcs->nCardState10);
    return RESULT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
//函数说明：卡座状态控制协议解码
//参数说明：协议结构体，需要解码的buff
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int NW_DecodeIck(void *pPackage, uchar *lpBuff, int nDataLen)
{
    TNWICK* pIck = (TNWICK*)pPackage;
    memset(pIck,0,sizeof(TNWICK));
    sscanf(lpBuff,"%*[^,],%[^,d],%[^,d],%[^*d]",
        &pIck->uc_CommandType,&pIck->uc_SoltNum,&pIck->uc_WorkState);
    return RESULT_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
//函数说明：卡座状态控制协议编码
//参数说明：协议结构体，编码后将放入buff中
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int NW_EncodeIck(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TNWICK* pIck = (TNWICK*)pPackage;

    nRet = sprintf(lpBuff,"%d,%d,%d",pIck->uc_CommandType,pIck->uc_SoltNum,pIck->uc_WorkState);
    return nRet;
}

//////////////////////////////////////////////////////////////////////////
//函数说明：协议编码
//参数说明：协议结构体，编码后将放入buff中
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int NW_EncodeIcq(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TNWICQ* pIcq = (TNWICQ*)pPackage;

    nRet = sprintf(lpBuff,"%d,%f,%s,%s",pIcq->uc_CommandType,pIcq->f_DeviceZero,pIcq->uc_SerialNum,pIcq->uc_ManageInfo);
    return nRet;
}

///////////////////////////////
//一体机协议
///////////////////////////////
int NW_DecodeLGN(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    TPackage pkg;
    TNWLGN *pDat = (TNWLGN *)pPackage;

    PT_SplitStringEx(&pkg, lpBuff, nBuffLen, ",*");	
    if (pkg.nItems < 4)
        return RESULT_ERROR;

	pDat->nSeqNum = atoi(pkg.pItem[1]);

    PT_DecodeString(pDat->szDeviceID, sizeof(pDat->szDeviceID), pkg.pItem[2]);

    return RESULT_SUCCESS;
}

int NW_EncodeLGN(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TNWLGN *pDat = (TNWLGN *)pPackage;
    
	nRet = sprintf(lpBuff, "%d,", pDat->nSeqNum);

    pDat->szDeviceID[15] = '\0';

    memcpy(lpBuff + nRet, pDat->szDeviceID, strlen(pDat->szDeviceID));

	nRet += strlen(pDat->szDeviceID);

    return nRet;
}

int NW_DecodeWZR(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    TPackage pkg;
	uchar i = 1;
	uchar *ptr;
    TNWWZR *pDat = (TNWWZR *)pPackage;

    PT_SplitStringEx(&pkg, lpBuff, nBuffLen, ",*");	
	if (pkg.nItems < 13)
    	return RESULT_ERROR;

//ddmmyy
	ptr = pkg.pItem[i++];
	pDat->nYear = atoi(ptr + 4);

	*(ptr + 4) = '\0';
	pDat->nMonth = atoi(ptr + 2);

	*(ptr + 2) = '\0';
	pDat->nDay = atoi(ptr);
//hhmmss
	ptr = pkg.pItem[i++];
	*(ptr + 6) = '\0';
	pDat->nSecond = atoi(ptr + 4);

	*(ptr + 4) = '\0';
	pDat->nMinute = atoi(ptr + 2);

	*(ptr + 2) = '\0';
	pDat->nHour = atoi(ptr);
//'N''S'
	pDat->chLat = *pkg.pItem[i++];
	pDat->chLong = *pkg.pItem[i++];
//
	pDat->nDeviceID = atoi(pkg.pItem[i++]);
//lat
	ptr = strchr(pkg.pItem[i], '.');
	pDat->dblLat = atof(ptr - 2) / 60.0;
	
	*(ptr - 2) = '\0';
	pDat->dblLat += atoi(pkg.pItem[i++]);
//long
	ptr = strchr(pkg.pItem[i], '.');
	pDat->dblLong = atof(ptr - 2) / 60.0;
	
	*(ptr - 2) = '\0';
	pDat->dblLong += atoi(pkg.pItem[i++]);

	pDat->dblSpeed = atof(pkg.pItem[i++]);
	pDat->dblCourse = atof(pkg.pItem[i++]);
    pDat->dblHeight = atof(pkg.pItem[i++]);
    
	pDat->nFlag = atoi(pkg.pItem[i++]);

    return RESULT_SUCCESS;
}

int NW_EncodeWZR(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
	unsigned int nTemp;
    TNWWZR *pDat = (TNWWZR *)pPackage;

    
	nRet = sprintf(lpBuff,"%02d%02d%02d,%02d%02d%02d.00,",
                            pDat->nDay, pDat->nMonth, pDat->nYear % 100,
                            pDat->nHour, pDat->nMinute, pDat->nSecond);
	
	lpBuff[nRet++] = pDat->chLat;
	lpBuff[nRet++] = ',';
	lpBuff[nRet++] = pDat->chLong;
	lpBuff[nRet++] = ',';

	nRet += sprintf(lpBuff + nRet, "%d,", pDat->nDeviceID);

	nTemp = (unsigned int)pDat->dblLat;
	nRet += sprintf(lpBuff + nRet, "%d", nTemp);
	nTemp =  (pDat->dblLat - nTemp) * 600000;
	nRet += sprintf(lpBuff + nRet, "%02d.%04d,", nTemp / 10000, nTemp % 10000);

	nTemp = (unsigned int)pDat->dblLong;
	nRet += sprintf(lpBuff + nRet, "%d", nTemp);
	nTemp =  (pDat->dblLong - nTemp) * 600000;
	nRet += sprintf(lpBuff + nRet, "%02d.%04d,", nTemp / 10000, nTemp % 10000);

	nTemp = pDat->dblSpeed * 10;
	nRet += sprintf(lpBuff + nRet, "%d.%d,", nTemp / 10, nTemp % 10);

	nTemp = pDat->dblCourse * 10;
	nRet += sprintf(lpBuff + nRet, "%d.%d,", nTemp / 10, nTemp % 10);
    
    nTemp = pDat->dblHeight * 10;
	nRet += sprintf(lpBuff + nRet, "%d.%d,", nTemp / 10, nTemp % 10);

	nRet += sprintf(lpBuff + nRet, "%d", pDat->nFlag);
		
    return nRet;
}

int NW_DecodeWZI(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    TPackage pkg;
    int i = 0;
    int nSize;
    uchar *ptr;
    TNWWZI *pDat = (TNWWZI *)pPackage;

    PT_SplitStringEx(&pkg, lpBuff, nBuffLen, ",*");	
    if (pkg.nItems < 5)
        return RESULT_ERROR;

    pDat->nSeqNum = atoi(pkg.pItem[1]);
    pDat->nServerNum = atoi(pkg.pItem[2]);

    nBuffLen = pkg.pItem[4] - pkg.pItem[3] - 1;
    
    pDat->nDataLen = 0;
    ptr = pkg.pItem[3];
    if (nBuffLen > 0)
    {
        nSize = sizeof(pDat->szData);
        for (i = 0; i < nBuffLen; )
        {
            pDat->szData[pDat->nDataLen] = RD_C2A(ptr[i++]);
            pDat->szData[pDat->nDataLen] <<= 4;
            pDat->szData[pDat->nDataLen++] |= RD_C2A(ptr[i++]);

            if (pDat->nDataLen >= nSize)
            {
                break;
            }
        }
    }

    return RESULT_SUCCESS;
}

int NW_EncodeWZI(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    unsigned int nTemp;
    TNWWZI *pDat = (TNWWZI *)pPackage;


    nRet = sprintf(lpBuff,"%d,%d%,", pDat->nSeqNum, pDat->nServerNum);
    nBuffLen -= nRet;

    if (pDat->nDataLen > 0)
    {
        for (nTemp = 0; nTemp < pDat->nDataLen; nTemp++)
        {
            lpBuff[nRet++] = RD_A2C((pDat->szData[nTemp] >> 4) & 0xF);
            lpBuff[nRet++] = RD_A2C(pDat->szData[nTemp] & 0xF);
            nBuffLen -= 2;
        }
    }
    
    return nRet;
}

int NW_DecodeWZQ(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	TPackage pkg;
    TNWWZQ *pDat = (TNWWZQ *)pPackage;
   
    PT_SplitStringEx(&pkg, lpBuff, nBuffLen, ",*");	
    if (pkg.nItems < 3)
        return RESULT_ERROR;

    pDat->nServerNum = atoi(pkg.pItem[1]);

    return RESULT_SUCCESS;
}

int NW_EncodeWZQ(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet = -1;
    TNWWZQ *pDat = (TNWWZQ *)pPackage;

    nRet = sprintf(lpBuff,"%d", pDat->nServerNum);

    return nRet;
}

int NW_DecodeSTA(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    TPackage pkg;
	uchar i = 1;
    TNWSTA *pDat = (TNWSTA *)pPackage;

    PT_SplitStringEx(&pkg, lpBuff, nBuffLen, ",*");	
    if (pDat->nDataType == 0)
	{
		if (pkg.nItems < 10)
        	return RESULT_ERROR;

        pDat->nSeqNum = 0;
	}
	else if (pDat->nDataType == 1)
	{
		if (pkg.nItems < 11)
        	return RESULT_ERROR;

		pDat->nSeqNum = atoi(pkg.pItem[i++]);
	}

	pDat->nWarnFlag = atoi(pkg.pItem[i++]);
	pDat->bGPSPos = atoi(pkg.pItem[i++]);
	pDat->bOil = atoi(pkg.pItem[i++]);
	pDat->bACC = atoi(pkg.pItem[i++]);
	pDat->bCharge = atoi(pkg.pItem[i++]);
	pDat->nPowerLevel = atoi(pkg.pItem[i++]);
	pDat->nGSMSignal = atoi(pkg.pItem[i++]);
	pDat->nGPRSState = atoi(pkg.pItem[i++]);
    
    return RESULT_SUCCESS;
}

int NW_EncodeSTA(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TNWSTA *pDat = (TNWSTA *)pPackage;

	if (pDat->nDataType == 0)
	{
    	nRet = sprintf(lpBuff,"%d,%d,%d,%d,%d,%d,%d,%d",
        				pDat->nWarnFlag, pDat->bGPSPos, pDat->bOil, pDat->bACC, 
        				pDat->bCharge, pDat->nPowerLevel, pDat->nGSMSignal, pDat->nGPRSState);
	}
	else if (pDat->nDataType == 1)
	{
    	nRet = sprintf(lpBuff,"%d,%d,%d,%d,%d,%d,%d,%d,%d",
        				pDat->nSeqNum, pDat->nWarnFlag, pDat->bGPSPos, pDat->bOil, pDat->bACC, 
        				pDat->bCharge, pDat->nPowerLevel, pDat->nGSMSignal, pDat->nGPRSState); 
	}

    return nRet;
}

int NW_DecodeTXI(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nCount = 0;
    uchar *ptr = NULL;
	uchar *ptr0 = lpBuff;
    TNWTXI *pDat = (TNWTXI *)pPackage;
    while (ptr = strchr(ptr0, ','))
    {
        *ptr = '\0';
        if (nCount == 1)
        {
            pDat->nServerNum = atoi(ptr0);
        }
        else if (nCount == 2)
        {
            pDat->nHour = atoi(ptr0);
        }
        else if (nCount == 3)
        {
            pDat->nMinute = atoi(ptr0);
        }
        else if (nCount == 4)
        {
            PT_DecodeString(pDat->szUserID, sizeof(pDat->szUserID), ptr0);
			break;
        }
        ptr++;
        ptr0 = ptr;
        nCount++;
    }

    if (nCount >= 4)
    {
		pDat->nDataLen = nBuffLen - (ptr - lpBuff) - 5;
        if (pDat->nDataLen > 0)
        {
            ptr++;
            if (pDat->nDataLen > sizeof(pDat->szData) - 1)
            {
                pDat->nDataLen = sizeof(pDat->szData) - 1;
            }
            memcpy(pDat->szData, ptr, pDat->nDataLen);
            pDat->szData[pDat->nDataLen] = '\0';
        }

        return RESULT_SUCCESS;
    }
    else
    {
        pDat->nDataLen = 0;
        pDat->szData[0] = '\0';
    }

    return RESULT_ERROR;
}

int NW_EncodeTXI(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    int i = 0;
    TNWTXI *pDat = (TNWTXI *)pPackage;

    pDat->szData[sizeof(pDat->szData) - 1] = '\0';

    nRet = sprintf(lpBuff,"%d,%d,%d,%s,", pDat->nServerNum, 
        pDat->nHour, pDat->nMinute, pDat->szUserID);

    if (pDat->nDataLen > 0)
    {
        for (i = 0; i < pDat->nDataLen; i++)
        {
            if (pDat->szData[i] == ',')
            {
                lpBuff[nRet++] = 0xA3;
                lpBuff[nRet++] = 0xAC;
                nBuffLen -= 2;
            }
            else if (pDat->szData[i] == '*')
            {
                lpBuff[nRet++] = 0xA9;
                lpBuff[nRet++] = 0x7E;
                nBuffLen -= 2;
            }
            else
            {
                lpBuff[nRet++] = pDat->szData[i];
                nBuffLen--;
            }
            
            if (nBuffLen == 0)
                break;
        }
    }

    return nRet;
}

int NW_DecodeFKS(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nCount = 0;
    int nDataLen = 0;
    uchar *ptr = NULL;
	uchar *ptr0 = lpBuff;
    TNWFKS *pDat = (TNWFKS *)pPackage;
    while (ptr = strchr(ptr0, ','))
    {
        *ptr = '\0';
        if (nCount == 1)
        {
            pDat->nSeqNum = atoi(ptr0);
        }
        else if (nCount == 2)
        {
            pDat->nType = NW_DecodeHeadEx(ptr0 - 3, ptr - ptr0 - 4);
        }
        else if (nCount == 3)
        {
            pDat->nRet = atoi(ptr0);
			break;
        }
        ptr++;
        ptr0 = ptr;
        nCount++;
    }

    if (nCount >= 3)
    {
		nDataLen = nBuffLen - (ptr - lpBuff) - 5;
        if (nDataLen > 0)
        {
            ptr++;
            if (nDataLen > sizeof(pDat->szData) - 1)
            {
                nDataLen = sizeof(pDat->szData) - 1;
            }
            memcpy(pDat->szData, ptr, nDataLen);
            pDat->szData[nDataLen] = '\0';
        }

        return RESULT_SUCCESS;
    }
    else
    {
        pDat->szData[0] = '\0';
    }

    return RESULT_ERROR;
}

int NW_EncodeFKS(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    int nDataLen = 0;
    TNWFKS *pDat = (TNWFKS *)pPackage;

    pDat->szData[sizeof(pDat->szData) - 1] = '\0';
    nDataLen = strlen(pDat->szData);

    nRet = sprintf(lpBuff,"%d,%s,%d,", pDat->nSeqNum, 
        (char *)NW_Head[pDat->nType - NW_ZERO], pDat->nRet);

    if (nDataLen > 0)
    {
        memcpy(lpBuff + nRet, pDat->szData, nDataLen);
        nRet += nDataLen;
    }

    return nRet;
}

int NW_DecodeFKD(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nCount = 0;
    int nDataLen = 0;
    uchar *ptr = NULL;
	uchar *ptr0 = lpBuff;
    TNWFKD *pDat = (TNWFKD *)pPackage;
    while (ptr = strchr(ptr0, ','))
    {
		*ptr = '\0';
        if (nCount == 1)
        {
            pDat->nServerNum = atoi(ptr0);
        }
        else if (nCount == 2)
        {
            pDat->nSeqNum = atoi(ptr0);
        }
        else if (nCount == 3)
        {
            pDat->nType = NW_DecodeHeadEx(ptr0 - 3, ptr - ptr0 + 3);
        }
        else if (nCount == 4)
        {
            pDat->nRet = atoi(ptr0);
			break;
        }
        ptr++;
        ptr0 = ptr;
        nCount++;
    }

    if (nCount >= 4)
    {
		nDataLen = nBuffLen - (ptr - lpBuff) - 5;
        if (nDataLen > 0)
        {
            ptr++;
            if (nDataLen > sizeof(pDat->szData) - 1)
            {
                nDataLen = sizeof(pDat->szData) - 1;
            }
            memcpy(pDat->szData, ptr, nDataLen);
            pDat->szData[nDataLen] = '\0';
        }

        return RESULT_SUCCESS;
    }
    else
    {
        pDat->szData[0] = '\0';
    }

    return RESULT_ERROR;
}

int NW_EncodeFKD(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    int nDataLen = 0;
    TNWFKD *pDat = (TNWFKD *)pPackage;

    pDat->szData[sizeof(pDat->szData) - 1] = '\0';
    nDataLen = strlen(pDat->szData);

    nRet = sprintf(lpBuff,"%d,%d,%s,%d,", pDat->nServerNum, 
        pDat->nSeqNum, (char *)NW_Head[pDat->nType - NW_ZERO], pDat->nRet);

    if (nDataLen > 0)
    {
        memcpy(lpBuff + nRet, pDat->szData, nDataLen);
        nRet += nDataLen;
    }

    return nRet;
}

int NW_DecodeTXS(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    TPackage pkg;
    TNWTXS *pDat = (TNWTXS *)pPackage;
	unsigned char *ptr = NULL;
   
    PT_SplitStringEx(&pkg, lpBuff, nBuffLen, ",*");	
    if (pkg.nItems < 3)
        return RESULT_ERROR;

    pDat->nCMDType = atoi(pkg.pItem[1]);
	if (pDat->nCMDType != 2)
	{
		if (pkg.nItems < 6)
        	return RESULT_ERROR;
					
    	pDat->nGWUserID = atoi(pkg.pItem[2]);

		ptr = strchr(pkg.pItem[3], ':');
		if (ptr)
		{
			pDat->nGWPort = ptr - pkg.pItem[3];
			if (pDat->nGWPort > sizeof(pDat->szGWAddr) - 1)
			{
				return RESULT_ERROR;
			}
			
			memcpy(pDat->szGWAddr, pkg.pItem[3], pDat->nGWPort);
		   	pDat->szGWAddr[pDat->nGWPort] = '\0';

			pDat->nGWPort = atoi(ptr + 1);
		}
		else
		{
			pDat->szGWAddr[0] = '\0';
			pDat->nGWPort = 0;
		}

		pDat->nFreq = atoi(pkg.pItem[4]);
	}

    return RESULT_SUCCESS;
}

int NW_EncodeTXS(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TNWTXS *pDat = (TNWTXS *)pPackage;

	if (pDat->nCMDType == 2)
	{
    	nRet = sprintf(lpBuff,"%d", pDat->nCMDType);
	}
	else
	{
		nRet = sprintf(lpBuff,"%d,%d", pDat->nCMDType, pDat->nGWUserID);

		if (strlen(pDat->szGWAddr) || pDat->nGWPort)
		{	
			nRet += sprintf(lpBuff + nRet,",%s:%d,", pDat->szGWAddr, pDat->nGWPort);
		}
		else
		{
			lpBuff[nRet++] = ',';
		}

		nRet += sprintf(lpBuff + nRet,"%d", pDat->nFreq);
	}

    return nRet;
}

int NW_DecodeYDM(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	TPackage pkg;
    TNWYDM *pDat = (TNWYDM *)pPackage;
   
    PT_SplitStringEx(&pkg, lpBuff, nBuffLen, ",*");	
	if (pDat->nDataType == 0)
	{
	    if (pkg.nItems < 4)
    	    return RESULT_ERROR;

		pDat->nCMDType = atoi(pkg.pItem[1]);
		pDat->nAction = atoi(pkg.pItem[2]);
	}
	else if (pDat->nDataType == 1)
	{
	    if (pkg.nItems < 5)
    	    return RESULT_ERROR;

		pDat->nServerNum = atoi(pkg.pItem[1]);
		pDat->nCMDType = atoi(pkg.pItem[2]);
		pDat->nAction = atoi(pkg.pItem[3]);
	}
	else
	{
		return RESULT_ERROR;
	}

    return RESULT_SUCCESS;
}

int NW_EncodeYDM(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet = -1;
    TNWYDM *pDat = (TNWYDM *)pPackage;

	if (pDat->nDataType == 0)
    	nRet = sprintf(lpBuff,"%d,%d", pDat->nCMDType, pDat->nAction);
	else if (pDat->nDataType == 1)
		nRet = sprintf(lpBuff,"%d,%d,%d", pDat->nServerNum, pDat->nCMDType, pDat->nAction);

    return nRet;
}

int NW_DecodeHMM(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	TPackage pkg;
    TNWHMM *pDat = (TNWHMM *)pPackage;
   
    PT_SplitStringEx(&pkg, lpBuff, nBuffLen, ",*");	
	if (pDat->nDataType == 0)
	{
	    if (pkg.nItems < 4)
    	    return RESULT_ERROR;

		pDat->nCMDType = atoi(pkg.pItem[1]);
		strcpy(pDat->szPhoneNumber, pkg.pItem[2]);
	}
	else if (pDat->nDataType == 1)
	{
	    if (pkg.nItems < 5)
    	    return RESULT_ERROR;

		pDat->nServerNum = atoi(pkg.pItem[1]);
		pDat->nCMDType = atoi(pkg.pItem[2]);
		strcpy(pDat->szPhoneNumber, pkg.pItem[3]);
	}
	else
	{
		return RESULT_ERROR;
	}

    return RESULT_SUCCESS;
}

int NW_EncodeHMM(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet = -1;
    TNWHMM *pDat = (TNWHMM *)pPackage;

	if (pDat->nDataType == 0)
    	nRet = sprintf(lpBuff,"%d,%s", pDat->nCMDType, pDat->szPhoneNumber);
	else if (pDat->nDataType == 1)
		nRet = sprintf(lpBuff,"%d,%d,%s", pDat->nServerNum, pDat->nCMDType, pDat->szPhoneNumber);

    return nRet;
}

int NW_DecodeHMQ(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    TPackage pkg;
    int nItem = 0;
    int nLen = 0;
    TNWHMQ *pDat = (TNWHMQ *)pPackage;
    
    PT_SplitStringEx(&pkg, lpBuff, nBuffLen, ",*");	
    if (pkg.nItems < 3)
        return RESULT_ERROR;

    pDat->nDataType = 0;
    
    pDat->nCMDType = atoi(pkg.pItem[1]);
    
    if (pDat->nCMDType == 1)
    {
        pDat->nSeqNum = 0;
        pDat->nServerNum = 0;
        pDat->nCount = 0;
        pDat->PhoneList[0][0] = '\0';
        pDat->PhoneList[1][0] = '\0';
        pDat->PhoneList[2][0] = '\0';
        
        if (pDat->nDataType == 1)//GPRS
        {
            if (pkg.nItems < 4)
                return RESULT_ERROR;
            
            pDat->nCMDType = atoi(pkg.pItem[2]);
        }
    }
    else if (pDat->nCMDType == 2)
    {
        if (pDat->nDataType == 1)//GPRS
        {
            if (pkg.nItems < 8)
                return RESULT_ERROR;
            
            pDat->nSeqNum = atoi(pkg.pItem[2]);
            pDat->nServerNum = atoi(pkg.pItem[3]);
            
            nItem = 4;
        }
        else if (pDat->nDataType == 0)//
        {
            if (pkg.nItems < 6)
                return RESULT_ERROR;
            
            pDat->nSeqNum = 0;
            pDat->nServerNum = 0;
            
            nItem = 2;
        }
        
        pDat->nCount = 0;
        for (; nItem < pkg.nItems - 1; nItem++)
        {
            nLen = strlen(pkg.pItem[nItem]);
            if (nLen > 0)
            {
                if (nLen  > MAX_NW_PHONE_LEN - 1)
                {
                    nLen = MAX_NW_PHONE_LEN - 1;
                }

                memcpy(pDat->PhoneList[nItem - 3], pkg.pItem[nItem], nLen);
                pDat->PhoneList[nItem - 3][nLen] = '\0';
                pDat->nCount++;
                if (pDat->nCount >= MAX_NW_PHONE_NUM)
                {
                    break;
                }
            }
        }
    }
    else
    {
        return RESULT_ERROR;
    }
    
    return RESULT_SUCCESS;
}

int NW_EncodeHMQ(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    char i = 0;
    uchar *ptr = lpBuff;
    TNWHMQ *pDat = (TNWHMQ *)pPackage;
    int nCount = pDat->nCount;
    
    pDat->nDataType = 0;
    
    nRet = sprintf(ptr, "%d", pDat->nCMDType);
    ptr += nRet;
    
    if (pDat->nCMDType == 1) //查询
    {
        if (pDat->nDataType == 1)
        {
            nRet = sprintf(ptr, ",%d", pDat->nServerNum);
            ptr += nRet;
        }
    }
    else if (pDat->nCMDType == 2)
    {
        if (pDat->nDataType == 1)
        {
            nRet = sprintf(ptr, ",%d,%d", pDat->nSeqNum, pDat->nServerNum);
            ptr += nRet;
        }
        
        for (i = 0; i < MAX_NW_PHONE_NUM; i++)
        {
            if ( i >= nCount)
            {
                *ptr++ = ',';
            }
            else
            {
                *ptr++ = ',';
                pDat->PhoneList[i][MAX_NW_PHONE_LEN - 1] = '\0';
                nRet = strlen(pDat->PhoneList[i]);
                strcpy(ptr, pDat->PhoneList[i]);

                ptr += nRet;
            }
        }
    }
   
    return (ptr - lpBuff);
}

int NW_DecodeDXQ(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	TPackage pkg;
    TNWDXQ *pDat = (TNWDXQ *)pPackage;
   
    PT_SplitStringEx(&pkg, lpBuff, nBuffLen, ",*");	
    if (pkg.nItems < 3)
        return RESULT_ERROR;

    pDat->nCount = atoi(pkg.pItem[1]);

    return RESULT_SUCCESS;
}

int NW_EncodeDXQ(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet = -1;
    TNWDXQ *pDat = (TNWDXQ *)pPackage;

    nRet = sprintf(lpBuff,"%d", pDat->nCount);

    return nRet;
}

int NW_DecodeDBG(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	TPackage pkg;
    TNWDBG *pDat = (TNWDBG *)pPackage;
   
    PT_SplitStringEx(&pkg, lpBuff, nBuffLen, ",*");	
    if (pkg.nItems < 3)
        return RESULT_ERROR;

    pDat->nType = atoi(pkg.pItem[1]);

    return RESULT_SUCCESS;
}

int NW_EncodeDBG(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet = -1;
    TNWDBG *pDat = (TNWDBG *)pPackage;

    nRet = sprintf(lpBuff,"%d", pDat->nType);

    return nRet;
}

int NW_DecodeMSS(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	TPackage pkg;
    TNWMSS *pDat = (TNWMSS *)pPackage;
   
    PT_SplitStringEx(&pkg, lpBuff, nBuffLen, ",*");	
    if (pkg.nItems < 3)
        return RESULT_ERROR;

    pDat->nCMDType = atoi(pkg.pItem[1]);
    
    if (pDat->nCMDType != 2 && pkg.nItems == 4)
        pDat->nMode = atoi(pkg.pItem[2]);

    return RESULT_SUCCESS;
}

int NW_EncodeMSS(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet = -1;
    TNWMSS *pDat = (TNWMSS *)pPackage;

    nRet = sprintf(lpBuff,"%d,%d", pDat->nCMDType, pDat->nMode);

    return nRet;
}

///////////////////////////////////////
#endif//end PT_ENABLE_NW

