/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name��     pt_config.h
* Abstract�� protocol config 
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

#ifndef H_PT_CONFIG_H
#define H_PT_CONFIG_H

#define PT_ENABLE_BD2_RD            //ʹ��BD2 RDSSЭ��
//#define PT_ENABLE_BD2_RN            //ʹ��BD2 RNSSЭ��
//#define PT_ENABLE_BD2_SPECIAL       //ʹ��BD2 ����Э��
//#define PT_ENABLE_BD2_EXPRESS       //ʹ��BD2 ר��Э��
//#define PT_ENABLE_NW                //ʹ��ŵά�Զ���Э�飬������һ���Զ���
//#define PT_ENABLE_BD1               //ʹ��BD1Э�� ��һ���Զ���Э��
//#define PT_ENABLE_GPS               //ʹ��GPSЭ�� 
//#define PT_ENABLE_GT06              //ʹ��GT06Э��

//���֧�ֵ�Э��ջ��
#define  PT_MAX_STACK_NUM    	  	 1
//Э��ջ�������ݻ����С
#define  PT_MAX_BUFFER_LEN           1280

//protocol define
//protocol define
#ifdef __cplusplus
    extern "C"
    {
        #include "pt_define.h"
    };
#else
    #include "pt_define.h"
#endif

#endif //end  H_PT_CONFIG_H  define