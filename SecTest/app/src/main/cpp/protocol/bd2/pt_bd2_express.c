/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name��     pt_bd2_express.c
* Abstract�� BD2 express protocol define
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

#include "pt_bd2_express.h"

#ifdef  PT_ENABLE_BD2_EXPRESS
const uchar BD2_EX_Head[][4] = { "ECS","ECT","TCS","IDV","PRD","PRO" 
                                ,"RIS","RMO","SCS","TXM","WGM"};
//--------------------------------------------------------------------------
//Э��ջ����뺯������
//--------------------------------------------------------------------------
int BD2_DecodeRMO(void *pPackage, char *lpBuff, int nDataLen);
int BD2_EncodeRMO(char *lpBuff, int nBuffLen, void *pPackage);

int BD2_DecodeTXM(void *pPackage, char *lpBuff, int nDataLen);
int BD2_EncodeTXM(char *lpBuff, int nBuffLen, void *pPackage);

int BD2_DecodeWGM(void *pPackage, char *lpBuff, int nDataLen);
int BD2_EncodeWGM(char *lpBuff, int nBuffLen, void *pPackage);
//--------------------------------------------------------------------------
//Э��ջ�ӿں�������
//--------------------------------------------------------------------------
/*
*�������ƣ�BD2_EX_DecodeHead
*����˵����������Ϣ����
*����˵����buf    �������ݻ���
*          len    �������ݳ���
*����ֵ��  -1 ��ʾ��Ч����Ϣ���ͣ�������ʾ����������
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
	        nCount = sizeof(BD2_EX_Head) / sizeof(BD2_EX_Head[0]);//�������Э�����
	        for(i = 0; i< nCount; i++)
	        {
		        if(strncmp(buf + 3, BD2_EX_Head[i], strlen(BD2_EX_Head[i])) == 0)	//Э��ͷ�ж�
		        {
			        return ( i + BD2_EX_ZERO);
		        }
	        }
        }
    }

	return -1;
}
/*
*�������ƣ�BD2_EX_EncodeHead
*����˵��������Э��ͷ
*����˵����lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          bDevice   �Ƿ��豸
*          nType     ��Ϣ����
*����ֵ��  -1 ��ʾʧ�ܣ�������ʾ�������ݳ���
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
*�������ƣ�BD2_EX_GetEncoder
*����˵����������Ϣ���ͻ�ȡ��Ӧ�ı��뺯��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�ޱ��뺯����������ʾ���뺯��ָ��
*/
FUNC_ENCODER BD2_EX_GetEncoder(int nType)
{
    switch (nType)
    {
    case BD2_EX_ECS://�������ԭʼ������Ϣ
    case BD2_EX_ECT://ԭʼ������Ϣ
    case BD2_EX_TCS://����ͨ��ǿ�Ƹ���
    case BD2_EX_IDV://���ż����Ϣ
    case BD2_EX_PRD://�����û��豸���α��۲�ֵ���ز���λ
    case BD2_EX_PRO://ԭʼα��۲�ֵ���ز���λ
    case BD2_EX_RIS://�豸��λ
		return NULL;
    case BD2_EX_RMO://�������
		return BD2_EncodeRMO;
    case BD2_EX_SCS://RDSS˫ͨ��ʱ������
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
*�������ƣ�BD2_EX_GetDecoder
*����˵����������Ϣ���ͻ�ȡ��Ӧ�Ľ��뺯��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�޽��뺯����������ʾ���뺯��ָ��
*/
FUNC_DECODER BD2_EX_GetDecoder(int nType)
{
    switch (nType)
    {
    case BD2_EX_ECS://�������ԭʼ������Ϣ
    case BD2_EX_ECT://ԭʼ������Ϣ
    case BD2_EX_TCS://����ͨ��ǿ�Ƹ���
    case BD2_EX_IDV://���ż����Ϣ
    case BD2_EX_PRD://�����û��豸���α��۲�ֵ���ز���λ
    case BD2_EX_PRO://ԭʼα��۲�ֵ���ز���λ
    case BD2_EX_RIS://�豸��λ
		return NULL;
    case BD2_EX_RMO://�������
		return BD2_DecodeRMO;
    case BD2_EX_SCS://RDSS˫ͨ��ʱ������
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
*�������ƣ�BD2_EX_GetPackage
*����˵����������Ϣ���ͻ�ȡ��Ӧ�����ݽṹ��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�޴��������ݽṹ����������ʾ���ݽṹ��ָ�룬�ṹ���ݰ�ʹ�����Ӧ���ͷſռ�
*/
void* BD2_EX_GetPackage(int nType)
{

    switch (nType)
    {
    case BD2_EX_ECS://�������ԭʼ������Ϣ
    case BD2_EX_ECT://ԭʼ������Ϣ
    case BD2_EX_TCS://����ͨ��ǿ�Ƹ���
    case BD2_EX_IDV://���ż����Ϣ
    case BD2_EX_PRD://�����û��豸���α��۲�ֵ���ز���λ
    case BD2_EX_PRO://ԭʼα��۲�ֵ���ز���λ
    case BD2_EX_RIS://�豸��λ
		return NULL;
    case BD2_EX_RMO://�������
    case BD2_EX_SCS://RDSS˫ͨ��ʱ������
    default:
        return NULL;
    }
}
#endif //WIN32
//--------------------------------------------------------------------------
//Э��ջ����뺯������
//--------------------------------------------------------------------------

int BD2_DecodeRMO(void *pPackage, char *lpBuff, int nDataLen)
{
    TPackage data;
	TBD2RMO* pkg = (TBD2RMO*)pPackage;

    DecodePackage_Ex(&data, lpBuff, nDataLen, ",*");
    if(data.nItems != 5)//û�пɽ���������
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
    if(data.nItems != 4)//û�пɽ���������
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
    if(data.nItems != 7)//û�пɽ���������
	{
        return RESULT_ERROR;
    }   
	
	pkg->nCMD = atoi(data.pItem[1]); 

	if (pkg->nCMD == 2)//��ѯ
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

	if (pkg->nCMD == 2)//��ѯ
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