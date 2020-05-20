/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name��     pt_bd1.c
* Abstract�� BD1 protocol define
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
#include "pt_bd1.h"

#ifdef PT_ENABLE_BD1

//һ��Э��ͷ����
const uchar BD1_Head[][6]={ "$GLJC","$DWSQ","$TXSQ","$HXSZ","$DHSQ","$DSSQ" //�������豸
                           ,"$SCSC","$ZBZH","$JSZL","$GBXX","$GYXX","$WMCS"
                           ,"$CKSC","$ICJC","$JJZH","$BSSZ","$GXZX","$GXDQ"
                           ,"$XTZJ","$LZSZ","$LZDQ","$SJSC","$BBDQ","$XHDQ"
                           ,"$GLZK","$DWXX","$TXXX","$TXHZ","$KLSB","$DHSC" //�豸������
                           ,"$DSJG","$SCSJ","$ZBSC","$GBSC","$GYSC","$ICXX"
                           ,"$ZHQR","$ZJXX","$LZXX","$SJXX","$BBXX","$XHXX"
                           ,"$GLXX","$FKXX","$ILXX","$QLXX"
						   ,"$RZCW","$BJLZ","$DQGK","$BHXX","$SSXX","$KZXX" //�Զ�����Ϣ
                           ,"$DSXX","$SZXX","$DSLX","$BJWZ","$FPGA","$BDSZ"};
//--------------------------------------------------------------------------
//Э��ջ����뺯������
//--------------------------------------------------------------------------
//�������û���
int BD1_NULL_Decode(void *pPackage, uchar *lpBuff, int nBuffLen);
int BD1_NULL_Encode(uchar *lpBuff, int nBuffLen, void *pPackage);

int BD1_GLJC_Decode(void *pPackage, uchar *lpBuff, int nBuffLen);
int BD1_GLJC_Encode(uchar *lpBuff, int nBuffLen, void *pPackage);

int BD1_DWSQ_Decode(void *pPackage, uchar *lpBuff, int nBuffLen);
int BD1_DWSQ_Encode(uchar *lpBuff, int nBuffLen, void *pPackage);

int BD1_TXSQ_Decode(void *pPackage, uchar *lpBuff, int nBuffLen);
int BD1_TXSQ_Encode(uchar *lpBuff, int nBuffLen, void *pPackage);

int BD1_ICJC_Decode(void *pPackage, uchar *lpBuff, int nBuffLen);
int BD1_ICJC_Encode(uchar *lpBuff, int nBuffLen, void *pPackage);

int BD1_XTZJ_Decode(void *pPackage, uchar *lpBuff, int nBuffLen);
int BD1_XTZJ_Encode(uchar *lpBuff, int nBuffLen, void *pPackage);

int BD1_GXZX_Decode(void *pPackage, uchar *lpBuff, int nBuffLen);
int BD1_GXZX_Encode(uchar *lpBuff, int nBuffLen, void *pPackage);

int BD1_JSZL_Decode(void *pPackage, uchar *lpBuff, int nBuffLen);
int BD1_JSZL_Encode(uchar *lpBuff, int nBuffLen, void *pPackage);

int BD1_BSSZ_Decode(void *pPackage, uchar *lpBuff, int nBuffLen);
int BD1_BSSZ_Encode(uchar *lpBuff, int nBuffLen, void *pPackage);
//�û���������
int BD1_GLZK_Decode(void *pPackage, uchar *lpBuff, int nBuffLen);
int BD1_GLZK_Encode(uchar *lpBuff, int nBuffLen, void *pPackage);

int BD1_DWXX_Decode(void *pPackage, uchar *lpBuff, int nBuffLen);
int BD1_DWXX_Encode(uchar *lpBuff, int nBuffLen, void *pPackage);

int BD1_TXXX_Decode(void *pPackage, uchar *lpBuff, int nBuffLen);
int BD1_TXXX_Encode(uchar *lpBuff, int nBuffLen, void *pPackage);

int BD1_TXHZ_Decode(void *pPackage, uchar *lpBuff, int nBuffLen);
int BD1_TXHZ_Encode(uchar *lpBuff, int nBuffLen, void *pPackage);

int BD1_ICXX_Decode(void *pPackage, uchar *lpBuff, int nBuffLen);
int BD1_ICXX_Encode(uchar *lpBuff, int nBuffLen, void *pPackage);

int BD1_FKXX_Decode(void *pPackage, uchar *lpBuff, int nBuffLen);
int BD1_FKXX_Encode(uchar *lpBuff, int nBuffLen, void *pPackage);

int BD1_ZJXX_Decode(void *pPackage, uchar *lpBuff, int nBuffLen);
int BD1_ZJXX_Encode(uchar *lpBuff, int nBuffLen, void *pPackage);

int BD1_XHXX_Decode(void *pPackage, uchar *lpBuff, int nBuffLen);



//--------------------------------------------------------------------------
//Э��ջ��������
//--------------------------------------------------------------------------
/*
*�������ƣ�BD1_DecodeHead
*����˵����һ��������Ϣ����
*����˵����buf    �������ݻ���
*          len    �������ݳ���
*����ֵ��  -1 ��ʾ��Ч����Ϣ���ͣ�������ʾ����������
*/
int BD1_DecodeHead(const uchar* buf, int len)
{
	int nCount;
	int i;

    if (len >= 5)
    {//����Ƿ�һ��Э��ͷ
	    nCount = sizeof(BD1_Head) / sizeof(BD1_Head[0]);//����һ��Э�����
	    for(i = 0; i < nCount; i++)
	    {
		    if(strncmp(buf,BD1_Head[i],strlen(BD1_Head[i])) == 0)	//Э��ͷ�ж�
		    {
			    return ( i + BD1_ZERO);
		    }
	    }
    }

	return -1;
}
/*
*�������ƣ�BD1_EncodeHead
*����˵����һ������Э��ͷ
*����˵����lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          bDevice   �Ƿ��豸
*          nType     ��Ϣ����
*����ֵ��  -1 ��ʾʧ�ܣ�������ʾ�������ݳ���
*/
int BD1_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType)
{
    uchar *pHead = NULL;
    int nLen = 0;

    if (nBuffLen < 5)
        return -1;

    if (BD1_ISTYPE(nType))
    {
        pHead = (uchar *)&BD1_Head[nType - BD1_ZERO];
        nLen = strlen(pHead);
        if (nBuffLen >= nLen)
        {
            strcpy(lpBuff, pHead);
            return nLen;
        }
    }

    return -1;
}
/*
*�������ƣ�BD1_GetEncoder
*����˵����������Ϣ���ͻ�ȡ��Ӧ�ı��뺯��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�ޱ��뺯����������ʾ���뺯��ָ��
*/
FUNC_ENCODER BD1_GetEncoder(int nType)
{
    switch (nType)
    {
//һ��Э��
    case BD1_GLJC://���ʼ��
        return BD1_GLJC_Encode;
    case BD1_DWSQ://��λ����
        return BD1_DWSQ_Encode;
    case BD1_TXSQ://ͨ������
        return BD1_TXSQ_Encode;
    case BD1_HXSZ://��������
    case BD1_DHSQ://��������
    case BD1_DSSQ://��ʱ����
    case BD1_SCSC://ʱ�����
    case BD1_ZBZH://����ת��
		return NULL;
    case BD1_JSZL://����ָ��
		return BD1_JSZL_Encode;
    case BD1_GBXX://�㲥��Ϣ
		return BD1_NULL_Encode;
    case BD1_GYXX://������Ϣ
		return BD1_NULL_Encode;
    case BD1_WMCS://�������
    case BD1_CKSC://�������
        return NULL;
    case BD1_ICJC://IC���
        return BD1_ICJC_Encode;
    case BD1_JJZH://�����Ի�
		return NULL;
    case BD1_BSSZ://��������
		return BD1_BSSZ_Encode;
    case BD1_GXZX://����ע��
		return BD1_GXZX_Encode;
    case BD1_GXDQ://���Ŷ�ȡ
        return BD1_NULL_Encode;
    case BD1_XTZJ://ϵͳ�Լ� 
        return BD1_XTZJ_Encode;
     case BD1_LZSZ://��ֵ����
		return NULL;
    case BD1_LZDQ://��ֵ��ȡ
		return BD1_NULL_Encode;
    case BD1_SJSC://ʱ�����
		return NULL;
    case BD1_BBDQ://�汾��ȡ
    case BD1_XHDQ://��Ŷ�ȡ
        return BD1_NULL_Encode;
    case BD1_GLZK://����״��
        return BD1_GLZK_Encode;
    case BD1_DWXX://��λ��Ϣ
        return BD1_DWXX_Encode;
    case BD1_TXXX://ͨ����Ϣ
        return BD1_TXXX_Encode;
    case BD1_TXHZ://ͨ�Ż�ִ
		return BD1_TXHZ_Encode;
    case BD1_KLSB://����ʶ��
    case BD1_DHSC://�������
    case BD1_DSJG://��ʱ���
    case BD1_SCSJ://ʱ������
    case BD1_ZBSC://�������
    case BD1_GBSC://�㲥���
    case BD1_GYSC://�������
        return NULL;
    case BD1_ICXX://IC��Ϣ
        return BD1_ICXX_Encode;
    case BD1_ZHQR://�Ի�ȷ��
        return NULL;
    case BD1_ZJXX://�Լ���Ϣ
        return BD1_ZJXX_Encode;
    case BD1_LZXX://��ֵ��Ϣ
    case BD1_SJXX://ʱ����Ϣ
    case BD1_BBXX://�汾��Ϣ
		return NULL;
    case BD1_XHXX://�����Ϣ
		return NULL;
    case BD1_GLXX://������Ϣ
		return BD1_GXZX_Encode;
    case BD1_FKXX://������Ϣ
        return BD1_FKXX_Encode;
    case BD1_ILXX://I·��Ϣ
    case BD1_QLXX://Q·��Ϣ
    case BD1_RZCW://һ���Զ���Э��
    case BD1_BJLZ://
    case BD1_DQGK://��ȡ����
    case BD1_BHXX://������Ϣ
    case BD1_SSXX://��ʱ��Ϣ
    case BD1_KZXX://
    case BD1_DSXX://      
    case BD1_SZXX://
    case BD1_DSLX://
    case BD1_BJWZ://
    case BD1_FPGA://
    case BD1_BDSZ://����������Ϣ
    default:
        return NULL;
    }
}
/*
*�������ƣ�BD1_GetDecoder
*����˵����������Ϣ���ͻ�ȡ��Ӧ�Ľ��뺯��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�޽��뺯����������ʾ���뺯��ָ��
*/
FUNC_DECODER BD1_GetDecoder(int nType)
{
    switch (nType)
    {
//һ��Э��
    case BD1_GLJC://���ʼ��
        return BD1_GLJC_Decode;
    case BD1_DWSQ://��λ����
        return BD1_DWSQ_Decode;
    case BD1_TXSQ://ͨ������
        return BD1_TXSQ_Decode;
    case BD1_HXSZ://��������
    case BD1_DHSQ://��������
    case BD1_DSSQ://��ʱ����
    case BD1_SCSC://ʱ�����
    case BD1_ZBZH://����ת��
		return NULL;
    case BD1_JSZL://����ָ��
		return BD1_JSZL_Decode;
    case BD1_GBXX://�㲥��Ϣ
    case BD1_GYXX://������Ϣ
		return BD1_NULL_Decode;
    case BD1_WMCS://�������
    case BD1_CKSC://�������
        return NULL;
    case BD1_ICJC://IC���
        return BD1_ICJC_Decode;
    case BD1_JJZH://�����Ի�
		return NULL;
    case BD1_BSSZ://��������
		return BD1_BSSZ_Decode;
    case BD1_GXZX://����ע��
		return BD1_GXZX_Decode;
    case BD1_GXDQ://���Ŷ�ȡ
        return BD1_NULL_Decode;
    case BD1_XTZJ://ϵͳ�Լ� 
        return BD1_XTZJ_Decode;
    case BD1_LZSZ://��ֵ����
		return NULL;
    case BD1_LZDQ://��ֵ��ȡ
		return BD1_NULL_Decode;
    case BD1_SJSC://ʱ�����
		return NULL;
    case BD1_BBDQ://�汾��ȡ
    case BD1_XHDQ://��Ŷ�ȡ
        return BD1_NULL_Decode;
    case BD1_GLZK://����״��
        return BD1_GLZK_Decode;
    case BD1_DWXX://��λ��Ϣ
        return BD1_DWXX_Decode;
    case BD1_TXXX://ͨ����Ϣ
        return BD1_TXXX_Decode;
    case BD1_TXHZ://ͨ�Ż�ִ
		return BD1_TXHZ_Decode;
    case BD1_KLSB://����ʶ��
    case BD1_DHSC://�������
    case BD1_DSJG://��ʱ���
    case BD1_SCSJ://ʱ������
    case BD1_ZBSC://�������
    case BD1_GBSC://�㲥���
    case BD1_GYSC://�������
        return NULL;
    case BD1_ICXX://IC��Ϣ
        return BD1_ICXX_Decode;
    case BD1_ZHQR://�Ի�ȷ��
        return NULL;
    case BD1_ZJXX://�Լ���Ϣ
        return BD1_ZJXX_Decode;
    case BD1_LZXX://��ֵ��Ϣ
    case BD1_SJXX://ʱ����Ϣ
    case BD1_BBXX://�汾��Ϣ
		return NULL;
    case BD1_XHXX://�����Ϣ
		return BD1_XHXX_Decode;
    case BD1_GLXX://������Ϣ
        return BD1_GXZX_Decode;
    case BD1_FKXX://������Ϣ
        return BD1_FKXX_Decode;
    case BD1_ILXX://I·��Ϣ
    case BD1_QLXX://Q·��Ϣ
    case BD1_RZCW://
    case BD1_BJLZ://
    case BD1_DQGK://��ȡ����
    case BD1_BHXX://������Ϣ
    case BD1_SSXX://��ʱ��Ϣ
    case BD1_KZXX://
    case BD1_DSXX://      
    case BD1_SZXX://
    case BD1_DSLX://
    case BD1_BJWZ://
    case BD1_FPGA://
    case BD1_BDSZ://����������Ϣ
		return NULL;
    default:
        return NULL;
    }
}
/*
*�������ƣ�BD1_RecvData
*����˵����������յ�������Э�����ݽṹ
*����˵����pParam Э�����ջָ��
*          data     ���յ�����
*����ֵ��  -1 ��ʾû���յ���֡���ݣ�������ʾ������֡���ݵ���Ϣ����
*/
int  BD1_RecvData(TStackParam *pParam, uchar data)
{
    int nRet = RESULT_ERROR;
    if (pParam->nRecvLen == 0)
    {
        if (data == '$')//�յ�֡ͷ
        {
            pParam->RecvBuff[0] = data;
            
            pParam->nRecvLen = 1;//��ʼ���ݽ���
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

        if (pParam->nRecvLen == 5)
        {
            pParam->nMsgType = BD1_DecodeHead(pParam->RecvBuff, pParam->nRecvLen);
            if (pParam->nMsgType == INVALID_TYPE)
            {
                pParam->nRecvLen = 0;
            }
        }
        else if (pParam->nRecvLen > 6)
        {
            nRet = BD1_RecvDataEx(pParam, data);
            if (nRet == RESULT_ERROR)//
            {
                pParam->nRecvLen = 0;
            }
            else if (nRet > 0) //�յ�����֡
            {
                pParam->nRecvLen = 0;
                return nRet;
            }
        }
    }

    return RESULT_ERROR;
}
/*
*�������ƣ�BD1_RecvData
*����˵����������յ�������Э�����ݽṹ
*����˵����pParam   ջ
*          data     ���յ�����
*����ֵ��  -1 ��ʾû���յ���֡���ݣ�������ʾ������֡���ݵ���Ϣ����
*/
int  BD1_RecvDataEx(TStackParam *pParam, uchar data)
{
    if (pParam->nRecvLen == 7)//һ����֡��
    {
        PT_Decode_SHORT(&(pParam->nFrameLen), &(pParam->RecvBuff[5]), 2);
        if (pParam->nFrameLen <= 7 || pParam->nFrameLen > pParam->nRecvBuffLen)//֡����Ч
        { //������֡����
            return RESULT_ERROR;
        }
    }
    else if (pParam->nFrameLen > 7)//��֡�������
    {
        if (pParam->nRecvLen == pParam->nFrameLen)//������֡����
        {
             pParam->nCRCValue ^= PT_MakeCRC(pParam->RecvBuff, 7);

             if ((pParam->nCRCValue & 0xFF) == data)//У����ȷ
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
*�������ƣ�BD1_EncodePackage
*����˵������������
*����˵����lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          pMsgData  Э�����ݽṹָ��
*          bDevice   no use
*����ֵ��  -1 ��ʾʧ�ܣ�������ʾ���ݳ���
*/
int BD1_EncodePackage(uchar *lpBuff, int nBuffLen, 
                       void *pPackage, int nMsgType, char bDevice)
{
    int nRet = RESULT_ERROR;
    int nHeadLen = 0;
    FUNC_ENCODER fnEncoder = NULL;
    uchar *ptr = lpBuff;

    fnEncoder = BD1_GetEncoder(nMsgType);
    if (fnEncoder == NULL)
        return RESULT_ERROR;

//encoe head
    nRet = BD1_EncodeHead(ptr, nBuffLen, bDevice, nMsgType);
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
    //length
    ptr += 2; 
    nBuffLen -= 2;
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

    if (nBuffLen < 1)
    {
        return RESULT_ERROR;
    }
    //length
    nRet = PT_Encode_SHORT(&lpBuff[nHeadLen], nBuffLen, (unsigned short)(ptr - lpBuff + 1));
    //CRC
    *ptr = PT_MakeCRC(lpBuff, ptr - lpBuff);//У�����֡ͷ'$'
    ptr++;

    return (ptr - lpBuff);
}

/*
*�������ƣ�BD1_DecodePackage
*����˵�������������֡������Э�����ݽṹ
*����˵����pPackage  ���ݽṹָ��
*          lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          nMsgType  Э������
*����ֵ��  0 ��ʾ����ɹ���������ʾʧ��
*/
int BD1_DecodePackage(void *pPackage, int nMsgType, uchar *lpBuff, int nBuffLen)
{
    unsigned char nCRC;
    int nFrameLen = 0;

    FUNC_DECODER fnDecoder = NULL;

    fnDecoder = BD1_GetDecoder(nMsgType);
    if (fnDecoder == NULL)
        return RESULT_ERROR;

    if (nBuffLen > 6)
    {
        PT_Decode_SHORT((short*)&nFrameLen, &lpBuff[5], 2);
    }

    if (nFrameLen > 6 && nFrameLen <= nBuffLen)
    {
        nCRC = PT_MakeCRC(lpBuff, nFrameLen - 1);
        if (nCRC != lpBuff[nFrameLen - 1])//У�鲻��ȷ
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
*�������ƣ�BD1_GetPackage
*����˵����������Ϣ���ͻ�ȡ��Ӧ�����ݽṹ��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�޴��������ݽṹ����������ʾ���ݽṹ��ָ�룬�ṹ���ݰ�ʹ�����Ӧ���ͷſռ�
*/
void *BD1_GetPackage(int nType)
{
    switch (nType)
    {
    case BD1_GLJC://���ʼ��
        return malloc(sizeof(TBD1GLJC));
    case BD1_DWSQ://��λ����
        return malloc(sizeof(TBD1DWSQ));
    case BD1_TXSQ://ͨ������
        return malloc(sizeof(TBD1TXSQ));
    case BD1_HXSZ://��������
    case BD1_DHSQ://��������
    case BD1_DSSQ://��ʱ����
    case BD1_SCSC://ʱ�����
    case BD1_ZBZH://����ת��
    case BD1_JSZL://����ָ��
    case BD1_GBXX://�㲥��Ϣ
    case BD1_GYXX://������Ϣ
    case BD1_WMCS://�������
    case BD1_CKSC://�������
        return NULL;
    case BD1_ICJC://IC���
        return malloc(sizeof(TBD1ICJC));
    case BD1_JJZH://�����Ի�
    case BD1_BSSZ://��������
    case BD1_GXZX://����ע��
    case BD1_GXDQ://���Ŷ�ȡ
        return NULL;
    case BD1_XTZJ://ϵͳ�Լ� 
        return malloc(sizeof(TBD1XTZJ));
    case BD1_LZSZ://��ֵ����
    case BD1_LZDQ://��ֵ��ȡ
    case BD1_SJSC://ʱ�����
    case BD1_BBDQ://�汾��ȡ
    case BD1_XHDQ://��Ŷ�ȡ
        return NULL;
    case BD1_GLZK://����״��
        return malloc(sizeof(TBD1GLZK));
    case BD1_DWXX://��λ��Ϣ
        return malloc(sizeof(TBD1DWXX));
    case BD1_TXXX://ͨ����Ϣ
        return malloc(sizeof(TBD1TXXX));
    case BD1_TXHZ://ͨ�Ż�ִ
    case BD1_KLSB://����ʶ��
    case BD1_DHSC://�������
    case BD1_DSJG://��ʱ���
    case BD1_SCSJ://ʱ������
    case BD1_ZBSC://�������
    case BD1_GBSC://�㲥���
    case BD1_GYSC://�������
        return NULL;
    case BD1_ICXX://IC��Ϣ
        return malloc(sizeof(TBD1ICXX));
    case BD1_ZHQR://�Ի�ȷ��
        return NULL;
    case BD1_ZJXX://�Լ���Ϣ
        return malloc(sizeof(TBD1ZJXX));
    case BD1_LZXX://��ֵ��Ϣ
    case BD1_SJXX://ʱ����Ϣ
    case BD1_BBXX://�汾��Ϣ
    case BD1_XHXX://�����Ϣ
    case BD1_GLXX://������Ϣ
        return NULL;
    case BD1_FKXX://������Ϣ
        return malloc(sizeof(TBD1FKXX));
    case BD1_ILXX://I·��Ϣ
    case BD1_QLXX://Q·��Ϣ
    case BD1_RZCW://
    case BD1_BJLZ://
    case BD1_DQGK://��ȡ����
    case BD1_BHXX://������Ϣ
    case BD1_SSXX://��ʱ��Ϣ
    case BD1_KZXX://
    case BD1_DSXX://      
    case BD1_SZXX://
    case BD1_DSLX://
    case BD1_BJWZ://
    case BD1_FPGA://
    case BD1_BDSZ://����������Ϣ
		return NULL;
    default:
        return NULL;
    }
}
#endif //WIN32
//--------------------------------------------------------------------------
//Э��ջ����뺯��ʵ��
//--------------------------------------------------------------------------
int BD1_NULL_Decode(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet = 0;
    uchar *ptr = lpBuff + 7;
    TBD1NULL *pkg = (TBD1NULL *)pPackage;

    nRet = PT_Decode_INT24(&(pkg->nUserAddr), ptr, nBuffLen);

    nBuffLen -= nRet;
    ptr += nRet;
	
	return (ptr - lpBuff);	
}

int BD1_NULL_Encode(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet = 0;
    uchar *ptr = lpBuff;
    TBD1NULL *pkg = (TBD1NULL *)pPackage;

    nRet = PT_Encode_INT24(ptr, nBuffLen, pkg->nUserAddr);
    nBuffLen -= nRet;
    ptr += nRet;


    return (ptr - lpBuff);	
}

int BD1_GLJC_Decode(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet = 0;
    uchar *ptr = lpBuff + 7;
    TBD1GLJC *pGlJC = (TBD1GLJC *)pPackage;

    nRet = PT_Decode_INT24(&(pGlJC->nUserAddr), ptr, nBuffLen);

    nBuffLen -= nRet;
    ptr += nRet;

    pGlJC->nFrequency = *ptr++;
    nBuffLen -= 1;

    return (ptr - lpBuff);
}

int BD1_GLJC_Encode(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = 0;
    uchar *ptr = lpBuff;
    TBD1GLJC *pGlJC = (TBD1GLJC *)pPackage;

    nRet = PT_Encode_INT24(ptr, nBuffLen, pGlJC->nUserAddr);

    ptr += nRet;
    nBuffLen -= nRet;

    *ptr++ = pGlJC->nFrequency;
    nBuffLen -= 1;

    return (ptr - lpBuff);
}

int BD1_DWSQ_Decode(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet = 0;
    uchar *ptr = lpBuff + 7;
    TBD1DWSQ *pDWSQ = (TBD1DWSQ *)pPackage;

    nRet = PT_Decode_INT24(&pDWSQ->nUserAddr, ptr, nBuffLen);

    ptr += nRet;
    nBuffLen -= nRet;
    //��Ϣ���
    pDWSQ->bEmergent = (*ptr >> 5) & 0x01;				///////6->5
    pDWSQ->nHeightTestMode = (*ptr >> 2) & 0x03;
    pDWSQ->nHeightFlag = (*ptr & 0x01);

    ptr++;
    nBuffLen--;
    //�߳����ݺ����߸�    
    nRet = PT_Decode_SHORT(&pDWSQ->nHeight, ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Decode_SHORT(&pDWSQ->nAntennaHeight, ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;
    //��ѹ����
    pDWSQ->nAirPressure = *ptr++;
    pDWSQ->nAirPressure <<= 12;

    pDWSQ->nAirPressure |= (*ptr++ << 4);
    pDWSQ->nAirPressure |= ((*ptr >> 4) & 0x0F);

    pDWSQ->nTemperature = ((*ptr) & 0x07);
    pDWSQ->nTemperature <<= 8;						
    pDWSQ->nTemperature |= *(ptr + 1);

    
    if (*ptr & 0x08)						
    {
        pDWSQ->nTemperature *= -1;
    }
    ptr += 2;
    nBuffLen -= 4;
    //��վƵ��
    nRet = PT_Decode_SHORT(&pDWSQ->nFrequency, ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}

int BD1_DWSQ_Encode(uchar *lpBuff, int nBuffLen, void* pPackage)
{
    int nRet = 0;
    uchar *ptr = lpBuff;
    TBD1DWSQ *pDWSQ = (TBD1DWSQ *)pPackage;

    nRet = PT_Encode_INT24(ptr, nBuffLen, pDWSQ->nUserAddr);

    ptr += nRet;
    nBuffLen -= nRet;
//��Ϣ���
    *ptr = 0x0;
    *ptr |= ((pDWSQ->bEmergent & 0x1) << 5);				//////6-->5
    *ptr |= ((pDWSQ->nHeightTestMode & 0x03) << 2);	
    *ptr |= (pDWSQ->nHeightFlag & 0x01);
    ptr++;
    nBuffLen--;
//�߳����ݺ����߸�    
    nRet = PT_Encode_SHORT(ptr, nBuffLen, pDWSQ->nHeight);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, pDWSQ->nAntennaHeight);
    ptr += nRet;
    nBuffLen -= nRet;
//��ѹ����
    *ptr++ = (pDWSQ->nAirPressure >> 12) & 0xFF;
    *ptr++ = (pDWSQ->nAirPressure >> 4) & 0xFF;
    *ptr = ((pDWSQ->nAirPressure) & 0x0F) << 4;	 	 /////(pDWSQ->nAirPressure) & 0x0F --> ((pDWSQ->nAirPressure) & 0x0F) << 4

    if (pDWSQ->nTemperature < 0)
    {
        *ptr |= (1 << 3);
        pDWSQ->nTemperature *= -1;
    }
    *ptr |= (pDWSQ->nTemperature >> 8) & 0x07;
    ptr++;						////??
    *ptr++ = (pDWSQ->nTemperature) & 0xFF;		////*ptr++ = (pDWSQ->nAirPressure) & 0xFF --> *ptr++ = (pDWSQ->nTemperature) & 0xFF
    nBuffLen -= 4;
//��վƵ��
    nRet = PT_Encode_SHORT(ptr, nBuffLen, pDWSQ->nFrequency);
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}

int BD1_TXSQ_Decode(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet = 0;
    unsigned short nLen = 0;
    uchar *pData;
    uchar *ptr = lpBuff + 7;
    TBD1TXSQ *pTXSQ = (TBD1TXSQ *)pPackage;

    nRet = PT_Decode_INT24(&pTXSQ->nUserAddr, ptr, nBuffLen);

    ptr += nRet;
    nBuffLen -= nRet;
    //��Ϣ���
    pTXSQ->nType = (*ptr >> 5) & 0x07;					
    if (pTXSQ->nType == TBDMT_SENDMESSAGE)
    {
        pTXSQ->data.msgComm.nType = (*ptr >> 2) & 0x03;
        pTXSQ->data.msgComm.nMode = (*ptr >> 1) & 0x01;
        pTXSQ->data.msgComm.bPassword = (*ptr& 0x01);
    }
    else if (pTXSQ->nType == TBDMT_QUERYMESSAGE)
    {
        pTXSQ->data.msgQuery.nType = (*ptr >> 4) & 0x01;
        pTXSQ->data.msgQuery.nMode = (*ptr >> 2) & 0x03;
    }
    else
    {
        return -1;
    }
    ptr++;
    nBuffLen--;
    //
    nRet = PT_Decode_INT24(&(pTXSQ->nPeerAddr), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    if (pTXSQ->nType == TBDMT_SENDMESSAGE)
    {
        nRet = PT_Decode_SHORT(&nLen, ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;

        pTXSQ->data.msgComm.bAnswer = *ptr++;
        nBuffLen--;

        if (pTXSQ->data.msgComm.nMode == 1)//code or mix or position
        {
            if (*ptr == 0xA4) //mix
            {
                pTXSQ->data.msgComm.nMode = 2;

                pTXSQ->data.msgComm.nDataLen = nLen / 8 - 1;

                ptr++;
            }
			else if (*ptr == 0xA0 || *ptr == 0xA1) //position
			{
				pTXSQ->data.msgComm.nMode = 3;

                pTXSQ->data.msgComm.nDataLen = nLen / 8 - 1;

                ptr++;	
			}
            else
            {
                pTXSQ->data.msgComm.nDataLen = nLen / 4;
            }
        }
        else //chinese
        {
            pTXSQ->data.msgComm.nDataLen = nLen / 8;
        }

        if (pTXSQ->data.msgComm.nDataLen > sizeof(pTXSQ->data.msgComm.szData) - 1)
        {
            return -1;
        }
        else
        {
            if (pTXSQ->data.msgComm.nMode == 1)//code
            {
                nLen /= 8;
                pData = pTXSQ->data.msgComm.szData;
                for (nRet = 0; nRet < nLen; nRet++, ptr++)
                {
                    *pData++ = ((*ptr >> 4) & 0x0F);
                    *pData++ = (*ptr & 0x0F);
                }

                if (pTXSQ->data.msgComm.nDataLen % 2)
                {
                    *pData++ = ((*ptr >> 4) & 0x0F);
                }
            }
            else//chinese or mix or position
            {
                memcpy(pTXSQ->data.msgComm.szData, ptr, pTXSQ->data.msgComm.nDataLen);
                ptr += pTXSQ->data.msgComm.nDataLen;
            }
            pTXSQ->data.msgComm.szData[pTXSQ->data.msgComm.nDataLen] = '\0';

            nBuffLen -= pTXSQ->data.msgComm.nDataLen;
        }
    }

    return (ptr - lpBuff);
}

int BD1_TXSQ_Encode(uchar *lpBuff, int nBuffLen, void* pPackage)
{
    int nRet = 0;
    unsigned short nLen = 0;
    uchar *pData;
    uchar *ptr = lpBuff;
    unsigned short nMaxBit = 1680;//��ͨ�����1680bit
    TBD1TXSQ *pTXSQ = (TBD1TXSQ *)pPackage;

    nRet = PT_Encode_INT24(ptr, nBuffLen, pTXSQ->nUserAddr);

    ptr += nRet;
    nBuffLen -= nRet;
    //��Ϣ���
    *ptr = 0x0;
    *ptr |= ((pTXSQ->nType & 0x07) << 5);

    if (pTXSQ->nType == TBDMT_SENDMESSAGE)
    {
        *ptr |= ((pTXSQ->data.msgComm.nType & 0x03) << 2);
        if (pTXSQ->data.msgComm.nMode > 0) //code or mix
        {
            *ptr |= 0x02;//((pTXSQ->msgComm.nMode & 0x01) << 1);
        }
        
        *ptr |= ((pTXSQ->data.msgComm.bPassword & 0x01));
    }
    else if (pTXSQ->nType == TBDMT_QUERYMESSAGE)
    {
        *ptr |= ((pTXSQ->data.msgQuery.nType & 0x01) << 4);
        *ptr |= ((pTXSQ->data.msgQuery.nMode & 0x03) << 2);		
    }
    else
    {
        return -1;
    }
    ptr++;
    nBuffLen--;

    //    
    nRet = PT_Encode_INT24(ptr, nBuffLen, pTXSQ->nPeerAddr);
    ptr += nRet;
    nBuffLen -= nRet;

    if (pTXSQ->nType == TBDMT_SENDMESSAGE)
    {
        //bit length
        if (pTXSQ->data.msgComm.nMode == 0)//chinese
        {
            nLen = pTXSQ->data.msgComm.nDataLen * 8;
        }
        else if (pTXSQ->data.msgComm.nMode == 1)//code
        {
            nLen = pTXSQ->data.msgComm.nDataLen * 4;
        }
        else //mix or position
        {
            nLen = (pTXSQ->data.msgComm.nDataLen + 1) * 8;
        }

        if (pTXSQ->data.msgComm.nType == 0x00)//
        {
            nMaxBit = 188;//����ͨѶ�����188bit
        }

        if (nLen <= nMaxBit)
        {
            nRet = PT_Encode_SHORT(ptr, nBuffLen, nLen);
            ptr += nRet;
            nBuffLen -= nRet;
        }
        else// > 1680 bit
        {
            return -1;
        }

        *ptr++ = pTXSQ->data.msgComm.bAnswer;
        nBuffLen--;
        //message
        if (pTXSQ->data.msgComm.nMode == 1)//code
        {
            nLen = pTXSQ->data.msgComm.nDataLen / 2;
            pData = pTXSQ->data.msgComm.szData;
            for (nRet = 0; nRet < nLen; nRet++)
            {
                *ptr = ((*pData++ & 0x0F) << 4);
                *ptr |= (*pData++ & 0x0F);
                ptr++;
                nBuffLen--;
            }
            if (pTXSQ->data.msgComm.nDataLen % 2)
            {
                *ptr = ((*pData++ & 0x0F) << 4);
                *ptr |= 0x0F;

                ptr++;
                nBuffLen--;
            }
        }
        else
        {
            if (pTXSQ->data.msgComm.nMode == 2)//mix
            {
                *ptr++ = 0xA4;//��ϱ���Լ����ʶ
                nBuffLen--;
            }
			else if (pTXSQ->data.msgComm.nMode == 3)//position
            {
                *ptr++ = 0xA0;//λ���ϱ�����Լ����ʶ
                nBuffLen--;
            }

            memcpy(ptr, pTXSQ->data.msgComm.szData, pTXSQ->data.msgComm.nDataLen);
            ptr +=  pTXSQ->data.msgComm.nDataLen;
            nBuffLen -= pTXSQ->data.msgComm.nDataLen;
        }
    }

    return (ptr - lpBuff);
}

int BD1_ICJC_Decode(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet = 0;
    uchar *ptr = lpBuff + 7;
    TBD1ICJC *pICJC = (TBD1ICJC *)pPackage;

    nRet = PT_Decode_INT24(&(pICJC->nUserAddr), ptr, nBuffLen);

    nBuffLen -= nRet;
    ptr += nRet;

    pICJC->nFrameID = *ptr++;
    nBuffLen -= 1;

    return (ptr - lpBuff);
}

int BD1_ICJC_Encode(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = 0;
    uchar *ptr = lpBuff;
    TBD1ICJC *pICJC = (TBD1ICJC *)pPackage;

    nRet = PT_Encode_INT24(ptr, nBuffLen, pICJC->nUserAddr);

    ptr += nRet;
    nBuffLen -= nRet;

    *ptr++ = pICJC->nFrameID;
    nBuffLen -= 1;

    return (ptr - lpBuff);
}

int BD1_XTZJ_Decode(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet = 0;
    uchar *ptr = lpBuff + 7;
    TBD1XTZJ *pXTZJ = (TBD1XTZJ *)pPackage;

    nRet = PT_Decode_INT24(&(pXTZJ->nUserAddr), ptr, nBuffLen);

    nBuffLen -= nRet;
    ptr += nRet;

    nRet = PT_Decode_SHORT(&(pXTZJ->nFrequency), ptr, nBuffLen);

    nBuffLen -= nRet;
    ptr += nRet;

    return (ptr - lpBuff);
}

int BD1_XTZJ_Encode(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = 0;
    uchar *ptr = lpBuff;
    TBD1XTZJ *pXTZJ = (TBD1XTZJ *)pPackage;

    nRet = PT_Encode_INT24(ptr, nBuffLen, pXTZJ->nUserAddr);

    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, pXTZJ->nFrequency);

    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}

//�û���������
int BD1_GLZK_Decode(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet = 0;
    uchar *ptr = lpBuff + 7;
    TBD1GLZK *pGLZK = (TBD1GLZK *)pPackage;

    nRet = PT_Decode_INT24(&(pGLZK->nUserAddr), ptr, nBuffLen);

    nBuffLen -= nRet;
    ptr += nRet;

    for (nRet = 0; nRet < 6; nRet++)
    {
        pGLZK->PowerState[nRet] = *ptr++;
    }
    
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}

int BD1_GLZK_Encode(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = 0;
    uchar *ptr = lpBuff;
    TBD1GLZK *pGLZK = (TBD1GLZK *)pPackage;


    nRet = PT_Encode_INT24(ptr, nBuffLen, pGLZK->nUserAddr);

    ptr += nRet;
    nBuffLen -= nRet;

    for (nRet = 0; nRet < 6; nRet++)
    {
        *ptr++ = pGLZK->PowerState[nRet];
    }
    
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}


int BD1_DWXX_Decode(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet = 0;
    uchar *ptr = lpBuff + 7;
    TBD1DWXX *pDWXX = (TBD1DWXX *)pPackage;

    nRet = PT_Decode_INT24(&(pDWXX->nUserAddr), ptr, nBuffLen);
    nBuffLen -= nRet;
    ptr += nRet;
//��Ϣ���
    pDWXX->nType = (*ptr >> 5) & 0x01;
    pDWXX->bSecretKey = (*ptr >> 4) & 0x01;
    pDWXX->nPrecision = (*ptr >> 3) & 0x01;
    pDWXX->bEmergent = (*ptr >> 2) & 0x01;
    pDWXX->bMultiValue = (*ptr >> 1) & 0x01;
    pDWXX->nHeightFlag = (*ptr & 0x01);
    nBuffLen -= 1;
    ptr += 1;
//��ѯ��ַ
    nRet = PT_Decode_INT24(&(pDWXX->nQueryAddr), ptr, nBuffLen);
    nBuffLen -= nRet;
    ptr += nRet;
//time
    pDWXX->nHour = *ptr++;
    pDWXX->nMinute = *ptr++;
    pDWXX->nSecond = *ptr++;
    pDWXX->nDotSecond = *ptr++;
    nBuffLen -= 4;

//lon
    pDWXX->lonPos.nDegree = *ptr++;
    pDWXX->lonPos.nMinute = *ptr++;
    pDWXX->lonPos.nSecond = *ptr++;
    pDWXX->lonPos.nDotSecond = *ptr++;
    nBuffLen -= 4;

//lat
    pDWXX->latPos.nDegree = *ptr++;
    pDWXX->latPos.nMinute = *ptr++;
    pDWXX->latPos.nSecond = *ptr++;
    pDWXX->latPos.nDotSecond = *ptr++;
    nBuffLen -= 4;

	pDWXX->nHeight = (*ptr) & 0x3F;
	pDWXX->nHeight <<= 8;

	pDWXX->nHeight |= *(ptr + 1);

	if (*ptr & 0x40)
	{
		pDWXX->nHeight *= -1;		
	}

	ptr += 2;
	nBuffLen -= 2;

	pDWXX->nHeightError = (*ptr) & 0x3F;
	pDWXX->nHeightError <<= 8;
	pDWXX->nHeightError |= *(ptr + 1);

	if (*ptr & 0x40)
	{
		pDWXX->nHeightError *= -1;		
	}

	ptr += 2;
	nBuffLen -= 2;

    return (ptr - lpBuff);
}

int BD1_DWXX_Encode(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = 0;
    uchar *ptr = lpBuff;
    TBD1DWXX *pDWXX = (TBD1DWXX *)pPackage;

    nRet = PT_Encode_INT24(ptr, nBuffLen, pDWXX->nUserAddr);
    nBuffLen -= nRet;
    ptr += nRet;
    //��Ϣ���
    *ptr = ((pDWXX->nType & 0x01) << 5);
    *ptr |= ((pDWXX->bSecretKey & 0x01) << 4);
    *ptr |= ((pDWXX->nPrecision & 0x01) << 3);
    *ptr |= ((pDWXX->bEmergent & 0x01) << 2);
    *ptr |= ((pDWXX->bMultiValue & 0x01) << 1);
    *ptr |= ((pDWXX->nHeightFlag & 0x01));
    nBuffLen -= 1;
    ptr += 1;
    //��ѯ��ַ
    nRet = PT_Encode_INT24(ptr, nBuffLen, pDWXX->nQueryAddr);
    nBuffLen -= nRet;
    ptr += nRet;
    //time
    *ptr++ = pDWXX->nHour;
    *ptr++ = pDWXX->nMinute;
    *ptr++ = pDWXX->nSecond;
    *ptr++ = pDWXX->nDotSecond;
    nBuffLen -= 4;
    //lon
    *ptr++ = pDWXX->lonPos.nDegree;
    *ptr++ = pDWXX->lonPos.nMinute;
    *ptr++ = pDWXX->lonPos.nSecond;
    *ptr++ = pDWXX->lonPos.nDotSecond;
    nBuffLen -= 4;
    //lat
    *ptr++ = pDWXX->latPos.nDegree;
    *ptr++ = pDWXX->latPos.nMinute;
    *ptr++ = pDWXX->latPos.nSecond;
    *ptr++ = pDWXX->latPos.nDotSecond;
    nBuffLen -= 4;

	nRet = pDWXX->nHeight;
	if (nRet < 0)
	{
		nRet *= -1;
		*ptr = 0x40;
	}
	else
	{
		*ptr = 0;
	}
	
	*ptr++ |= (nRet >> 8 & 0x3F);
	*ptr++ = (nRet & 0xFF);
	nBuffLen -= 2;

	nRet = pDWXX->nHeightError;
	if (nRet < 0)
	{
		nRet *= -1;
		*ptr = 0x40;
	}
	else
	{
		*ptr = 0;
	}
	
	*ptr++ |= (nRet >> 8 & 0x3F);
	*ptr++ = (nRet & 0xFF);
	nBuffLen -= 2;

    return (ptr - lpBuff);
}

int BD1_TXXX_Decode(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet = 0;
    unsigned short nLen = 0;
    uchar *pData;
    uchar *ptr = lpBuff + 7;
    TBD1TXXX *pTXXX = (TBD1TXXX *)pPackage;

    nRet = PT_Decode_INT24(&(pTXXX->nUserAddr), ptr, nBuffLen);
    nBuffLen -= nRet;
    ptr += nRet;
    //��Ϣ���
    pTXXX->nMode = (*ptr >> 5) & 0x01;
    pTXXX->nType = (*ptr >> 3) & 0x01;
    pTXXX->bSecretKey = (*ptr >> 2) & 0x01;
    nBuffLen -= 1;
    ptr += 1;
    //��ѯ��ַ
    nRet = PT_Decode_INT24(&(pTXXX->nPeerAddr), ptr, nBuffLen);
    nBuffLen -= nRet;
    ptr += nRet;
    //time
    pTXXX->nHour = *ptr++;
    pTXXX->nMinute = *ptr++;
    nBuffLen -= 2;

    nRet = PT_Decode_SHORT(&nLen, ptr, nBuffLen);
    nBuffLen -= nRet;
    ptr += nRet;

    if (pTXXX->nMode == 1)//code or mix
    {
        if (*ptr == 0xA4) //mix
        {
            pTXXX->nMode = 2;

            pTXXX->nDataLen = nLen / 8 - 1;//byte number

            ptr++;
        }
		else if (*ptr == 0xA0 || *ptr == 0xA1)//position
		{
			pTXXX->nMode = 3;

            pTXXX->nDataLen = nLen / 8 - 1;//byte number

            ptr++;
		}
		else if (*ptr == 0xA2)
		{
			pTXXX->nMode = 4;

            pTXXX->nDataLen = nLen / 8 - 1;//byte number

            ptr++;	
		}
        else
        {
            pTXXX->nDataLen = nLen / 4;//byte number
        }
    }
    else //chinese
    {
        pTXXX->nDataLen = nLen / 8;//byte number
    }

    if (pTXXX->nDataLen > sizeof(pTXXX->szData) - 1)
    {
        return -1;
    }
    else
    {
        if (pTXXX->nMode == 1)//code
        {
            pData = pTXXX->szData;
            nLen /= 8;
            for (nRet = 0; nRet < nLen; nRet++, ptr++)
            {
                *pData++ = ((*ptr >> 4) & 0x0F);
                *pData++ = (*ptr & 0x0F);
            }

            if (pTXXX->nDataLen % 2)
            {
                *pData++ = (((*ptr++) >> 4) & 0x0F);
            }
        }
        else//chinese or mix or position
        {
            memcpy(pTXXX->szData, ptr, pTXXX->nDataLen);
            ptr += pTXXX->nDataLen;
        }
        pTXXX->szData[pTXXX->nDataLen] = '\0';

        nBuffLen -= pTXXX->nDataLen;
    }

    pTXXX->nCRCFlag = *ptr++;
    nBuffLen -= 1;

    return (ptr - lpBuff);
}

int BD1_TXXX_Encode(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = 0;
    unsigned short nLen = 0;
    uchar *pData;
    uchar *ptr = lpBuff;
    TBD1TXXX *pTXXX = (TBD1TXXX *)pPackage;

    nRet = PT_Encode_INT24(ptr, nBuffLen, pTXXX->nUserAddr);
    nBuffLen -= nRet;
    ptr += nRet;
    //��Ϣ���
	*ptr = 0x40;
    if (pTXXX->nMode > 0)
    {
        *ptr |= (0X01 << 5);//((pTXXX->nMode & 0x01) << 5);
    }
    *ptr |= ((pTXXX->nType & 0x01) << 3);
    *ptr |= ((pTXXX->bSecretKey & 0x01) << 2);
    nBuffLen -= 1;
    ptr += 1;
    //���ŷ���ַ
    nRet = PT_Encode_INT24(ptr, nBuffLen, pTXXX->nPeerAddr);
    nBuffLen -= nRet;
    ptr += nRet;
    //time
    *ptr++ = pTXXX->nHour;
    *ptr++ = pTXXX->nMinute;
    nBuffLen -= 2;

    //bit length
    if (pTXXX->nMode == 0)//chinese
    {
        nLen = pTXXX->nDataLen * 8;
    }
    else if (pTXXX->nMode == 1)//code
    {
        nLen = pTXXX->nDataLen * 4;
    }
    else //mix or position
    {
        nLen = (pTXXX->nDataLen + 1) * 8;
    }

    if (nLen <= 1680)
    {
        nRet = PT_Encode_SHORT(ptr, nBuffLen, nLen);
        ptr += nRet;
        nBuffLen -= nRet;
    }
    else// > 1680 bit
    {
        return -1;
    }

    //message
    if (pTXXX->nMode == 1)//code
    {
        nLen = pTXXX->nDataLen / 2;
        pData = pTXXX->szData;
        for (nRet = 0; nRet < nLen; nRet++)
        {
            *ptr = ((*pData++ & 0x0F) << 4);
            *ptr |= (*pData++ & 0x0F);
            ptr++;
            nBuffLen--;
        }

        if (pTXXX->nDataLen % 2)
        {
            *ptr = ((*pData++ & 0x0F) << 4);
            *ptr |= 0x0F;
            ptr++;
        }
    }
    else
    {
        if (pTXXX->nMode == 2)//mix
        {
            *ptr++ = 0xA4;
            nBuffLen--;
        }
		else if (pTXXX->nMode == 3)//position
        {
            *ptr++ = 0xA0;
            nBuffLen--;
        }

        memcpy(ptr, pTXXX->szData, pTXXX->nDataLen);
        ptr +=  pTXXX->nDataLen;
        nBuffLen -= pTXXX->nDataLen;
    }

    *ptr++ = (pTXXX->nCRCFlag & 0x01);
    nBuffLen -= 1;

    return (ptr - lpBuff);
}

int BD1_TXHZ_Decode(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet = 0;
    int i = 0;
    uchar *ptr = lpBuff + 7;
    TBD1TXHZ *pkg = (TBD1TXHZ *)pPackage;

    nRet = PT_Decode_INT24(&(pkg->nUserAddr), ptr, nBuffLen);
    nBuffLen -= nRet;
    ptr += nRet;

	nRet = PT_Decode_INT24(&(pkg->nRecvAddr), ptr, nBuffLen);
    nBuffLen -= nRet;
    ptr += nRet;

    pkg->nCount = *ptr++;

	if (pkg->nCount > BD1_MAX_NUM_HZ)
	{
		pkg->nCount = BD1_MAX_NUM_HZ;
	}

	for (i =0 ; i < pkg->nCount; i++)
	{
		pkg->SendTime[i].nHour = *ptr++;
		pkg->SendTime[i].nMinute = *ptr++;
		pkg->HZTime[i].nHour = *ptr++;
		pkg->HZTime[i].nMinute = *ptr++;

		nBuffLen -= 4;

		if (nBuffLen < 4)
			break;
	}

	return (ptr - lpBuff);
}

int BD1_TXHZ_Encode(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet = 0;
    int i = 0;
    uchar *ptr = lpBuff;
    TBD1TXHZ *pkg = (TBD1TXHZ *)pPackage;

    nRet = PT_Encode_INT24(ptr, nBuffLen, pkg->nUserAddr);
    nBuffLen -= nRet;
    ptr += nRet;
	nBuffLen--;

	nRet = PT_Encode_INT24(ptr, nBuffLen, pkg->nRecvAddr);
    nBuffLen -= nRet;
    ptr += nRet;
	nBuffLen--;

	if (pkg->nCount > BD1_MAX_NUM_HZ)
	{
		pkg->nCount = BD1_MAX_NUM_HZ;
	}

	*ptr++ = pkg->nCount;

	for (i =0 ; i < pkg->nCount; i++)
	{
		*ptr++ = pkg->SendTime[i].nHour;
		*ptr++ = pkg->SendTime[i].nMinute;
		*ptr++ = pkg->HZTime[i].nHour;
		*ptr++ = pkg->HZTime[i].nMinute;
		nBuffLen -= 4;
	}

	return (ptr - lpBuff);
}

int BD1_ICXX_Decode(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet = 0;
    int nCount = 0;
    uchar *ptr = lpBuff + 7;
    TBD1ICXX *pICXX = (TBD1ICXX *)pPackage;

    nRet = PT_Decode_INT24(&(pICXX->nUserAddr), ptr, nBuffLen);
    nBuffLen -= nRet;
    ptr += nRet;

    pICXX->nFrameID = *ptr++;     					/////*ptr++ = pICXX->nFrameID --> pICXX->nFrameID = *ptr++;
    if (pICXX->nFrameID == 0)
    {
        nRet = PT_Decode_INT24(&(pICXX->nBroadcastID), ptr, nBuffLen);
        nBuffLen -= nRet;
        ptr += nRet;

        pICXX->nUserType = *ptr++;
        nBuffLen--;

        nRet = PT_Decode_SHORT(&(pICXX->nFrequency), ptr, nBuffLen);
        nBuffLen -= nRet;
        ptr += nRet;

        pICXX->nLevel = *ptr++;
        nBuffLen--;

        pICXX->bSecret = *ptr++;
        nBuffLen--;

        nRet = PT_Decode_SHORT(&(pICXX->nSubUserNum), ptr, nBuffLen);
        nBuffLen -= nRet;
        ptr += nRet;
    }
    else
    {
		pICXX->nSubUserNum = 0;
        for (nCount = 0; nCount < 100; nCount++)
        {
            if (nBuffLen > 3)
            {
                nRet = PT_Decode_INT24(&(pICXX->SubUserAddr[nCount]), ptr, nBuffLen);
                nBuffLen -= nRet;
                ptr += nRet;

				pICXX->nSubUserNum++;
            }
            else
            {
                break;
            }
        }
    }

    return (ptr - lpBuff);
}

int BD1_ICXX_Encode(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = 0;
    int nCount = 0;
    uchar *ptr = lpBuff;
    TBD1ICXX *pICXX = (TBD1ICXX *)pPackage;

    nRet = PT_Encode_INT24(ptr, nBuffLen, pICXX->nUserAddr);
    nBuffLen -= nRet;
    ptr += nRet;

    *ptr++ = pICXX->nFrameID;
    if (pICXX->nFrameID == 0)
    {
        nRet = PT_Encode_INT24(ptr, nBuffLen, pICXX->nBroadcastID);
        nBuffLen -= nRet;
        ptr += nRet;

        *ptr++ = pICXX->nUserType;
        nBuffLen--;

        nRet = PT_Encode_SHORT(ptr, nBuffLen, pICXX->nFrequency);
        nBuffLen -= nRet;
        ptr += nRet;

        *ptr++ = pICXX->nLevel;
        nBuffLen--;

        *ptr++ = pICXX->bSecret;
        nBuffLen--;

        nRet = PT_Encode_SHORT(ptr, nBuffLen, pICXX->nSubUserNum);
        nBuffLen -= nRet;
        ptr += nRet;
    }
    else
    {
        for (nCount = 0; nCount < 100; nCount++)
        {
            if (pICXX->SubUserAddr[nCount] > 0)
            {
                nRet = PT_Encode_INT24(ptr, nBuffLen, pICXX->SubUserAddr[nCount]);
                nBuffLen -= nRet;
                ptr += nRet;
            }
            else
            {
                break;
            }
        }
    }

    return (ptr - lpBuff);
}

int BD1_FKXX_Decode(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet = 0;
    int nCount = 0;
    uchar *ptr = lpBuff + 7;
    TBD1FKXX *pFKXX = (TBD1FKXX *)pPackage;

    nRet = PT_Decode_INT24(&(pFKXX->nUserAddr), ptr, nBuffLen);
    nBuffLen -= nRet;
    ptr += nRet;

    pFKXX->nResult = *ptr++;
    nBuffLen--;

    pFKXX->szResult[0] = *ptr++;
    pFKXX->szResult[1] = *ptr++;
    pFKXX->szResult[2] = *ptr++;
    pFKXX->szResult[3] = *ptr++;

	if (pFKXX->nResult == 4)
	{
        pFKXX->nFreq = pFKXX->szResult[3];
        
        pFKXX->nFreq <<= 8;
        pFKXX->nFreq |= pFKXX->szResult[2];

        pFKXX->nFreq <<= 8;
		pFKXX->nFreq |= pFKXX->szResult[1];
		
        pFKXX->nFreq <<= 8;
		pFKXX->nFreq |= pFKXX->szResult[0];
	}
	else
	{
		pFKXX->nFreq = 0;
	}

    nBuffLen -= 4;

    return (ptr - lpBuff);
}

int BD1_FKXX_Encode(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = 0;
    int nCount = 0;
    uchar *ptr = lpBuff;
    TBD1FKXX *pFKXX = (TBD1FKXX *)pPackage;

    nRet = PT_Encode_INT24(ptr, nBuffLen, pFKXX->nUserAddr);
    nBuffLen -= nRet;
    ptr += nRet;

    *ptr++ = pFKXX->nResult;
    nBuffLen--;

    if (pFKXX->nResult == 4)
    {
        *ptr++ = (pFKXX->nFreq) & 0xFF;
        *ptr++ = (pFKXX->nFreq >> 8) & 0xFF;
        *ptr++ = (pFKXX->nFreq >> 16) & 0xFF;
        *ptr++ = (pFKXX->nFreq >> 24) & 0xFF;
    }
    else
    {
        *ptr++ = pFKXX->szResult[0];
        *ptr++ = pFKXX->szResult[1];
        *ptr++ = pFKXX->szResult[2];
        *ptr++ = pFKXX->szResult[3];
    }
    nBuffLen -= 4;

    return (ptr - lpBuff);
}

int BD1_ZJXX_Decode(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet = 0;
    int nCount = 0;
    uchar *ptr = lpBuff + 7;
    TBD1ZJXX *pZJXX = (TBD1ZJXX *)pPackage;

    nRet = PT_Decode_INT24(&(pZJXX->nUserAddr), ptr, nBuffLen);
    nBuffLen -= nRet;
    ptr += nRet;

    pZJXX->nICState = *ptr++;
    nBuffLen--;

    pZJXX->nHardwareState = *ptr++;
    nBuffLen--;

    pZJXX->nBatteryState = *ptr++;
    nBuffLen--;

    pZJXX->nInboundState = *ptr++;
    nBuffLen--;

    pZJXX->PowerState[0] = *ptr++;
    pZJXX->PowerState[1] = *ptr++;
    pZJXX->PowerState[2] = *ptr++;
    pZJXX->PowerState[3] = *ptr++;
    pZJXX->PowerState[4] = *ptr++;
    pZJXX->PowerState[5] = *ptr++;
    nBuffLen -= 6;

    return (ptr - lpBuff);
}

int BD1_ZJXX_Encode(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet = 0;
    int nCount = 0;
    uchar *ptr = lpBuff;
    TBD1ZJXX *pZJXX = (TBD1ZJXX *)pPackage;

    nRet = PT_Encode_INT24(ptr, nBuffLen, pZJXX->nUserAddr);
    nBuffLen -= nRet;
    ptr += nRet;

    *ptr++ = pZJXX->nICState;
    nBuffLen--;

    *ptr++ = pZJXX->nHardwareState;
    nBuffLen--;

    *ptr++ = pZJXX->nBatteryState;
    nBuffLen--;

    *ptr++ = pZJXX->nInboundState;
    nBuffLen--;

    *ptr++ = pZJXX->PowerState[0];
    *ptr++ = pZJXX->PowerState[1];
    *ptr++ = pZJXX->PowerState[2];
    *ptr++ = pZJXX->PowerState[3];
    *ptr++ = pZJXX->PowerState[4];
    *ptr++ = pZJXX->PowerState[5];
    nBuffLen -= 6;

    return (ptr - lpBuff);
}

int BD1_XHXX_Decode(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet = 0;
    uchar *ptr = lpBuff + 7;
    TBD1XHXX *pkg = (TBD1XHXX *)pPackage;

    nRet = PT_Decode_INT24(&(pkg->nUserAddr), ptr, nBuffLen);
    nBuffLen -= nRet;
    ptr += nRet;

    nRet = PT_Decode_INT32(&(pkg->nSerialNum), ptr, nBuffLen);
    nBuffLen -= nRet;
    ptr += nRet;

    return (ptr - lpBuff);	
}

int BD1_GXZX_Decode(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet = 0;
    uchar *ptr = lpBuff + 7;
    TBD1GXZX *pkg = (TBD1GXZX *)pPackage;

    nRet = PT_Decode_INT24(&(pkg->nUserAddr), ptr, nBuffLen);
    nBuffLen -= nRet;
    ptr += nRet;

    memcpy(pkg->MngInfo, ptr, 32);
	ptr += 32;
    nBuffLen -= 32;

    return (ptr - lpBuff);	
}

int BD1_GXZX_Encode(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet = 0;
    uchar *ptr = lpBuff;
    TBD1GXZX *pkg = (TBD1GXZX *)pPackage;

    nRet = PT_Encode_INT24(ptr, nBuffLen, pkg->nUserAddr);
    nBuffLen -= nRet;
    ptr += nRet;

	memcpy(ptr, pkg->MngInfo, 32);
	nBuffLen -= 32;
    ptr += 32;

    return (ptr - lpBuff);	
}

int BD1_JSZL_Decode(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet = 0;
    uchar *ptr = lpBuff + 7;
    TBD1JSZL *pkg = (TBD1JSZL *)pPackage;

    nRet = PT_Decode_INT24(&(pkg->nUserAddr), ptr, nBuffLen);
    nBuffLen -= nRet;
    ptr += nRet;

	if (*ptr == 0x55)
	{
	    pkg->nType = 1;
		pkg->szCMD[0] = '\0';
	}
	else
	{
		memcpy(pkg->szCMD, ptr, 4);
		ptr += 4;
    	nBuffLen -= 4;
	}

    return (ptr - lpBuff);	
}

int BD1_JSZL_Encode(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet = 0;
    uchar *ptr = lpBuff;
    TBD1JSZL *pkg = (TBD1JSZL *)pPackage;

    nRet = PT_Encode_INT24(ptr, nBuffLen, pkg->nUserAddr);
    nBuffLen -= nRet;
    ptr += nRet;

	if (pkg->nType == 1)
	{
		*ptr++ = 0x55;
		*ptr++ = 0x55;
		*ptr++ = 0x55;
		nBuffLen -= 3;
	}
	else
	{
		memcpy(ptr, pkg->szCMD, 4);
		nBuffLen -= 4;
	    ptr += 4;
	}

    return (ptr - lpBuff);	
}

int BD1_BSSZ_Decode(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet = 0;
    uchar *ptr = lpBuff + 7;
    TBD1BSSZ *pkg = (TBD1BSSZ *)pPackage;

    nRet = PT_Decode_INT24(&(pkg->nUserAddr), ptr, nBuffLen);
    nBuffLen -= nRet;
    ptr += nRet;

	memcpy(&pkg->Channel, ptr, 6);
	ptr += 6;
	nBuffLen -= 6;

	pkg->nBeamRes = *ptr++;
	nBuffLen--;

    return (ptr - lpBuff);
}

int BD1_BSSZ_Encode(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet = 0;
    uchar *ptr = lpBuff;
    TBD1BSSZ *pkg = (TBD1BSSZ *)pPackage;

    nRet = PT_Encode_INT24(ptr, nBuffLen, pkg->nUserAddr);
    nBuffLen -= nRet;
    ptr += nRet;

	memcpy(ptr, &pkg->Channel, 6);
	ptr += 6;
	nBuffLen -= 6;

	*ptr++ = pkg->nBeamRes;
	nBuffLen--;

    return (ptr - lpBuff);		
}
#endif //#ifdef PT_ENABLE_BD1











