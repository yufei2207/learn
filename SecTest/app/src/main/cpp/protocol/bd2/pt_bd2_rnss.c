/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     pt_bd2_rnss.c
* Abstract： BD2 RNSS protocol define 
* 
* Version： 1.0
* Author：  yfwang
* Date：    2012/03/19
*
* history   :
*	     when            who    	          what:
*      -------------   --------------    -------------------
*      Mar 19, 2012     yfwang                create
*
*/

#include "pt_bd2_rnss.h"
#include "../../utility/utility.h"
#ifdef  PT_ENABLE_BD2_RN

//二代RN协议头定义
const uchar BD2_RN_Head[][4] = { "AAM","ALM","APL","BSD","BSQ","BSX" 
								,"COM","DHV","GBS","GGA","GLL","GLS"
								,"GSA","GST","GSV","ICM","IHI","IHO"
								,"LPM","MSS","PMU","RMC","TXT","VTG"
                                ,"ZBS","ZDA","ZTI"};
//--------------------------------------------------------------------------
//协议栈编解码函数定义
//--------------------------------------------------------------------------
int RN_COM_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_COM_Encode(char *lpBuff, int nBuffLen, void *pPackage);

int RN_DHV_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_DHV_Encode(char *lpBuff, int nBuffLen, void *pPackage);

int RN_GBS_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_GBS_Encode(char *lpBuff, int nBuffLen, void *pPackage);

int RN_GGA_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_GGA_Encode(char *lpBuff, int nBuffLen, void *pPackage);

int RN_GLL_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_GLL_Encode(char *lpBuff, int nBuffLen, void *pPackage);

int RN_GLS_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_GLS_Encode(char *lpBuff, int nBuffLen, void *pPackage);

int RN_GSA_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_GSA_Encode(char *lpBuff, int nBuffLen, void *pPackage);

int RN_GST_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_GST_Encode(char *lpBuff, int nBuffLen, void *pPackage);

int RN_GSV_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_GSV_Encode(char *lpBuff, int nBuffLen, void *pPackage);

int RN_ICM_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_ICM_Encode(char *lpBuff, int nBuffLen, void *pPackage);

int RN_IHI_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_IHI_Encode(char *lpBuff, int nBuffLen, void *pPackage);

int RN_IHO_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_IHO_Encode(char *lpBuff, int nBuffLen, void *pPackage);

int RN_LPM_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_LPM_Encode(char *lpBuff, int nBuffLen, void *pPackage);

int RN_MSS_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_MSS_Encode(char *lpBuff, int nBuffLen, void *pPackage);

int RN_PMU_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_PMU_Encode(char *lpBuff, int nBuffLen, void *pPackage);

int RN_RMC_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_RMC_Encode(char *lpBuff, int nBuffLen, void *pPackage);

int RN_TXT_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_TXT_Encode(char *lpBuff, int nBuffLen, void *pPackage);

int RN_ZBS_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_ZBS_Encode(char *lpBuff, int nBuffLen, void *pPackage);

int RN_ZDA_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_ZDA_Encode(char *lpBuff, int nBuffLen, void *pPackage);

int RN_ZTI_Decode(void *pPackage, char *lpBuff, int nDataLen);
int RN_ZTI_Encode(char *lpBuff, int nBuffLen, void *pPackage);

//--------------------------------------------------------------------------
//协议栈基本函数
//--------------------------------------------------------------------------
/*
*函数名称：BD2_RN_DecodeHead
*函数说明：解析消息类型
*参数说明：buf    接收数据缓存
*          len    接收数据长度
*返回值：  -1 表示无效的消息类型，其他表示解析的类型
*/
int BD2_RN_DecodeHead(const uchar* buf, int len)
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
	        nCount = sizeof(BD2_RN_Head) / sizeof(BD2_RN_Head[0]);//计算二代协议个数
	        for(i = 0; i< nCount; i++)
	        {
		        if(strncmp(buf + 3, BD2_RN_Head[i], strlen(BD2_RN_Head[i])) == 0)	//协议头判断
		        {
			        return ( i + BD2_RN_ZERO);
		        }
	        }
        }
    }

	return -1;
}
/*
*函数名称：BD2_RN_EncodeHead
*函数说明：编码协议头
*参数说明：lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          bDevice   是否设备
*          nType     消息类型
*返回值：  -1 表示失败，其他表示编码数据长度
*/
int BD2_RN_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType)
{
    uchar *pHead = NULL;
    int nLen = 0;

    if (nBuffLen < 7)
        return -1;

    if (BD2_RN_ISTYPE(nType))
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

        pHead = (uchar *)&BD2_RN_Head[nType - BD2_RN_ZERO];
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
*函数名称：BD2_RN_GetEncoder
*函数说明：根据消息类型获取相应的编码函数
*参数说明：nType 消息类型
*返回值：  NULL 表示无编码函数，其他表示编码函数指针
*/
FUNC_ENCODER BD2_RN_GetEncoder(int nType)
{
    switch (nType)
    {
    case BD2_RN_AAM://航路点到达报警
    case BD2_RN_ALM://卫星历书数据
    case BD2_RN_APL://完好性保护门限
    case BD2_RN_BSD://描述标识点
    case BD2_RN_BSQ://描述标识区
    case BD2_RN_BSX://描述标识线
    case BD2_RN_COM://设置用户设备串口
        return RN_COM_Encode;
    case BD2_RN_DHV://速度类导航信息
        return RN_DHV_Encode;
    case BD2_RN_GBS://描述GNSS卫星故障检测
        return RN_GBS_Encode;
    case BD2_RN_GGA://描述定位数据
        return RN_GGA_Encode;
    case BD2_RN_GLL://大地坐标定位信息
        return RN_GLL_Encode;
    case BD2_RN_GLS://设置用户设备位置等初始化信息
        return RN_GLS_Encode;
    case BD2_RN_GSA://设置用户设备工作模式等
        return RN_GSA_Encode;
    case BD2_RN_GST://描述伪距误差统计数据
        return RN_GST_Encode;
    case BD2_RN_GSV://输出卫星信息
        return RN_GSV_Encode;
    case BD2_RN_ICM://输出用户设备中加解密模块密钥有效期信息
        return RN_ICM_Encode;
    case BD2_RN_IHI://设置用户设备输出速度加速度等信息
        return RN_IHI_Encode;
    case BD2_RN_IHO://输出导航辅助信息
        return RN_IHO_Encode;
    case BD2_RN_LPM://设置设备工作在省电模式
        return RN_LPM_Encode;
    case BD2_RN_MSS://设置设备当前定位方式
        return RN_MSS_Encode;
    case BD2_RN_PMU://输出PRM的时效参数信息
        return RN_PMU_Encode;
    case BD2_RN_RMC://输出最简导航传输数据
        return RN_RMC_Encode;
    case BD2_RN_TXT://用于传输短文本
        return RN_TXT_Encode;
    case BD2_RN_VTG://描述航迹向和地速
    case BD2_RN_ZBS://设置坐标系参数
        return RN_ZBS_Encode;
    case BD2_RN_ZDA://描述UTC时间、日期和本地时区
        return RN_ZDA_Encode;
    case BD2_RN_ZTI://输出设备当前工作状态信息
        return RN_ZTI_Encode;
    default:
        return NULL;
    }
}
/*
*函数名称：BD2_RN_GetDecoder
*函数说明：根据消息类型获取相应的解码函数
*参数说明：nType 消息类型
*返回值：  NULL 表示无解码函数，其他表示解码函数指针
*/
FUNC_DECODER BD2_RN_GetDecoder(int nType)
{
    switch (nType)
    {
    case BD2_RN_AAM://航路点到达报警
    case BD2_RN_ALM://卫星历书数据
    case BD2_RN_APL://完好性保护门限
    case BD2_RN_BSD://描述标识点
    case BD2_RN_BSQ://描述标识区
    case BD2_RN_BSX://描述标识线
    case BD2_RN_COM://设置用户设备串口
        return RN_COM_Decode;
    case BD2_RN_DHV://速度类导航信息
        return RN_DHV_Decode;
    case BD2_RN_GBS://描述GNSS卫星故障检测
        return RN_GBS_Decode;
    case BD2_RN_GGA://描述定位数据
        return RN_GGA_Decode;
    case BD2_RN_GLL://大地坐标定位信息
        return RN_GLL_Decode;
    case BD2_RN_GLS://设置用户设备位置等初始化信息
        return RN_GLS_Decode;
    case BD2_RN_GSA://设置用户设备工作模式等
        return RN_GSA_Decode;
    case BD2_RN_GST://描述伪距误差统计数据
        return RN_GST_Decode;
    case BD2_RN_GSV://输出卫星信息
        return RN_GSV_Decode;
    case BD2_RN_ICM://输出用户设备中加解密模块密钥有效期信息
        return RN_ICM_Decode;
    case BD2_RN_IHI://设置用户设备输出速度加速度等信息
        return RN_IHI_Decode;
    case BD2_RN_IHO://输出导航辅助信息
        return RN_IHO_Decode;
    case BD2_RN_LPM://设置设备工作在省电模式
        return RN_LPM_Decode;
    case BD2_RN_MSS://设置设备当前定位方式
        return RN_MSS_Decode;
    case BD2_RN_PMU://输出PRM的时效参数信息
        return RN_PMU_Decode;
    case BD2_RN_RMC://输出最简导航传输数据
        return RN_RMC_Decode;
    case BD2_RN_TXT://用于传输短文本
        return RN_TXT_Decode;
    case BD2_RN_VTG://描述航迹向和地速
    case BD2_RN_ZBS://设置坐标系参数
        return RN_ZBS_Decode;
    case BD2_RN_ZDA://描述UTC时间、日期和本地时区
        return RN_ZDA_Decode;
    case BD2_RN_ZTI://输出设备当前工作状态信息
        return RN_ZTI_Decode;
    default:
        return NULL;
    }
}

#ifdef WIN32
/*
*函数名称：BD2_RN_GetPackage
*函数说明：根据消息类型获取相应的数据结构包
*参数说明：nType 消息类型
*返回值：  NULL 表示无此类型数据结构包，其他表示数据结构包指针，结构数据包使用完后应当释放空间
*/
void* BD2_RN_GetPackage(int nType)
{
    switch (nType)
    {
    case BD2_RN_AAM://航路点到达报警
    case BD2_RN_ALM://卫星历书数据
    case BD2_RN_APL://完好性保护门限
    case BD2_RN_BSD://描述标识点
    case BD2_RN_BSQ://描述标识区
    case BD2_RN_BSX://描述标识线
    case BD2_RN_COM://设置用户设备串口
    case BD2_RN_DHV://速度类导航信息
    case BD2_RN_GBS://描述GNSS卫星故障检测
    case BD2_RN_GGA://描述定位数据
    case BD2_RN_GLL://大地坐标定位信息
    case BD2_RN_GLS://设置用户设备位置等初始化信息
    case BD2_RN_GSA://设置用户设备工作模式等
    case BD2_RN_GST://描述伪距误差统计数据
    case BD2_RN_GSV://输出卫星信息
    case BD2_RN_ICM://输出用户设备中加解密模块密钥有效期信息
    case BD2_RN_IHI://设置用户设备输出速度加速度等信息
    case BD2_RN_IHO://输出导航辅助信息
    case BD2_RN_LPM://设置设备工作在省电模式
    case BD2_RN_MSS://设置设备当前定位方式
    case BD2_RN_PMU://输出PRM的时效参数信息
    case BD2_RN_RMC://输出最简导航传输数据
    case BD2_RN_TXT://用于传输短文本
    case BD2_RN_VTG://描述航迹向和地速
    case BD2_RN_ZBS://设置坐标系参数
    case BD2_RN_ZDA://描述UTC时间、日期和本地时区
    case BD2_RN_ZTI://输出设备当前工作状态信息
    default:
        return NULL;
    }
}

#endif //WIN32
//--------------------------------------------------------------------------
//协议栈编解码函数
//--------------------------------------------------------------------------

int RN_COM_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
    TRNCOM *pCom = (TRNCOM *)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    int i = 0;
    
    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 6)//数据长度过长
        return RESULT_VERIFY_ERROR; 
        
   pdt = pkg.pItem[1];
   pCom->nBaudRate = atoi(pdt);
   pdt = pkg.pItem[2];
   pCom->nDataBits = atoi(pdt);
   pdt = pkg.pItem[3];
   pCom->nStopBits = atoi(pdt);
   pdt = pkg.pItem[4];
   pCom->nParity = atoi(pdt);

    return RESULT_SUCCESS;    
}

int RN_COM_Encode(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TRNCOM* pCom = (TRNCOM *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    
    len = sprintf(ptr,"%d,%d,%d,%d",pCom->nBaudRate,pCom->nDataBits
                                   ,pCom->nStopBits,pCom->nParity);
    ptr += len;
    nBuffLen -= len;
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);    
}

int RN_DHV_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
    TRNDHV *pDhv = (TRNDHV *)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    int i = 0;
    
    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 13)//数据长度过长
        return RESULT_VERIFY_ERROR; 
        
   pdt = pkg.pItem[1];

    return RESULT_SUCCESS;        
}
int RN_DHV_Encode(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TRNDHV *pDhv = (TRNDHV *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);     
}
int RN_GBS_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
     TRNGBS *pGbs = (TRNGBS *)pPackage;
     TPackage pkg;
     char *ptr = lpBuff;
     char *pdt = NULL;
     int i = 0;
     
     DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
     if(pkg.nItems < 2)//没有可解析的数据
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 10)//数据长度过长
         return RESULT_VERIFY_ERROR; 
         
    pdt = pkg.pItem[1];
    
     return RESULT_SUCCESS;        
    
}
int RN_GBS_Encode(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TRNGBS *pGbs = (TRNGBS *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);     
}

int RN_GGA_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
     TRNGGA *pGga = (TRNGGA *)pPackage;
     TPackage pkg;
     char *ptr = lpBuff;
     char *pdt = NULL;
     int i = 0;
     
     DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
     if(pkg.nItems < 2)//没有可解析的数据
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 17)//数据长度过长
         return RESULT_VERIFY_ERROR; 
         
    pdt = pkg.pItem[1];
    
     return RESULT_SUCCESS;    
}
int RN_GGA_Encode(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TRNGGA *pGga = (TRNGGA *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);         
}

int RN_GLL_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
     TRNGLL *pGll = (TRNGLL *)pPackage;
     TPackage pkg;
     char *ptr = lpBuff;
     char *pdt = NULL;
     int i = 0;
     
     DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
     if(pkg.nItems < 2)//没有可解析的数据
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 9)//数据长度过长
         return RESULT_VERIFY_ERROR; 
         
    pdt = pkg.pItem[1];
    
     return RESULT_SUCCESS;     
}
int RN_GLL_Encode(char *lpBuff, int nBuffLen, void *pPackage)

{
    int nRet = -1;
    TRNGLL *pGll = (TRNGLL *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);            
}

int RN_GLS_Decode(void *pPackage, char *lpBuff, int nDataLen)
{   
     TRNGLS *pGls = (TRNGLS *)pPackage;
     TPackage pkg;
     char *ptr = lpBuff;
     char *pdt = NULL;
     int i = 0;
     
     DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
     if(pkg.nItems < 2)//没有可解析的数据
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 13)//数据长度过长
         return RESULT_VERIFY_ERROR; 
         
    pdt = pkg.pItem[1];
    
     return RESULT_SUCCESS;         
}
int RN_GLS_Encode(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TRNGLS *pGls = (TRNGLS *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);                
}
int RN_GSA_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
     TRNGSA *pGsa = (TRNGSA *)pPackage;
     TPackage pkg;
     char *ptr = lpBuff;
     char *pdt = NULL;
     int i = 0;
     
     DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
     if(pkg.nItems < 2)//没有可解析的数据
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 20)//数据长度过长
         return RESULT_VERIFY_ERROR; 
         
    pdt = pkg.pItem[1];
    
     return RESULT_SUCCESS;     
}
int RN_GSA_Encode(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TRNGSA *pGsa = (TRNGSA *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);
}

int RN_GST_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
    return 1;
}
int RN_GST_Encode(char *lpBuff, int nBuffLen, void *pPackage)
{   
	return 1;
}

int RN_GSV_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
     TRNGSV *pGsv = (TRNGSV *)pPackage;
     TPackage pkg;
     char *ptr = lpBuff;
     char *pdt = NULL;
     int i = 0;
     
     DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
     if(pkg.nItems < 2)//没有可解析的数据
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 21)//数据长度过长
         return RESULT_VERIFY_ERROR; 
         
    pdt = pkg.pItem[1];
    
     return RESULT_SUCCESS;     
    
}

int RN_GSV_Encode(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TRNGSV *pGsv = (TRNGSV *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);    
}

int RN_ICM_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
     TRNICM *pIcm = (TRNICM *)pPackage;
     TPackage pkg;
     char *ptr = lpBuff;
     char *pdt = NULL;
     int i = 0;
     
     DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
     if(pkg.nItems < 2)//没有可解析的数据
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 4)//数据长度过长
         return RESULT_VERIFY_ERROR; 
         
    pdt = pkg.pItem[1];
    
     return RESULT_SUCCESS;     
    
}

int RN_ICM_Encode(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TRNICM *pIcm = (TRNICM *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);        
}

int RN_IHI_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
     TRNIHI *pIhi = (TRNIHI *)pPackage;
     TPackage pkg;
     char *ptr = lpBuff;
     char *pdt = NULL;
     int i = 0;
     
     DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
     if(pkg.nItems < 2)//没有可解析的数据
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 9)//数据长度过长
         return RESULT_VERIFY_ERROR; 
         
    pdt = pkg.pItem[1];
    
     return RESULT_SUCCESS;     
    
}
int RN_IHI_Encode(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TRNIHI *pIhi = (TRNIHI *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);            
}

int RN_IHO_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
     TRNIHO *pIho = (TRNIHO *)pPackage;
     TPackage pkg;
     char *ptr = lpBuff;
     char *pdt = NULL;
     int i = 0;
     
     DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
     if(pkg.nItems < 2)//没有可解析的数据
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 16)//数据长度过长
         return RESULT_VERIFY_ERROR; 
         
    pdt = pkg.pItem[1];
    
     return RESULT_SUCCESS;    
}
int RN_IHO_Encode(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TRNIHO *pIho = (TRNIHO *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);    
}

int RN_LPM_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
     TRNLPM *pLpm = (TRNLPM *)pPackage;
     TPackage pkg;
     char *ptr = lpBuff;
     char *pdt = NULL;
     int i = 0;
     
     DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
     if(pkg.nItems < 2)//没有可解析的数据
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 3)//数据长度过长
         return RESULT_VERIFY_ERROR; 
         
    pdt = pkg.pItem[1];
    
     return RESULT_SUCCESS;     
}
int RN_LPM_Encode(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TRNLPM *pLpm = (TRNLPM *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);    
}

int RN_MSS_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
     TRNMSS *pMss = (TRNMSS *)pPackage;
     TPackage pkg;
     char *ptr = lpBuff;
     char *pdt = NULL;
     int i = 0;
     
     DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
     if(pkg.nItems < 2)//没有可解析的数据
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 10)//数据长度过长
         return RESULT_VERIFY_ERROR; 
         
    pdt = pkg.pItem[1];
    
     return RESULT_SUCCESS;    
}

int RN_MSS_Encode(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TRNMSS *pMss = (TRNMSS *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);
}

int RN_PMU_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
     TRNPMU *pMss = (TRNPMU *)pPackage;
     TPackage pkg;
     char *ptr = lpBuff;
     char *pdt = NULL;
     int i = 0;
     
     DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
     if(pkg.nItems < 2)//没有可解析的数据
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 4)//数据长度过长
         return RESULT_VERIFY_ERROR; 
         
    pdt = pkg.pItem[1];
    
     return RESULT_SUCCESS;     
}

int RN_PMU_Encode(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TRNPMU *pPmu = (TRNPMU *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);    
}

int RN_RMC_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
     TRNRMC *pRmc = (TRNRMC *)pPackage;
     TPackage pkg;
     char *ptr = lpBuff;
     char *pdt = NULL;
     int i = 0;
     
     DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
     if(pkg.nItems < 2)//没有可解析的数据
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 14)//数据长度过长
         return RESULT_VERIFY_ERROR; 
         
    pdt = pkg.pItem[1];
    
     return RESULT_SUCCESS;    
}

int RN_RMC_Encode(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TRNRMC *pPmu = (TRNRMC *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);     
}

int RN_TXT_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
     TRNTXT *pTxt = (TRNTXT *)pPackage;
     TPackage pkg;
     char *ptr = lpBuff;
     char *pdt = NULL;
     int i = 0;
     
     DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
     if(pkg.nItems < 2)//没有可解析的数据
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 6)//数据长度过长
         return RESULT_VERIFY_ERROR; 
         
    pdt = pkg.pItem[1];
    
     return RESULT_SUCCESS;    
}

int RN_TXT_Encode(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TRNTXT *pTxt = (TRNTXT *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);         
}

int RN_ZBS_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
     TRNZBS *pZbs = (TRNZBS *)pPackage;
     TPackage pkg;
     char *ptr = lpBuff;
     char *pdt = NULL;
     int i = 0;
     
     DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
     if(pkg.nItems < 2)//没有可解析的数据
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 8)//数据长度过长
         return RESULT_VERIFY_ERROR; 
         
    pdt = pkg.pItem[1];
    
     return RESULT_SUCCESS;    
}
int RN_ZBS_Encode(char *lpBuff, int nBuffLen, void *pPackage)
{
    
    int nRet = -1;
    TRNZBS *pZbs = (TRNZBS *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);         
}
int RN_ZDA_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
     TRNZDA *pZda = (TRNZDA *)pPackage;
     TPackage pkg;
     char *ptr = lpBuff;
     char *pdt = NULL;
     int i = 0;
     
     DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
     if(pkg.nItems < 2)//没有可解析的数据
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 13)//数据长度过长
         return RESULT_VERIFY_ERROR; 
         
    pdt = pkg.pItem[1];
    
     return RESULT_SUCCESS;    

}

int RN_ZDA_Encode(char *lpBuff, int nBuffLen, void *pPackage)
{    
    int nRet = -1;
    TRNZDA *pZda = (TRNZDA *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);  

}

int RN_ZTI_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
     TRNZTI *pZti = (TRNZTI *)pPackage;
     TPackage pkg;
     char *ptr = lpBuff;
     char *pdt = NULL;
     int i = 0;
     
     DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
     if(pkg.nItems < 2)//没有可解析的数据
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 10)//数据长度过长
         return RESULT_VERIFY_ERROR; 
         
    pdt = pkg.pItem[1];
    
     return RESULT_SUCCESS;    
}

int RN_ZTI_Encode(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TRNZTI *pZti = (TRNZTI *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff); 
}

#endif //end PT_ENABLE_BD2_RN define
