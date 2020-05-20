/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     pt_gps.c
* Abstract： gps protocol define
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
#include "pt_gps.h"
#include "..\..\utility\utility.h"
#ifdef  PT_ENABLE_GPS

//GPS协议头定义
const uchar GPS_Head[][4] = { "GGA","GSA","GSV","RMC","VTG","GLL","ZDA"};
//--------------------------------------------------------------------------
//协议栈编解码函数定义
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
//协议栈基本函数
//--------------------------------------------------------------------------
/*
*函数名称：GPS_DecodeHead
*函数说明：解析消息类型
*参数说明：buf    接收数据缓存
*          len    接收数据长度
*返回值：  -1 表示无效的消息类型，其他表示解析的类型
*/
int GPS_DecodeHead(const uchar* buf, int len)
{
	int nCount;
	int i;

    if (len >= 6 && (buf[0] == '$' && buf[1] == 'G' && buf[2] == 'P'))
    {
	    nCount = sizeof(GPS_Head) / sizeof(GPS_Head[0]);//计算二代协议个数
	    for(i = 0; i< nCount; i++)
	    {
		    if(strncmp(buf+3, GPS_Head[i], strlen(GPS_Head[i])) == 0)	//协议头判断
		    {
			    return ( i + GPS_ZERO);
		    }
	    }
    }

	return -1;
}
/*
*函数名称：GPS_EncodeHead
*函数说明：编码协议头
*参数说明：lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          bDevice   是否设备
*          nType     消息类型
*返回值：  -1 表示失败，其他表示编码数据长度
*/
int GPS_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType)
{
    uchar *pHead = NULL;
    int nLen = 0;

    if (nBuffLen < 7)
        return -1;

    if (GPS_ISTYPE(nType))
    {
        lpBuff[0] = '$';
        lpBuff[1] = 'G';
        lpBuff[2] = 'P';
        nBuffLen -= 3;

        pHead = &GPS_Head[nType - GPS_ZERO];
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
//--------------------------------------------------------------------------
//协议栈编解码函数
//--------------------------------------------------------------------------
/*
*函数名称：GPS_GetEncoder
*函数说明：根据消息类型获取相应的编码函数
*参数说明：nType 消息类型
*返回值：  NULL 表示无编码函数，其他表示编码函数指针
*/
FUNC_ENCODER GPS_GetEncoder(int nType)
{
    switch (nType)
    {
    case GPS_GGA://全球定位数据
        return NULL;
    case GPS_GSA://卫星PRN数据
        return NULL;
    case GPS_GSV://卫星状态信息
        return NULL;
    case GPS_RMC://运输定位数据
        return NULL;
    case GPS_VTG://地面速度信息
        return NULL;
    case GPS_GLL://大地坐标数据
        return NULL;
    case GPS_ZDA://UTC时间和日期
        return NULL;
    default:
        return NULL;
    }
}
/*
*函数名称：GPS_GetDecoder
*函数说明：根据消息类型获取相应的解码函数
*参数说明：nType 消息类型
*返回值：  NULL 表示无解码函数，其他表示解码函数指针
*/
FUNC_DECODER GPS_GetDecoder(int nType)
{
    switch (nType)
    {
    case GPS_GGA://全球定位数据
        return NULL;
    case GPS_GSA://卫星PRN数据
        return NULL;
    case GPS_GSV://卫星状态信息
        return NULL;
    case GPS_RMC://运输定位数据
        return NULL;
    case GPS_VTG://地面速度信息
        return NULL;
    case GPS_GLL://大地坐标数据
        return NULL;
    case GPS_ZDA://UTC时间和日期
        return NULL;
    default:
        return NULL;
    }
}

#ifdef WIN32
/*
*函数名称：GPS_GetPackage
*函数说明：根据消息类型获取相应的数据结构包
*参数说明：nType 消息类型
*返回值：  NULL 表示无此类型数据结构包，其他表示数据结构包指针，结构数据包使用完后应当释放空间
*/
void* GPS_GetPackage(int nType)
{
    switch (nType)
    {
    case GPS_GGA://全球定位数据
    case GPS_GSA://卫星PRN数据
    case GPS_GSV://卫星状态信息
    case GPS_RMC://运输定位数据
    case GPS_VTG://地面速度信息
    case GPS_GLL://大地坐标数据
    case GPS_ZDA://UTC时间和日期
    default:
        return NULL;
    }
}
#endif //WIN32

//--------------------------------------------------------------------------
//协议栈编解码函数
//--------------------------------------------------------------------------
int GPS_GGA_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
    TGPSGGA *pGga = (TGPSGGA *)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    
    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 14)//数据长度过长
        return RESULT_VERIFY_ERROR; 

    return RESULT_SUCCESS;    
}


#endif //end PT_ENABLE_GPS define