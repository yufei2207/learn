/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name��     pt_bd2_rnss.c
* Abstract�� BD2 RNSS protocol define 
* 
* Version�� 1.0
* Author��  yfwang
* Date��    2012/03/19
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

//����RNЭ��ͷ����
const uchar BD2_RN_Head[][4] = { "AAM","ALM","APL","BSD","BSQ","BSX" 
								,"COM","DHV","GBS","GGA","GLL","GLS"
								,"GSA","GST","GSV","ICM","IHI","IHO"
								,"LPM","MSS","PMU","RMC","TXT","VTG"
                                ,"ZBS","ZDA","ZTI"};
//--------------------------------------------------------------------------
//Э��ջ����뺯������
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
//Э��ջ��������
//--------------------------------------------------------------------------
/*
*�������ƣ�BD2_RN_DecodeHead
*����˵����������Ϣ����
*����˵����buf    �������ݻ���
*          len    �������ݳ���
*����ֵ��  -1 ��ʾ��Ч����Ϣ���ͣ�������ʾ����������
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
	        nCount = sizeof(BD2_RN_Head) / sizeof(BD2_RN_Head[0]);//�������Э�����
	        for(i = 0; i< nCount; i++)
	        {
		        if(strncmp(buf + 3, BD2_RN_Head[i], strlen(BD2_RN_Head[i])) == 0)	//Э��ͷ�ж�
		        {
			        return ( i + BD2_RN_ZERO);
		        }
	        }
        }
    }

	return -1;
}
/*
*�������ƣ�BD2_RN_EncodeHead
*����˵��������Э��ͷ
*����˵����lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          bDevice   �Ƿ��豸
*          nType     ��Ϣ����
*����ֵ��  -1 ��ʾʧ�ܣ�������ʾ�������ݳ���
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
*�������ƣ�BD2_RN_GetEncoder
*����˵����������Ϣ���ͻ�ȡ��Ӧ�ı��뺯��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�ޱ��뺯����������ʾ���뺯��ָ��
*/
FUNC_ENCODER BD2_RN_GetEncoder(int nType)
{
    switch (nType)
    {
    case BD2_RN_AAM://��·�㵽�ﱨ��
    case BD2_RN_ALM://������������
    case BD2_RN_APL://����Ա�������
    case BD2_RN_BSD://������ʶ��
    case BD2_RN_BSQ://������ʶ��
    case BD2_RN_BSX://������ʶ��
    case BD2_RN_COM://�����û��豸����
        return RN_COM_Encode;
    case BD2_RN_DHV://�ٶ��ർ����Ϣ
        return RN_DHV_Encode;
    case BD2_RN_GBS://����GNSS���ǹ��ϼ��
        return RN_GBS_Encode;
    case BD2_RN_GGA://������λ����
        return RN_GGA_Encode;
    case BD2_RN_GLL://������궨λ��Ϣ
        return RN_GLL_Encode;
    case BD2_RN_GLS://�����û��豸λ�õȳ�ʼ����Ϣ
        return RN_GLS_Encode;
    case BD2_RN_GSA://�����û��豸����ģʽ��
        return RN_GSA_Encode;
    case BD2_RN_GST://����α�����ͳ������
        return RN_GST_Encode;
    case BD2_RN_GSV://���������Ϣ
        return RN_GSV_Encode;
    case BD2_RN_ICM://����û��豸�мӽ���ģ����Կ��Ч����Ϣ
        return RN_ICM_Encode;
    case BD2_RN_IHI://�����û��豸����ٶȼ��ٶȵ���Ϣ
        return RN_IHI_Encode;
    case BD2_RN_IHO://�������������Ϣ
        return RN_IHO_Encode;
    case BD2_RN_LPM://�����豸������ʡ��ģʽ
        return RN_LPM_Encode;
    case BD2_RN_MSS://�����豸��ǰ��λ��ʽ
        return RN_MSS_Encode;
    case BD2_RN_PMU://���PRM��ʱЧ������Ϣ
        return RN_PMU_Encode;
    case BD2_RN_RMC://�����򵼺���������
        return RN_RMC_Encode;
    case BD2_RN_TXT://���ڴ�����ı�
        return RN_TXT_Encode;
    case BD2_RN_VTG://����������͵���
    case BD2_RN_ZBS://��������ϵ����
        return RN_ZBS_Encode;
    case BD2_RN_ZDA://����UTCʱ�䡢���ںͱ���ʱ��
        return RN_ZDA_Encode;
    case BD2_RN_ZTI://����豸��ǰ����״̬��Ϣ
        return RN_ZTI_Encode;
    default:
        return NULL;
    }
}
/*
*�������ƣ�BD2_RN_GetDecoder
*����˵����������Ϣ���ͻ�ȡ��Ӧ�Ľ��뺯��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�޽��뺯����������ʾ���뺯��ָ��
*/
FUNC_DECODER BD2_RN_GetDecoder(int nType)
{
    switch (nType)
    {
    case BD2_RN_AAM://��·�㵽�ﱨ��
    case BD2_RN_ALM://������������
    case BD2_RN_APL://����Ա�������
    case BD2_RN_BSD://������ʶ��
    case BD2_RN_BSQ://������ʶ��
    case BD2_RN_BSX://������ʶ��
    case BD2_RN_COM://�����û��豸����
        return RN_COM_Decode;
    case BD2_RN_DHV://�ٶ��ർ����Ϣ
        return RN_DHV_Decode;
    case BD2_RN_GBS://����GNSS���ǹ��ϼ��
        return RN_GBS_Decode;
    case BD2_RN_GGA://������λ����
        return RN_GGA_Decode;
    case BD2_RN_GLL://������궨λ��Ϣ
        return RN_GLL_Decode;
    case BD2_RN_GLS://�����û��豸λ�õȳ�ʼ����Ϣ
        return RN_GLS_Decode;
    case BD2_RN_GSA://�����û��豸����ģʽ��
        return RN_GSA_Decode;
    case BD2_RN_GST://����α�����ͳ������
        return RN_GST_Decode;
    case BD2_RN_GSV://���������Ϣ
        return RN_GSV_Decode;
    case BD2_RN_ICM://����û��豸�мӽ���ģ����Կ��Ч����Ϣ
        return RN_ICM_Decode;
    case BD2_RN_IHI://�����û��豸����ٶȼ��ٶȵ���Ϣ
        return RN_IHI_Decode;
    case BD2_RN_IHO://�������������Ϣ
        return RN_IHO_Decode;
    case BD2_RN_LPM://�����豸������ʡ��ģʽ
        return RN_LPM_Decode;
    case BD2_RN_MSS://�����豸��ǰ��λ��ʽ
        return RN_MSS_Decode;
    case BD2_RN_PMU://���PRM��ʱЧ������Ϣ
        return RN_PMU_Decode;
    case BD2_RN_RMC://�����򵼺���������
        return RN_RMC_Decode;
    case BD2_RN_TXT://���ڴ�����ı�
        return RN_TXT_Decode;
    case BD2_RN_VTG://����������͵���
    case BD2_RN_ZBS://��������ϵ����
        return RN_ZBS_Decode;
    case BD2_RN_ZDA://����UTCʱ�䡢���ںͱ���ʱ��
        return RN_ZDA_Decode;
    case BD2_RN_ZTI://����豸��ǰ����״̬��Ϣ
        return RN_ZTI_Decode;
    default:
        return NULL;
    }
}

#ifdef WIN32
/*
*�������ƣ�BD2_RN_GetPackage
*����˵����������Ϣ���ͻ�ȡ��Ӧ�����ݽṹ��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�޴��������ݽṹ����������ʾ���ݽṹ��ָ�룬�ṹ���ݰ�ʹ�����Ӧ���ͷſռ�
*/
void* BD2_RN_GetPackage(int nType)
{
    switch (nType)
    {
    case BD2_RN_AAM://��·�㵽�ﱨ��
    case BD2_RN_ALM://������������
    case BD2_RN_APL://����Ա�������
    case BD2_RN_BSD://������ʶ��
    case BD2_RN_BSQ://������ʶ��
    case BD2_RN_BSX://������ʶ��
    case BD2_RN_COM://�����û��豸����
    case BD2_RN_DHV://�ٶ��ർ����Ϣ
    case BD2_RN_GBS://����GNSS���ǹ��ϼ��
    case BD2_RN_GGA://������λ����
    case BD2_RN_GLL://������궨λ��Ϣ
    case BD2_RN_GLS://�����û��豸λ�õȳ�ʼ����Ϣ
    case BD2_RN_GSA://�����û��豸����ģʽ��
    case BD2_RN_GST://����α�����ͳ������
    case BD2_RN_GSV://���������Ϣ
    case BD2_RN_ICM://����û��豸�мӽ���ģ����Կ��Ч����Ϣ
    case BD2_RN_IHI://�����û��豸����ٶȼ��ٶȵ���Ϣ
    case BD2_RN_IHO://�������������Ϣ
    case BD2_RN_LPM://�����豸������ʡ��ģʽ
    case BD2_RN_MSS://�����豸��ǰ��λ��ʽ
    case BD2_RN_PMU://���PRM��ʱЧ������Ϣ
    case BD2_RN_RMC://�����򵼺���������
    case BD2_RN_TXT://���ڴ�����ı�
    case BD2_RN_VTG://����������͵���
    case BD2_RN_ZBS://��������ϵ����
    case BD2_RN_ZDA://����UTCʱ�䡢���ںͱ���ʱ��
    case BD2_RN_ZTI://����豸��ǰ����״̬��Ϣ
    default:
        return NULL;
    }
}

#endif //WIN32
//--------------------------------------------------------------------------
//Э��ջ����뺯��
//--------------------------------------------------------------------------

int RN_COM_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
    TRNCOM *pCom = (TRNCOM *)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    int i = 0;
    
    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//û�пɽ���������
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 6)//���ݳ��ȹ���
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
    if(pkg.nItems < 2)//û�пɽ���������
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 13)//���ݳ��ȹ���
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
     if(pkg.nItems < 2)//û�пɽ���������
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 10)//���ݳ��ȹ���
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
     if(pkg.nItems < 2)//û�пɽ���������
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 17)//���ݳ��ȹ���
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
     if(pkg.nItems < 2)//û�пɽ���������
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 9)//���ݳ��ȹ���
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
     if(pkg.nItems < 2)//û�пɽ���������
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 13)//���ݳ��ȹ���
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
     if(pkg.nItems < 2)//û�пɽ���������
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 20)//���ݳ��ȹ���
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
     if(pkg.nItems < 2)//û�пɽ���������
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 21)//���ݳ��ȹ���
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
     if(pkg.nItems < 2)//û�пɽ���������
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 4)//���ݳ��ȹ���
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
     if(pkg.nItems < 2)//û�пɽ���������
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 9)//���ݳ��ȹ���
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
     if(pkg.nItems < 2)//û�пɽ���������
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 16)//���ݳ��ȹ���
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
     if(pkg.nItems < 2)//û�пɽ���������
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 3)//���ݳ��ȹ���
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
     if(pkg.nItems < 2)//û�пɽ���������
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 10)//���ݳ��ȹ���
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
     if(pkg.nItems < 2)//û�пɽ���������
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 4)//���ݳ��ȹ���
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
     if(pkg.nItems < 2)//û�пɽ���������
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 14)//���ݳ��ȹ���
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
     if(pkg.nItems < 2)//û�пɽ���������
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 6)//���ݳ��ȹ���
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
     if(pkg.nItems < 2)//û�пɽ���������
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 8)//���ݳ��ȹ���
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
     if(pkg.nItems < 2)//û�пɽ���������
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 13)//���ݳ��ȹ���
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
     if(pkg.nItems < 2)//û�пɽ���������
         return RESULT_VERIFY_ERROR;
     else if(pkg.nItems > 10)//���ݳ��ȹ���
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
