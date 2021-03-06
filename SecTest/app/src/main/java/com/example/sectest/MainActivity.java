package com.example.sectest;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;
import java.lang.reflect.Field;
import java.util.HashMap;
import java.util.Map;
import java.io.UnsupportedEncodingException;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        int test_a = addJNI(2,3);
        String rcvBuf = "$BDICI,2097151,00000000,2097151,0,9999.0,0,N,0*25\r\n";
        //int test_b = PTDecodeTypeJNI(rcvBuf);

        //TrdICI trdICI = new TrdICI();
        //int test_ICI = PTDecodePackageICIJNI(trdICI,rcvBuf);

        Map objectTrd = PTDecodePackage(rcvBuf);
//        int bbb = objectTrd.nUserID;
        //TrdICI trdICI = new TrdICI();
        //int test_ICI = PTDecodePackageICIJNI(trdICI,RcvBuf);
        //TrdICI test_ICA = PTDecodePackageICIJNI2(trdICI,RcvBuf);
        int ddd = 1;
        ddd = ddd+1;
    }

    public Map<String,String> PTDecodePackage(String rcvBuf){
        //rcvBuf = "$BDICI,2097151,00000000,2097151,0,9999.0,0,N,0*25\r\n";
        int nType = - 1;
        Map<String, String> map = new HashMap();
        nType = PTDecodeTypeJNI(rcvBuf);
        map.put("nTrdType", String.valueOf(nType));

        switch (nType){
            case SSD.BD2_RD_BSI:
            {
                TrdBSI trdObj = new TrdBSI();
                int decType = PTDecodePackageBSIJNI(trdObj,rcvBuf);
                if(decType >= 0) {
                    map.put("nInboudBeam", String.valueOf(trdObj.nInboudBeam));
                    map.put("nTmDifBeam", String.valueOf(trdObj.nTmDifBeam));
                    map.put("nPower", String.valueOf(trdObj.nPower));
                }
            }
            break;
            case SSD.BD2_RD_BSS:
                break;
            case SSD.BD2_RD_CXA:
                break;
            case SSD.BD2_RD_DSA:
                break;
            case SSD.BD2_RD_DWA:
                break;
            case SSD.BD2_RD_DWR:
            {
                TrdDWR trdObj = new TrdDWR();
                int decType = PTDecodePackageDWRJNI(trdObj,rcvBuf);
                if(decType >= 0) {
                    map.put("nPosType", String.valueOf(trdObj.nPosType));
                    map.put("nUserID", String.valueOf(trdObj.nUserID));
                    map.put("nHour", String.valueOf(trdObj.nHour));
                    map.put("nMinute", String.valueOf(trdObj.nMinute));
                    map.put("f_Second", String.valueOf(trdObj.f_Second));
                    map.put("db_Lat", String.valueOf(trdObj.db_Lat));
                    map.put("cLatDir", String.valueOf(trdObj.cLatDir));
                    map.put("db_Lon", String.valueOf(trdObj.db_Lon));

                    map.put("cLonDir", String.valueOf(trdObj.cLonDir));
                    map.put("db_Height", String.valueOf(trdObj.db_Height));
                    map.put("cHunit", String.valueOf(trdObj.cHunit));
                    map.put("f_Kesi", String.valueOf(trdObj.f_Kesi));
                    map.put("cKesiUnit", String.valueOf(trdObj.cKesiUnit));
                    map.put("nPrecision", String.valueOf(trdObj.nPrecision));
                    map.put("cUrgencyPos", String.valueOf(trdObj.cUrgencyPos));
                    map.put("cMutilpleValue", String.valueOf(trdObj.cMutilpleValue));
                    map.put("cLocaType", String.valueOf(trdObj.cLocaType));
                }
            }
            break;
            case SSD.BD2_RD_FKI:
            {
                TrdFKI trdObj = new TrdFKI();
                int decType = PTDecodePackageFKIJNI(trdObj,rcvBuf);
                if(decType >= 0) {
                    map.put("cCmdName", String.valueOf(trdObj.cCmdName));
                    map.put("cCmdExecFlag", String.valueOf(trdObj.cCmdExecFlag));
                    map.put("cFreqSetFlag", String.valueOf(trdObj.cFreqSetFlag));
                    map.put("nRestrainFlag", String.valueOf(trdObj.nRestrainFlag));
                    map.put("nWaitHour", String.valueOf(trdObj.nWaitHour));
                    map.put("nWaitSecond", String.valueOf(trdObj.nWaitSecond));
                }
            }
            break;
            case SSD.BD2_RD_GXM:
                break;
            case SSD.BD2_RD_ICA:
                break;
            case SSD.BD2_RD_ICI:
            {
                TrdICI trdObj = new TrdICI();
                int decType = PTDecodePackageICIJNI(trdObj,rcvBuf);
                if(decType >= 0) {
                    map.put("nUserID", String.valueOf(trdObj.nUserID));
                    map.put("nSerialNum", String.valueOf(trdObj.nSerialNum));
                    map.put("nBrdID", String.valueOf(trdObj.nBrdID));
                    map.put("nUserType", String.valueOf(trdObj.nUserType));
                    map.put("f_Freq", String.valueOf(trdObj.f_Freq));
                    map.put("nCommLevel", String.valueOf(trdObj.nCommLevel));
                    map.put("cEncryptFlag", String.valueOf(trdObj.cEncryptFlag));
                    map.put("nSubuserNum", String.valueOf(trdObj.nSubuserNum));
                }
            }
            break;
            case SSD.BD2_RD_ICZ:
            {
                TrdICZ trdObj = new TrdICZ();
                int decType = PTDecodePackageICZJNI(trdObj,rcvBuf);
                if(decType >= 0) {
                    map.put("nSubuserNum", String.valueOf(trdObj.nSubuserNum));
                    map.put("nSubuserID", String.valueOf(trdObj.nSubuserID));
                }
            }
            break;
            case SSD.BD2_RD_JMS:
                break;
            case SSD.BD2_RD_KLS:
                break;
            case SSD.BD2_RD_KLT:
                break;
            case SSD.BD2_RD_LZM:
                break;
            case SSD.BD2_RD_HZR:
            {
                TrdHZR trdObj = new TrdHZR();
                int decType = PTDecodePackageHZRJNI(trdObj,rcvBuf);
                if(decType >= 0) {
                    map.put("nUserID", String.valueOf(trdObj.nUserID));
                    map.put("nHZNum", String.valueOf(trdObj.nHZNum));

                    map.put("SendTimeHour", String.valueOf(trdObj.SendTimeHour));
                    map.put("SendTimeMinute", String.valueOf(trdObj.SendTimeMinute));
                    map.put("RespTimeHour", String.valueOf(trdObj.RespTimeHour));
                    map.put("RespTimeMinute", String.valueOf(trdObj.RespTimeMinute));
                }
            }
            break;
            case SSD.BD2_RD_TXA:
                break;
            case SSD.BD2_RD_TXR:
            {
                TrdTXR trdObj = new TrdTXR();
                int decType = PTDecodePackageTXRJNI(trdObj,rcvBuf);
                if(decType >= 0) {
                    map.put("nMsgType", String.valueOf(trdObj.nMsgType));
                    map.put("nUserID", String.valueOf(trdObj.nUserID));
                    map.put("szUserID", String.valueOf(trdObj.szUserID));
                    map.put("nInfoType", String.valueOf(trdObj.nInfoType));
                    map.put("nSendHour", String.valueOf(trdObj.nSendHour));
                    map.put("nSendMin", String.valueOf(trdObj.nSendMin));
                    map.put("nInfolen", String.valueOf(trdObj.nInfolen));
                    map.put("Info", String.valueOf(trdObj.Info));
                }
            }
            break;
            case SSD.BD2_RD_WAA:
            {
                TrdWAA trdObj = new TrdWAA();
                int decType = PTDecodePackageWAAJNI(trdObj,rcvBuf);
                if(decType >= 0) {
                    map.put("nMsgType", String.valueOf(trdObj.nMsgType));
                    map.put("f_Freq", String.valueOf(trdObj.f_Freq));
                    map.put("nUserID", String.valueOf(trdObj.nUserID));
                    map.put("nReportHour", String.valueOf(trdObj.nReportHour));
                    map.put("nReportMin", String.valueOf(trdObj.nReportMin));
                    map.put("f_ReportSecond", String.valueOf(trdObj.f_ReportSecond));

                    map.put("f_Lat", String.valueOf(trdObj.f_Lat));
                    map.put("cLatAspect", String.valueOf(trdObj.cLatAspect));
                    map.put("f_Lon", String.valueOf(trdObj.f_Lon));
                    map.put("cLonAspect", String.valueOf(trdObj.cLonAspect));
                    map.put("f_Height", String.valueOf(trdObj.f_Height));
                    map.put("cHeightUnit", String.valueOf(trdObj.cHeightUnit));
                }
            }
            break;
            case SSD.BD2_RD_WBA:
                break;
            case SSD.BD2_RD_ZHS:
                break;
            default:
                break;
        }

        return map;
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */

    public native String stringFromJNI();
    public native int addJNI(int a, int b);
    public native int PTDecodeTypeJNI(String rcvBuf);
    public native String PTEncodePackageICAJNI(int nCmdType, int nFrameNum);
    public native String PTEncodePackageDWAJNI(int nUserID, char cUrgencyPos, char uc_MHMode, char cLocaType, double db_HeightData, double db_AntHigh, double db_AirPressure, double db_Temperature, double db_Freq);
    public native String PTEncodePackageTXAJNI(int nUserID, char nCommMode, char nInfoType, String Info);
    public native String PTEncodePackageCXAJNI(int nUserID, char nQueryType, char nQueryMode);
    public native String PTEncodePackageWAAJNI(int nUserID, char nMsgType, float f_Freq, char nReportHour, char nReportMin, float f_ReportSecond, float f_Lat, char cLatAspect, float f_Lon, char cLonAspect, float f_Height, char cHeightUnit);
    public native String PTEncodePackageWBAJNI(int nUserID, char cHeightflag, double f_AnteHeight, float f_Freq);

    public class TrdICI
    {
        //用户地址（ID号）
        protected int nUserID;
        //序列号
        protected int nSerialNum;
        //通播地址
        protected int nBrdID;
        //用户特征值
        protected char nUserType;
        //服务频度（秒）
        protected float f_Freq;
        //通信等级（1-4）
        protected char nCommLevel;
        //加密标志（E/N）
        protected char cEncryptFlag;
        //下属用户数
        protected int nSubuserNum;
    };
    public native int PTDecodePackageICIJNI(TrdICI trdObj,String rcvBuf);

    public class TrdBSI
    {
        //响应波束号
        protected char nInboudBeam;
        //时差波束号
        protected char nTmDifBeam;
        //波束信号功率
        protected String nPower;
    };
    public native int PTDecodePackageBSIJNI(TrdBSI trdObj,String rcvBuf);

    public class TrdICZ
    {
        //总下属用户数
        protected int nSubuserNum;
        //下属用户ID号（一包协议最多有40个）
        protected String nSubuserID;
    };
    //TODO::此方法还没有处理完成,后面待处理
    public native int PTDecodePackageICZJNI(TrdICZ trdObj,String rcvBuf);

    public class TrdDWR
    {
        //定位信息类型（1-3）
        protected char nPosType;
        //用户地址（ID号）
        int nUserID;
        //定位时刻（UTC时间）
        protected char nHour;
        protected char nMinute;
        protected float f_Second;
        //纬度
        protected double db_Lat;
        //纬度方向（N/S）
        protected char cLatDir;
        //经度
        protected double db_Lon;
        //经度方向（E/W）
        protected char cLonDir;
        //大地高
        protected double db_Height;
        //大地高单位（M）
        protected char cHunit;
        //高程异常
        protected float f_Kesi;
        //高程异常单位（M）
        protected char cKesiUnit;
        //精度指示（0-1）
        protected char nPrecision;
        //紧急定位指示（A/V）
        protected char cUrgencyPos;
        //多值解指示（A/V）
        protected char cMutilpleValue;
        //高程类型指示（H/L）
        protected char cLocaType;
    };
    public native int PTDecodePackageDWRJNI(TrdDWR trdObj,String rcvBuf);

    public class TrdTXR
    {
        //信息类别（1-5）
        protected char nMsgType;
        //用户地址（ID号）
        int nUserID;
        protected String szUserID;
        //电文形式（0：汉字  1：代码 2:混发 3:位置上报）
        protected char nInfoType;
        //发送时间
        protected char nSendHour;
        protected char nSendMin;
        //通信电文内容
        protected int nInfolen;
        protected String Info;
    };
    public native int PTDecodePackageTXRJNI(TrdTXR trdObj,String rcvBuf);

    public class TrdFKI
    {
        //指令名称
        protected String cCmdName;
        //指令执行情况（Y/N）
        protected char cCmdExecFlag;
        //频度设置指示（Y/N）
        protected char cFreqSetFlag;
        //发射抑制指示（0-3）
        protected char nRestrainFlag;
        //等待时间
        protected char nWaitHour;
        protected char nWaitSecond;
    };
    public native int PTDecodePackageFKIJNI(TrdFKI trdObj,String rcvBuf);

    public class TrdHZR
    {
        //用户地址（ID号）
        protected int nUserID;
        //回执数（0-5 0：无回执）
        protected char nHZNum;
        //发信时间（时分）
        protected String SendTimeHour;
        protected String SendTimeMinute;
        //回执时间（时分）
        protected String RespTimeHour;
        protected String RespTimeMinute;
    };
    //TODO::此方法还没有处理完成,后面待处理
    public native int PTDecodePackageHZRJNI(TrdHZR trdObj,String rcvBuf);

    public class TrdWAA
    {
        //信息类型（0-1  0：用户设备就收的位置报告  1：用户设备发送的位置报告）
        protected char nMsgType;
        //报告频度（秒  0：单次位置报告）
        protected float f_Freq;
        //用户地址（ID号）
        protected int nUserID;
        //位置报告时间
        protected char nReportHour;
        protected char nReportMin;
        protected float f_ReportSecond;
        //纬度
        protected float f_Lat;
        //纬度方向
        protected char cLatAspect;
        //经度
        protected float f_Lon;
        //经度方向
        protected char cLonAspect;
        //高程值
        protected float f_Height;
        //高程单位（米）
        protected char cHeightUnit;
    };
    public native int PTDecodePackageWAAJNI(TrdWAA trdObj,String rcvBuf);


    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
}
