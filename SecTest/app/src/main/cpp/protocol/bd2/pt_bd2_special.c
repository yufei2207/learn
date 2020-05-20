//////////////////////////////////////////////////////////////////////////
//��    ˾������ŵά��Ϣ�������޹�˾
//��	�ߣ�������
//�������ڣ�2012-2-10
//�޸����ڣ�2012-2-10
//�ļ�˵������������RDЭ���׼C��װ�����ļ�
//�޸�˵��������Э��ṹ��ʵ�ֻ���Э�鴦����
//�汾˵����V0.1
//
//////////////////////////////////////////////////////////////////////////

#include "pt_bd2_special.h"

#ifdef  PT_ENABLE_BD2_SPECIAL

//����RD��ŵά�Զ���Э��ͷ����
const uchar BD2_SP_Head[][4] = {{0x55,0xAA,0x01},"XSD","XST"};
//--------------------------------------------------------------------------
//Э��ջ����뺯������
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
//Э��ջ��������
//--------------------------------------------------------------------------
/*
*�������ƣ�BD2_SP_DecodeHead
*����˵����������Ϣ����
*����˵����buf    �������ݻ���
*          len    �������ݳ���
*����ֵ��  -1 ��ʾ��Ч����Ϣ���ͣ�������ʾ����������
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
	        nCount = sizeof(BD2_SP_Head) / sizeof(BD2_SP_Head[0]);//�������Э�����
	        for(i = 0; i< nCount; i++)
	        {
		        if(strncmp(buf + 3, BD2_SP_Head[i], strlen(BD2_SP_Head[i])) == 0)	//Э��ͷ�ж�
		        {
			        return ( i + BD2_SP_ZERO);
		        }
	        }
        }
    }
    else if (len >= 3 && strncmp(buf, BD2_SP_Head[BD2_SP_XNF - BD2_SP_ZERO], 3) == 0)//�û��豸���չߵ�������Ϣ��ʽ
    {
        return BD2_SP_XNF;
    }

	return -1;
}
/*
*�������ƣ�BD2_SP_EncodeHead
*����˵��������Э��ͷ
*����˵����lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          bDevice   �Ƿ��豸
*          nType     ��Ϣ����
*����ֵ��  -1 ��ʾʧ�ܣ�������ʾ�������ݳ���
*/
int BD2_SP_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType)
{
    uchar *pHead = NULL;
    int nLen = 0;

    if (nBuffLen < 7)
        return -1;

    if (nType == BD2_SP_XNF)//�û��豸���չߵ�������Ϣ��ʽ
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
*�������ƣ�BD2_SP_GetEncoder
*����˵����������Ϣ���ͻ�ȡ��Ӧ�ı��뺯��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�ޱ��뺯����������ʾ���뺯��ָ��
*/
FUNC_ENCODER BD2_SP_GetEncoder(int nType)
{
    switch (nType)
    {
    case BD2_SP_XNF://�û��豸���չߵ�������Ϣ��ʽ
    case BD2_SP_XSD://��������û���λ��Ϣ 
    case BD2_SP_XST://��������û�ͨѶ��Ϣ
    default:
        return NULL;
    }
}
/*
*�������ƣ�BD2_SP_GetDecoder
*����˵����������Ϣ���ͻ�ȡ��Ӧ�Ľ��뺯��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�޽��뺯����������ʾ���뺯��ָ��
*/
FUNC_DECODER BD2_SP_GetDecoder(int nType)
{
    switch (nType)
    {
    case BD2_SP_XNF://�û��豸���չߵ�������Ϣ��ʽ
    case BD2_SP_XSD://��������û���λ��Ϣ 
    case BD2_SP_XST://��������û�ͨѶ��Ϣ
    default:
        return NULL;
    }
}
/*
*�������ƣ�BD2_SP_RecvData
*����˵����������յ�������Э�����ݽṹ
*����˵����pParam   ջ
*          data     ���յ�����
*����ֵ��  -1 ��ʾû���յ���֡���ݣ�������ʾ������֡���ݵ���Ϣ����
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
        
        if (pParam->nFrameLen <= 8 || pParam->nFrameLen > pParam->nRecvBuffLen)//֡����Ч,������֡����
        {
            return RESULT_ERROR;
        }
    }
    else if (pParam->nFrameLen > 8)
    {
        if (pParam->nRecvLen == pParam->nFrameLen)//��֡�������
        {
            pParam->nCRCValue ^= PT_MakeCRC(pParam->RecvBuff, 8);

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
*�������ƣ�BD2_SP_EncodePackage
*����˵������������
*����˵����lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          pMsgData  Э�����ݽṹָ��
*          bDevice   �Ƿ��豸ջ���豸ջ����Э��ͷ����"BD"��1��ʾ��PCջ����"CC"��0��ʾ
*����ֵ��  -1 ��ʾʧ�ܣ�������ʾ���ݳ���
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
    {//����RD����Э�鳤��ռ�����ֽ�
        ptr += 2; 
        nBuffLen -= 2;
    }
    else if (nMsgType == BD2_SP_XNF)
    {//�û��豸���չߵ�������Ϣ��ʽ ֡��ռ��1�ֽ�
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
        lpBuff[nHeadLen] =  ptr - lpBuff  - 4;//���Ȳ�����֡ͷ��У��λ
    }
    else
    {
        PT_Encode_SHORT(&lpBuff[nHeadLen], 2, (unsigned short)(ptr - lpBuff + 1));
     }
     //CRC
    *ptr = PT_MakeCRC(lpBuff, ptr - lpBuff);//У�����֡ͷ'$'
    ptr++;
        
    return (ptr - lpBuff);
}
/*
*�������ƣ�BD2_SP_DecodePackage
*����˵�������������֡������Э�����ݽṹ
*����˵����pPackage  ���ݽṹָ��
*          lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          nMsgType  Э������
*����ֵ��  0 ��ʾ����ɹ���������ʾʧ��
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
*�������ƣ�BD2_SP_GetPackage
*����˵����������Ϣ���ͻ�ȡ��Ӧ�����ݽṹ��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�޴��������ݽṹ����������ʾ���ݽṹ��ָ�룬�ṹ���ݰ�ʹ�����Ӧ���ͷſռ�
*/
void* BD2_SP_GetPackage(int nType)
{
    switch (nType)
    {
    case BD2_SP_XNF://�û��豸���չߵ�������Ϣ��ʽ
    case BD2_SP_XSD://��������û���λ��Ϣ 
    case BD2_SP_XST://��������û�ͨѶ��Ϣ
    default:
        return NULL;
    }
}
#endif //WIN32
//--------------------------------------------------------------------------
//Э��ջ����뺯��
//--------------------------------------------------------------------------





#endif //end PT_ENABLE_BD2_SPECIAL define