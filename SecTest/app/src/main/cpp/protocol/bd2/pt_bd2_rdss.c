/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     pt_bd2_rdss.c
* Abstract： BD2 RDSS protocol define 
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

#include "pt_bd2_rdss.h"
//#include "..\..\utility\utility.h"
#ifdef  PT_ENABLE_BD2_RD

//二代RDSS
const uchar BD2_RD_Head[][4] = { "BSI","BSS","CXA","DSA","DWA","DWR" 
								,"FKI","GXM","ICA","ICI","ICZ","JMS"
								,"KLS","KLT","LZM","HZR","TXA","TXR"
								,"WAA","WBA","ZHS"};
//--------------------------------------------------------------------------
//协议栈编解码函数定义
//--------------------------------------------------------------------------
int RD_DecodeBsi(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeBsi(char *lpBuff, int nBuffLen, void* pPackage);

int RD_DecodeBss(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeBss(char *lpBuff, int nBuffLen, void* pPackage);


int RD_DecodeCxa(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeCxa(char *lpBuff, int nBuffLen, void* pPackage);

int RD_DecodeDwa(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeDwa(char *lpBuff, int nBuffLen, void* pPackage);

int RD_DecodeDwr(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeDwr(char *lpBuff, int nBuffLen, void* pPackage);

int RD_DecodeDsa(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeDsa(char *lpBuff, int nBuffLen, void *pPackage);

//int RD_DecodeDwa(void *pPackage, char *lpBuff, int nDataLen);
//int RD_EncodeDwa(char *lpBuff, int nBuffLen, void *pPackage);

int RD_DecodeFki(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeFki(char *lpBuff, int nBuffLen, void *pPackage);

int RD_DecodeGxm(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeGxm(char *lpBuff, int nBuffLen, void *pPackage);

int RD_DecodeIca(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeIca(char *lpBuff, int nBuffLen, void *pPackage);

int RD_DecodeHzr(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeHzr(char *lpBuff, int nBuffLen, void *pPackage);

int RD_DecodeIci(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeIci(char *lpBuff, int nBuffLen, void *pPackage);

//int RD_DecodeIca(void *pPackage, char *lpBuff, int nDataLen);
//int RD_EncodeIca(char *lpBuff, int nBuffLen, void *pPackage);

int RD_DecodeIcz(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeIcz(char *lpBuff, int nBuffLen, void *pPackage);

int RD_DecodeJms(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeJms(char *lpBuff, int nBuffLen, void *pPackage);

int RD_DecodeKls(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeKls(char *lpBuff, int nBuffLen, void *pPackage);

int RD_DecodeKlt(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeKlt(char *lpBuff, int nBuffLen, void *pPackage);

int RD_DecodeLzm(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeLzm(char *lpBuff, int nBuffLen, void *pPackage);

int RD_DecodeTxa(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeTxa(char *lpBuff, int nBuffLen, void *pPackage);

int RD_DecodeTxr(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeTxr(char *lpBuff, int nBuffLen, void *pPackage);

int RD_DecodeWaa(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeWaa(char *lpBuff, int nBuffLen, void *pPackage);

int RD_DecodeWba(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeWba(char *lpBuff, int nBuffLen, void *pPackage);

int RD_DecodeZhs(void *pPackage, char *lpBuff, int nDataLen);
int RD_EncodeZhs(char *lpBuff, int nBuffLen, void *pPackage);
//--------------------------------------------------------------------------
//协议栈基本函数
//--------------------------------------------------------------------------
/*
*函数名称：BD2_RD_DecodeHead
*函数说明：解析消息类型
*参数说明：buf    接收数据缓存
*          len    接收数据长度
*返回值：  -1 表示无效的消息类型，其他表示解析的类型
*/
int BD2_RD_DecodeHead(const uchar* buf, int len)
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
	        nCount = sizeof(BD2_RD_Head) / sizeof(BD2_RD_Head[0]);//计算二代协议个数
	        for(i = 0; i< nCount; i++)
	        {
		        if(strncmp(buf+3, BD2_RD_Head[i], strlen(BD2_RD_Head[i])) == 0)	//协议头判断
		        {
			        return ( i + BD2_RD_ZERO);
		        }
	        }
        }
    }

	return -1;
}
/*
*函数名称：BD2_RD_EncodeHead
*函数说明：编码协议头
*参数说明：lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          bDevice   是否设备
*          nType     消息类型
*返回值：  -1 表示失败，其他表示编码数据长度
*/
int BD2_RD_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType)
{
    uchar *pHead = NULL;
    int nLen = 0;

    if (nBuffLen < 7)
        return -1;

    if (BD2_RD_ISTYPE(nType))
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

        pHead = (uchar *)&BD2_RD_Head[nType - BD2_RD_ZERO];
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
*函数名称：BD2_RD_GetEncoder
*函数说明：根据消息类型获取相应的编码函数
*参数说明：nType 消息类型
*返回值：  NULL 表示无编码函数，其他表示编码函数指针
*/
FUNC_ENCODER BD2_RD_GetEncoder(int nType)
{
    switch (nType)
    {
    case BD2_RD_BSI:
        return RD_EncodeBsi;
    case BD2_RD_BSS://设置响应波束和时差波束
        return RD_EncodeBss;														
    case BD2_RD_CXA://查询本机通信信息
        return RD_EncodeCxa;														
    case BD2_RD_DSA://发送定时申请
        return RD_EncodeDsa;														
    case BD2_RD_DWA://发送定位申请
        return RD_EncodeDwa;
    case BD2_RD_DWR://定位信息
        return RD_EncodeDwr;
    case BD2_RD_FKI://用户设备反馈信息
        return RD_EncodeFki;
    case BD2_RD_GXM://设置用户设备管理信息
        return RD_EncodeGxm;														
    case BD2_RD_ICA://检测本用户设备加解密模块信息
        return RD_EncodeIca;
    case BD2_RD_ICI://用户设备本机加解密模块信息
        return RD_EncodeIci;
    case BD2_RD_ICZ://指挥机下属用户信息
        return RD_EncodeIcz;
    case BD2_RD_JMS://设置用户设备实现无线电静默
        return RD_EncodeJms;														
    case BD2_RD_KLS://发送口令识别指令
        return RD_EncodeKls;														
    case BD2_RD_KLT://普通型用户设备响应指挥型用户设备口令识别指令的信息
        return RD_EncodeKlt;														
    case BD2_RD_LZM://外设向用户设备设置零值
        return RD_EncodeLzm;
    case BD2_RD_HZR://回执信息
        return RD_EncodeHzr;
    case BD2_RD_TXA://用于设置用户设备发送通信申请
        return RD_EncodeTxa;
    case BD2_RD_TXR://通讯信息
        return RD_EncodeTxr;
    case BD2_RD_WAA://用户设置用户设备发送位置报告1申请
        return RD_EncodeWaa;
    case BD2_RD_WBA://用户设置用户设备发送位置报告2申请
        return RD_EncodeWba;
    case BD2_RD_ZHS://外设向用户设备发送自毁指令
        return RD_EncodeZhs;
    default:
        return NULL;
    }
}
/*
*函数名称：BD2_RD_GetDecoder
*函数说明：根据消息类型获取相应的解码函数
*参数说明：nType 消息类型
*返回值：  NULL 表示无解码函数，其他表示解码函数指针
*/
FUNC_DECODER BD2_RD_GetDecoder(int nType)
{
    switch (nType)
    {
    case BD2_RD_BSI:
        return RD_DecodeBsi;
    case BD2_RD_BSS://设置响应波束和时差波束
        return RD_DecodeBss;														
    case BD2_RD_CXA://查询本机通信信息
        return RD_DecodeCxa;														
    case BD2_RD_DSA://发送定时申请
        return NULL;														
    case BD2_RD_DWA://发送定位申请
        return RD_DecodeDwa;
    case BD2_RD_DWR://定位信息
        return RD_DecodeDwr;
    case BD2_RD_FKI://用户设备反馈信息
        return RD_DecodeFki;
    case BD2_RD_GXM://设置用户设备管理信息
        return RD_DecodeGxm;														
    case BD2_RD_ICA://检测本用户设备加解密模块信息
        return RD_DecodeIca;
    case BD2_RD_ICI://用户设备本机加解密模块信息
         return RD_DecodeIci;
    case BD2_RD_ICZ://指挥机下属用户信息
        return RD_DecodeIcz;
    case BD2_RD_JMS://设置用户设备实现无线电静默
        return RD_DecodeJms;														
    case BD2_RD_KLS://发送口令识别指令
        return RD_DecodeKls;														
    case BD2_RD_KLT://普通型用户设备响应指挥型用户设备口令识别指令的信息
        return RD_DecodeKlt;														
    case BD2_RD_LZM://外设向用户设备设置零值
        return RD_DecodeLzm;
    case BD2_RD_HZR://回执信息
        return RD_DecodeHzr;
    case BD2_RD_TXA://用于设置用户设备发送通信申请
        return RD_DecodeTxa;
    case BD2_RD_TXR://通讯信息
        return RD_DecodeTxr;
    case BD2_RD_WAA://用户设置用户设备发送位置报告1申请
        return RD_DecodeWaa;
    case BD2_RD_WBA://用户设置用户设备发送位置报告2申请
        return RD_DecodeWba;
    case BD2_RD_ZHS://外设向用户设备发送自毁指令
        return RD_DecodeZhs;
    default:
        return NULL;
    }
}

#ifdef WIN32
/*
*函数名称：PT_GetPackage
*函数说明：根据消息类型获取相应的数据结构包
*参数说明：nType 消息类型
*返回值：  NULL 表示无此类型数据结构包，其他表示数据结构包指针，结构数据包使用完后应当释放空间
*/
void* BD2_RD_GetPackage(int nType)
{
    switch (nType)
    {
    case BD2_RD_BSI://接收波束状态信息
        return malloc(sizeof(TRDBSI));
    case BD2_RD_BSS://设置响应波束和时差波束
        return malloc(sizeof(TRDBSS));														
    case BD2_RD_CXA://查询本机通信信息
        return malloc(sizeof(TRDCXA));													
    case BD2_RD_DSA://发送定时申请
        return malloc(sizeof(TRDDSA));													
    case BD2_RD_DWA://发送定位申请
        return malloc(sizeof(TRDDWA));
    case BD2_RD_DWR://定位信息
        return malloc(sizeof(TRDDWR));
    case BD2_RD_FKI://用户设备反馈信息
        return malloc(sizeof(TRDFKI));
    case BD2_RD_GXM://设置用户设备管理信息
        return malloc(sizeof(TRDGXM));														
    case BD2_RD_ICA://检测本用户设备加解密模块信息
        return malloc(sizeof(TRDICA));
    case BD2_RD_ICI://用户设备本机加解密模块信息
         return malloc(sizeof(TRDICI));
    case BD2_RD_ICZ://指挥机下属用户信息
        return malloc(sizeof(TRDICZ));
    case BD2_RD_JMS://设置用户设备实现无线电静默
        return malloc(sizeof(TRDJMS));														
    case BD2_RD_KLS://发送口令识别指令
        return malloc(sizeof(TRDKLS));													
    case BD2_RD_KLT://普通型用户设备响应指挥型用户设备口令识别指令的信息
        return malloc(sizeof(TRDKLT));													
    case BD2_RD_LZM://外设向用户设备设置零值
        return malloc(sizeof(TRDLZM));
    case BD2_RD_HZR://回执信息
        return malloc(sizeof(TRDHZR));
    case BD2_RD_TXA://用于设置用户设备发送通信申请
        return malloc(sizeof(TRDTXA));
    case BD2_RD_TXR://通讯信息
        return malloc(sizeof(TRDTXR));
    case BD2_RD_WAA://用户设置用户设备发送位置报告1申请
        return malloc(sizeof(TRDWAA));
    case BD2_RD_WBA://用户设置用户设备发送位置报告2申请
        return malloc(sizeof(TRDWBA));
    case BD2_RD_ZHS://外设向用户设备发送自毁指令
        return malloc(sizeof(TRDZHS));
    default:
        return NULL;
    }
}
#endif //WIN32
//--------------------------------------------------------------------------
//协议栈编解码函数
//--------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////
//函数说明：协议解码
//参数说明：协议结构体，需要解码的buff
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int RD_DecodeBsi(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDBSI *pBsi = (TRDBSI *)pPackage;
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
   pBsi->nInboudBeam = atoi(pdt);//响应波束
   
   pdt = pkg.pItem[2];
   pBsi->nTmDifBeam = atoi(pdt);//时差波束
   
   for(i = 0; i<10; i++)
   {
        pdt = pkg.pItem[3+i];
        pBsi->nPower[i] = atoi(pdt);
   }
    return RESULT_SUCCESS;
}
int RD_EncodeBsi(char *lpBuff, int nBuffLen, void* pPackage)
{
    int nRet = -1;
    TRDBSI* pBsi = (TRDBSI*)pPackage;
    char *ptr = lpBuff;
    int len = 0,i = 0;
    
    if (nBuffLen < 2)
        return -1;
		    
//    len = sprintf(ptr,"%02d,%02d",pBsi->nInboudBeam,pBsi->nTmDifBeam);
//    ptr += len;
//    nBuffLen -= len;
	*ptr++ = pBsi->nInboudBeam + 0x30;
	*ptr++ = ',';
	*ptr++ = pBsi->nTmDifBeam + 0x30;

    for(i = 0; i< 10; i++)
    {
		*ptr++ = ',';
        *ptr++ = pBsi->nPower[i] + 0x30;
		nBuffLen -= 2;
    }
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);
}

int RD_DecodeBss(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDBSS *pBss = (TRDBSS *)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    
    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 4)//数据长度过长
        return RESULT_VERIFY_ERROR; 

    pdt = pkg.pItem[1];
    pBss->nInboudBeam = atoi(pdt);
    pdt = pkg.pItem[2];
    pBss->nTmDifBeam= atoi(pdt);

    return RESULT_SUCCESS;
}
int RD_EncodeBss(char *lpBuff, int nBuffLen, void* pPackage)
{
    int nRet = -1;
    TRDBSS* pBss = (TRDBSS*)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    if (nBuffLen < 2)
        return -1;    
    len = sprintf(ptr,"%d,%d",pBss->nInboudBeam,pBss->nTmDifBeam);
    ptr += len;
    nBuffLen -= len;


    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);
}
int RD_DecodeCxa(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDCXA *pCxa = (TRDCXA *)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    
    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 5)//数据长度过长
        return RESULT_VERIFY_ERROR;   
        
    pdt = pkg.pItem[1];
    pCxa->nQueryType = atoi(pdt);
    
    pdt = pkg.pItem[2];
    pCxa->nQueryMode= atoi(pdt);  

    pdt = pkg.pItem[3]; 
    if((*pdt == '\0')||(*pdt == ' '))
    {
        pCxa->nUserID = 0;    
    }
    else
    {
        pCxa->nUserID = atoi(pdt);
    }
    return RESULT_SUCCESS;
    
}
int RD_EncodeCxa(char *lpBuff, int nBuffLen, void* pPackage)
{
    int nRet = -1;
    TRDCXA *pCxa = (TRDCXA *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    if (nBuffLen < 2)
        return -1;    
    len = sprintf(ptr,"%d,%d,",pCxa->nQueryType,pCxa->nQueryMode);
    ptr += len;
    nBuffLen -= len;

    if((pCxa->nQueryType == 1)&&(pCxa->nQueryMode == 1))//按照最新电文查询
    {
        
    }
    else
    {
        len = sprintf(ptr,"%07d",pCxa->nUserID);
        ptr += len;
        nBuffLen -= len;

    }
    if(nBuffLen < 0)
    {
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);
}

int RD_DecodeDsa(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDDSA *pDsa = (TRDDSA *)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    
    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 10)//数据长度过长
        return RESULT_VERIFY_ERROR;   

    pdt = pkg.pItem[1]; 
    pDsa->nUserID = atoi(pdt);//用户地址

    pdt = pkg.pItem[2]; 
    pDsa->nTimingMode = atoi(pdt);//定时方式

    pdt = pkg.pItem[3]; 
    pDsa->cLocaFlag = *pdt;//有无位置信息

    if(pDsa->cLocaFlag == 'A')//有位置信息
    {
        pdt = pkg.pItem[4];
        pDsa->db_Lat = atof(pdt);
        pdt = pkg.pItem[5];
        pDsa->db_Lon= atof(pdt);
    }
    else
    {
        pDsa->db_Lat = 0.0;
        pDsa->db_Lon = 0.0;        
    }
    pdt = pkg.pItem[6]; 
    if((*pdt == 0)||(*pdt == ' '))// 未输入频度
    {
        pDsa->f_Freq = 0.0;
    }
    else
    {
        pDsa->f_Freq = atof(pdt);
    }
    pdt = pkg.pItem[7];//单向零值
    if(pDsa->nTimingMode == 1)
    {
        pDsa->f_SingleZero = atof(pdt); //单向零值
    }
    else
    {
        pDsa->f_SingleZero = 0.0;
    }
    pdt = pkg.pItem[8];//附加零值
    if((*pdt == 0)||(*pdt == ' '))// 没有输入附加零值
    {
        pDsa->f_AddonZero = 0.0;
    }
    else
    {
        pDsa->f_SingleZero = atof(pdt); 
    }
    return RESULT_SUCCESS;
    
}
//////////////////////////////////////////////////////////////////////////
//函数说明：协议编码
//参数说明：协议结构体，编码后将放入buff中
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int  RD_EncodeDsa(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1,len = 0;
    TRDDSA* pDsa = (TRDDSA*)pPackage;
    char *ptr = lpBuff;
    unsigned int t1 = 0,t2 = 0;    
    
    if(nBuffLen < 7)
        return RESULT_ERROR;
    len = sprintf(ptr,"%7d",pDsa->nUserID);//用户ID
    nBuffLen -= len;
    ptr += len;
    *ptr++ = ',';
    nBuffLen--;
    len = sprintf(ptr,"%01d",pDsa->nTimingMode);//定时方式
	ptr++;
    *ptr++ = ',';
	nBuffLen -= 2;
	*ptr++ = pDsa->cLocaFlag;//有无位置信息
    *ptr++ = ',';
    nBuffLen -= 2;
	if(pDsa->cLocaFlag == 'A')
	{         
        //------纬度
        len = ftoa(pDsa->db_Lat,ptr,2, -1);
        if(nBuffLen < len)
        {
            return RESULT_ERROR;
        }
        nBuffLen -= len;
        *ptr++ = ',';
        nBuffLen--;
        //经度
        len = ftoa(pDsa->db_Lon, ptr, 2, -1);
        if(nBuffLen < len)
        {
            return RESULT_ERROR;
        }        
        *ptr++ = ',';
        nBuffLen--;
    }
    else
    {
        *ptr++ = ',';
        *ptr++ = ',';
    }        
    len = ftoa(pDsa->f_Freq, ptr, 1, -1);//频度
    if(nBuffLen < len)
    {
        return RESULT_ERROR;
    }   
    nBuffLen -= len;
    *ptr++ = ',';

    nBuffLen--;
    if(pDsa->nTimingMode == 2)
    {
        *ptr++ = '0';
        *ptr++ = '.';
        *ptr++ = '0';
    }
    else
    {
        len = ftoa(pDsa->f_SingleZero, ptr, 1, -1);//单向零值
        if(nBuffLen < len)
        {
            return RESULT_ERROR;
        }   
        nBuffLen -= len;
    }
    *ptr++ = ',';
    nBuffLen--;  
    len = ftoa(pDsa->f_AddonZero, ptr, 1, -1);//附加零值
    if(nBuffLen < len)
    {
        return RESULT_ERROR;
    }   
    nBuffLen -= len;
    
    return (ptr - lpBuff);
}

//////////////////////////////////////////////////////////////////////////
//函数说明：协议解码
//参数说明：协议结构体，需要解码的buff
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int RD_DecodeDwa(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDDWA *pDwa = (TRDDWA *)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    
    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 11)//数据长度过长
        return RESULT_VERIFY_ERROR;   

     pdt = pkg.pItem[1];
     pDwa->nUserID = atoi(pdt);
     pdt = pkg.pItem[2];
     pDwa->cUrgencyPos = *pdt;
     pdt = pkg.pItem[3];
     pDwa->uc_MHMode = atoi(pdt);
     pdt = pkg.pItem[4];
     pDwa->cLocaType = *pdt;

     if(pDwa->uc_MHMode == 0)//有高程
     {
        pdt = pkg.pItem[5];
        pDwa->db_HeightData = atof(pdt); //高程数据
        pDwa->db_AntHigh = 0.0;//天线高
        pDwa->db_AirPressure = 0.0;//气压
        pDwa->db_Temperature = 0.0;//温度
     }
     else if(pDwa->uc_MHMode == 1)//无高程
     {
        pDwa->db_HeightData = 0.0;//高程数据
        pdt = pkg.pItem[6];
        pDwa->db_AntHigh = atof(pdt);//天线高
        pDwa->db_AirPressure = 0.0;//气压
        pDwa->db_Temperature = 0.0;//温度
     }
     else if(pDwa->uc_MHMode == 2)//测高1
     {
        pDwa->db_HeightData = 0.0;//高程数据
        pdt = pkg.pItem[6];
        pDwa->db_AntHigh = atof(pdt);//天线高
        pdt = pkg.pItem[7];
        pDwa->db_AirPressure = atof(pdt);//气压
        pdt = pkg.pItem[8];
        pDwa->db_Temperature = atof(pdt);//温度       
     }
     else if(pDwa->uc_MHMode == 3)//测高2
     {
        pdt = pkg.pItem[5];
        pDwa->db_HeightData = atof(pdt);//高程数据
        pdt = pkg.pItem[6];
        pDwa->db_AntHigh = atof(pdt);//天线高
        pdt = pkg.pItem[7];
        pDwa->db_AirPressure = atof(pdt);//气压
        pdt = pkg.pItem[8];
        pDwa->db_Temperature = atof(pdt);//温度          
     }
     pdt = pkg.pItem[9];
     if((*pdt == 0)||(*pdt == ' '))
     {
        pDwa->db_Freq = 0.0;
     }
     else
     {
        pDwa->db_Freq = atof(pdt);//频度
     }
     return RESULT_SUCCESS;
     
}
int RD_EncodeDwa(char *lpBuff, int nBuffLen, void* pPackage)
{
    int nRet = -1;
    TRDDWA *pDwa = (TRDDWA *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    if(nBuffLen < 7)
        return RESULT_ERROR;
    //用户地址,紧急定位标志,测高方式,高程指示
    len = sprintf(ptr,"%07d,%c,%01d,%c,",pDwa->nUserID,pDwa->cUrgencyPos
                                        ,pDwa->uc_MHMode,pDwa->cLocaType);
    ptr += len;
    nBuffLen -= len;
    len = ftoa(pDwa->db_HeightData,ptr,1,-1);//高程数据
    ptr += len;
    nBuffLen -= len;
    *ptr++ = ',';
    nBuffLen--;
    
    len = ftoa(pDwa->db_AntHigh,ptr,1,-1);//天线数据
    ptr += len;
    nBuffLen -= len;
    *ptr++ = ',';
    nBuffLen--;

    len = ftoa(pDwa->db_AntHigh,ptr,1,-1);//气压数据
    ptr += len;
    nBuffLen -= len;
    *ptr++ = ',';
    nBuffLen--;

    len = ftoa(pDwa->db_Temperature,ptr,1,-1);//温度数据
    ptr += len;
    nBuffLen -= len;
    *ptr++ = ',';
    nBuffLen--;

    len = ftoa(pDwa->db_Freq,ptr,1,-1);//申请频度
    ptr += len;
    nBuffLen -= len;

    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);
}

/////////////////////////////////////////////////////////////////////////
//函数说明：协议解码
//参数说明：协议结构体，需要解码的buff
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int RD_DecodeDwr(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDDWR *pDwr = (TRDDWR *)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    float f_Time = 0,f_second = 0;
	double dbtemp = 0;
    int nTime  = 0;
    
    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 17)//数据长度过长
        return RESULT_VERIFY_ERROR; 
    

    pdt = pkg.pItem[1];
    pDwr->nPosType= atoi(pdt);//定位类型
    
    pdt = pkg.pItem[2];
    pDwr->nUserID = atoi(pdt); //用户ID

    pdt = pkg.pItem[3];
    pDwr->f_Second = atof(pdt+4);//秒
    *(pdt+4) = '\0';
    pDwr->nMinute = atoi(pdt+2);//分
    *(pdt+2) = '\0';
    pDwr->nHour = atoi(pdt);//时
    
    pdt = pkg.pItem[4];
    dbtemp = atof(pdt);//纬度
	pDwr->db_Lat = (int)(dbtemp / 100);
	dbtemp = dbtemp - pDwr->db_Lat*100;
	pDwr->db_Lat += (dbtemp/60.0);
	
    pdt = pkg.pItem[5];
    pDwr->cLatDir = *pdt;//纬度方向
	
    pdt = pkg.pItem[6];
    dbtemp = atof(pdt);//经度
	pDwr->db_Lon = (int)(dbtemp / 100);
	dbtemp = dbtemp - pDwr->db_Lon*100;
	pDwr->db_Lon += (dbtemp/60.0);
	
    pdt = pkg.pItem[7];
    pDwr->cLonDir = *pdt;//精度方向
    
    pdt = pkg.pItem[8];
    pDwr->db_Height = atof(pdt);//高程数据
    pdt = pkg.pItem[9];
    pDwr->cHunit= *pdt;    //M

    pdt = pkg.pItem[10];
    pDwr->f_Kesi = atof(pdt);//高程异常
    pdt = pkg.pItem[11];
    pDwr->cKesiUnit = *pdt;    //M

    pdt = pkg.pItem[12];
    pDwr->nPrecision = atoi(pdt);  //精度指示

    pdt = pkg.pItem[13];
    pDwr->cUrgencyPos = *pdt;//紧急定位

    pdt = pkg.pItem[14];
    pDwr->cMutilpleValue = *pdt; //多值解
    
    pdt = pkg.pItem[15];
    pDwr->cLocaType = *pdt;   //高程指示 
		
		return RESULT_SUCCESS;
}

int RD_EncodeDwr(char *lpBuff, int nBuffLen, void* pPackage)
{
    int nRet = -1;
    TRDDWR *pDwr = (TRDDWR *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    //定位数据类型，用户ID,时,分
    len = sprintf(ptr,"%01d,%07d,%02d%02d",pDwr->nPosType,pDwr->nUserID
                                           ,pDwr->nHour,pDwr->nMinute);
    nBuffLen -= len;
    ptr += len;

    len = ftoa(pDwr->f_Second,ptr,2,2);//秒 ss.ss
    nBuffLen -= len;
    ptr += len;
    *ptr++ = ',';
    nBuffLen--;

    len = ftoa(pDwr->db_Lat,ptr,2,-1);//纬度
    nBuffLen -= len;
    ptr += len;
    *ptr++ = ',';
    nBuffLen--;

    *ptr++ = pDwr->cLatDir;//纬度方向
    nBuffLen--;
    *ptr++ = ',';
    nBuffLen--;
    
    len = ftoa(pDwr->db_Lon,ptr,2,-1);//经度
    nBuffLen -= len;
    ptr += len;
    *ptr++ = ',';
    nBuffLen--;
    
    *ptr++ = pDwr->cLonDir;//经度方向
    nBuffLen--;
    *ptr++ = ',';
    nBuffLen--;    
    
    len = ftoa(pDwr->db_Height, ptr, 2, -1);//大地高
    nBuffLen -= len;
    ptr += len;
    *ptr++ = ',';
    nBuffLen--;
    
    *ptr++ = pDwr->cHunit;//大地高单位
    nBuffLen--;
    *ptr++ = ',';
    nBuffLen--;  

    len = ftoa(pDwr->f_Kesi, ptr, 2, -1);//高程异常
    nBuffLen -= len;
    ptr += len;
    *ptr++ = ',';
    nBuffLen--;
    
    *ptr++ = pDwr->cKesiUnit;//高程异常单位
    nBuffLen--;
    *ptr++ = ',';
    nBuffLen--; 
    //精度指示,紧急定位指示,多值解指示,高程类型指示
    len = sprintf(ptr,"%d,%c,%c,%c",pDwr->nPrecision,pDwr->cUrgencyPos
                                   ,pDwr->cMutilpleValue,pDwr->cLocaType);
    nBuffLen -= len;
    ptr += len;
    if(nBuffLen < 0)
    {
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);
}

//////////////////////////////////////////////////////////////////////////
//函数说明：协议解码
//参数说明：协议结构体，需要解码的buff
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int RD_DecodeFki(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDFKI *pFki = (TRDFKI *)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    uchar temp_time = 0;
      
    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 7)//数据长度过长
        return RESULT_VERIFY_ERROR;

    pdt = pkg.pItem[1];
    memcpy(pFki->cCmdName,pdt,3);//指令
    
    pdt = pkg.pItem[2];
    pFki->cCmdExecFlag = *pdt;//执行情况
    
    pdt = pkg.pItem[3];    
    pFki->cFreqSetFlag = *pdt;//频度设置指示

    pdt = pkg.pItem[4];
    pFki->nRestrainFlag = atoi(ptr);//发射抑制

    pdt = pkg.pItem[5];
    pFki->nWaitSecond = atoi(pdt+2);//等待时间    
    *(pdt+2) = '\0';
    pFki->nWaitHour = atoi(pdt);//

    
    return RESULT_SUCCESS;
}
int RD_EncodeFki(char *lpBuff, int nBuffLen, void* pPackage)
{
    int nRet = -1;
    TRDFKI *pFki = (TRDFKI *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    //指令名称,指令执行情况,频度设置指示,发射抑制指示,等待时间
    len = sprintf(ptr,"%s,%c,%c,%d,%02d%02d",pFki->cCmdName,pFki->cCmdExecFlag,pFki->cFreqSetFlag
                                          ,pFki->nRestrainFlag,pFki->nWaitHour,pFki->nWaitSecond);
	ptr += len; 
    return (ptr - lpBuff);
}

//////////////////////////////////////////////////////////////////////////
//函数说明：协议解码
//参数说明：协议结构体，需要解码的buff
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int RD_DecodeGxm(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDGXM* pGxm = (TRDGXM*)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    uchar i = 0;
    
    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 4)//数据长度过长
        return RESULT_VERIFY_ERROR;

    pGxm->cCmdType = atoi(pkg.pItem[1]);//指令类型
    
    ptr = pkg.pItem[2];
    if(pGxm->cCmdType != 2)//不是读取管理信息指令
    {
        for(i = 0; i < 32; i++)
        {
            pGxm->MngInfo[i] = atoh8(ptr);//管理信息
            ptr += 2;
        }
    }
	    
    return RESULT_SUCCESS;
}

int RD_EncodeGxm(char *lpBuff, int nBuffLen, void* pPackage)
{
    int nRet = -1;
    TRDGXM *pGxm = (TRDGXM *)pPackage;
    char *ptr = lpBuff;
    
    nRet = sprintf(ptr,"%01d,",pGxm->cCmdType);//指令类型
    nBuffLen -= nRet;
    ptr += nRet;
    
    if(pGxm->cCmdType == 1) //set
    {
		memcpy(ptr, pGxm->MngInfo, 64);//管理信息
        nBuffLen -= 64;
        ptr += 64;
    }
	else if (pGxm->cCmdType == 3)//res
	{
		for (nRet = 0; nRet < 32; nRet++)
		{
			sprintf(ptr,"%02X",pGxm->MngInfo[nRet]);//管理信息
        	nBuffLen -= 2;
        	ptr += 2;	
		}
	}
    
    if(nBuffLen < 0)
    {
        return RESULT_ERROR;
    }  
    
    return (ptr - lpBuff);
}

int RD_DecodeIca(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDICA *pIca = (TRDICA *)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    uchar i = 0;
    
    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 4)//数据长度过长
        return RESULT_VERIFY_ERROR;

    pdt = pkg.pItem[1];
    pIca->nCmdType = atoi(pdt);
    
    pdt = pkg.pItem[2];
    pIca->nFrameNum= atoi(pdt); 
    
    return RESULT_SUCCESS;
}
int RD_EncodeIca(char *lpBuff, int nBuffLen, void* pPackage)
{
    int nRet = -1;
    TRDICA *pIca = (TRDICA *)pPackage;
    char *ptr = lpBuff;
    int len = 0;

    len = sprintf(ptr,"%01d,%01d",pIca->nCmdType,pIca->nFrameNum);
    nBuffLen -= len;
    ptr += len;
    
    if(nBuffLen < 0)
    {
        return RESULT_ERROR;
    }
    return (ptr - lpBuff);
}

//////////////////////////////////////////////////////////////////////////
//函数说明：协议解码
//参数说明：协议结构体，需要解码的buff
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int RD_DecodeIci(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDICI* pIci = (TRDICI*)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    uchar i = 0;
    
    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 10)//数据长度过长
        return RESULT_VERIFY_ERROR;

    pdt = pkg.pItem[1];
    pIci->nUserID = atoi(pdt);//用户ID
    
    pdt = pkg.pItem[2];
    pIci->nSerialNum = atoi(pdt); //序列号
    
    pdt = pkg.pItem[3];
    pIci->nBrdID= atoi(pdt);//通播ID
    
    pdt = pkg.pItem[4];
    pIci->nUserType = atoi(pdt);//用户特征值
    pdt = pkg.pItem[5];
    pIci->f_Freq = atof(pdt);  //频度
    
    pdt = pkg.pItem[6];
    pIci->nCommLevel= atoi(pdt);  //通信等级
    pdt = pkg.pItem[7];
    pIci->cEncryptFlag = *pdt;  //加密标志
    pdt = pkg.pItem[8];
    pIci->nSubuserNum = atoi(pdt);  //下属用户机的个数
    
    return RESULT_SUCCESS;
}
int RD_EncodeIci(char *lpBuff, int nBuffLen, void* pPackage)
{
    int nRet = -1;
    TRDICI *pIci = (TRDICI *)pPackage;
    char *ptr = lpBuff;
    int len = 0;
    //用户地址,序列号,通播地址,用户特征值
    len = sprintf(ptr,"%07d,%08d,%07d,%01d,",pIci->nUserID,pIci->nSerialNum,pIci->nBrdID
                                            ,pIci->nUserType);
    ptr += len;
    nBuffLen -= len;

    len = ftoa(pIci->f_Freq, ptr, 1, -1);//服务频度
    ptr += len;
    nBuffLen -= len;   
    //通信等级,加密标志,下属用户机的个数
    len = sprintf(ptr,",%d,%c,%d",pIci->nCommLevel,pIci->cEncryptFlag,pIci->nSubuserNum);
    ptr += len;
    nBuffLen -= len; 

    return (ptr - lpBuff);
}

//////////////////////////////////////////////////////////////////////////
//函数说明：协议解码
//参数说明：协议结构体，需要解码的buff
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int RD_DecodeIcz(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDICZ* pIcz = (TRDICZ*)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    uchar i = 0;
    int nlen = 0;
    
    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > MAX_DATA_ITEM)//数据长度过长
        return RESULT_VERIFY_ERROR;

    pdt = pkg.pItem[1]; 
    pIcz->nSubuserNum = atoi(pdt);
    if(pIcz->nSubuserNum <= (MAX_DATA_ITEM - 3))//提取',',可以一次性提取完成
    {
        for(i = 0; i< pIcz->nSubuserNum; i++)//
        {
            pdt = pkg.pItem[2+i]; 
            pIcz->nSubuserID[i] = atoi(pdt);
        }
    }
    else//不能一次性提取完成
    {
         for(i = 0; i< (MAX_DATA_ITEM - 3); i++)//提取前20个
        {
            pdt = pkg.pItem[2+i]; 
            pIcz->nSubuserID[i] = atoi(pdt);
        }   
       
        ptr = pkg.pItem[MAX_DATA_ITEM-1];
        nlen = nDataLen - (ptr-lpBuff);//are you OK?
        DecodePackage_Ex(&pkg, ptr, nlen, ",*");
        if(pkg.nItems < 1)//没有可解析的数据
            return RESULT_VERIFY_ERROR;
        else if(pkg.nItems > MAX_DATA_ITEM)//数据长度过长
            return RESULT_VERIFY_ERROR;            
         for(i = 0; i< pkg.nItems; i++)//提取前20个
        {
            pdt = pkg.pItem[i]; 
            pIcz->nSubuserID[i+20] = atoi(pdt);
        }        
    }
    return RESULT_SUCCESS;
}
int RD_EncodeIcz(char *lpBuff, int nBuffLen, void* pPackage)
{
    int nRet = -1;
    TRDICZ *pIcz = (TRDICZ *)pPackage;
    char *ptr = lpBuff;
    int len = 0,i = 0;    

    len = sprintf(ptr,"%d",pIcz->nSubuserNum);//下属用户个数
    ptr += len;
    nBuffLen -= len;

    for(i = 0; i<pIcz->nSubuserNum; i++)
    {
        len = sprintf(ptr,",%d",pIcz->nSubuserID[i]);//下属用户ID
        ptr += len;
        nBuffLen -= len;
    }
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }

    
    return (ptr - lpBuff);
}

int RD_DecodeJms(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDJMS *pJms = (TRDJMS *)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    uchar i = 0;
    
    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 3)//数据长度过长
        return RESULT_VERIFY_ERROR;

    pdt = pkg.pItem[1];
    pJms->cJMFlag = *pdt;        
    return RESULT_SUCCESS;
}
int RD_EncodeJms(char *lpBuff, int nBuffLen, void* pPackage)
{
    int nRet = -1;
    TRDJMS *pJms = (TRDJMS *)pPackage;
    char *ptr = lpBuff;
    int len = 0,i = 0;    

    len = sprintf(ptr,"%c",pJms->cJMFlag);//静默指示
    ptr += len;
    nBuffLen -= len;
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }

    
    return (ptr - lpBuff);
}

int RD_DecodeKls(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDKLS *pKls = (TRDKLS *)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    uchar i = 0;
    
    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 4)//数据长度过长
        return RESULT_VERIFY_ERROR;

    pdt = pkg.pItem[1];
    pKls->nUserID = atoi(pdt);//用户地址
    pdt = pkg.pItem[2];
    pKls->cRequestFlag = *pdt;//应答标志
    
    return RESULT_SUCCESS;
}
int RD_EncodeKls(char *lpBuff, int nBuffLen, void* pPackage)
{
    int nRet = -1;
    TRDKLS *pKls = (TRDKLS *)pPackage;
    char *ptr = lpBuff;
    int len = 0,i = 0;    

    len = sprintf(ptr,"%d,%c",pKls->nUserID,pKls->cRequestFlag);
    ptr += len;
    nBuffLen -= len;
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }

    
    return (ptr - lpBuff);
}

//////////////////////////////////////////////////////////////////////////
//函数说明：协议解码
//参数说明：协议结构体，需要解码的buff
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int RD_DecodeKlt(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDKLT* pKlt = (TRDKLT*)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    uchar i = 0,nlen = 0;
    int temp1 = 0,temp2 = 0;
    
    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 6)//数据长度过长
        return RESULT_VERIFY_ERROR;
        
    pdt = pkg.pItem[1];
    pKlt->cUserType = *pdt;
    pdt = pkg.pItem[2];
    pKlt->nUserID = atoi(pdt);
    pdt = pkg.pItem[3];
    pKlt->nInfoType = atoi(pdt);  
    
    pdt = pkg.pItem[4];
    nlen = pkg.pItem[5] - pdt;
    if(pKlt->nInfoType == 0)//汉字
    {
        for(i = 0; i< nlen; i++)

        {
            temp1 = *(pdt+i)&0x00ff;
            i++; 
            temp2 = *(pdt+i)&0x00ff;
            pKlt->nInfo[i]= (temp1<<8)|temp2 ;//机内码
            pKlt->nInfolen++;
        }
    }
    else//代码
    {
        for(i = 0; i<nlen; i++)
        {
            pKlt->nInfo[i] = atoi(pdt + 1);
            pKlt->nInfolen++;
        }
    }
    return RESULT_SUCCESS;
}
int RD_EncodeKlt(char *lpBuff, int nBuffLen, void* pPackage)
{
    int nRet = -1;
    TRDKLT *pKlt = (TRDKLT *)pPackage;
    char *ptr = lpBuff;
    int len = 0,i = 0,temp = 0;    

    len = sprintf(ptr,"%c,%d,%d,",pKlt->cUserType,pKlt->nUserID,pKlt->nInfoType);
    ptr += len;
    nBuffLen -= len;

    if(pKlt->nInfoType == 0)//汉字
    {
        for(i = 0; i< pKlt->nInfolen; i++)
        {
            temp = pKlt->nInfo[i]>>8;
            *ptr++ = temp&0x00ff;
            *ptr++ = pKlt->nInfo[i]&0x00ff;
            nBuffLen -= 2;
        }
        
    }
    else//代码
    {   
        for(i = 0; i< pKlt->nInfolen; i++)
        {
            len = sprintf(ptr,"%d",pKlt->nInfo[i]);
            ptr += len;
            nBuffLen -= len;
        }
    }
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }

    
    return (ptr - lpBuff);
}

//////////////////////////////////////////////////////////////////////////
//函数说明：协议解码
//参数说明：协议结构体，需要解码的buff
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int RD_DecodeLzm(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDLZM *pLzm = (TRDLZM *)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;

    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 4)//数据长度过长
        return RESULT_VERIFY_ERROR;

    pdt = pkg.pItem[1];
    pLzm->nMode = atoi(pdt);
    if(pLzm->nMode == 1)
    {
        pLzm->dbZeroValue = 0.0;
    }
    else
    {
        pdt = pkg.pItem[2];
        pLzm->dbZeroValue = atof(pdt);
    }    
    return RESULT_SUCCESS;
}

int RD_EncodeLzm(char *lpBuff, int nBuffLen, void* pPackage)
{
    int nRet = -1;
    TRDLZM *pLzm = (TRDLZM *)pPackage;
    char *ptr = lpBuff;  
	int len = 0;
    len = sprintf(ptr,"%d,",pLzm->nMode);//管理模式
    ptr += len;
    nBuffLen -= len;

    ftoa(pLzm->dbZeroValue,ptr,1,-1);//设备零值
    ptr += len;
    nBuffLen -= len;

    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }

    
    return (ptr - lpBuff);
}
//////////////////////////////////////////////////////////////////////////
//函数说明：协议解码
//参数说明：协议结构体，需要解码的buff
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int RD_DecodeHzr(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDHZR* pHzr = (TRDHZR*)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    uchar i = 0;

    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 14)//数据长度过长
        return RESULT_VERIFY_ERROR;

    pdt = pkg.pItem[1];
    pHzr->nUserID = atoi(pdt);//用户ID

    pdt = pkg.pItem[2];
    pHzr->nHZNum = atoi(pdt);//回执数

    for(i = 0; i < pHzr->nHZNum; i++)
    {
        pdt = pkg.pItem[3+i];//发信
        pHzr->SendTime[i].nMinute = atoi(pdt + 2);//分
        *(pdt + 2) = '\0';
        pHzr->SendTime[i].nHour= atoi(pdt);//时
        
        pdt = pkg.pItem[4+i];//回执
        pHzr->RespTime[i].nMinute = atoi(pdt + 2);//分
        *(pdt + 2) = '\0';
        pHzr->RespTime[i].nHour= atoi(pdt);//时

    }
    
    return RESULT_SUCCESS;
}

int RD_EncodeHzr(char *lpBuff, int nBuffLen, void* pPackage)
{
    int nRet = -1;
    TRDHZR *pHzr = (TRDHZR *)pPackage;
    char *ptr = lpBuff;
    int len = 0,i = 0;  
    
    //用户ID和回执数
    len = sprintf(ptr,"%d,%d",pHzr->nUserID,pHzr->nHZNum);
    ptr += len;
    nBuffLen -= len;
    //回执时间
    for(i = 0; i< pHzr->nHZNum;i++)
    {
        len = sprintf(ptr,",%02d%02d,%02d%02d",pHzr->SendTime[i].nHour,pHzr->SendTime[i].nMinute
                                              ,pHzr->RespTime[i].nHour,pHzr->RespTime[i].nMinute);
        ptr += len;
        nBuffLen -= len;
    }
    
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }

    
    return (ptr - lpBuff);
} 

char RD_C2A(char ch)
{
    if (ch >= 'A' && ch <= 'F')
    {
        return (ch - 'A' + 0xA);
    }
    else if (ch >= 'a' && ch <= 'f')
    {
        return (ch - 'a' + 0xA);
    }
    else if (ch >= '0' && ch <= '9')
    {
        return ch - '0';
    }
    else
    {
        return 0;
    }
}

char RD_A2C(char ch)
{
    if (ch >= 0x0A && ch <= 0x0F)
    {
        return ('A' + ch - 0xA);
    }
    else if (ch >= 0 && ch <= 9)
    {
        return ch + '0';
    }
    else
    {
        return 0;
    }
}

int RD_DecodeTxa(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDTXA *pTxa = (TRDTXA *)pPackage;

    char str_SendTime[5] = {0};
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    uchar i = 0,nlen = 0;
    int temp1= 0,temp2= 0; 

    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");

    if(pkg.nItems < 6)
        return RESULT_VERIFY_ERROR;

    pdt = pkg.pItem[1];
    pTxa->nUserID = atoi(pdt);

    pdt = pkg.pItem[2];
    pTxa->nCommMode = atoi(pdt);
    pdt = pkg.pItem[3];
    pTxa->nInfoType = atoi(pdt);
    
	pdt = pTxa->Info; 
	ptr = pkg.pItem[4];
	pTxa->nInfolen = 0;
	if (pTxa->nInfoType == 1)
	{
		if (ptr[0] == 'A' && ptr[1] == '4')
		{
			pTxa->nInfoType = 2;
		}
        else if (ptr[0] == 'A' && ptr[1] == '0')//位置上报
		{
			pTxa->nInfoType = 3;
		}
	}
    
    if (pTxa->nInfoType == 2)
    {
        ptr += 2;	
        if (ptr[0] == 'A' && ptr[1] == '0')//位置上报
		{
			pTxa->nInfoType = 3;
		}
    }
    
	if (pTxa->nInfoType == 3) //
	{
        ptr += 2;
	}
	
	if (pTxa->nInfoType == 0)
	{
		pTxa->nInfolen = strlen(ptr);
		memcpy(pdt, ptr, pTxa->nInfolen);

		pdt += pTxa->nInfolen;
	}
	else if (pTxa->nInfoType == 1)
	{
		while (*ptr)
		{
			*pdt++ = RD_C2A(*(ptr++));
			pTxa->nInfolen++;
		}
	}
	else
	{
		while (*ptr)
		{
			*pdt = RD_C2A(*(ptr++));
	        *pdt <<= 4;
	        *pdt |= RD_C2A(*(ptr++));

			pdt++;
			pTxa->nInfolen++;
		}	
	}
	
    *pdt = '\0';

    return RESULT_SUCCESS;
}

int RD_EncodeTxa(char *lpBuff, int nBuffLen, void* pPackage)
{
    int nRet = -1;
    TRDTXA *pDat = (TRDTXA *)pPackage;
    char *ptr = lpBuff;
    int len = 0, i = 0;  
    
    //信息类别,用户ID,电文类型,发信时间
    len = sprintf(ptr,"%d,%d,%d,", pDat->nUserID, pDat->nCommMode, (pDat->nInfoType == 3 ? 2 : pDat->nInfoType));
    ptr += len;
    nBuffLen -= len;

    if(pDat->nInfoType == 0 )//汉字
    {
        for (i = 0; i < pDat->nInfolen; i++)
        {
            if (pDat->Info[i] == ',')
            {
                *ptr++ = 0xA3;
                *ptr++ = 0xAC;
                nBuffLen -= 2;
            }
            else if (pDat->Info[i] == '*')
            {
                *ptr++ = 0xA9;
                *ptr++ = 0x7E;
                nBuffLen -= 2;
            }
            else
            {
                *ptr++ = pDat->Info[i];
                nBuffLen--;
            }
            
            if (nBuffLen == 0)
                break;
        }
    }
    else if (pDat->nInfoType == 1)//代码
    {
       /*for(i = 0; i< pDat->nInfolen; i++)
       {
            *ptr++ = RD_A2C(pDat->Info[i] & 0xF);
            nBuffLen--;
       }*/
		memcpy(ptr, pDat->Info, pDat->nInfolen);//modify by zhengyongchao 20141125
		ptr += pDat->nInfolen;
		nBuffLen -= pDat->nInfolen;
    }
    else 
    {   
        if (pDat->nInfoType == 2)//混发
        {
            *ptr++ = 'A';
            *ptr++ = '4';
            nBuffLen -= 2;
        }
        else if (pDat->nInfoType == 3)
        {
            *ptr++ = 'A';
            *ptr++ = '4';
            *ptr++ = 'A';
            *ptr++ = '0';
            nBuffLen -= 4;
        }
        
        for(i = 0; i< pDat->nInfolen; i++)
        {
            *ptr++ = RD_A2C((pDat->Info[i] >> 4) & 0xF);
            *ptr++ = RD_A2C(pDat->Info[i] & 0xF);
            nBuffLen -= 2;
            
            if (nBuffLen < 2)
                break;
        }
    } 
     
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    
    return (ptr - lpBuff);
} 

//////////////////////////////////////////////////////////////////////////
//函数说明：协议解码
//参数说明：协议结构体，需要解码的buff
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int RD_DecodeTxr(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDTXR *pDat = (TRDTXR *)pPackage;

    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    uchar i = 0,nlen = 0;
    int temp1= 0,temp2= 0; 

    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 7)//没有可解析的数据
        return RESULT_VERIFY_ERROR;

    pDat->nMsgType = atoi(pkg.pItem[1]);

    PT_DecodeString(pDat->szUserID, sizeof(pDat->szUserID), pkg.pItem[2]);
    pDat->nUserID = atoi(pkg.pItem[2]);
    pDat->nInfoType = atoi(pkg.pItem[3]);
    pdt = pkg.pItem[4];
    if (strlen(pdt) >= 4)
    {
        pdt = pkg.pItem[4];
        pDat->nSendMin = atoi(pdt +2);
        *(pdt + 2) = '\0';
        pDat->nSendHour = atoi(pdt);
    }
    else
    {
        pDat->nSendMin = 0;
        pDat->nSendHour = 0;
    }
    
    nlen = pkg.pItem[6] - pkg.pItem[5];
    
	pdt = pDat->Info; 
	ptr = pkg.pItem[5];
	pDat->nInfolen = 0;
	if (pDat->nInfoType == 1)
	{
		if (ptr[0] == 'A' && ptr[1] == '4')
		{
			pDat->nInfoType = 2;
        }
		else if (ptr[0] == 'A' && ptr[1] == '0')//位置上报
		{
			pDat->nInfoType = 3;
		}
	}
    
	if (pDat->nInfoType == 2) //
	{
		ptr += 2;

        if (ptr[0] == 'A' && ptr[1] == '0')//位置上报
		{
			pDat->nInfoType = 3;
		}
	}
    
    if (pDat->nInfoType == 3)
    {
        ptr += 2;	
    }
	
	if (pDat->nInfoType == 0)
	{
		pDat->nInfolen = strlen(ptr);
		memcpy(pdt, ptr, pDat->nInfolen);

		pdt += pDat->nInfolen;
	}
	else if (pDat->nInfoType == 1)
	{
		/*while (*ptr)
		{
			*pdt++ = RD_C2A(*(ptr++));
			pDat->nInfolen++;
		}*/
		pDat->nInfolen = strlen(ptr);	//modify by zhengyongchao 20141124
		memcpy(pdt, ptr, pDat->nInfolen);

		pdt += pDat->nInfolen;
	}
	else
	{
		while (*ptr)
		{
			*pdt = RD_C2A(*(ptr++));
	        *pdt <<= 4;

            if (*ptr)
            {
	            *pdt |= RD_C2A(*(ptr++));
                pdt++;
                pDat->nInfolen++;
            }
            else//半字节
            {
                *pdt |= 0;

                pdt++;
                pDat->nInfolen++;

                break;
            }
		}	
	}
	
    *pdt = '\0';

    return RESULT_SUCCESS;
}

int RD_EncodeTxr(char *lpBuff, int nBuffLen, void* pPackage)
{
    int nRet = -1;
    TRDTXR *pDat = (TRDTXR *)pPackage;
    char *ptr = lpBuff;
    int len = 0, i = 0, temp = 0;  
    
    //信息类别,用户ID,电文类型,发信时间
    if (pDat->nUserID == 0)
    {
        pDat->szUserID[15] = '\0';
        len = sprintf(ptr,"%d,%s,%d,%02d%02d,",pDat->nMsgType,pDat->szUserID,
            (pDat->nInfoType == 3 ? 1 : pDat->nInfoType),pDat->nSendHour,pDat->nSendMin);
    }
    else
    {
        len = sprintf(ptr,"%d,%07d,%d,%02d%02d,",pDat->nMsgType,pDat->nUserID,
            (pDat->nInfoType == 3 ? 1 : pDat->nInfoType),pDat->nSendHour,pDat->nSendMin);
    }
    
    ptr += len;
    nBuffLen -= len;

    
    if(pDat->nInfoType == 0 )//汉字
    {
		for (i = 0; i < pDat->nInfolen; i++)
        {
            if (pDat->Info[i] == ',')
            {
                *ptr++ = 0xA3;
                *ptr++ = 0xAC;
                nBuffLen -= 2;
            }
            else if (pDat->Info[i] == '*')
            {
                *ptr++ = 0xA9;
                *ptr++ = 0x7E;
                nBuffLen -= 2;
            }
            else
            {
                *ptr++ = pDat->Info[i];
                nBuffLen--;
            }
            
            if (nBuffLen == 0)
                break;
        }
    }
    else if(pDat->nInfoType == 1 )//代码
    {
       for(i = 0; i< pDat->nInfolen; i++)
       {
           *ptr++ = RD_A2C(pDat->Info[i] & 0xF);
            nBuffLen--;
       }
    }
    else 
    {   
        if (pDat->nInfoType == 2)//混发
        {
            *ptr++ = 'A';
            *ptr++ = '4';
            nBuffLen -= 2;
        }
        else if (pDat->nInfoType == 3)//位置报告
        {
            *ptr++ = 'A';
            *ptr++ = '4';
            *ptr++ = 'A';
            *ptr++ = '0';
            nBuffLen -= 4;
        }

        for(i = 0; i< pDat->nInfolen; i++)
        {
            *ptr++ = RD_A2C((pDat->Info[i] >> 4) & 0xF);
            *ptr++ = RD_A2C(pDat->Info[i] & 0xF);
            
            nBuffLen -= 2;
            if (nBuffLen < 2)
                break;
        }
     } 
     
    if(nBuffLen < 0)
    {   
        return RESULT_ERROR;
    }
    
    return (ptr - lpBuff);
} 

//////////////////////////////////////////////////////////////////////////
//函数说明：协议解码
//参数说明：协议结构体，需要解码的buff
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int RD_DecodeWaa(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDWAA* pWaa = (TRDWAA*)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    uchar i = 0,nlen = 0;
    int temp1= 0,temp2= 0; 
	double dbtemp = 0.0;

    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 12)//数据长度过长
        return RESULT_VERIFY_ERROR;
        
    pdt = pkg.pItem[1];
    pWaa->nMsgType = atoi(pdt);
    if(pWaa->nMsgType == 0)
    {
        pWaa->f_Freq =0.0;   
    }
    else
    {
        pdt = pkg.pItem[2];
        pWaa->f_Freq = atof(pdt);
    }
    pdt = pkg.pItem[3];
    pWaa->nUserID = atoi(pdt);
    pdt = pkg.pItem[4];
    pWaa->f_ReportSecond = atof(pdt+4);
    *(pdt+4) = '\0';
    pWaa->nReportMin= atoi(pdt+2);
    *(pdt+2) = '\0';
    pWaa->nReportHour = atoi(pdt);
    pdt = pkg.pItem[5];
	/*dbtemp = atof(pdt);//纬度  3409.15
	pWaa->f_Lat = (int)(dbtemp / 100);
	dbtemp = dbtemp - pWaa->f_Lat*100;
	pWaa->f_Lat += (dbtemp/60.0);*/

    pWaa->f_Lat = atof(pdt);

    pdt = pkg.pItem[6];
    pWaa->cLatAspect = *pdt;
    pdt = pkg.pItem[7];
	/*dbtemp = atof(pdt);//经度
	pWaa->f_Lon = (int)(dbtemp / 100);
	dbtemp = dbtemp - pWaa->f_Lon*100;
	pWaa->f_Lon += (dbtemp/60.0);*/
    pWaa->f_Lon = atof(pdt);
    pdt = pkg.pItem[8];
    pWaa->cLonAspect = *pdt;
    pdt = pkg.pItem[9];
    pWaa->f_Height = atof(pdt);
    pdt = pkg.pItem[10];
    pWaa->cHeightUnit = *pdt;
    
    return RESULT_SUCCESS;
}
int RD_EncodeWaa(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TRDWAA* pWaa = (TRDWAA*)pPackage;
	char *ptr = lpBuff;

	*ptr++ = pWaa->nMsgType + 0x30;
	*ptr++ = ',';
	nBuffLen -= 2;

	if (pWaa->nMsgType == 1)
	{
		nRet = sprintf(ptr, "%d.0", (int)pWaa->f_Freq);
		ptr += nRet;
		nBuffLen -= nRet;
	}

	nRet = sprintf(ptr, ",%07d,%02d%02d", pWaa->nUserID,pWaa->nReportHour,pWaa->nReportMin);
	ptr += nRet;
	nBuffLen -= nRet;

	nRet = ftoa(pWaa->f_ReportSecond,ptr, 2, 2);//秒 ss.ss
    nBuffLen -= nRet;
    ptr += nRet;

    *ptr++ = ',';
    nBuffLen--;

    nRet = ftoa(pWaa->f_Lat,ptr,6,-1);//纬度
    nBuffLen -= nRet;
    ptr += nRet;
    *ptr++ = ',';
    nBuffLen--;

    *ptr++ = pWaa->cLatAspect;//纬度方向
    nBuffLen--;
    *ptr++ = ',';
    nBuffLen--;
    
    nRet = ftoa(pWaa->f_Lon,ptr,6,-1);//经度
    nBuffLen -= nRet;
    ptr += nRet;
    *ptr++ = ',';
    nBuffLen--;
    
    *ptr++ = pWaa->cLonAspect;//经度方向
    nBuffLen--;
    *ptr++ = ',';
    nBuffLen--;    
    
    nRet = ftoa(pWaa->f_Height, ptr, 2, -1);//大地高
    nBuffLen -= nRet;
    ptr += nRet;
    *ptr++ = ',';
    nBuffLen--;
    
    *ptr++ = pWaa->cHeightUnit;//大地高单位
    nBuffLen--;

    return ptr - lpBuff;

}

int RD_DecodeWba(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDWBA *pWba = (TRDWBA *)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    uchar i = 0,nlen = 0;
    int temp1= 0,temp2= 0; 

    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 6)//数据长度过长
        return RESULT_VERIFY_ERROR;

    pdt = pkg.pItem[1];
    pWba->nUserID = atoi(pdt);
    pdt = pkg.pItem[2];
    pWba->cHeightflag = *pdt;    
    pdt = pkg.pItem[3];
    pWba->f_AnteHeight = atof(pdt);
    pdt = pkg.pItem[4];
    pWba->f_Freq = atof(pdt);

    return RESULT_SUCCESS;       
}
int RD_EncodeWba(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TRDWBA* pWba = (TRDWBA*)pPackage;

	nRet = sprintf(lpBuff, "%d,", pWba->nUserID);
	lpBuff[nRet++] = pWba->cHeightflag;

	nRet += sprintf(lpBuff + nRet, ",%.2f,", pWba->f_AnteHeight);
	nRet += sprintf(lpBuff + nRet, "%.2f", pWba->f_Freq);

    return nRet;
}

int RD_DecodeZhs(void *pPackage, char *lpBuff, int nDataLen)
{
    TRDZHS *pZhs = (TRDZHS *)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    uchar i = 0,nlen = 0;
    int temp1= 0,temp2= 0; 

    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//没有可解析的数据
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 3)//数据长度过长
        return RESULT_VERIFY_ERROR;
    pdt = pkg.pItem[1];
    memcpy(pZhs->cSelfDestory, pdt, 9);
    return RESULT_SUCCESS;       
}
//////////////////////////////////////////////////////////////////////////
//函数说明：协议编码
//参数说明：协议结构体，编码后将放入buff中
//返  回  值：无
//////////////////////////////////////////////////////////////////////////
int RD_EncodeZhs(char *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = -1;
    TRDZHS *pZhs = (TRDZHS *)pPackage;


    return nRet;
}
#endif //end PT_ENABLE_BD2_RD define









