#include <jni.h>
#include <string>
#include "add.h"
//#include "./utility/utility.h"
#include "./protocol/pt_config.h"



enum  WM_SF_OUTCMD
{
    WM_SF_OUTCMD_NUL =0,
    WM_SF_OUTCMD_BSS = 1,//  波束设置
    WM_SF_OUTCMD_BSIO ,//  打开波束功率
    WM_SF_OUTCMD_BSIC ,//  关闭波束功率
    WM_SF_OUTCMD_CXA ,//  查询申请
    WM_SF_OUTCMD_DSA ,//  定时申请
    WM_SF_OUTCMD_DWA ,//  定位申请
    WM_SF_OUTCMD_TXA ,//  通信申请

    WM_SF_OUTCMD_ICA ,//  模块保密模块信息检测
    WM_SF_OUTCMD_GXM ,//  管理信息读取
    WM_SF_OUTCMD_LZMREAD ,// 零值读取
    WM_SF_OUTCMD_LZMSET ,//  零值设置
    WM_SF_OUTCMD_PKY ,//  PKY加注

    WM_SF_OUTCMD_ZTA ,//  模块自检
    WM_SF_OUTCMD_RMO ,//  请求语句
    WM_SF_OUTCMD_SBX ,//  模块信息
    WM_SF_OUTCMD_KLS ,//  口令识别
    WM_SF_OUTCMD_JMS ,//  静默设置
    WM_SF_OUTCMD_JMQ ,//  静默设置
    WM_SF_OUTCMD_ZHS ,//  自毁设置
    WM_SF_OUTCMD_RIS ,//  复位设置
    WM_SF_OUTCMD_GBS ,//  永久关闭设置

    WM_SF_OUTCMD_UPPOS ,//  上传位置

};


int jstringToChar(JNIEnv* env, jstring jstr, uchar* rtn) {
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("GB2312");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if ((alen > 0)&&(alen < 1024)) {
        //rtn = (char*) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return alen;
}



//=========================================================================================================
//int  byType;  // 2=打开波束功率 3=WM_SF_OUTCMD_BSIC 15=模块信息
//int   nMode;   // 输出模式
//int   nFreq;   // 输出频度
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_sectest_MainActivity_EncodeSFCmdRMOJNI(
        JNIEnv* env,
        jobject ,
        jint byType,
        jint nMode,
        jint nFreq
)
{
    bool bValid = true;
    uchar SendBuf[128] = {0};
    uchar checkSum = 0;
    int nLen = 0;

    if (bValid)
    {
        if (nMode > 2)
        {
            nLen = sprintf((char *)SendBuf, "$CCRMO,,%d,%d*",nMode, nFreq);
        }
        else
        {
            switch(byType)
            {
                case WM_SF_OUTCMD_BSIO:
                    nLen = sprintf((char *)SendBuf, "$CCRMO,%s,%d,%d*","BSI",nMode, nFreq);
                    break;
                case WM_SF_OUTCMD_BSIC:
                    nLen = sprintf((char *)SendBuf, "$CCRMO,%s,%d,%d*","BSI",nMode, nFreq);
                    break;
                case WM_SF_OUTCMD_SBX:
                    nLen = sprintf((char *)SendBuf, "$CCRMO,%s,%d,%d*","SBX",nMode, nFreq);
                    break;
                default:
                    break;
            }

        }
        if (nLen > 2)
        {
            for (int i = 1; i < nLen; i++)
            {
                checkSum ^= SendBuf[i];
            }
            nLen += sprintf((char *)(SendBuf + nLen), "%02X", checkSum);
        }
    }

    std::string s ;
    for(int i=0; i<nLen; i++){
        s.append(1,SendBuf[i]);
    }
    if(nLen>2){
        return env->NewStringUTF(s.c_str());
    }

    return env->NewStringUTF("");
}

// 波束设置
//int   byXYBSID; // 响应波束ID
//int   bySCBSID; // 时差波束
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_sectest_MainActivity_EncodeSFCmdBSSJNI(
        JNIEnv* env,
        jobject ,
        jint byXYBSID,
        jint bySCBSID
)
{
    bool bValid = true;
    uchar SendBuf[128] = {0};
    uchar checkSum = 0;
    int nLen = 0;

    if (bValid)
    {
        if (byXYBSID == 0 && bySCBSID == 0)
        {
            nLen = sprintf((char *)SendBuf, "$CCBSS,,*");
        }
        else if (bySCBSID == 0)
        {
            nLen = sprintf((char *)SendBuf, "$CCBSS,%d,*",byXYBSID);
        }
        else if (byXYBSID == 0)
        {
            nLen = sprintf((char *)SendBuf, "$CCBSS,,%d*",bySCBSID);
        }
        else
            nLen = sprintf((char *)SendBuf, "$CCBSS,%02d,%02d*",byXYBSID, bySCBSID);

        if (nLen > 2)
        {
            for (int i = 1; i < nLen; i++)
            {
                checkSum ^= SendBuf[i];
            }
            nLen += sprintf((char *)(SendBuf + nLen), "%02X", checkSum);
        }
    }

    std::string s ;
    for(int i=0; i<nLen; i++){
        s.append(1,SendBuf[i]);
    }
    if(nLen>2){
        return env->NewStringUTF(s.c_str());
    }

    return env->NewStringUTF("");
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_sectest_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jint JNICALL
Java_com_example_sectest_MainActivity_addJNI(
        JNIEnv* env,
        jobject ,
        jint a,
        jint b) {

    int he = add(a,b);
    return he;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_example_sectest_MainActivity_PTDecodeTypeJNI(
        JNIEnv* env,
        jobject ,
        jstring jstrBuf) {
    uchar _RcvBuf[1024];	//串口数据接收buff
    int nBuffLen = 0;
    nBuffLen = jstringToChar(env,jstrBuf,_RcvBuf);
    if((0 < nBuffLen)&&(nBuffLen < 1024)){
        int he = PT_DecodeType(_RcvBuf, nBuffLen, STACK_BD2_RD);
        return  he;
    }
    return 0;
}

//ICA,2代RDSS输入协议，检测本用户设备加解密模块信息
//指令类型（0-1  0：本机  1：下属用户）
//uchar nCmdType;
//下属用户信息帧号
//uchar nFrameNum;
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_sectest_MainActivity_PTEncodePackageICAJNI(
        JNIEnv* env,
        jobject ,
        jint nCmdType,
        jint nFrameNum) {
    TRDICA pkg;
    uchar SendBuf[50] = {0};

    pkg.nCmdType = nCmdType;
    pkg.nFrameNum = nFrameNum;/////需根据ICI信息解析确定

    int nLen = PT_EncodePackage(SendBuf, 50, &pkg, BD2_RD_ICA, 0);
    std::string s ;
    for(int i=0; i<nLen; i++){
        s[i] = SendBuf[i];
    }
    if(nLen>0){
        return env->NewStringUTF(s.c_str());
    }

    return env->NewStringUTF("");
}

//DWA,2代RDSS输入协议，发送定位申请
//用户地址（ID号）
//int nUserID;
//紧急定位（A:紧急定位,V:普通定位）
//uchar cUrgencyPos;
//测高方式（0:有高程,1:无高程,2:测高1,3:测高2）
//uchar uc_MHMode;
//高程指示（H:高空用户,L:普通用户）
//uchar cLocaType;
//高程数据（米）
//double db_HeightData;
//天线高（米）
//double db_AntHigh;
//气压数据（帕）
//double db_AirPressure;
//温度数据（度）
//double db_Temperature;
//申请频度（秒,0:单次定位）
//double db_Freq;
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_sectest_MainActivity_PTEncodePackageDWAJNI(
        JNIEnv* env,
        jobject ,
        jint nUserID,
        jchar cUrgencyPos,
        jchar uc_MHMode,
        jchar cLocaType,
        jdouble db_HeightData,
        jdouble db_AntHigh,
        jdouble db_AirPressure,
        jdouble db_Temperature,
        jdouble db_Freq
        ) {
    TRDDWA pkg;
    uchar SendBuf[256] = {0};

    pkg.nUserID = nUserID;
    pkg.cUrgencyPos = cUrgencyPos;
    pkg.uc_MHMode = uc_MHMode;
    pkg.cLocaType = cLocaType;
    pkg.db_HeightData = db_HeightData;
    pkg.db_AntHigh = db_AntHigh;
    pkg.db_AirPressure = db_AirPressure;
    pkg.db_Temperature = db_Temperature;
    pkg.db_Freq = db_Freq;

    int nLen = PT_EncodePackage(SendBuf, 256, &pkg, BD2_RD_DWA, 0);
    std::string s ;
    for(int i=0; i<nLen; i++){
        s[i] = SendBuf[i];
    }
    if(nLen>0){
        return env->NewStringUTF(s.c_str());
    }

    return env->NewStringUTF("");
}

//TXA,2代RDSS输入协议，用于设置用户设备发送通信申请
//用户地址（ID号）
//int nUserID;
//通信类别（0/1 0：特快 1：普通）
//uchar nCommMode;
//传输方式（0-2 0：汉字 1：代码 2:混发 3:位置上报）
//uchar nInfoType;
//通信电文内容
//string Info;
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_sectest_MainActivity_PTEncodePackageTXAJNI(
        JNIEnv* env,
        jobject ,
        jint nUserID,
        jchar nCommMode,
        jchar nInfoType,
        jstring Info
) {
    TRDTXA pkg;
    uchar SendBuf[512] = {0};
    uchar _RcvBuf[1024];

    pkg.nUserID = nUserID;
    pkg.nCommMode = nCommMode;
    pkg.nInfoType = nInfoType;
    pkg.nInfolen = jstringToChar(env,Info,_RcvBuf);
    for(int i=0; i<pkg.nInfolen; i++){
        pkg.Info[i] = _RcvBuf[i];
    }

    int nLen = PT_EncodePackage(SendBuf, 512, &pkg, BD2_RD_TXA, 0);
    std::string s ;
    for(int i=0; i<nLen; i++){
        s[i] = SendBuf[i];
    }
    if(nLen>0){
        return env->NewStringUTF(s.c_str());
    }

    return env->NewStringUTF("");
}

//CXA,2代RDSS输入协议，查询本机通信信息
//查询类别（0:定位查询,1:通信查询）
//uchar nQueryType;
//查询方式（1-3）
//uchar nQueryMode;
//用户地址（ID号）
//int nUserID;
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_sectest_MainActivity_PTEncodePackageCXAJNI(
        JNIEnv* env,
        jobject ,
        jint nUserID,
        jint nQueryType,
        jint nQueryMode
) {
    TRDCXA pkg;
    uchar SendBuf[50] = {0};

    pkg.nUserID = nUserID;
    pkg.nQueryType = nQueryType;
    pkg.nQueryMode = nQueryMode;

    int nLen = PT_EncodePackage(SendBuf, 50, &pkg, BD2_RD_CXA, 0);
    std::string s ;
    for(int i=0; i<nLen-2; i++){
        s.append(1,SendBuf[i]);
    }
    if(nLen>0){
        return env->NewStringUTF(s.c_str());
    }

    return env->NewStringUTF("");
}

//WAA,2代RDSS双向协议，用户设置用户设备发送位置报告 1申请，本语句不适用于指挥型用户设备监收的位置信息输出
//用户地址（ID号）
//int nUserID;
//信息类型（0-1  0：用户设备就收的位置报告  1：用户设备发送的位置报告）
//uchar nMsgType;
//报告频度（秒  0：单次位置报告）
//float f_Freq;
//位置报告时间
//uchar nReportHour;
//uchar nReportMin;
//float f_ReportSecond;
//纬度
//float f_Lat;
//纬度方向
//uchar cLatAspect;
//经度
//float f_Lon;
//经度方向
//uchar cLonAspect;
//高程值
//float f_Height;
//高程单位（米）
//uchar cHeightUnit;
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_sectest_MainActivity_PTEncodePackageWAAJNI(
        JNIEnv* env,
        jobject ,
        jint nUserID,
        jchar nMsgType,
        jfloat f_Freq,
        jchar nReportHour,
        jchar nReportMin,
        jfloat f_ReportSecond,
        jfloat f_Lat,
        jchar cLatAspect,
        jfloat f_Lon,
        jchar cLonAspect,
        jfloat f_Height,
        jchar cHeightUnit
) {
    TRDWAA pkg;
    uchar SendBuf[256] = {0};

    pkg.nUserID = nUserID;
    pkg.nMsgType = nMsgType;
    pkg.f_Freq = f_Freq;
    pkg.nReportHour = nReportHour;
    pkg.nReportMin = nReportMin;
    pkg.f_ReportSecond = f_ReportSecond;
    pkg.f_Lat = f_Lat;
    pkg.cLatAspect = cLatAspect;
    pkg.f_Lon = f_Lon;
    pkg.cLonAspect = cLonAspect;
    pkg.f_Height = f_Height;
    pkg.cHeightUnit = cHeightUnit;

    int nLen = PT_EncodePackage(SendBuf, 256, &pkg, BD2_RD_WAA, 0);
    std::string s ;
    for(int i=0; i<nLen; i++){
        s[i] = SendBuf[i];
    }
    if(nLen>0){
        return env->NewStringUTF(s.c_str());
    }

    return env->NewStringUTF("");
}

//WBA,2代RDSS输入协议，用户设置用户设备发送位置报告 2申请，该语句对应的输出语句为DWR
//用户地址（ID号）
//int nUserID;
//高程指示（H/L）
//uchar cHeightflag;
//天线高（米）
//double f_AnteHeight;
//报告频度（秒 0：单次位置报告）
//float f_Freq;
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_sectest_MainActivity_PTEncodePackageWBAJNI(
        JNIEnv* env,
        jobject ,
        jint nUserID,
        jchar cHeightflag,
        jdouble f_AnteHeight,
        jfloat f_Freq
) {
    TRDWBA pkg;
    uchar SendBuf[256] = {0};

    pkg.nUserID = nUserID;
    pkg.cHeightflag = cHeightflag;
    pkg.f_AnteHeight = f_AnteHeight;
    pkg.f_Freq = f_Freq;

    int nLen = PT_EncodePackage(SendBuf, 256, &pkg, BD2_RD_WBA, 0);
    std::string s ;
    for(int i=0; i<nLen; i++){
        s[i] = SendBuf[i];
    }
    if(nLen>0){
        return env->NewStringUTF(s.c_str());
    }

    return env->NewStringUTF("");
}

extern "C" JNIEXPORT jint JNICALL
Java_com_example_sectest_MainActivity_PTDecodePackageICIJNI(
        JNIEnv* env,
        jobject ,
        jobject trdObj,
        jstring rcvBuf
) {
    TRDICI pkg;
    uchar buf[1024] = {0};
    int nLen = jstringToChar(env,rcvBuf,buf);
    int nType = PT_DecodePackage(&pkg, buf, nLen, STACK_BD2_RD);

    jfieldID fid;

    jclass clazz = env->GetObjectClass(trdObj);
    if (0 == clazz) {
        printf("GetObjectClass returned 0\n");
        return (-1);
    }
    fid = env->GetFieldID(clazz, "nUserID", "I");
    env->SetIntField(trdObj, fid, pkg.nUserID);
    fid = env->GetFieldID( clazz, "nSerialNum", "I");
    env->SetIntField( trdObj, fid, pkg.nSerialNum);
    fid = env->GetFieldID( clazz, "nBrdID", "I");
    env->SetIntField( trdObj, fid, pkg.nBrdID);

    fid = env->GetFieldID( clazz, "nUserType", "C");
    env->SetCharField( trdObj, fid, pkg.nUserType);

    fid = env->GetFieldID( clazz, "f_Freq", "F");
    env->SetFloatField( trdObj, fid, pkg.f_Freq);

    fid = env->GetFieldID( clazz, "nCommLevel", "C");
    env->SetCharField( trdObj, fid, pkg.nCommLevel);

    fid = env->GetFieldID( clazz, "cEncryptFlag", "C");
    env->SetCharField( trdObj, fid, pkg.cEncryptFlag);

    fid = env->GetFieldID( clazz, "nSubuserNum", "I");
    env->SetIntField( trdObj, fid, pkg.nSubuserNum);

    return nType;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_example_sectest_MainActivity_PTDecodePackageBSIJNI(
        JNIEnv* env,
        jobject ,
        jobject trdObj,
        jstring rcvBuf
) {
    TRDBSI pkg;
    uchar buf[1024] = {0};
    int nLen = jstringToChar(env,rcvBuf,buf);
    int nType = PT_DecodePackage(&pkg, buf, nLen, STACK_BD2_RD);

    jfieldID fid;

    jclass clazz = env->GetObjectClass(trdObj);
    if (0 == clazz) {
        printf("GetObjectClass returned 0\n");
        return (-1);
    }

    fid = env->GetFieldID( clazz, "nInboudBeam", "C");
    env->SetCharField( trdObj, fid, pkg.nInboudBeam);

    fid = env->GetFieldID( clazz, "nTmDifBeam", "C");
    env->SetCharField( trdObj, fid, pkg.nTmDifBeam);

    fid =env->GetFieldID( clazz, "nPower", "Ljava/lang/String;");
    std::string s ;
    for(int i=0; i<10; i++){
        s[i] = pkg.nPower[i];
    }
    jstring name = env->NewStringUTF(s.c_str());
    env->SetObjectField(trdObj, fid, name);

    return nType;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_example_sectest_MainActivity_PTDecodePackageICZJNI(
        JNIEnv* env,
        jobject ,
        jobject trdObj,
        jstring rcvBuf
) {
    TRDICZ pkg;
    uchar buf[1024] = {0};
    int nLen = jstringToChar(env,rcvBuf,buf);
    int nType = PT_DecodePackage(&pkg, buf, nLen, STACK_BD2_RD);

    jfieldID fid;

    jclass clazz = env->GetObjectClass(trdObj);
    if (0 == clazz) {
        printf("GetObjectClass returned 0\n");
        return (-1);
    }

    fid = env->GetFieldID( clazz, "nSubuserNum", "I");
    env->SetIntField( trdObj, fid, pkg.nSubuserNum);

    int arrayLength=sizeof(pkg.nSubuserID)/sizeof(pkg.nSubuserID[0]);
    std::string strs;
    for(int i=0;i<arrayLength;i++)
    {
        if(i != 0){ strs += ",";}
        int temp=pkg.nSubuserID[i];
        strs += std::to_string(temp);
    }
    fid =env->GetFieldID( clazz, "nSubuserID", "Ljava/lang/String;");
    jstring cCmdName = env->NewStringUTF(strs.c_str());
    env->SetObjectField(trdObj, fid, cCmdName);

    return nType;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_example_sectest_MainActivity_PTDecodePackageDWRJNI(
        JNIEnv* env,
        jobject ,
        jobject trdObj,
        jstring rcvBuf
) {
    TRDDWR pkg;
    uchar buf[1024] = {0};
    int nLen = jstringToChar(env,rcvBuf,buf);
    int nType = PT_DecodePackage(&pkg, buf, nLen, STACK_BD2_RD);

    jfieldID fid;

    jclass clazz = env->GetObjectClass(trdObj);
    if (0 == clazz) {
        printf("GetObjectClass returned 0\n");
        return (-1);
    }

    fid = env->GetFieldID( clazz, "nPosType", "C");
    env->SetCharField( trdObj, fid, pkg.nPosType);

    fid = env->GetFieldID( clazz, "nUserID", "I");
    env->SetIntField( trdObj, fid, pkg.nUserID);

    fid = env->GetFieldID( clazz, "nHour", "C");
    env->SetCharField( trdObj, fid, pkg.nHour);
    fid = env->GetFieldID( clazz, "nMinute", "C");
    env->SetCharField( trdObj, fid, pkg.nMinute);
    fid = env->GetFieldID( clazz, "f_Second", "F");
    env->SetFloatField( trdObj, fid, pkg.f_Second);

    fid = env->GetFieldID( clazz, "db_Lat", "D");
    env->SetDoubleField( trdObj, fid, pkg.db_Lat);
    fid = env->GetFieldID( clazz, "cLatDir", "C");
    env->SetCharField( trdObj, fid, pkg.cLatDir);
    fid = env->GetFieldID( clazz, "db_Lon", "D");
    env->SetDoubleField( trdObj, fid, pkg.db_Lon);
    fid = env->GetFieldID( clazz, "cLonDir", "C");
    env->SetCharField( trdObj, fid, pkg.cLonDir);
    fid = env->GetFieldID( clazz, "db_Height", "D");
    env->SetDoubleField( trdObj, fid, pkg.db_Height);
    fid = env->GetFieldID( clazz, "cHunit", "C");
    env->SetCharField( trdObj, fid, pkg.cHunit);

    fid = env->GetFieldID( clazz, "f_Kesi", "F");
    env->SetFloatField( trdObj, fid, pkg.f_Kesi);
    fid = env->GetFieldID( clazz, "cKesiUnit", "C");
    env->SetCharField( trdObj, fid, pkg.cKesiUnit);

    fid = env->GetFieldID( clazz, "nPrecision", "C");
    env->SetCharField( trdObj, fid, pkg.nPrecision);
    fid = env->GetFieldID( clazz, "cUrgencyPos", "C");
    env->SetCharField( trdObj, fid, pkg.cUrgencyPos);
    fid = env->GetFieldID( clazz, "cMutilpleValue", "C");
    env->SetCharField( trdObj, fid, pkg.cMutilpleValue);
    fid = env->GetFieldID( clazz, "cLocaType", "C");
    env->SetCharField( trdObj, fid, pkg.cLocaType);

    return nType;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_example_sectest_MainActivity_PTDecodePackageTXRJNI(
        JNIEnv* env,
        jobject ,
        jobject trdObj,
        jstring rcvBuf
) {
    TRDTXR pkg;
    uchar buf[1024] = {0};
    int nLen = jstringToChar(env,rcvBuf,buf);
    int nType = PT_DecodePackage(&pkg, buf, nLen, STACK_BD2_RD);

    jfieldID fid;

    jclass clazz = env->GetObjectClass(trdObj);
    if (0 == clazz) {
        printf("GetObjectClass returned 0\n");
        return (-1);
    }

    fid = env->GetFieldID( clazz, "nMsgType", "C");
    env->SetCharField( trdObj, fid, pkg.nMsgType);
    fid = env->GetFieldID( clazz, "nUserID", "I");
    env->SetIntField( trdObj, fid, pkg.nUserID);
    fid =env->GetFieldID( clazz, "szUserID", "Ljava/lang/String;");
    std::string s ;
    for(int i=0; i<16; i++){
        s[i] = pkg.szUserID[i];
    }
    jstring szUserID = env->NewStringUTF(s.c_str());
    env->SetObjectField(trdObj, fid, szUserID);

    fid = env->GetFieldID( clazz, "nInfoType", "C");
    env->SetCharField( trdObj, fid, pkg.nInfoType);
    fid = env->GetFieldID( clazz, "nSendHour", "C");
    env->SetCharField( trdObj, fid, pkg.nSendHour);
    fid = env->GetFieldID( clazz, "nSendMin", "C");
    env->SetCharField( trdObj, fid, pkg.nSendMin);

    fid = env->GetFieldID( clazz, "nInfolen", "I");
    env->SetIntField( trdObj, fid, pkg.nInfolen);
    fid =env->GetFieldID( clazz, "Info", "Ljava/lang/String;");
    std::string s2 ;
    for(int i=0; i<(pkg.nInfolen+2); i++){
        s2[i] = pkg.Info[i];
    }
    jstring Info = env->NewStringUTF(s2.c_str());
    env->SetObjectField(trdObj, fid, Info);

    return nType;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_example_sectest_MainActivity_PTDecodePackageFKIJNI(
        JNIEnv* env,
        jobject ,
        jobject trdObj,
        jstring rcvBuf
) {
    TRDFKI pkg;
    uchar buf[1024] = {0};
    int nLen = jstringToChar(env,rcvBuf,buf);
    int nType = PT_DecodePackage(&pkg, buf, nLen, STACK_BD2_RD);

    jfieldID fid;

    jclass clazz = env->GetObjectClass(trdObj);
    if (0 == clazz) {
        printf("GetObjectClass returned 0\n");
        return (-1);
    }

    fid = env->GetFieldID( clazz, "cCmdExecFlag", "C");
    env->SetCharField( trdObj, fid, pkg.cCmdExecFlag);
    fid = env->GetFieldID( clazz, "cFreqSetFlag", "C");
    env->SetCharField( trdObj, fid, pkg.cFreqSetFlag);
    fid = env->GetFieldID( clazz, "nRestrainFlag", "C");
    env->SetCharField( trdObj, fid, pkg.nRestrainFlag);
    fid = env->GetFieldID( clazz, "nWaitHour", "C");
    env->SetCharField( trdObj, fid, pkg.nWaitHour);
    fid = env->GetFieldID( clazz, "nWaitSecond", "C");
    env->SetCharField( trdObj, fid, pkg.nWaitSecond);

    fid =env->GetFieldID( clazz, "cCmdName", "Ljava/lang/String;");
    std::string s ;
    for(int i=0; i<4; i++){
        s[i] = pkg.cCmdName[i];
    }
    jstring cCmdName = env->NewStringUTF(s.c_str());
    env->SetObjectField(trdObj, fid, cCmdName);

    return nType;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_example_sectest_MainActivity_PTDecodePackageHZRJNI(
        JNIEnv* env,
        jobject ,
        jobject trdObj,
        jstring rcvBuf
) {
    TRDHZR pkg;
    uchar buf[1024] = {0};
    int nLen = jstringToChar(env,rcvBuf,buf);
    int nType = PT_DecodePackage(&pkg, buf, nLen, STACK_BD2_RD);

    jfieldID fid;

    jclass clazz = env->GetObjectClass(trdObj);
    if (0 == clazz) {
        printf("GetObjectClass returned 0\n");
        return (-1);
    }

    fid = env->GetFieldID( clazz, "nUserID", "I");
    env->SetIntField( trdObj, fid, pkg.nUserID);
    fid = env->GetFieldID( clazz, "nHZNum", "C");
    env->SetCharField( trdObj, fid, pkg.nHZNum);

    std::string strs;
    for(int i=0;i<5;i++)
    {
        if(i != 0){ strs += ",";}
        int temp=pkg.SendTime[i].nHour;
        strs += std::to_string(temp);
    }
    fid =env->GetFieldID( clazz, "SendTimeHour", "Ljava/lang/String;");
    jstring cCmdName = env->NewStringUTF(strs.c_str());
    env->SetObjectField(trdObj, fid, cCmdName);

    strs = "";
    for(int i=0;i<5;i++)
    {
        if(i != 0){ strs += ",";}
        int temp=pkg.SendTime[i].nMinute;
        strs += std::to_string(temp);
    }
    fid =env->GetFieldID( clazz, "SendTimeMinute", "Ljava/lang/String;");
    cCmdName = env->NewStringUTF(strs.c_str());
    env->SetObjectField(trdObj, fid, cCmdName);

    strs = "";
    for(int i=0;i<5;i++)
    {
        if(i != 0){ strs += ",";}
        int temp=pkg.RespTime[i].nHour;
        strs += std::to_string(temp);
    }
    fid =env->GetFieldID( clazz, "RespTimeHour", "Ljava/lang/String;");
    cCmdName = env->NewStringUTF(strs.c_str());
    env->SetObjectField(trdObj, fid, cCmdName);

    strs = "";
    for(int i=0;i<5;i++)
    {
        if(i != 0){ strs += ",";}
        int temp=pkg.RespTime[i].nMinute;
        strs += std::to_string(temp);
    }
    fid =env->GetFieldID( clazz, "RespTimeMinute", "Ljava/lang/String;");
    cCmdName = env->NewStringUTF(strs.c_str());
    env->SetObjectField(trdObj, fid, cCmdName);

    return nType;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_example_sectest_MainActivity_PTDecodePackageWAAJNI(
        JNIEnv* env,
        jobject ,
        jobject trdObj,
        jstring rcvBuf
) {
    TRDWAA pkg;
    uchar buf[1024] = {0};
    int nLen = jstringToChar(env,rcvBuf,buf);
    int nType = PT_DecodePackage(&pkg, buf, nLen, STACK_BD2_RD);

    jfieldID fid;

    jclass clazz = env->GetObjectClass(trdObj);
    if (0 == clazz) {
        printf("GetObjectClass returned 0\n");
        return (-1);
    }

    fid = env->GetFieldID( clazz, "nMsgType", "C");
    env->SetCharField( trdObj, fid, pkg.nMsgType);
    fid = env->GetFieldID( clazz, "f_Freq", "F");
    env->SetFloatField( trdObj, fid, pkg.f_Freq);
    fid = env->GetFieldID( clazz, "nUserID", "I");
    env->SetIntField( trdObj, fid, pkg.nUserID);

    fid = env->GetFieldID( clazz, "nReportHour", "C");
    env->SetCharField( trdObj, fid, pkg.nReportHour);
    fid = env->GetFieldID( clazz, "nReportMin", "C");
    env->SetCharField( trdObj, fid, pkg.nReportMin);
    fid = env->GetFieldID( clazz, "f_ReportSecond", "F");
    env->SetFloatField( trdObj, fid, pkg.f_ReportSecond);

    fid = env->GetFieldID( clazz, "f_Lat", "F");
    env->SetFloatField( trdObj, fid, pkg.f_Lat);
    fid = env->GetFieldID( clazz, "cLatAspect", "C");
    env->SetCharField( trdObj, fid, pkg.cLatAspect);

    fid = env->GetFieldID( clazz, "f_Lon", "F");
    env->SetFloatField( trdObj, fid, pkg.f_Lon);
    fid = env->GetFieldID( clazz, "cLonAspect", "C");
    env->SetCharField( trdObj, fid, pkg.cLonAspect);

    fid = env->GetFieldID( clazz, "f_Height", "F");
    env->SetFloatField( trdObj, fid, pkg.f_Height);
    fid = env->GetFieldID( clazz, "cHeightUnit", "C");
    env->SetCharField( trdObj, fid, pkg.cHeightUnit);

    return nType;
}

