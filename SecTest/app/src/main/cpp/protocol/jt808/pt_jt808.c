/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     pt_jt808.c
* Abstract： JT/T808 protocol 
* 
* Version： 1.0
* Author：  yfwang
* Date：    2012/07/26
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

const unsigned short JT808_CODE[]={ 0x0001,//终端通用应答
                                    0x8001,//平台通用应答
                                    0x0002,//终端心跳
                                    0x0100,//终端注册
                                    0x8100,//终端注册应答
                                    0x0003,//终端注消  xx  0x0003
                                    0x0102,//终端鉴权
                                    0x8103,//设置终端参数
                                    0x8104,//查询终端参数
                                    0x0104,//查询终端参数应答
                                    0x8105,//终端控制
                                    0x0200,//位置信息汇报
                                    0x8201,//位置信息查询
                                    0x0201,//位置信息查询应答
                                    0x8202,//临时位置跟踪
                                    0x8300,//文本下发
                                    0x8301,//事件设置
                                    0x0301,//事件报告
                                    0x8302,//提问下发  
                                    0x0302,//提问应答
                                    0x8303,//消息点播菜单设置
                                    0x0303,//信息点播/取消
                                    0x8304,//信息服务
                                    0x8400,//电话回拨
                                    0x8401,//设置电话本
                                    0x8500,//车辆控制
                                    0x0500,//车辆控制应答
                                    0x8600,//设置圆形区域
                                    0x8601,//删除圆形区域
                                    0x8602,//设置矩形区域
                                    0x8603,//删除矩形区域
                                    0x8604,//设置多边形区域
                                    0x8605,//删除多边形区域
                                    0x8606,//设置路线
                                    0x8607,//删除路线
                                    0x8700,//行驶记录仪数据采集命令
                                    0x0700,//行驶记录仪数据上传
                                    0x8701,//行驶记录仪参数下传命令
                                    0x0701,//电子运单上报
                                    0x0702,//驾驶员身份信息采集上报
                                    0x0800,//多媒体事件信息上传
                                    0x0801,//多媒体数据上传
                                    0x8800,//多媒体数据上传应答
                                    0x8801,//摄像头立即拍摄指令

									0x8802,//存储多媒体数据检索
									0x0802,//存储多媒体数据检索应答

                                    0x8803,//存储多媒体数据上传命令
                                    0x8804,//录音开始命令
                                    0x8900,//数据下行透传
                                    0x0900,//数据上行透传
                                    0x0901,//数据压缩上报
                                    0x8A00,//平台RAS公钥
                                    0x0A00,//终端RAS公钥
                                    0xFFFF,//自定义多用户位置报告
                                    0xFFFE,//传感器数据上传
                                    0xFFFD,//传感器数据查询请求
                                    0xFFFC,//传感器数据查询反馈
									0x8003,//补传分包请求
									0x8106,//查询指定终端参数
									0x8107,//查询终端属性
									0x0107,//查询终端属性应答
									0x8108,//下发终端升级包
									0x0108,//终端升级结果通知
									0x8203,//人工确认报警消息
									0x8702,//上报驾驶员身份信息请求
									0x0704,//定位数据批量上传
									0x0705,//CAN总线数据上传
									0x0805,//摄像头立即拍摄命令应答
									0x8805,//单条存储多媒体数据检索上传命令
                                    0x0110,//设备解锁
                                   };

extern int PT_Decode_INT32(int *pVal, uchar *lpBuff, int nLen);
extern int PT_Encode_INT32(uchar *lpBuff, int nLen, int nVal);

extern int PT_Decode_SHORT(short *pVal, uchar *lpBuff, int nLen);
extern int PT_Encode_SHORT(uchar *lpBuff, int nLen, short nVal);
//--------------------------------------------------------------------------
//协议栈编解码函数定义
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
//协议栈基本函数
//--------------------------------------------------------------------------
/*
*函数名称：JT808_MakeCRC
*函数说明：计算校验值
*参数说明：buf    接收数据缓存
*          len    接收数据长度
*返回值：  校验值
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
*函数名称：JT808_MakeCRC
*函数说明：计算校验值
*参数说明：buf    接收数据缓存
*          len    接收数据长度
*返回值：  校验值
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
            {//数据发生变化
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
    nCount = sizeof(JT808_CODE) / sizeof(JT808_CODE[0]);//计算个数
    for(i = 0; i < nCount; i++)
    {
        if(nType == JT808_CODE[i])	//协议头判断
        {
            return ( i + JT808_ZERO);
        }
    }

    return -1;
}
/*
*函数名称：JT808_DecodeHead
*函数说明：解析消息类型
*参数说明：buf    接收数据缓存
*          len    接收数据长度
*返回值：  -1 表示无效的消息类型，其他表示解析的类型
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
*函数名称：JT808_EncodeHead
*函数说明：编码协议头
*参数说明：lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          bDevice   是否设备
*          nType     消息类型
*返回值：  -1 表示失败，其他表示编码数据长度
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
*函数名称：JT808_DecodeHeadEx
*函数说明：编码协议头
*参数说明：head      数据结构
*          buf       数据缓存
*          len       数据长度
*返回值：  -1 表示失败，其他表示编码数据长度
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
*函数名称：JT808_EncodeHeadEx
*函数说明：编码协议头
*参数说明：lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          head      数据结构
*返回值：  -1 表示失败，其他表示编码数据长度
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
*函数名称：JT808_GetEncoder
*函数说明：根据消息类型获取相应的编码函数
*参数说明：nType 消息类型
*返回值：  NULL 表示无编码函数，其他表示编码函数指针
*/
FUNC_ENCODER JT808_GetEncoder(int nType)
{
    switch (nType)
    {
    case JT808_TERMINALRES://终端通用应答
    case JT808_PLATFORMRES://平台通用应答
        return JT808_EncodeCommonRes;
    case JT808_KEEPALIVE://终端心跳
        return JT808_EncodeCommonReq;
	case JT808_SEND_AGAINREQ://补传分包请求   Ver201301
		return JT808_EncodeSendAgainReq;
    case JT808_REGISTERREQ://终端注册
        return JT808_EncodeRegisterReq;
    case JT808_REGISTERRES://终端注册应答
        return JT808_EncodeRegisterRes;
    case JT808_DEREGISTER://终端注消
        return JT808_EncodeCommonReq;
    case JT808_LOGIN://终端鉴权
        return JT808_EncodeLogin;
    case JT808_PARAMETET_SET://设置终端参数
        return JT808_EncodeParameterSet;
    case JT808_PARAMETER_QUERYREQ://查询终端参数
        return JT808_EncodeCommonReq;
    case JT808_PARAMETER_QUERYRES://查询终端参数应答
        return JT808_EncodeParameterQueryRes;
	case JT808_SPECIFY_PARAM_QUERYREQ://查询指定终端参数 Ver201301
		return JT808_EncodeSpecifyParamQueryReq;
	case JT808_PROPERTY_QUERYREQ://查询终端属性  Ver201301
		return JT808_EncodeCommonReq;
	case JT808_PROPERTY_QUERYRES://查询终端属性应答  Ver201301
		return JT808_EncodePropertyQueryRes;
	case JT808_UPDATE_PACKAGEREQ://下发终端升级包  Ver201301
		return JT808_EncodeUpdatePackageReq;
	case JT808_UPDATE_PACKAGERES://终端升级结果通知  Ver201301
		return JT808_EncodeUpdatePackageRes;
    case JT808_TERMINALCTRL://终端控制
        return JT808_EncodeTerminalCtrl;
    case JT808_POSITION_REPORT://位置信息汇报
        return JT808_EncodePositionReport;
    case JT808_POSITION_QUERYREQ://位置信息查询
        return JT808_EncodeCommonReq;
    case JT808_POSITION_QUERYRES://位置信息查询应答
        return JT808_EncodePositionQueryRes;
    case JT808_POSITION_TRACK://临时位置跟踪
        return JT808_EncodePositionTrack;
	case JT808_WARM_MANAGE://人工确认报警消息   Ver201301
		return JT808_EncodeWarmManage;
    case JT808_TEXT_NOTICE://文本下发
        return JT808_EncodeTextNotice;
    case JT808_EVENT_SET://事件设置
        return JT808_EncodeEventSet;
    case JT808_EVENT_REPORT://事件报告
        return JT808_EncodeEventReport;
    case JT808_QUESTION_REQ://提问下发  
        return JT808_EncodeQuestionReq;
    case JT808_QUESTION_RES://提问应答
        return JT808_EncodeQuestionRes;
    case JT808_SERVICE_SET://消息点播菜单设置
        return JT808_EncodeServiceSet;
    case JT808_SERVICE_OPT://信息点播/取消
        return JT808_EncodeServiceOpt;
    case JT808_SERVICE_INFO://信息服务
        return JT808_EncodeServiceInfo;
    case JT808_PHONECALLBACK://电话回拨
        return JT808_EncodePhoneCallback;
    case JT808_PHONEBOOK://设置电话本
        return JT808_EncodePhoneBook;
    case JT808_CAR_CTRLREQ://车辆控制
        return JT808_EncodeCarCtrlReq;
    case JT808_CAR_CTRLRES://车辆控制应答
        return JT808_EncodeCarCtrlRes;
    case JT808_REGION_CIRCLE_SET://设置圆形区域
        return JT808_EncodeRegionCircleSet;
    case JT808_REGION_CIRCLE_REMOVE://删除圆形区域
        return JT808_EncodeRegionRemove;
    case JT808_REGION_RECT_SET://设置矩形区域
        return JT808_EncodeRegionRectSet;
    case JT808_REGION_RECT_REMOVE://删除矩形区域
        return JT808_EncodeRegionRemove;
    case JT808_REGION_POLYGON_SET://设置多边形区域
        return JT808_EncodeRegionPolygonSet;
    case JT808_REGION_POLYGON_REMOVE://删除多边形区域
        return JT808_EncodeRegionRemove;
    case JT808_ROUTE_SET://设置路线
        return JT808_EncodeRouteSet;
    case JT808_ROUTE_REMOVE://删除路线
        return JT808_EncodeRegionRemove;
    case JT808_DRIVINGRECORDER_REQ://行驶记录仪数据采集命令
        return JT808_EncodeDrivingRecorderReq;
    case JT808_DRIVINGRECORDER_RES://行驶记录仪数据上传
        return JT808_EncodeDrivingRecorderRes;
    case JT808_DRIVINGRECORDER_PARAM:  //行驶记录仪参数下传命令
        return JT808_EncodeDrivingRecorderParam;
    case JT808_ORDER_REPORT:              //电子运单上报
        return JT808_EncodeOrderReport;
	case JT808_DRIVERINFO_REPORTREQ://上报驾驶员身份信息请求  Ver201301
		return JT808_EncodeCommonReq;
    case JT808_DRIVERINFO_REPORT:         //驾驶员身份信息采集上报
        return JT808_EncodeDriverInfoReport;
	case JT808_POSDATA_UPLOAD://定位数据批量上传  Ver201301
		return JT808_EncodePosDataUpload;
	case JT808_CAN_DATA_UPLOAD://CAN总线数据上传  Ver201301
		return JT808_EncodeCanDataUpload;
    case JT808_MULTIMEDIA_EVENT_REPORT://多媒体事件信息上传
        return JT808_EncodeMediaEventReport;
    case JT808_MULTIMEDIA_DATA_UPLOADREQ://多媒体数据上传
        return JT808_EncodeMediaDataUploadReq;
    case JT808_MULTIMEDIA_DATA_UPLOADRES://多媒体数据上传应答
        return JT808_EncodeMediaDataUploadRes;
    case JT808_CAMERA_CTRL://摄像头拍摄指令控制
        return JT808_EncodeCameralCtrl;
	case JT808_CAMERA_CTRL_RES: //摄像头立即拍摄命令应答  Ver201301
		return JT808_EncodeCameralCtrlRes;
    case JT808_MULTIMEDIA_DATA_QUERYREQ://存储多媒体数据检索
        return JT808_EncodeMediaIndexReq;
    case JT808_MULTIMEDIA_DATA_QUERYRES://存储多媒体数据检索应答
        return JT808_EncodeMediaIndexRes;
    case JT808_MULTIMEDIA_DATA_UPLOADCMD://存储多媒体数据上传命令
        return JT808_EncodeMediaDataUpload;
	case JT808_ONE_MULTIMEDIA_DATA_QUERYREQ://单条存储多媒体数据检索上传命令 Ver201301
		return JT808_EncodeOneMediaDataQueryReq;
    case JT808_RECORDER_CTRL://录音命令开始/停止
        return JT808_EncodeRecorderCtrl;
    case JT808_DATA_DOWNLOAD://数据下行透传
    case JT808_DATA_UPLOAD://数据上行透传
		return JT808_EncodeDataLoad;
    case JT808_DATA_GZIP_UPLOAD://数据压缩上报  //modify by xlsong
        return JT808_EncodeDataGzipLoad;
    case JT808_PLATFORM_RAS://平台RAS公钥
    case JT808_TERMINAL_RAS://终端RAS公钥
        return JT808_EncodeRas;
    case JT808_MULTIPOSITION://多用户位置上报,自定义协议
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
*函数名称：JT808_GetDecoder
*函数说明：根据消息类型获取相应的解码函数
*参数说明：nType 消息类型
*返回值：  NULL 表示无解码函数，其他表示解码函数指针
*/
FUNC_DECODER JT808_GetDecoder(int nType)
{
    switch (nType)
    {
    case JT808_TERMINALRES://终端通用应答
    case JT808_PLATFORMRES://平台通用应答
        return JT808_DecodeCommonRes;
    case JT808_KEEPALIVE://终端心跳
        return JT808_DecodeCommonReq;
	case JT808_SEND_AGAINREQ://补传分包请求
		return JT808_DecodeSendAgainReq;
    case JT808_REGISTERREQ://终端注册
        return JT808_DecodeRegisterReq;
    case JT808_REGISTERRES://终端注册应答
        return JT808_DecodeRegisterRes;
    case JT808_DEREGISTER://终端注消
        return JT808_DecodeCommonReq;
    case JT808_LOGIN://终端鉴权
        return JT808_DecodeLogin;
    case JT808_PARAMETET_SET://设置终端参数
        return JT808_DecodeParameterSet;
    case JT808_PARAMETER_QUERYREQ://查询终端参数
        return JT808_DecodeCommonReq;
    case JT808_PARAMETER_QUERYRES://查询终端参数应答
        return JT808_DecodeParameterQueryRes;
	case JT808_SPECIFY_PARAM_QUERYREQ://查询指定终端参数  Ver301301
		return JT808_DecodeSpecifyParamQueryReq;
	case JT808_PROPERTY_QUERYREQ://查询终端属性   Ver301301
		return JT808_DecodeCommonReq;
	case JT808_PROPERTY_QUERYRES://查询终端属性应答 Ver301301
		return JT808_DecodePropertyQueryRes;
	case JT808_UPDATE_PACKAGEREQ://下发终端升级包  Ver301301
		return JT808_DecodeUpdatePackageReq;
	case JT808_UPDATE_PACKAGERES://终端升级结果通知  Ver301301
		return JT808_DecodeUpdatePackageRes;
    case JT808_TERMINALCTRL://终端控制
        return JT808_DecodeTerminalCtrl;
    case JT808_POSITION_REPORT://位置信息汇报
        return JT808_DecodePositionReport;;
    case JT808_POSITION_QUERYREQ://位置信息查询
        return JT808_DecodeCommonReq;
    case JT808_POSITION_QUERYRES://位置信息查询应答
        return JT808_DecodePositionQueryRes;
    case JT808_POSITION_TRACK://临时位置跟踪
        return JT808_DecodePositionTrack;
	case JT808_WARM_MANAGE://人工确认报警消息   Ver201301
		return JT808_DecodeWarmManage; 
    case JT808_TEXT_NOTICE://文本下发
        return JT808_DecodeTextNotice;
    case JT808_EVENT_SET://事件设置
        return JT808_DecodeEventSet;
    case JT808_EVENT_REPORT://事件报告
        return JT808_DecodeEventReport;
    case JT808_QUESTION_REQ://提问下发  
        return JT808_DecodeQuestionReq;
    case JT808_QUESTION_RES: //提问应答
        return JT808_DecodeQuestionRes;
    case JT808_SERVICE_SET://消息点播菜单设置
        return JT808_DecodeServiceSet;
    case JT808_SERVICE_OPT://信息点播/取消
        return JT808_DecodeServiceOpt;
    case JT808_SERVICE_INFO://信息服务
        return JT808_DecodeServiceInfo;
    case JT808_PHONECALLBACK://电话回拨
        return JT808_DecodePhoneCallback;
    case JT808_PHONEBOOK://设置电话本
        return JT808_DecodePhoneBook;
    case JT808_CAR_CTRLREQ://车辆控制
        return JT808_DecodeCarCtrlReq;
    case JT808_CAR_CTRLRES://车辆控制应答
        return JT808_DecodeCarCtrlRes;
    case JT808_REGION_CIRCLE_SET://设置圆形区域
        return JT808_DecodeRegionCircleSet;
    case JT808_REGION_CIRCLE_REMOVE://删除圆形区域
        return JT808_DecodeRegionRemove;
    case JT808_REGION_RECT_SET://设置矩形区域
        return JT808_DecodeRegionRectSet;
    case JT808_REGION_RECT_REMOVE://删除矩形区域
        return JT808_DecodeRegionRemove;
    case JT808_REGION_POLYGON_SET://设置多边形区域
        return JT808_DecodeRegionPolygonSet;
    case JT808_REGION_POLYGON_REMOVE://删除多边形区域
        return JT808_DecodeRegionRemove;
    case JT808_ROUTE_SET://设置路线
        return JT808_DecodeRouteSet;
    case JT808_ROUTE_REMOVE://删除路线
        return JT808_DecodeRegionRemove;
    case JT808_DRIVINGRECORDER_REQ://行驶记录仪数据采集命令
        return JT808_DecodeDrivingRecorderReq;
    case JT808_DRIVINGRECORDER_RES: //行驶记录仪数据上传
        return JT808_DecodeDrivingRecorderRes;
    case JT808_DRIVINGRECORDER_PARAM:  //行驶记录仪参数下传命令
        return JT808_DecodeDrivingRecorderParam;
    case JT808_ORDER_REPORT:           //电子运单上报
        return JT808_DecodeOrderReport;
    case JT808_DRIVERINFO_REPORT:         //驾驶员身份信息采集上报
        return JT808_DecodeDriverInfoReport;
    case JT808_MULTIMEDIA_EVENT_REPORT://多媒体事件信息上传
        return JT808_DecodeMediaEventReport;
    case JT808_MULTIMEDIA_DATA_UPLOADREQ://多媒体数据上传
        return JT808_DecodeMediaDataUploadReq;
    case JT808_MULTIMEDIA_DATA_UPLOADRES://多媒体数据上传应答
        return JT808_DecodeMediaDataUploadRes;
    case JT808_CAMERA_CTRL://摄像头拍摄指令控制
        return JT808_DecodeCameralCtrl;
    case JT808_MULTIMEDIA_DATA_QUERYREQ://存储多媒体数据检索
        return JT808_DecodeMediaIndexReq;
    case JT808_MULTIMEDIA_DATA_QUERYRES://存储多媒体数据检索应答
        return JT808_DecodeMediaIndexRes;
    case JT808_MULTIMEDIA_DATA_UPLOADCMD://存储多媒体数据上传命令
        return JT808_DecodeMediaDataUpload;
    case JT808_RECORDER_CTRL://录音命令开始/停止
        return JT808_DecodeRecorderCtrl;
    case JT808_DATA_DOWNLOAD://数据下行透传
    case JT808_DATA_UPLOAD://数据上行透传
		return JT808_DecodeDataLoad;
    case JT808_DATA_GZIP_UPLOAD://数据压缩上报  modify by xlsong
        return JT808_DecodeDataGzipLoad;
    case JT808_PLATFORM_RAS://平台RAS公钥
    case JT808_TERMINAL_RAS://终端RAS公钥
        return JT808_DecodeRas;
    case JT808_MULTIPOSITION://多用户位置上报,自定义协议
        return JT808_DecodeMultiPosition;
	case JT808_DRIVERINFO_REPORTREQ://上报驾驶员身份信息请求  Ver201301
		return JT808_DecodeCommonReq;
	case JT808_POSDATA_UPLOAD://定位数据批量上传  Ver201301
		return JT808_DecodePosDataUpload;
	case JT808_CAN_DATA_UPLOAD://CAN总线数据上传  Ver201301
		return JT808_DecodeCanDataUpload;
	case JT808_CAMERA_CTRL_RES: //摄像头立即拍摄命令应答 Ver201301
		return JT808_DecodeCameralCtrlRes;
	case JT808_ONE_MULTIMEDIA_DATA_QUERYREQ://单条存储多媒体数据检索上传命令 Ver201301
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
*函数名称：JT808_RecvData
*函数说明：解码接收的数据至协议数据结构
*参数说明：pParam 协议接收栈指针
*          data     接收的数据
*返回值：  -1 表示没有收到整帧数据，其他表示所接收帧数据的消息类型
*/
int  JT808_RecvData(TStackParam *pParam, uchar data)
{
    int nRet = RESULT_ERROR;
    if (pParam->nRecvLen == 0)
    {
        if (data == 0x7E)//收到帧头
        {
            pParam->RecvBuff[0] = data;
            
            pParam->nRecvLen = 1;//开始数据接收
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
            else if (nRet > 0) //收到完整帧
            {
                pParam->nRecvLen = 0;
                return nRet;
            }
        }
    }

    return RESULT_ERROR;
}
/*
*函数名称：JT808_RecvDataEx
*函数说明：解码接收的数据至协议数据结构
*参数说明：pParam   栈
*          data     接收的数据
*返回值：  -1 表示没有收到整帧数据，其他表示所接收帧数据的消息类型
*/
int  JT808_RecvDataEx(TStackParam *pParam, uchar data)
{
    if (data == 0x7E) //帧结束符
    {
        pParam->nFrameLen = JT808_Translate(pParam->RecvBuff + 1, pParam->nRecvLen - 2, 0, pParam->nRecvBuffLen);
        pParam->nFrameLen += 2;
        pParam->RecvBuff[pParam->nFrameLen - 1] = 0x7E;
        
        pParam->nCRCValue = JT808_MakeCRC(pParam->RecvBuff + 1, pParam->nFrameLen - 3);

        if (pParam->nCRCValue == pParam->RecvBuff[pParam->nFrameLen - 2])//校验正确
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
*函数名称：JT808_DecodePackage
*函数说明：解码接收整帧数据至协议数据结构
*参数说明：pPackage  数据结构指针
*          lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          nMsgType  协议类型
*返回值：  0 表示解码成功，其他表示失败
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
*函数名称：GT06_EncodePackage
*函数说明：编码数据
*参数说明：lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          pMsgData  协议数据结构指针
*          bDevice   是否设备栈，设备栈二代协议头编码"BD"用1表示，PC栈编码"CC"用0表示
*返回值：  -1 表示失败，其他表示数据长度
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

    //length include frame tail char（0x7E）and crc(one bytes)
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
*函数名称：NW_GetPackage
*函数说明：根据消息类型获取相应的数据结构包
*参数说明：nType 消息类型
*返回值：  NULL 表示无此类型数据结构包，其他表示数据结构包指针，结构数据包使用完后应当释放空间
*/
void* JT808_GetPackage(int nType)
{
    switch (nType)
    { 
    case JT808_TERMINALRES://终端通用应答
    case JT808_PLATFORMRES://平台通用应答
    case JT808_KEEPALIVE://终端心跳
    case JT808_REGISTERREQ://终端注册
    case JT808_REGISTERRES://终端注册应答
    case JT808_DEREGISTER://终端注消
    case JT808_LOGIN://终端鉴权
    case JT808_PARAMETET_SET://设置终端参数
    case JT808_PARAMETER_QUERYREQ://查询终端参数
    case JT808_PARAMETER_QUERYRES://查询终端参数应答
    case JT808_TERMINALCTRL://终端控制
    case JT808_POSITION_REPORT://位置信息汇报
    case JT808_POSITION_QUERYREQ://位置信息查询
    case JT808_POSITION_QUERYRES://位置信息查询应答
    case JT808_POSITION_TRACK://临时位置跟踪
    case JT808_TEXT_NOTICE://文本下发
    case JT808_EVENT_SET://事件设置
    case JT808_EVENT_REPORT://事件报告
    case JT808_QUESTION_REQ://提问下发  
    case JT808_QUESTION_RES://提问应答
    case JT808_SERVICE_SET://消息点播菜单设置
    case JT808_SERVICE_OPT://信息点播/取消
    case JT808_SERVICE_INFO://信息服务
    case JT808_PHONECALLBACK://电话回拨
    case JT808_PHONEBOOK://设置电话本
    case JT808_CAR_CTRLREQ://车辆控制
    case JT808_CAR_CTRLRES://车辆控制应答
    case JT808_REGION_CIRCLE_SET://设置圆形区域
    case JT808_REGION_CIRCLE_REMOVE://删除圆形区域
    case JT808_REGION_RECT_SET://设置矩形区域
    case JT808_REGION_RECT_REMOVE://删除矩形区域
    case JT808_REGION_POLYGON_SET://设置多边形区域
    case JT808_REGION_POLYGON_REMOVE://删除多边形区域
    case JT808_ROUTE_SET://设置路线
    case JT808_ROUTE_REMOVE://删除路线
    case JT808_DRIVINGRECORDER_REQ://行驶记录仪数据采集命令
    case JT808_DRIVINGRECORDER_RES://行驶记录仪数据上传
    case JT808_DRIVINGRECORDER_PARAM:  //行驶记录仪参数下传命令
    case JT808_ORDER_REPORT:              //电子运单上报
    case JT808_DRIVERINFO_REPORT:         //驾驶员身份信息采集上报
    case JT808_MULTIMEDIA_EVENT_REPORT://多媒体事件信息上传
    case JT808_MULTIMEDIA_DATA_UPLOADREQ://多媒体数据上传
    case JT808_MULTIMEDIA_DATA_UPLOADRES://多媒体数据上传应答
    case JT808_CAMERA_CTRL://摄像头拍摄指令控制
    case JT808_MULTIMEDIA_DATA_QUERYREQ://存储多媒体数据检索
    case JT808_MULTIMEDIA_DATA_QUERYRES://存储多媒体数据检索应答
    case JT808_MULTIMEDIA_DATA_UPLOADCMD://存储多媒体数据上传命令
    case JT808_RECORDER_CTRL://录音命令开始/停止
    case JT808_DATA_DOWNLOAD://数据下行透传
    case JT808_DATA_UPLOAD://数据上行透传
    case JT808_DATA_GZIP_UPLOAD://数据压缩上报
    case JT808_PLATFORM_RAS://平台RAS公钥
    case JT808_TERMINAL_RAS://终端RAS公钥
    default:
        return NULL;
    }
}
#endif //WIN32
//--------------------------------------------------------------------------
//协议栈编解码函数实现
//--------------------------------------------------------------------------
int JT808_DecodeCommonReq(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    return 0;
}

int JT808_EncodeCommonReq(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    return 0;
}
//通用应答解码
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

int JT808_EncodeCommonRes(uchar *lpBuff, int nBuffLen, void *pPackage)//通用应答编码
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
//补传分包请求   Ver201301
int JT808_DecodeSendAgainReq(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet, i = 0;
	uchar *ptr = lpBuff;
	TJT808SendAgainReq *dat = (TJT808SendAgainReq *)pPackage;

	// 应答流水号
	nRet = PT_Decode_SHORT(&(dat->nSeqNum), ptr, nBuffLen);
	ptr += nRet;
	nBuffLen -= nRet;

	//重传包总数
	dat->nCount = *ptr++;
	nBuffLen--;

    if (dat->nCount > 128)
    {
        dat->nCount = 128;
    }
    
	//重传 ID 列表
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

	// 应答流水号
	nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nSeqNum);
	ptr += nRet;
	nBuffLen -= nRet;

	//重传包总数
	*ptr++ = dat->nCount;
	nBuffLen--;

	//重传 ID 列表
	for (i = 0; i<(dat->nCount); i++)
	{
		nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nIDlist[i]);
		ptr += nRet;
		nBuffLen -= nRet;
	}
	return (ptr - lpBuff);
}
//终端注册
int JT808_DecodeRegisterReq(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet,nLen = 0;
    uchar *ptr = lpBuff;
    TJT808REGISTERREQ *dat = (TJT808REGISTERREQ *)pPackage;

    //省域ID
    nRet = PT_Decode_SHORT(&(dat->nProvID), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    //市县域ID
    nRet = PT_Decode_SHORT(&(dat->nCityID), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    //制造商ID
    dat->szManID[0] = *ptr++;
    dat->szManID[1] = *ptr++;
    dat->szManID[2] = *ptr++;
    dat->szManID[3] = *ptr++;
    dat->szManID[4] = *ptr++;
    nBuffLen -= 5;

    //终端型号
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

    //终端ID
    memcpy(dat->szTerminalID, ptr, 7);
    dat->szTerminalID[7] = '\0';
    ptr += 7;
    nBuffLen -= 7;

    //车牌颜色
    dat->nColor = *ptr++;
    nBuffLen -= 1;

    //车牌
    if (nBuffLen > sizeof(dat->szLicensePlate) - 1)
    {
        nBuffLen = sizeof(dat->szLicensePlate) - 1;
    }

    memcpy(dat->szLicensePlate, ptr, nBuffLen);
    dat->szLicensePlate[nBuffLen] = 0;
    ptr += nBuffLen;

    return (ptr - lpBuff);
}
//终端注册
int JT808_EncodeRegisterReq(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet,nLen = 0;
    uchar *ptr = lpBuff;
    TJT808REGISTERREQ *dat = (TJT808REGISTERREQ *)pPackage;

    //省域ID
    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nProvID);
    ptr += nRet;
    nBuffLen -= nRet;

    //市县域ID
    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nCityID);
    ptr += nRet;
    nBuffLen -= nRet;

    //制造商ID
    memcpy(ptr, dat->szManID, 5);
    ptr += 5;
    nBuffLen -= 5;

    //终端型号
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

    //终端ID
    memcpy(ptr, dat->szTerminalID, 7);
    ptr += 7;
    nBuffLen -= 7;

    //车牌颜色
    *ptr++ = dat->nColor;
    nBuffLen -= 1;

    //车牌
    nRet = strlen(dat->szLicensePlate);//包含结束符
    memcpy(ptr,dat->szLicensePlate,nRet); 
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}
//终端注册应答
int JT808_DecodeRegisterRes(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808REGISTERRES *dat = (TJT808REGISTERRES *)pPackage;

    //应答流水号
    nRet = PT_Decode_SHORT(&(dat->nSeqNum), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    //结果
    dat->nResult = *ptr++;
    nBuffLen -= 1;

    //鉴权码
    if(dat->nResult == 0)//成功
    {
        if (dat->head.nProtocol == JT808_DEF_PROTOCOL_BD)//基于北斗的平台为设备分配唯一入网号
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
//终端注册应答
int JT808_EncodeRegisterRes(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808REGISTERRES *dat = (TJT808REGISTERRES *)pPackage;

    //应答流水号
    nRet = PT_Encode_SHORT(ptr, nBuffLen,dat->nSeqNum);
    ptr += nRet;
    nBuffLen -= nRet;

    //结果
    *ptr++ = dat->nResult;
    nBuffLen -= 1;

    //鉴权码
    if(dat->nResult == 0)//成功
    {
        if (dat->head.nProtocol == JT808_DEF_PROTOCOL_BD)//基于北斗的平台为设备分配唯一入网号
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

        nRet = strlen(dat->szAuthcode) + 1;//包含结束符
        memcpy(ptr, dat->szAuthcode, nRet); 
        ptr += nRet;
        nBuffLen -= nRet;
    }	

    return (ptr - lpBuff);
}
//终端鉴权
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
//终端鉴权
int JT808_EncodeLogin(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808LOGIN *dat = (TJT808LOGIN *)pPackage;

    nRet = strlen(dat->szAuthcode);//包含结束符
    memcpy(ptr, dat->szAuthcode, nRet); 
    ptr += nRet;
    nBuffLen -= nRet;

    return (ptr - lpBuff);
}

int JT808_GetParamDataType(unsigned short nParamType)
{
    if (nParamType >= JT808_DEF_PARAM_CAN_ID_SET_FIRST && 
        nParamType <= JT808_DEF_PARAM_CAN_ID_SET_LAST)//CAN 总线 ID 单独采集设置
    {
		return 4;//BYTE[8] --------C文件需要增加BYTE[8]的解析函数---by ?xlsong
    }
    else if (nParamType >= JT808_DEF_PARAM_TYRE_ID_FIRST && 
             nParamType <= JT808_DEF_PARAM_TYRE_ID_LAST)//轮胎压传感器ID
    {
        return 2;
    }
    
    switch (nParamType)
    {
    case JT808_DEF_PARAM_KEEPALIVE://终端心跳发送间隔，单位为秒S
    case JT808_DEF_PARAM_TCP_TIMEOUT://TCP消息应答超时时间，单位为秒S
    case JT808_DEF_PARAM_TCP_RESEND://TCP消息重传参数
    case JT808_DEF_PARAM_UDP_TIMEOUT://UDP消息应答超时时间，单位为秒S
    case JT808_DEF_PARAM_UDP_RESEND://UDP消息重传参数
    case JT808_DEF_PARAM_SMS_TIMEOUT://SMS消息应答超时时间，单位为秒S
    case JT808_DEF_PARAM_SMS_RESEND://SMS消息重传参数
    case JT808_DEF_PARAM_BD_TIMEOUT://北斗消息应答超时时间，单位为秒S
    case JT808_DEF_PARAM_BD_RESEND://北斗消息重传参数
        return 2;//DWORD
    case JT808_DEF_PARAM_MASTER_APN://主服务器APN，无线通讯拨号访问点.
    case JT808_DEF_PARAM_MASTER_USERNAME://主服务器无线通讯拨号用户名
    case JT808_DEF_PARAM_MASTER_PASSWORD://主服务器无线通讯拨号密码
    case JT808_DEF_PARAM_MASTER_ADDRESS://主服务器IP地址或者域名
    case JT808_DEF_PARAM_SLAVE_APN://备服务器APN，无线通讯拨号访问点.
    case JT808_DEF_PARAM_SLAVE_USERNAME://备服务器无线通讯拨号用户名
    case JT808_DEF_PARAM_SLAVE_PASSWORD://备服务器无线通讯拨号密码
    case JT808_DEF_PARAM_SLAVE_ADDRESS://备服务器IP地址或者域名
	case JT808_DEF_IC_MASTER_ADDRESS://道路运输证 IC 卡认证主服务器 IP 地址或域名  by xlsong
         return 3;//STRING
	case JT808_DEF_IC_TCP_PORT://道路运输证 IC 卡认证主服务器 TCP 端口
	case JT808_DEF_IC_UDP_PORT://道路运输证 IC 卡认证主服务器 UDP 端口
		return 2;//DWORD
	case JT808_DEF_IC_SLAVE_ADDRESS://道路运输证 IC 卡认证备份服务器 IP 地址或域名，端口同主服务器
		return 3;//STRING
    case JT808_DEF_PARAM_TCP_PORT://服务器TCP端口
    case JT808_DEF_PARAM_UDP_PORT://服务器UDP端口
    case JT808_DEF_PARAM_MASTER_BDID://主服务器北斗用户机IC卡号码
    case JT808_DEF_PARAM_SLAVE_BDID://备服务器北斗用户机IC卡号码
    case JT808_DEF_PARAM_REPORT_STRATEGY://位置汇报策略。
    case JT808_DEF_PARAM_REPORT_MODE://位置汇报方案。0，根据Acc状态；
    case JT808_DEF_PARAM_INTERVAL_NODRIVER://驾驶员未登录汇报时间间隔，单位为秒S，>0
    case JT808_DEF_PARAM_INTERVAL_RESTING://休眠时汇报时间间隔，单位为秒S，>0
    case JT808_DEF_PARAM_INTERVAL_ALARM://紧急报警时汇报时间间隔，单位为秒S，>0 
    case JT808_DEF_PARAM_INTERVAL_DEFAULT://缺省时间汇报间隔，单位为秒S，>0
    case JT808_DEF_PARAM_DISTANCE_DEFAULT://缺省距离汇报间隔，单位为米m,>0
    case JT808_DEF_PARAM_DISTANCE_NODRIVER://驾驶员未登录汇报距离间隔。单位为米m，>0
    case JT808_DEF_PARAM_DISTANCE_RESTING://休眠时汇报距离间隔，单位为米m，>0 
    case JT808_DEF_PARAM_DISTANCE_ALARM://紧急报警时汇报距离间隔，单位为米m，>0  
    case JT808_DEF_PARAM_TURNINGANGLE://拐点补传角度，<180°
         return 2;//DWORD
    case JT808_DEF_PARAM_PHONE_PLATFORM://监控平台电话号码
    case JT808_DEF_PARAM_PHONE_RESET://复位电话号码。可用此号码电话拨打终端电话让终端复
    case JT808_DEF_PARAM_PHONE_RESTORE://恢复出厂设置电话号码
    case JT808_DEF_PARAM_PHONE_SMS://监控平台SMS电话号码
    case JT808_DEF_PARAM_PHONE_SMS_SOS://接收终端SMS文本报警号码
        return 3;//STRING
    case JT808_DEF_PARAM_ACCEPT_MODE://终端电话接听策略。0主动接听；
    case JT808_DEF_PARAM_TALK_LIMIT_ONCE://每次通话最长时间。单位为秒S。
    case JT808_DEF_PARAM_TALK_LIMIT_MONTH://当月最长通话时间。单位为秒S。
        return 2;//DWORD
    case JT808_DEF_PARAM_PHONE_MONITOR://监听电话号码
    case JT808_DEF_PARAM_PHONE_PRIVATESMS://监管平台特权短信号码
        return 3;//STRING
    case JT808_DEF_PARAM_WARN_FLAG_IGNORE://报警屏蔽字，与位置信息汇报消息中的报警标识
    case JT808_DEF_PARAM_WARN_FLAG_SMS://报警发送文本SMS开关，与位置信息汇报消息中
    case JT808_DEF_PARAM_WARN_FLAG_CAMERA://报警拍摄开关，与位置信息汇报消息中的报警标志相对
    case JT808_DEF_PARAM_WARN_FLAG_SAVE://报警拍摄存储标志，与位置信息汇报消息中的报警标志
	case JT808_DEF_PARAM_KEY_FLAG://关键标志，与位置信息汇报消息中的报警标志相对应，相应位为 1 则相应报警为关键报警   20130121 add by xlsong
    case JT808_DEF_PARAM_MAX_SPEED://最高速度，单位为千米/小时（Km/h）
    case JT808_DEF_PARAM_MAX_OVERSPEED://超速持续时间，单位为秒S
    case JT808_DEF_PARAM_MAX_DRIVING_ONCE://连续驾驶时间门限，单位为秒S
    case JT808_DEF_PARAM_MAX_DRIVING_DAY://当天累计驾驶时间门限，单位为秒S
    case JT808_DEF_PARAM_MIN_REST://最小休息时间，单位为秒S
    case JT808_DEF_PARAM_MAX_REST://最长停车时间，单位为秒S
		return 2;//DWORD
	case JT808_DEF_PARAM_DIF_OVERSPEED://超速报警预警差值
	case JT808_DEF_PARAM_DIF_TIREDRIVING://疲劳驾驶预警差值
	case JT808_DEF_PARAM_ALARM_HIT://碰撞报警参数设置
	case JT808_DEF_PARAM_ALARM_TURNON://侧翻报警参数设置
		return 1;//WORD
	case JT808_DEF_PARAM_TIMING_CTRL://定时拍照控制
	case JT808_DEF_PARAM_FIXEDDIS_CTRL://定距拍照控制
    case JT808_DEF_PARAM_MEDIA_QUALITY://图像/视频质量，1~10，1为最好
    case JT808_DEF_PARAM_MEDIA_BRIGHTENESS://亮度。0~255
    case JT808_DEF_PARAM_MEDIA_CONTRAST://对比度。0~127 
    case JT808_DEF_PARAM_MEDIA_SATURATION://饱和度。0~127
    case JT808_DEF_PARAM_MEDIA_CHROMA://色度。0~255 
    case JT808_DEF_PARAM_MILEAGE://车辆里程表读数，1/10Km
        return 2;//DWORD
    case JT808_DEF_PARAM_PROVINCEID://车辆所在的省域ID
    case JT808_DEF_PARAM_CITYID://车辆所在的市域ID 
        return 1;//WORD
    case JT808_DEF_PARAM_LICENSEPLATE://车牌号码
        return 3;//STRING
    case JT808_DEF_PARAM_COLOR://车牌颜色。按照JT/T415—2006中5.4.12的规定 
	case JT808_DEF_PARAM_GNSS_POSMODE://GNSS 定位模式
	case JT808_DEF_PARAM_GNSS_BOUTERAYE://GNSS 波特率
	case JT808_DEF_PARAM_GNSS_OUTOUT_HZ://GNSS 模块详细定位数据输出频率
		return 0;//BYTE
	case JT808_DEF_PARAM_GNSS_COLLECT_HZ://GNSS 模块详细定位数据采集频率
		return 2;//DWORD
	case JT808_DEF_PARAM_GNSS_UPLOAD_MODE://GNSS 模块详细定位数据上传方式
        return 0;//BYTE
	case JT808_DEF_PARAM_GNSS_UPLOAD_SET://GNSS 模块详细定位数据上传设置
	case JT808_DEF_PARAM_CAN_CHL1_COLLECT://CAN 总线通道 1 采集时间间隔(ms)
    case JT808_DEF_PARAM_CAN_CHL2_COLLECT://CAN 总线通道 2 采集时间间隔(ms)
        return 2;
	case JT808_DEF_PARAM_CAN_CHL1_UPLOAD://CAN 总线通道 1 上传时间间隔(ms)
	case JT808_DEF_PARAM_CAN_CHL2_UPLOAD://CAN 总线通道 2 上传时间间隔(ms)
		return 1;//WORD
	case JT808_DEF_PARAM_POSI_STANDARD://位移报警标准
	case JT808_DEF_PARAM_TEMP_MIN:				//温度下限
	case JT808_DEF_PARAM_TEMP_MAX:				//温度上限
	case JT808_DEF_PARAM_TYRE_STANDARD:			//胎压标准
		return 1; //WORD
    case JT808_DEF_PARAM_CAMERA_NUMBER://摄像头安装数
        return 0; //CHAR
    case JT808_DEF_PARAM_OILBOX_LENGTH://油箱长度
    case JT808_DEF_PARAM_OILBOX_WIDTH://油箱宽度
    case JT808_DEF_PARAM_OILBOX_HEIGHT://油箱高度
    case JT808_DEF_PARAM_OILSENSOR_HEIGHT://油压传感器高度
    case JT808_DEF_PARAM_OILSENSOR_WARNVALUE://油压异常变化阀值
        return 1;//WORD
    case JT808_DEF_PARAM_TYRE_TYPE://胎压传感器类型
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
//设置终端参数
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
//设置终端参数
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
//查询指定终端参数 Ver201301
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
//查询指定终端参数 Ver201301
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
//查询终端属性   Ver301301
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
//查询终端属性应答  Ver201301
int JT808_DecodePropertyQueryRes(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet = 0,nLen = 0;
	uchar *ptr =lpBuff;
	TJT808PropertyQueryRes *dat = (TJT808PropertyQueryRes *)pPackage;
	
	//终端类型
	nRet = PT_Decode_SHORT(&(dat->nType), ptr, nBuffLen);
	ptr += nRet;
	nBuffLen -= nRet;

	//制造商 ID
	memcpy(dat->szManID, ptr, 5);
	ptr += 5;
	nBuffLen -= 5;
	dat->szManID[5] = '\0';

	//终端型号
    memcpy(dat->szModel, ptr, 20);
    dat->szModel[19] = '\0';
    ptr += 20;
    nBuffLen -= 20;
	//终端ID
	memcpy(dat->szTerminalID, ptr, 7);
	dat->szTerminalID[7] = '\0';
    ptr += 7;
	nBuffLen -= 7;

	//终端 SIM 卡 ICCID
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
	//终端硬件版本号
	nLen = *ptr++;
	nBuffLen--;
	memcpy(dat->szHWVerNum,ptr,nLen);
	nBuffLen -= nLen;
	ptr += nLen;
	// 终端固件版本号
	nLen = *ptr++;
	nBuffLen--;
	memcpy(dat->szFWVerNum,ptr,nLen);
	nBuffLen -= nLen;
	ptr += nLen;
	//GNSS 模块属性
	dat->nGNSSProp = *ptr++;
	nBuffLen--;
	//通信模块属性
	dat->nCommProp = *ptr++;
	nBuffLen--;
    
	return (ptr - lpBuff);
}
int JT808_EncodePropertyQueryRes(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet, nLen;
	uchar *ptr = lpBuff;
	TJT808PropertyQueryRes *dat = (TJT808PropertyQueryRes *)pPackage;

	//终端类型
	nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nType);
	ptr += nRet;
	nBuffLen -= nRet;

	//制造商 ID
	memcpy(ptr, dat->szManID, 5);
	ptr += 5;
	nBuffLen -= 5;

	//终端型号
    nLen = strlen(dat->szModel);
    memcpy(ptr, dat->szModel, nLen);
    
    if (nLen < 20)
    {
        memset(ptr + nLen, 0, 20 - nLen);
    }
    
    ptr += 20;
	nBuffLen -= 20;

	//终端ID
    nLen = strlen(dat->szTerminalID);
    memcpy(ptr, dat->szTerminalID, nLen);
    
    if (nLen < 7)
    {
        memset(ptr + nLen, 0, 7 - nLen);
    }
    
    ptr += 7;
	nBuffLen -= 7;
	//终端 SIM 卡 ICCID
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
	//硬件版本号
    nLen = strlen(dat->szHWVerNum) + 1;
	*ptr++ = nLen;
	nBuffLen--;
	memcpy(ptr, dat->szHWVerNum, nLen);
	ptr += nLen;
	nBuffLen -= nLen;
	//固件版本号
    nLen = strlen(dat->szFWVerNum) + 1;
	*ptr++ = nLen;
	nBuffLen--;
	memcpy(ptr, dat->szFWVerNum, nLen);
	ptr += nLen;
	nBuffLen -= nLen;

	//GNSS 模块属性
	*ptr++ = dat->nGNSSProp;
	nBuffLen--;

	//通信模块属性
	*ptr++ = dat->nCommProp;
	nBuffLen--;
	return (ptr - lpBuff);
		
}
//下发终端升级包  Ver301301
int JT808_DecodeUpdatePackageReq(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	unsigned int nRet;
	uchar *ptr = lpBuff;
	TJT808UpdatePackageReq *dat = (TJT808UpdatePackageReq *)pPackage;

    if (dat->head.nPacketSeq == 1)
    {
	    // 升级类型
	    dat->nType = *ptr++;
	    nBuffLen--;

	    //制造商 ID
	    memcpy(dat->szManID, ptr, 5);
	    ptr += 5;
	    nBuffLen -= 5;
	    dat->szManID[5] = '\0';

	    //版本号
	    nRet = *ptr++;
	    nBuffLen--;

	    memcpy(dat->szVerNum, ptr, nRet);
        dat->szVerNum[nRet] = '\0';
	    nBuffLen -= nRet;
	    ptr += nRet;

        //升级数据包长度
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
//下发终端升级包  Ver301301
int JT808_EncodeUpdatePackageReq(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	unsigned int nRet;
	uchar *ptr = lpBuff;
	TJT808UpdatePackageReq *dat = (TJT808UpdatePackageReq *)pPackage;

    if (dat->head.nPacketSeq == 1)
    {
	    //升级类型
	    *ptr++ = dat->nType;
	    nBuffLen--;
    	
	    //制造商 ID
	    memcpy(ptr, dat->szManID, 5);
	    ptr += 5;
	    nBuffLen -= 5;

	    //版本号
        nRet = strlen(dat->szVerNum) + 1;
	    *ptr++ = nRet;
	    nBuffLen--;

	    memcpy(ptr, dat->szVerNum, nRet);
	    ptr += nRet;
	    nBuffLen -= nRet;

        //升级数据包长度
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
//终端升级结果通知  Ver301301
int JT808_DecodeUpdatePackageRes(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	uchar *ptr = lpBuff;
	TJT808UpdatePackageRes *dat = (TJT808UpdatePackageRes *)pPackage;

	// 升级类型
	dat->nUpdateType = *ptr++;
	nBuffLen--;

	//升级结果 0：成功，1：失败，2：取消
	dat->nResult = *ptr++;
	nBuffLen--;

	return (ptr - lpBuff);
}
//终端升级结果通知  Ver301301
int JT808_EncodeUpdatePackageRes(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	uchar *ptr = lpBuff;
	TJT808UpdatePackageRes *dat = (TJT808UpdatePackageRes *)pPackage;

	// 升级类型
	*ptr++ = dat->nUpdateType;
	nBuffLen--;

	//升级结果 0：成功，1：失败，2：取消
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

    if (dat->nCMD == 1)//无线升级
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
    else if (dat->nCMD == 2)//控制终端连接指定服务器
    {
		dat->cmdinfo.platform.nCtrl = *ptr++;//连接控制
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
    
    if (dat->nCMD == 1)//无线升级
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
        nBuffLen -= nRet;//20130121 modify by xlsong     未验证！！！

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
    else if (dat->nCMD == 2)//控制终端连接指定服务器
    {
        nRet = sprintf(ptr, "%d", dat->cmdinfo.platform.nCtrl);
        ptr += nRet;
        nBuffLen -= nRet;

        if (dat->cmdinfo.platform.nCtrl == 0)//切换到指定监管平台服务器,
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
    while (nBuffLen > 2) //附加信息
    {
        nType = *ptr++;
        nValue = *ptr++;//信息长度字节
        nBuffLen -= 2;

        if (nType == 0x01 && nBuffLen >= nValue && nValue == 4)//里程
        {
            pos->addininfo.nFlag |= JT808_DEF_INFO_MILEAGE;
            nRet = PT_Decode_INT32(&(pos->addininfo.nMileage), ptr, nBuffLen);
            ptr += nRet;
            nBuffLen -= nRet;
        }
        else if (nType == 0x02 && nBuffLen >= nValue && nValue == 2)//油量
        {
            pos->addininfo.nFlag |= JT808_DEF_INFO_OIL;
            nRet = PT_Decode_SHORT(&(pos->addininfo.nOil), ptr, nBuffLen);
            ptr += nRet;
            nBuffLen -= nRet;
        }
        else if (nType == 0x03 && nBuffLen >= nValue && nValue == 2)//记录仪速度
        {
            pos->addininfo.nFlag |= JT808_DEF_INFO_SPEED;
            nRet = PT_Decode_SHORT(&(pos->addininfo.nSpeed), ptr, nBuffLen);
            pos->addininfo.nSpeed /= 10;
            ptr += nRet;
            nBuffLen -= nRet;
        }
		else if (nType == 0x04 && nBuffLen >= nValue && nValue == 2)//需要人工确认报警事件的ID
		{
			pos->addininfo.nFlag |= JT808_DEF_INFO_EVENTID;
			nRet = PT_Decode_SHORT(&(pos->addininfo.nEventID), ptr, nBuffLen);
			ptr += nRet;
			nBuffLen -= nRet;
		}
        else if (nType == 0x11 && nBuffLen >= nValue && (nValue == 1 || nValue == 5))//超速报警附加信息
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
        else if (nType == 0x12 && nBuffLen >= nValue && nValue == 6)//进出区域/路线报警附加信息
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
        else if (nType == 0x13 && nBuffLen >= nValue && nValue == 7)//路段行驶时间不足/过长报警附加信息
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
		else if (nType == 0x25 && nBuffLen >= nValue && nValue == 4)//扩展车辆信号状态位
		{
			pos->addininfo.nFlag |= JT808_DEF_INFO_CARSIGNAL;

			nRet = PT_Decode_INT32(&(pos->addininfo.nCarSignalStatus), ptr, nBuffLen);
			ptr += nRet;
			nBuffLen -= nRet;
		}
		else if (nType == 0x2A && nBuffLen >= nValue && nValue == 2)//IO状态位
		{
			pos->addininfo.nFlag |= JT808_DEF_INFO_IOSTATUS;
			nRet = PT_Decode_SHORT(&(pos->addininfo.nIoStatus), ptr, nBuffLen);
			ptr += nRet;
			nBuffLen -= nRet;		
		}
		else if (nType == 0x2B && nBuffLen >= nValue && nValue == 4)////模拟量
		{
			pos->addininfo.nFlag |= JT808_DEF_INFO_AD;
			nRet = PT_Decode_INT32(&(pos->addininfo.nAnalogData), ptr, nBuffLen);
			ptr += nRet;
			nBuffLen -= nRet;
		}
		else if (nType == 0x30 && nBuffLen >= nValue && nValue == 1)//0x30 无线通信网络信号强度
		{
			pos->addininfo.nFlag |= JT808_DEF_INFO_WLSIGNAL;
			pos->addininfo.nWLSignal = *ptr++; 
			nBuffLen--;
		}
		else if (nType == 0x31 && nBuffLen >= nValue && nValue == 1)//0x31 GNSS 定位卫星数
		{
			pos->addininfo.nFlag |= JT808_DEF_INFO_GNSSSTARS;
			pos->addininfo.nGnssStars = *ptr++; 
			nBuffLen--;
		}
        else if (nType == 0xE0 && nBuffLen >= nValue )//自定义信息长度
        {
            nUserDefLen = nValue;
            break;
        }
		else//unknown
		{
			nBuffLen -= nValue;	
		}
    }
    
    //自定义信息
    pos->userdef.nFlag = 0;
    while (nBuffLen > 0)
    {
        nType = *ptr++;
        nBuffLen--;
        
        if (nType == 0xF0 && nBuffLen > 0)//油压
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
        else if (nType == 0xF1 && nBuffLen > 0)//胎压旧版，无轮位标识
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
        else if (nType == 0xF2 && nBuffLen > 0)//温度
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
        else if (nType == 0xF3 && nBuffLen > 0)//位移
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
        else if (nType == 0xF4 && nBuffLen > 0)//载货状态
        {
            pos->userdef.nFlag |= JT808_DEF_INFO1_CARRY;
            pos->userdef.nCarryState = *ptr++;
            nBuffLen--;
        }
        else if (nType == 0xF5 && nBuffLen > 0)//胎压新版，增加轮位标识
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
        else if (nType == 0xFF && nBuffLen >= 4)//服务器标识信息,自定义附加信息
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
	
    if ((nBuffLen > 2) && (pos->addininfo.nFlag)) //附加信息
    {
        if (pos->addininfo.nFlag & JT808_DEF_INFO_MILEAGE)//里程     modify by xlsong
        {
			*ptr++ = 0x01;//id
			*ptr++ = 4;	  //length
			nBuffLen -= 2;

            nRet = PT_Encode_INT32(ptr, nBuffLen, pos->addininfo.nMileage);
            ptr += nRet;
            nBuffLen -= nRet;
        }

        if (pos->addininfo.nFlag & JT808_DEF_INFO_OIL)//油量
        {
			*ptr++ = 0x02;	//id
			*ptr++ = 2;	    //length
			nBuffLen -= 2;

            nRet = PT_Encode_SHORT(ptr, nBuffLen, pos->addininfo.nOil);
            ptr += nRet;
            nBuffLen -= nRet;
        }

        if (pos->addininfo.nFlag & JT808_DEF_INFO_SPEED)//记录仪速度
        {
			*ptr++ = 0x03; //id
			*ptr++ = 2;	  //length
			nBuffLen -= 2;

            nRet = PT_Encode_SHORT(ptr, nBuffLen, pos->addininfo.nSpeed * 10);
            ptr += nRet;
            nBuffLen -= nRet;
        }

        if (pos->addininfo.nFlag & JT808_DEF_INFO_OVERSPEED)//超速报警附加信息
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

        if (pos->addininfo.nFlag & JT808_DEF_INFO_AREA_ROUTE)//进出区域/路线报警附加信息
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

        if (pos->addininfo.nFlag & JT808_DEF_INFO_DRIVING_TIME)//路段行驶时间不足/过长报警附加信息
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

		if (pos->addininfo.nFlag & JT808_DEF_INFO_EVENTID)//需要人工确认报警事件的 ID
		{
            *ptr++ = 0x04; //id
			*ptr++ = 2;	  //length
			nBuffLen -= 2;
			nRet = PT_Encode_SHORT(ptr, nBuffLen, pos->addininfo.nEventID);
			ptr += nRet;
			nBuffLen -= nRet;
		}

		if (pos->addininfo.nFlag & JT808_DEF_INFO_CARSIGNAL)//扩展车辆信号状态位
		{
            *ptr++ = 0x25; //id
			*ptr++ = 4;	  //length
			nBuffLen -= 2;
			nRet = PT_Encode_INT32(ptr, nBuffLen, pos->addininfo.nCarSignalStatus);
			ptr += nRet;
			nBuffLen -= nRet;
		}

		if (pos->addininfo.nFlag & JT808_DEF_INFO_IOSTATUS)//IO状态位
		{
            *ptr++ = 0x2A; //id
			*ptr++ = 2;	  //length
			nBuffLen -= 2;
			nRet = PT_Encode_SHORT(ptr, nBuffLen, pos->addininfo.nIoStatus);
			ptr += nRet;
			nBuffLen -= nRet;
		}

		if (pos->addininfo.nFlag & JT808_DEF_INFO_AD)//模拟量
		{
            *ptr++ = 0x2B; //id
			*ptr++ = 4;	  //length
			nBuffLen -= 2;
			nRet = PT_Encode_INT32(ptr, nBuffLen, pos->addininfo.nAnalogData);
			ptr += nRet;
			nBuffLen -= nRet;
		}

		if (pos->addininfo.nFlag & JT808_DEF_INFO_WLSIGNAL)//无线通信网络信号强度
		{
            *ptr++ = 0x30; //id
			*ptr++ = 1;	  //length
			nBuffLen -= 2;
			*ptr++ = pos->addininfo.nWLSignal;
			nBuffLen--;
		}

		if (pos->addininfo.nFlag & JT808_DEF_INFO_GNSSSTARS)//GNSS 定位卫星数
		{
            *ptr++ = 0x31; //id
			*ptr++ = 1;	  //length
			*ptr++ = pos->addininfo.nGnssStars;
			nBuffLen--;
		}
    }

    //自定义信息
    if (pos->userdef.nFlag && nBuffLen > 2)
    {
        unsigned char * p = 0;
        unsigned char i = 0;
        *ptr++ = 0xE0;
        p = ptr++;

        if ((pos->userdef.nFlag & JT808_DEF_INFO1_OIL) && pos->userdef.nOilCount > 0)//油压
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

        if ((pos->userdef.nFlag & JT808_DEF_INFO1_TYRE) && pos->userdef.nTyreCount > 0)//胎压
        {
            //*ptr++ = 0xF1; //id早期版本，无轮位标识
            *ptr++ = 0xF5; //id增加轮位标识
            *ptr++ = pos->userdef.nTyreCount;
            nBuffLen -= 2;

            for (i = 0; i < pos->userdef.nTyreCount; i++)
            {
                *ptr++ = pos->userdef.tyres[i].nIndex;//轮位
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

        if ((pos->userdef.nFlag & JT808_DEF_INFO1_TEMP) && pos->userdef.nTempCount > 0)//温度
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

        if ((pos->userdef.nFlag & JT808_DEF_INFO1_POSI) && pos->userdef.nPosiCount > 0)//位移
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

        if (pos->userdef.nFlag & JT808_DEF_INFO1_CARRY)//载货
        {
            *ptr++ = 0xF4; //id
            *ptr++ = pos->userdef.nCarryState;
            nBuffLen -= 2;
        }

        if (pos->userdef.nFlag & JT808_DEF_INFO1_SERVERFLAG)//服务器标识信息,自定义附加信息
        {
            *ptr++ = 0xFF; //id
            nBuffLen--;

            nRet = PT_Encode_INT32(ptr, nBuffLen, pos->userdef.nServerFlag);
            ptr += nRet;
            nBuffLen -= nRet;
        }

        *p = ptr - p - 1;//自定信息编码长度
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
//人工确认报警消息   Ver201301
int JT808_DecodeWarmManage(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet;
	uchar *ptr = lpBuff;
	TJT808WarmManage *dat = (TJT808WarmManage *)pPackage;

	//报警消息流水号 
	nRet = PT_Decode_SHORT(&(dat->nSeqNum),ptr, nBuffLen);
	ptr += nRet;
	nBuffLen -= nRet;

	//人工确认报警类型
	nRet = PT_Decode_INT32(&(dat->nWarmType),ptr, nBuffLen);
	ptr += nRet;
	nBuffLen -= nRet;

	return ptr - lpBuff;
}
//人工确认报警消息   Ver201301
int JT808_EncodeWarmManage(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet;
	uchar *ptr = lpBuff;
	TJT808WarmManage *dat = (TJT808WarmManage *)pPackage;

	//报警消息流水号 
	nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nSeqNum);
	ptr += nRet;
	nBuffLen -= nRet;

	//人工确认报警类型
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
//文本下发
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
//文本下发
int JT808_EncodeTextNotice(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808TextNotice *dat = (TJT808TextNotice *)pPackage;

    *ptr++ = dat->nFlag;
    nBuffLen--;

    nRet = strlen(dat->szText) + 1;//包含结束符
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

                nRet = strlen(dat->events[i].szEvent) + 1;//包含结束符
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

    nRet = strlen(dat->szQuestion) + 1;//包含结束符
    *ptr++ = nRet;
    nBuffLen--;

    memcpy(ptr, dat->szQuestion, nRet);
    ptr += nRet;
    nBuffLen -= nRet;

    for (i = 0; i < dat->nKeyCount; i++)
    {
        *ptr++ = dat->keys[i].nKeyID;
        nBuffLen--;
        
        nLen = strlen(dat->keys[i].szKey) + 1;//包含结束符
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

            nLen = strlen(dat->services[i].szService) + 1;//包含结束符
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

    nLen = strlen(dat->szService) + 1;//包含结束符
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

    nRet = strlen(dat->szPhone) + 1;//包含结束符
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
            
            nRet = strlen(dat->book[i].szPhone) + 1;//包含结束符
            *ptr++ = nRet;
            nBuffLen--;

            memcpy(ptr, dat->book[i].szPhone, nRet);
            ptr += nRet; 
            nBuffLen -= nRet;

            nRet = strlen(dat->book[i].szName) + 1; //包含结束符
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
    //【注】ITS 客户端,该指令为16bit     but I am not entirely sure      ---by xlsong
	nRet = PT_Decode_SHORT(&(dat->nCMD), ptr, nBuffLen);
	ptr += nRet;
	nBuffLen -= nRet;
#else 
    dat->nCMD = *ptr++;
    nBuffLen--;
 #endif
    //自定义
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

    //自定义
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

    if (dat->head.nPacketSeq == 1)//首包增加数据信息
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

    if (dat->head.nPacketSeq == 1)//首包增加数据信息
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
//驾驶员身份信息采集上报
int JT808_DecodeDriverInfoReport(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808DriverInfoReport *dat = (TJT808DriverInfoReport *)pPackage;

#ifdef PT_JT808_VER_201301
	//状态
    dat->nStatus = *ptr++;
	nBuffLen--;
	//时间
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
//驾驶员身份信息采集上报
int JT808_EncodeDriverInfoReport(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808DriverInfoReport *dat = (TJT808DriverInfoReport *)pPackage;

#ifdef PT_JT808_VER_201301
	//状态
	*ptr++ = dat->nStatus;
	nBuffLen--;
	//时间
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
    nRet = strlen(dat->szName) + 1;//包含结束符
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
    nRet = strlen(dat->szIssueDepartment) + 1;//包含结束符
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
//定位数据批量上传  Ver201301
int JT808_DecodePosDataUpload(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet,i = 0;
	unsigned short nLen = 0;
	uchar *ptr = lpBuff;
	TJT808PosDataUpload *dat = (TJT808PosDataUpload *)pPackage;	
    
    if (dat->head.nPacketSeq == 1)
    {
        //数据项个数
        nRet = PT_Decode_SHORT(&(dat->nTotalCount), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;

        //位置数据类型
        dat->nPosType = *ptr++;
        nBuffLen--;
    }

#ifdef JT808_PLATFORM	
	//位置汇报数据体
    dat->nDataCount = 0;
	while (i = 0; i < dat->nDataCount; i++)
	{
		//位置汇报数据体长度
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
//定位数据批量上传  Ver201301
int JT808_EncodePosDataUpload(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet,i = 0;
	unsigned short nLen = 0;
	uchar *ptr = lpBuff;
	TJT808PosDataUpload *dat = (TJT808PosDataUpload *)pPackage;	

    if (dat->head.nPacketSeq == 1)
    {
        //数据项个数
        nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nTotalCount);
        ptr += nRet;
        nBuffLen -= nRet;

        //位置数据类型
        *ptr++ = dat->nPosType;
        nBuffLen--;
    }
#ifdef JT808_PLATFORM
	//位置汇报数据体
	for(i = 0; i < dat->nDataCount; i++)
	{
		nLen = JT808_EncodePosition(ptr + 2, nBuffLen, &(dat->posdata[i]));
		
		//位置汇报数据体长度
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
//CAN总线数据上传  Ver201301
int JT808_DecodeCanDataUpload(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet;
	uchar *ptr = lpBuff;
	TJT808CanDataUpload *dat = (TJT808CanDataUpload *)pPackage;	

    if (dat->head.nPacketSeq == 1)
    {
        //数据项个数
        nRet = PT_Decode_SHORT(&(dat->nTotalCount), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;
        //CAN 总线数据接收时间
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
//CAN总线数据上传  Ver201301
int JT808_EncodeCanDataUpload(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet;
    unsigned char i = 0;
	uchar *ptr = lpBuff;
	TJT808CanDataUpload *dat = (TJT808CanDataUpload *)pPackage;	
    
    if (dat->head.nPacketSeq == 1)
    {
        //数据项个数
        nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nTotalCount);
        ptr += nRet;
        nBuffLen -= nRet;
        //CAN 总线数据接收时间
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

    if (dat->head.nPacketSeq == 1)//多媒体数据首包包含以下数据
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
//    #ifdef PT_JT808_VER_201301//解析基本位置信息
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

    if (dat->head.nPacketSeq == 1)//多媒体数据首包包含以下数据
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

/*摄像头立即拍摄指令 解码*/
int JT808_DecodeCameralCtrl(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    int nRet;
    uchar *ptr = lpBuff;
    TJT808CameralCtrl *dat = (TJT808CameralCtrl *)pPackage;
    
	dat->nChannel= *ptr++;
    nBuffLen--;

	nRet = PT_Decode_SHORT(&(dat->nType), ptr, nBuffLen);//拍摄标志
    ptr += nRet;
    nBuffLen -= nRet;

	nRet = PT_Decode_SHORT(&(dat->nInterval), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;
	
    dat->nSaveFlag = *ptr++;//保存标志
    nBuffLen--;

    dat->nDistinguishability = *ptr++;//分辨率
    nBuffLen--;

	dat->nPictureQuality = *ptr++;//图像/视频质量
    nBuffLen--;

	dat->nBrightness = *ptr++;//亮度
    nBuffLen--;

	dat->nContrast = *ptr++;//对比度
    nBuffLen--;

	dat->nSaturation = *ptr++;//饱和度
    nBuffLen--;

	dat->nChroma = *ptr++;//色度
    nBuffLen--;

    return ptr - lpBuff;
    
    
}
/*摄像头立即拍摄指令 编码*/
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
//摄像头立即拍摄命令应答 Ver201301
int JT808_DecodeCameralCtrlRes(void *pPackage, uchar *lpBuff, int nBuffLen)
{
	int nRet,i = 0;
	uchar *ptr = lpBuff;
	TJT808CameraCtrlRes *dat = (TJT808CameraCtrlRes *)pPackage;

	nRet = PT_Decode_SHORT(&(dat->nSeqNum), ptr, nBuffLen);//应答流水号
	ptr += nRet;
	nBuffLen -= nRet;

	dat->nResult = *ptr++;//结果
	nBuffLen--;

	//拍摄成功的多媒体个数
	nRet = PT_Decode_SHORT(&(dat->nMediaCount), ptr, nBuffLen);
	ptr += nRet;
	nBuffLen -= nRet;

	//多媒体 ID 列表
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
//摄像头立即拍摄命令应答 Ver201301
int JT808_EncodeCameralCtrlRes(uchar *lpBuff, int nBuffLen, void *pPackage)
{
	int nRet,i = 0;
	uchar *ptr = lpBuff;
	TJT808CameraCtrlRes *dat = (TJT808CameraCtrlRes *)pPackage;

	nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nSeqNum);//应答流水号
	ptr += nRet;
	nBuffLen -= nRet;

	*ptr++ = dat->nResult;//结果
	nBuffLen--;

	//拍摄成功的多媒体个数 
	nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nMediaCount);
	ptr += nRet;
	nBuffLen -= nRet;

	//多媒体 ID 列表
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
/*存储多媒体数据检索 解码*/
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
/*存储多媒体数据检索 编码*/
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
	
    *ptr++ = (((dat->tmStart.nYear / 10) % 10) << 4) | (dat->tmStart.nYear % 10);//起始时间
    *ptr++ = BIN2BCD(dat->tmStart.nMonth);
    *ptr++ = BIN2BCD(dat->tmStart.nDay);
    *ptr++ = BIN2BCD(dat->tmStart.nHour);
    *ptr++ = BIN2BCD(dat->tmStart.nMinute);
    *ptr++ = BIN2BCD(dat->tmStart.nSecond);
    nBuffLen -= 6;

    *ptr++ = (((dat->tmEnd.nYear / 10) % 10) << 4) | (dat->tmEnd.nYear % 10);//结束时间
    *ptr++ = BIN2BCD(dat->tmEnd.nMonth);
    *ptr++ = BIN2BCD(dat->tmEnd.nDay);
    *ptr++ = BIN2BCD(dat->tmEnd.nHour);
    *ptr++ = BIN2BCD(dat->tmEnd.nMinute);
    *ptr++ = BIN2BCD(dat->tmEnd.nSecond);
    nBuffLen -= 6;

	return ptr - lpBuff;
}
/*存储多媒体数据检索应答 解码*/
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
/*存储多媒体数据检索应答  编码*/
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

/*存储多媒体数据上传命令 解码*/
int JT808_DecodeMediaDataUpload(void *pPackage, uchar *lpBuff, int nBuffLen)
{
    uchar *ptr = lpBuff;
    TJT808MediaDataUpload *dat = (TJT808MediaDataUpload *)pPackage;

	dat->nMedia = *ptr++;//多媒体类型
	nBuffLen--;

    dat->nChannel = *ptr++;//通道ID
	nBuffLen--;

	dat->nEventType = *ptr++;//事件项编码
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
/*存储多媒体数据上传命令  编码*/
int JT808_EncodeMediaDataUpload(uchar *lpBuff, int nBuffLen, void *pPackage)
{
    uchar *ptr = lpBuff;
    TJT808MediaDataUpload *dat = (TJT808MediaDataUpload *)pPackage;

	*ptr++ = dat->nMedia;//多媒体类型
	nBuffLen --;

	*ptr++ = dat->nChannel;//通道ID
	nBuffLen --;

	*ptr++ = dat->nEventType;//事件项编码
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
//单条存储多媒体数据检索上传命令 Ver201301
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
//单条存储多媒体数据检索上传命令 Ver201301
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
//上下行数据透传
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
//上下行数据透传
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
//压缩数据上传
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
//压缩数据上传
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

    //油量传感器数
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
    //温度传感器数
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

    //胎压传感器数
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

    //位移传感器数
    dat->nPosiCount = *ptr++;
    nBuffLen--;

    for (i = 0; i < dat->nPosiCount; i++)
    {
        nRet = PT_Decode_SHORT(&(dat->positions[i]), ptr, nBuffLen);
        ptr += nRet;
        nBuffLen -= nRet;
    }

    if (nBuffLen >= 1)//载货状态
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

    //油量传感器数
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
    //温度传感器数
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

    //轮胎传感器数
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

    //位移传感器数
    *ptr++ = dat->nPosiCount;
    nBuffLen--;

    for (i = 0; i < dat->nPosiCount; i++)
    {
        nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->positions[i]);
        ptr += nRet;
        nBuffLen -= nRet;
    }

    //载货状态
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

        //载货状态
        dat->nCarryState = *ptr++;
        nBuffLen--;

        //油量传感器数据长度
        nRet = *ptr++;
        nBuffLen--;

        if (nRet > 0)
        {
            memcpy(dat->szOil, ptr, nRet);
            dat->szOil[nRet] = '\0';

            ptr += nRet;
            nBuffLen -= nRet;
        }
        
        //温度传感器数
        nRet = *ptr++;
        nBuffLen--;

        if (nRet > 0)
        {
            memcpy(dat->szTemp, ptr, nRet);
            dat->szTemp[nRet] = '\0';

            ptr += nRet;
            nBuffLen -= nRet;
        }


        //胎压传感器数
        nRet = *ptr++;
        nBuffLen--;

        if (nRet > 0)
        {
            memcpy(dat->szTyre, ptr, nRet);
            dat->szTyre[nRet] = '\0';

            ptr += nRet;
            nBuffLen -= nRet;
        }

        //位移传感器数
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

        //载货状态
        *ptr++ = dat->nCarryState;
        nBuffLen--;

        //油量传感器数据长度
        nRet = strlen(dat->szOil);
        *ptr++ = nRet;
        nBuffLen--;

        if (nRet > 0)
        {
            memcpy(ptr, dat->szOil, nRet);

            ptr += nRet;
            nBuffLen -= nRet;
        }

        //温度传感器数
        nRet = strlen(dat->szTemp);
        *ptr++ = nRet;
        nBuffLen--;

        if (nRet > 0)
        {
            memcpy(ptr, dat->szTemp, nRet);

            ptr += nRet;
            nBuffLen -= nRet;
        }

        //胎压传感器数
        nRet = strlen(dat->szTyre);
        *ptr++ = nRet;
        nBuffLen--;

        if (nRet > 0)
        {
            memcpy(ptr, dat->szTyre, nRet);

            ptr += nRet;
            nBuffLen -= nRet;
        }
        //位移传感器数
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
    
    //省域ID
    nRet = PT_Decode_SHORT(&(dat->nProvID), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    //市县域ID
    nRet = PT_Decode_SHORT(&(dat->nCity), ptr, nBuffLen);
    ptr += nRet;
    nBuffLen -= nRet;

    //制造商ID
    memcpy(dat->szManufacturer, ptr, 5);
    dat->szManufacturer[5] = '\0';
    ptr += 5;
    nBuffLen -= 5;

    //终端型号
    memcpy(dat->szModel, ptr, 20);
    dat->szModel[19] = '\0';
    ptr += 20;
    nBuffLen -= 20;

    //终端ID
    memcpy(dat->szDeviceID, ptr, 7);
    dat->szDeviceID[7] = '\0';
    ptr += 7;
    nBuffLen -= 7;

    //车牌颜色
    *ptr++ = dat->nColor;
    nBuffLen -= 1;

    //车牌
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
    
    //加密密钥
    nRet = PT_Encode_INT32(ptr, nBuffLen, dat->nEncryptKey);
    ptr += nRet;
    nBuffLen -= nRet;
    //车主手机号码
    memcpy(ptr, dat->szUserMobile, 12);
    ptr += 12;
    nBuffLen -= 12;
    //省域ID
    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nProvID);
    ptr += nRet;
    nBuffLen -= nRet;

    //市县域ID
    nRet = PT_Encode_SHORT(ptr, nBuffLen, dat->nCity);
    ptr += nRet;
    nBuffLen -= nRet;

    //制造商ID
    memcpy(ptr, dat->szManufacturer, 5);
    ptr += 5;
    nBuffLen -= 5;

    //终端型号
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

    //终端ID
    nRet = strlen(dat->szDeviceID);//包含结束符
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
    
    //车牌颜色
    *ptr++ = dat->nColor;
    nBuffLen -= 1;

    //车牌
    nRet = strlen(dat->szLicenseplate);//包含结束符
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
    nRet = strlen(dat->szVIN) + 1;//包含结束符
    memcpy(ptr,dat->szVIN, 17); 
    ptr += 17;
    nBuffLen -= 17;
    *ptr++ = 0x00;

    return (ptr - lpBuff);
}
///////////////////////////////////////
#endif//end PT_ENABLE_JT808
