/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     pt_bd2_express.c
* Abstract： BD2 express protocol define
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

#include "pt_bd2_express.h"

#ifdef  PT_ENABLE_BD2_EXPRESS
const uchar BD2_EX_Head[][4] = { "ECS","ECT","TCS","IDV","PRD","PRO" 
                                ,"RIS","RMO","SCS","TXM","WGM"};
//--------------------------------------------------------------------------
//协议栈编解码函数定义
//--------------------------------------------------------------------------
int BD2_DecodeRMO(void *pPackage, char *lpBuff, int nDataLen);
int BD2_EncodeRMO(char *lpBuff, int nBuffLen, void *pPackage);

int BD2_DecodeTXM(void *pPackage, char *lpBuff, int nDataLen);
int BD2_EncodeTXM(char *lpBuff, int nBuffLen, void *pPackage);

int BD2_DecodeWGM(void *pPackage, char *lpBuff, int nDataLen);
int BD2_EncodeWGM(char *lpBuff, int nBuffLen, void *pPackage);
//--------------------------------------------------------------------------
//协议栈接口函数定义
//--------------------------------------------------------------------------
/*
*函数名称：BD2_EX_DecodeHead
*函数说明：解析消息类型
*参数说明：buf    接收数据缓存
*          len    接收数据长度
*返回值：  -1 表示无效的消息类型，其他表示解析的类型
*/
int BD2_EX_DecodeHead(const uchar* buf, int len)
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
	        nCount = sizeof(BD2_EX_Head) / sizeof(BD2_EX_Head[0]);//计算二代协议个数
	        for(i = 0; i< nCount; i++)
	        {
		        if(strncmp(buf + 3, BD2_EX_Head[i], strlen(BD2_EX_Head[i])) == 0)	//协议头判断
		        {
			        return ( i + BD2_EX_ZERO);
		        }
	        }
        }
    }

	return -1;
}
/*
*函数名称：BD2_EX_EncodeHead
*函数说明：编码协议头
*参数说明：lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          bDevice   是否设备
*          nType     消息类型
*返回值：  -1 表示失败，其他表示编码数据长度
*/
int BD2_EX_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType)
{
    uchar *pHead = NULL;
    int nLen = 0;

    if (nBuffLen < 7)
        return -1;

    if (BD2_EX_ISTYPE(nType))
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

        pHead = (char *)&BD2_EX_Head[nType - BD2_EX_ZERO];
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
*函数名称：BD2_EX_GetEncoder
*函数说明：根据消息类型获取相应的编码函数
*参数说明：nType 消息类型
*返回值：  NULL 表示无编码函数，其他表示编码函数指针
*/
FUNC_ENCODER BD2_EX_GetEncoder(int nType)
{
    switch (nType)
    {
    case BD2_EX_ECS://设置输出原始导航信息
    case BD2_EX_ECT://原始导航信息
    case BD2_EX_TCS://接收通道强制跟踪
    case BD2_EX_IDV://干扰检测信息
    case BD2_EX_PRD://设置用户设备输出伪距观测值和载波相位
    case BD2_EX_PRO://原始伪距观测值和载波相位
    case BD2_EX_RIS://设备复位
		return NULL;
    case BD2_EX_RMO://输出激活
		return BD2_EncodeRMO;
    case BD2_EX_SCS://RDSS双通道时差数据
		return NULL;
	case BD2_EX_TXM:
		return BD2_EncodeTXM;
	case BD2_EX_WGM:
		return BD2_EncodeWGM;
    default:
        return NULL;
    }
}
/*
*函数名称：BD2_EX_GetDecoder
*函数说明：根据消息类型获取相应的解码函数
*参数说明：nType 消息类型
*返回值：  NULL 表示无解码函数，其他表示解码函数指针
*/
FUNC_DECODER BD2_EX_GetDecoder(int nType)
{
    switch (nType)
    {
    case BD2_EX_ECS://设置输出原始导航信息
    case BD2_EX_ECT://原始导航信息
    case BD2_EX_TCS://接收通道强制跟踪
    case BD2_EX_IDV://干扰检测信息
    case BD2_EX_PRD://设置用户设备输出伪距观测值和载波相位
    case BD2_EX_PRO://原始伪距观测值和载波相位
    case BD2_EX_RIS://设备复位
		return NULL;
    case BD2_EX_RMO://输出激活
		return BD2_DecodeRMO;
    case BD2_EX_SCS://RDSS双通道时差数据
		return NULL;
	case BD2_EX_TXM:
		return BD2_DecodeTXM;
	case BD2_EX_WGM:
		return BD2_DecodeWGM;
    default:
        return NULL;
    }
}
#ifdef WIN32
/*
*函数名称：BD2_EX_GetPackage
*函数说明：根据消息类型获取相应的数据结构包
*参数说明：nType 消息类型
*返回值：  NULL 表示无此类型数据结构包，其他表示数据结构包指针，结构数据包使用完后应当释放空间
*/
void* BD2_EX_GetPackage(int nType)
{

    switch (nType)
    {
    case BD2_EX_ECS://设置输出原始导航信息
    case BD2_EX_ECT://原始导航信息
    case BD2_EX_TCS://接收通道强制跟踪
    case BD2_EX_IDV://干扰检测信息
    case BD2_EX_PRD://设置用户设备输出伪距观测值和载波相位
    case BD2_EX_PRO://原始伪距观测值和载波相位
    case BD2_EX_RIS://设备复位
		return NULL;
    case BD2_EX_RMO://输出激活
    case BD2_EX_SCS://RDSS双通道时差数据
    default:
        return NULL;
    }
}
#endif //WIN32
//--------------------------------------------------------------------------
//协议栈编解码函数定义
//--------------------------------------------------------------------------

int BD2_DecodeRMO(void *pPackage, char *lpBuff, int nDataLen)
{
    TPackage data;
	TBD2RMO* pkg = (TBD2RMO*)pPackage;

    DecodePackage_Ex(&data, lpBuff, nDataLen, ",*");
    if(data.nItems != 5)//没有可解析的数据
	{
        return RESULT_ERROR;
    }    
    strcpy(pkg->szCMD, data.pItem[1]);
    pkg->nMode = atoi(data.pItem[2]);
    pkg->nFreq = atoi(data.pItem[3]);

	return RESULT_SUCCESS;
}

int BD2_EncodeRMO(char *lpBuff, int nBuffLen, void *pPackage)
{
    char *ptr = lpBuff;
	int nLen;
	TBD2RMO* pkg = (TBD2RMO*)pPackage;

	if (pkg->nMode == 4)
	{
		*ptr++ = ',';
		*ptr++ = '4';
		*ptr++ = ',';
	}
	else if (pkg->nMode == 3)
	{
		*ptr++ = ',';
		*ptr++ = '3';
		*ptr++ = ',';
		nLen = sprintf(ptr, "%d.0", pkg->nFreq);
		ptr += nLen;
	}
	else if (pkg->nMode == 1)
	{
		nLen = sprintf(ptr, "%s,1,", pkg->szCMD);
		ptr += nLen;		
	}
	else if (pkg->nMode == 2)
	{
		nLen = sprintf(ptr, "%s,2,%d.0", pkg->szCMD,pkg->nFreq);
		ptr += nLen;		
	}
	else
	{
		return RESULT_ERROR;
	}

	return ptr - lpBuff;
}

int BD2_DecodeTXM(void *pPackage, char *lpBuff, int nDataLen)
{
	TPackage data;
	TBD2TXM* pkg = (TBD2TXM*)pPackage;

    DecodePackage_Ex(&data, lpBuff, nDataLen, ",*");
    if(data.nItems != 4)//没有可解析的数据
	{
        return RESULT_ERROR;
    }   
	
	pkg->nCMD = atoi(data.pItem[1]); 
    pkg->nMode = atoi(data.pItem[2]);

	return RESULT_SUCCESS;
}

int BD2_EncodeTXM(char *lpBuff, int nBuffLen, void *pPackage)
{
    char *ptr = lpBuff;
	int nLen;
	TBD2TXM* pkg = (TBD2TXM*)pPackage;

	*ptr++ = pkg->nCMD + 0x30;
	*ptr++ = ',';
	*ptr++ = pkg->nMode + 0x30;

	return ptr - lpBuff;
}

int BD2_DecodeWGM(void *pPackage, char *lpBuff, int nDataLen)
{
	TPackage data;
	TBD2WGM* pkg = (TBD2WGM*)pPackage;

    DecodePackage_Ex(&data, lpBuff, nDataLen, ",*");
    if(data.nItems != 7)//没有可解析的数据
	{
        return RESULT_ERROR;
    }   
	
	pkg->nCMD = atoi(data.pItem[1]); 

	if (pkg->nCMD == 2)//查询
	{
		pkg->nUserID = 0;
		pkg->szAddr[0] = '\0';
		pkg->nPort = 0;
		pkg->szDeviceID[0] = '\0';
	}
	else
	{
		pkg->nUserID = atoi(data.pItem[2]);

		PT_DecodeString(pkg->szAddr, sizeof(pkg->szAddr), data.pItem[3]);

		pkg->nPort = atoi(data.pItem[4]);

		PT_DecodeString(pkg->szDeviceID, sizeof(pkg->szDeviceID), data.pItem[5]);
	}

	return RESULT_SUCCESS;
}

int BD2_EncodeWGM(char *lpBuff, int nBuffLen, void *pPackage)
{
    char *ptr = lpBuff;
	int nLen;
	TBD2WGM* pkg = (TBD2WGM*)pPackage;

	*ptr++ = pkg->nCMD + 0x30;
	*ptr++ = ',';

	if (pkg->nCMD == 2)//查询
	{
		*ptr++ = ',';
		*ptr++ = ',';
		*ptr++ = ',';
	}
	else
	{
		nLen = sprintf(ptr, "%07d,%s,%d,%s", pkg->nUserID, pkg->szAddr, pkg->nPort, pkg->szDeviceID);
		ptr += nLen;
	}


	return ptr - lpBuff;
}

#endif //end PT_ENABLE_BD2_EXPRESS define