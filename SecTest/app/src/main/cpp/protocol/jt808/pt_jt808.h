/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     pt_jt808.h
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
#ifndef H_PT_JT808_H
#define H_PT_JT808_H

#include "../pt_define.h"

//	//

#define BCD2BIN(a)	(((((a)>>4) & 0x0F)*10) + ((a) & 0x0F))

#define BIN2BCD(a)	((((a)/10)<<4) + ((a)%10))

#ifdef  PT_ENABLE_JT808
//////////////////////////////////////////////////////////////////////////
//interface define
//////////////////////////////////////////////////////////////////////////

#define JT808_ISTYPE(TYPE)   (TYPE >= JT808_ZERO && TYPE < JT808_LAST)
extern unsigned char JT808_MakeCRC(uchar *buf, int nLen);

extern  int JT808_Translate(uchar *buf, int nLen, uchar bEncode, int bufsize);

extern  int JT808_DecodeHead(const uchar* buf, int len);
extern  int JT808_EncodeHead(uchar *lpBuff, int nBuffLen, char bDevice, int nType);

extern  FUNC_ENCODER JT808_GetEncoder(int nType);
extern  FUNC_DECODER JT808_GetDecoder(int nType);

extern  int JT808_RecvData(TStackParam *pParam, uchar data);
extern  int JT808_RecvDataEx(TStackParam *pParam, uchar data);
extern  int JT808_EncodePackage(uchar *lpBuff, int nBuffLen, void *pPackage, 
                               int nMsgType, char bDevice);
extern  int JT808_DecodePackage(void *pPackage, int nMsgType, uchar *lpBuff, int nBuffLen);

#ifdef WIN32
    extern void* JT808_GetPackage(int nType);
#endif
//////////////////////////////////////////////////////////////////////////
//data struct define
//////////////////////////////////////////////////////////////////////////
typedef enum TJT808DEFINE
{  
    //常量定义
    JT808_DEF_MAX_NUM_PARAMETER    = 0x50, //终端参数设置最大值     20130121 modify by xlsong
    JT808_DEF_MAX_NUM_PHONEITEM    = 10, //电话本设置最大数
    JT808_DEF_MAX_NUM_USERPOSITION = 10, //位置上报最大用户数
    JT808_DEF_MAX_NUM_EVENT        = 10, //事件设置最大数
    JT808_DEF_MAX_NUM_KEY          = 10, //问题答案最大数
    JT808_DEF_MAX_NUM_SERVICE      = 10, //信息服务最大数
    JT808_DEF_MAX_NUM_REGION       = 24, //区域最大数
    JT808_DEF_MAX_NUM_POLYGON_POINT= 20, //多边形区域最大顶点数
    JT808_DEF_MAX_NUM_ROUTE_CORNER = 20, //路线最大拐点数
    JT808_DEF_MAX_NUM_INDEX_MEDIA  = 29, //多媒体数据检索最大数
    JT808_DEF_MAX_NUM_SENSOR_TYRE  = 32, //最大轮胎传感器数
    JT808_DEF_MAX_NUM_SENSOR_TEMP  = 2,  //最大温度传感器数
    JT808_DEF_MAX_NUM_SENSOR_OIL   = 2,  //最大油压传感器数
    JT808_DEF_MAX_NUM_SENSOR_POSI  = 2,  //最大位移传感器数
    JT808_DEF_MAX_NUM_MEDIAID      = 128,//最大上报媒体ID数
    //应答
    JT808_DEF_RESULT_SUCCESS      = 0,//成功/确认
    JT808_DEF_RESULT_FAIL         = 1,//失败
    JT808_DEF_RESULT_ERROR        = 2,//消息有误
    JT808_DEF_RESULT_NOTSUPPORT   = 3,//不支持
	JT808_DEF_RESULT_CONFIRM_ALARM= 4,//确认报警
    //协议类型
    JT808_DEF_PROTOCOL_GSM       = 0,//基于GSM的JT808协议
    JT808_DEF_PROTOCOL_BD        = 1,//基于北斗的JT808协议
    //终端参数类型
    JT808_DEF_PARAM_FIRST       = 0x0001,//参数起始
    JT808_DEF_PARAM_KEEPALIVE   = 0x0001,//终端心跳发送间隔，单位为秒S
    JT808_DEF_PARAM_TCP_TIMEOUT = 0x0002,//TCP消息应答超时时间，单位为秒S
    JT808_DEF_PARAM_TCP_RESEND  = 0x0003,//TCP消息重传参数
    JT808_DEF_PARAM_UDP_TIMEOUT = 0x0004,//UDP消息应答超时时间，单位为秒S
    JT808_DEF_PARAM_UDP_RESEND  = 0x0005,//UDP消息重传参数
    JT808_DEF_PARAM_SMS_TIMEOUT = 0x0006,//SMS消息应答超时时间，单位为秒S
    JT808_DEF_PARAM_SMS_RESEND  = 0x0007,//SMS消息重传参数
    JT808_DEF_PARAM_BD_TIMEOUT  = 0x0008,//北斗消息应答超时时间，单位为秒S
    JT808_DEF_PARAM_BD_RESEND   = 0x0009,//北斗消息重传参数
    JT808_DEF_PARAM_MASTER_APN  = 0x0010,//主服务器APN，无线通讯拨号访问点.
                                         //若网络制式为CDMA，则该处为PPP拨号号码
    JT808_DEF_PARAM_MASTER_USERNAME= 0x0011,//主服务器无线通讯拨号用户名
    JT808_DEF_PARAM_MASTER_PASSWORD= 0x0012,//主服务器无线通讯拨号密码
    JT808_DEF_PARAM_MASTER_ADDRESS = 0x0013,//主服务器IP地址或者域名
    JT808_DEF_PARAM_SLAVE_APN      = 0x0014,//备服务器APN，无线通讯拨号访问点.
                                             //若网络制式为CDMA，则该处为PPP拨号号码
    JT808_DEF_PARAM_SLAVE_USERNAME = 0x0015,//备服务器无线通讯拨号用户名
    JT808_DEF_PARAM_SLAVE_PASSWORD = 0x0016,//备服务器无线通讯拨号密码
    JT808_DEF_PARAM_SLAVE_ADDRESS  = 0x0017,//备服务器IP地址或者域名
    JT808_DEF_PARAM_TCP_PORT       = 0x0018,//服务器TCP端口
    JT808_DEF_PARAM_UDP_PORT       = 0x0019,//服务器UDP端口
	JT808_DEF_IC_MASTER_ADDRESS    = 0x1A,//道路运输证 IC 卡认证主服务器 IP 地址或域名------201301新协议   by xlsong
	JT808_DEF_IC_TCP_PORT          = 0x1B,//道路运输证 IC 卡认证主服务器 TCP 端口------201301新协议        by xlsong
	JT808_DEF_IC_UDP_PORT          = 0x1C,//道路运输证 IC 卡认证主服务器 UDP 端口------201301新协议		 by xlsong
	JT808_DEF_IC_SLAVE_ADDRESS    = 0x1D,//道路运输证 IC 卡认证备份服务器 IP 地址或域名，端口同主服务器------201301s新协议 by xlsong

    JT808_DEF_PARAM_MASTER_BDID    = 0x001E,//主服务器北斗用户机IC卡号码 
    JT808_DEF_PARAM_SLAVE_BDID     = 0x001F,//备服务器北斗用户机IC卡号码 
		
    JT808_DEF_PARAM_REPORT_STRATEGY    = 0x0020,//位置汇报策略。
                                            //0，定时汇报；1，定距汇报；2，定时和定距汇报
    JT808_DEF_PARAM_REPORT_MODE    = 0x0021,//位置汇报方案。0，根据Acc状态；
                                            //1，根据登录状态和Acc状态，先判断登录状态，若登录再根据Acc状态
    JT808_DEF_PARAM_INTERVAL_NODRIVER= 0x0022,//驾驶员未登录汇报时间间隔，单位为秒S，>0
    JT808_DEF_PARAM_INTERVAL_RESTING = 0x0027,//休眠时汇报时间间隔，单位为秒S，>0
    JT808_DEF_PARAM_INTERVAL_ALARM     = 0x0028,//紧急报警时汇报时间间隔，单位为秒S，>0 
    JT808_DEF_PARAM_INTERVAL_DEFAULT = 0x0029,//缺省时间汇报间隔，单位为秒S，>0
    JT808_DEF_PARAM_DISTANCE_DEFAULT = 0x002C,//缺省距离汇报间隔，单位为米m,>0
    JT808_DEF_PARAM_DISTANCE_NODRIVER= 0x002D,//驾驶员未登录汇报距离间隔。单位为米m，>0
    JT808_DEF_PARAM_DISTANCE_RESTING = 0x002E,//休眠时汇报距离间隔，单位为米m，>0 
    JT808_DEF_PARAM_DISTANCE_ALARM   = 0x002F,//紧急报警时汇报距离间隔，单位为米m，>0  
    JT808_DEF_PARAM_TURNINGANGLE     = 0x0030,//拐点补传角度，<180°
	JT808_DEF_PARAM_ELEC_FENCE_RADII = 0x0031,//电子围栏半径（非法位移阈值），单位为米 ---新协议
    JT808_DEF_PARAM_PHONE_PLATFORM = 0x0040,//监控平台电话号码
    JT808_DEF_PARAM_PHONE_RESET    = 0x0041,//复位电话号码。可用此号码电话拨打终端电话让终端复
    JT808_DEF_PARAM_PHONE_RESTORE  = 0x0042,//恢复出厂设置电话号码
    JT808_DEF_PARAM_PHONE_SMS      = 0x0043,//监控平台SMS电话号码
    JT808_DEF_PARAM_PHONE_SMS_SOS  = 0x0044,//接收终端SMS文本报警号码
    JT808_DEF_PARAM_ACCEPT_MODE    = 0x0045,//终端电话接听策略。0主动接听；
                                            //1 Acc ON时自动接听、OFF时手动接听
    JT808_DEF_PARAM_TALK_LIMIT_ONCE= 0x0046,//每次通话最长时间。单位为秒S。
                                            //0 为不允许通话，0xFFFFFFFF为不限制
    JT808_DEF_PARAM_TALK_LIMIT_MONTH= 0x0047,//当月最长通话时间。单位为秒S。
                                            //0 为不允许通话，0xFFFFFFFF为不限制
    JT808_DEF_PARAM_PHONE_MONITOR  = 0x0048,//监听电话号码
    JT808_DEF_PARAM_PHONE_PRIVATESMS= 0x0049,//监管平台特权短信号码
    JT808_DEF_PARAM_WARN_FLAG_IGNORE= 0x0050,//报警屏蔽字，与位置信息汇报消息中的报警标识
                                            //相对应。相应位为1则相应位报警被屏蔽
    JT808_DEF_PARAM_WARN_FLAG_SMS   = 0x0051,//报警发送文本SMS开关，与位置信息汇报消息中
                                            //的报警标志相对应，相应位为1则相应报警时发送文本SMS
    JT808_DEF_PARAM_WARN_FLAG_CAMERA= 0x0052,//报警拍摄开关，与位置信息汇报消息中的报警标志相对
                                            //应，相应位为1则相应报警时摄像头拍摄
    JT808_DEF_PARAM_WARN_FLAG_SAVE  = 0x0053,//报警拍摄存储标志，与位置信息汇报消息中的报警标志
                                               //相对应，相应位为1则相应报警时拍摄的照片进行存储，
                                               //否则实时上传
	JT808_DEF_PARAM_KEY_FLAG       = 0x0054,//关键标志，与位置信息汇报消息中的报警标志相对应，//20130121 add by xlsong
											//相应位为 1 则相应报警为关键报警
    JT808_DEF_PARAM_MAX_SPEED      = 0x0055,//最高速度，单位为千米/小时（Km/h）
    JT808_DEF_PARAM_MAX_OVERSPEED  = 0x0056,//超速持续时间，单位为秒S
    JT808_DEF_PARAM_MAX_DRIVING_ONCE= 0x0057,//连续驾驶时间门限，单位为秒S
    JT808_DEF_PARAM_MAX_DRIVING_DAY= 0x0058,//当天累计驾驶时间门限，单位为秒S
    JT808_DEF_PARAM_MIN_REST       = 0x0059,//最小休息时间，单位为秒S
    JT808_DEF_PARAM_MAX_REST       = 0x005A,//最长停车时间，单位为秒S
	JT808_DEF_PARAM_DIF_OVERSPEED  = 0x005B,//超速报警预警差值，单位为 1/10Km/h ---Ver201301
	JT808_DEF_PARAM_DIF_TIREDRIVING= 0x005C,//疲劳驾驶预警差值，单位为秒（s），>0 ---Ver201301

	JT808_DEF_PARAM_ALARM_HIT      = 0x005D,//碰撞报警参数设置： ---Ver201301
											//b7-b0： 碰撞时间，单位 4ms； 
											//b15-b8：碰撞加速度，单位 0.1g，设置范围在：0-79 之间，默认为10。
    JT808_DEF_PARAM_ALARM_TURNON    = 0x005E,//侧翻报警参数设置： ---Ver201301
											//侧翻角度，单位 1 度，默认为 30 度。
	JT808_DEF_PARAM_TIMING_CTRL     = 0x0064,//定时拍照控制， ---Ver201301
	JT808_DEF_PARAM_FIXEDDIS_CTRL   = 0x0065,//定距拍照控制		---Ver201301
	JT808_DEF_PARAM_MEDIA_QUALITY  = 0x0070,//图像/视频质量，1~10，1为最好
    JT808_DEF_PARAM_MEDIA_BRIGHTENESS= 0x0071,//亮度。0~255
    JT808_DEF_PARAM_MEDIA_CONTRAST  = 0x0072,//对比度。0~127 
    JT808_DEF_PARAM_MEDIA_SATURATION= 0x0073,//饱和度。0~127
    JT808_DEF_PARAM_MEDIA_CHROMA    = 0x0074,//色度。0~255 
    JT808_DEF_PARAM_MILEAGE         = 0x0080,//车辆里程表读数，1/10Km
    JT808_DEF_PARAM_PROVINCEID      = 0x0081,//车辆所在的省域ID
    JT808_DEF_PARAM_CITYID          = 0x0082,//车辆所在的市域ID 
    JT808_DEF_PARAM_LICENSEPLATE    = 0x0083,//车牌号码
    JT808_DEF_PARAM_COLOR           = 0x0084,//车牌颜色。按照JT/T415—2006中5.4.12的规定 
	JT808_DEF_PARAM_GNSS_POSMODE    =0x0090,//GNSS 定位模式，定义如下： ---Ver201301
											//bit0，0：禁用 GPS 定位， 1：启用 GPS 定位； 
											//bit1，0：禁用北斗定位， 1：启用北斗定位； 
											//bit2，0：禁用 GLONASS 定位， 1：启用 GLONASS 定位； 
											//bit3，0：禁用 Galileo 定位， 1：启用 Galileo 定位。
	JT808_DEF_PARAM_GNSS_BOUTERAYE    = 0x0091,//GNSS 波特率   ---Ver201301
	JT808_DEF_PARAM_GNSS_OUTOUT_HZ    = 0x0092,//GNSS 模块详细定位数据输出频率  ---Ver201301
	JT808_DEF_PARAM_GNSS_COLLECT_HZ   = 0x0093,//GNSS 模块详细定位数据采集频率  ---Ver201301
	JT808_DEF_PARAM_GNSS_UPLOAD_MODE  = 0x0094,//GNSS 模块详细定位数据上传方式  ---Ver201301
	JT808_DEF_PARAM_GNSS_UPLOAD_SET   = 0x0095,//GNSS 模块详细定位数据上传设置  ---Ver201301
	JT808_DEF_PARAM_CAN_CHL1_COLLECT  = 0x0100,//CAN 总线通道 1 采集时间间隔(ms)  ---Ver201301
	JT808_DEF_PARAM_CAN_CHL1_UPLOAD   = 0x0101,//CAN 总线通道 1 上传时间间隔(ms) ---Ver201301
	JT808_DEF_PARAM_CAN_CHL2_COLLECT  = 0x0102,//CAN 总线通道 2 采集时间间隔(ms) ---Ver201301
	JT808_DEF_PARAM_CAN_CHL2_UPLOAD   = 0x0103,//CAN 总线通道 2 上传时间间隔(ms) ---Ver201301
	JT808_DEF_PARAM_CAN_ID_SET_FIRST  = 0x0110,//CAN 总线 ID 单独采集设置 ---Ver201301
    JT808_DEF_PARAM_CAN_ID_SET_LAST   = 0x01FF,//CAN 总线 ID 单独采集设置 ---Ver201301

    JT808_DEF_PARAM_POSI_STANDARD     = 0xF011,//位移报警标准
	JT808_DEF_PARAM_TEMP_MIN		  = 0xF012,	//温度报警下限
	JT808_DEF_PARAM_TEMP_MAX		  = 0xF013,	//温度报警上限
	JT808_DEF_PARAM_TYRE_STANDARD     = 0xF014,	//胎压标准

    JT808_DEF_PARAM_CAMERA_NUMBER     = 0xF100,//摄像头安装数
    JT808_DEF_PARAM_OILBOX_LENGTH     = 0xF101,//油箱长度
    JT808_DEF_PARAM_OILBOX_WIDTH      = 0xF102,//油箱宽度
    JT808_DEF_PARAM_OILBOX_HEIGHT     = 0xF103,//油箱高度
    JT808_DEF_PARAM_OILSENSOR_HEIGHT  = 0xF104,//油压传感器高度
    JT808_DEF_PARAM_OILSENSOR_WARNVALUE  = 0xF105,//油压异常变化阀值

    JT808_DEF_PARAM_TYRE_TYPE         = 0xF111,//胎压传感器类型 ， 0:无；1:伟盈;2:维克朗宁旧;3:维克朗宁新
    JT808_DEF_PARAM_TYRE_ID_FIRST     = 0xF112,//左边最前边轮胎压传感器ID， 0：表示未安装；其他：传感器ID
    JT808_DEF_PARAM_TYRE_ID_LAST      = JT808_DEF_PARAM_TYRE_ID_FIRST + JT808_DEF_MAX_NUM_SENSOR_TYRE,//右边最后轮胎压传感器ID， 0：表示未安装；其他：传感器ID
    JT808_DEF_PARAM_LAST              = JT808_DEF_PARAM_TYRE_ID_LAST,	//参数结束

	
	//GNSS模块属性
	JT808_DEF_GPS_SERVICE = 1<<0,//支持 GPS 定位
	JT808_DEF_BD_SERVICE  = 1<<1,//支持 BD 定位
	JT808_DEF_GLONASS_SERVICE = 1<<2,//支持 GLONASS 定位
	JT808_DEF_GALILEO_SERVICE = 1<<3,//支持 Galileo 定位

	//通信模块属性
	JT808_DEF_GPRS_SERVICE     = 1<<0,//支持GPRS通信
	JT808_DEF_CDMA_SERVICE     = 1<<1,//支持CDMA通信
	JT808_DEF_TDSCDMA_SERVICE  = 1<<2,//支持TD-SCDMA通信
	JT808_DEF_WCDMA_SERVICE    = 1<<3,//支持WCDMA通信
	JT808_DEF_CDMA2000_SERVICE = 1<<4,//支持CDMA2000通信
	JT808_DEF_TD_LTE_SERVICE   = 1<<5,//支持TD-LTE通信
	JT808_DEF_OHTER_SERVICE    = 1<<7,//支持其他通信方式

	//终端控制指令
    JT808_DEF_TERMINAL_UPDATE     = 1,//无线升级  TUpdateInfo
    JT808_DEF_TERMINAL_REDIRECT   = 2,//控制终端连接指定服务器  TPlatform
    JT808_DEF_TERMINAL_SHUTDOWN   = 3,//终端关机
    JT808_DEF_TERMINAL_RESET      = 4,//终端复位
    JT808_DEF_TERMINAL_RESTORE    = 5,//终端恢复出厂设置
    JT808_DEF_TERMINAL_DISCONNECT = 6,//关闭数据通讯
    JT808_DEF_TERMINAL_MUTE       = 7,//关闭所有无线通讯
    //位置信息定义
    JT808_DEF_STATE_ACC           = 1 << 0,  //ACC ，0：关，1：开
    JT808_DEF_STATE_POSITION      = 1 << 1,  //定位，0：未定位，1：已定位
    JT808_DEF_STATE_SOUTH         = 1 << 2,  //南纬, 0：北纬，1：南纬
    JT808_DEF_STATE_WEST          = 1 << 3,  //西经，0：东经，1：西经
    JT808_DEF_STATE_WORKING       = 1 << 4,  //运营状态， 0：停运；1：运营
    JT808_DEF_STATE_ENCRYPT       = 1 << 5,  //经纬度加密，0:未加密；1:已加密
	JT808_DEF_STATE_CARRY_EMPTY   = 0, //载货，空车
	JT808_DEF_STATE_CARRY_HALF    = 1 << 8, //载货，半载
	JT808_DEF_STATE_CARRY_RESERVED= 2 << 8, //载货，保留
	JT808_DEF_STATE_CARRY_FULL    = 3 << 8, //载货，满载
    JT808_DEF_STATE_OIL           = 1 << 10, //油路,0：关，1：开
    JT808_DEF_STATE_POWER         = 1 << 11, //电路,0：关，1：开
    JT808_DEF_STATE_LOCK          = 1 << 12, //0：车门解锁；1：车门加锁
	JT808_DEF_STATE_DOOR1         = 1 << 13, //0：门 1 关；1：门 1 开（前门）//新协议
	JT808_DEF_STATE_DOOR2         = 1 << 14, //0：门 2 关；1：门 2 开（中门）//新协议 
	JT808_DEF_STATE_DOOR3         = 1 << 15, //0：门 3 关；1：门 3 开（后门）//新协议
	JT808_DEF_STATE_DOOR4         = 1 << 16, //0：门 4 关；1：门 4 开（驾驶席门）//新协议
	JT808_DEF_STATE_DOOR5         = 1 << 17, //0：门 5 关；1：门 5 开（自定义）  //新协议
	JT808_DEF_STATE_GPS           = 1 << 18, //0：未使用 GPS 卫星进行定位；1：使用 GPS 卫星进行定位//新协议  
	JT808_DEF_STATE_BD            = 1 << 19, //0：未使用北斗卫星进行定位；1：使用北斗卫星进行定位//新协议
	JT808_DEF_STATE_GLONASS       = 1 << 20, //0：未使用 GLONASS 卫星进行定位；1：使用 GLONASS 卫星进行定位 //新协议
	JT808_DEF_STATE_GALILEO       = 1 << 21, //0：未使用 Galileo 卫星进行定位；1：使用 Galileo 卫星进行定位  //新协议

    JT808_DEF_WARN_SOS        = 1 << 0, //紧急报警，出动报警开关后触发，收到应答后清零
    JT808_DEF_WARN_OVERSPEED  = 1 << 1, //超速报警，标志位维持至报警条件解除
    JT808_DEF_WARN_TIRED      = 1 << 2, //疲劳驾驶，标志位维持至报警条件解除
    JT808_DEF_WARN_URGEN      = 1 << 3, //危险预警，标志位维持至报警条件解除
    JT808_DEF_WARN_GNSS_1     = 1 << 4, //GNSS模块发生故障，标志位维持至报警条件解除
    JT808_DEF_WARN_GNSS_2     = 1 << 5, //GNSS天线未接或被剪断，标志位维持至报警条件解除
    JT808_DEF_WARN_GNSS_3     = 1 << 6, //GNSS天线短路，标志位维持至报警条件解除
    JT808_DEF_WARN_POWER_LACK = 1 << 7, //终端主电源欠压，标志位维持至报警条件解除
    JT808_DEF_WARN_POWER_OFF  = 1 << 8, //终端主电源掉电，标志位维持至报警条件解除
    JT808_DEF_WARN_LCD        = 1 << 9, //终端LCD或者显示器故障，标志位维持至报警条件解除
    JT808_DEF_WARN_TTS        = 1 << 10, //TTS模块故障，标志位维持至报警条件解除
    JT808_DEF_WARN_CAMERA     = 1 << 11, //摄像头故障，标志位维持至报警条件解除
	JT808_DEF_WARN_IC_MODULE  = 1 << 12, //1：道路运输证 IC 卡模块故障
	JT808_DEF_WARN_EARLY_OVERSPEED = 1 << 13, //1：超速预警 
	JT808_DEF_WARN_EARLY_TIRED     = 1 << 14, //1：疲劳驾驶预警
    JT808_DEF_WARN_GSM             = 1 << 15, //GSM模块故障，标志位维持至报警条件解除
    JT808_DEF_WARN_BD              = 1 << 16, //北斗模块故障，标志位维持至报警条件解除
    JT808_DEF_WARN_OVERTIME_DRIVE  = 1 << 18, //当天累计驾驶超时，标志位维持至报警条件解除
    JT808_DEF_WARN_OVERTIME_PARK   = 1 << 19, //超时停车，标志位维持至报警条件解除
    JT808_DEF_WARN_AREA       = 1 << 20, //进出区域，收到应答后清零
    JT808_DEF_WARN_ROUTE      = 1 << 21, //进出路线，收到应答后清零
    JT808_DEF_WARN_DRIVING_TIME = 1 << 22, //路段行驶时间不足/过长，收到应答后清零
    JT808_DEF_WARN_ROUTE_OFFSET = 1 << 23, //路线偏离报警，标志位维持至报警条件解除
    JT808_DEF_WARN_VSS        = 1 << 24, //车辆Vss故障，标志位维持至报警条件解除
    JT808_DEF_WARN_OIL        = 1 << 25, //车辆油量异常，标志位维持至报警条件解除
    JT808_DEF_WARN_THEFT      = 1 << 26, //车辆被盗，标志位维持至报警条件解除
    JT808_DEF_WARN_LIGHT      = 1 << 27, //车辆非法点火，收到应答后清零
    JT808_DEF_WARN_MOVE       = 1 << 28, //车辆非法移位，收到应答后清零
    JT808_DEF_WARN_HIT        = 1 << 29, //碰撞预警 ，标志位维持至报警条件解除 //新协议
	JT808_DEF_WARN_TURNON     = 1 << 30, //侧翻预警 标志维持至报警条件解除   //新协议
	JT808_DEF_WARN_OPENDOOR   = 1 << 31, //非法开门报警（终端未设置区域时，不判断非法开门） //新协议
	
	//扩展信号状态位
	JT808_DEF_SIGNAL_DIPPED_LIGHT    = 1<<0,//近光灯信号
	JT808_DEF_SIGNAL_HIGH_BEAM       = 1<<1,//远光灯信号
	JT808_DEF_SIGNAL_RIGHT_TURNSIGNAL= 1<<2,//右转向灯信号
	JT808_DEF_SIGNAL_LEFT_TURNSIGNAL = 1<<3,//左转向灯信号
	JT808_DEF_SIGNAL_BRAKING         = 1<<4,//制动信号
	JT808_DEF_SIGNAL_REVERSE         = 1<<5,//倒档信号
	JT808_DEF_SIGNAL_FOGLIGHT        = 1<<6,//雾灯信号
	JT808_DEF_SIGNAL_CLEARRANCE      = 1<<7,//示廓灯
	JT808_DEF_SIGNAL_HORN            = 1<<8,//喇叭信号
	JT808_DEF_SIGNAL_AIRCONDITION    = 1<<9,//空调状态
	JT808_DEF_SIGNAL_NEUTRAL         = 1<<10,//空挡信号 
	JT808_DEF_SIGNAL_RETARDER        = 1<<11,//缓速器工作
	JT808_DEF_SIGNAL_ABS             = 1<<12,//ABS 工作
	JT808_DEF_SIGNAL_HEATER          = 1<<13,//加热器工作
	JT808_DEF_SIGNAL_CLUTCH          = 1<<14,//离合器状态
	//IO状态
	JT808_DEF_IO_DEEP_SLEEP = 1<<0,//深度休眠状态
	JT808_DEF_IO_SLEEP      = 1<<1,//休眠状态
	//附加信息
    JT808_DEF_INFO_MILEAGE    = 1 << 1,    //里程
    JT808_DEF_INFO_OIL        = 1 << 2,    //油量
    JT808_DEF_INFO_SPEED      = 1 << 3,    //行驶记录仪获取的速度
    JT808_DEF_INFO_OVERSPEED  = 1 << 4,    //超速报警附加信息
    JT808_DEF_INFO_AREA_ROUTE = 1 << 5,    //进出区域/路线报警附加信息
    JT808_DEF_INFO_DRIVING_TIME= 1 << 6,   //路段行驶时间不足/过长报警附加信息
	JT808_DEF_INFO_EVENTID     = 1<<8,//需要人工确认报警事件的 ID
	JT808_DEF_INFO_CARSIGNAL   = 1<<9,//扩展车辆信号状态位
	JT808_DEF_INFO_IOSTATUS    = 1<<10,//IO状态位
	JT808_DEF_INFO_AD          = 1<<11,//模拟量
	JT808_DEF_INFO_WLSIGNAL    = 1<<12,//无线通信网络信号强度
	JT808_DEF_INFO_GNSSSTARS   = 1<<13,//GNSS 定位卫星数
	JT808_DEF_INFO_ADDDATALEN  = 1<<14,//后续自定义信息长度
    //自定义附加信息
    JT808_DEF_INFO1_OIL        = 1 << 0,    //油量
    JT808_DEF_INFO1_TYRE       = 1 << 1,    //胎压
    JT808_DEF_INFO1_TEMP       = 1 << 2,    //温度
    JT808_DEF_INFO1_POSI       = 1 << 3,    //位移
    JT808_DEF_INFO1_CARRY      = 1 << 4,    //载货状态
    JT808_DEF_INFO1_SERVERFLAG = 1 << 5,    //服务器标识信息,自定义附加信息

    //文本下发标志
    JT808_DEF_TEXT_URGENCY     = 1 << 0,//紧急
    JT808_DEF_TEXT_LED         = 1 << 2,//终端显示器显示
    JT808_DEF_TEXT_TTS         = 1 << 3,//终端TTS报读
    JT808_DEF_TEXT_AD          = 1 << 4,//广告屏显示
    JT808_DEF_TEXT_CAN         = 1 << 5,//CNA故障信息

    //事件设置类型
    JT808_DEF_EVENT_REMOVEALL  = 0,//删除终端现有所有事件，该命令后不带后继字节
    JT808_DEF_EVENT_UPDATE     = 1,//更新事件
    JT808_DEF_EVENT_APPEND     = 2,//追加事件
    JT808_DEF_EVENT_MODIFY     = 3,//修改事件
    JT808_DEF_EVENT_REMOVE     = 4,// 删除特定几项事件,之后事件项中无需带事件内容

    //点播设置类型
    JT808_DEF_SERVICE_REMOVEALL  = 0,//删除终端现有所有事件，该命令后不带后继字节
    JT808_DEF_SERVICE_UPDATE     = 1,//更新菜单
    JT808_DEF_SERVICE_APPEND     = 2,//追加菜单
    JT808_DEF_SERVICE_MODIFY     = 3,//修改菜单

    //车辆控制指令
    JT808_DEF_CAR_CTRL_LOCK      = 1 << 0, //0 车门解锁；1 车门加锁
    JT808_DEF_CAR_CTRL_YD        = 1 << 7, //0 恢复油电；1 切断油电，自定义

    //电话本设置类型
    JT808_DEF_PHONEBOOK_REMOVEALL  = 0,//删除终端上所有存储的联系人，该命令后不带后继字节
    JT808_DEF_PHONEBOOK_UPDATE     = 1,//表示更新电话本(删除终端中已有的全部联系人并追加消息体中的联系人)
    JT808_DEF_PHONEBOOK_APPEND     = 2,//追加电话本
    JT808_DEF_PHONEBOOK_MODIFY     = 3,//修改电话本

    //区域设置类型
    JT808_DEF_REGION_UPDATE      = 0,//更新区域
    JT808_DEF_REGION_APPEND      = 1,//追加区域
    JT808_DEF_REGION_MODIFY      = 2,//修改区域
    //区域属性
    JT808_DEF_REGION_LIMIT_TIME    = 1 << 0,//根据时间
    JT808_DEF_REGION_LIMIT_SPEED   = 1 << 1,//限速
    JT808_DEF_REGION_ENTER_DRIVER  = 1 << 2,//进区域报警给驾驶员
    JT808_DEF_REGION_ENTER_PLATFORM= 1 << 3,//进区域报警给平台
    JT808_DEF_REGION_LEAVE_DRIVER  = 1 << 4,//出区域报警给驾驶员
    JT808_DEF_REGION_LEAVE_PLATFORM= 1 << 5,//出区域报警给平台
    JT808_DEF_REGION_SOUTH_LAT     = 1 << 6,//0 北纬；1 南纬
    JT808_DEF_REGION_WEST_LONG     = 1 << 7,//0 东经；1 西经
	JT808_DEF_REGION_OPENDOOR_SET  = 1 << 8,//0：允许开门；1：禁止开门
	JT808_DEF_REGION_ENTER_COMM    = 1 << 14,//0：进区域开启通信模块；1：进区域关闭通信模块
	JT808_DEF_REGION_ENTER_GNSSPOS = 1 << 15,//0：进区域不采集 GNSS 详细定位数据；1：进区域采集 GNSS 详细定位数据

    JT808_DEF_ROAD_LIMIT_TIME    = 1 << 0,//根据时间
    JT808_DEF_ROAD_LIMIT_SPEED   = 1 << 1,//限速
    JT808_DEF_ROAD_SOUTH_LAT     = 1 << 2,//进区域报警给驾驶员
    JT808_DEF_ROAD_WEST_LONG     = 1 << 3,//进区域报警给平台

	//IC 卡读取结果
	JT808_DEF_READIC_SECCESS      = 0x00,//IC 卡读卡成功    Ver201301
	JT808_DEF_READIC_SECRETKEY_ERR= 0x01,//读卡失败，原因为卡片密钥认证未通过
	JT808_DEF_READIC_CARD_LOCKED  = 0x02,//读卡失败，原因为卡片已被锁定；
	JT808_DEF_READIC_CARD_DISPEARD= 0x03,//读卡失败，原因为卡片被拔出
	JT808_DEF_READIC_CRC_ERR      = 0x04,//读卡失败，原因为数据校验错误
	
	//位置数据类型
	JT808_DEF_REPORT_NORMAL = 0,//正常位置批量汇报
	JT808_DEF_REPORT_REPLACE = 1,//盲区补报
	
	//CAN 标志
	JT808_DEF_CAN_AVERAGE = 1<<0,//数据采集方式 0：原始数据，1：采集区间的平均值；
	JT808_DEF_CAN_EXT_FRAME = 1<<1,//帧类型 0：标准帧，1：扩展帧；


     //多媒体数据类型定义
    JT808_DEF_MEDIA_TYPE_PICTURE = 0,//媒体类型,图像
    JT808_DEF_MEDIA_TYPE_AUDIO   = 1,//媒体类型,音频
    JT808_DEF_MEDIA_TYPE_VIDEO   = 2,//媒体类型,视频
   
    JT808_DEF_MEDIA_CODE_JPEG    = 0,//媒体编码
    JT808_DEF_MEDIA_CODE_TIF     = 1,//媒体编码
    JT808_DEF_MEDIA_CODE_MP3     = 2,//媒体编码
    JT808_DEF_MEDIA_CODE_WAV     = 3,//媒体编码
    JT808_DEF_MEDIA_CODE_WMV     = 4,//媒体编码
    
    
    JT808_DEF_PHOTO_SIZE_320_240 = 1,//分辨率
    JT808_DEF_PHOTO_SIZE_640_480 = 2,//分辨率
    JT808_DEF_PHOTO_SIZE_800_600 = 3,//分辨率
    JT808_DEF_PHOTO_SIZE_1280_768 = 4,//分辨率
    JT808_DEF_PHOTO_SIZE_176_144 = 5,//分辨率
    JT808_DEF_PHOTO_SIZE_352_288 = 6,//分辨率
    JT808_DEF_PHOTO_SIZE_704_288 = 7,//分辨率
    JT808_DEF_PHOTO_SIZE_704_576 = 8,//分辨率

    JT808_DEF_MEDIA_EVENT_PLATFORM = 0,//事件项编码,平台下发指令
    JT808_DEF_MEDIA_EVENT_TERMLY = 1,//事件项编码,定时
    JT808_DEF_MEDIA_EVENT_HEIST  = 2,//事件项编码,抢劫报警
    JT808_DEF_MEDIA_EVENT_HIT    = 3,//事件项编码,碰撞侧翻
    JT808_DEF_MEDIA_EVENT_DOOR_OPEN= 4,//事件项编码,开门
    JT808_DEF_MEDIA_EVENT_DOOR_CLOSE= 5,//事件项编码,关门
    JT808_DEF_MEDIA_EVENT_DOOR_SPEED= 6,//事件项编码,车门由开变关，时速从＜20 公里到超过 20 公里
    JT808_DEF_MEDIA_EVENT_DISTANCE= 7,//事件项编码,定距
    JT808_DEF_MEDIA_EVENT_OTHER  = 8,//事件项编码,其他
    
		
	JT808_DEF_OIL_STATE_NORMAL   = 0,//油量正常
    JT808_DEF_OIL_STATE_LACK	 = 1,//油量不足
    JT808_DEF_OIL_STATE_EXCEPTION= 2,//油量异常
		
    JT808_DEF_TPRE_STATE_SIGNAL_NORMAL  = 0x80,  //轮胎信号正常
    JT808_DEF_TPRE_STATE_SIGNAL_LOST    = 0x81,   //轮胎信号丢失
    JT808_DEF_TPRE_STATE_PRESSURE_NORMAL= 0x00,   //轮胎压力正常
    JT808_DEF_TPRE_STATE_PRESSURE_LOW   = 0x02,   //轮胎低压
    JT808_DEF_TPRE_STATE_PRESSURE_HIGH  = 0x04,   //轮胎高压
    JT808_DEF_TPRE_STATE_TEMP_NORMAL    = 0x00,   //轮胎温度正常
    JT808_DEF_TPRE_STATE_TEMP_LOW       = 0x08,   //轮胎低温
    JT808_DEF_TPRE_STATE_TEMP_HIGH      = 0x10,   //轮胎高温
    JT808_DEF_TPRE_STATE_AIRLEAK_NORMAL = 0x00,   //轮胎不漏气正常
    JT808_DEF_TPRE_STATE_AIRLEAK_LOW    = 0x20,   //轮胎慢速漏气
    JT808_DEF_TPRE_STATE_AIRLEAK_HIGH   = 0x40,   //轮胎快速漏气

	JT808_DEF_TEMP_STATE_NORMAL   = 0,//温度正常
    JT808_DEF_TEMP_STATE_HIGH	  = 1,//温度过高
    JT808_DEF_TEMP_STATE_LOW	  = 2,//温度过低
    
	//数据透传类型  ---Ver201301
	JT808_DEF_DATA_GNSS = 0x00,//GNSS 模块详细定位数据
	JT808_DEF_DATA_IC   = 0x0B,//道路运输证 IC 卡信息上传消息为 64Byte，下传消息为
							   //24Byte。道路运输证 IC 卡认证透传超时时间为 30s。超时后，不重发
	JT808_DEF_DATA_COM1 = 0x41,//串口 1 透传消息 
	JT808_DEF_DATA_COM2 = 0x42,//串口 2 透传消息 
										 //0xF0-0xFF  用户自定义透传消息
}TJT808DEFINE;


typedef struct TJT808Head
{
    unsigned char  nProtocol;//协议类型 0：GSM， 1：北斗
    unsigned short nID;  //消息ID
    unsigned char  bMultiPacket;//是否多包
    unsigned char  nEncryptType;//加密类型
    unsigned char  szSystemID[12];//12位系统编号
    unsigned short nSeqNum;//流水号
    unsigned short nPacketCount;//多包总数
    unsigned short nPacketSeq;//多包序号
}TJT808Head;

//通用请求
typedef struct TJT808COMMONREQ
{
    TJT808Head head;
    //nodata
}TJT808COMMONREQ;

//通用应答
typedef struct TJT808COMMONRES
{
    TJT808Head head;

    unsigned short nSeqNum; //sequence number
    unsigned short nResID;//Response ID
    unsigned char  nResult;//结果0,成功/确认；1,失败；2,消息有误；3,不支持
} TJT808COMMONRES;

//平台通用应答
typedef TJT808COMMONRES TJT808PlatformRes;

//终端通用应答
typedef TJT808COMMONRES TJT808TerminalRes;

//终端心跳
typedef TJT808COMMONREQ TJT808KeepAlive;

//终端注册
typedef struct TJT808REGISTERREQ
{
    TJT808Head head;

    unsigned short nProvID;//省域ID 
    unsigned short nCityID;//市县域ID 
    unsigned char  szManID[6] ;//制造商ID
#ifdef PT_JT808_VER_201301
	unsigned char  szModel[20] ;//终端型号  Ver201301   xlsong
#else
    unsigned char  szModel[8] ;//终端型号
#endif
    unsigned char  szTerminalID[8] ;//终端ID
    unsigned char  nColor ;//车牌颜色
    unsigned char  szLicensePlate[32];//车牌
}TJT808REGISTERREQ;

//终端注册应答
typedef struct TJT808REGISTERRES
{
    TJT808Head head;

    unsigned short nSeqNum;// 应答流水号
    unsigned char nResult;//结果
    unsigned char szSystemID[13];//入网编号
    unsigned char szAuthcode[32];//鉴权码
}TJT808REGISTERRES;

//终端注消
typedef TJT808COMMONREQ TJT808Deregister;

//终端鉴权
typedef struct TJT808LOGIN
{
    TJT808Head head;

    unsigned char szAuthcode[32];//鉴权码
}TJT808LOGIN;

//终端参数定义
typedef struct TJT808CanParam
{
    unsigned int  nInterval;   //表示此 ID 采集时间间隔(ms)，0 表示不采集
    unsigned char nChannel;   //表示 CAN 通道号，0：CAN1，1：CAN2
    unsigned char nFrameType; //表示帧类型，0：标准帧，1：扩展帧
    unsigned char nMode;      //表示数据采集方式，0：原始数据，1：采集区间的计算值
    unsigned int  nCanID;     //表示 CAN 总线 ID
    unsigned char bValid;
}TJT808CanParam;
        
typedef  struct TJT808ParameterItem
{
    unsigned int  nType;    //参数类型
    //参数值
    union
    {
        unsigned int  nValue;       //数值参数
        unsigned char szValue[32];  //字符串参数
        TJT808CanParam canValue;
    }param;                         
}TJT808ParameterItem;
//设置终端参数
typedef struct TJT808ParameterSet
{
    TJT808Head head;

    unsigned char nCount;//参数个数
    TJT808ParameterItem params[JT808_DEF_MAX_NUM_PARAMETER];//参数项列表
}TJT808ParameterSet;

//查询终端参数
typedef struct TJT808ParameterQueryReq
{
	TJT808Head head;
	unsigned int ncount;//参数项的个数
	unsigned int nType[JT808_DEF_MAX_NUM_PARAMETER];//参数项列表
}TJT808ParameterQueryReq;

//typedef TJT808COMMONREQ TJT808ParameterQueryReq; 
//查询终端参数应答
typedef struct TJT808ParameterQueryRes
{
    TJT808Head head;

    unsigned int  nSeqNum;//应答流水号
    unsigned char nCount; //应答参数个数
    TJT808ParameterItem params[JT808_DEF_MAX_NUM_PARAMETER];//参数项列表
}TJT808ParameterQueryRes;

//终端连接信息定义
typedef struct TJT808ConnInfo
{
    unsigned char  szAPN[32];//拨号点名称
    unsigned char  szUser[32];//拨号用户名
    unsigned char  szPassword[32];//拨号密码
    unsigned char  szAddress[64];//服务器地址，IP或者域名
    unsigned short nTCPPort;//服务器TCP端口
    unsigned short nUDPPort;//服务器UDP端口
    unsigned int   nBDID;//用户机IC卡号
}TJT808ConnInfo;
//终端控制
typedef struct TJT808TerminalCtrl
{
    TJT808Head head;
    unsigned char nCMD;//命令字
                       //1:无线升级  TUpdateInfo
                       //2:控制终端连接指定服务器  TPlatform
                       //3:终端关机
                       //4:终端复位
                       //5:终端恢复出厂设置
                       //6:关闭数据通讯
                       //7:关闭所有无线通讯
    union
    {
        struct TUpdateInfo
        {
            unsigned char szURL[64];//URL 地址；

            TJT808ConnInfo conninfo;

            unsigned char  ManID[6];//制造商ID
            unsigned char  szHardwareVersion[16];//硬件版本
            unsigned char  szSoftwareVersion[16];//软件版本
            unsigned short nTimeLimit;//连接到指定服务器时限
        }updateinfo;
        
        struct TPlatform
        {
            unsigned char  nCtrl;//连接控制,
                                //0:切换到指定监管平台服务器,
                                //1:切换回原缺省监管平台服务器，并恢复正常状态
            unsigned char  szAuthcode[32];//鉴权码

            TJT808ConnInfo conninfo;

            unsigned short nTimeLimit;//连接到指定服务器时限
        }platform;
    }cmdinfo;
    
}TJT808TerminalCtrl;

typedef struct TJT808DateTime
{
    unsigned short nYear;//年
    unsigned char  nMonth;//月
    unsigned char  nDay;//日
    unsigned char  nHour;//时
    unsigned char  nMinute;//分
    unsigned char  nSecond;//秒
}TJT808DateTime;

typedef struct JT808TyreSensor//胎压传感器
{
    unsigned char nIndex;//轮位，1:左1,2：右1，其他：以此类推
    short nPressure;//kpa
    short nTemp;    //摄氏度°
    unsigned char nState;
}JT808TyreSensor;

typedef struct TJT808PosiState
{
    //位置基本信息
    unsigned int nWarnFlag;//告警标志
    unsigned int nState;//状态
    double dblLatitude;//纬度;
    double dblLongitude;//经度
    unsigned short nHigh;//海拔高度 m
    double dblSpeed;//速度 Km/h 
    unsigned short nDeriction;//方向

    unsigned short nYear;//年
    unsigned char  nMonth;//月
    unsigned char  nDay;//日
    unsigned char  nHour;//时
    unsigned char  nMinute;//分
    unsigned char  nSecond;//秒
}TJT808PosiState;

typedef struct TJT808PositionInfo
{
    //位置基本信息
    unsigned int nWarnFlag;//告警标志
    unsigned int nState;//状态
    double dblLatitude;//纬度;
    double dblLongitude;//经度
    unsigned short nHigh;//海拔高度 m
    double dblSpeed;//速度 Km/h 
    unsigned short nDeriction;//方向

    unsigned short nYear;//年
    unsigned char  nMonth;//月
    unsigned char  nDay;//日
    unsigned char  nHour;//时
    unsigned char  nMinute;//分
    unsigned char  nSecond;//秒
    //附加信息
    struct 
    {
		unsigned int  nFlag;//0:附加信息；其他：有附加信息
        unsigned int   nMileage;//里程0x01 1/10km
        unsigned short nOil;    //油量0x02 1/10L
        unsigned short nSpeed;  //记录仪速度 0x03 1/10km/h
		unsigned short nEventID;//0x04需要人工确认报警事件的ID，WORD，从 1 开始计数 ---Ver201301
        //0x11 超速报警附加信息
        struct TOverspeedInfo
        {
            unsigned char nPositionType;//位置类型
            unsigned int nAreaID;//区域ID
        } overspeedinfo;

        //0x12 进出区域/路线报警附加信息
        struct TAreaInfo
        {
            unsigned char nPositionType;//位置类型
            unsigned int nAreaID;//区域ID
            unsigned char nDeriction;//方向 0:进；1:出
        } areainfo;

        //0x13 路段行驶时间不足/过长报警附加信息
        struct TDrivingInfo
        {
            unsigned int   nAreaID;//区域
            unsigned short nTime;//路段行驶时间
            unsigned char  nResult;//结果 0:不足；1:过长
        } drivinginfo;
		
		unsigned int   nCarSignalStatus; //0x25 扩展车辆信号状态位
		unsigned short nIoStatus;//0x2A IO状态位
		unsigned int   nAnalogData;//0x2B 模拟量，bit0-15，AD0；bit16-31，AD1
		unsigned char  nWLSignal;//0x30 无线通信网络信号强度
		unsigned char  nGnssStars;//0x31 GNSS 定位卫星数
		unsigned int   nAddDataLen;//后续自定义信息长度
    }addininfo;
    
    //自定义信息
    struct
    {
        unsigned int   nFlag;//0:附加信息；其他：有附加信息

        unsigned char  nOilCount;//油量传感器数
        unsigned short oils[JT808_DEF_MAX_NUM_SENSOR_OIL];//油量 %
        unsigned char  oilsstate[JT808_DEF_MAX_NUM_SENSOR_OIL];//油量状态

        unsigned char  nTempCount;//温度传感器数
        unsigned short temps[JT808_DEF_MAX_NUM_SENSOR_TEMP];//摄氏度°
        unsigned char  tempsstate[JT808_DEF_MAX_NUM_SENSOR_TEMP];//温度状态

        unsigned char  nTyreCount;//胎压传感器数
        JT808TyreSensor tyres[JT808_DEF_MAX_NUM_SENSOR_TYRE];

        unsigned char  nPosiCount;//位移传感器数
        unsigned short positions[JT808_DEF_MAX_NUM_SENSOR_POSI];//位移
        unsigned char  posistate[JT808_DEF_MAX_NUM_SENSOR_POSI];//位移状态

        unsigned char  nCarryState;		//载货状态 

        unsigned int   nServerFlag;//0xFF服务器标识信息 自定义附加信息
    }userdef;
}TJT808PositionInfo;
//位置信息汇报
typedef struct TJT808PositionReport
{
    TJT808Head head;
    TJT808PositionInfo position;
}TJT808PositionReport;

//位置信息查询
typedef TJT808COMMONREQ TJT808PositionQueryReq;

//位置信息查询应答
typedef struct TJT808PositionQueryRes
{
    TJT808Head head;

    unsigned short nSeqNum;// 应答流水号

    TJT808PositionInfo position;
}TJT808PositionQueryRes;

//临时位置跟踪
typedef struct TJT808PositionTrack
{
    TJT808Head head;

    unsigned short nInterval;//时间间隔,unit:s, 0:停止跟踪,无后续字段
    unsigned int   nTimeLimit;//位置跟踪有效期,unit:s,
                              //终端在接收到位置跟踪控制消息后，
                              //在有效期截止时间之前，依据消息中的时间间隔发送位置汇报
}TJT808PositionTrack;

//文本下发
typedef struct TJT808TextNotice
{
    TJT808Head head;

    unsigned char nFlag;//文本信息标志位含义见表
    unsigned char szText[256];//文本信息
}TJT808TextNotice;

//事件设置
typedef struct TJT808Event
{
    unsigned char nEventID;//事件ID,若终端已有同ID的事件，则被覆盖
    unsigned char szEvent[24];//事件内容，经GBK编码
}TJT808Event;

typedef struct TJT808EventSet
{
    TJT808Head head;

    unsigned char nType;//设置类型
    unsigned char nCount;//设置总数
    TJT808Event events[JT808_DEF_MAX_NUM_EVENT];//事件项列表
}TJT808EventSet;

//事件报告
typedef struct TJT808EventReport
{
    TJT808Head head;

    unsigned char nEventID;//事件ID
}TJT808EventReport;

//提问下发
typedef struct TJT808Key
{
    unsigned char nKeyID;//答案ID
	unsigned short nKeyLen;//答案内容字段字节长度    确定答案的最大长度   ---by xlsong
	unsigned char szKey[256];//答案内容，经GBK编码
}TJT808Key;

typedef struct TJT808QuestionReq
{
    TJT808Head head;

    unsigned char nFlag;//提问下发标志位定义
    
    unsigned char szQuestion[256];//问题文本，经GBK编码
    unsigned char nKeyCount;//答案总数
    TJT808Key keys[JT808_DEF_MAX_NUM_KEY];//候选答案列表
}TJT808QuestionReq;

//提问应答
typedef struct TJT808QuestionRes
{
    TJT808Head head;

    unsigned short nSeqNum;//应答流水号
    unsigned char  nKeyID;//答案ID
}TJT808QuestionRes;

//消息点播菜单设置
typedef struct TJT808Service
{
    unsigned char nServiceID;//信息类型
    unsigned char szService[24];//信息名称
}TJT808Service;

typedef struct TJT808ServiceSet
{
    TJT808Head head;

    unsigned char nType;//设置类型,0:删除终端全部信息项;1:更新菜单；2追加菜单；3修改菜单
    unsigned char nCount;//信息项总数
    TJT808Service services[JT808_DEF_MAX_NUM_SERVICE];//信息项列表
}TJT808ServiceSet;

//信息点播/取消
typedef struct TJT808ServiceOpt
{
    TJT808Head head;

    unsigned char nType;//信息类型
    unsigned char nFlag;//0 取消；1点播
}TJT808ServiceOpt;

//信息服务
typedef struct TJT808ServiceInfo
{
    TJT808Head head;

    unsigned char nType;//信息类型
    unsigned char szService[256];//信息内容
}TJT808ServiceInfo;

//电话回拨
typedef struct TJT808PhoneCallback
{
    TJT808Head head;

    unsigned char nFlag;//标志 ,0:普通通话；1:监听
    unsigned char szPhone[24];//回呼电话号码
}TJT808PhoneCallback;

//设置电话本
typedef struct TJT808PhoneItem
{
    unsigned char nFlag;//标志,1: 呼入；2: 呼出；3: 呼入/呼出
    unsigned char szPhone[24];
    unsigned char szName[24];
}TJT808PhoneItem;

typedef struct TJT808PhoneBook
{
    TJT808Head head;

    unsigned char nType;//设置类型 
                        //0 删除终端上所有存储的联系人
                        //1 更新电话本(删除终端中已有的全部联系人并追加消息体中的联系人)
                        //2 追加电话本
                        //3 修改电话本(以联系人为索引)
    
    unsigned char nCount;//联系人总数
    TJT808PhoneItem book[JT808_DEF_MAX_NUM_PHONEITEM];//电话本联系人项
}TJT808PhoneBook;

//车辆控制
typedef struct TJT808CarCtrlReq
{
    TJT808Head head;

#ifdef PT_JT808_VER_ITS //ITS平台定义为Short
    unsigned short nCMD;//ITS 客户端,该指令为16bit ,
                        //且保留位不一定全为0，
                        //使用时，只取有效位bit0    ---by xlsong   
#else
    unsigned char nCMD; //控制标志, bit0-0:车门解锁；1:车门加锁  
                        //bit1~6保留    
                        //bit7-0:恢复油电；1:断油电，自定义
#endif
	unsigned int  nServerFlag;//服务器标识信息 自定义
}TJT808CarCtrlReq;

//车辆控制应答
typedef TJT808PositionQueryRes TJT808CarCtrlRes;

//多边形区域区域项数据
typedef struct TJT808Point
{
    double dblLatitude;//纬度;
    double dblLongitude;//经度
}TJT808Point;
//圆形区域区域项内容数据
typedef struct TJT808Circle
{
    unsigned int   nRegionID;//区域ID
    unsigned short nFlag;//区域属性
    TJT808Point    ptCenter;//圆点
    unsigned int   nRadius;//半径,单位为米m
    TJT808DateTime tmStart;//起始时间,若区域属性0位为0则没有该字段
    TJT808DateTime tmEnd;//结束时间,若区域属性0位为0则没有该字段
    unsigned short nMaxSpeed;//最高速度,Km/h，若区域属性1位为0则没有该字段
    unsigned char  nTimeLimit;//超速持续时间,单位为秒s，若区域属性位1为0则没有该字段
}TJT808Circle;
//设置圆形区域
typedef struct TJT808RegionCircleSet
{
    TJT808Head head;

    unsigned char nType;     //设置属性 0:更新区域；1:追加区域；2:修改区域
    unsigned char nRegionCount;//区域总数
    TJT808Circle  Regions[JT808_DEF_MAX_NUM_REGION];//圆形区域的区域项数据
}TJT808RegionCircleSet;

//删除圆形区域
typedef struct TJT808RegionRemove
{
    TJT808Head head;

    unsigned char nRegionCount;//区域总数,不超过125个，多于125个建议用多条消息，0为删除所有圆形区域
    unsigned int  Regions[JT808_DEF_MAX_NUM_REGION];//圆形区域的区域ID
}TJT808RegionRemove;

typedef TJT808RegionRemove TJT808RegionCircleRemove;
//矩形区域区域项数据
typedef struct TJT808Rect
{
    unsigned int   nRegionID;//区域ID
    unsigned short nFlag;//区域属性
    TJT808Point    ptLeftTop;//左上点
    TJT808Point    ptRightBottom;//右下点
    TJT808DateTime tmStart;//起始时间,若区域属性0位为0则没有该字段
    TJT808DateTime tmEnd;//结束时间,若区域属性0位为0则没有该字段
    unsigned short nMaxSpeed;//最高速度,Km/h，若区域属性1位为0则没有该字段
    unsigned char  nTimeLimit;//超速持续时间,单位为秒s，若区域属性位1为0则没有该字段
}TJT808Rect;
//设置矩形区域
typedef struct TJT808RegionRectSet
{
    TJT808Head head;

    unsigned char nType;       //设置属性 0:更新区域；1:追加区域；2:修改区域
    unsigned char nRegionCount;//区域总数
    TJT808Rect  Regions[JT808_DEF_MAX_NUM_REGION];//矩形区域的区域项数据
}TJT808RegionRectSet;

//删除矩形区域
typedef struct TJT808RegionRemove TJT808RegionRectRemove;


//设置多边形区域
typedef struct TJT808RegionPolygonSet
{
    TJT808Head head;

    unsigned int   nRegionID;//区域ID
    unsigned short nFlag;//区域属性
    TJT808DateTime tmStart;//起始时间,若区域属性0位为0则没有该字段
    TJT808DateTime tmEnd;//结束时间,若区域属性0位为0则没有该字段
    unsigned short nMaxSpeed;//最高速度,Km/h，若区域属性1位为0则没有该字段
    unsigned char  nTimeLimit;//超速持续时间,单位为秒s，若区域属性位1为0则没有该字段

    unsigned char nPointCount;//区域顶点总数
    TJT808Point  Points[JT808_DEF_MAX_NUM_POLYGON_POINT];//多边形区域的顶点项
}TJT808RegionPolygonSet;

//删除多边形区域
typedef struct TJT808RegionRemove TJT808RegionPolygonRemove;

//路线拐点项数据
typedef struct TJT808Corner
{
    unsigned int   nCornerID;//拐点ID
    unsigned int   nRoadID;//路段ID
    TJT808Point    ptCorner;//拐点
    unsigned char  nRoadWidth;//路段宽度
    unsigned char  nFlag;//路段属性,单位为米m，路段为该拐点到下一拐点
    unsigned short nMaxTime;//路段行驶过长阀值,单位为秒s，若路段属性0位为0则没有该字段
    unsigned short nMinTime;//路段行驶不足阀值,单位为秒s,若区域属性0位为0则没有该字段
    unsigned short nMaxSpeed;//路段最高速度,Km/h，若区域属性1位为0则没有该字段
    unsigned char  nTimeLimit;//超速持续时间,单位为秒s，若区域属性位1为0则没有该字段
}TJT808Corner;

//设置路线
typedef struct TJT808RouteSet
{
    TJT808Head head;

    unsigned int   nRouteID;//路线ID,重复则更新区域
    unsigned short nFlag;//路线属性
    TJT808DateTime tmStart;//起始时间,若路线属性0位为0则没有该字段
    TJT808DateTime tmEnd;//结束时间,若路线属性0位为0则没有该字段
   
    unsigned short nCornerCount;//路线总拐点数
    TJT808Corner  Corners[JT808_DEF_MAX_NUM_ROUTE_CORNER];//路线拐点项数据
}TJT808RouteSet;

//删除路线
typedef struct TJT808RegionRemove TJT808RouteRemove;

//行驶记录仪数据采集命令
typedef struct TJT808DriveingRecorderReq
{
    TJT808Head head;

    unsigned char nType;    //命令字,列表见GB/T19056中相关要求
    unsigned int  nDataLen; //数据块长度
    unsigned char data[256];//数据
}TJT808DriveingRecorderReq;

//行驶记录仪采集数据上传
typedef struct TJT808DriveingRecorderRes
{
    TJT808Head head;

    unsigned short nSeqNum;// 应答流水号
    unsigned char  nType;    //命令字,列表见GB/T19056中相关要求

    unsigned short nDataLen; //数据块长度
    unsigned char  data[1008];//数据
}TJT808DriveingRecorderRes;

//行驶记录仪参数下传命令
typedef struct TJT808DriveingRecorderParam
{
    TJT808Head head;

    unsigned char nType;    //命令字,列表见GB/T19056中相关要求
    unsigned int  nDataLen; //数据块长度
    unsigned char data[256];//数据
}TJT808DriveingRecorderParam;

//电子运单上报
typedef struct TJT808OrderReport
{
    TJT808Head head;

    unsigned int  nDataLen;//电子运单长度
    unsigned char data[256];//电子运单内容
}TJT808OrderReport;

//驾驶员身份信息采集上报
typedef struct TJT808DriverInfoReport
{
    TJT808Head head;
	unsigned char  nStatus;//0x01：从业资格证 IC 卡插入,0x02：从业资格证 IC 卡拔出---Ver201301 add
	TJT808DateTime time;//时间 YY-MM-DD-hh-mm-ss   ---Ver201301 add
	unsigned char  nICStatus;//IC 卡读取结果 ---Ver201301 add
    unsigned char  szName[24];//驾驶员姓名
    unsigned char  szIDCard[24];//驾驶员身份证号码  ---Ver201301 delete this item    by xlsong
    unsigned char  szCertificateID[48];//从业资格证编号
    unsigned char  szIssueDepartment[128];//发证机构名称
	unsigned char  szUsefulLife[4];//证件有效期  YYYYMMDD  ---Ver201301 add
}TJT808DriverInfoReport;

//多媒体事件信息上传
typedef struct TJT808MediaEventReport
{
    TJT808Head head;

    unsigned int   nMediaID;//多媒体ID
    unsigned char  nMediaType;//0图像；1音频；2视频
    unsigned char  nCodeType;//0JPEG；1TIF；2MP3；3WAV；4WMV；其他保留
    unsigned char  nEventType;//0平台下发指令；1定时动作；2抢劫报警触发；3碰撞侧翻报警触发；其他保留
    unsigned char  nChannel;//通道ID
}TJT808MediaEventReport;

//多媒体数据上传
typedef struct TJT808MediaDataUploadReq
{
    TJT808Head head;
    
    unsigned int   nMediaID;//多媒体ID
    unsigned char  nMediaType;//0图像；1音频；2视频
    unsigned char  nCodeType;//0JPEG；1TIF；2MP3；3WAV；4WMV；其他保留
    unsigned char  nEventType;//0平台下发指令；1定时动作；2抢劫报警触发；3碰撞侧翻报警触发；其他保留
    unsigned char  nChannel;//通道ID
	TJT808PosiState position;//多媒体数据的位置基本信息数据   ---Ver201301
    unsigned short nDataLen;
    unsigned char  data[1024];//多媒体数据包
}TJT808MediaDataUploadReq;

//多媒体数据上传应答
typedef struct TJT808MediaDataUploadRes
{
    TJT808Head head;

    unsigned int   nMediaID;
    unsigned char  nResendCount;//重传包数
    unsigned short  ResendIDList[255];//重传数据包ID列表 
}TJT808MediaDataUploadRes;

//摄像头立即拍摄指令 
typedef struct TJT808CameralCtrl
{
    TJT808Head head;
	
	unsigned char  	nChannel;//通道ID  (根据ITS的协议:bit0-channel1,bit1-channel2...)
	unsigned short  nType;   //拍摄指令 0:表示停止拍摄 1:开始拍摄 0xFFFF:表示录像;其它表示拍照次数
	//unsigned short  nPhototime;//拍照张数   modify by xlsong
	unsigned short	nInterval;//拍摄间隔/录像时间 单位:s; 0:表示按最小时间间隔拍照或一直录像
	unsigned char  	nSaveFlag;//保存标志 0:表示实时上传;1:表示保存
	
	unsigned char    nDistinguishability;//分辨率
    unsigned char  	nPictureQuality;//图像/视频质量 1~10 1:表示质量损失最小 10:压缩比最大
	unsigned char  	nBrightness;//亮度 0~255
	unsigned char  	nContrast;//对比度 0~127
	unsigned char  	nSaturation;//饱和度 0~127
	unsigned char  	nChroma;//色度 0~255
	
}TJT808CameralCtrl;


//存储多媒体数据检索
typedef struct TJT808MediaIndexReq
{
    TJT808Head head;
	
    unsigned char nMedia;//多媒体类型 0:图像; 1:音频; 2:视频
    unsigned char nChannel;//通道ID   0:表示检索该媒体类型的所有通道
    unsigned char nEventType;////事件项编码 0:平台下发指令 1:定时动作 2:抢劫报警触发 3:碰撞侧翻报警触发；其它保留

    TJT808DateTime  tmStart;//起始时间 YY-MM-DD-HH-MM-SS
    TJT808DateTime  tmEnd;//结束时间  YY-MM-DD-HH-MM-SS
	
}TJT808MediaIndexReq;

//多媒体数据总项数
typedef struct TJT808MediaIndex
{
	unsigned int nMediaID;//多媒体ID  20130124新协议增加  ---- Ver201301 add
    unsigned char nMedia;//多媒体类型 0:图像; 1:音频; 2:视频
    unsigned char nChannel;//通道ID   0:表示检索该媒体类型的所有通道
    unsigned char nEventType;//事件项编码 0:平台下发指令 1:定时动作 2:抢劫报警触发 3:碰撞侧翻报警触发；其它保留
    TJT808PosiState position;//位置信息汇报
}TJT808MediaIndex;
	
//存储多媒体数据检索应答 
typedef struct TJT808MediaIndexRes
{
    TJT808Head head;

	unsigned short nSeqNum;//应答流水号
	
	unsigned short nIndexcount;//多媒体数据检索总项数 2013-01-11
    TJT808MediaIndex IndexData[JT808_DEF_MAX_NUM_INDEX_MEDIA];//多媒体检索项数据 2013-01-11
	
}TJT808MediaIndexRes;

//存储多媒体数据上传命令 
typedef struct TJT808MediaDataUpload
{
    TJT808Head head;

	unsigned char nMedia;//多媒体类型 0:图像; 1:音频; 2:视频
    unsigned char nChannel;//通道ID   0:表示检索该媒体类型的所有通道
    unsigned char nEventType;//事件项编码 0:平台下发指令 1:定时动作 2:抢劫报警触发 3:碰撞侧翻报警触发；其它保留
    
    TJT808DateTime  tmStart;//起始时间 YY-MM-DD-HH-MM-SS
    TJT808DateTime  tmEnd;//结束时间   YY-MM-DD-HH-MM-SS
	unsigned char   nDelFlag;//删除标志
}TJT808MediaDataUpload;

//录音命令开始/停止
typedef struct TJT808RecorderCtrl
{
    TJT808Head head;

    unsigned char  nCMD;    //0:停止；1：开始
    unsigned short nTimeLimit; //时间长度 0：表示一直工作
    unsigned char  nSaveFlag;//0:上传；1：保存本地
    unsigned char  nSample;//录音采样率 0:8K；1:11K；2: 23K；3: 32K；其他保留
}TJT808RecorderCtrl;

//数据透传
typedef struct TJT808DataLoad
{
    TJT808Head head;

    unsigned char  nDataType;//数据类型
    unsigned int nDataLen; //数据长度   by xlsong
    unsigned char  data[420];//数据内容
}TJT808DataLoad;

//数据下行透传
typedef TJT808DataLoad  TJT808DataDownload;

//数据上行透传
typedef TJT808DataLoad  TJT808DataUpload;

//数据压缩上报
typedef TJT808DataLoad  TJT808DataGzipLoad;

//RSA公钥
typedef struct TJT808Ras
{
    TJT808Head head;

    unsigned int   nRasE;//RSA公钥{e,n}中的e
    unsigned short nRasNLen; //RSA公钥{e,n}中的n长度
    unsigned char  RasN[128];//RSA公钥{e,n}中的n
}TJT808Ras;

//平台RAS公钥
typedef TJT808Ras  TJT808PlatFormRas;

//终端RAS公钥
typedef TJT808Ras  TJT808TerminalRas;

//自定义协议
//多用户位置上报
typedef struct TSimplePosition
{
    unsigned char nUserID;//设备编号
    double dblLatitude;//纬度;
    double dblLongitude;//经度
    double dblSpeed;//速度 Km/h 
    unsigned short nDeriction;//方向
}TSimplePosition;

typedef struct TJT808MultiPosition
{
    TJT808Head head;

    unsigned short nYear;//年
    unsigned char  nMonth;//月
    unsigned char  nDay;//日
    unsigned char  nHour;//时
    unsigned char  nMinute;//分
    unsigned char  nSecond;//秒

    unsigned char  nCount;//用户数
    TSimplePosition position[JT808_DEF_MAX_NUM_USERPOSITION];
}TJT808MultiPosition;

//传感器数据上传

typedef struct TJT808SensorReport
{
    TJT808Head head;

    unsigned char  nOilCount;//油量传感器数
    unsigned short oils[JT808_DEF_MAX_NUM_SENSOR_OIL];//油量 %
    unsigned char  oilsstate[JT808_DEF_MAX_NUM_SENSOR_OIL];//油量状态
		
    unsigned char  nTempCount;//温度传感器数
    unsigned short temps[JT808_DEF_MAX_NUM_SENSOR_TEMP];//摄氏度°
    unsigned char  tempsstate[JT808_DEF_MAX_NUM_SENSOR_TEMP];//温度状态
	
    unsigned char  nTyreCount;//胎压传感器数
    JT808TyreSensor tyres[JT808_DEF_MAX_NUM_SENSOR_TYRE];
	
    unsigned char  nPosiCount;//位移传感器数
    unsigned short positions[JT808_DEF_MAX_NUM_SENSOR_POSI];//位移

	unsigned char  nCarryState;		//载货状态 
    unsigned int   nWarnFlag;//报警状态
    unsigned int   nState;//车辆状态
}TJT808SensorReport;
//传感器数据查询请求
typedef struct TJT808SensorQueryReq
{
    TJT808Head head;
    
    unsigned char  szDevice[16];//设备编号
}TJT808SensorQueryReq;

//传感器数据查询反馈
typedef struct TJT808SensorQueryRes
{
    TJT808Head head;
    unsigned short nSeqNum;// 应答流水号
    unsigned char  nResult;// 应答结果
    unsigned char  nCarryState;//载货状态
    unsigned int   nWarnFlag;//报警状态
    unsigned int   nState;//车辆状态
    unsigned char  szOil[24];//油量数据
    unsigned char  szTemp[24];//温度传感器数据
    unsigned char  szTyre[128];//胎压数据
    unsigned char  szPosi[32];//位移传感器数据
}TJT808SensorQueryRes;
//------Ver201301 Ver201301---
typedef struct TJT808SendAgainReq
{
	TJT808Head head;
	unsigned short nSeqNum;// 应答流水号
	unsigned char nCount;//重传包总数
	unsigned short nIDlist[128];//重传 ID 列表
}TJT808SendAgainReq;

//查询终端属性应答  Ver201301
typedef struct TJT808PropertyQueryRes
{
	TJT808Head head;
	unsigned short nType;//终端类型
	unsigned char szManID[6];//制造商ID
	unsigned char szModel[20] ;//终端型号
	unsigned char szTerminalID[8] ;//终端ID
	unsigned char szSIMID[20];//终端 SIM 卡 ICCID
	unsigned char szHWVerNum[8];//终端硬件版本号
	unsigned char szFWVerNum[8];// 终端固件版本号
	unsigned char nGNSSProp;//GNSS 模块属性
	unsigned char nCommProp;//通信模块属性
}TJT808PropertyQueryRes;

//下发终端升级包  Ver301301
typedef struct TJT808UpdatePackageReq
{
	TJT808Head head;
	unsigned char nType;// 升级类型[0：终端,12:道路运输证 IC 卡读卡器,52:北斗卫星定位模块]
	unsigned char szManID[6]; //制造商ID
	unsigned char szVerNum[8];//版本号
    unsigned int  nPackageLen;//升级数据包总长度
	unsigned int  nDataLen;   //本包数据长度
	unsigned char data[1004]; //升级数据  1023-18 = 1005
}TJT808UpdatePackageReq;

//终端升级结果通知  Ver301301
typedef struct TJT808UpdatePackageRes
{
	TJT808Head head;
	unsigned char nUpdateType;// 升级类型
	unsigned char nResult;//升级结果 0：成功，1：失败，2：取消
}TJT808UpdatePackageRes;

//人工确认报警消息   Ver201301
typedef struct TJT808WarmManage
{
	TJT808Head head;
	unsigned short nSeqNum;//报警消息流水号  需人工确认的报警消息流水号，0 表示该报警类型所有消息
	unsigned int   nWarmType;//人工确认报警类型
}TJT808WarmManage;

//定位数据批量上传  Ver201301
typedef struct TJT808PosDataUpload
{
	TJT808Head head;
	unsigned short nTotalCount;//数据项总数
	unsigned char  nPosType;//位置数据类型
    
#ifdef JT808_PLATFORM
    unsigned short nDataCount;//本包数据项个数
	TJT808PositionInfo posdata[20];//位置汇报消息体
#else//设备
    unsigned short nDataLen;
    unsigned char  data[1000];
#endif    
}TJT808PosDataUpload;

//CAN总线数据上传  Ver201301
typedef struct TJT808CanDataUpload
{
	TJT808Head head;
    
	unsigned short nTotalCount;
	unsigned char  nHour;//时   CAN 总线数据接收时间
	unsigned char  nMinute;//分
	unsigned char  nSecond;//秒
	unsigned char  nmSecond;//毫秒
    
    unsigned short nDataCount;//数据项个数
	TJT808CanParam CanParam[128];//CAN 总线数据项 
    unsigned char  CanData[128][8];
}TJT808CanDataUpload;

//摄像头立即拍摄命令应答 Ver201301
typedef struct TJT808CameraCtrlRes
{
	TJT808Head head;
	unsigned short nSeqNum;// 应答流水号
	unsigned char nResult;//结果
	unsigned short nMediaCount;//拍摄成功的多媒体个数 
	unsigned int   MediaIDList[JT808_DEF_MAX_NUM_MEDIAID];//多媒体 ID 列表
}TJT808CameraCtrlRes;

//单条存储多媒体数据检索上传命令 Ver201301
typedef struct TJT808OneMediaDataQueryReq
{
	TJT808Head head;
	unsigned int nMediaID;//多媒体 ID
	unsigned char   nDelFlag;//删除标志
}TJT808OneMediaDataQueryReq;

//设备使用前解锁 Ver201301
typedef struct TJT808DeviceUnlockReq
{
	TJT808Head head;
    
    unsigned char bEncrypt;
    unsigned int  nEncryptKey;
             char szUserMobile[12];
    
    unsigned short nProvID;//省域ID
    unsigned short nCity;//市县域ID
    
	unsigned char  szManufacturer[6];//制造商ID
    unsigned char  szModel[20];//终端型号
    unsigned char  szDeviceID[8];//终端ID
    unsigned char  nColor;//车牌颜色
    unsigned char  szLicenseplate[10];//车牌号
    unsigned char  szVIN[20];//车辆VIN
}TJT808DeviceUnlockReq;

//--------------------------------
#endif// end PT_ENABLE_JT808

#endif//end  H_PT_JT808_H

