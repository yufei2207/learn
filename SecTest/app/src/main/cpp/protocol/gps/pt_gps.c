/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name��     pt_gps.c
* Abstract�� gps protocol define
* 
* Version�� 1.0
* Author��  yfwang
* Date��    2012/03/19
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

//GPSЭ��ͷ����
const uchar GPS_Head[][4] = { "GGA","GSA","GSV","RMC","VTG","GLL","ZDA"};
//--------------------------------------------------------------------------
//Э��ջ����뺯������
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
//Э��ջ��������
//--------------------------------------------------------------------------
/*
*�������ƣ�GPS_DecodeHead
*����˵����������Ϣ����
*����˵����buf    �������ݻ���
*          len    �������ݳ���
*����ֵ��  -1 ��ʾ��Ч����Ϣ���ͣ�������ʾ����������
*/
int GPS_DecodeHead(const uchar* buf, int len)
{
	int nCount;
	int i;

    if (len >= 6 && (buf[0] == '$' && buf[1] == 'G' && buf[2] == 'P'))
    {
	    nCount = sizeof(GPS_Head) / sizeof(GPS_Head[0]);//�������Э�����
	    for(i = 0; i< nCount; i++)
	    {
		    if(strncmp(buf+3, GPS_Head[i], strlen(GPS_Head[i])) == 0)	//Э��ͷ�ж�
		    {
			    return ( i + GPS_ZERO);
		    }
	    }
    }

	return -1;
}
/*
*�������ƣ�GPS_EncodeHead
*����˵��������Э��ͷ
*����˵����lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          bDevice   �Ƿ��豸
*          nType     ��Ϣ����
*����ֵ��  -1 ��ʾʧ�ܣ�������ʾ�������ݳ���
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
//Э��ջ����뺯��
//--------------------------------------------------------------------------
/*
*�������ƣ�GPS_GetEncoder
*����˵����������Ϣ���ͻ�ȡ��Ӧ�ı��뺯��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�ޱ��뺯����������ʾ���뺯��ָ��
*/
FUNC_ENCODER GPS_GetEncoder(int nType)
{
    switch (nType)
    {
    case GPS_GGA://ȫ��λ����
        return NULL;
    case GPS_GSA://����PRN����
        return NULL;
    case GPS_GSV://����״̬��Ϣ
        return NULL;
    case GPS_RMC://���䶨λ����
        return NULL;
    case GPS_VTG://�����ٶ���Ϣ
        return NULL;
    case GPS_GLL://�����������
        return NULL;
    case GPS_ZDA://UTCʱ�������
        return NULL;
    default:
        return NULL;
    }
}
/*
*�������ƣ�GPS_GetDecoder
*����˵����������Ϣ���ͻ�ȡ��Ӧ�Ľ��뺯��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�޽��뺯����������ʾ���뺯��ָ��
*/
FUNC_DECODER GPS_GetDecoder(int nType)
{
    switch (nType)
    {
    case GPS_GGA://ȫ��λ����
        return NULL;
    case GPS_GSA://����PRN����
        return NULL;
    case GPS_GSV://����״̬��Ϣ
        return NULL;
    case GPS_RMC://���䶨λ����
        return NULL;
    case GPS_VTG://�����ٶ���Ϣ
        return NULL;
    case GPS_GLL://�����������
        return NULL;
    case GPS_ZDA://UTCʱ�������
        return NULL;
    default:
        return NULL;
    }
}

#ifdef WIN32
/*
*�������ƣ�GPS_GetPackage
*����˵����������Ϣ���ͻ�ȡ��Ӧ�����ݽṹ��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�޴��������ݽṹ����������ʾ���ݽṹ��ָ�룬�ṹ���ݰ�ʹ�����Ӧ���ͷſռ�
*/
void* GPS_GetPackage(int nType)
{
    switch (nType)
    {
    case GPS_GGA://ȫ��λ����
    case GPS_GSA://����PRN����
    case GPS_GSV://����״̬��Ϣ
    case GPS_RMC://���䶨λ����
    case GPS_VTG://�����ٶ���Ϣ
    case GPS_GLL://�����������
    case GPS_ZDA://UTCʱ�������
    default:
        return NULL;
    }
}
#endif //WIN32

//--------------------------------------------------------------------------
//Э��ջ����뺯��
//--------------------------------------------------------------------------
int GPS_GGA_Decode(void *pPackage, char *lpBuff, int nDataLen)
{
    TGPSGGA *pGga = (TGPSGGA *)pPackage;
    TPackage pkg;
    char *ptr = lpBuff;
    char *pdt = NULL;
    
    DecodePackage_Ex(&pkg, ptr, nDataLen, ",*");
    if(pkg.nItems < 2)//û�пɽ���������
        return RESULT_VERIFY_ERROR;
    else if(pkg.nItems > 14)//���ݳ��ȹ���
        return RESULT_VERIFY_ERROR; 

    return RESULT_SUCCESS;    
}


#endif //end PT_ENABLE_GPS define