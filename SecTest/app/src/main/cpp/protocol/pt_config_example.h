/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name：     pt_config.h
* Abstract： protocol config 
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

#ifndef H_PT_CONFIG_H
#define H_PT_CONFIG_H

#define PT_ENABLE_BD2_RD            //使能BD2 RDSS协议
#define PT_ENABLE_BD2_RN            //使能BD2 RNSS协议
#define PT_ENABLE_BD2_SPECIAL       //使能BD2 特殊协议
#define PT_ENABLE_BD2_EXPRESS       //使能BD2 专用协议
#define PT_ENABLE_NW                //使能诺维自定义协议，不包含一代自定义
#define PT_ENABLE_BD1               //使能BD1协议 含一代自定义协议
#define PT_ENABLE_GPS               //使能GPS协议 
#define PT_ENABLE_GT06              //使能GT06协议

//最多支持的协议栈数
#define  PT_MAX_STACK_NUM    	  	 1
//协议栈接收数据缓存大小
#define  PT_MAX_BUFFER_LEN           1280

//protocol define
//protocol define
#ifdef __cplusplus
    extern "C"
    {
        #include "../../../../Common/protocol/pt_define.h"
    };
#else
    #include "../../../../Common/protocol/pt_define.h"
#endif

#endif //end  H_PT_CONFIG_H  define