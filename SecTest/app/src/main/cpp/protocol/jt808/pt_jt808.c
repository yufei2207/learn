/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name��     pt_jt808.c
* Abstract�� JT/T808 protocol 
* 
* Version�� 1.0
* Author��  yfwang
* Date��    2012/07/26
*
* history   :
*	     when            who    	          what:
*      -------------   --------------    -------------------
*      Jul 26, 2012   yfwang                create
*
*/

#include "pt_config.h"
#include "pt_jt808.h"

#ifdef  PT_ENABLE_JT808 

#define JT808_MILLION  1000000.0

const unsigned short JT808_CODE[]={ 0x0001,//�ն�ͨ��Ӧ��
                                    0x8001,//ƽ̨ͨ��Ӧ��
                                    0x0002,//�ն�����
                                    0x0100,//�ն�ע��
                                    0x8100,//�ն�ע��Ӧ��
                                    0x0003,//�ն�ע��  xx  0x0003
                                    0x0102,//�ն˼�Ȩ
                                    0x8103,//�����ն˲���
                                    0x8104,//��ѯ�ն˲���
                                    0x0104,//��ѯ�ն˲���Ӧ��
                                    0x8105,//�ն˿���
                                    0x0200,//λ����Ϣ�㱨
                                    0x8201,//λ����Ϣ��ѯ
                                    0x0201,//λ����Ϣ��ѯӦ��
                                    0x8202,//��ʱλ�ø���
                                    0x8300,//�ı��·�
                                    0x8301,//�¼�����
                                    0x0301,//�¼�����
                                    0x8302,//�����·�  
                                    0x0302,//����Ӧ��
                                    0x8303,//��Ϣ�㲥�˵�����
                                    0x0303,//��Ϣ�㲥/ȡ��
                                    0x8304,//��Ϣ����
                                    0x8400,//�绰�ز�
                                    0x8401,//���õ绰��
                                    0x8500,//��������
                                    0x0500,//��������Ӧ��
                                    0x8600,//����Բ������
                                    0x8601,//ɾ��Բ������
                                    0x8602,//���þ�������
                                    0x8603,//ɾ����������
                                    0x8604,//���ö��������
                                    0x8605,//ɾ�����������
                                    0x8606,//����·��
                                    0x8607,//ɾ��·��
                                    0x8700,//��ʻ��¼�����ݲɼ�����
                                    0x0700,//��ʻ��¼�������ϴ�
                                    0x8701,//��ʻ��¼�ǲ����´�����
                                    0x0701,//�����˵��ϱ�
                                    0x0702,//��ʻԱ�����Ϣ�ɼ��ϱ�
                                    0x0800,//��ý���¼���Ϣ�ϴ�
                                    0x0801,//��ý�������ϴ�
                                    0x8800,//��ý�������ϴ�Ӧ��
                                    0x8801,//����ͷ��������ָ��

									0x8802,//�洢��ý�����ݼ���
									0x0802,//�洢��ý�����ݼ���Ӧ��

                                    0x8803,//�洢��ý�������ϴ�����
                                    0x8804,//¼����ʼ����
                                    0x8900,//��������͸��
                                    0x0900,//��������͸��
                                    0x0901,//����ѹ���ϱ�
                                    0x8A00,//ƽ̨RAS��Կ
                                    0x0A00,//�ն�RAS��Կ
                                    0xFFFF,//�Զ�����û�λ�ñ���
                                    0xFFFE,//�����������ϴ�
                                    0xFFFD,//���������ݲ�ѯ����
                                    0xFFFC,//���������ݲ�ѯ����
									0x8003,//�����ְ�����
									0x8106,//��ѯָ���ն˲���
									0x8107,//��ѯ�ն�����
									0x0107,//��ѯ�ն�����Ӧ��
									0x8108,//�·��ն�������
									0x0108,//�ն��������֪ͨ
									0x8203,//�˹�ȷ�ϱ�����Ϣ
									0x8702,//�ϱ���ʻԱ�����Ϣ����
									0x0704,//��λ���������ϴ�
									0x0705,//CAN���������ϴ�
									0x0805,//����ͷ������������Ӧ��
									0x8805,//�����洢��ý�����ݼ����ϴ�����
                                    0x0110,//�豸����
                                   };

extern int PT_Decode_INT32(int *pVal, uchar *lpBuff, int nLen);
extern int PT_Encode_INT32(uchar *lpBuff, int nLen, int nVal);

extern int PT_Decode_SHORT(short *pVal, uchar *lpBuff, int nLen);
extern int PT_Encode_SHORT(uchar *lpBuff, int nLen, short nVal);
//--------------------------------------------------------------------------
//Э��ջ����뺯������
//--------------------------------------------------------------------------
int JT808_DecodeCommonReq(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeCommonReq(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeCommonRes(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeCommonRes(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeSendAgainReq(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeSendAgainReq(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeRegisterReq(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeRegisterReq(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeRegisterRes(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeRegisterRes(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeLogin(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeLogin(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeParameterSet(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeParameterSet(uchar *lpBuff, int nBuffLen, void *pPackage);


int JT808_DecodeParameterQueryReq(void *pPackage, uchar *lpBuff, int nBuffLen);

int JT808_DecodeParameterQueryRes(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeParameterQueryRes(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeSpecifyParamQueryReq(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeSpecifyParamQueryReq(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodePropertyQueryRes(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodePropertyQueryRes(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeUpdatePackageReq(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeUpdatePackageReq(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeUpdatePackageRes(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeUpdatePackageRes(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeSpecifyParamQueryReq(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeSpecifyParamQueryReq(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodePropertyQueryReq(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodePropertyQueryReq(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeTerminalCtrl(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeTerminalCtrl(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodePositionReport(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodePositionReport(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodePositionQueryRes(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodePositionQueryRes(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeWarmManage(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeWarmManage(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodePositionTrack(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodePositionTrack(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeTextNotice(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeTextNotice(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeEventSet(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeEventSet(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeEventReport(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeEventReport(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeQuestionReq(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeQuestionReq(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeQuestionRes(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeQuestionRes(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeServiceSet(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeServiceSet(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeServiceOpt(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeServiceOpt(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeServiceInfo(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeServiceInfo(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodePhoneCallback(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodePhoneCallback(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodePhoneBook(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodePhoneBook(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeCarCtrlReq(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeCarCtrlReq(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeCarCtrlRes(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeCarCtrlRes(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeRegionCircleSet(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeRegionCircleSet(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeRegionRemove(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeRegionRemove(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeRegionRectSet(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeRegionRectSet(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeRegionPolygonSet(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeRegionPolygonSet(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeRouteSet(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeRouteSet(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeDrivingRecorderReq(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeDrivingRecorderReq(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeDrivingRecorderRes(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeDrivingRecorderRes(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeDrivingRecorderParam(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeDrivingRecorderParam(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeDriverInfoReportReq(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeDriverInfoReportReq(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeOrderReport(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeOrderReport(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeDriverInfoReport(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeDriverInfoReport(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodePosDataUpload(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodePosDataUpload(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeCanDataUpload(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeCanDataUpload(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeMediaEventReport(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeMediaEventReport(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeMediaDataUploadReq(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeMediaDataUploadReq(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeMediaDataUploadRes(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeMediaDataUploadRes(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeOneMediaDataQueryReq(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeOneMediaDataQueryReq(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeRecorderCtrl(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeRecorderCtrl(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeDataLoad(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeDataLoad(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeDataGzipLoad(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeDataGzipLoad(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeRas(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeRas(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeMultiPosition(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeMultiPosition(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeSensorReport(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeSensorReport(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeSensorQueryReq(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeSensorQueryReq(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeSensorQueryRes(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeSensorQueryRes(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeCameralCtrl(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeCameralCtrl(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeCameralCtrlRes(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeCameralCtrlRes(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeMediaIndexReq(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeMediaIndexReq(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeMediaIndexRes(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeMediaIndexRes(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeMediaDataUpload(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeMediaDataUpload(uchar *lpBuff, int nBuffLen, void *pPackage);

int JT808_DecodeDeviceUnlock(void *pPackage, uchar *lpBuff, int nBuffLen);
int JT808_EncodeDeviceUnlock(uchar *lpBuff, int nBuffLen, void *pPackage);

//--------------------------------------------------------------------------
//Э��ջ��������
//--------------------------------------------------------------------------
/*
*�������ƣ�JT808_MakeCRC
*����˵��������У��ֵ
*����˵����buf    �������ݻ���
*          len    �������ݳ���
*����ֵ��  У��ֵ
*/
unsigned char JT808_MakeCRC(uchar *buf, int nLen)
{
    unsigned char nValue = 0;
    unsigned short i = 0;
    for (;i < nLen;i++) 
    {
        nValue ^= buf[i];			
    }

    return nValue;
}
/*
*�������ƣ�JT808_MakeCRC
*����˵��������У��ֵ
*����˵����buf    �������ݻ���
*          len    �������ݳ���
*����ֵ��  У��ֵ
*/
int JT808_Translate(uchar *buf, int nLen, uchar bEncode, int bufsize)
{
    unsigned short nDataLen = 0;
    unsigned short i = 0;

    if (bEncode)//
    {
        short nPos = bufsize - 1;
        for (i = (nLen-1); i >= 0; i--) 
        {
            if (buf[i] == 0x7D)
            {
                buf[nPos--] = 0x01;
                buf[nPos--] = 0x7D;
                nDataLen += 2;
            }
            else if (buf[i] == 0x7E)
            {
                buf[nPos--] = 0x02;
                buf[nPos--] = 0x7D;
                nDataLen += 2;
            }
            else
            {
                buf[nPos--] = buf[i];
                nDataLen++;
            }
            
            if (nPos < 0)
                break;

            if (i == 0)
                break;
        }

        if (i == 0)
        {
            if (nDataLen != nLen || (nDataLen == nLen && nLen > bufsize / 2))
            {//���ݷ����仯
                memcpy(buf, &buf[nPos + 1], nDataLen);
            }
        }
        else
        {
            return -1;
        }
    }
    else//decode
    {
        for (i = 0; i < nLen - 1; i++) 
        {
            if (buf[i] == 0x7D && buf[i + 1] == 0x02)
            {
                buf[nDataLen++] = 0x7E;
                i++;
            }
            else if (buf[i] == 0x7D && buf[i + 1] == 0x01)
            {
                buf[nDataLen++] = 0x7D;
                i++;
            }
            else
            {
                buf[nDataLen++] = buf[i];
            }
        }

        if (i < nLen)
        {
            buf[nDataLen++] = buf[i];
        }
    }

    return nDataLen;
}

int JT808_GetType(unsigned short nType)
{
    int i, nCount;
    nCount = sizeof(JT808_CODE) / sizeof(JT808_CODE[0]);//�������
    for(i = 0; i < nCount; i++)
    {
        if(nType == JT808_CODE[i])	//Э��ͷ�ж�
        {
            return ( i + JT808_ZERO);
        }
    }

    return -1;
}
/*
*�������ƣ�JT808_DecodeHead
*����˵����������Ϣ����
*����˵����buf    �������ݻ���
*          len    �������ݳ���
*����ֵ��  -1 ��ʾ��Ч����Ϣ���ͣ�������ʾ����������
*/
int JT808_DecodeHead(const uchar* buf, int len)
{
    if (len >= 3 && (buf[0] == 0x7E))
    {
        unsigned short nType = 0;

        PT_Decode_SHORT(&nType, (uchar *)(buf + 1), len - 1);

        return JT808_GetType(nType);
    }

	return -1;
}
/*
*�������ƣ�JT808_EncodeHead
*����˵��������Э��ͷ
*����˵����lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          bDevice   �Ƿ��豸
*          nType     ��Ϣ����
*����ֵ��  -1 ��ʾʧ�ܣ�������ʾ�������ݳ���
*/
int JT808_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType)
{
    if (nBuffLen < 4)
        return -1;

    if (JT808_ISTYPE(nType))
    {
        lpBuff[0] = 0x7E;
        nBuffLen--;

        PT_Encode_SHORT(lpBuff + 1, nBuffLen, JT808_CODE[nType - JT808_ZERO]);

        return 3;
    }

    return -1;
}
/*
*�������ƣ�JT808_DecodeHeadEx
*����˵��������Э��ͷ
*����˵����head      ���ݽṹ
*          buf       ���ݻ���
*          len       ���ݳ���
*����ֵ��  -1 ��ʾʧ�ܣ�������ʾ�������ݳ���
*/
int JT808_DecodeHeadEx(TJT808Head* head, unsigned char *buf, int len)
{
    int nRet;
    int nDataLen;
    unsigned char *ptr = buf;

    if(len >= 10)
    {
        head->bMultiPacket = ((*ptr) >> 5) & 0x01;
        head->nEncryptType = ((*ptr) >> 2) & 0x07;

        nDataLen = (*ptr++) & 0x03;
        nDataLen <<= 8 ;
        nDataLen |= *ptr++;
        len -= 2;

        for (nRet = 0; nRet < 12; ptr++)
        {
            if ((*ptr >> 4) >= 0x0 && (*ptr >> 4) <= 0x9)
            {
                head->szSystemID[nRet++] = '0' + (*ptr >> 4);
            }
            else if ((*ptr >> 4) >= 0xA && (*ptr >> 4) <= 0xF)
            {
                head->szSystemID[nRet++] = 'A' + (*ptr >> 4);
            }

            if ((*ptr & 0xF) >= 0x0 && (*ptr & 0xF) <= 0x9)
            {
                head->szSystemID[nRet++] = '0' + (*ptr & 0xF);
            }
            else if ((*ptr & 0xF) >= 0xA && (*ptr & 0xF) <= 0xF)
            {
                head->szSystemID[nRet++] = 'A' + (*ptr & 0xF);
            }
        }
        len -= 6;

        nRet = PT_Decode_SHORT(&(head->nSeqNum), ptr, len);
        ptr += nRet;
        len -= nRet;

        if (head->bMultiPacket)
        {
            if (len >= 4)
            {
                nRet = PT_Decode_SHORT(&(head->nPacketCount), ptr, len);
                ptr += nRet;
                len -= nRet;

                nRet = PT_Decode_SHORT(&(head->nPacketSeq), ptr, len);
                ptr += nRet;
                len -= nRet;
                
                if (nDataLen == len)
                {
                    return ptr - buf;
                }
            }
        }
        else
        {
            head->nPacketCount = 1;
            head->nPacketSeq = 1;

            if (nDataLen == len)
            {
                return ptr - buf;
            }
        }
    }

    return -1;
}

/*
*�������ƣ�JT808_EncodeHeadEx
*����˵��������Э��ͷ
*����˵����lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          head      ���ݽṹ
*����ֵ��  -1 ��ʾʧ�ܣ�������ʾ�������ݳ���
*/
int JT808_EncodeHeadEx(uchar *lpBuff, int nBuffLen, TJT808Head* head)
{
   	unsigned char *ptr = lpBuff;
   	int nRet = 0;
	
	if (nBuffLen < 14)
	{
		return -1;
	}

	if (head->bMultiPacket == 1 || head->nPacketCount > 1)
	{
		*ptr = (1 << 5);
	}
	else 
	{
		*ptr = 0;
	}

	*ptr |= ((head->nEncryptType & 0x07) << 2);
	//datalen 10bit
	
	ptr += 2;

    for (nRet = 0; nRet < 12; nRet++)
    {
        if (head->szSystemID[nRet] >= '0' && head->szSystemID[nRet] <= '9')
        {
            *ptr = (head->szSystemID[nRet] - '0') << 4;
        }
        else if (head->szSystemID[nRet] >= 'A' && head->szSystemID[nRet] <= 'F')
        {
            *ptr = (head->szSystemID[nRet] - 'A') << 4;
        }
        else if (head->szSystemID[nRet] >= 'a' && head->szSystemID[nRet] <= 'f')
        {
            *ptr = (head->szSystemID[nRet] - 'a') << 4;
        }
        
        nRet++;
        if (head->szSystemID[nRet] >= '0' && head->szSystemID[nRet] <= '9')
        {
            *ptr |= (head->szSystemID[nRet] - '0');
        }
        else if (head->szSystemID[nRet] >= 'A' && head->szSystemID[nRet] <= 'F')
        {
            *ptr |= (head->szSystemID[nRet] - 'A');
        }
        else if (head->szSystemID[nRet] >= 'a' && head->szSystemID[nRet] <= 'f')
        {
            *ptr |= (head->szSystemID[nRet] - 'a');
        }

        ptr++;
    }

	nRet = PT_Encode_SHORT(ptr, 2, head->nSeqNum);
	ptr += nRet; 
	
    if (head->bMultiPacket)
	{
		nRet = PT_Encode_SHORT(ptr, 2, head->nPacketCount);
		ptr += nRet; 

		nRet = PT_Encode_SHORT(ptr, 2, head->nPacketSeq);
		ptr += nRet; 
	}

	return ptr - lpBuff;
}
/*
*�������ƣ�JT808_GetEncoder
*����˵����������Ϣ���ͻ�ȡ��Ӧ�ı��뺯��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�ޱ��뺯����������ʾ���뺯��ָ��
*/
FUNC_ENCODER JT808_GetEncoder(int nType)
{
    switch (nType)
    {
    case JT808_TERMINALRES://�ն�ͨ��Ӧ��
    case JT808_PLATFORMRES://ƽ̨ͨ��Ӧ��
        return JT808_EncodeCommonRes;
    case JT808_KEEPALIVE://�ն�����
        return JT808_EncodeCommonReq;
	case JT808_SEND_AGAINREQ://�����ְ�����   Ver201301
		return JT808_EncodeSendAgainReq;
    case JT808_REGISTERREQ://�ն�ע��
        return JT808_EncodeRegisterReq;
    case JT808_REGISTERRES://�ն�ע��Ӧ��
        return JT808_EncodeRegisterRes;
    case JT808_DEREGISTER://�ն�ע��
        return JT808_EncodeCommonReq;
    case JT808_LOGIN://�ն˼�Ȩ
        return JT808_EncodeLogin;
    case JT808_PARAMETET_SET://�����ն˲���
        return JT808_EncodeParameterSet;
    case JT808_PARAMETER_QUERYREQ://��ѯ�ն˲���
        return JT808_EncodeCommonReq;
    case JT808_PARAMETER_QUERYRES://��ѯ�ն˲���Ӧ��
        return JT808_EncodeParameterQueryRes;
	case JT808_SPECIFY_PARAM_QUERYREQ://��ѯָ���ն˲��� Ver201301
		return JT808_EncodeSpecifyParamQueryReq;
	case JT808_PROPERTY_QUERYREQ://��ѯ�ն�����  Ver201301
		return JT808_EncodeCommonReq;
	case JT808_PROPERTY_QUERYRES://��ѯ�ն�����Ӧ��  Ver201301
		return JT808_EncodePropertyQueryRes;
	case JT808_UPDATE_PACKAGEREQ://�·��ն�������  Ver201301
		return JT808_EncodeUpdatePackageReq;
	case JT808_UPDATE_PACKAGERES://�ն��������֪ͨ  Ver201301
		return JT808_EncodeUpdatePackageRes;
    case JT808_TERMINALCTRL://�ն˿���
        return JT808_EncodeTerminalCtrl;
    case JT808_POSITION_REPORT://λ����Ϣ�㱨
        return JT808_EncodePositionReport;
    case JT808_POSITION_QUERYREQ://λ����Ϣ��ѯ
        return JT808_EncodeCommonReq;
    case JT808_POSITION_QUERYRES://λ����Ϣ��ѯӦ��
        return JT808_EncodePositionQueryRes;
    case JT808_POSITION_TRACK://��ʱλ�ø���
        return JT808_EncodePositionTrack;
	case JT808_WARM_MANAGE://�˹�ȷ�ϱ�����Ϣ   Ver201301
		return JT808_EncodeWarmManage;
    case JT808_TEXT_NOTICE://�ı��·�
        return JT808_EncodeTextNotice;
    case JT808_EVENT_SET://�¼�����
        return JT808_EncodeEventSet;
    case JT808_EVENT_REPORT://�¼�����
        return JT808_EncodeEventReport;
    case JT808_QUESTION_REQ://�����·�  
        return JT808_EncodeQuestionReq;
    case JT808_QUESTION_RES://����Ӧ��
        return JT808_EncodeQuestionRes;
    case JT808_SERVICE_SET://��Ϣ�㲥�˵�����
        return JT808_EncodeServiceSet;
    case JT808_SERVICE_OPT://��Ϣ�㲥/ȡ��
        return JT808_EncodeServiceOpt;
    case JT808_SERVICE_INFO://��Ϣ����
        return JT808_EncodeServiceInfo;
    case JT808_PHONECALLBACK://�绰�ز�
        return JT808_EncodePhoneCallback;
    case JT808_PHONEBOOK://���õ绰��
        return JT808_EncodePhoneBook;
    case JT808_CAR_CTRLREQ://��������
        return JT808_EncodeCarCtrlReq;
    case JT808_CAR_CTRLRES://��������Ӧ��
        return JT808_EncodeCarCtrlRes;
    case JT808_REGION_CIRCLE_SET://����Բ������
        return JT808_EncodeRegionCircleSet;
    case JT808_REGION_CIRCLE_REMOVE://ɾ��Բ������
        return JT808_EncodeRegionRemove;
    case JT808_REGION_RECT_SET://���þ�������
        return JT808_EncodeRegionRectSet;
    case JT808_REGION_RECT_REMOVE://ɾ����������
        return JT808_EncodeRegionRemove;
    case JT808_REGION_POLYGON_SET://���ö��������
        return JT808_EncodeRegionPolygonSet;
    case JT808_REGION_POLYGON_REMOVE://ɾ�����������
        return JT808_EncodeRegionRemove;
    case JT808_ROUTE_SET://����·��
        return JT808_EncodeRouteSet;
    case JT808_ROUTE_REMOVE://ɾ��·��
        return JT808_EncodeRegionRemove;
    case JT808_DRIVINGRECORDER_REQ://��ʻ��¼�����ݲɼ�����
        return JT808_EncodeDrivingRecorderReq;
    case JT808_DRIVINGRECORDER_RES://��ʻ��¼�������ϴ�
        return JT808_EncodeDrivingRecorderRes;
    case JT808_DRIVINGRECORDER_PARAM:  //��ʻ��¼�ǲ����´�����
        return JT808_EncodeDrivingRecorderParam;
    case JT808_ORDER_REPORT:              //�����˵��ϱ�
        return JT808_EncodeOrderReport;
	case JT808_DRIVERINFO_REPORTREQ://�ϱ���ʻԱ�����Ϣ����  Ver201301
		return JT808_EncodeCommonReq;
    case JT808_DRIVERINFO_REPORT:         //��ʻԱ�����Ϣ�ɼ��ϱ�
        return JT808_EncodeDriverInfoReport;
	case JT808_POSDATA_UPLOAD://��λ���������ϴ�  Ver201301
		return JT808_EncodePosDataUpload;
	case JT808_CAN_DATA_UPLOAD://CAN���������ϴ�  Ver201301
		return JT808_EncodeCanDataUpload;
    case JT808_MULTIMEDIA_EVENT_REPORT://��ý���¼���Ϣ�ϴ�
        return JT808_EncodeMediaEventReport;
    case JT808_MULTIMEDIA_DATA_UPLOADREQ://��ý�������ϴ�
        return JT808_EncodeMediaDataUploadReq;
    case JT808_MULTIMEDIA_DATA_UPLOADRES://��ý�������ϴ�Ӧ��
        return JT808_EncodeMediaDataUploadRes;
    case JT808_CAMERA_CTRL://����ͷ����ָ�����
        return JT808_EncodeCameralCtrl;
	case JT808_CAMERA_CTRL_RES: //����ͷ������������Ӧ��  Ver201301
		return JT808_EncodeCameralCtrlRes;
    case JT808_MULTIMEDIA_DATA_QUERYREQ://�洢��ý�����ݼ���
        return JT808_EncodeMediaIndexReq;
    case JT808_MULTIMEDIA_DATA_QUERYRES://�洢��ý�����ݼ���Ӧ��
        return JT808_EncodeMediaIndexRes;
    case JT808_MULTIMEDIA_DATA_UPLOADCMD://�洢��ý�������ϴ�����
        return JT808_EncodeMediaDataUpload;
	case JT808_ONE_MULTIMEDIA_DATA_QUERYREQ://�����洢��ý�����ݼ����ϴ����� Ver201301
		return JT808_EncodeOneMediaDataQueryReq;
    case JT808_RECORDER_CTRL://¼�����ʼ/ֹͣ
        return JT808_EncodeRecorderCtrl;
    case JT808_DATA_DOWNLOAD://��������͸��
    case JT808_DATA_UPLOAD://��������͸��
		return JT808_EncodeDataLoad;
    case JT808_DATA_GZIP_UPLOAD://����ѹ���ϱ�  //modify by xlsong
        return JT808_EncodeDataGzipLoad;
    case JT808_PLATFORM_RAS://ƽ̨RAS��Կ
    case JT808_TERMINAL_RAS://�ն�RAS��Կ
        return JT808_EncodeRas;
    case JT808_MULTIPOSITION://���û�λ���ϱ�,�Զ���Э��
        return JT808_EncodeMultiPosition;
    case JT808_SENSOR_REPORT:
        return JT808_EncodeSensorReport;
    case JT808_SENSOR_QUERYREQ:
        return JT808_EncodeSensorQueryReq;
    case JT808_SENSOR_QUERYRES:
        return JT808_EncodeSensorQueryRes;
    case JT808_DEVICE_UNLOCK:
        return JT808_EncodeDeviceUnlock;
    default:
        return NULL;
    }
}
/*
*�������ƣ�JT808_GetDecoder
*����˵����������Ϣ���ͻ�ȡ��Ӧ�Ľ��뺯��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�޽��뺯����������ʾ���뺯��ָ��
*/
FUNC_DECODER JT808_GetDecoder(int nType)
{
    switch (nType)
    {
    case JT808_TERMINALRES://�ն�ͨ��Ӧ��
    case JT808_PLATFORMRES://ƽ̨ͨ��Ӧ��
        return JT808_DecodeCommonRes;
    case JT808_KEEPALIVE://�ն�����
        return JT808_DecodeCommonReq;
	case JT808_SEND_AGAINREQ://�����ְ�����
		return JT808_DecodeSendAgainReq;
    case JT808_REGISTERREQ://�ն�ע��
        return JT808_DecodeRegisterReq;
    case JT808_REGISTERRES://�ն�ע��Ӧ��
        return JT808_DecodeRegisterRes;
    case JT808_DEREGISTER://�ն�ע��
        return JT808_DecodeCommonReq;
    case JT808_LOGIN://�ն˼�Ȩ
        return JT808_DecodeLogin;
    case JT808_PARAMETET_SET://�����ն˲���
        return JT808_DecodeParameterSet;
    case JT808_PARAMETER_QUERYREQ://��ѯ�ն˲���
        return JT808_DecodeCommonReq;
    case JT808_PARAMETER_QUERYRES://��ѯ�ն˲���Ӧ��
        return JT808_DecodeParameterQueryRes;
	case JT808_SPECIFY_PARAM_QUERYREQ://��ѯָ���ն˲���  Ver301301
		return JT808_DecodeSpecifyParamQueryReq;
	case JT808_PROPERTY_QUERYREQ://��ѯ�ն�����   Ver301301
		return JT808_DecodeCommonReq;
	case JT808_PROPERTY_QUERYRES://��ѯ�ն�����Ӧ�� Ver301301
		return JT808_DecodePropertyQueryRes;
	case JT808_UPDATE_PACKAGEREQ://�·��ն�������  Ver301301
		return JT808_DecodeUpdatePackageReq;
	case JT808_UPDATE_PACKAGERES://�ն��������֪ͨ  Ver301301
		return JT808_DecodeUpdatePackageRes;
    case JT808_TERMINALCTRL://�ն˿���
        return JT808_DecodeTerminalCtrl;
    case JT808_POSITION_REPORT://λ����Ϣ�㱨
        return JT808_DecodePositionReport;;
    case JT808_POSITION_QUERYREQ://λ����Ϣ��ѯ
        return JT808_DecodeCommonReq;
    case JT808_POSITION_QUERYRES://λ����Ϣ��ѯӦ��
        return JT808_DecodePositionQueryRes;
    case JT808_POSITION_TRACK://��ʱλ�ø���
        return JT808_DecodePositionTrack;
	case JT808_WARM_MANAGE://�˹�ȷ�ϱ�����Ϣ   Ver201301
		return JT808_DecodeWarmManage; 
    case JT808_TEXT_NOTICE://�ı��·�
        return JT808_DecodeTextNotice;
    case JT808_EVENT_SET://�¼�����
        return JT808_DecodeEventSet;
    case JT808_EVENT_REPORT://�¼�����
        return JT808_DecodeEventReport;
    case JT808_QUESTION_REQ://�����·�  
        return JT808_DecodeQuestionReq;
    case JT808_QUESTION_RES: //����Ӧ��
        return JT808_DecodeQuestionRes;
    case JT808_SERVICE_SET://��Ϣ�㲥�˵�����
        return JT808_DecodeServiceSet;
    case JT808_SERVICE_OPT://��Ϣ�㲥/ȡ��
        return JT808_DecodeServiceOpt;
    case JT808_SERVICE_INFO://��Ϣ����
        return JT808_DecodeServiceInfo;
    case JT808_PHONECALLBACK://�绰�ز�
        return JT808_DecodePhoneCallback;
    case JT808_PHONEBOOK://���õ绰��
        return JT808_DecodePhoneBook;
    case JT808_CAR_CTRLREQ://��������
        return JT808_DecodeCarCtrlReq;
    case JT808_CAR_CTRLRES://��������Ӧ��
        return JT808_DecodeCarCtrlRes;
    case JT808_REGION_CIRCLE_SET://����Բ������
        return JT808_DecodeRegionCircleSet;
    case JT808_REGION_CIRCLE_REMOVE://ɾ��Բ������
        return JT808_DecodeRegionRemove;
    case JT808_REGION_RECT_SET://���þ�������
        return JT808_DecodeRegionRectSet;
    case JT808_REGION_RECT_REMOVE://ɾ����������
        return JT808_DecodeRegionRemove;
    case JT808_REGION_POLYGON_SET://���ö��������
        return JT808_DecodeRegionPolygonSet;
    case JT808_REGION_POLYGON_REMOVE://ɾ�����������
        return JT808_DecodeRegionRemove;
    case JT808_ROUTE_SET://����·��
        return JT808_DecodeRouteSet;
    case JT808_ROUTE_REMOVE://ɾ��·��
        return JT808_DecodeRegionRemove;
    case JT808_DRIVINGRECORDER_REQ://��ʻ��¼�����ݲɼ�����
        return JT808_DecodeDrivingRecorderReq;
    case JT808_DRIVINGRECORDER_RES: //��ʻ��¼�������ϴ�
        return JT808_DecodeDrivingRecorderRes;
    case JT808_DRIVINGRECORDER_PARAM:  //��ʻ��¼�ǲ����´�����
        return JT808_DecodeDrivingRecorderParam;
    case JT808_ORDER_REPORT:           //�����˵��ϱ�
        return JT808_DecodeOrderReport;
    case JT808_DRIVERINFO_REPORT:         //��ʻԱ�����Ϣ�ɼ��ϱ�
        return JT808_DecodeDriverInfoReport;
    case JT808_MULTIMEDIA_EVENT_REPORT://��ý���¼���Ϣ�ϴ�
        return JT808_DecodeMediaEventReport;
    case JT808_MULTIMEDIA_DATA_UPLOADREQ://��ý�������ϴ�
        return JT808_DecodeMediaDataUploadReq;
    case JT808_MULTIMEDIA_DATA_UPLOADRES://��ý�������ϴ�Ӧ��
        return JT808_DecodeMediaDataUploadRes;
    case JT808_CAMERA_CTRL://����ͷ����ָ�����
        return JT808_DecodeCameralCtrl;
    case JT808_MULTIMEDIA_DATA_QUERYREQ://�洢��ý�����ݼ���
        return JT808_DecodeMediaIndexReq;
    case JT808_MULTIMEDIA_DATA_QUERYRES://�洢��ý�����ݼ���Ӧ��
        return JT808_DecodeMediaIndexRes;
    case JT808_MULTIMEDIA_DATA_UPLOADCMD://�洢��ý�������ϴ�����
        return JT808_DecodeMediaDataUpload;
    case JT808_RECORDER_CTRL://¼�����ʼ/ֹͣ
        return JT808_DecodeRecorderCtrl;
    case JT808_DATA_DOWNLOAD://��������͸��
    case JT808_DATA_UPLOAD://��������͸��
		return JT808_DecodeDataLoad;
    case JT808_DATA_GZIP_UPLOAD://����ѹ���ϱ�  modify by xlsong
        return JT808_DecodeDataGzipLoad;
    case JT808_PLATFORM_RAS://ƽ̨RAS��Կ
    case JT808_TERMINAL_RAS://�ն�RAS��Կ
        return JT808_DecodeRas;
    case JT808_MULTIPOSITION://���û�λ���ϱ�,�Զ���Э��
        return JT808_DecodeMultiPosition;
	case JT808_DRIVERINFO_REPORTREQ://�ϱ���ʻԱ�����Ϣ����  Ver201301
		return JT808_DecodeCommonReq;
	case JT808_POSDATA_UPLOAD://��λ���������ϴ�  Ver201301
		return JT808_DecodePosDataUpload;
	case JT808_CAN_DATA_UPLOAD://CAN���������ϴ�  Ver201301
		return JT808_DecodeCanDataUpload;
	case JT808_CAMERA_CTRL_RES: //����ͷ������������Ӧ�� Ver201301
		return JT808_DecodeCameralCtrlRes;
	case JT808_ONE_MULTIMEDIA_DATA_QUERYREQ://�����洢��ý�����ݼ����ϴ����� Ver201301
		return JT808_DecodeOneMediaDataQueryReq;
    case JT808_SENSOR_REPORT:
        return JT808_DecodeSensorReport;
    case JT808_SENSOR_QUERYREQ:
        return JT808_DecodeSensorQueryReq;
    case JT808_SENSOR_QUERYRES:
        return JT808_DecodeSensorQueryRes;
    case JT808_DEVICE_UNLOCK:
        return JT808_DecodeDeviceUnlock;
    default:
        return NULL;
    }
}
/*
*�������ƣ�JT808_RecvData
*����˵����������յ�������Э�����ݽṹ
*����˵����pParam Э�����ջָ��
*          data     ���յ�����
*����ֵ��  -1 ��ʾû���յ���֡���ݣ�������ʾ������֡���ݵ���Ϣ����
*/
int  JT808_RecvData(TStackParam *pParam, uchar data)
{
    int nRet = RESULT_ERROR;
    if (pParam->nRecvLen == 0)
    {
        if (data == 0x7E)//�յ�֡ͷ
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
    else if (data == 0x7E && pParam->nRecvLen == 1)
    {
        pParam->nRecvLen = 1;
    }
    else
    {
        pParam->RecvBuff[pParam->nRecvLen++] = data;

        if (pParam->nRecvLen == 3)
        {
            pParam->nMsgType = JT808_DecodeHead(pParam->RecvBuff, pParam->nRecvLen);
            if (pParam->nMsgType == INVALID_TYPE)
            {
                pParam->nRecvLen = 0;
            }
        }
        else if (pParam->nRecvLen > 3)
        {
            nRet = JT808_RecvDataEx(pParam, data);
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
*�������ƣ�JT808_RecvDataEx
*����˵����������յ�������Э�����ݽṹ
*����˵����pParam   ջ
*          data     ���յ�����
*����ֵ��  -1 ��ʾû���յ���֡���ݣ�������ʾ������֡���ݵ���Ϣ����
*/
int  JT808_RecvDataEx(TStackParam *pParam, uchar data)
{
    if (data == 0x7E) //֡������
    {
        pParam->nFrameLen = JT808_Translate(pParam->RecvBuff + 1, pParam->nRecvLen - 2, 0, pParam->nRecvBuffLen);
        pParam->nFrameLen += 2;
        pParam->RecvBuff[pParam->nFrameLen - 1] = 0x7E;
        
        pParam->nCRCValue = JT808_MakeCRC(pParam->RecvBuff + 1, pParam->nFrameLen - 3);

        if (pParam->nCRCValue == pParam->RecvBuff[pParam->nFrameLen - 2])//У����ȷ
        {
             pParam->bRecvFrame = 1;
             return pParam->nMsgType;
        }
        else
        {
             return RESULT_ERROR;
        }
    }
    else if (pParam->nRecvLen >= pParam->nRecvBuffLen)
    {
        return RESULT_ERROR;
    }
    
    return RESULT_SUCCESS;
}
/*
*�������ƣ�JT808_DecodePackage
*����˵�������������֡������Э�����ݽṹ
*����˵����pPackage  ���ݽṹָ��
*          lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          nMsgType  Э������
*����ֵ��  0 ��ʾ����ɹ���������ʾʧ��
*/
int JT808_DecodePackage(void *pPackage, int nMsgType, uchar *lpBuff, int nBuffLen)
{
    int nTmp;

    FUNC_DECODER fnDecoder = NULL;
    TJT808Head *head = (TJT808Head *)pPackage;

    fnDecoder = JT808_GetDecoder(nMsgType);
    if (fnDecoder == NULL)
        return RESULT_ERROR;

    if (nBuffLen > 12)
    {
        nTmp = JT808_MakeCRC(lpBuff + 1, nBuffLen - 3);
        if (nTmp == lpBuff[nBuffLen - 2])
        {
            nTmp = JT808_DecodeHeadEx(head, lpBuff + 3, nBuffLen - 5);
            if (nTmp >= 0)
            {
                head->nID = nMsgType;
                lpBuff[nBuffLen - 2] = '\0';
                return fnDecoder(pPackage, lpBuff + 3 + nTmp, nBuffLen - nTmp - 5);
            }
        }
    }

    return RESULT_ERROR;
}

/*
*�������ƣ�GT06_EncodePackage
*����˵������������
*����˵����lpBuff    ���ݻ���
*          nBuffLen  ���ݻ��泤��
*          pMsgData  Э�����ݽṹָ��
*          bDevice   �Ƿ��豸ջ���豸ջ����Э��ͷ����"BD"��1��ʾ��PCջ����"CC"��0��ʾ
*����ֵ��  -1 ��ʾʧ�ܣ�������ʾ���ݳ���
*/
int JT808_EncodePackage(uchar *lpBuff, int nBuffLen, 
                       void *pPackage, int nMsgType, char bDevice)
{
    int nRet = RESULT_ERROR;
    int nBuffSize = nBuffLen;
    FUNC_ENCODER fnEncoder = NULL;
    uchar *ptr = lpBuff;
    TJT808Head *head = (TJT808Head *)pPackage;

    fnEncoder = JT808_GetEncoder(nMsgType);
    if (fnEncoder == NULL)
        return RESULT_ERROR;

//encode head
    nRet = JT808_EncodeHead(ptr, nBuffLen, bDevice, nMsgType);
    if (nRet == RESULT_ERROR)
    {
        return nRet;
    }
    else
    {
        ptr += nRet;
        nBuffLen -= nRet;
    }

    nRet = JT808_EncodeHeadEx(ptr, nBuffLen, head);
    if (nRet == RESULT_ERROR)
    {
        return nRet;
    }
    else
    {
        ptr += nRet;
        nBuffLen -= nRet;
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
//encode length, CRC ,frame tail
    if (nBuffLen < 2)
    {
        return RESULT_ERROR;
    }

    //encode message body length
    lpBuff[3] |= ((nRet >> 8) & 0x03);
    lpBuff[4] = (nRet & 0xFF);

    //length include frame tail char��0x7E��and crc(one bytes)
    *ptr++ = JT808_MakeCRC(lpBuff + 1, ptr - lpBuff - 1);

    nBuffLen = JT808_Translate(lpBuff + 1, ptr - lpBuff - 1, 1, nBuffSize - 2);
    if (nBuffLen > 0)
    {
        nBuffLen += 1;

        lpBuff[nBuffLen++] = 0x7E;

        return nBuffLen;
    }

    return RESULT_ERROR;
}

#ifdef WIN32
/*
*�������ƣ�NW_GetPackage
*����˵����������Ϣ���ͻ�ȡ��Ӧ�����ݽṹ��
*����˵����nType ��Ϣ����
*����ֵ��  NULL ��ʾ�޴��������ݽṹ����������ʾ���ݽṹ��ָ�룬�ṹ���ݰ�ʹ�����Ӧ���ͷſռ�
*/
void* JT808_GetPackage(int nType)
{
    switch (nType)
    { 
    case JT808_TERMINALRES://�ն�ͨ��Ӧ��
    case JT808_PLATFORMRES://ƽ̨ͨ��Ӧ��
    case JT808_KEEPALIVE://�ն�����
    case JT808_REGISTERREQ://�ն�ע��
    case JT808_REGISTERRES://�ն�ע��Ӧ��
    case JT808_DEREGISTER://�ն�ע��
    case JT808_LOGIN://�ն˼�Ȩ
    case JT808_PARAMETET_SET://�����ն˲���
    case JT808_PARAMETER_QUERYREQ://��ѯ�ն˲���
    case JT808_PARAMETER_QUERYRES://��ѯ�ն˲���Ӧ��
    case JT808_TERMINALCTRL://�ն˿���
    case JT808_POSITION_REPORT://λ����Ϣ�㱨
    case JT808_POSITION_QUERYREQ://λ����Ϣ��ѯ
    case JT808_POSITION_QUERYRES://λ����Ϣ��ѯӦ��
    case JT808_POSITION_TRACK://��ʱλ�ø���
    case JT808_TEXT_NOTICE://�ı��·�
    case JT808_EVENT_SET://�¼�����
    case JT808_EVENT_REPORT://�¼�����
    case JT808_QUESTION_REQ://�����·�  
    case JT808_QUESTION_RES://����Ӧ��
    case JT808_SERVICE_SET://��Ϣ�㲥�˵�����
    case JT808_SERVICE_OPT://��Ϣ�㲥/ȡ��
    case JT808_SERVICE_INFO://��Ϣ����
    case JT808_PHONECALLBACK://�绰�ز�
    case JT808_PHONEBOOK://���õ绰��
    case JT808_CAR_CTRLREQ://��������
    case JT808_CAR_CTRLRES://��������Ӧ��
    case JT808_REGION_CIRCLE_SET://����Բ������
    case JT808_REGION_CIRCLE_REMOVE://ɾ��Բ������
    case JT808_REGION_RECT_SET://���þ�������
    case JT808_REGION_RECT_REMOVE://ɾ����������
    case JT808_REGION_POLYGON_SET://���ö��������
    case JT808_REGION_POLYGON_REMOVE://ɾ�����������
    case JT808_ROUTE_SET://����·��
    case JT808_ROUTE_REMOVE://ɾ��·��
    case JT808_DRIVINGRECORDER_REQ://��ʻ��¼�����ݲɼ�����
    case JT808_DRIVINGRECORDER_RES://��ʻ��¼�������ϴ�
    case JT808_DRIVINGRECORDER_PARAM:  //��ʻ��¼�ǲ����´�����
    case JT808_ORDER_REPORT:              //�����˵��ϱ�
    case JT808_DRIVERINFO_REPORT:         //��ʻԱ�����Ϣ�ɼ��ϱ�
    case JT808_MULTIMEDIA_EVENT_REPORT://��ý���¼���Ϣ�ϴ�
    case JT808_MULTIMEDIA_DATA_UPLOADREQ://��ý�������ϴ�
    case JT808_MULTIMEDIA_DATA_UPLOADRES://��ý�������ϴ�Ӧ��
    case JT808_CAMERA_CTRL://����ͷ����ָ�����
    case JT808_MULTIMEDIA_DATA_QUERYREQ://�洢��ý�����ݼ���
    case JT808_MULTIMEDIA_DATA_QUERYRES://�洢��ý�����ݼ���Ӧ��
    case JT808_MULTIMEDIA_DATA_UPLOADCMD://�洢��ý�������ϴ�����
    case JT808_RECORDER_CTRL://¼�����ʼ/ֹͣ
    case JT808_DATA_DOWNLOAD://��������͸��
    case JT808_DATA_UPLOAD://��������͸��
    case JT808_DATA_GZIP_UPLOAD://����ѹ���ϱ�
    case JT808_PLATFORM_RAS://ƽ̨RAS��Կ
    case JT808_TERMINAL_RAS://�ն�RAS��Կ
    default:
        return NULL;
    }
}
#endif //WIN32
//--------------------------------------------------------------------------
//Э��ջ����뺯��ʵ��
//--------------------------------------------------------------------------
int JT808_DecodeCommonReq(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    return 0;
}

int JT808_EncodeCommonReq(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    return 0;
}
//ͨ��Ӧ�����
int JT808_DecodeCommonRes(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808COMMONRES *dat = (TJT808COMMONRES *)pPackage;

    nRet = PT_Decode_SHORT(&(dat->nSeqNum), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Decode_SHORT(&(dat->nResID), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    dat->nResID = JT808_GetType(dat->nResID);

    dat->nResult = *ptr++;

    return (ptr - lpBuff);
}

int JT808_EncodeCommonRes(uchar *lpBuff, int nBuffLen, void *pPackage)//ͨ��Ӧ�����
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808COMMONRES *dat = (TJT808COMMONRES *)pPackage;

    if (!JT808_ISTYPE(dat->nResID))
    {
        return -1;
    }

    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nSeqNum);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, JT808_CODE[dat->nResID - JT808_ZERO]);
    ptr += nRet;
    nBuffLen -= nRet;

    *ptr++ = dat->nResult;

    return (ptr - lpBuff);
}
//�����ְ�����   Ver201301
int JT808_DecodeSendAgainReq(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet, i = 0;
	uchar *ptr = lpBuff;
	TJT808SendAgainReq *dat = (TJT808SendAgainReq *)pPackage;

	// Ӧ����ˮ��
	nRet = PT_Decode_SHORT(&(dat->nSeqNum), ptr, nBuffLen);
	ptr += nRet;
	nBuffLen -= nRet;

	//�ش�������
	dat->nCount = *ptr++;
	nBuffLen--;

    if (dat->nCount > 128)
    {
        dat->nCount = 128;
    }
    
	//�ش� ID �б�
	for (i = 0; i < (dat->nCount); i++)
	{
		nRet = PT_Decode_SHORT(&(dat->nIDlist[i]), ptr, nBuffLen);
		ptr += nRet;
		nBuffLen -= nRet;
	}
	return (ptr - lpBuff);
}
int JT808_EncodeSendAgainReq(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet, i = 0;
	uchar *ptr = lpBuff;
	TJT808SendAgainReq *dat = (TJT808SendAgainReq *)pPackage;

	// Ӧ����ˮ��
	nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nSeqNum);
	ptr += nRet;
	nBuffLen -= nRet;

	//�ش�������
	*ptr++ = dat->nCount;
	nBuffLen--;

	//�ش� ID �б�
	for (i = 0; i<(dat->nCount); i++)
	{
		nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nIDlist[i]);
		ptr += nRet;
		nBuffLen -= nRet;
	}
	return (ptr - lpBuff);
}
//�ն�ע��
int JT808_DecodeRegisterReq(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet,nLen = 0;
    uchar *ptr = lpBuff;
    TJT808REGISTERREQ *dat = (TJT808REGISTERREQ *)pPackage;

    //ʡ��ID
    nRet = PT_Decode_SHORT(&(dat->nProvID), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    //������ID
    nRet = PT_Decode_SHORT(&(dat->nCityID), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    //������ID
    dat->szManID[0] = *ptr++;
    dat->szManID[1] = *ptr++;
    dat->szManID[2] = *ptr++;
    dat->szManID[3] = *ptr++;
    dat->szManID[4] = *ptr++;
    nBuffLen -= 5;

    //�ն��ͺ�
	nLen = sizeof(dat->szModel);   //---   by xlsong
    for (nRet = 0; nRet < nLen; nRet++)
    {
        dat->szModel[nRet] = *ptr++;
        if (dat->szModel[nRet] == ' ')
        {
            dat->szModel[nRet] = '\0';
        }
    }
    nBuffLen -= nLen;

    //�ն�ID
    memcpy(dat->szTerminalID, ptr, 7);
    dat->szTerminalID[7] = '\0';
    ptr += 7;
    nBuffLen -= 7;

    //������ɫ
    dat->nColor = *ptr++;
    nBuffLen -= 1;

    //����
    if (nBuffLen > sizeof(dat->szLicensePlate) - 1)
    {
        nBuffLen = sizeof(dat->szLicensePlate) - 1;
    }

    memcpy(dat->szLicensePlate, ptr, nBuffLen);
    dat->szLicensePlate[nBuffLen] = 0;
    ptr += nBuffLen;

    return (ptr - lpBuff);
}
//�ն�ע��
int JT808_EncodeRegisterReq(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet,nLen = 0;
    uchar *ptr = lpBuff;
    TJT808REGISTERREQ *dat = (TJT808REGISTERREQ *)pPackage;

    //ʡ��ID
    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nProvID);
    ptr += nRet;
    nBuffLen -= nRet;

    //������ID
    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nCityID);
    ptr += nRet;
    nBuffLen -= nRet;

    //������ID
    memcpy(ptr, dat->szManID, 5);
    ptr += 5;
    nBuffLen -= 5;

    //�ն��ͺ�
	nLen = sizeof(dat->szModel);//  -- by xlsong
    nRet = strlen((char *)dat->szModel);
    if (nRet < nLen)
    {
        memcpy(ptr, dat->szModel, nRet);
#ifdef  PT_JT808_VER_201301        
        memset(ptr + nRet, 0, nLen - nRet);
#else
        memset(ptr + nRet, 0x20, nLen - nRet);
#endif        
    }
    else
    {
        memcpy(ptr, dat->szModel, nLen);
    }
    ptr += nLen;
    nBuffLen -= nLen;

    //�ն�ID
    memcpy(ptr, dat->szTerminalID, 7);
    ptr += 7;
    nBuffLen -= 7;

    //������ɫ
    *ptr++ = dat->nColor;
    nBuffLen -= 1;

    //����
    nRet = strlen(dat->szLicensePlate);//����������
    memcpy(ptr,dat->szLicensePlate,nRet); 
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}
//�ն�ע��Ӧ��
int JT808_DecodeRegisterRes(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808REGISTERRES *dat = (TJT808REGISTERRES *)pPackage;

    //Ӧ����ˮ��
    nRet = PT_Decode_SHORT(&(dat->nSeqNum), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    //���
    dat->nResult = *ptr++;
    nBuffLen -= 1;

    //��Ȩ��
    if(dat->nResult == 0)//�ɹ�
    {
        if (dat->head.nProtocol == JT808_DEF_PROTOCOL_BD)//���ڱ�����ƽ̨Ϊ�豸����Ψһ������
        {
            for (nRet = 0; nRet < 12; ptr++)
            {
                if ((*ptr >> 4) >= 0x0 && (*ptr >> 4) <= 0x9)
                {
                    dat->szSystemID[nRet++] = '0' + (*ptr >> 4);
                }
                else if ((*ptr >> 4) >= 0xA && (*ptr >> 4) <= 0xF)
                {
                    dat->szSystemID[nRet++] = 'A' + (*ptr >> 4);
                }

                if ((*ptr & 0xF) >= 0x0 && (*ptr & 0xF) <= 0x9)
                {
                    dat->szSystemID[nRet++] = '0' + (*ptr & 0xF);
                }
                else if ((*ptr & 0xF) >= 0xA && (*ptr & 0xF) <= 0xF)
                {
                    dat->szSystemID[nRet++] = 'A' + (*ptr & 0xF);
                }
            }
            dat->szSystemID[nRet] = '\0';
            nBuffLen -= 6;
        }
        else
        {
            dat->szSystemID[0] = '\0';
        }
        
        if (nBuffLen > sizeof(dat->szAuthcode) - 1)
        {
            dat->szAuthcode[nBuffLen] = '\0';
            return -1;
        }
        else
        {
            memcpy(dat->szAuthcode, ptr, nBuffLen);
            dat->szAuthcode[nBuffLen] = '\0';
        }

        ptr += nBuffLen;
    }
    else
    {
        dat->szAuthcode[0] = '\0';
        dat->szSystemID[0] = '\0';
    }

    return (ptr - lpBuff);
}
//�ն�ע��Ӧ��
int JT808_EncodeRegisterRes(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808REGISTERRES *dat = (TJT808REGISTERRES *)pPackage;

    //Ӧ����ˮ��
    nRet = PT_Encode_SHORT(ptr, nBuffLen,dat->nSeqNum);
    ptr += nRet;
    nBuffLen -= nRet;

    //���
    *ptr++ = dat->nResult;
    nBuffLen -= 1;

    //��Ȩ��
    if(dat->nResult == 0)//�ɹ�
    {
        if (dat->head.nProtocol == JT808_DEF_PROTOCOL_BD)//���ڱ�����ƽ̨Ϊ�豸����Ψһ������
        {
            for (nRet = 0; nRet < 12; nRet++)
            {
                if (dat->szSystemID[nRet] >= '0' && dat->szSystemID[nRet] <= '9')
                {
                    *ptr = (dat->szSystemID[nRet] - '0') << 4;
                }
                else if (dat->szSystemID[nRet] >= 'A' && dat->szSystemID[nRet] <= 'F')
                {
                    *ptr = (dat->szSystemID[nRet] - 'A') << 4;
                }
                else if (dat->szSystemID[nRet] >= 'a' && dat->szSystemID[nRet] <= 'f')
                {
                    *ptr = (dat->szSystemID[nRet] - 'a') << 4;
                }

                nRet++;
                if (dat->szSystemID[nRet] >= '0' && dat->szSystemID[nRet] <= '9')
                {
                    *ptr |= (dat->szSystemID[nRet] - '0');
                }
                else if (dat->szSystemID[nRet] >= 'A' && dat->szSystemID[nRet] <= 'F')
                {
                    *ptr |= (dat->szSystemID[nRet] - 'A');
                }
                else if (dat->szSystemID[nRet] >= 'a' && dat->szSystemID[nRet] <= 'f')
                {
                    *ptr |= (dat->szSystemID[nRet] - 'a');
                }

                ptr++;
            }

            nBuffLen -= 6;
        }

        nRet = strlen(dat->szAuthcode) + 1;//����������
        memcpy(ptr, dat->szAuthcode, nRet); 
        ptr += nRet;
        nBuffLen -= nRet;
    }	

    return (ptr - lpBuff);
}
//�ն˼�Ȩ
int JT808_DecodeLogin(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    uchar *ptr = lpBuff;
    TJT808LOGIN *dat = (TJT808LOGIN *)pPackage;

    if (nBuffLen > sizeof(dat->szAuthcode) - 1)
    {
        dat->szAuthcode[0] = '\0';
        return -1;
    }
    else
    {
        memcpy(dat->szAuthcode, ptr, nBuffLen);
        dat->szAuthcode[nBuffLen] = '\0';
    }

    ptr += nBuffLen;

    return (ptr - lpBuff);
}
//�ն˼�Ȩ
int JT808_EncodeLogin(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808LOGIN *dat = (TJT808LOGIN *)pPackage;

    nRet = strlen(dat->szAuthcode);//����������
    memcpy(ptr, dat->szAuthcode, nRet); 
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}

int JT808_GetParamDataType(unsigned short nParamType)
{
    if (nParamType >= JT808_DEF_PARAM_CAN_ID_SET_FIRST && 
        nParamType <= JT808_DEF_PARAM_CAN_ID_SET_LAST)//CAN ���� ID �����ɼ�����
    {
		return 4;//BYTE[8] --------C�ļ���Ҫ����BYTE[8]�Ľ�������---by ?xlsong
    }
    else if (nParamType >= JT808_DEF_PARAM_TYRE_ID_FIRST && 
             nParamType <= JT808_DEF_PARAM_TYRE_ID_LAST)//��̥ѹ������ID
    {
        return 2;
    }
    
    switch (nParamType)
    {
    case JT808_DEF_PARAM_KEEPALIVE://�ն��������ͼ������λΪ��S
    case JT808_DEF_PARAM_TCP_TIMEOUT://TCP��ϢӦ��ʱʱ�䣬��λΪ��S
    case JT808_DEF_PARAM_TCP_RESEND://TCP��Ϣ�ش�����
    case JT808_DEF_PARAM_UDP_TIMEOUT://UDP��ϢӦ��ʱʱ�䣬��λΪ��S
    case JT808_DEF_PARAM_UDP_RESEND://UDP��Ϣ�ش�����
    case JT808_DEF_PARAM_SMS_TIMEOUT://SMS��ϢӦ��ʱʱ�䣬��λΪ��S
    case JT808_DEF_PARAM_SMS_RESEND://SMS��Ϣ�ش�����
    case JT808_DEF_PARAM_BD_TIMEOUT://������ϢӦ��ʱʱ�䣬��λΪ��S
    case JT808_DEF_PARAM_BD_RESEND://������Ϣ�ش�����
        return 2;//DWORD
    case JT808_DEF_PARAM_MASTER_APN://��������APN������ͨѶ���ŷ��ʵ�.
    case JT808_DEF_PARAM_MASTER_USERNAME://������������ͨѶ�����û���
    case JT808_DEF_PARAM_MASTER_PASSWORD://������������ͨѶ��������
    case JT808_DEF_PARAM_MASTER_ADDRESS://��������IP��ַ��������
    case JT808_DEF_PARAM_SLAVE_APN://��������APN������ͨѶ���ŷ��ʵ�.
    case JT808_DEF_PARAM_SLAVE_USERNAME://������������ͨѶ�����û���
    case JT808_DEF_PARAM_SLAVE_PASSWORD://������������ͨѶ��������
    case JT808_DEF_PARAM_SLAVE_ADDRESS://��������IP��ַ��������
	case JT808_DEF_IC_MASTER_ADDRESS://��·����֤ IC ����֤�������� IP ��ַ������  by xlsong
         return 3;//STRING
	case JT808_DEF_IC_TCP_PORT://��·����֤ IC ����֤�������� TCP �˿�
	case JT808_DEF_IC_UDP_PORT://��·����֤ IC ����֤�������� UDP �˿�
		return 2;//DWORD
	case JT808_DEF_IC_SLAVE_ADDRESS://��·����֤ IC ����֤���ݷ����� IP ��ַ���������˿�ͬ��������
		return 3;//STRING
    case JT808_DEF_PARAM_TCP_PORT://������TCP�˿�
    case JT808_DEF_PARAM_UDP_PORT://������UDP�˿�
    case JT808_DEF_PARAM_MASTER_BDID://�������������û���IC������
    case JT808_DEF_PARAM_SLAVE_BDID://�������������û���IC������
    case JT808_DEF_PARAM_REPORT_STRATEGY://λ�û㱨���ԡ�
    case JT808_DEF_PARAM_REPORT_MODE://λ�û㱨������0������Acc״̬��
    case JT808_DEF_PARAM_INTERVAL_NODRIVER://��ʻԱδ��¼�㱨ʱ��������λΪ��S��>0
    case JT808_DEF_PARAM_INTERVAL_RESTING://����ʱ�㱨ʱ��������λΪ��S��>0
    case JT808_DEF_PARAM_INTERVAL_ALARM://��������ʱ�㱨ʱ��������λΪ��S��>0 
    case JT808_DEF_PARAM_INTERVAL_DEFAULT://ȱʡʱ��㱨�������λΪ��S��>0
    case JT808_DEF_PARAM_DISTANCE_DEFAULT://ȱʡ����㱨�������λΪ��m,>0
    case JT808_DEF_PARAM_DISTANCE_NODRIVER://��ʻԱδ��¼�㱨����������λΪ��m��>0
    case JT808_DEF_PARAM_DISTANCE_RESTING://����ʱ�㱨����������λΪ��m��>0 
    case JT808_DEF_PARAM_DISTANCE_ALARM://��������ʱ�㱨����������λΪ��m��>0  
    case JT808_DEF_PARAM_TURNINGANGLE://�յ㲹���Ƕȣ�<180��
         return 2;//DWORD
    case JT808_DEF_PARAM_PHONE_PLATFORM://���ƽ̨�绰����
    case JT808_DEF_PARAM_PHONE_RESET://��λ�绰���롣���ô˺���绰�����ն˵绰���ն˸�
    case JT808_DEF_PARAM_PHONE_RESTORE://�ָ��������õ绰����
    case JT808_DEF_PARAM_PHONE_SMS://���ƽ̨SMS�绰����
    case JT808_DEF_PARAM_PHONE_SMS_SOS://�����ն�SMS�ı���������
        return 3;//STRING
    case JT808_DEF_PARAM_ACCEPT_MODE://�ն˵绰�������ԡ�0����������
    case JT808_DEF_PARAM_TALK_LIMIT_ONCE://ÿ��ͨ���ʱ�䡣��λΪ��S��
    case JT808_DEF_PARAM_TALK_LIMIT_MONTH://�����ͨ��ʱ�䡣��λΪ��S��
        return 2;//DWORD
    case JT808_DEF_PARAM_PHONE_MONITOR://�����绰����
    case JT808_DEF_PARAM_PHONE_PRIVATESMS://���ƽ̨��Ȩ���ź���
        return 3;//STRING
    case JT808_DEF_PARAM_WARN_FLAG_IGNORE://���������֣���λ����Ϣ�㱨��Ϣ�еı�����ʶ
    case JT808_DEF_PARAM_WARN_FLAG_SMS://���������ı�SMS���أ���λ����Ϣ�㱨��Ϣ��
    case JT808_DEF_PARAM_WARN_FLAG_CAMERA://�������㿪�أ���λ����Ϣ�㱨��Ϣ�еı�����־���
    case JT808_DEF_PARAM_WARN_FLAG_SAVE://��������洢��־����λ����Ϣ�㱨��Ϣ�еı�����־
	case JT808_DEF_PARAM_KEY_FLAG://�ؼ���־����λ����Ϣ�㱨��Ϣ�еı�����־���Ӧ����ӦλΪ 1 ����Ӧ����Ϊ�ؼ�����   20130121 add by xlsong
    case JT808_DEF_PARAM_MAX_SPEED://����ٶȣ���λΪǧ��/Сʱ��Km/h��
    case JT808_DEF_PARAM_MAX_OVERSPEED://���ٳ���ʱ�䣬��λΪ��S
    case JT808_DEF_PARAM_MAX_DRIVING_ONCE://������ʻʱ�����ޣ���λΪ��S
    case JT808_DEF_PARAM_MAX_DRIVING_DAY://�����ۼƼ�ʻʱ�����ޣ���λΪ��S
    case JT808_DEF_PARAM_MIN_REST://��С��Ϣʱ�䣬��λΪ��S
    case JT808_DEF_PARAM_MAX_REST://�ͣ��ʱ�䣬��λΪ��S
		return 2;//DWORD
	case JT808_DEF_PARAM_DIF_OVERSPEED://���ٱ���Ԥ����ֵ
	case JT808_DEF_PARAM_DIF_TIREDRIVING://ƣ�ͼ�ʻԤ����ֵ
	case JT808_DEF_PARAM_ALARM_HIT://��ײ������������
	case JT808_DEF_PARAM_ALARM_TURNON://�෭������������
		return 1;//WORD
	case JT808_DEF_PARAM_TIMING_CTRL://��ʱ���տ���
	case JT808_DEF_PARAM_FIXEDDIS_CTRL://�������տ���
    case JT808_DEF_PARAM_MEDIA_QUALITY://ͼ��/��Ƶ������1~10��1Ϊ���
    case JT808_DEF_PARAM_MEDIA_BRIGHTENESS://���ȡ�0~255
    case JT808_DEF_PARAM_MEDIA_CONTRAST://�Աȶȡ�0~127 
    case JT808_DEF_PARAM_MEDIA_SATURATION://���Ͷȡ�0~127
    case JT808_DEF_PARAM_MEDIA_CHROMA://ɫ�ȡ�0~255 
    case JT808_DEF_PARAM_MILEAGE://������̱������1/10Km
        return 2;//DWORD
    case JT808_DEF_PARAM_PROVINCEID://�������ڵ�ʡ��ID
    case JT808_DEF_PARAM_CITYID://�������ڵ�����ID 
        return 1;//WORD
    case JT808_DEF_PARAM_LICENSEPLATE://���ƺ���
        return 3;//STRING
    case JT808_DEF_PARAM_COLOR://������ɫ������JT/T415��2006��5.4.12�Ĺ涨 
	case JT808_DEF_PARAM_GNSS_POSMODE://GNSS ��λģʽ
	case JT808_DEF_PARAM_GNSS_BOUTERAYE://GNSS ������
	case JT808_DEF_PARAM_GNSS_OUTOUT_HZ://GNSS ģ����ϸ��λ�������Ƶ��
		return 0;//BYTE
	case JT808_DEF_PARAM_GNSS_COLLECT_HZ://GNSS ģ����ϸ��λ���ݲɼ�Ƶ��
		return 2;//DWORD
	case JT808_DEF_PARAM_GNSS_UPLOAD_MODE://GNSS ģ����ϸ��λ�����ϴ���ʽ
        return 0;//BYTE
	case JT808_DEF_PARAM_GNSS_UPLOAD_SET://GNSS ģ����ϸ��λ�����ϴ�����
	case JT808_DEF_PARAM_CAN_CHL1_COLLECT://CAN ����ͨ�� 1 �ɼ�ʱ����(ms)
    case JT808_DEF_PARAM_CAN_CHL2_COLLECT://CAN ����ͨ�� 2 �ɼ�ʱ����(ms)
        return 2;
	case JT808_DEF_PARAM_CAN_CHL1_UPLOAD://CAN ����ͨ�� 1 �ϴ�ʱ����(ms)
	case JT808_DEF_PARAM_CAN_CHL2_UPLOAD://CAN ����ͨ�� 2 �ϴ�ʱ����(ms)
		return 1;//WORD
	case JT808_DEF_PARAM_POSI_STANDARD://λ�Ʊ�����׼
	case JT808_DEF_PARAM_TEMP_MIN:				//�¶�����
	case JT808_DEF_PARAM_TEMP_MAX:				//�¶�����
	case JT808_DEF_PARAM_TYRE_STANDARD:			//̥ѹ��׼
		return 1; //WORD
    case JT808_DEF_PARAM_CAMERA_NUMBER://����ͷ��װ��
        return 0; //CHAR
    case JT808_DEF_PARAM_OILBOX_LENGTH://���䳤��
    case JT808_DEF_PARAM_OILBOX_WIDTH://������
    case JT808_DEF_PARAM_OILBOX_HEIGHT://����߶�
    case JT808_DEF_PARAM_OILSENSOR_HEIGHT://��ѹ�������߶�
    case JT808_DEF_PARAM_OILSENSOR_WARNVALUE://��ѹ�쳣�仯��ֵ
        return 1;//WORD
    case JT808_DEF_PARAM_TYRE_TYPE://̥ѹ����������
        return 0;
    default:
        return -1;
    }
}

int JT808_DecodeParameter(uchar *lpBuff, int nBuffLen, TJT808ParameterItem *params, unsigned char nCount)
{
    int nRet;
    int i;
    int nDataType;
    uchar nDataLen;
    uchar *ptr = lpBuff;

    for (i = 0; i < nCount; i++)
    {
        nRet = PT_Decode_INT32(&(params[i].nType), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;
        
        nDataLen = *ptr++;
        nBuffLen--;

        nDataType = JT808_GetParamDataType(params[i].nType);
        if (nDataType == 0 && nDataLen == 1 && nBuffLen >= 1)//BYTE
        {
            params[i].param.nValue = *ptr;
        }
        else if (nDataType == 1 && nDataLen == 2 && nBuffLen >= 2)//WORD
        {
            params[i].param.nValue = 0;
            PT_Decode_SHORT((short *)&(params[i].param.nValue), ptr, nBuffLen);
        }
        else if (nDataType == 2 && nDataLen == 4 && nBuffLen >= 4)//DWORD
        {
            PT_Decode_INT32(&(params[i].param.nValue), ptr, nBuffLen);
        }
        else if (nDataType == 3 && nBuffLen >= nDataLen)//STRING
        {
            memcpy(params[i].param.szValue, ptr, nDataLen);
            params[i].param.szValue[nDataLen] = '\0';
        }
		else if (nDataType == 4 && nBuffLen >= nDataLen)//BYTE[8]
		{
			nRet = PT_Decode_INT32(&(params[i].param.canValue.nInterval), ptr, nBuffLen);
			
            params[i].param.canValue.nChannel = (*(ptr + 4) >> 7) & 0x1;
            params[i].param.canValue.nFrameType = (*(ptr + 4) >> 6) & 0x1;
            params[i].param.canValue.nMode = (*(ptr + 4) >> 5) & 0x1;
            
            params[i].param.canValue.nCanID = (*(ptr + 4)) & 0x1F;
            params[i].param.canValue.nCanID <<= 8;
            params[i].param.canValue.nCanID |= (*(ptr + 5));
            params[i].param.canValue.nCanID <<= 8;
            params[i].param.canValue.nCanID |= (*(ptr + 6));
            params[i].param.canValue.nCanID <<= 8;
            params[i].param.canValue.nCanID |= (*(ptr + 7));
            params[i].param.canValue.bValid = 1;
		}

        ptr += nDataLen;
        nBuffLen -= nDataLen;

        if (nBuffLen < 5)
            break;
    }

    return (ptr - lpBuff);
}

int JT808_EncodeParameter(uchar *lpBuff, int nBuffLen, TJT808ParameterItem *params, unsigned char nCount)
{
    int nRet;
    int i;
    int nDataType;
    uchar *ptr = lpBuff;

    for (i = 0; i < nCount; i++)
    {
        nDataType = JT808_GetParamDataType(params[i].nType);
        if (nDataType < 0)//invalid data type
            continue;

        nRet = PT_Encode_INT32(ptr, nBuffLen, params[i].nType);
        ptr += nRet;
        nBuffLen -= nRet;

        if (nDataType == 0)//BYTE
        {
            *ptr++ = 1;
            nBuffLen--;

            *ptr++ = params[i].param.nValue & 0xFF;
            nBuffLen -= 1;
        }
        else if (nDataType == 1)//WORD
        {
            *ptr++ = 2;
            nBuffLen--;

            nRet = PT_Encode_SHORT(ptr, nBuffLen, params[i].param.nValue);
            ptr += nRet;
            nBuffLen -= nRet;
        }
        else if (nDataType == 2)//DWORD
        {
            *ptr++ = 4;
            nBuffLen--;

            nRet = PT_Encode_INT32(ptr, nBuffLen, params[i].param.nValue);
            ptr += nRet;
            nBuffLen -= nRet;
        }
        else if (nDataType == 3)//STRING
        {
            nRet = strlen(params[i].param.szValue) + 1;
            *ptr++ = nRet;
            nBuffLen--;

            memcpy(ptr, params[i].param.szValue, nRet);
            ptr += nRet;
            nBuffLen -= nRet;
        }
		else if (nDataType == 4)//BYTE[8]
		{
			*ptr++ = 8;
			nBuffLen--;

			nRet = PT_Encode_INT32(ptr, nBuffLen, params[i].param.canValue.nInterval);
			ptr += nRet;
			nBuffLen -= nRet;
            
            *ptr = (params[i].param.canValue.nChannel & 0x1) << 7;
            *ptr |= (params[i].param.canValue.nFrameType & 0x1) << 6;
            *ptr |= (params[i].param.canValue.nMode & 0x1) << 5;
            *ptr |= (params[i].param.canValue.nCanID >> 24) & 0x1F;
            ptr++;
            
            *ptr++ = (params[i].param.canValue.nCanID >> 16) & 0xFF;
            *ptr++ = (params[i].param.canValue.nCanID >> 8) & 0xFF;
            *ptr++ = (params[i].param.canValue.nCanID ) & 0xFF;
            nBuffLen -= 4;
		}
    }

    return (ptr - lpBuff);
}
//�����ն˲���
int JT808_DecodeParameterSet(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808ParameterSet *dat = (TJT808ParameterSet *)pPackage;

    dat->nCount = *ptr++;
    nBuffLen--;

    nRet = JT808_DecodeParameter(ptr, nBuffLen, dat->params, dat->nCount);//20130121 modify by xlsong
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}
//�����ն˲���
int JT808_EncodeParameterSet(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808ParameterSet *dat = (TJT808ParameterSet *)pPackage;

    *ptr++ = dat->nCount;
    nBuffLen--;
    
    nRet = JT808_EncodeParameter(lpBuff, nBuffLen, dat->params, dat->nCount);
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}

int JT808_DecodeParameterQueryReq(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet,i =0,nlen = 0;
	uchar *ptr = lpBuff;

	TJT808ParameterQueryReq *dat = (TJT808ParameterQueryReq *)pPackage;

	dat->ncount = 0;
	nlen = nBuffLen;
	for (i = 0; i < nlen; i++)
	{
		nRet = PT_Decode_INT32(&(dat->nType[i]), ptr, nBuffLen);
		ptr += nRet;
		nBuffLen -= nRet;
		dat->ncount++;
		if (nBuffLen <= 0)
		{
			break;
		}
	}
	return (ptr - lpBuff);
}

int JT808_DecodeParameterQueryRes(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808ParameterQueryRes *dat = (TJT808ParameterQueryRes *)pPackage;

    nRet = PT_Decode_INT32(&(dat->nSeqNum), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    dat->nCount = *ptr++;
    nBuffLen--;

    nRet = JT808_DecodeParameter(lpBuff, nBuffLen, dat->params, dat->nCount);
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}

int JT808_EncodeParameterQueryRes(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808ParameterQueryRes *dat = (TJT808ParameterQueryRes *)pPackage;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nSeqNum);//20130121 modify by xlsong
    ptr += nRet;
    nBuffLen -= nRet;

    *ptr++ = dat->nCount;
    nBuffLen--;

    nRet = JT808_EncodeParameter(ptr, nBuffLen, dat->params, dat->nCount);//20130121 modify by xlsong
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}
//��ѯָ���ն˲��� Ver201301
int JT808_DecodeSpecifyParamQueryReq(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	unsigned int nRet,i =0,nlen = 0;
	uchar *ptr = lpBuff;

	TJT808ParameterQueryReq *dat = (TJT808ParameterQueryReq *)pPackage;

	dat->ncount = *ptr++;
	nBuffLen--;
	for (i = 0; i < dat->ncount; i++)
	{
		nRet = PT_Decode_INT32(&(dat->nType[i]), ptr, nBuffLen);
		ptr += nRet;
		nBuffLen -= nRet;
		if (nBuffLen <= 0)
		{
			break;
		}
	}
	return (ptr - lpBuff);
}
//��ѯָ���ն˲��� Ver201301
int JT808_EncodeSpecifyParamQueryReq(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	unsigned int nRet,i =0,nlen = 0;
	uchar *ptr = lpBuff;

	TJT808ParameterQueryReq *dat = (TJT808ParameterQueryReq *)pPackage;

	*ptr++ = dat->ncount;
	nBuffLen--;
	for (i = 0; i < dat->ncount; i++)
	{
		nRet = PT_Encode_INT32(ptr,nBuffLen,dat->nType[i]);
		ptr += nRet;
		nBuffLen -= nRet;
		if (nBuffLen <= 0)
		{
			break;
		}
	}
	return (ptr - lpBuff);
}
//��ѯ�ն�����   Ver301301
int JT808_DecodePropertyQueryReq(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	uchar *ptr =lpBuff;
	 return (ptr - lpBuff);
}
int JT808_EncodePropertyQueryReq(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	 uchar *ptr =lpBuff;
	 return (ptr - lpBuff);
}
//��ѯ�ն�����Ӧ��  Ver201301
int JT808_DecodePropertyQueryRes(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet = 0,nLen = 0;
	uchar *ptr =lpBuff;
	TJT808PropertyQueryRes *dat = (TJT808PropertyQueryRes *)pPackage;
	
	//�ն�����
	nRet = PT_Decode_SHORT(&(dat->nType), ptr, nBuffLen);
	ptr += nRet;
	nBuffLen -= nRet;

	//������ ID
	memcpy(dat->szManID, ptr, 5);
	ptr += 5;
	nBuffLen -= 5;
	dat->szManID[5] = '\0';

	//�ն��ͺ�
    memcpy(dat->szModel, ptr, 20);
    dat->szModel[19] = '\0';
    ptr += 20;
    nBuffLen -= 20;
	//�ն�ID
	memcpy(dat->szTerminalID, ptr, 7);
	dat->szTerminalID[7] = '\0';
    ptr += 7;
	nBuffLen -= 7;

	//�ն� SIM �� ICCID
	for (nRet = 0; nRet < 20; ptr++)
	{
		if ((*ptr >> 4) >= 0x0 && (*ptr >> 4) <= 0x9)
		{
			dat->szSIMID[nRet++] = '0' + (*ptr >> 4);
		}
		else if ((*ptr >> 4) >= 0xA && (*ptr >> 4) <= 0xF)
		{
			dat->szSIMID[nRet++] = 'A' + (*ptr >> 4);
		}

		if ((*ptr & 0xF) >= 0x0 && (*ptr & 0xF) <= 0x9)
		{
			dat->szSIMID[nRet++] = '0' + (*ptr & 0xF);
		}
		else if ((*ptr & 0xF) >= 0xA && (*ptr & 0xF) <= 0xF)
		{
			dat->szSIMID[nRet++] = 'A' + (*ptr & 0xF);
		}
	}
	nBuffLen -= 10;
	//�ն�Ӳ���汾��
	nLen = *ptr++;
	nBuffLen--;
	memcpy(dat->szHWVerNum,ptr,nLen);
	nBuffLen -= nLen;
	ptr += nLen;
	// �ն˹̼��汾��
	nLen = *ptr++;
	nBuffLen--;
	memcpy(dat->szFWVerNum,ptr,nLen);
	nBuffLen -= nLen;
	ptr += nLen;
	//GNSS ģ������
	dat->nGNSSProp = *ptr++;
	nBuffLen--;
	//ͨ��ģ������
	dat->nCommProp = *ptr++;
	nBuffLen--;
    
	return (ptr - lpBuff);
}
int JT808_EncodePropertyQueryRes(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet, nLen;
	uchar *ptr = lpBuff;
	TJT808PropertyQueryRes *dat = (TJT808PropertyQueryRes *)pPackage;

	//�ն�����
	nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nType);
	ptr += nRet;
	nBuffLen -= nRet;

	//������ ID
	memcpy(ptr, dat->szManID, 5);
	ptr += 5;
	nBuffLen -= 5;

	//�ն��ͺ�
    nLen = strlen(dat->szModel);
    memcpy(ptr, dat->szModel, nLen);
    
    if (nLen < 20)
    {
        memset(ptr + nLen, 0, 20 - nLen);
    }
    
    ptr += 20;
	nBuffLen -= 20;

	//�ն�ID
    nLen = strlen(dat->szTerminalID);
    memcpy(ptr, dat->szTerminalID, nLen);
    
    if (nLen < 7)
    {
        memset(ptr + nLen, 0, 7 - nLen);
    }
    
    ptr += 7;
	nBuffLen -= 7;
	//�ն� SIM �� ICCID
	for (nRet = 0; nRet < 20; nRet++)
	{
		if ((dat->szSIMID[nRet] & 0xF) >= 0x0 && (dat->szSIMID[nRet] & 0xF) <= 0x9)
		{
			*ptr = (dat->szSIMID[nRet]-'0')<<4;
		}
		else if ((dat->szSIMID[nRet] & 0xF) >= 0xA && (dat->szSIMID[nRet]& 0xF) <= 0xF)
		{
			*ptr = (dat->szSIMID[nRet]-'A')<<4;
		}
		nRet++;
		if ((dat->szSIMID[nRet] & 0xF) >= 0x0 && (dat->szSIMID[nRet] & 0xF) <= 0x9)
		{
			*ptr |= dat->szSIMID[nRet]-'0';
		}
		else if ((dat->szSIMID[nRet] & 0xF) >= 0xA && (dat->szSIMID[nRet] & 0xF) <= 0xF)
		{
			*ptr |= dat->szSIMID[nRet]-'A';
		}
		ptr++;
	}
	//Ӳ���汾��
    nLen = strlen(dat->szHWVerNum) + 1;
	*ptr++ = nLen;
	nBuffLen--;
	memcpy(ptr, dat->szHWVerNum, nLen);
	ptr += nLen;
	nBuffLen -= nLen;
	//�̼��汾��
    nLen = strlen(dat->szFWVerNum) + 1;
	*ptr++ = nLen;
	nBuffLen--;
	memcpy(ptr, dat->szFWVerNum, nLen);
	ptr += nLen;
	nBuffLen -= nLen;

	//GNSS ģ������
	*ptr++ = dat->nGNSSProp;
	nBuffLen--;

	//ͨ��ģ������
	*ptr++ = dat->nCommProp;
	nBuffLen--;
	return (ptr - lpBuff);
		
}
//�·��ն�������  Ver301301
int JT808_DecodeUpdatePackageReq(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	unsigned int nRet;
	uchar *ptr = lpBuff;
	TJT808UpdatePackageReq *dat = (TJT808UpdatePackageReq *)pPackage;

    if (dat->head.nPacketSeq == 1)
    {
	    // ��������
	    dat->nType = *ptr++;
	    nBuffLen--;

	    //������ ID
	    memcpy(dat->szManID, ptr, 5);
	    ptr += 5;
	    nBuffLen -= 5;
	    dat->szManID[5] = '\0';

	    //�汾��
	    nRet = *ptr++;
	    nBuffLen--;

	    memcpy(dat->szVerNum, ptr, nRet);
        dat->szVerNum[nRet] = '\0';
	    nBuffLen -= nRet;
	    ptr += nRet;

        //�������ݰ�����
        nRet = PT_Decode_INT32(&dat->nPackageLen, ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;
    }
    else
    {
        dat->nType = 0;
        dat->szManID[0] = '\0';
        dat->szVerNum[0] = '\0';
        dat->nPackageLen = 0;
    }
	
	if (nBuffLen > sizeof(dat->data))
		dat->nDataLen = sizeof(dat->data);
    else
        dat->nDataLen = nBuffLen;

	memcpy(dat->data,ptr,dat->nDataLen);
	nBuffLen -= dat->nDataLen;
	ptr += dat->nDataLen;

	return (ptr - lpBuff);
}
//�·��ն�������  Ver301301
int JT808_EncodeUpdatePackageReq(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	unsigned int nRet;
	uchar *ptr = lpBuff;
	TJT808UpdatePackageReq *dat = (TJT808UpdatePackageReq *)pPackage;

    if (dat->head.nPacketSeq == 1)
    {
	    //��������
	    *ptr++ = dat->nType;
	    nBuffLen--;
    	
	    //������ ID
	    memcpy(ptr, dat->szManID, 5);
	    ptr += 5;
	    nBuffLen -= 5;

	    //�汾��
        nRet = strlen(dat->szVerNum) + 1;
	    *ptr++ = nRet;
	    nBuffLen--;

	    memcpy(ptr, dat->szVerNum, nRet);
	    ptr += nRet;
	    nBuffLen -= nRet;

        //�������ݰ�����
        nRet = PT_Encode_INT32(ptr, nBuffLen, dat->nPackageLen);
        nBuffLen -= nRet;
        ptr += nRet;
    }

    if (dat->nDataLen > 0)
    {
	    memcpy(ptr, dat->data, dat->nDataLen);
	    nBuffLen -= dat->nDataLen;
	    ptr += dat->nDataLen;
    }

	return (ptr - lpBuff);
}
//�ն��������֪ͨ  Ver301301
int JT808_DecodeUpdatePackageRes(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	uchar *ptr = lpBuff;
	TJT808UpdatePackageRes *dat = (TJT808UpdatePackageRes *)pPackage;

	// ��������
	dat->nUpdateType = *ptr++;
	nBuffLen--;

	//������� 0���ɹ���1��ʧ�ܣ�2��ȡ��
	dat->nResult = *ptr++;
	nBuffLen--;

	return (ptr - lpBuff);
}
//�ն��������֪ͨ  Ver301301
int JT808_EncodeUpdatePackageRes(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	uchar *ptr = lpBuff;
	TJT808UpdatePackageRes *dat = (TJT808UpdatePackageRes *)pPackage;

	// ��������
	*ptr++ = dat->nUpdateType;
	nBuffLen--;

	//������� 0���ɹ���1��ʧ�ܣ�2��ȡ��
	*ptr++ = dat->nResult;
	nBuffLen--;

	return (ptr - lpBuff);
}
int JT808_DecodeTerminalCtrl(void *pPackage, uchar *lpBuff, int nBuffLen)//20130121 modify by xlsong
{
    TPackage pkg;
    uchar *ptr =lpBuff;
	int i = 0;
    TJT808TerminalCtrl *dat = (TJT808TerminalCtrl *)pPackage;

    dat->nCMD = *ptr++;
    nBuffLen--;

    if (dat->nCMD == 1)//��������
    {
		PT_SplitStringEx2(&pkg, ptr, nBuffLen, ";",5);
        if (pkg.nItems >= 5)
        {
            PT_DecodeString(dat->cmdinfo.updateinfo.szURL, 
                sizeof(dat->cmdinfo.updateinfo.szURL), pkg.pItem[0]);
            PT_DecodeString(dat->cmdinfo.updateinfo.conninfo.szAPN, 
                sizeof(dat->cmdinfo.updateinfo.conninfo.szAPN), pkg.pItem[1]);
            PT_DecodeString(dat->cmdinfo.updateinfo.conninfo.szUser, 
                sizeof(dat->cmdinfo.updateinfo.conninfo.szUser), pkg.pItem[2]);
            PT_DecodeString(dat->cmdinfo.updateinfo.conninfo.szPassword, 
                sizeof(dat->cmdinfo.updateinfo.conninfo.szPassword), pkg.pItem[3]);
            PT_DecodeString(dat->cmdinfo.updateinfo.conninfo.szAddress, 
                sizeof(dat->cmdinfo.updateinfo.conninfo.szAddress), pkg.pItem[4]);
			
			i = 0;
			dat->cmdinfo.updateinfo.conninfo.nTCPPort = pkg.pItem[5][i++]&0xFF;
			dat->cmdinfo.updateinfo.conninfo.nTCPPort <<= 8;
			dat->cmdinfo.updateinfo.conninfo.nTCPPort |= pkg.pItem[5][i++]&0xFF;
			i++;
			dat->cmdinfo.updateinfo.conninfo.nUDPPort = pkg.pItem[5][i++]&0xFF;
			dat->cmdinfo.updateinfo.conninfo.nUDPPort <<= 8;
			dat->cmdinfo.updateinfo.conninfo.nUDPPort |= pkg.pItem[5][i++]&0xFF;
			i++;
			nBuffLen -=(pkg.pItem[5]-ptr+i); 
			ptr = &pkg.pItem[5][i];
			PT_SplitStringEx2(&pkg, ptr, nBuffLen, ";",3);

			PT_DecodeString(dat->cmdinfo.updateinfo.ManID, 
				sizeof(dat->cmdinfo.updateinfo.ManID), pkg.pItem[0]);
			PT_DecodeString(dat->cmdinfo.updateinfo.szHardwareVersion, 
				sizeof(dat->cmdinfo.updateinfo.szHardwareVersion), pkg.pItem[1]);
			PT_DecodeString(dat->cmdinfo.updateinfo.szSoftwareVersion, 
				sizeof(dat->cmdinfo.updateinfo.szSoftwareVersion), pkg.pItem[2]);
			i = 0;
			dat->cmdinfo.updateinfo.nTimeLimit = pkg.pItem[3][i++]&0xFF;
			dat->cmdinfo.updateinfo.nTimeLimit <<= 8;
			dat->cmdinfo.updateinfo.nTimeLimit |= pkg.pItem[3][i++]&0xFF;
			i++;
			nBuffLen -=(pkg.pItem[3]-ptr+i); 
			ptr = &pkg.pItem[3][i];	
			PT_SplitStringEx2(&pkg, ptr, nBuffLen, ";",1);
			if (nBuffLen >= 0)
			{
				dat->cmdinfo.updateinfo.conninfo.nBDID = atoi(pkg.pItem[0]);
			}
			else
			{
				dat->cmdinfo.updateinfo.conninfo.nBDID = 0;
			}
        }
        else
        {
            return -1;
        }
    }
    else if (dat->nCMD == 2)//�����ն�����ָ��������
    {
		dat->cmdinfo.platform.nCtrl = *ptr++;//���ӿ���
		nBuffLen--;
		*ptr++;
		nBuffLen--;
		if(dat->cmdinfo.platform.nCtrl == 0)
		{
			PT_SplitStringEx2(&pkg, ptr, nBuffLen, ";",5);
			if (pkg.nItems>5)
			{
				PT_DecodeString(dat->cmdinfo.platform.szAuthcode, 
					sizeof(dat->cmdinfo.platform.szAuthcode), pkg.pItem[0]);
				PT_DecodeString(dat->cmdinfo.platform.conninfo.szAPN, 
					sizeof(dat->cmdinfo.platform.conninfo.szAPN), pkg.pItem[1]);
				PT_DecodeString(dat->cmdinfo.platform.conninfo.szUser, 
					sizeof(dat->cmdinfo.platform.conninfo.szUser), pkg.pItem[2]);
				PT_DecodeString(dat->cmdinfo.platform.conninfo.szPassword, 
					sizeof(dat->cmdinfo.platform.conninfo.szPassword), pkg.pItem[3]);
				PT_DecodeString(dat->cmdinfo.platform.conninfo.szAddress, 
					sizeof(dat->cmdinfo.platform.conninfo.szAddress), pkg.pItem[4]);
				i = 0;
				dat->cmdinfo.platform.conninfo.nTCPPort = pkg.pItem[5][i++]&0xFF;
				dat->cmdinfo.platform.conninfo.nTCPPort <<= 8;
				dat->cmdinfo.platform.conninfo.nTCPPort |= pkg.pItem[5][i++]&0xFF;
				i++;
				dat->cmdinfo.platform.conninfo.nUDPPort = pkg.pItem[5][i++]&0xFF;
				dat->cmdinfo.platform.conninfo.nUDPPort <<= 8;
				dat->cmdinfo.platform.conninfo.nUDPPort |= pkg.pItem[5][i++]&0xFF;
				i++;
				dat->cmdinfo.platform.nTimeLimit = pkg.pItem[5][i++]&0xFF;
				dat->cmdinfo.platform.nTimeLimit <<= 8;
				dat->cmdinfo.platform.nTimeLimit |= pkg.pItem[5][i++]&0xFF;
				i++;
				nBuffLen -=(pkg.pItem[5]-ptr+i); 
				if (nBuffLen > 0)
				{
					ptr = &pkg.pItem[5][i];	
					PT_SplitStringEx2(&pkg, ptr, nBuffLen, ";",1);
					dat->cmdinfo.platform.conninfo.nBDID = atoi(pkg.pItem[0]);
				}
				else
				{
					dat->cmdinfo.platform.conninfo.nBDID = 0;
				}			

			}
			else
			{
				return -1;
			}
		}
    }

    return (ptr - lpBuff);
}

int JT808_EncodeTerminalCtrl(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808TerminalCtrl *dat = (TJT808TerminalCtrl *)pPackage;

    *ptr++ = dat->nCMD;
    nBuffLen--;
    
    if (dat->nCMD == 1)//��������
    {
        nRet = strlen(dat->cmdinfo.updateinfo.szURL);
        memcpy(ptr, dat->cmdinfo.updateinfo.szURL, nRet);
        ptr += nRet;
        nBuffLen -= nRet;

        *ptr++ = ';';
        nBuffLen--;

        nRet = strlen(dat->cmdinfo.updateinfo.conninfo.szAPN);
        memcpy(ptr, dat->cmdinfo.updateinfo.conninfo.szAPN, nRet);
        ptr += nRet;
        nBuffLen -= nRet;

        *ptr++ = ';';
        nBuffLen--;

        nRet = strlen(dat->cmdinfo.updateinfo.conninfo.szUser);
        memcpy(ptr, dat->cmdinfo.updateinfo.conninfo.szUser, nRet);
        ptr += nRet;
        nBuffLen -= nRet;

        *ptr++ = ';';
        nBuffLen--;

        nRet = strlen(dat->cmdinfo.updateinfo.conninfo.szPassword);
        memcpy(ptr, dat->cmdinfo.updateinfo.conninfo.szPassword, nRet);
        ptr += nRet;
        nBuffLen -= nRet;

        *ptr++ = ';';
        nBuffLen--;

        nRet = strlen(dat->cmdinfo.updateinfo.conninfo.szAddress);
        memcpy(ptr, dat->cmdinfo.updateinfo.conninfo.szAddress, nRet);
        ptr += nRet;
        nBuffLen -= nRet;//20130121 modify by xlsong     δ��֤������

		*ptr++ = (dat->cmdinfo.updateinfo.conninfo.nTCPPort>>8)&&0x00ff;
		nBuffLen--;
		*ptr++ = dat->cmdinfo.updateinfo.conninfo.nTCPPort&&0x00ff;
		nBuffLen--;
		*ptr++ = ';';
		nBuffLen--;
		*ptr++ = (dat->cmdinfo.updateinfo.conninfo.nUDPPort>>8)&&0x00ff;
		nBuffLen--;
		*ptr++ = dat->cmdinfo.updateinfo.conninfo.nUDPPort&&0x00ff;
		nBuffLen--;
		*ptr++ = ';';
		nBuffLen--;
		/*
        nRet = sprintf(ptr, ";%d;%d;", dat->cmdinfo.updateinfo.conninfo.nTCPPort,
            dat->cmdinfo.updateinfo.conninfo.nUDPPort);
        ptr += nRet;
        nBuffLen -= nRet;
		*/
		nRet = strlen(dat->cmdinfo.updateinfo.ManID);
		memcpy(ptr, dat->cmdinfo.updateinfo.ManID, nRet);
		ptr += nRet;
		nBuffLen -= nRet;
		/*
        *ptr++ = dat->cmdinfo.updateinfo.ManID[0] + 0x30;
        *ptr++ = dat->cmdinfo.updateinfo.ManID[1] + 0x30;
        *ptr++ = dat->cmdinfo.updateinfo.ManID[2] + 0x30;
        *ptr++ = dat->cmdinfo.updateinfo.ManID[3] + 0x30;
        *ptr++ = dat->cmdinfo.updateinfo.ManID[4] + 0x30;
        nBuffLen -= 5;
		*/
        nRet = strlen(dat->cmdinfo.updateinfo.szHardwareVersion);
        memcpy(ptr, dat->cmdinfo.updateinfo.szHardwareVersion, nRet);
        ptr += nRet;
        nBuffLen -= nRet;

        *ptr++ = ';';
        nBuffLen--;

        nRet = strlen(dat->cmdinfo.updateinfo.szSoftwareVersion);
        memcpy(ptr, dat->cmdinfo.updateinfo.szSoftwareVersion, nRet);
        ptr += nRet;
        nBuffLen -= nRet;

		*ptr++ = (dat->cmdinfo.updateinfo.nTimeLimit>>8)&&0x00ff;
		nBuffLen--;
		*ptr++ = dat->cmdinfo.updateinfo.nTimeLimit&&0x00ff;
		nBuffLen--;
		if (dat->cmdinfo.updateinfo.conninfo.nBDID > 0)
		{				
			*ptr++ = ';';
			nBuffLen--;		
			*ptr++ = (dat->cmdinfo.updateinfo.conninfo.nBDID>>24)&&0x00ff;
			nBuffLen--;
			*ptr++ = (dat->cmdinfo.updateinfo.conninfo.nBDID>>16)&&0x00ff;
			nBuffLen--;
			*ptr++ = (dat->cmdinfo.updateinfo.conninfo.nBDID>>8)&&0x00ff;
			nBuffLen--;
			*ptr++ = dat->cmdinfo.updateinfo.conninfo.nBDID&&0x00ff;
			nBuffLen--;
		}
		/*
        nRet = sprintf(ptr, ";%d;%d", dat->cmdinfo.updateinfo.nTimeLimit, dat->cmdinfo.updateinfo.conninfo.nBDID);
        ptr += nRet;
        nBuffLen -= nRet;
		*/
    }
    else if (dat->nCMD == 2)//�����ն�����ָ��������
    {
        nRet = sprintf(ptr, "%d", dat->cmdinfo.platform.nCtrl);
        ptr += nRet;
        nBuffLen -= nRet;

        if (dat->cmdinfo.platform.nCtrl == 0)//�л���ָ�����ƽ̨������,
        {
            *ptr++ = ';';
            nBuffLen--;

            nRet = strlen(dat->cmdinfo.platform.szAuthcode);
            memcpy(ptr, dat->cmdinfo.platform.szAuthcode, nRet);
            ptr += nRet;
            nBuffLen -= nRet;

            *ptr++ = ';';
            nBuffLen--;

            nRet = strlen(dat->cmdinfo.platform.conninfo.szAPN);
            memcpy(ptr, dat->cmdinfo.platform.conninfo.szAPN, nRet);
            ptr += nRet;
            nBuffLen -= nRet;

            *ptr++ = ';';
            nBuffLen--;

            nRet = strlen(dat->cmdinfo.platform.conninfo.szUser);
            memcpy(ptr, dat->cmdinfo.platform.conninfo.szUser, nRet);
            ptr += nRet;
            nBuffLen -= nRet;

            *ptr++ = ';';
            nBuffLen--;

            nRet = strlen(dat->cmdinfo.platform.conninfo.szPassword);
            memcpy(ptr, dat->cmdinfo.platform.conninfo.szPassword, nRet);
            ptr += nRet;
            nBuffLen -= nRet;

            *ptr++ = ';';
            nBuffLen--;

            nRet = strlen(dat->cmdinfo.platform.conninfo.szAddress);
            memcpy(ptr, dat->cmdinfo.platform.conninfo.szAddress, nRet);
            ptr += nRet;
            nBuffLen -= nRet;

			*ptr++ = (dat->cmdinfo.platform.conninfo.nTCPPort>>8)&&0x00ff;
			nBuffLen--;
			*ptr++ = dat->cmdinfo.platform.conninfo.nTCPPort&&0x00ff;
			nBuffLen--;
			*ptr++ = ';';
			nBuffLen--;
			*ptr++ = (dat->cmdinfo.platform.conninfo.nUDPPort>>8)&&0x00ff;
			nBuffLen--;
			*ptr++ = dat->cmdinfo.platform.conninfo.nUDPPort&&0x00ff;
			nBuffLen--;
			*ptr++ = ';';
			*ptr++ = (dat->cmdinfo.platform.nTimeLimit>>8)&&0x00ff;
			nBuffLen--;
			*ptr++ = dat->cmdinfo.platform.nTimeLimit&&0x00ff;
			nBuffLen--;
			*ptr++ = ';';
			nBuffLen--;		
			*ptr++ = (dat->cmdinfo.platform.conninfo.nBDID>>24)&&0x00ff;
			nBuffLen--;
			*ptr++ = (dat->cmdinfo.platform.conninfo.nBDID>>16)&&0x00ff;
			nBuffLen--;
			*ptr++ = (dat->cmdinfo.platform.conninfo.nBDID>>8)&&0x00ff;
			nBuffLen--;
			*ptr++ = dat->cmdinfo.platform.conninfo.nBDID&&0x00ff;
			nBuffLen--;
			/*
            nRet = sprintf(ptr, ";%d;%d;%d;%d", dat->cmdinfo.platform.conninfo.nTCPPort,
                dat->cmdinfo.platform.conninfo.nUDPPort, dat->cmdinfo.platform.nTimeLimit,
                dat->cmdinfo.platform.conninfo.nBDID);

            ptr += nRet;
            nBuffLen -= nRet;
			*/
        }
    }

    return (ptr - lpBuff);
}

int JT808_DecodePositionState(TJT808PosiState *pos, uchar *lpBuff, int nBuffLen)
{
    unsigned int nRet;
	unsigned char nType;
    unsigned char i;
    int nValue;
    unsigned char nUserDefLen = 0;
    uchar *ptr = lpBuff;

    nRet = PT_Decode_INT32(&(pos->nWarnFlag), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Decode_INT32(&(pos->nState),ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Decode_INT32(&nValue, ptr, nBuffLen);
    pos->dblLatitude = nValue / JT808_MILLION;
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Decode_INT32(&nValue, ptr, nBuffLen);
    pos->dblLongitude = nValue / JT808_MILLION;
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Decode_SHORT(&(pos->nHigh), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    nValue = 0;
    nRet = PT_Decode_SHORT((unsigned short *)&nValue, ptr, nBuffLen);
    pos->dblSpeed = nValue / 10.0;
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Decode_SHORT(&(pos->nDeriction), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    pos->nYear = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF) + 2000;
	ptr++;

    pos->nMonth = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);
	ptr++;

    pos->nDay = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);
	ptr++;

    pos->nHour = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);
	ptr++;

    pos->nMinute = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);
	ptr++;

    pos->nSecond = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);
	ptr++;

    nBuffLen -= 6;

    return (ptr - lpBuff);
}

int JT808_EncodePositionState(uchar *lpBuff, int nBuffLen, TJT808PosiState *pos)
{
    int nRet;
    unsigned int nValue;
    uchar *ptr = lpBuff;

    nRet = PT_Encode_INT32(ptr, nBuffLen, pos->nWarnFlag);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Encode_INT32(ptr, nBuffLen, pos->nState);
    ptr += nRet;
    nBuffLen -= nRet;

    nValue = (unsigned int)(pos->dblLatitude * JT808_MILLION);
    nRet = PT_Encode_INT32(ptr, nBuffLen, nValue);
    ptr += nRet;
    nBuffLen -= nRet;

    nValue = (unsigned int)(pos->dblLongitude * JT808_MILLION);
    nRet = PT_Encode_INT32(ptr, nBuffLen, nValue);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, pos->nHigh);
    ptr += nRet;
    nBuffLen -= nRet;

    nValue = (unsigned short)(pos->dblSpeed * 10);
    nRet = PT_Encode_SHORT(ptr, nBuffLen, (unsigned short)nValue);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, pos->nDeriction);
    ptr += nRet;
    nBuffLen -= nRet;

	*ptr++ = (((pos->nYear - 2000) / 10) << 4) | (((pos->nYear - 2000) % 10) & 0x0F);
    *ptr++ = ((pos->nMonth / 10) << 4) | ((pos->nMonth % 10) & 0x0F);
	*ptr++ = ((pos->nDay / 10) << 4) | ((pos->nDay % 10) & 0x0F);
	*ptr++ = ((pos->nHour / 10) << 4) | ((pos->nHour % 10) & 0x0F);
	*ptr++ = ((pos->nMinute / 10) << 4) | ((pos->nMinute % 10) & 0x0F);
	*ptr++ = ((pos->nSecond / 10) << 4) | ((pos->nSecond % 10) & 0x0F);

    nBuffLen -= 6;
	
    return (ptr - lpBuff);
}

int JT808_DecodePosition(TJT808PositionInfo *pos, uchar *lpBuff, int nBuffLen)
{
    unsigned int nRet;
	unsigned char nType;
    unsigned char i;
    int nValue;
    unsigned char nUserDefLen = 0;
    uchar *ptr = lpBuff;

    nRet = PT_Decode_INT32(&(pos->nWarnFlag), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Decode_INT32(&(pos->nState),ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Decode_INT32(&nValue, ptr, nBuffLen);
    pos->dblLatitude = nValue / JT808_MILLION;
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Decode_INT32(&nValue, ptr, nBuffLen);
    pos->dblLongitude = nValue / JT808_MILLION;
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Decode_SHORT(&(pos->nHigh), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    nValue = 0;
    nRet = PT_Decode_SHORT((unsigned short *)&nValue, ptr, nBuffLen);
    pos->dblSpeed = nValue / 10.0;
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Decode_SHORT(&(pos->nDeriction), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    pos->nYear = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF) + 2000;
	ptr++;

    pos->nMonth = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);
	ptr++;

    pos->nDay = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);
	ptr++;

    pos->nHour = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);
	ptr++;

    pos->nMinute = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);
	ptr++;

    pos->nSecond = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);
	ptr++;

    nBuffLen -= 6;

    pos->addininfo.nFlag = 0;
    while (nBuffLen > 2) //������Ϣ
    {
        nType = *ptr++;
        nValue = *ptr++;//��Ϣ�����ֽ�
        nBuffLen -= 2;

        if (nType == 0x01 && nBuffLen >= nValue && nValue == 4)//���
        {
            pos->addininfo.nFlag |= JT808_DEF_INFO_MILEAGE;
            nRet = PT_Decode_INT32(&(pos->addininfo.nMileage), ptr, nBuffLen);
            ptr += nRet;
            nBuffLen -= nRet;
        }
        else if (nType == 0x02 && nBuffLen >= nValue && nValue == 2)//����
        {
            pos->addininfo.nFlag |= JT808_DEF_INFO_OIL;
            nRet = PT_Decode_SHORT(&(pos->addininfo.nOil), ptr, nBuffLen);
            ptr += nRet;
            nBuffLen -= nRet;
        }
        else if (nType == 0x03 && nBuffLen >= nValue && nValue == 2)//��¼���ٶ�
        {
            pos->addininfo.nFlag |= JT808_DEF_INFO_SPEED;
            nRet = PT_Decode_SHORT(&(pos->addininfo.nSpeed), ptr, nBuffLen);
            pos->addininfo.nSpeed /= 10;
            ptr += nRet;
            nBuffLen -= nRet;
        }
		else if (nType == 0x04 && nBuffLen >= nValue && nValue == 2)//��Ҫ�˹�ȷ�ϱ����¼���ID
		{
			pos->addininfo.nFlag |= JT808_DEF_INFO_EVENTID;
			nRet = PT_Decode_SHORT(&(pos->addininfo.nEventID), ptr, nBuffLen);
			ptr += nRet;
			nBuffLen -= nRet;
		}
        else if (nType == 0x11 && nBuffLen >= nValue && (nValue == 1 || nValue == 5))//���ٱ���������Ϣ
        {
            pos->addininfo.nFlag |= JT808_DEF_INFO_OVERSPEED;

            pos->addininfo.overspeedinfo.nPositionType = *ptr++;//modify by xlsong
            nBuffLen--;

            if (pos->addininfo.overspeedinfo.nPositionType > 0 && nBuffLen >= 4)
            {
                nRet = PT_Decode_INT32(&(pos->addininfo.overspeedinfo.nAreaID), ptr, nBuffLen);
                ptr += nRet;
                nBuffLen -= nRet;
            }
        }
        else if (nType == 0x12 && nBuffLen >= nValue && nValue == 6)//��������/·�߱���������Ϣ
        {
            pos->addininfo.nFlag |= JT808_DEF_INFO_AREA_ROUTE;

            pos->addininfo.areainfo.nPositionType = *ptr++;
            nBuffLen--;

            nRet = PT_Decode_INT32(&(pos->addininfo.areainfo.nAreaID), ptr, nBuffLen);
            ptr += nRet;
            nBuffLen -= nRet;

            pos->addininfo.areainfo.nDeriction = *ptr++;
            nBuffLen--;
        }
        else if (nType == 0x13 && nBuffLen >= nValue && nValue == 7)//·����ʻʱ�䲻��/��������������Ϣ
        {
            pos->addininfo.nFlag |= JT808_DEF_INFO_AREA_ROUTE;

            nRet = PT_Decode_INT32(&(pos->addininfo.drivinginfo.nAreaID), ptr, nBuffLen);
            ptr += nRet;
            nBuffLen -= nRet;

            nRet = PT_Decode_SHORT(&(pos->addininfo.drivinginfo.nTime), ptr, nBuffLen);
            ptr += nRet;
            nBuffLen -= nRet;

            pos->addininfo.drivinginfo.nResult = *ptr++;
            nBuffLen--;
        }
		else if (nType == 0x25 && nBuffLen >= nValue && nValue == 4)//��չ�����ź�״̬λ
		{
			pos->addininfo.nFlag |= JT808_DEF_INFO_CARSIGNAL;

			nRet = PT_Decode_INT32(&(pos->addininfo.nCarSignalStatus), ptr, nBuffLen);
			ptr += nRet;
			nBuffLen -= nRet;
		}
		else if (nType == 0x2A && nBuffLen >= nValue && nValue == 2)//IO״̬λ
		{
			pos->addininfo.nFlag |= JT808_DEF_INFO_IOSTATUS;
			nRet = PT_Decode_SHORT(&(pos->addininfo.nIoStatus), ptr, nBuffLen);
			ptr += nRet;
			nBuffLen -= nRet;		
		}
		else if (nType == 0x2B && nBuffLen >= nValue && nValue == 4)////ģ����
		{
			pos->addininfo.nFlag |= JT808_DEF_INFO_AD;
			nRet = PT_Decode_INT32(&(pos->addininfo.nAnalogData), ptr, nBuffLen);
			ptr += nRet;
			nBuffLen -= nRet;
		}
		else if (nType == 0x30 && nBuffLen >= nValue && nValue == 1)//0x30 ����ͨ�������ź�ǿ��
		{
			pos->addininfo.nFlag |= JT808_DEF_INFO_WLSIGNAL;
			pos->addininfo.nWLSignal = *ptr++; 
			nBuffLen--;
		}
		else if (nType == 0x31 && nBuffLen >= nValue && nValue == 1)//0x31 GNSS ��λ������
		{
			pos->addininfo.nFlag |= JT808_DEF_INFO_GNSSSTARS;
			pos->addininfo.nGnssStars = *ptr++; 
			nBuffLen--;
		}
        else if (nType == 0xE0 && nBuffLen >= nValue )//�Զ�����Ϣ����
        {
            nUserDefLen = nValue;
            break;
        }
		else//unknown
		{
			nBuffLen -= nValue;	
		}
    }
    
    //�Զ�����Ϣ
    pos->userdef.nFlag = 0;
    while (nBuffLen > 0)
    {
        nType = *ptr++;
        nBuffLen--;
        
        if (nType == 0xF0 && nBuffLen > 0)//��ѹ
        {
            nValue = *ptr++;
            nBuffLen--;

            pos->userdef.nOilCount = 0;
            while (nValue--)
            {
                if (nBuffLen >= 3)
                {
                    if (pos->userdef.nOilCount < JT808_DEF_MAX_NUM_SENSOR_OIL)
                    {
                        nRet = PT_Decode_SHORT(&(pos->userdef.oils[pos->userdef.nOilCount]), ptr, nBuffLen);
                        ptr += nRet;
                        nBuffLen -= nRet;

                        pos->userdef.oilsstate[pos->userdef.nOilCount] = *ptr++;
                        nBuffLen--;

                        pos->userdef.nOilCount++;
                    }
                    else
                    {
                        nBuffLen -= 3;
                    }
                }
            }

            if (pos->userdef.nOilCount > 0)
            {
                pos->userdef.nFlag |= JT808_DEF_INFO1_OIL;
            }
        }
        else if (nType == 0xF1 && nBuffLen > 0)//̥ѹ�ɰ棬����λ��ʶ
        {
            nValue = *ptr++;
            nBuffLen--;

            pos->userdef.nTyreCount = 0;
            while (nValue--)
            {
                if (nBuffLen >= 5)
                {
                    if (pos->userdef.nTyreCount < JT808_DEF_MAX_NUM_SENSOR_TYRE)
                    {
                        nRet = PT_Decode_SHORT(&(pos->userdef.tyres[pos->userdef.nTyreCount].nPressure), ptr, nBuffLen);
                        ptr += nRet;
                        nBuffLen -= nRet;

                        nRet = PT_Decode_SHORT(&(pos->userdef.tyres[pos->userdef.nTyreCount].nTemp), ptr, nBuffLen);
                        ptr += nRet;
                        nBuffLen -= nRet;

                        pos->userdef.tyres[pos->userdef.nTyreCount].nState = *ptr++;
                        nBuffLen--;

                        pos->userdef.nTyreCount++;
                    }
                    else
                    {
                        nBuffLen -= 5;
                    }
                }
            }

            if (pos->userdef.nTyreCount > 0)
            {
                pos->userdef.nFlag |= JT808_DEF_INFO1_TYRE;
            }
        }
        else if (nType == 0xF2 && nBuffLen > 0)//�¶�
        {
            nValue = *ptr++;
            nBuffLen--;

            pos->userdef.nTempCount = 0;
            while (nValue--)
            {
                if (nBuffLen >= 3)
                {
                    if (pos->userdef.nTempCount < JT808_DEF_MAX_NUM_SENSOR_TEMP)
                    {
                        nRet = PT_Decode_SHORT(&(pos->userdef.temps[pos->userdef.nTempCount]), ptr, nBuffLen);
                        ptr += nRet;
                        nBuffLen -= nRet;

                        pos->userdef.tempsstate[pos->userdef.nTempCount] = *ptr++;
                        nBuffLen--;

                        pos->userdef.nTempCount++;
                    }
                    else
                    {
                        nBuffLen--;
                    }
                }
            }

            if (pos->userdef.nTempCount > 0)
            {
                pos->userdef.nFlag |= JT808_DEF_INFO1_TEMP;
            }
        }
        else if (nType == 0xF3 && nBuffLen > 0)//λ��
        {
            nValue = *ptr++;
            nBuffLen--;

            pos->userdef.nPosiCount = 0;
            while (nValue--)
            {
                if (nBuffLen >= 3)
                {
                    if (pos->userdef.nPosiCount < JT808_DEF_MAX_NUM_SENSOR_POSI)
                    {
                        nRet = PT_Decode_SHORT(&(pos->userdef.positions[pos->userdef.nPosiCount]), ptr, nBuffLen);
                        ptr += nRet;
                        nBuffLen -= nRet;

                        pos->userdef.posistate[pos->userdef.nPosiCount] = *ptr++;
                        nBuffLen--;

                        pos->userdef.nPosiCount++;
                    }
                    else
                    {
                        nBuffLen-= 3;
                    }
                }
            }

            if (pos->userdef.nPosiCount > 0)
            {
                pos->userdef.nFlag |= JT808_DEF_INFO1_POSI;
            }
        }
        else if (nType == 0xF4 && nBuffLen > 0)//�ػ�״̬
        {
            pos->userdef.nFlag |= JT808_DEF_INFO1_CARRY;
            pos->userdef.nCarryState = *ptr++;
            nBuffLen--;
        }
        else if (nType == 0xF5 && nBuffLen > 0)//̥ѹ�°棬������λ��ʶ
        {
            nValue = *ptr++;
            nBuffLen--;

            pos->userdef.nTyreCount = 0;
            while (nValue--)
            {
                if (nBuffLen >= 6)
                {
                    if (pos->userdef.nTyreCount < JT808_DEF_MAX_NUM_SENSOR_TYRE)
                    {
                        pos->userdef.tyres[pos->userdef.nTyreCount].nIndex = *ptr++;
                        nBuffLen--;

                        nRet = PT_Decode_SHORT(&(pos->userdef.tyres[pos->userdef.nTyreCount].nPressure), ptr, nBuffLen);
                        ptr += nRet;
                        nBuffLen -= nRet;

                        nRet = PT_Decode_SHORT(&(pos->userdef.tyres[pos->userdef.nTyreCount].nTemp), ptr, nBuffLen);
                        ptr += nRet;
                        nBuffLen -= nRet;

                        pos->userdef.tyres[pos->userdef.nTyreCount].nState = *ptr++;
                        nBuffLen--;

                        pos->userdef.nTyreCount++;
                    }
                    else
                    {
                        nBuffLen -= 6;
                    }
                }
            }

            if (pos->userdef.nTyreCount > 0)
            {
                pos->userdef.nFlag |= JT808_DEF_INFO1_TYRE;
            }
        }
        else if (nType == 0xFF && nBuffLen >= 4)//��������ʶ��Ϣ,�Զ��帽����Ϣ
        {
           pos->userdef.nFlag |= JT808_DEF_INFO1_SERVERFLAG;

           nRet = PT_Decode_INT32(&(pos->userdef.nServerFlag), ptr, nBuffLen);
           ptr += nRet;
           nBuffLen -= nRet;
        }
    }
    return (ptr - lpBuff);
}

int JT808_EncodePosition(uchar *lpBuff, int nBuffLen, TJT808PositionInfo *pos)
{
    int nRet;
    unsigned int nValue;
    uchar *ptr = lpBuff;

    nRet = PT_Encode_INT32(ptr, nBuffLen, pos->nWarnFlag);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Encode_INT32(ptr, nBuffLen, pos->nState);
    ptr += nRet;
    nBuffLen -= nRet;

    nValue = (unsigned int)(pos->dblLatitude * JT808_MILLION);
    nRet = PT_Encode_INT32(ptr, nBuffLen, nValue);
    ptr += nRet;
    nBuffLen -= nRet;

    nValue = (unsigned int)(pos->dblLongitude * JT808_MILLION);
    nRet = PT_Encode_INT32(ptr, nBuffLen, nValue);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, pos->nHigh);
    ptr += nRet;
    nBuffLen -= nRet;

    nValue = (unsigned short)(pos->dblSpeed * 10);
    nRet = PT_Encode_SHORT(ptr, nBuffLen, (unsigned short)nValue);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, pos->nDeriction);
    ptr += nRet;
    nBuffLen -= nRet;

	*ptr++ = (((pos->nYear - 2000) / 10) << 4) | (((pos->nYear - 2000) % 10) & 0x0F);
    *ptr++ = ((pos->nMonth / 10) << 4) | ((pos->nMonth % 10) & 0x0F);
	*ptr++ = ((pos->nDay / 10) << 4) | ((pos->nDay % 10) & 0x0F);
	*ptr++ = ((pos->nHour / 10) << 4) | ((pos->nHour % 10) & 0x0F);
	*ptr++ = ((pos->nMinute / 10) << 4) | ((pos->nMinute % 10) & 0x0F);
	*ptr++ = ((pos->nSecond / 10) << 4) | ((pos->nSecond % 10) & 0x0F);

    nBuffLen -= 6;
	
    if ((nBuffLen > 2) && (pos->addininfo.nFlag)) //������Ϣ
    {
        if (pos->addininfo.nFlag & JT808_DEF_INFO_MILEAGE)//���     modify by xlsong
        {
			*ptr++ = 0x01;//id
			*ptr++ = 4;	  //length
			nBuffLen -= 2;

            nRet = PT_Encode_INT32(ptr, nBuffLen, pos->addininfo.nMileage);
            ptr += nRet;
            nBuffLen -= nRet;
        }

        if (pos->addininfo.nFlag & JT808_DEF_INFO_OIL)//����
        {
			*ptr++ = 0x02;	//id
			*ptr++ = 2;	    //length
			nBuffLen -= 2;

            nRet = PT_Encode_SHORT(ptr, nBuffLen, pos->addininfo.nOil);
            ptr += nRet;
            nBuffLen -= nRet;
        }

        if (pos->addininfo.nFlag & JT808_DEF_INFO_SPEED)//��¼���ٶ�
        {
			*ptr++ = 0x03; //id
			*ptr++ = 2;	  //length
			nBuffLen -= 2;

            nRet = PT_Encode_SHORT(ptr, nBuffLen, pos->addininfo.nSpeed * 10);
            ptr += nRet;
            nBuffLen -= nRet;
        }

        if (pos->addininfo.nFlag & JT808_DEF_INFO_OVERSPEED)//���ٱ���������Ϣ
        {
			*ptr++ = 0x11; //id
			nBuffLen--;

            if (pos->addininfo.overspeedinfo.nPositionType > 0)
            {
				*ptr++ = 5;	   //length
				nBuffLen--;

				*ptr++ = pos->addininfo.overspeedinfo.nPositionType;
            	nBuffLen--;

                nRet = PT_Encode_INT32(ptr, nBuffLen, pos->addininfo.overspeedinfo.nAreaID);
                ptr += nRet;
                nBuffLen -= nRet;
            }
			else
			{
				*ptr++ = 1;	 //length

				*ptr++ = pos->addininfo.overspeedinfo.nPositionType;
            	nBuffLen -= 2;
			}
        }

        if (pos->addininfo.nFlag & JT808_DEF_INFO_AREA_ROUTE)//��������/·�߱���������Ϣ
        {
			*ptr++ = 0x12; //id
			*ptr++ = 6;	  //length
			nBuffLen -= 2;

            *ptr++ = pos->addininfo.areainfo.nPositionType;
            nBuffLen--;

            nRet = PT_Encode_INT32(ptr, nBuffLen, pos->addininfo.areainfo.nAreaID);
            ptr += nRet;
            nBuffLen -= nRet;

            *ptr++ = pos->addininfo.areainfo.nDeriction;
            nBuffLen--;
        }

        if (pos->addininfo.nFlag & JT808_DEF_INFO_DRIVING_TIME)//·����ʻʱ�䲻��/��������������Ϣ
        {
			*ptr++ = 0x13; //id
			*ptr++ = 7;	  //length
			nBuffLen -= 2;

            nRet = PT_Encode_INT32(ptr, nBuffLen, pos->addininfo.drivinginfo.nAreaID);
            ptr += nRet;
            nBuffLen -= nRet;

            nRet = PT_Encode_SHORT(ptr, nBuffLen, pos->addininfo.drivinginfo.nTime);
            ptr += nRet;
            nBuffLen -= nRet;

            *ptr++ = pos->addininfo.drivinginfo.nResult;
            nBuffLen--;
        }

		if (pos->addininfo.nFlag & JT808_DEF_INFO_EVENTID)//��Ҫ�˹�ȷ�ϱ����¼��� ID
		{
            *ptr++ = 0x04; //id
			*ptr++ = 2;	  //length
			nBuffLen -= 2;
			nRet = PT_Encode_SHORT(ptr, nBuffLen, pos->addininfo.nEventID);
			ptr += nRet;
			nBuffLen -= nRet;
		}

		if (pos->addininfo.nFlag & JT808_DEF_INFO_CARSIGNAL)//��չ�����ź�״̬λ
		{
            *ptr++ = 0x25; //id
			*ptr++ = 4;	  //length
			nBuffLen -= 2;
			nRet = PT_Encode_INT32(ptr, nBuffLen, pos->addininfo.nCarSignalStatus);
			ptr += nRet;
			nBuffLen -= nRet;
		}

		if (pos->addininfo.nFlag & JT808_DEF_INFO_IOSTATUS)//IO״̬λ
		{
            *ptr++ = 0x2A; //id
			*ptr++ = 2;	  //length
			nBuffLen -= 2;
			nRet = PT_Encode_SHORT(ptr, nBuffLen, pos->addininfo.nIoStatus);
			ptr += nRet;
			nBuffLen -= nRet;
		}

		if (pos->addininfo.nFlag & JT808_DEF_INFO_AD)//ģ����
		{
            *ptr++ = 0x2B; //id
			*ptr++ = 4;	  //length
			nBuffLen -= 2;
			nRet = PT_Encode_INT32(ptr, nBuffLen, pos->addininfo.nAnalogData);
			ptr += nRet;
			nBuffLen -= nRet;
		}

		if (pos->addininfo.nFlag & JT808_DEF_INFO_WLSIGNAL)//����ͨ�������ź�ǿ��
		{
            *ptr++ = 0x30; //id
			*ptr++ = 1;	  //length
			nBuffLen -= 2;
			*ptr++ = pos->addininfo.nWLSignal;
			nBuffLen--;
		}

		if (pos->addininfo.nFlag & JT808_DEF_INFO_GNSSSTARS)//GNSS ��λ������
		{
            *ptr++ = 0x31; //id
			*ptr++ = 1;	  //length
			*ptr++ = pos->addininfo.nGnssStars;
			nBuffLen--;
		}
    }

    //�Զ�����Ϣ
    if (pos->userdef.nFlag && nBuffLen > 2)
    {
        unsigned char * p = 0;
        unsigned char i = 0;
        *ptr++ = 0xE0;
        p = ptr++;

        if ((pos->userdef.nFlag & JT808_DEF_INFO1_OIL) && pos->userdef.nOilCount > 0)//��ѹ
        {
            *ptr++ = 0xF0; //id
            *ptr++ = pos->userdef.nOilCount;
            nBuffLen -= 2;
            
            for (i = 0; i < pos->userdef.nOilCount; i++)
            {
                nRet = PT_Encode_SHORT(ptr, nBuffLen, pos->userdef.oils[i]);
                ptr += nRet;
                nBuffLen -= nRet;

                *ptr++ = pos->userdef.oilsstate[i];
                nBuffLen--;
            }
        }

        if ((pos->userdef.nFlag & JT808_DEF_INFO1_TYRE) && pos->userdef.nTyreCount > 0)//̥ѹ
        {
            //*ptr++ = 0xF1; //id���ڰ汾������λ��ʶ
            *ptr++ = 0xF5; //id������λ��ʶ
            *ptr++ = pos->userdef.nTyreCount;
            nBuffLen -= 2;

            for (i = 0; i < pos->userdef.nTyreCount; i++)
            {
                *ptr++ = pos->userdef.tyres[i].nIndex;//��λ
                nBuffLen--;

                nRet = PT_Encode_SHORT(ptr, nBuffLen, pos->userdef.tyres[i].nPressure);
                ptr += nRet;
                nBuffLen -= nRet;

                nRet = PT_Encode_SHORT(ptr, nBuffLen, pos->userdef.tyres[i].nTemp);
                ptr += nRet;
                nBuffLen -= nRet;

                *ptr++ = pos->userdef.tyres[i].nState;
                nBuffLen--;
            }
        }

        if ((pos->userdef.nFlag & JT808_DEF_INFO1_TEMP) && pos->userdef.nTempCount > 0)//�¶�
        {
            *ptr++ = 0xF2; //id
            *ptr++ = pos->userdef.nTempCount;
            nBuffLen -= 2;

            for (i = 0; i < pos->userdef.nTempCount; i++)
            {
                nRet = PT_Encode_SHORT(ptr, nBuffLen, pos->userdef.temps[i]);
                ptr += nRet;
                nBuffLen -= nRet;

                *ptr++ = pos->userdef.tempsstate[i];
                nBuffLen--;
            }
        }

        if ((pos->userdef.nFlag & JT808_DEF_INFO1_POSI) && pos->userdef.nPosiCount > 0)//λ��
        {
            *ptr++ = 0xF3; //id
            *ptr++ = pos->userdef.nPosiCount;
            nBuffLen -= 2;

            for (i = 0; i < pos->userdef.nPosiCount; i++)
            {
                nRet = PT_Encode_SHORT(ptr, nBuffLen, pos->userdef.positions[i]);
                ptr += nRet;
                nBuffLen -= nRet;

                *ptr++ = pos->userdef.posistate[i];
                nBuffLen--;
            }
        }

        if (pos->userdef.nFlag & JT808_DEF_INFO1_CARRY)//�ػ�
        {
            *ptr++ = 0xF4; //id
            *ptr++ = pos->userdef.nCarryState;
            nBuffLen -= 2;
        }

        if (pos->userdef.nFlag & JT808_DEF_INFO1_SERVERFLAG)//��������ʶ��Ϣ,�Զ��帽����Ϣ
        {
            *ptr++ = 0xFF; //id
            nBuffLen--;

            nRet = PT_Encode_INT32(ptr, nBuffLen, pos->userdef.nServerFlag);
            ptr += nRet;
            nBuffLen -= nRet;
        }

        *p = ptr - p - 1;//�Զ���Ϣ���볤��
    }
    
    return (ptr - lpBuff);
}

int JT808_DecodePositionReport(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    TJT808PositionReport *dat = (TJT808PositionReport *)pPackage;

    return JT808_DecodePosition(&(dat->position), lpBuff, nBuffLen);
}

int JT808_EncodePositionReport(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    TJT808PositionReport *dat = (TJT808PositionReport *)pPackage;
    
    return JT808_EncodePosition(lpBuff, nBuffLen, &(dat->position));
}

int JT808_DecodePositionQueryRes(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808PositionQueryRes *dat = (TJT808PositionQueryRes *)pPackage;

    nRet = PT_Decode_SHORT(&(dat->nSeqNum), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = JT808_DecodePosition(&(dat->position), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    return ptr - lpBuff;
}

int JT808_EncodePositionQueryRes(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808PositionQueryRes *dat = (TJT808PositionQueryRes *)pPackage;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nSeqNum);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = JT808_EncodePosition(ptr, nBuffLen, &(dat->position));
    ptr += nRet;
    nBuffLen -= nRet;

    return ptr - lpBuff;
}
//�˹�ȷ�ϱ�����Ϣ   Ver201301
int JT808_DecodeWarmManage(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet;
	uchar *ptr = lpBuff;
	TJT808WarmManage *dat = (TJT808WarmManage *)pPackage;

	//������Ϣ��ˮ�� 
	nRet = PT_Decode_SHORT(&(dat->nSeqNum),ptr, nBuffLen);
	ptr += nRet;
	nBuffLen -= nRet;

	//�˹�ȷ�ϱ�������
	nRet = PT_Decode_INT32(&(dat->nWarmType),ptr, nBuffLen);
	ptr += nRet;
	nBuffLen -= nRet;

	return ptr - lpBuff;
}
//�˹�ȷ�ϱ�����Ϣ   Ver201301
int JT808_EncodeWarmManage(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet;
	uchar *ptr = lpBuff;
	TJT808WarmManage *dat = (TJT808WarmManage *)pPackage;

	//������Ϣ��ˮ�� 
	nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nSeqNum);
	ptr += nRet;
	nBuffLen -= nRet;

	//�˹�ȷ�ϱ�������
	nRet = PT_Encode_INT32(ptr, nBuffLen, dat->nWarmType);
	ptr += nRet;
	nBuffLen -= nRet;
	return ptr - lpBuff;
}
int JT808_DecodePositionTrack(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808PositionTrack *dat = (TJT808PositionTrack *)pPackage;

    nRet = PT_Decode_SHORT(&(dat->nInterval), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Decode_INT32(&(dat->nTimeLimit), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    return ptr - lpBuff;
}

int JT808_EncodePositionTrack(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808PositionTrack *dat = (TJT808PositionTrack *)pPackage;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nInterval);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Encode_INT32(ptr, nBuffLen, dat->nTimeLimit);
    ptr += nRet;
    nBuffLen -= nRet;

    return ptr - lpBuff;
}
//�ı��·�
int JT808_DecodeTextNotice(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    uchar *ptr = lpBuff;
    TJT808TextNotice *dat = (TJT808TextNotice *)pPackage;

    dat->nFlag = *ptr++;
    nBuffLen--;

    if (nBuffLen >= sizeof(dat->szText) - 1)
    {
        nBuffLen = sizeof(dat->szText) - 1;
    }

    memcpy(dat->szText, ptr, nBuffLen);

    dat->szText[nBuffLen] = '\0';

    return ptr - lpBuff;
}
//�ı��·�
int JT808_EncodeTextNotice(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808TextNotice *dat = (TJT808TextNotice *)pPackage;

    *ptr++ = dat->nFlag;
    nBuffLen--;

    nRet = strlen(dat->szText) + 1;//����������
    memcpy(ptr, dat->szText, nRet);
    ptr += nRet;
    nBuffLen -= nRet;

    return ptr - lpBuff;
}

int JT808_DecodeEventSet(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet, i;
    uchar nLen;
    uchar *ptr = lpBuff;
     TJT808EventSet *dat = (TJT808EventSet *)pPackage;

    dat->nType = *ptr++;
    nBuffLen--;

    if (dat->nType > JT808_DEF_EVENT_REMOVEALL)
    {
        dat->nCount = *ptr++;
        nBuffLen--;

        if (dat->nType == JT808_DEF_EVENT_REMOVE)
        {
            for (i = 0; i < dat->nCount; i++)
            {
                dat->events[i].nEventID = *ptr++;
                nBuffLen--;
            }
        }
        else 
        {
            for (i = 0; i < dat->nCount; i++)
            {
                dat->events[i].nEventID = *ptr++;
                nBuffLen--;

                nLen = *ptr++;
                nBuffLen--;

                nRet = sizeof(dat->events[i].szEvent) - 1;
                if (nRet > nLen)
                {
                    nRet = nLen;
                }

                memcpy(dat->events[i].szEvent, ptr, nRet);
                dat->events[i].szEvent[nRet] = '\0';
                ptr += nLen;
                nBuffLen -= nLen;
            }
        }
    }

    return ptr - lpBuff;
}

int JT808_EncodeEventSet(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet, i;
    uchar *ptr = lpBuff;
    TJT808EventSet *dat = (TJT808EventSet *)pPackage;

    *ptr++ = dat->nType;
    nBuffLen--;

    if (dat->nType > JT808_DEF_EVENT_REMOVEALL)
    {
        *ptr++ = dat->nCount;
        nBuffLen--;
        
        if (dat->nType == JT808_DEF_EVENT_REMOVE)
        {
            for (i = 0; i < dat->nCount; i++)
            {
                *ptr++ = dat->events[i].nEventID;
                nBuffLen--;
            }
        }
        else 
        {
            for (i = 0; i < dat->nCount; i++)
            {
                *ptr++ = dat->events[i].nEventID;
                nBuffLen--;

                nRet = strlen(dat->events[i].szEvent) + 1;//����������
                *ptr++ = nRet;
                nBuffLen--;

                memcpy(ptr, dat->events[i].szEvent, nRet);
                ptr += nRet;
                nBuffLen -= nRet;
            }
        }
    }

    return ptr - lpBuff;
}

int JT808_DecodeEventReport(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    uchar *ptr = lpBuff;
    TJT808EventReport *dat = (TJT808EventReport *)pPackage;

    dat->nEventID = *ptr++;
    nBuffLen--;

    return ptr - lpBuff;
}

int JT808_EncodeEventReport(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    uchar *ptr = lpBuff;
    TJT808EventReport *dat = (TJT808EventReport *)pPackage;

    *ptr++ = dat->nEventID;
    nBuffLen--;

    return ptr - lpBuff;
}

int JT808_DecodeQuestionReq(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    unsigned short nLen;
    uchar *ptr = lpBuff;
    TJT808QuestionReq *dat = (TJT808QuestionReq *)pPackage;

    dat->nFlag = *ptr++;
    nBuffLen--;

    nRet = *ptr++;
    nBuffLen--;
    
    nLen = sizeof(dat->szQuestion) - 1;
    if (nRet < nLen)
    {
        nLen = nRet;
    }

    memcpy(dat->szQuestion, ptr, nLen);
    dat->szQuestion[nLen] = '\0';
    ptr += nRet;
    nBuffLen -= nRet;

    for (dat->nKeyCount = 0; dat->nKeyCount < JT808_DEF_MAX_NUM_KEY; dat->nKeyCount++)
    {
        if (nBuffLen >= 3)
        {
            dat->keys[dat->nKeyCount].nKeyID = *ptr++;
            nBuffLen--;

            nRet = PT_Decode_SHORT(&nLen, ptr, nBuffLen);
            ptr += nRet;
            nBuffLen -= nRet;
        }
        else
        {
            break;
        }

        if (nBuffLen >= nLen)
        {
            nRet = sizeof(dat->keys[dat->nKeyCount].szKey) - 1;
            if (nLen < nRet)
            {
                nRet = nLen;
            }
            memcpy(dat->keys[dat->nKeyCount].szKey, ptr, nRet); //---by xlsong
            dat->keys[dat->nKeyCount].szKey[nRet] = '\0';
            ptr += nLen;
            nBuffLen -= nLen;
        }
        else
        {
            break;
        }
    }

    return ptr - lpBuff;
}

int JT808_EncodeQuestionReq(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet, i;
    unsigned short nLen;
    uchar *ptr = lpBuff;
    TJT808QuestionReq *dat = (TJT808QuestionReq *)pPackage;

    *ptr++ = dat->nFlag;
    nBuffLen--;

    nRet = strlen(dat->szQuestion) + 1;//����������
    *ptr++ = nRet;
    nBuffLen--;

    memcpy(ptr, dat->szQuestion, nRet);
    ptr += nRet;
    nBuffLen -= nRet;

    for (i = 0; i < dat->nKeyCount; i++)
    {
        *ptr++ = dat->keys[i].nKeyID;
        nBuffLen--;
        
        nLen = strlen(dat->keys[i].szKey) + 1;//����������
        nRet = PT_Encode_SHORT(ptr, nBuffLen, nLen);
        ptr += nRet;
        nBuffLen -= nRet;

        memcpy(ptr, dat->keys[i].szKey, nLen);
        ptr += nLen;
        nBuffLen -= nLen;
    }

    return ptr - lpBuff;
}

int JT808_DecodeQuestionRes(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808QuestionRes *dat = (TJT808QuestionRes *)pPackage;

    nRet = PT_Decode_SHORT(&(dat->nSeqNum), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    dat->nKeyID = *ptr++;
    nBuffLen--;

    return ptr - lpBuff;
}

int JT808_EncodeQuestionRes(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808QuestionRes *dat = (TJT808QuestionRes *)pPackage;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nSeqNum);
    ptr += nRet;
    nBuffLen -= nRet;

    *ptr++ = dat->nKeyID;
    nBuffLen--;

    return ptr - lpBuff;
}

int JT808_DecodeServiceSet(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    unsigned short nLen;
    uchar *ptr = lpBuff;
    TJT808ServiceSet *dat = (TJT808ServiceSet *)pPackage;

    dat->nType = *ptr++;
    nBuffLen--;

    if (dat->nType > JT808_DEF_SERVICE_REMOVEALL)
    {
        ptr++;
        nBuffLen--;

        for (dat->nCount = 0; dat->nCount < JT808_DEF_MAX_NUM_SERVICE; dat->nCount++)
        {
            if (nBuffLen >= 3)
            {
                dat->services[dat->nCount].nServiceID = *ptr++;
                nBuffLen--;

                nRet = PT_Decode_SHORT(&nLen, ptr, nBuffLen);
                ptr += nRet;
                nBuffLen -= nRet;
            }
            else
            {
                break;
            }
            
            if (nBuffLen >= nLen)
            {
                nRet = sizeof(dat->services[dat->nCount].szService) - 1;
                if (nLen < nRet)
                {
                    nRet = nLen;
                }

                memcpy(dat->services[dat->nCount].szService, ptr, nRet);
                dat->services[dat->nCount].szService[nRet] = '\0';
                ptr += nLen;
                nBuffLen -= nLen;
            }
            else
            {
                break;
            }
        }
    }

    return ptr - lpBuff;
}

int JT808_EncodeServiceSet(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet, i;
    unsigned short nLen;
    uchar *ptr = lpBuff;
    TJT808ServiceSet *dat = (TJT808ServiceSet *)pPackage;

    *ptr++ = dat->nType;
    nBuffLen--;

    if (dat->nType > JT808_DEF_SERVICE_REMOVEALL)
    {
        *ptr++ = dat->nCount;
        nBuffLen--;

        for (i = 0; i < dat->nCount; i++)
        {
            *ptr++ = dat->services[i].nServiceID;
            nBuffLen--;

            nLen = strlen(dat->services[i].szService) + 1;//����������
            nRet = PT_Encode_SHORT(ptr, nBuffLen, nLen);
            ptr += nRet;
            nBuffLen -= nRet;

            memcpy(ptr, dat->services[i].szService, nLen);
            ptr += nLen;
            nBuffLen -= nLen;
        }
    }

    return ptr - lpBuff;
}

int JT808_DecodeServiceOpt(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    uchar *ptr = lpBuff;
    TJT808ServiceOpt *dat = (TJT808ServiceOpt *)pPackage;

    dat->nType = *ptr++;
    nBuffLen--;

    dat->nFlag = *ptr++;
    nBuffLen--;

    return ptr - lpBuff;
}

int JT808_EncodeServiceOpt(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    uchar *ptr = lpBuff;
    TJT808ServiceOpt *dat = (TJT808ServiceOpt *)pPackage;

    *ptr++ = dat->nType;
    nBuffLen--;

    *ptr++ = dat->nFlag;
    nBuffLen--;

    return ptr - lpBuff;
}

int JT808_DecodeServiceInfo(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    unsigned short nLen;
    uchar *ptr = lpBuff;
    TJT808ServiceInfo *dat = (TJT808ServiceInfo *)pPackage;

    dat->nType = *ptr++;
    nBuffLen--;

    nRet = PT_Decode_SHORT(&nLen, ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = 0;
    if (nBuffLen < nLen)
    {
        nLen = nBuffLen; 
    }

    nRet = sizeof(dat->szService) - 1;
    if (nRet > nLen)
    {
        nRet = nLen;
    }

    memcpy(dat->szService, ptr, nRet);
    dat->szService[nRet] = '\0';
    ptr += nLen;
    nBuffLen -= nLen;
    
    return ptr - lpBuff;
}

int JT808_EncodeServiceInfo(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet, nLen;
    uchar *ptr = lpBuff;
    TJT808ServiceInfo *dat = (TJT808ServiceInfo *)pPackage;

    *ptr++ = dat->nType;
    nBuffLen--;

    nLen = strlen(dat->szService) + 1;//����������
    nRet = PT_Encode_SHORT(ptr, nBuffLen, nLen);
    ptr += nRet;
    nBuffLen -= nRet;

    memcpy(ptr, dat->szService, nLen);
    ptr += nLen;
    nBuffLen -= nLen;

    return ptr - lpBuff;
}

int JT808_DecodePhoneCallback(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    uchar *ptr = lpBuff;
    TJT808PhoneCallback *dat = (TJT808PhoneCallback *)pPackage;

    dat->nFlag = *ptr++;
    nBuffLen--;

    if (nBuffLen <= (sizeof(dat->szPhone) - 1))
    {
        nBuffLen = sizeof(dat->szPhone) - 1;

        memcpy(dat->szPhone, ptr, nBuffLen);
    }

    dat->szPhone[nBuffLen] = '\0';

    return ptr - lpBuff;
}

int JT808_EncodePhoneCallback(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808PhoneCallback *dat = (TJT808PhoneCallback *)pPackage;

    *ptr++ = dat->nFlag;
    nBuffLen--;

    nRet = strlen(dat->szPhone) + 1;//����������
    memcpy(ptr, dat->szPhone, nRet);
    ptr += nRet;
    nBuffLen -= nRet;

    return ptr - lpBuff;
}

int JT808_DecodePhoneBook(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet, i;
    uchar *ptr = lpBuff;
    TJT808PhoneBook *dat = (TJT808PhoneBook *)pPackage;

    dat->nType = *ptr++;
    nBuffLen--;
    
    if (dat->nType > JT808_DEF_PHONEBOOK_REMOVEALL)
    {
        dat->nCount = *ptr++;
        nBuffLen--;

        for (i = 0; i < dat->nCount; i++)
        {
            dat->book[i].nFlag = *ptr++;
            nBuffLen--;
            
            nRet = *ptr++;
            nBuffLen--;

            memcpy(dat->book[i].szPhone, ptr, nRet);
            dat->book[i].szPhone[nRet] = '\0';
            ptr += nRet; 
            nBuffLen -= nRet;

            nRet = *ptr++;
            nBuffLen--;

            memcpy(dat->book[i].szName, ptr, nRet);
            dat->book[i].szName[nRet] = '\0';
            ptr += nRet; 
            nBuffLen -= nRet;
        }
    }
    
    return ptr - lpBuff;
}

int JT808_EncodePhoneBook(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet, i;
    uchar *ptr = lpBuff;
    TJT808PhoneBook *dat = (TJT808PhoneBook *)pPackage;

    *ptr++ = dat->nType;
    nBuffLen--;

    if (dat->nType > JT808_DEF_PHONEBOOK_REMOVEALL)
    {
        *ptr++ = dat->nCount;
        nBuffLen--;

        if (dat->nCount > JT808_DEF_MAX_NUM_PHONEITEM)
        {
            dat->nCount = JT808_DEF_MAX_NUM_PHONEITEM;
        }

        for (i = 0; i < dat->nCount; i++)
        {
            *ptr++ = dat->book[i].nFlag;
            nBuffLen--;
            
            nRet = strlen(dat->book[i].szPhone) + 1;//����������
            *ptr++ = nRet;
            nBuffLen--;

            memcpy(ptr, dat->book[i].szPhone, nRet);
            ptr += nRet; 
            nBuffLen -= nRet;

            nRet = strlen(dat->book[i].szName) + 1; //����������
            *ptr++ = nRet;
            nBuffLen--;

            memcpy(ptr, dat->book[i].szName, nRet);
            ptr += nRet; 
            nBuffLen -= nRet;
        }
    }

    return ptr - lpBuff;
}

int JT808_DecodeCarCtrlReq(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808CarCtrlReq *dat = (TJT808CarCtrlReq *)pPackage;
#ifdef PT_JT808_VER_ITS
    //��ע��ITS �ͻ���,��ָ��Ϊ16bit     but I am not entirely sure      ---by xlsong
	nRet = PT_Decode_SHORT(&(dat->nCMD), ptr, nBuffLen);
	ptr += nRet;
	nBuffLen -= nRet;
#else 
    dat->nCMD = *ptr++;
    nBuffLen--;
 #endif
    //�Զ���
    if (nBuffLen >= 4)
    {
        nRet = PT_Decode_INT32(&(dat->nServerFlag), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;
    }
    else
    {
        dat->nServerFlag = 0;
    }

    return ptr - lpBuff;
}

int JT808_EncodeCarCtrlReq(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808CarCtrlReq *dat = (TJT808CarCtrlReq *)pPackage;

    *ptr++ = dat->nCMD;
    nBuffLen--;

    //�Զ���
    if (dat->nServerFlag > 0)
    {
         nRet = PT_Encode_INT32(ptr, nBuffLen, dat->nServerFlag);
         ptr += nRet;
         nBuffLen -= nRet;
    }

    return ptr - lpBuff;
}

int JT808_DecodeCarCtrlRes(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    return JT808_DecodePositionQueryRes(pPackage, lpBuff, nBuffLen);
}

int JT808_EncodeCarCtrlRes(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    return JT808_EncodePositionQueryRes(lpBuff, nBuffLen, pPackage);
}

int JT808_DecodeRegionCircleSet(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    unsigned char i;
    unsigned int nTemp;
    uchar *ptr = lpBuff;
    TJT808RegionCircleSet *dat = (TJT808RegionCircleSet *)pPackage;

    dat->nType = *ptr++;
    nBuffLen--;

    dat->nRegionCount = *ptr++;
    nBuffLen--;

    if (dat->nRegionCount > 0)
    {
        if (dat->nRegionCount > JT808_DEF_MAX_NUM_REGION)
        {
            dat->nRegionCount = JT808_DEF_MAX_NUM_REGION;
        }

        for (i = 0; i < dat->nRegionCount; i++)
        {
            nRet = PT_Decode_INT32(&(dat->Regions[i].nRegionID),ptr, nBuffLen);
            ptr += nRet;
            nBuffLen -= nRet;

            nRet = PT_Decode_SHORT(&(dat->Regions[i].nFlag), ptr, nBuffLen);
            ptr += nRet;
            nBuffLen -= nRet;

            nRet = PT_Decode_INT32(&nTemp, ptr, nBuffLen);
            dat->Regions[i].ptCenter.dblLatitude = nTemp / JT808_MILLION;
            ptr += nRet;
            nBuffLen -= nRet;

            nRet = PT_Decode_INT32(&nTemp, ptr, nBuffLen);
            dat->Regions[i].ptCenter.dblLongitude = nTemp / JT808_MILLION;
            ptr += nRet;
            nBuffLen -= nRet;

            nRet = PT_Decode_INT32(&(dat->Regions[i].nRadius), ptr, nBuffLen);
            ptr += nRet;
            nBuffLen -= nRet;

            if (dat->Regions[i].nFlag & JT808_DEF_REGION_LIMIT_TIME)
            {
                dat->Regions[i].tmStart.nYear = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);
                if (dat->Regions[i].tmStart.nYear > 0)
                {
                    dat->Regions[i].tmStart.nYear += 2000;
                }

                ptr++;
                dat->Regions[i].tmStart.nMonth = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

                ptr++;
                dat->Regions[i].tmStart.nDay = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

                ptr++;
                dat->Regions[i].tmStart.nHour = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

                ptr++;
                dat->Regions[i].tmStart.nMinute = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

                ptr++;
                dat->Regions[i].tmStart.nSecond = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);


                ptr++;
                dat->Regions[i].tmEnd.nYear = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);
                if (dat->Regions[i].tmEnd.nYear > 0)
                {
                    dat->Regions[i].tmEnd.nYear += 2000;
                }

                ptr++;
                dat->Regions[i].tmEnd.nMonth = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

                ptr++;
                dat->Regions[i].tmEnd.nDay = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

                ptr++;
                dat->Regions[i].tmEnd.nHour = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

                ptr++;
                dat->Regions[i].tmEnd.nMinute = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

                ptr++;
                dat->Regions[i].tmEnd.nSecond = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

                ptr++;
                nBuffLen -= 12;
            }
            else
            {
                dat->Regions[i].tmStart.nYear = 0;
                dat->Regions[i].tmStart.nMonth = 0;
                dat->Regions[i].tmStart.nDay = 0;
                dat->Regions[i].tmStart.nHour = 0;
                dat->Regions[i].tmStart.nMinute = 0;
                dat->Regions[i].tmStart.nSecond = 0;

                dat->Regions[i].tmEnd.nYear = 0;
                dat->Regions[i].tmEnd.nMonth = 0;
                dat->Regions[i].tmEnd.nDay = 0;
                dat->Regions[i].tmEnd.nHour = 0;
                dat->Regions[i].tmEnd.nMinute = 0;
                dat->Regions[i].tmEnd.nSecond = 0;
            }

            if (dat->Regions[i].nFlag & JT808_DEF_REGION_LIMIT_SPEED)
            {
                nRet = PT_Decode_SHORT(&(dat->Regions[i].nMaxSpeed), ptr, nBuffLen);
                ptr += nRet;
                nBuffLen -= nRet;

                dat->Regions[i].nTimeLimit = *ptr++;
                nBuffLen--;
            }
            else
            {
                dat->Regions[i].nMaxSpeed = 0;
                dat->Regions[i].nTimeLimit = 0;
            }
        }
    }

    return ptr - lpBuff;
}

int JT808_EncodeRegionCircleSet(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    unsigned char i;
    unsigned int nTemp;
    uchar *ptr = lpBuff;
    TJT808RegionCircleSet *dat = (TJT808RegionCircleSet *)pPackage;

    *ptr++ = dat->nType;
    nBuffLen--;

    *ptr++ = dat->nRegionCount;
    nBuffLen--;

    if (dat->nRegionCount > 0 && dat->nRegionCount <= JT808_DEF_MAX_NUM_REGION)
    {
        for (i = 0; i < dat->nRegionCount; i++)
        {
            nRet = PT_Encode_INT32(ptr, nBuffLen, dat->Regions[i].nRegionID);
            ptr += nRet;
            nBuffLen -= nRet;

            nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->Regions[i].nFlag);
            ptr += nRet;
            nBuffLen -= nRet;

            nTemp = dat->Regions[i].ptCenter.dblLatitude * JT808_MILLION;
            nRet = PT_Encode_INT32(ptr, nBuffLen, nTemp);
            ptr += nRet;
            nBuffLen -= nRet;

            nTemp = dat->Regions[i].ptCenter.dblLongitude * JT808_MILLION;
            nRet = PT_Encode_INT32(ptr, nBuffLen, nTemp);
            ptr += nRet;
            nBuffLen -= nRet;

            nRet = PT_Encode_INT32(ptr, nBuffLen, dat->Regions[i].nRadius);
            ptr += nRet;
            nBuffLen -= nRet;

            if (dat->Regions[i].nFlag & JT808_DEF_REGION_LIMIT_TIME)
            {
                *ptr++ = (((dat->Regions[i].tmStart.nYear / 10) % 10) << 4) | (dat->Regions[i].tmStart.nYear % 10);
                *ptr++ = ((dat->Regions[i].tmStart.nMonth / 10) << 4) | (dat->Regions[i].tmStart.nMonth % 10);
                *ptr++ = ((dat->Regions[i].tmStart.nDay / 10) << 4) | (dat->Regions[i].tmStart.nDay % 10);
                *ptr++ = ((dat->Regions[i].tmStart.nHour / 10) << 4) | (dat->Regions[i].tmStart.nHour % 10);
                *ptr++ = ((dat->Regions[i].tmStart.nMinute / 10) << 4) | (dat->Regions[i].tmStart.nMinute % 10);
                *ptr++ = ((dat->Regions[i].tmStart.nSecond / 10) << 4) | (dat->Regions[i].tmStart.nSecond % 10);

                *ptr++ = (((dat->Regions[i].tmEnd.nYear / 10) % 10) << 4) | (dat->Regions[i].tmEnd.nYear % 10);
                *ptr++ = ((dat->Regions[i].tmEnd.nMonth / 10) << 4) | (dat->Regions[i].tmEnd.nMonth % 10);
                *ptr++ = ((dat->Regions[i].tmEnd.nDay / 10) << 4) | (dat->Regions[i].tmEnd.nDay % 10);
                *ptr++ = ((dat->Regions[i].tmEnd.nHour / 10) << 4) | (dat->Regions[i].tmEnd.nHour % 10);
                *ptr++ = ((dat->Regions[i].tmEnd.nMinute / 10)  << 4) | (dat->Regions[i].tmEnd.nMinute % 10);
                *ptr++ = ((dat->Regions[i].tmEnd.nSecond / 10) << 4) | (dat->Regions[i].tmEnd.nSecond % 10);

                nBuffLen -= 12;
            }

            if (dat->Regions[i].nFlag & JT808_DEF_REGION_LIMIT_SPEED)
            {
                nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->Regions[i].nMaxSpeed);
                ptr += nRet;
                nBuffLen -= nRet;

                *ptr++ = dat->Regions[i].nTimeLimit;
                nBuffLen--;
            }
        }
    }

    return ptr - lpBuff;
}

int JT808_DecodeRegionRemove(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    unsigned char i;
    uchar *ptr = lpBuff;
    TJT808RegionRemove *dat = (TJT808RegionRemove *)pPackage;

    dat->nRegionCount = *ptr++;
    nBuffLen--;

    if (dat->nRegionCount > JT808_DEF_MAX_NUM_REGION)
    {
        dat->nRegionCount = JT808_DEF_MAX_NUM_REGION;
    }

    for (i = 0; i < dat->nRegionCount; i++)
    {
        nRet = PT_Decode_INT32(&(dat->Regions[i]), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;
    }

    return ptr - lpBuff;
}

int JT808_EncodeRegionRemove(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    unsigned char i;
    uchar *ptr = lpBuff;
    TJT808RegionRemove *dat = (TJT808RegionRemove *)pPackage;

    *ptr++ = dat->nRegionCount;
    nBuffLen--;

    for (i = 0; i < dat->nRegionCount; i++)
    {
        nRet = PT_Encode_INT32(ptr, nBuffLen, dat->Regions[i]);
        ptr += nRet;
        nBuffLen -= nRet;
    }

    return ptr - lpBuff;
}

int JT808_DecodeRegionRectSet(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    unsigned char i;
    unsigned int nTemp;
    uchar *ptr = lpBuff;
    TJT808RegionRectSet *dat = (TJT808RegionRectSet *)pPackage;

    dat->nType = *ptr++;
    nBuffLen--;

    dat->nRegionCount = *ptr++;
    nBuffLen--;

    if (dat->nRegionCount > 0)
    {
        if (dat->nRegionCount > JT808_DEF_MAX_NUM_REGION)
        {
            dat->nRegionCount = JT808_DEF_MAX_NUM_REGION;
        }

        for (i = 0; i < dat->nRegionCount; i++)
        {
            nRet = PT_Decode_INT32(&(dat->Regions[i].nRegionID), ptr, nBuffLen);
            ptr += nRet;
            nBuffLen -= nRet;

            nRet = PT_Decode_SHORT(&(dat->Regions[i].nFlag), ptr, nBuffLen);
            ptr += nRet;
            nBuffLen -= nRet;

            nRet = PT_Decode_INT32(&nTemp, ptr, nBuffLen);
            dat->Regions[i].ptLeftTop.dblLatitude = nTemp / JT808_MILLION;
            ptr += nRet;
            nBuffLen -= nRet;

            nRet = PT_Decode_INT32(&nTemp, ptr, nBuffLen);
            dat->Regions[i].ptLeftTop.dblLongitude = nTemp / JT808_MILLION;
            ptr += nRet;
            nBuffLen -= nRet;

            nRet = PT_Decode_INT32(&nTemp, ptr, nBuffLen);
            dat->Regions[i].ptRightBottom.dblLatitude = nTemp / JT808_MILLION;
            ptr += nRet;
            nBuffLen -= nRet;

            nRet = PT_Decode_INT32(&nTemp, ptr, nBuffLen);
            dat->Regions[i].ptRightBottom.dblLongitude = nTemp / JT808_MILLION;
            ptr += nRet;
            nBuffLen -= nRet;

            if (dat->Regions[i].nFlag & JT808_DEF_REGION_LIMIT_TIME)
            {
                dat->Regions[i].tmStart.nYear = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);
                if (dat->Regions[i].tmStart.nYear > 0)
                {
                    dat->Regions[i].tmStart.nYear += 2000;
                }

                ptr++;
                dat->Regions[i].tmStart.nMonth = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

                ptr++;
                dat->Regions[i].tmStart.nDay = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

                ptr++;
                dat->Regions[i].tmStart.nHour = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

                ptr++;
                dat->Regions[i].tmStart.nMinute = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

                ptr++;
                dat->Regions[i].tmStart.nSecond = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);


                ptr++;
                dat->Regions[i].tmEnd.nYear = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);
                if (dat->Regions[i].tmEnd.nYear > 0)
                {
                    dat->Regions[i].tmEnd.nYear += 2000;
                }

                ptr++;
                dat->Regions[i].tmEnd.nMonth = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

                ptr++;
                dat->Regions[i].tmEnd.nDay = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

                ptr++;
                dat->Regions[i].tmEnd.nHour = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

                ptr++;
                dat->Regions[i].tmEnd.nMinute = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

                ptr++;
                dat->Regions[i].tmEnd.nSecond = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

                ptr++;
                nBuffLen -= 12;
            }
            else
            {
                dat->Regions[i].tmStart.nYear = 0;
                dat->Regions[i].tmStart.nMonth = 0;
                dat->Regions[i].tmStart.nDay = 0;
                dat->Regions[i].tmStart.nHour = 0;
                dat->Regions[i].tmStart.nMinute = 0;
                dat->Regions[i].tmStart.nSecond = 0;

                dat->Regions[i].tmEnd.nYear = 0;
                dat->Regions[i].tmEnd.nMonth = 0;
                dat->Regions[i].tmEnd.nDay = 0;
                dat->Regions[i].tmEnd.nHour = 0;
                dat->Regions[i].tmEnd.nMinute = 0;
                dat->Regions[i].tmEnd.nSecond = 0;
            }

            if (dat->Regions[i].nFlag & JT808_DEF_REGION_LIMIT_SPEED)
            {
                nRet = PT_Decode_SHORT(&(dat->Regions[i].nMaxSpeed), ptr, nBuffLen);
                ptr += nRet;
                nBuffLen -= nRet;

                dat->Regions[i].nTimeLimit = *ptr++;
                nBuffLen--;
            }
            else
            {
                dat->Regions[i].nMaxSpeed = 0;
                dat->Regions[i].nTimeLimit = 0;
            }
        }
    }

    return ptr - lpBuff;
}

int JT808_EncodeRegionRectSet(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    unsigned char i;
    unsigned int nTemp;
    uchar *ptr = lpBuff;
    TJT808RegionRectSet *dat = (TJT808RegionRectSet *)pPackage;

    *ptr++ = dat->nType;
    nBuffLen--;

    *ptr++ = dat->nRegionCount;
    nBuffLen--;

    if (dat->nRegionCount > 0 && dat->nRegionCount <= JT808_DEF_MAX_NUM_REGION)
    {
        for (i = 0; i < dat->nRegionCount; i++)
        {
            nRet = PT_Encode_INT32(ptr, nBuffLen, dat->Regions[i].nRegionID);
            ptr += nRet;
            nBuffLen -= nRet;

            nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->Regions[i].nFlag);
            ptr += nRet;
            nBuffLen -= nRet;

            nTemp = dat->Regions[i].ptLeftTop.dblLatitude * JT808_MILLION;
            nRet = PT_Encode_INT32(ptr, nBuffLen, nTemp);
            ptr += nRet;
            nBuffLen -= nRet;

            nTemp = dat->Regions[i].ptLeftTop.dblLongitude * JT808_MILLION;
            nRet = PT_Encode_INT32(ptr, nBuffLen, nTemp);
            ptr += nRet;
            nBuffLen -= nRet;

            nTemp = dat->Regions[i].ptRightBottom.dblLatitude * JT808_MILLION;
            nRet = PT_Encode_INT32(ptr, nBuffLen, nTemp);
            ptr += nRet;
            nBuffLen -= nRet;

            nTemp = dat->Regions[i].ptRightBottom.dblLongitude * JT808_MILLION;
            nRet = PT_Encode_INT32(ptr, nBuffLen, nTemp);
            ptr += nRet;
            nBuffLen -= nRet;

            if (dat->Regions[i].nFlag & JT808_DEF_REGION_LIMIT_TIME)
            {
                *ptr++ = (((dat->Regions[i].tmStart.nYear / 10) % 10) << 4) | (dat->Regions[i].tmStart.nYear % 10);
                *ptr++ = ((dat->Regions[i].tmStart.nMonth / 10) << 4) | (dat->Regions[i].tmStart.nMonth % 10);
                *ptr++ = ((dat->Regions[i].tmStart.nDay / 10) << 4) | (dat->Regions[i].tmStart.nDay % 10);
                *ptr++ = ((dat->Regions[i].tmStart.nHour / 10) << 4) | (dat->Regions[i].tmStart.nHour % 10);
                *ptr++ = ((dat->Regions[i].tmStart.nMinute / 10) << 4) | (dat->Regions[i].tmStart.nMinute % 10);
                *ptr++ = ((dat->Regions[i].tmStart.nSecond / 10) << 4) | (dat->Regions[i].tmStart.nSecond % 10);

                *ptr++ = (((dat->Regions[i].tmEnd.nYear / 10) % 10) << 4) | (dat->Regions[i].tmEnd.nYear % 10);
                *ptr++ = ((dat->Regions[i].tmEnd.nMonth / 10) << 4) | (dat->Regions[i].tmEnd.nMonth % 10);
                *ptr++ = ((dat->Regions[i].tmEnd.nDay / 10) << 4) | (dat->Regions[i].tmEnd.nDay % 10);
                *ptr++ = ((dat->Regions[i].tmEnd.nHour / 10) << 4) | (dat->Regions[i].tmEnd.nHour % 10);
                *ptr++ = ((dat->Regions[i].tmEnd.nMinute / 10)  << 4) | (dat->Regions[i].tmEnd.nMinute % 10);
                *ptr++ = ((dat->Regions[i].tmEnd.nSecond / 10) << 4) | (dat->Regions[i].tmEnd.nSecond % 10);

                nBuffLen -= 12;
            }

            if (dat->Regions[i].nFlag & JT808_DEF_REGION_LIMIT_SPEED)
            {
                nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->Regions[i].nMaxSpeed);
                ptr += nRet;
                nBuffLen -= nRet;

                *ptr++ = dat->Regions[i].nTimeLimit;
                nBuffLen--;
            }
        }
    }

    return ptr - lpBuff;
}

int JT808_DecodeRegionPolygonSet(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    unsigned char i;
    unsigned int nTemp;
    uchar *ptr = lpBuff;
    TJT808RegionPolygonSet *dat = (TJT808RegionPolygonSet *)pPackage;

    nRet = PT_Decode_INT32(&(dat->nRegionID), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Decode_SHORT(&(dat->nFlag), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    if (dat->nFlag & JT808_DEF_REGION_LIMIT_TIME)
    {
        dat->tmStart.nYear = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);
        if (dat->tmStart.nYear > 0)
        {
            dat->tmStart.nYear += 2000;
        }

        ptr++;
        dat->tmStart.nMonth = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

        ptr++;
        dat->tmStart.nDay = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

        ptr++;
        dat->tmStart.nHour = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

        ptr++;
        dat->tmStart.nMinute = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

        ptr++;
        dat->tmStart.nSecond = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);


        ptr++;
        dat->tmEnd.nYear = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);
        if (dat->tmEnd.nYear > 0)
        {
            dat->tmEnd.nYear += 2000;
        }

        ptr++;
        dat->tmEnd.nMonth = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

        ptr++;
        dat->tmEnd.nDay = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

        ptr++;
        dat->tmEnd.nHour = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

        ptr++;
        dat->tmEnd.nMinute = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

        ptr++;
        dat->tmEnd.nSecond = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

        ptr++;
        nBuffLen -= 12;
    }
    else
    {
        dat->tmStart.nYear = 0;
        dat->tmStart.nMonth = 0;
        dat->tmStart.nDay = 0;
        dat->tmStart.nHour = 0;
        dat->tmStart.nMinute = 0;
        dat->tmStart.nSecond = 0;

        dat->tmEnd.nYear = 0;
        dat->tmEnd.nMonth = 0;
        dat->tmEnd.nDay = 0;
        dat->tmEnd.nHour = 0;
        dat->tmEnd.nMinute = 0;
        dat->tmEnd.nSecond = 0;
    }

    if (dat->nFlag & JT808_DEF_REGION_LIMIT_SPEED)
    {
        nRet = PT_Decode_SHORT(&(dat->nMaxSpeed), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;

        dat->nTimeLimit = *ptr++;
        nBuffLen--;
    }
    else
    {
        dat->nMaxSpeed = 0;
        dat->nTimeLimit = 0;
    }

    dat->nPointCount = *ptr++;
    nBuffLen--;

    if (dat->nPointCount > 0)
    {
        if (dat->nPointCount > JT808_DEF_MAX_NUM_POLYGON_POINT)
        {
            dat->nPointCount = JT808_DEF_MAX_NUM_POLYGON_POINT;
        }

        for (i = 0; i < dat->nPointCount; i++)
        {
            nRet = PT_Decode_INT32(&nTemp, ptr, nBuffLen);
            dat->Points[i].dblLatitude = nTemp / JT808_MILLION;
            ptr += nRet;
            nBuffLen -= nRet;

            nRet = PT_Decode_INT32(&nTemp, ptr, nBuffLen);
            dat->Points[i].dblLongitude = nTemp / JT808_MILLION;
            ptr += nRet;
            nBuffLen -= nRet;
        }
    }

    return ptr - lpBuff;
}

int JT808_EncodeRegionPolygonSet(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    unsigned char i;
    unsigned int nTemp;
    uchar *ptr = lpBuff;
    TJT808RegionPolygonSet *dat = (TJT808RegionPolygonSet *)pPackage;

    nRet = PT_Encode_INT32(ptr, nBuffLen, dat->nRegionID);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nFlag);
    ptr += nRet;
    nBuffLen -= nRet;

    if (dat->nFlag & JT808_DEF_REGION_LIMIT_TIME)
    {
        *ptr++ = (((dat->tmStart.nYear / 10) % 10) << 4) | (dat->tmStart.nYear % 10);
        *ptr++ = ((dat->tmStart.nMonth / 10) << 4) | (dat->tmStart.nMonth % 10);
        *ptr++ = ((dat->tmStart.nDay / 10) << 4) | (dat->tmStart.nDay % 10);
        *ptr++ = ((dat->tmStart.nHour / 10) << 4) | (dat->tmStart.nHour % 10);
        *ptr++ = ((dat->tmStart.nMinute / 10) << 4) | (dat->tmStart.nMinute % 10);
        *ptr++ = ((dat->tmStart.nSecond / 10) << 4) | (dat->tmStart.nSecond % 10);

        *ptr++ = (((dat->tmEnd.nYear / 10) % 10) << 4) | (dat->tmEnd.nYear % 10);
        *ptr++ = ((dat->tmEnd.nMonth / 10) << 4) | (dat->tmEnd.nMonth % 10);
        *ptr++ = ((dat->tmEnd.nDay / 10) << 4) | (dat->tmEnd.nDay % 10);
        *ptr++ = ((dat->tmEnd.nHour / 10) << 4) | (dat->tmEnd.nHour % 10);
        *ptr++ = ((dat->tmEnd.nMinute / 10)  << 4) | (dat->tmEnd.nMinute % 10);
        *ptr++ = ((dat->tmEnd.nSecond / 10) << 4) | (dat->tmEnd.nSecond % 10);
        nBuffLen -= 12;
    }

    if (dat->nFlag & JT808_DEF_REGION_LIMIT_SPEED)
    {
        nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nMaxSpeed);
        ptr += nRet;
        nBuffLen -= nRet;

        *ptr++ = dat->nTimeLimit;
        nBuffLen--;
    }

    if (dat->nPointCount > 0 && dat->nPointCount <= JT808_DEF_MAX_NUM_REGION)
    {
        for (i = 0; i < dat->nPointCount; i++)
        {
            nTemp = dat->Points[i].dblLatitude * JT808_MILLION;
            nRet = PT_Encode_INT32(ptr, nBuffLen, nTemp);
            ptr += nRet;
            nBuffLen -= nRet;

            nTemp = dat->Points[i].dblLongitude * JT808_MILLION;
            nRet = PT_Encode_INT32(ptr, nBuffLen, nTemp);
            ptr += nRet;
            nBuffLen -= nRet;
        }
    }

    return ptr - lpBuff;
}

int JT808_DecodeRouteSet(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    unsigned char i;
    unsigned int nTemp;
    uchar *ptr = lpBuff;
    TJT808RouteSet *dat = (TJT808RouteSet *)pPackage;

    nRet = PT_Decode_INT32(&(dat->nRouteID), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Decode_SHORT(&(dat->nFlag), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    if (dat->nFlag & JT808_DEF_REGION_LIMIT_TIME)
    {
        dat->tmStart.nYear = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);
        if (dat->tmStart.nYear > 0)
        {
            dat->tmStart.nYear += 2000;
        }

        ptr++;
        dat->tmStart.nMonth = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

        ptr++;
        dat->tmStart.nDay = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

        ptr++;
        dat->tmStart.nHour = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

        ptr++;
        dat->tmStart.nMinute = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

        ptr++;
        dat->tmStart.nSecond = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);


        ptr++;
        dat->tmEnd.nYear = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);
        if (dat->tmEnd.nYear > 0)
        {
            dat->tmEnd.nYear += 2000;
        }

        ptr++;
        dat->tmEnd.nMonth = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

        ptr++;
        dat->tmEnd.nDay = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

        ptr++;
        dat->tmEnd.nHour = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

        ptr++;
        dat->tmEnd.nMinute = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

        ptr++;
        dat->tmEnd.nSecond = ((*ptr >> 4) & 0xF) * 10 + (*ptr & 0xF);

        ptr++;
        nBuffLen -= 12;
    }
    else
    {
        dat->tmStart.nYear = 0;
        dat->tmStart.nMonth = 0;
        dat->tmStart.nDay = 0;
        dat->tmStart.nHour = 0;
        dat->tmStart.nMinute = 0;
        dat->tmStart.nSecond = 0;

        dat->tmEnd.nYear = 0;
        dat->tmEnd.nMonth = 0;
        dat->tmEnd.nDay = 0;
        dat->tmEnd.nHour = 0;
        dat->tmEnd.nMinute = 0;
        dat->tmEnd.nSecond = 0;
    }

    nRet = PT_Decode_SHORT(&(dat->nCornerCount), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    if (dat->nCornerCount > 0)
    {
        if (dat->nCornerCount > JT808_DEF_MAX_NUM_ROUTE_CORNER)
        {
            dat->nCornerCount = JT808_DEF_MAX_NUM_ROUTE_CORNER;
        }

        for (i = 0; i < dat->nCornerCount; i++)
        {
            nRet = PT_Decode_INT32(&(dat->Corners[i].nCornerID), ptr, nBuffLen);
            ptr += nRet;
            nBuffLen -= nRet;

            nRet = PT_Decode_INT32(&(dat->Corners[i].nRoadID), ptr, nBuffLen);
            ptr += nRet;
            nBuffLen -= nRet;

            nRet = PT_Decode_INT32(&nTemp, ptr, nBuffLen);
            dat->Corners[i].ptCorner.dblLatitude = nTemp / JT808_MILLION;
            ptr += nRet;
            nBuffLen -= nRet;

            nRet = PT_Decode_INT32(&nTemp, ptr, nBuffLen);
            dat->Corners[i].ptCorner.dblLongitude = nTemp / JT808_MILLION;
            ptr += nRet;
            nBuffLen -= nRet;

            dat->Corners[i].nRoadWidth = *ptr++;
            nBuffLen--;

            dat->Corners[i].nFlag = *ptr++;
            nBuffLen--;

            if (dat->Corners[i].nFlag & JT808_DEF_ROAD_LIMIT_TIME)
            {
                nRet = PT_Decode_SHORT(&(dat->Corners[i].nMaxTime), ptr, nBuffLen);
                ptr += nRet;
                nBuffLen -= nRet;

                nRet = PT_Decode_SHORT(&(dat->Corners[i].nMinTime), ptr, nBuffLen);
                ptr += nRet;
                nBuffLen -= nRet;
            }
            else
            {
                dat->Corners[i].nMaxTime = 0;
                dat->Corners[i].nMinTime = 0;
            }

            if (dat->Corners[i].nFlag & JT808_DEF_ROAD_LIMIT_SPEED)
            {
                nRet = PT_Decode_SHORT(&(dat->Corners[i].nMaxSpeed), ptr, nBuffLen);
                ptr += nRet;
                nBuffLen -= nRet;

                dat->Corners[i].nTimeLimit = *ptr++;
                nBuffLen--;
            }
            else
            {
                dat->Corners[i].nMaxSpeed = 0;
                dat->Corners[i].nTimeLimit = 0;
            }
        }
    }

    return ptr - lpBuff;
}

int JT808_EncodeRouteSet(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    unsigned char i;
    unsigned int nTemp;
    uchar *ptr = lpBuff;
    TJT808RouteSet *dat = (TJT808RouteSet *)pPackage;

    nRet = PT_Encode_INT32(ptr, nBuffLen, dat->nRouteID);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nFlag);
    ptr += nRet;
    nBuffLen -= nRet;

    if (dat->nFlag & JT808_DEF_REGION_LIMIT_TIME)
    {
        *ptr++ = (((dat->tmStart.nYear / 10) % 10) << 4) | (dat->tmStart.nYear % 10);
        *ptr++ = ((dat->tmStart.nMonth / 10) << 4) | (dat->tmStart.nMonth % 10);
        *ptr++ = ((dat->tmStart.nDay / 10) << 4) | (dat->tmStart.nDay % 10);
        *ptr++ = ((dat->tmStart.nHour / 10) << 4) | (dat->tmStart.nHour % 10);
        *ptr++ = ((dat->tmStart.nMinute / 10) << 4) | (dat->tmStart.nMinute % 10);
        *ptr++ = ((dat->tmStart.nSecond / 10) << 4) | (dat->tmStart.nSecond % 10);

        *ptr++ = (((dat->tmEnd.nYear / 10) % 10) << 4) | (dat->tmEnd.nYear % 10);
        *ptr++ = ((dat->tmEnd.nMonth / 10) << 4) | (dat->tmEnd.nMonth % 10);
        *ptr++ = ((dat->tmEnd.nDay / 10) << 4) | (dat->tmEnd.nDay % 10);
        *ptr++ = ((dat->tmEnd.nHour / 10) << 4) | (dat->tmEnd.nHour % 10);
        *ptr++ = ((dat->tmEnd.nMinute / 10)  << 4) | (dat->tmEnd.nMinute % 10);
        *ptr++ = ((dat->tmEnd.nSecond / 10) << 4) | (dat->tmEnd.nSecond % 10);

        nBuffLen -= 12;
    }

    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nCornerCount);
    ptr += nRet;
    nBuffLen -= nRet;

    if (dat->nCornerCount > 0 && dat->nCornerCount <= JT808_DEF_MAX_NUM_ROUTE_CORNER)
    {
        for (i = 0; i < dat->nCornerCount; i++)
        {
            nRet = PT_Encode_INT32(ptr, nBuffLen, dat->Corners[i].nCornerID);
            ptr += nRet;
            nBuffLen -= nRet;

            nRet = PT_Encode_INT32(ptr, nBuffLen, dat->Corners[i].nRoadID);
            ptr += nRet;
            nBuffLen -= nRet;

            nTemp = dat->Corners[i].ptCorner.dblLatitude * JT808_MILLION;
            nRet = PT_Encode_INT32(ptr, nBuffLen, nTemp);
            ptr += nRet;
            nBuffLen -= nRet;

            nTemp = dat->Corners[i].ptCorner.dblLongitude * JT808_MILLION;
            nRet = PT_Encode_INT32(ptr, nBuffLen, nTemp);
            ptr += nRet;
            nBuffLen -= nRet;

            *ptr++ = dat->Corners[i].nRoadWidth;
            nBuffLen--;

            *ptr++ = dat->Corners[i].nFlag;
            nBuffLen--;

            if (dat->Corners[i].nFlag & JT808_DEF_ROAD_LIMIT_TIME)
            {
                nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->Corners[i].nMaxTime);
                ptr += nRet;
                nBuffLen -= nRet;

                nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->Corners[i].nMinTime);
                ptr += nRet;
                nBuffLen -= nRet;
            }

            if (dat->Corners[i].nFlag & JT808_DEF_ROAD_LIMIT_SPEED)
            {
                nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->Corners[i].nMaxSpeed);
                ptr += nRet;
                nBuffLen -= nRet;

                *ptr++ = dat->Corners[i].nTimeLimit;
                nBuffLen--;
            }
        }
    }

    return ptr - lpBuff;
}

int JT808_DecodeDrivingRecorderReq(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    uchar *ptr = lpBuff;
    TJT808DriveingRecorderReq *dat = (TJT808DriveingRecorderReq *)pPackage;

    dat->nType = *ptr++;
    nBuffLen--;
    
    dat->nDataLen = nBuffLen;
    if (nBuffLen > 0)
    {
        memcpy(dat->data, ptr, nBuffLen);
        ptr += nBuffLen;
    }

    return ptr - lpBuff;
}

int JT808_EncodeDrivingRecorderReq(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    uchar *ptr = lpBuff;
    TJT808DriveingRecorderReq *dat = (TJT808DriveingRecorderReq *)pPackage;

    *ptr++ = dat->nType;
    nBuffLen--;
    
    if (dat->nDataLen > 0)
    {
        memcpy(ptr, dat->data, dat->nDataLen);
        ptr += dat->nDataLen;
        nBuffLen -= dat->nDataLen;
    }
    
    return ptr - lpBuff;
}

int JT808_DecodeDrivingRecorderRes(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808DriveingRecorderRes *dat = (TJT808DriveingRecorderRes *)pPackage;

    if (dat->head.nPacketSeq == 1)//�װ�����������Ϣ
    {
        nRet = PT_Decode_SHORT(&(dat->nSeqNum), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;

        dat->nType = *ptr++;
        nBuffLen--;
    }
    
    if (nBuffLen > sizeof(dat->data))
    {
        dat->nDataLen = sizeof(dat->data);
    }
    else
    {
        dat->nDataLen = nBuffLen;
    }

    memcpy(dat->data, ptr, dat->nDataLen);

    ptr += nBuffLen;
    nBuffLen -= nBuffLen;

    return ptr - lpBuff;
}

int JT808_EncodeDrivingRecorderRes(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808DriveingRecorderRes *dat = (TJT808DriveingRecorderRes *)pPackage;

    if (dat->head.nPacketSeq == 1)//�װ�����������Ϣ
    {
        nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nSeqNum);
        ptr += nRet;
        nBuffLen -= nRet;

        *ptr++ = dat->nType;
        nBuffLen--;
    }
    
    if (dat->nDataLen > 0)
    {
        memcpy(ptr, dat->data, dat->nDataLen);

        ptr += dat->nDataLen;
        nBuffLen -= dat->nDataLen;
    }
    
    return ptr - lpBuff;
}

int JT808_DecodeDrivingRecorderParam(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    uchar *ptr = lpBuff;
    TJT808DriveingRecorderParam *dat = (TJT808DriveingRecorderParam *)pPackage;

    dat->nType = *ptr++;
    nBuffLen--;

    if (nBuffLen > sizeof(dat->data))
    {
        dat->nDataLen = sizeof(dat->data);
    }
    else
    {
        dat->nDataLen = nBuffLen;
    }

    memcpy(dat->data, ptr, dat->nDataLen);

    ptr += nBuffLen;
    nBuffLen -= nBuffLen;

    return ptr - lpBuff;
}

int JT808_EncodeDrivingRecorderParam(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    uchar *ptr = lpBuff;
    TJT808DriveingRecorderParam *dat = (TJT808DriveingRecorderParam *)pPackage;

    *ptr++ = dat->nType;
    nBuffLen--;

    if (dat->nDataLen > 0)
    {
        memcpy(ptr, dat->data, dat->nDataLen);

        ptr += dat->nDataLen;
        nBuffLen -= dat->nDataLen;
    }
    
    return ptr - lpBuff;
}


int JT808_DecodeOrderReport(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808OrderReport *dat = (TJT808OrderReport *)pPackage;

    nRet = PT_Decode_INT32(&(dat->nDataLen), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    if (nRet > sizeof(dat->data))
    {
        memcpy(dat->data, ptr, sizeof(dat->data));
    }
    else
    {
        memcpy(dat->data, ptr, nRet);
    }

    ptr += nRet;
    nBuffLen -= nRet;
    
    return ptr - lpBuff;
}

int JT808_EncodeOrderReport(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808OrderReport *dat = (TJT808OrderReport *)pPackage;

    nRet = PT_Encode_INT32(ptr, nBuffLen, dat->nDataLen);
    ptr += nRet;
    nBuffLen -= nRet;

    memcpy(ptr, dat->data, dat->nDataLen);

    ptr += dat->nDataLen;
    nBuffLen -= dat->nDataLen;

    return ptr - lpBuff;
}
//��ʻԱ�����Ϣ�ɼ��ϱ�
int JT808_DecodeDriverInfoReport(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808DriverInfoReport *dat = (TJT808DriverInfoReport *)pPackage;

#ifdef PT_JT808_VER_201301
	//״̬
    dat->nStatus = *ptr++;
	nBuffLen--;
	//ʱ��
	dat->time.nYear  = (*ptr >> 4) * 10 +(*ptr++ & 0xF);
	dat->time.nMonth = (*ptr >> 4) * 10 +(*ptr++ & 0xF);
	dat->time.nDay  = (*ptr >> 4) * 10 +(*ptr++ & 0xF);
	dat->time.nHour = (*ptr >> 4) * 10 +(*ptr++ & 0xF);
	dat->time.nMinute = (*ptr >> 4) * 10 +(*ptr++ & 0xF);
	dat->time.nSecond = (*ptr >> 4) * 10 +(*ptr++ & 0xF);
	if (dat->time.nYear)
	{
		dat->time.nYear += 2000;
	}
    if (dat->nStatus == 0x01)
    {
        dat->nICStatus = *ptr++;
        nBuffLen--;
        
        if (dat->nICStatus != 0x00)
        {
            return ptr - lpBuff; 
        }
    }
    else
    {
        return ptr - lpBuff; 
    }
#endif	
	//name
    nRet = *ptr++; //name length
    nBuffLen--;

    if (nRet >= sizeof(dat->szName) - 1)
    {
        memcpy(dat->szName, ptr, sizeof(dat->szName) - 1);
        dat->szName[sizeof(dat->szName) - 1] = '\0';
    }
    else
    {
        memcpy(dat->szName, ptr, nRet);
        dat->szName[nRet] = '\0';
    }
    ptr += nRet;
    nBuffLen -= nRet;
    
#ifndef PT_JT808_VER_201301
    //id card
    nRet = 20;
    memcpy(dat->szIDCard, ptr, nRet);
    dat->szIDCard[nRet] = '\0';
    ptr += nRet;
    nBuffLen -= nRet;
#endif
    //Certificate ID
    nRet = 40;
    memcpy(dat->szCertificateID, ptr, nRet);
    dat->szCertificateID[nRet] = '\0';
    ptr += nRet;
    nBuffLen -= nRet;

    //Issue Department
    nRet = *ptr++;
    nBuffLen--;

    if (nRet >= sizeof(dat->szIssueDepartment) - 1)
    {
        memcpy(dat->szIssueDepartment, ptr, sizeof(dat->szIssueDepartment) - 1);
        dat->szIssueDepartment[sizeof(dat->szIssueDepartment) - 1] = '\0';
    }
    else
    {
        memcpy(dat->szIssueDepartment, ptr, nRet);
        dat->szIssueDepartment[nRet] = '\0';
    }
    ptr += nRet;
    nBuffLen -= nRet;
    
#ifdef PT_JT808_VER_201301
    memcpy(dat->szUsefulLife, ptr, 4);
    ptr += 4;
    nBuffLen -= 4;
#endif    
    return ptr - lpBuff;
}
//��ʻԱ�����Ϣ�ɼ��ϱ�
int JT808_EncodeDriverInfoReport(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808DriverInfoReport *dat = (TJT808DriverInfoReport *)pPackage;

#ifdef PT_JT808_VER_201301
	//״̬
	*ptr++ = dat->nStatus;
	nBuffLen--;
	//ʱ��
	*ptr++ = ((dat->time.nYear / 10 % 10)<<4) + (dat->time.nYear % 10);
	*ptr++ = ((dat->time.nMonth / 10 % 10)<<4) + (dat->time.nMonth % 10);
	*ptr++ = ((dat->time.nDay / 10 % 10)<<4) + (dat->time.nDay % 10);
	*ptr++ = ((dat->time.nHour / 10 % 10)<<4) + (dat->time.nHour % 10);
	*ptr++ = ((dat->time.nMinute / 10 % 10)<<4) + (dat->time.nMinute % 10);
	*ptr++ = ((dat->time.nSecond / 10 % 10)<<4) + (dat->time.nSecond % 10);

    if (dat->nStatus == 0x01)
    {
        *ptr++ = dat->nICStatus;
        nBuffLen--;
        
        if (dat->nICStatus != 0x00)
        {
            return ptr - lpBuff; 
        }
    }
    else
    {
        return ptr - lpBuff; 
    }
#endif	
    //name
    nRet = strlen(dat->szName) + 1;//����������
    *ptr++ = nRet;
    nBuffLen--;

    memcpy(ptr, dat->szName, nRet);
    ptr += nRet;
    nBuffLen -= nRet;
#ifdef PT_JT808_VER_201301
    //Certificate ID
    nRet = strlen(dat->szCertificateID);
    if (nRet < 20)
    {
        memcpy(ptr, dat->szCertificateID, nRet);
        memset(ptr + nRet, 0, 20 - nRet);
    }
    else
    {
        memcpy(ptr, dat->szCertificateID, 20);
        ptr += 20;
    }
    ptr += 20;
    nBuffLen -= 20;
#else    
    //id card
    nRet = strlen(dat->szIDCard);
    if (nRet < 20)
    {
        memcpy(ptr, dat->szIDCard, nRet);
        memset(ptr + nRet, 0, 20 - nRet);
    }
    else
    {
        memcpy(ptr, dat->szIDCard, 20);
    }
    ptr += 20;
    nBuffLen -= 20;
    
    //Certificate ID
    nRet = strlen(dat->szCertificateID);
    if (nRet < 40)
    {
        memcpy(ptr, dat->szCertificateID, nRet);
        memset(ptr + nRet, 0, 40 - nRet);
    }
    else
    {
        memcpy(ptr, dat->szCertificateID, 40);
    }
    ptr += 40;
    nBuffLen -= 40;
#endif
   
    //Issue Department
    nRet = strlen(dat->szIssueDepartment) + 1;//����������
    *ptr++ = nRet;
    nBuffLen--;

    memcpy(ptr, dat->szIssueDepartment, nRet);
    ptr += nRet;
    nBuffLen -= nRet;

#ifdef PT_JT808_VER_201301
    memcpy(ptr, dat->szUsefulLife, 4);
    ptr += 4;
    nBuffLen -= 4;
#endif    
    return ptr - lpBuff;
}
//��λ���������ϴ�  Ver201301
int JT808_DecodePosDataUpload(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet,i = 0;
	unsigned short nLen = 0;
	uchar *ptr = lpBuff;
	TJT808PosDataUpload *dat = (TJT808PosDataUpload *)pPackage;	
    
    if (dat->head.nPacketSeq == 1)
    {
        //���������
        nRet = PT_Decode_SHORT(&(dat->nTotalCount), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;

        //λ����������
        dat->nPosType = *ptr++;
        nBuffLen--;
    }

#ifdef JT808_PLATFORM	
	//λ�û㱨������
    dat->nDataCount = 0;
	while (i = 0; i < dat->nDataCount; i++)
	{
		//λ�û㱨�����峤��
		nRet = PT_Decode_SHORT(&nLen, ptr, nBuffLen);
		ptr += nRet;
		nBuffLen -= nRet;
		if (nLen <= nBuffLen)
		{
			nRet = JT808_DecodePosition(&(dat->posdata[dat->nDataCount++]), ptr, nLen);
			ptr += nRet;
			nBuffLen -= nRet;

            if(nBuffLen < 2)
                break;
		}
        else
        {
            break;
        }
	}
#else
    dat->nDataLen = nBuffLen;
    memcpy(dat->data, ptr, nBuffLen);
    ptr += nBuffLen;
#endif    
	return (ptr - lpBuff);
}
//��λ���������ϴ�  Ver201301
int JT808_EncodePosDataUpload(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet,i = 0;
	unsigned short nLen = 0;
	uchar *ptr = lpBuff;
	TJT808PosDataUpload *dat = (TJT808PosDataUpload *)pPackage;	

    if (dat->head.nPacketSeq == 1)
    {
        //���������
        nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nTotalCount);
        ptr += nRet;
        nBuffLen -= nRet;

        //λ����������
        *ptr++ = dat->nPosType;
        nBuffLen--;
    }
#ifdef JT808_PLATFORM
	//λ�û㱨������
	for(i = 0; i < dat->nDataCount; i++)
	{
		nLen = JT808_EncodePosition(ptr + 2, nBuffLen, &(dat->posdata[i]));
		
		//λ�û㱨�����峤��
		nRet = PT_Encode_SHORT(ptr, nBuffLen, nLen);
		ptr += nRet;
		nBuffLen -= nRet;	
        
        ptr += nLen;
		nBuffLen -= nLen;

		if(nBuffLen < 2)
			break;
	}
#else
    memcpy(ptr, dat->data, dat->nDataLen);
    ptr += dat->nDataLen;
#endif    
	return (ptr - lpBuff);
}
//CAN���������ϴ�  Ver201301
int JT808_DecodeCanDataUpload(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet;
	uchar *ptr = lpBuff;
	TJT808CanDataUpload *dat = (TJT808CanDataUpload *)pPackage;	

    if (dat->head.nPacketSeq == 1)
    {
        //���������
        nRet = PT_Decode_SHORT(&(dat->nTotalCount), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;
        //CAN �������ݽ���ʱ��
        dat->nHour = ((*ptr>>4) & 0xF) * 10 + ((*ptr++) & 0xF);
        dat->nMinute = ((*ptr>>4) & 0xF)*10 + ((*ptr++) & 0xF);
        dat->nSecond = ((*ptr>>4) & 0xF)*10 + ((*ptr++) & 0xF);
        
        dat->nmSecond =  ((*ptr >> 4) & 0xF) * 10 + ((*ptr++) & 0xF);
        dat->nmSecond *= 100;
        dat->nmSecond += (((*ptr >> 4) & 0xF) * 10 + ((*ptr++) & 0xF));
	}
    
    dat->nDataCount = 0;    
	while (nBuffLen >= 12)
	{
        dat->CanParam[dat->nDataCount].nChannel = (*ptr >> 7) & 0x01;
        dat->CanParam[dat->nDataCount].nFrameType = (*ptr >> 6) & 0x01;
        dat->CanParam[dat->nDataCount].nMode = (*ptr >> 5) & 0x01;
        dat->CanParam[dat->nDataCount].nCanID = *ptr & 0x1F;
        ptr++;
        
        dat->CanParam[dat->nDataCount].nCanID <<= 8;
        dat->CanParam[dat->nDataCount].nCanID |= *ptr++;
        dat->CanParam[dat->nDataCount].nCanID <<= 8;
        dat->CanParam[dat->nDataCount].nCanID |= *ptr++;
        dat->CanParam[dat->nDataCount].nCanID <<= 8;
        dat->CanParam[dat->nDataCount].nCanID |= *ptr++;
        nBuffLen -= 4;
        
		memcpy(dat->CanData[dat->nDataCount], ptr, 8);
		ptr += 8;
		nBuffLen -= 8;
        
        dat->nDataCount++;
	}
	return ptr - lpBuff;
}
//CAN���������ϴ�  Ver201301
int JT808_EncodeCanDataUpload(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet;
    unsigned char i = 0;
	uchar *ptr = lpBuff;
	TJT808CanDataUpload *dat = (TJT808CanDataUpload *)pPackage;	
    
    if (dat->head.nPacketSeq == 1)
    {
        //���������
        nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nTotalCount);
        ptr += nRet;
        nBuffLen -= nRet;
        //CAN �������ݽ���ʱ��
        *ptr++ = ((dat->nHour/10)   << 4) | (dat->nHour   % 10);
        *ptr++ = ((dat->nMinute/10) << 4) | (dat->nMinute % 10);
        *ptr++ = ((dat->nSecond/10) << 4) | (dat->nSecond % 10);
        
        i = dat->nmSecond / 100;
        *ptr++ = ((i / 10) << 4) | (i % 10);
        i = dat->nmSecond % 100;
        *ptr++ = ((i / 10) << 4) | (i % 10);
	}
    
	for(i = 0; i < dat->nDataCount; i++)
	{
        *ptr = 0;
        if (dat->CanParam[i].nChannel == 1)
        {
            *ptr |= (1 << 7);
        }
        
        if (dat->CanParam[i].nFrameType == 1)
        {
            *ptr |= (1 << 6);
        }
        
        if (dat->CanParam[i].nMode == 1)
        {
            *ptr |= (1 << 5);
        }
        
        *ptr++ |= ((dat->CanParam[i].nCanID >> 24) & 0x1F);
        *ptr++  = ((dat->CanParam[i].nCanID >> 16) & 0xFF);
        *ptr++  = ((dat->CanParam[i].nCanID >> 8) & 0xFF);
        *ptr++  = (dat->CanParam[i].nCanID & 0xFF);
        nBuffLen -= 4;
        
		memcpy(ptr, dat->CanData[i], 8);
		ptr += 8;
		nBuffLen -= 8;
        
		if(nBuffLen < 12)
			break;
	}
    
	return ptr - lpBuff;
}
int JT808_DecodeMediaEventReport(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808MediaEventReport *dat = (TJT808MediaEventReport *)pPackage;

    nRet = PT_Decode_INT32(&(dat->nMediaID), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    dat->nMediaType = *ptr++;
    nBuffLen--;

    dat->nCodeType = *ptr++;
    nBuffLen--;

    dat->nEventType = *ptr++;
    nBuffLen--;

    dat->nChannel = *ptr++;
    nBuffLen--;

    return ptr - lpBuff;
}

int JT808_EncodeMediaEventReport(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808MediaEventReport *dat = (TJT808MediaEventReport *)pPackage;

    nRet = PT_Encode_INT32(ptr, nBuffLen, dat->nMediaID);
    ptr += nRet;
    nBuffLen -= nRet;

    *ptr++ = dat->nMediaType;
    nBuffLen--;

    *ptr++ = dat->nCodeType;
    nBuffLen--;

    *ptr++ = dat->nEventType;
    nBuffLen--;

    *ptr++ = dat->nChannel;
    nBuffLen--;

    return ptr - lpBuff;
}

int JT808_DecodeMediaDataUploadReq(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808MediaDataUploadReq *dat = (TJT808MediaDataUploadReq *)pPackage;

    if (dat->head.nPacketSeq == 1)//��ý�������װ�������������
    {
        nRet = PT_Decode_INT32(&(dat->nMediaID), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;

        dat->nMediaType = *ptr++;
        nBuffLen--;

        dat->nCodeType = *ptr++;
        nBuffLen--;

        dat->nEventType = *ptr++;
        nBuffLen--;

        dat->nChannel = *ptr++;
        nBuffLen--;
	    //Ver201301
//    #ifdef PT_JT808_VER_201301//��������λ����Ϣ
	    nRet = JT808_DecodePositionState(&(dat->position), ptr,28);
	    nBuffLen -= nRet;
	    ptr += nRet;
//    #endif
    }
    dat->nDataLen = nBuffLen;
    if (nBuffLen > sizeof(dat->data))
    {
        dat->nDataLen = sizeof(dat->data);
    }

    memcpy(dat->data, ptr, dat->nDataLen);
    
    ptr += dat->nDataLen;
    nBuffLen -= dat->nDataLen;

    return ptr - lpBuff;
}

int JT808_EncodeMediaDataUploadReq(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808MediaDataUploadReq *dat = (TJT808MediaDataUploadReq *)pPackage;

    if (dat->head.nPacketSeq == 1)//��ý�������װ�������������
    {
        nRet = PT_Encode_INT32(ptr, nBuffLen, dat->nMediaID);
        ptr += nRet;
        nBuffLen -= nRet;

        *ptr++ = dat->nMediaType;
        nBuffLen--;

        *ptr++ = dat->nCodeType;
        nBuffLen--;

        *ptr++ = dat->nEventType;
        nBuffLen--;

        *ptr++ = dat->nChannel;
        nBuffLen--;

        //Ver201301
//#ifdef PT_JT808_VER_201301
        nRet = JT808_EncodePositionState(ptr, 28, &(dat->position));
        nBuffLen -= nRet;
        ptr += nRet;
//#endif
    }

    if (dat->nDataLen > 0)
    {
        if (dat->nDataLen <= nBuffLen)
        {
            memcpy(ptr, dat->data, dat->nDataLen);

            ptr += dat->nDataLen;
            nBuffLen -= dat->nDataLen;
        }
        else
        {
            return -1;
        }
    }

    return ptr - lpBuff;
}

int JT808_DecodeMediaDataUploadRes(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808MediaDataUploadRes *dat = (TJT808MediaDataUploadRes *)pPackage;

    nRet = PT_Decode_INT32(&(dat->nMediaID), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    dat->nResendCount = *ptr++;
    nBuffLen--;

    if (dat->nResendCount > 0)
    {
        if (dat->nResendCount == nBuffLen && dat->nResendCount <= sizeof(dat->ResendIDList))
        {
            memcpy(dat->ResendIDList, ptr, dat->nResendCount);
            ptr += dat->nResendCount;
            nBuffLen -= dat->nResendCount;        
        }
        else
        {
            return -1;
        }
    }

    return ptr - lpBuff;
}

int JT808_EncodeMediaDataUploadRes(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808MediaDataUploadRes *dat = (TJT808MediaDataUploadRes *)pPackage;

    nRet = PT_Encode_INT32(ptr, nBuffLen, dat->nMediaID);
    ptr += nRet;
    nBuffLen -= nRet;

    *ptr++ = dat->nResendCount;
    nBuffLen--;

    if (dat->nResendCount > 0)
    {
        if (dat->nResendCount <= nBuffLen)
        {
            memcpy(ptr, dat->ResendIDList, dat->nResendCount);

            ptr += dat->nResendCount;
            nBuffLen -= dat->nResendCount;
        }
        else
        {
            return -1;
        }
    }

    return ptr - lpBuff;
}

/*����ͷ��������ָ�� ����*/
int JT808_DecodeCameralCtrl(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808CameralCtrl *dat = (TJT808CameralCtrl *)pPackage;
    
	dat->nChannel= *ptr++;
    nBuffLen--;

	nRet = PT_Decode_SHORT(&(dat->nType), ptr, nBuffLen);//�����־
    ptr += nRet;
    nBuffLen -= nRet;

	nRet = PT_Decode_SHORT(&(dat->nInterval), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;
	
    dat->nSaveFlag = *ptr++;//�����־
    nBuffLen--;

    dat->nDistinguishability = *ptr++;//�ֱ���
    nBuffLen--;

	dat->nPictureQuality = *ptr++;//ͼ��/��Ƶ����
    nBuffLen--;

	dat->nBrightness = *ptr++;//����
    nBuffLen--;

	dat->nContrast = *ptr++;//�Աȶ�
    nBuffLen--;

	dat->nSaturation = *ptr++;//���Ͷ�
    nBuffLen--;

	dat->nChroma = *ptr++;//ɫ��
    nBuffLen--;

    return ptr - lpBuff;
    
    
}
/*����ͷ��������ָ�� ����*/
int JT808_EncodeCameralCtrl(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808CameralCtrl *dat = (TJT808CameralCtrl *)pPackage;

    *ptr++ = dat->nChannel;
    nBuffLen--;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nType);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nInterval);
    ptr += nRet;
    nBuffLen -= nRet;
	
    *ptr++ = dat->nSaveFlag;
    nBuffLen--;

    *ptr++ = dat->nDistinguishability;
    nBuffLen--;

	*ptr++ = dat->nPictureQuality;
    nBuffLen--;

	*ptr++ = dat->nBrightness;
    nBuffLen--;

	*ptr++ = dat->nContrast;
    nBuffLen--;

	*ptr++ = dat->nSaturation;
    nBuffLen--;

	*ptr++ = dat->nChroma;
    nBuffLen--;

    return ptr - lpBuff;
}
//����ͷ������������Ӧ�� Ver201301
int JT808_DecodeCameralCtrlRes(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet,i = 0;
	uchar *ptr = lpBuff;
	TJT808CameraCtrlRes *dat = (TJT808CameraCtrlRes *)pPackage;

	nRet = PT_Decode_SHORT(&(dat->nSeqNum), ptr, nBuffLen);//Ӧ����ˮ��
	ptr += nRet;
	nBuffLen -= nRet;

	dat->nResult = *ptr++;//���
	nBuffLen--;

	//����ɹ��Ķ�ý�����
	nRet = PT_Decode_SHORT(&(dat->nMediaCount), ptr, nBuffLen);
	ptr += nRet;
	nBuffLen -= nRet;

	//��ý�� ID �б�
	for (i = 0; i < dat->nMediaCount; i++)
	{
		nRet = PT_Decode_INT32(&(dat->MediaIDList[i]), ptr, nBuffLen);
		ptr += nRet;
		nBuffLen -= nRet;
		if(nBuffLen < 4)
			break;
	}
	return ptr - lpBuff;
}
//����ͷ������������Ӧ�� Ver201301
int JT808_EncodeCameralCtrlRes(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet,i = 0;
	uchar *ptr = lpBuff;
	TJT808CameraCtrlRes *dat = (TJT808CameraCtrlRes *)pPackage;

	nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nSeqNum);//Ӧ����ˮ��
	ptr += nRet;
	nBuffLen -= nRet;

	*ptr++ = dat->nResult;//���
	nBuffLen--;

	//����ɹ��Ķ�ý����� 
	nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nMediaCount);
	ptr += nRet;
	nBuffLen -= nRet;

	//��ý�� ID �б�
	for (i = 0; i < dat->nMediaCount; i++)
	{
		nRet = PT_Encode_INT32(ptr, nBuffLen, dat->MediaIDList[i]);
		ptr += nRet;
		nBuffLen -= nRet;
		if(nBuffLen < 4)
			break;
	}
	return ptr - lpBuff;
}
/*�洢��ý�����ݼ��� ����*/
int JT808_DecodeMediaIndexReq(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    uchar *ptr = lpBuff;
    TJT808MediaIndexReq *dat = (TJT808MediaIndexReq *)pPackage;

	dat->nMedia = *ptr++;
	nBuffLen--;

    dat->nChannel= *ptr++;
	nBuffLen--;

	dat->nEventType= *ptr++;
	nBuffLen--;
    
    dat->tmStart.nYear =  BCD2BIN(*ptr);
    if (dat->tmStart.nYear > 0)
    {
        dat->tmStart.nYear += 2000;
    }

    ptr++;
    dat->tmStart.nMonth = BCD2BIN(*ptr);

    ptr++;
    dat->tmStart.nDay = BCD2BIN(*ptr);

    ptr++;
    dat->tmStart.nHour = BCD2BIN(*ptr);

    ptr++;
    dat->tmStart.nMinute = BCD2BIN(*ptr);

    ptr++;
    dat->tmStart.nSecond = BCD2BIN(*ptr);


    ptr++;
    dat->tmEnd.nYear = BCD2BIN(*ptr);
    if (dat->tmEnd.nYear > 0)
    {
        dat->tmEnd.nYear += 2000;
    }

    ptr++;
    dat->tmEnd.nMonth = BCD2BIN(*ptr);

    ptr++;
    dat->tmEnd.nDay = BCD2BIN(*ptr);

    ptr++;
    dat->tmEnd.nHour = BCD2BIN(*ptr);

    ptr++;
    dat->tmEnd.nMinute = BCD2BIN(*ptr);

    ptr++;
    dat->tmEnd.nSecond = BCD2BIN(*ptr);

    ptr++;
    nBuffLen -= 12;

	return ptr - lpBuff;
}
/*�洢��ý�����ݼ��� ����*/
int JT808_EncodeMediaIndexReq(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    uchar *ptr = lpBuff;
    TJT808MediaIndexReq *dat = (TJT808MediaIndexReq *)pPackage;

	*ptr++ = dat->nMedia;
	nBuffLen --;

	*ptr++ = dat->nChannel;
	nBuffLen --;

	*ptr++ = dat->nEventType;
	nBuffLen --;
	
    *ptr++ = (((dat->tmStart.nYear / 10) % 10) << 4) | (dat->tmStart.nYear % 10);//��ʼʱ��
    *ptr++ = BIN2BCD(dat->tmStart.nMonth);
    *ptr++ = BIN2BCD(dat->tmStart.nDay);
    *ptr++ = BIN2BCD(dat->tmStart.nHour);
    *ptr++ = BIN2BCD(dat->tmStart.nMinute);
    *ptr++ = BIN2BCD(dat->tmStart.nSecond);
    nBuffLen -= 6;

    *ptr++ = (((dat->tmEnd.nYear / 10) % 10) << 4) | (dat->tmEnd.nYear % 10);//����ʱ��
    *ptr++ = BIN2BCD(dat->tmEnd.nMonth);
    *ptr++ = BIN2BCD(dat->tmEnd.nDay);
    *ptr++ = BIN2BCD(dat->tmEnd.nHour);
    *ptr++ = BIN2BCD(dat->tmEnd.nMinute);
    *ptr++ = BIN2BCD(dat->tmEnd.nSecond);
    nBuffLen -= 6;

	return ptr - lpBuff;
}
/*�洢��ý�����ݼ���Ӧ�� ����*/
int JT808_DecodeMediaIndexRes(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet,i;
    uchar *ptr = lpBuff;
    TJT808MediaIndexRes *dat = (TJT808MediaIndexRes *)pPackage;

	nRet = PT_Decode_SHORT(&(dat->nSeqNum), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;
    
    nRet = PT_Decode_SHORT(&(dat->nIndexcount), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;
    
    if(dat->nIndexcount > 0 )
	{
        if (dat->nIndexcount > JT808_DEF_MAX_NUM_INDEX_MEDIA)
        {
            dat->nIndexcount = JT808_DEF_MAX_NUM_INDEX_MEDIA;
        }
        
        for(i = 0; i < dat->nIndexcount; i++)
		{
#ifdef PT_JT808_VER_201301
			nRet = PT_Decode_INT32(&(dat->IndexData[i].nMediaID), ptr, nBuffLen);//Ver201301
			ptr += nRet;
			nBuffLen -= nRet;      
#endif
			dat->IndexData[i].nMedia = *ptr++;
			nBuffLen -= nRet;

			dat->IndexData[i].nChannel= *ptr++;
			nBuffLen -= nRet;

			dat->IndexData[i].nEventType= *ptr++;
			nBuffLen -= nRet;

			nRet = JT808_DecodePositionState(&(dat->IndexData[i].position), ptr, nBuffLen);
			ptr += nRet;
			nBuffLen -= nRet;
        }
    }
    
    return ptr - lpBuff;
	
}
/*�洢��ý�����ݼ���Ӧ��  ����*/
int JT808_EncodeMediaIndexRes(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet,i;
    uchar *ptr = lpBuff;
    TJT808MediaIndexRes *dat = (TJT808MediaIndexRes *)pPackage;

	nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nSeqNum);
	ptr +=nRet;
	nBuffLen -= nRet;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nIndexcount);
	ptr +=nRet;
	nBuffLen -= nRet;

    if(dat->nIndexcount>0)
    {
        if(dat->nIndexcount > JT808_DEF_MAX_NUM_INDEX_MEDIA)
        {
			dat->nIndexcount = JT808_DEF_MAX_NUM_INDEX_MEDIA;
        }
        
		for(i = 0; i < dat->nIndexcount; i++)
	    {
#ifdef PT_JT808_VER_201301
			nRet = PT_Encode_INT32(ptr, nBuffLen, dat->IndexData[i].nMediaID);
			ptr +=nRet;
			nBuffLen -= nRet;
#endif
			*ptr++ = dat->IndexData[i].nMedia;
	        nBuffLen --;

	        *ptr++ = dat->IndexData[i].nChannel;
	        nBuffLen --;

	        *ptr++ = dat->IndexData[i].nEventType;
	        nBuffLen --;

	        nRet = JT808_EncodePositionState(ptr, nBuffLen, &(dat->IndexData[i].position));
            ptr += nRet;
            nBuffLen -= nRet;
        }
    }
	
    return ptr - lpBuff;
}

/*�洢��ý�������ϴ����� ����*/
int JT808_DecodeMediaDataUpload(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    uchar *ptr = lpBuff;
    TJT808MediaDataUpload *dat = (TJT808MediaDataUpload *)pPackage;

	dat->nMedia = *ptr++;//��ý������
	nBuffLen--;

    dat->nChannel = *ptr++;//ͨ��ID
	nBuffLen--;

	dat->nEventType = *ptr++;//�¼������
	nBuffLen--;

    dat->tmStart.nYear = BCD2BIN(*ptr);
    if (dat->tmStart.nYear > 0)
    {
        dat->tmStart.nYear += 2000;
    }

    ptr++;
    dat->tmStart.nMonth = BCD2BIN(*ptr);

    ptr++;
    dat->tmStart.nDay = BCD2BIN(*ptr);

    ptr++;
    dat->tmStart.nHour = BCD2BIN(*ptr);

    ptr++;
    dat->tmStart.nMinute = BCD2BIN(*ptr);

    ptr++;
    dat->tmStart.nSecond = BCD2BIN(*ptr);


    ptr++;
    dat->tmEnd.nYear = BCD2BIN(*ptr);
    if (dat->tmEnd.nYear > 0)
    {
        dat->tmEnd.nYear += 2000;
    }

    ptr++;
    dat->tmEnd.nMonth = BCD2BIN(*ptr);

    ptr++;
    dat->tmEnd.nDay = BCD2BIN(*ptr);

    ptr++;
    dat->tmEnd.nHour = BCD2BIN(*ptr);

    ptr++;
    dat->tmEnd.nMinute = BCD2BIN(*ptr);

    ptr++;
    dat->tmEnd.nSecond = BCD2BIN(*ptr);

    ptr++;
    nBuffLen -= 12;

	dat->nDelFlag = *ptr++;
	nBuffLen--;

	return ptr - lpBuff;
}
/*�洢��ý�������ϴ�����  ����*/
int JT808_EncodeMediaDataUpload(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    uchar *ptr = lpBuff;
    TJT808MediaDataUpload *dat = (TJT808MediaDataUpload *)pPackage;

	*ptr++ = dat->nMedia;//��ý������
	nBuffLen --;

	*ptr++ = dat->nChannel;//ͨ��ID
	nBuffLen --;

	*ptr++ = dat->nEventType;//�¼������
	nBuffLen --;
	
    *ptr++ = (((dat->tmStart.nYear / 10) % 10) << 4) | (dat->tmStart.nYear % 10);
    *ptr++ = BIN2BCD(dat->tmStart.nMonth);
    *ptr++ = BIN2BCD(dat->tmStart.nDay);
    *ptr++ = BIN2BCD(dat->tmStart.nHour);
    *ptr++ = BIN2BCD(dat->tmStart.nMinute);
    *ptr++ = BIN2BCD(dat->tmStart.nSecond);
    nBuffLen -= 6;

    *ptr++ = (((dat->tmEnd.nYear / 10) % 10) << 4) | (dat->tmEnd.nYear % 10);
    *ptr++ = BIN2BCD(dat->tmEnd.nMonth);
    *ptr++ = BIN2BCD(dat->tmEnd.nDay);
    *ptr++ = BIN2BCD(dat->tmEnd.nHour);
    *ptr++ = BIN2BCD(dat->tmEnd.nMinute);
    *ptr++ = BIN2BCD(dat->tmEnd.nSecond);
    nBuffLen -= 6;

	*ptr++ = dat->nDelFlag;
	nBuffLen--;

	return ptr - lpBuff;
}
//�����洢��ý�����ݼ����ϴ����� Ver201301
int JT808_DecodeOneMediaDataQueryReq(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet;
	uchar *ptr = lpBuff;
	TJT808OneMediaDataQueryReq *dat = (TJT808OneMediaDataQueryReq *)pPackage;

	nRet = PT_Decode_INT32(&(dat->nMediaID), ptr, nBuffLen);
	ptr += nRet;
	nBuffLen -= nBuffLen;

	dat->nDelFlag = *ptr++;
	nBuffLen--;

	return ptr - lpBuff;
}
//�����洢��ý�����ݼ����ϴ����� Ver201301
int JT808_EncodeOneMediaDataQueryReq(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet;
	uchar *ptr = lpBuff;
	TJT808OneMediaDataQueryReq *dat = (TJT808OneMediaDataQueryReq *)pPackage;

	nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nMediaID);
	ptr += nRet;
	nBuffLen -= nBuffLen;

	*ptr++ = dat->nDelFlag;
	nBuffLen--;

	return ptr - lpBuff;
}
int JT808_DecodeRecorderCtrl(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet = 0;
    uchar *ptr = lpBuff;
    TJT808RecorderCtrl *dat = (TJT808RecorderCtrl *)pPackage;

    dat->nCMD = *ptr++;
    nBuffLen--;

    nRet = PT_Decode_SHORT(&(dat->nTimeLimit), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    dat->nSaveFlag = *ptr++;
    nBuffLen--;

    dat->nSample = *ptr++;
    nBuffLen--;

    return ptr - lpBuff;
}

int JT808_EncodeRecorderCtrl(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet = 0;
    uchar *ptr = lpBuff;
    TJT808RecorderCtrl *dat = (TJT808RecorderCtrl *)pPackage;

    *ptr++ = dat->nCMD;
    nBuffLen--;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nTimeLimit);

    *ptr++ = dat->nSaveFlag;
    nBuffLen--;

    *ptr++ = dat->nSample;
    nBuffLen--;

    return ptr - lpBuff;
}
//����������͸��
int JT808_DecodeDataLoad(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    uchar *ptr = lpBuff;
    TJT808DataLoad *dat = (TJT808DataLoad *)pPackage;

    dat->nDataType = *ptr++;
    nBuffLen--;

    if (nBuffLen > 0)
    {
        if (nBuffLen <= sizeof(dat->data))
        {
            dat->nDataLen = nBuffLen;
            memcpy(dat->data, ptr, dat->nDataLen);

            ptr += dat->nDataLen;
            nBuffLen -= dat->nDataLen;            
        }
        else
        {
            return -1;
        }
    }

    return ptr - lpBuff;
}
//����������͸��
int JT808_EncodeDataLoad(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    uchar *ptr = lpBuff;
    TJT808DataLoad *dat = (TJT808DataLoad *)pPackage;

    *ptr++ = dat->nDataType;
    nBuffLen--;

    if (dat->nDataLen > 0)
    {
        if (dat->nDataLen <= nBuffLen)
        {
            memcpy(ptr, dat->data, dat->nDataLen);

            ptr += dat->nDataLen;
            nBuffLen -= dat->nDataLen;
        }
        else
        {
            return -1;
        }
    }

    return ptr - lpBuff;
}
//ѹ�������ϴ�
int JT808_DecodeDataGzipLoad(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	uchar *ptr = lpBuff;
	TJT808DataGzipLoad *dat = (TJT808DataGzipLoad *)pPackage;

	PT_Decode_INT32(&(dat->nDataLen), ptr, nBuffLen);
	nBuffLen--;

	if (nBuffLen > 0)
	{
		if (nBuffLen <= sizeof(dat->data))
		{
			dat->nDataLen = nBuffLen;
			memcpy(dat->data, ptr, dat->nDataLen);

			ptr += dat->nDataLen;
			nBuffLen -= dat->nDataLen;            
		}
		else
		{
			return -1;
		}
	}

	return ptr - lpBuff;
}
//ѹ�������ϴ�
int JT808_EncodeDataGzipLoad(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet;
	uchar *ptr = lpBuff;
	TJT808DataGzipLoad *dat = (TJT808DataGzipLoad *)pPackage;

	nRet = PT_Encode_INT32(ptr, nBuffLen, dat->nDataLen);
	ptr += nRet;
	nBuffLen -= nRet;

	if (dat->nDataLen > 0)
	{
		if (dat->nDataLen <= nBuffLen)
		{
			memcpy(ptr, dat->data, dat->nDataLen);

			ptr += dat->nDataLen;
			nBuffLen -= dat->nDataLen;
		}
		else
		{
			return -1;
		}
	}

	return ptr - lpBuff;
}
int JT808_DecodeRas(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808Ras *dat = (TJT808Ras *)pPackage;

    nRet = PT_Decode_INT32(&(dat->nRasE), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    if (nBuffLen > 0)
    {
        if (nBuffLen <= sizeof(dat->RasN))
        {
            dat->nRasNLen = nBuffLen;
            memcpy(dat->RasN, ptr, dat->nRasNLen);

            ptr += dat->nRasNLen;
            nBuffLen -= dat->nRasNLen;            
        }
        else
        {
            return -1;
        }
    }

    return ptr - lpBuff;
}

int JT808_EncodeRas(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808Ras *dat = (TJT808Ras *)pPackage;

    nRet = PT_Encode_INT32(ptr, nBuffLen, dat->nRasE);
    ptr += nRet;
    nBuffLen -= nRet;

    if (dat->nRasNLen > 0)
    {
        if (dat->nRasNLen <= nBuffLen)
        {
            memcpy(ptr, dat->RasN, dat->nRasNLen);

            ptr += dat->nRasNLen;
            nBuffLen -= dat->nRasNLen;            
        }
        else
        {
            return -1;
        }
    }

    return ptr - lpBuff;
}

int JT808_DecodeMultiPosition(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet, i;
    unsigned int nValue;
    uchar *ptr = lpBuff;
    TJT808MultiPosition *dat = (TJT808MultiPosition *)pPackage;

    dat->nYear = (*ptr++) + 2000;
    dat->nMonth = *ptr++;
    dat->nDay = *ptr++;
    dat->nHour = *ptr++;
    dat->nMinute = *ptr++;
    dat->nSecond = *ptr++;
    nBuffLen -= 6;

    dat->nCount = *ptr++;
    nBuffLen--;
    if (dat->nCount > JT808_DEF_MAX_NUM_USERPOSITION)
    {
        dat->nCount = JT808_DEF_MAX_NUM_USERPOSITION;
    }

    for (i = 0; i < dat->nCount; i++)
    {
        dat->position[i].nUserID = *ptr++;
        nBuffLen--;

        nRet = PT_Decode_INT32(&nValue, ptr, nBuffLen);
        dat->position[i].dblLatitude = nValue / JT808_MILLION;
        ptr += nRet;
        nBuffLen -= nRet;

        nRet = PT_Decode_INT32(&nValue, ptr, nBuffLen);
        dat->position[i].dblLongitude = nValue / JT808_MILLION;
        ptr += nRet;
        nBuffLen -= nRet;
      
        nValue = 0;
        nRet = PT_Decode_SHORT((unsigned short *)&nValue, ptr, nBuffLen);
        dat->position[i].dblSpeed = nValue / 10.0;
        ptr += nRet;
        nBuffLen -= nRet;

        nRet = PT_Decode_SHORT(&(dat->position[i].nDeriction), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;
    }

    return ptr - lpBuff;
}

int JT808_EncodeMultiPosition(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet, i;
    unsigned int nValue;
    uchar *ptr = lpBuff;
    TJT808MultiPosition *dat = (TJT808MultiPosition *)pPackage;

    *ptr++ = dat->nYear - 2000;
    *ptr++ = dat->nMonth;
    *ptr++ = dat->nDay;
    *ptr++ = dat->nHour;
    *ptr++ = dat->nMinute;
    *ptr++ = dat->nSecond;
    nBuffLen -= 6;

    *ptr++ = dat->nCount;
    nBuffLen--;

    for (i = 0; i < dat->nCount; i++)
    {
        *ptr++ = dat->position[i].nUserID;
        nBuffLen--;

        nValue = (unsigned int)(dat->position[i].dblLatitude * JT808_MILLION);
        nRet = PT_Encode_INT32(ptr, nBuffLen, nValue);
        ptr += nRet;
        nBuffLen -= nRet;

        nValue = (unsigned int)(dat->position[i].dblLongitude * JT808_MILLION);
        nRet = PT_Encode_INT32(ptr, nBuffLen, nValue);
        ptr += nRet;
        nBuffLen -= nRet;

        nValue = (unsigned short)(dat->position[i].dblSpeed * 10);
        nRet = PT_Encode_SHORT(ptr, nBuffLen, (unsigned short)nValue);
        ptr += nRet;
        nBuffLen -= nRet;

        nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->position[i].nDeriction);
        ptr += nRet;
        nBuffLen -= nRet;
    }

    return ptr - lpBuff;
}

int JT808_DecodeSensorReport(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet, i;
    uchar *ptr = lpBuff;
    TJT808SensorReport *dat = (TJT808SensorReport *)pPackage;

    //������������
    dat->nOilCount = *ptr++;
    nBuffLen--;

    for (i = 0; i < dat->nOilCount; i++)
    {
        nRet = PT_Decode_SHORT(&(dat->oils[i]), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;
				
		dat->oilsstate[i] = *ptr++;
		nBuffLen--;
    }
    //�¶ȴ�������
    dat->nTempCount = *ptr++;
    nBuffLen--;

    for (i = 0; i < dat->nTempCount; i++)
    {
        nRet = PT_Decode_SHORT(&(dat->temps[i]), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;
			
		dat->tempsstate[i] = *ptr++;
		nBuffLen--;
    }

    //̥ѹ��������
    dat->nTyreCount = *ptr++;
    nBuffLen--;

    for (i = 0; i < dat->nTyreCount; i++)
    {
        nRet = PT_Decode_SHORT(&(dat->tyres[i].nPressure), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;
        
        nRet = PT_Decode_SHORT(&(dat->tyres[i].nTemp), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;
			
		dat->tyres[i].nState = *ptr++;
		nBuffLen--;
    }

    //λ�ƴ�������
    dat->nPosiCount = *ptr++;
    nBuffLen--;

    for (i = 0; i < dat->nPosiCount; i++)
    {
        nRet = PT_Decode_SHORT(&(dat->positions[i]), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;
    }

    if (nBuffLen >= 1)//�ػ�״̬
    {
        dat->nCarryState = *ptr++;
        nBuffLen--;
    }
    else
    {
        dat->nCarryState = 0;
    }

    if (nBuffLen >= 8)
    {
        nRet = PT_Decode_INT32(&(dat->nWarnFlag), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;

        nRet = PT_Decode_INT32(&(dat->nState), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;
    }
    else
    {
        dat->nWarnFlag = 0;
        dat->nState = 0;
    }

    return ptr - lpBuff;
}

int JT808_EncodeSensorReport(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet, i;
    uchar *ptr = lpBuff;
    TJT808SensorReport *dat = (TJT808SensorReport *)pPackage;

    //������������
    *ptr++ = dat->nOilCount;
    nBuffLen--;

    for (i = 0; i < dat->nOilCount; i++)
    {
        nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->oils[i]);
        ptr += nRet;
        nBuffLen -= nRet;
			
		*ptr++ = dat->oilsstate[i];
        nBuffLen--;
    }
    //�¶ȴ�������
    *ptr++ = dat->nTempCount;
    nBuffLen--;

    for (i = 0; i < dat->nTempCount; i++)
    {
        nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->temps[i]);
        ptr += nRet;
        nBuffLen -= nRet;
			
		*ptr++ = dat->tempsstate[i];
        nBuffLen--;
				
    }

    //��̥��������
    *ptr++ = dat->nTyreCount;
    nBuffLen--;

    for (i = 0; i < dat->nTyreCount; i++)
    {
        nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->tyres[i].nPressure);
        ptr += nRet;
        nBuffLen -= nRet;
        
        nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->tyres[i].nTemp);
        ptr += nRet;
        nBuffLen -= nRet;
			
		*ptr++ = dat->tyres[i].nState;
        nBuffLen--;
    }

    //λ�ƴ�������
    *ptr++ = dat->nPosiCount;
    nBuffLen--;

    for (i = 0; i < dat->nPosiCount; i++)
    {
        nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->positions[i]);
        ptr += nRet;
        nBuffLen -= nRet;
    }

    //�ػ�״̬
    *ptr++ = dat->nCarryState;
    nBuffLen--;

    nRet = PT_Encode_INT32(ptr, nBuffLen, dat->nWarnFlag);
    ptr += nRet;
    nBuffLen -= nRet;

    nRet = PT_Encode_INT32(ptr, nBuffLen, dat->nState);
    ptr += nRet;
    nBuffLen -= nRet;

    return ptr - lpBuff;
}

int JT808_DecodeSensorQueryReq(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808SensorQueryReq *dat = (TJT808SensorQueryReq *)pPackage;

    nRet = nBuffLen;
    memcpy(dat->szDevice, ptr, nBuffLen);
    dat->szDevice[nRet] = '\0';
    ptr += nRet;
    nBuffLen -= nRet;

    return ptr - lpBuff;
}

int JT808_EncodeSensorQueryReq(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808SensorQueryReq *dat = (TJT808SensorQueryReq *)pPackage;

    nRet = strlen(dat->szDevice);
    memcpy(ptr, dat->szDevice, nRet);
    ptr += nRet;
    nBuffLen -= nRet;

    return ptr - lpBuff;
}

int JT808_DecodeSensorQueryRes(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808SensorQueryRes *dat = (TJT808SensorQueryRes *)pPackage;

    nRet = PT_Decode_SHORT(&(dat->nSeqNum), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    dat->nResult = *ptr++;
    nBuffLen--;

    if (dat->nResult == JT808_DEF_RESULT_SUCCESS)
    {
        nRet = PT_Decode_INT32(&(dat->nWarnFlag), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;

        nRet = PT_Decode_INT32(&(dat->nState), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;

        //�ػ�״̬
        dat->nCarryState = *ptr++;
        nBuffLen--;

        //�������������ݳ���
        nRet = *ptr++;
        nBuffLen--;

        if (nRet > 0)
        {
            memcpy(dat->szOil, ptr, nRet);
            dat->szOil[nRet] = '\0';

            ptr += nRet;
            nBuffLen -= nRet;
        }
        
        //�¶ȴ�������
        nRet = *ptr++;
        nBuffLen--;

        if (nRet > 0)
        {
            memcpy(dat->szTemp, ptr, nRet);
            dat->szTemp[nRet] = '\0';

            ptr += nRet;
            nBuffLen -= nRet;
        }


        //̥ѹ��������
        nRet = *ptr++;
        nBuffLen--;

        if (nRet > 0)
        {
            memcpy(dat->szTyre, ptr, nRet);
            dat->szTyre[nRet] = '\0';

            ptr += nRet;
            nBuffLen -= nRet;
        }

        //λ�ƴ�������
        nRet = *ptr++;
        nBuffLen--;

        if (nRet > 0)
        {
            memcpy(dat->szPosi, ptr, nRet);
            dat->szPosi[nRet] = '\0';

            ptr += nRet;
            nBuffLen -= nRet;
        }
    }

    return ptr - lpBuff;
}

int JT808_EncodeSensorQueryRes(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808SensorQueryRes *dat = (TJT808SensorQueryRes *)pPackage;

    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nSeqNum);
    ptr += nRet;
    nBuffLen -= nRet;

    *ptr++ = dat->nResult;
    nBuffLen--;

    if (dat->nResult == JT808_DEF_RESULT_SUCCESS)
    {
        nRet = PT_Encode_INT32(ptr, nBuffLen, dat->nWarnFlag);
        ptr += nRet;
        nBuffLen -= nRet;

        nRet = PT_Encode_INT32(ptr, nBuffLen, dat->nState);
        ptr += nRet;
        nBuffLen -= nRet;

        //�ػ�״̬
        *ptr++ = dat->nCarryState;
        nBuffLen--;

        //�������������ݳ���
        nRet = strlen(dat->szOil);
        *ptr++ = nRet;
        nBuffLen--;

        if (nRet > 0)
        {
            memcpy(ptr, dat->szOil, nRet);

            ptr += nRet;
            nBuffLen -= nRet;
        }

        //�¶ȴ�������
        nRet = strlen(dat->szTemp);
        *ptr++ = nRet;
        nBuffLen--;

        if (nRet > 0)
        {
            memcpy(ptr, dat->szTemp, nRet);

            ptr += nRet;
            nBuffLen -= nRet;
        }

        //̥ѹ��������
        nRet = strlen(dat->szTyre);
        *ptr++ = nRet;
        nBuffLen--;

        if (nRet > 0)
        {
            memcpy(ptr, dat->szTyre, nRet);

            ptr += nRet;
            nBuffLen -= nRet;
        }
        //λ�ƴ�������
        nRet = strlen(dat->szPosi);
        *ptr++ = nRet;
        nBuffLen--;

        if (nRet > 0)
        {
            memcpy(ptr, dat->szPosi, nRet);

            ptr += nRet;
            nBuffLen -= nRet;
        }
    }
    return ptr - lpBuff;
}

int JT808_DecodeDeviceUnlock(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet,nLen = 0;
    uchar *ptr = lpBuff;
    TJT808DeviceUnlockReq *dat = (TJT808DeviceUnlockReq *)pPackage;

    dat->bEncrypt = *ptr++;
    nBuffLen--;
    
    nRet = PT_Decode_INT32(&(dat->nEncryptKey), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;
    
    //ʡ��ID
    nRet = PT_Decode_SHORT(&(dat->nProvID), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    //������ID
    nRet = PT_Decode_SHORT(&(dat->nCity), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    //������ID
    memcpy(dat->szManufacturer, ptr, 5);
    dat->szManufacturer[5] = '\0';
    ptr += 5;
    nBuffLen -= 5;

    //�ն��ͺ�
    memcpy(dat->szModel, ptr, 20);
    dat->szModel[19] = '\0';
    ptr += 20;
    nBuffLen -= 20;

    //�ն�ID
    memcpy(dat->szDeviceID, ptr, 7);
    dat->szDeviceID[7] = '\0';
    ptr += 7;
    nBuffLen -= 7;

    //������ɫ
    *ptr++ = dat->nColor;
    nBuffLen -= 1;

    //����
    memcpy(dat->szLicenseplate, ptr, 9);
    dat->szLicenseplate[9] = '\0';
    ptr += 9;
    nBuffLen -= 9;
    
    if (nBuffLen > 0)
    {
        nRet = sizeof(dat->szVIN) - 1;
        if (nRet > nBuffLen)
        {
            nRet = nBuffLen;
        }
        
        memcpy(dat->szVIN, ptr, nRet);
        dat->szVIN[nRet] = '\0';
        
        ptr += nBuffLen;
        nBuffLen -= nBuffLen;
    }
    
    return (ptr - lpBuff);
}

int JT808_EncodeDeviceUnlock(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet,nLen = 0;
    uchar *ptr = lpBuff;
    TJT808DeviceUnlockReq *dat = (TJT808DeviceUnlockReq *)pPackage;

    *ptr++ = dat->bEncrypt;
    nBuffLen--;
    
    //������Կ
    nRet = PT_Encode_INT32(ptr, nBuffLen, dat->nEncryptKey);
    ptr += nRet;
    nBuffLen -= nRet;
    //�����ֻ�����
    memcpy(ptr, dat->szUserMobile, 12);
    ptr += 12;
    nBuffLen -= 12;
    //ʡ��ID
    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nProvID);
    ptr += nRet;
    nBuffLen -= nRet;

    //������ID
    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nCity);
    ptr += nRet;
    nBuffLen -= nRet;

    //������ID
    memcpy(ptr, dat->szManufacturer, 5);
    ptr += 5;
    nBuffLen -= 5;

    //�ն��ͺ�
	nLen = sizeof(dat->szModel);//  -- by xlsong
    nRet = strlen((char *)dat->szModel);
    if (nRet < nLen)
    {
        memcpy(ptr, dat->szModel, nRet);
        memset(ptr + nRet, 0, nLen - nRet);
    }
    else
    {
        memcpy(ptr, dat->szModel, nLen);
    }
    ptr += nLen;
    nBuffLen -= nLen;

    //�ն�ID
    nRet = strlen(dat->szDeviceID);//����������
    if (nRet >= 7)
    {
        memcpy(ptr, dat->szDeviceID, 7); 
    }
    else
    {
        memcpy(ptr, dat->szDeviceID, nRet); 
        memset(ptr + nRet, 0, 7 - nRet);
    }
    ptr += 7;
    nBuffLen -= 7;
    
    //������ɫ
    *ptr++ = dat->nColor;
    nBuffLen -= 1;

    //����
    nRet = strlen(dat->szLicenseplate);//����������
    if (nRet >= 9)
    {
        memcpy(ptr, dat->szLicenseplate, 9); 
    }
    else
    {
        memcpy(ptr, dat->szLicenseplate, nRet); 
        memset(ptr + nRet, 0, 9 - nRet);
    }
    
    //VIN
    nRet = strlen(dat->szVIN) + 1;//����������
    memcpy(ptr,dat->szVIN, 17); 
    ptr += 17;
    nBuffLen -= 17;
    *ptr++ = 0x00;

    return (ptr - lpBuff);
}
///////////////////////////////////////
#endif//end PT_ENABLE_JT808
