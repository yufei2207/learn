/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     protocol.h
* Abstract： protocol interface
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

#ifndef H_PT_DEFINE_H
#define H_PT_DEFINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef NULL
    #define NULL    0
#endif

typedef unsigned char  uchar;

#include "pt_utility.h"

//result define 
#define	 RESULT_ERROR           -1
#define	 RESULT_VERIFY_ERROR    -2
#define	 RESULT_INVILID_TYPE    -3
#define	 RESULT_SUCCESS          0

typedef enum TStackType
{
   STACK_BD1        = 1 << 0, 
   STACK_BD2_RD     = 1 << 1, 
   STACK_BD2_RN     = 1 << 2, 
   STACK_BD2_SP     = 1 << 3, 
   STACK_BD2_EX     = 1 << 4, 
   STACK_NW         = 1 << 5,
   STACK_GPS        = 1 << 6,
   STACK_GT06       = 1 << 7,
   STACK_JT808      = 1 << 8,
   STACK_JT808_DV   = 1 << 9,
}TStackType;

typedef struct TStackParam
{
    int  nStackType; //协议栈类型
    int  nRecvFlag;  //接收状态
    char bRecvFrame; //是否接收到一帧数据

    short nFrameLen;//一代协议以及二代RD特殊协议帧长度
    unsigned short nCRCValue;//校验
    //是否计算校验
    uchar bCRC;
    //接收buff
    uchar *RecvBuff;
    //接收缓存长度
    int   nRecvBuffLen;
    //接收字符计数
    int   nRecvLen;
    //协议类型
    int   nMsgType;
    //数据接收函数指针
    void *fnRecv;
}TStackParam;

//解码函数接口定义
typedef int (*FUNC_DECODER)(void *pPackage, uchar *lpBuff, int nBuffLen);
//编码函数接口定义
typedef int (*FUNC_ENCODER)(uchar *lpBuff, int nBuffLen, void* pPackage);

#include "pt_config.h"  //协议栈配置文件

#ifdef PT_ENABLE_BD1
    #include "./bd1/pt_bd1.h"
#endif 

#ifdef PT_ENABLE_BD2_RD
    #include "./bd2/pt_bd2_rdss.h"
#endif 

#ifdef PT_ENABLE_BD2_RN
    #include "./bd2/pt_bd2_rnss.h"
#endif 

#ifdef PT_ENABLE_BD2_EXPRESS
    #include "./bd2/pt_bd2_express.h"
#endif 
       
#ifdef PT_ENABLE_BD2_SPECIAL
    #include "./bd2/pt_bd2_special.h"
#endif 

#ifdef PT_ENABLE_NW
    #include "./nw/pt_nw_define.h"
#endif 

#ifdef PT_ENABLE_GPS
    #include "./gps/pt_gps.h"
#endif 

#ifdef PT_ENABLE_GT06
    #include "./gt06/pt_gt06.h"
#endif

#ifdef PT_ENABLE_JT808
    #include "./jt808/pt_jt808.h"
#endif

#ifdef PT_ENABLE_JT808_DV
    #include "./jt808/pt_jt808_device.h"
#endif

typedef enum TProtocolType
{
    INVALID_TYPE = -1,
    TY_NULL = 0,
#ifdef PT_ENABLE_BD1   //一代协议，外设至用户机信息
    BD1_ZERO,
    BD1_GLJC = BD1_ZERO,//功率检测
    BD1_DWSQ,      //定位申请
    BD1_TXSQ,      //通信申请
    BD1_HXSZ,      //航线设置
    BD1_DHSQ,      //导航申请
    BD1_DSSQ,      //定时申请
    BD1_SCSC,      //时差输出
    BD1_ZBZH,      //坐标转换
    BD1_JSZL,      //结束指令
    BD1_GBXX,      //广播信息
    BD1_GYXX,      //公用信息
    BD1_WMCS,      //误码测试
    BD1_CKSC,      //串口输出
    BD1_ICJC,      //IC检测
    BD1_JJZH,      //紧急自毁
    BD1_BSSZ,      //波束设置
    BD1_GXZX,      //管信注入
    BD1_GXDQ,      //管信读取
    BD1_XTZJ,      //系统自检 
    BD1_LZSZ,      //零值设置
    BD1_LZDQ,      //零值读取
    BD1_SJSC,      //时间输出
    BD1_BBDQ,      //版本读取
    BD1_XHDQ,      //序号读取
    BD1_GLZK,      //功率状况//用户机至外设信息
    BD1_DWXX,      //定位信息
    BD1_TXXX,      //通信信息
    BD1_TXHZ,      //通信回执
    BD1_KLSB,      //口令识别
    BD1_DHSC,      //导航输出
    BD1_DSJG,      //定时结果
    BD1_SCSJ,      //时差数据
    BD1_ZBSC,      //坐标输出
    BD1_GBSC,      //广播输出
    BD1_GYSC,      //公用输出
    BD1_ICXX,      //IC信息
    BD1_ZHQR,      //自毁确认
    BD1_ZJXX,      //自检信息
    BD1_LZXX,      //零值信息
    BD1_SJXX,      //时间信息
    BD1_BBXX,      //版本信息
    BD1_XHXX,      //序号信息
    BD1_GLXX,      //管理信息
    BD1_FKXX,      //反馈信息
    BD1_ILXX,      //I路信息
    BD1_QLXX,      //Q路信息
    BD1_RZCW,       //自定义协议
    BD1_BJLZ,
    BD1_DQGK,      //读取功况
    BD1_BHXX,      //捕获信息
    BD1_SSXX,      //授时信息
    BD1_KZXX,      //
    BD1_DSXX,      
    BD1_SZXX,
    BD1_DSLX,
    BD1_BJWZ,
    BD1_FPGA,
    BD1_BDSZ,      //电力设置信息
    BD1_LAST = BD1_BDSZ, //一代协议结束
#endif//PT_ENABLE_BD1
#ifdef PT_ENABLE_BD2_RN   //二代RNSS协议
    BD2_RN_ZERO,
    BD2_RN_AAM = BD2_RN_ZERO,//航路点到达报警
    BD2_RN_ALM,//卫星历书数据
    BD2_RN_APL,//完好性保护门限
    BD2_RN_BSD,//描述标识点
    BD2_RN_BSQ,//描述标识区
    BD2_RN_BSX,//描述标识线
    BD2_RN_COM,//设置用户设备串口
    BD2_RN_DHV,//速度类导航信息
    BD2_RN_GBS,//描述GNSS卫星故障检测
    BD2_RN_GGA,//描述定位数据
    BD2_RN_GLL,//大地坐标定位信息
    BD2_RN_GLS,//设置用户设备位置等初始化信息
    BD2_RN_GSA,//设置用户设备工作模式等
    BD2_RN_GST,//描述伪距误差统计数据
    BD2_RN_GSV,//输出卫星信息
    BD2_RN_ICM,//输出用户设备中加解密模块密钥有效期信息
    BD2_RN_IHI,//设置用户设备输出速度加速度等信息
    BD2_RN_IHO,//输出导航辅助信息
    BD2_RN_LPM,//设置设备工作在省电模式
    BD2_RN_MSS,//设置设备当前定位方式
    BD2_RN_PMU,//输出PRM的时效参数信息
    BD2_RN_RMC,//输出最简导航传输数据
    BD2_RN_TXT,//用于传输短文本
    BD2_RN_VTG,//描述航迹向和地速
    BD2_RN_ZBS,//设置坐标系参数
    BD2_RN_ZDA,//描述UTC时间、日期和本地时区
    BD2_RN_ZTI,//输出设备当前工作状态信息
    BD2_RN_LAST = BD2_RN_ZTI,
#endif //PT_ENABLE_BD2_RN
#ifdef PT_ENABLE_BD2_RD   //二代RDSS协议
    BD2_RD_ZERO,
    BD2_RD_BSI = BD2_RD_ZERO,//接收波束状态信息
    BD2_RD_BSS,      //设置响应波束和时差波束
    BD2_RD_CXA,      //设置查询申请
    BD2_RD_DSA,      //设置定时申请
    BD2_RD_DWA,      //设置定位申请
    BD2_RD_DWR,      //定位信息
    BD2_RD_FKI,      //用户设备反馈信息
    BD2_RD_GXM,      //管理信息设置、查询
    BD2_RD_ICA,      //设置检测加解密模块
    BD2_RD_ICI,      //用户设备本机加解密模块信息
    BD2_RD_ICZ,      //指挥机下属用户信息
    BD2_RD_JMS,      //无线静默设置
    BD2_RD_KLS,      //指挥机发送口令识别指令
    BD2_RD_KLT,      //口令识别应答
    BD2_RD_LZM,      //用户设备零值管理
    BD2_RD_HZR,      //回执信息
    BD2_RD_TXA,      //设置通讯申请
    BD2_RD_TXR,      //通讯信息
    BD2_RD_WAA,      //设置或接收位置报告1的位置数据
    BD2_RD_WBA,      //设置位置报告2申请
    BD2_RD_ZHS,      //设置自毁
    BD2_RD_LAST = BD2_RD_ZHS,
#endif//PT_ENABLE_BD2_RD
#ifdef PT_ENABLE_BD2_EXPRESS    //专用语句
    BD2_EX_ZERO,
    BD2_EX_ECS = BD2_EX_ZERO,      //设置输出原始导航信息
    BD2_EX_ECT,      //原始导航信息
    BD2_EX_TCS,      //接收通道强制跟踪
    BD2_EX_IDV,      //干扰检测信息
    BD2_EX_PRD,      //设置用户设备输出伪距观测值和载波相位
    BD2_EX_PRO,      //原始伪距观测值和载波相位
    BD2_EX_RIS,      //设备复位
    BD2_EX_RMO,      //输出激活
    BD2_EX_SCS,      //RDSS双通道时差数据
	BD2_EX_TXM,		//通信设置
	BD2_EX_WGM,		//网关设置
    BD2_EX_LAST = BD2_EX_WGM,
#endif// PT_ENABLE_BD2_EXPRESS
#ifdef PT_ENABLE_BD2_SPECIAL//特殊语句
    BD2_SP_ZERO,
    BD2_SP_XNF = BD2_SP_ZERO,      //用户设备接收惯导辅助信息格式
    BD2_SP_XSD,      //输出下属用户定位信息 
    BD2_SP_XST,      //输出下属用户通讯信息
    BD2_SP_LAST = BD2_SP_XST,
#endif// PT_ENABLE_BD2_SPECIAL
#ifdef PT_ENABLE_NW//诺维自定义协议
    NW_ZERO,
    NW_KJD = NW_ZERO, //开机握手指令
    NW_ICK,     //卡座状态控制
    NW_ICS,     //所有卡座的状态
    NW_ICQ,     //读取卡内信息
    NW_ICH,     //当前卡的基本信息
    NW_JKD,     //接口切换
	NW_LGN,     //设备登录网关
    NW_WZR,     //设备输出位置至客户端
    NW_WZI,     //设备上报位置至平台
	NW_WZQ,     //平台查询设备位置
	NW_STA,     //设备上报状态
	NW_TXI,     //平台与设备短信通信
	NW_FKS,     //网关发送至设备的响应	 服务器发给一体机
    NW_FKD,     //设备发送至网关的响应	 一体机发给服务器
	NW_YDM,     //网关控制指令 关闭\接通油电  服务器发给一体机
	NW_TXS,     //客户端设置一体机通信参数
    NW_HMM,     //增加、删除SOS号码
    NW_HMQ,     //查询SOS号码
	NW_DXQ,     //查询设备存储短信
	NW_DBG,     //管理设备调试信息
    NW_MSS,     //设置工作模式
    NW_LAST = NW_MSS,//诺维自定义协议结束
#endif//PT_ENABLE_NW
#ifdef PT_ENABLE_GPS//GPS协议
    GPS_ZERO,
    GPS_GGA = GPS_ZERO,//全球定位数据
    GPS_GSA,      //卫星PRN数据
    GPS_GSV,      //卫星状态信息
    GPS_RMC,      //运输定位数据
    GPS_VTG,      //地面速度信息
    GPS_GLL,      //大地坐标数据
    GPS_ZDA,      //UTC时间和日期
    GPS_LAST = GPS_ZDA,
#endif//PT_ENABLE_GPS
#ifdef PT_ENABLE_GT06//GPS协议
    GT06_ZERO,
    GT06_LOGINREQ = GT06_ZERO,//登录请求
    GT06_LOGINRES,//登录响应
    GT06_POSITION, //定位
    GT06_STATEREQ,  //状态信息请求
    GT06_STATERES,  //状态信息响应
    GT06_CMDREQ,  //控制指令
    GT06_CMDRES,  //控制指令响应
    GT06_PHOTO,  //上传图片
    GT06_LAST = GT06_PHOTO,
#endif//PT_ENABLE_GPS
#ifdef PT_ENABLE_JT808
    JT808_ZERO,
    JT808_TERMINALRES = JT808_ZERO,//终端通用应答√
    JT808_PLATFORMRES,//平台通用应答√
    JT808_KEEPALIVE,//终端心跳√
    JT808_REGISTERREQ,//终端注册√〇
    JT808_REGISTERRES,//终端注册应答√
    JT808_DEREGISTER,//终端注消√
    JT808_LOGIN,//终端鉴权√
    JT808_PARAMETET_SET,//设置终端参数√〇
    JT808_PARAMETER_QUERYREQ,//查询终端参数√
    JT808_PARAMETER_QUERYRES,//查询终端参数应答√
    JT808_TERMINALCTRL,//终端控制√
    JT808_POSITION_REPORT,//位置信息汇报√〇
    JT808_POSITION_QUERYREQ,//位置信息查询√
    JT808_POSITION_QUERYRES,//位置信息查询应答√
    JT808_POSITION_TRACK,//临时位置跟踪√
    JT808_TEXT_NOTICE,//文本下发√
    JT808_EVENT_SET,//事件设置√
    JT808_EVENT_REPORT,//事件报告√
    JT808_QUESTION_REQ,//提问下发√  
    JT808_QUESTION_RES,//提问应答√
    JT808_SERVICE_SET,//消息点播菜单设置√
    JT808_SERVICE_OPT,//信息点播/取消√
    JT808_SERVICE_INFO,//信息服务√
    JT808_PHONECALLBACK,//电话回拨√
    JT808_PHONEBOOK,//设置电话本√
    JT808_CAR_CTRLREQ,//车辆控制√
    JT808_CAR_CTRLRES,//车辆控制应答√
    JT808_REGION_CIRCLE_SET,//设置圆形区域√
    JT808_REGION_CIRCLE_REMOVE,//删除圆形区域√
    JT808_REGION_RECT_SET,//设置矩形区域√
    JT808_REGION_RECT_REMOVE,//删除矩形区域√
    JT808_REGION_POLYGON_SET,//设置多边形区域√
    JT808_REGION_POLYGON_REMOVE,//删除多边形区域√
    JT808_ROUTE_SET,//设置路线√
    JT808_ROUTE_REMOVE,//删除路线√
    JT808_DRIVINGRECORDER_REQ,//行驶记录仪数据采集命令√
    JT808_DRIVINGRECORDER_RES,//行驶记录仪数据上传√
    JT808_DRIVINGRECORDER_PARAM,  //行驶记录仪参数下传命令√
    JT808_ORDER_REPORT,              //电子运单上报√
    JT808_DRIVERINFO_REPORT,         //驾驶员身份信息采集上报√〇
    JT808_MULTIMEDIA_EVENT_REPORT,//多媒体事件信息上传√
    JT808_MULTIMEDIA_DATA_UPLOADREQ,//多媒体数据上传√
    JT808_MULTIMEDIA_DATA_UPLOADRES,//多媒体数据上传应答√
    JT808_CAMERA_CTRL,//摄像头拍摄指令控制
    JT808_MULTIMEDIA_DATA_QUERYREQ,//存储多媒体数据检索
    JT808_MULTIMEDIA_DATA_QUERYRES,//存储多媒体数据检索应答
    JT808_MULTIMEDIA_DATA_UPLOADCMD,//存储多媒体数据上传命令
    JT808_RECORDER_CTRL,//录音命令开始/停止√
    JT808_DATA_DOWNLOAD,//数据下行透传√
    JT808_DATA_UPLOAD,//数据上行透传√
    JT808_DATA_GZIP_UPLOAD,//数据压缩上报√
    JT808_PLATFORM_RAS,//平台RAS公钥√
    JT808_TERMINAL_RAS,//终端RAS公钥√
    JT808_MULTIPOSITION,//多用户位置上报,自定义协议√
    JT808_SENSOR_REPORT,//传感器数据上传√
    JT808_SENSOR_QUERYREQ,//传感器数据查询请求√
    JT808_SENSOR_QUERYRES,//传感器数据查询反馈√
	JT808_SEND_AGAINREQ,//补传分包请求√  VER_201301
	JT808_SPECIFY_PARAM_QUERYREQ,//查询指定终端参数√
	JT808_PROPERTY_QUERYREQ,//查询终端属性√
	JT808_PROPERTY_QUERYRES,//查询终端属性应答√
	JT808_UPDATE_PACKAGEREQ,//下发终端升级包√
	JT808_UPDATE_PACKAGERES,//终端升级结果通知√
	JT808_WARM_MANAGE,//人工确认报警消息√
	JT808_DRIVERINFO_REPORTREQ,//上报驾驶员身份信息请求√
	JT808_POSDATA_UPLOAD,//定位数据批量上传√
	JT808_CAN_DATA_UPLOAD,//CAN总线数据上传
	JT808_CAMERA_CTRL_RES,//摄像头立即拍摄命令应答
	JT808_ONE_MULTIMEDIA_DATA_QUERYREQ,//单条存储多媒体数据检索上传命令
    JT808_DEVICE_UNLOCK,//使用前解锁
	JT808_LAST,

#endif //PT_ENABLE_JT808
#ifdef PT_ENABLE_JT808_DV
    JT808_DV_ZERO,
    JT808_DV_POWERON = JT808_DV_ZERO,  //上电指示，通用消息
    JT808_DV_LINKCHECK,//链路检测，通用消息
    JT808_DV_POWERCTRL,//电源控制，通用消息
    JT808_DV_VERSION,  //外设版本检测，通用消息
    JT808_DV_SELFCHECK, //外设自检，通用消息
    JT808_DV_LIVEUPDATE,//外设固件升级，通用消息
    JT808_DV_IC_AUTHENTICATE,  //IC认证请求/应答
    JT808_DV_IC_READRESULT, //IC读取结果请求/应答
    JT808_DV_IC_PULLOUT, //IC拔出通知/应答
    JT808_DV_IC_READ, //IC主动触发读取请求/应答
    JT808_DV_POSITION, //外设定位数据,自定义消息
    JT808_DV_PARAM, //外设参数,自定义消息
    JT808_DV_LAST,
#endif //PT_ENABLE_JT808_DV
}TProtocolType;

/*
*函数名称：PT_InitLib
*函数说明：初始化协议库
*参数说明：无
*返回值：  无
*/
void  PT_InitLib(void);

/*
*函数名称：PT_Initialize
*函数说明：初始化协议栈参数
*参数说明：nStackType 协议栈类型
*返回值：  -1 表示协议栈数不足，其他表示栈ID
*/
int  PT_Initialize(int nStackType);
/*
*函数名称：PT_InitializeEx
*函数说明：初始化协议栈参数
*参数说明：pParam 协议接收栈指针
*返回值：  -1 表示失败，其他成功
*/
int  PT_InitializeEx(TStackParam *pParam);

/*
*函数名称：PT_Uninitialize
*函数说明：释放协议栈
*参数说明：nStackID 栈ID
*返回值：  无
*/
void PT_Uninitialize(int nStackID);
/*
*函数名称：PT_UninitializeEx
*函数说明：释放协议栈
*参数说明：pParam 协议接收栈指针
*返回值：  无
*/
void PT_UninitializeEx(TStackParam *pParam);

/*
*函数名称：PT_GetRecvData
*函数说明：获取接收缓存的数据
*参数说明：lpBuff   数据缓存指针
*          nBuffLen 数据缓存长度
*          nStackID 栈ID
*返回值：  0 表示解码成功，其他表示失败
*/
int PT_GetRecvData(uchar *lpBuff, int nBuffLen, int nStackID);

/*
*函数名称：PT_RecvData
*函数说明：解码接收的数据至协议数据结构
*参数说明：nStackID 栈ID
*          data     接收的数据
*返回值：  -1 表示没有收到整帧数据，其他表示所接收帧数据的消息类型
*/
int  PT_RecvData(int nStackID, uchar data);

/*
*函数名称：PT_RecvDataEx
*函数说明：解码接收的数据至协议数据结构
*参数说明：pParam 协议接收栈指针
*          data     接收的数据
*返回值：  -1 表示没有收到整帧数据，其他表示所接收帧数据的消息类型
*/
int  PT_RecvDataEx(TStackParam *pParam, uchar data);


/*
*函数名称：PT_DecodeRecvData
*函数说明：解码协议栈接收的数据至协议数据结构
*参数说明：pPackage 结构体指针
*          nStackID 栈ID
*返回值：  0 表示解码成功，其他表示失败
*/
int PT_DecodeRecvData(void *pPackage, int nStackID);

/*
*函数名称：PT_DecodeRecvDataEx
*函数说明：解码协议栈接收的数据至协议数据结构
*参数说明：pPackage 结构体指针
*          pParam 协议接收栈指针
*返回值：  0 表示解码成功，其他表示失败
*/
int PT_DecodeRecvDataEx(void *pPackage, TStackParam *pParam);

/*
*函数名称：PT_DecodeType
*函数说明：解码接收到整帧数据的消息类型
*参数说明：buf    接收数据缓存
*          len    接收数据长度
*          nStackType 协议栈类型 
*返回值：  -1 表示无效的消息类型，其他表示解析的类型
*/
int PT_DecodeType(const uchar* lpBuff, int nBuffLen, int nStackType);

/*
*函数名称：PT_DecodePackage
*函数说明：解码接收整帧数据至协议数据结构
*参数说明：pPackage  数据结构指针
*          lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          nStackType 协议栈类型
*返回值：  0 表示解码成功，其他表示失败
*/
int PT_DecodePackage(void *pPackage, uchar *lpBuff, int nBuffLen, int nStackType);

/*
*函数名称：PT_EncodePackage
*函数说明：编码数据
*参数说明：lpBuff    数据缓存
*          nBuffLen  数据缓存长度
*          pMsgData  协议数据结构指针
*          bDevice   是否设备栈，设备栈二代协议头编码"BD"用1表示，PC栈编码"CC"用0表示
*返回值：  -1 表示失败，其他表示数据长度
*/
int PT_EncodePackage(uchar *lpBuff, int nBuffLen, 
                       void *pPackage, int nType, char bDevice);

#ifdef WIN32
/*
*函数名称：PT_GetPackage
*函数说明：根据消息类型获取相应的数据结构包,此函数仅适用于WIN32平台
*参数说明：nType 消息类型
*返回值：  NULL 表示无此类型数据结构包，其他表示数据结构包指针，结构数据包使用完后应使用free释放
*/
void* PT_GetPackage(int nType);

#endif // end WIN32 define

#endif //end  H_PT_DEFINE_H  define






















