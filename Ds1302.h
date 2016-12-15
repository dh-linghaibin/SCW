/*******************************************************************************
* Copyright 2016 HangZhou 杭州久天科技有限公司
* All right reserved
*
* 文件名称：
*
* 文件标识：
* 摘    要：
*
*
* 当前版本：
* 作    者：lhb Steven
* 完成日期：2016/9/19
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
*******************************************************************************/
#ifndef __DS1302_H
#define __DS1302_H

#include "Type.h"
/**********************************************函数定义***************************************************** 
* 函数名称: void Ds1302Init(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/19
************************************************************************************************************/ 
void Ds1302Init(void);
/*向DS1302写一个字节*/
static void _wds13byte(u8 _code);
void Ds1302GetTime(void);
u8 Ds1302GetTimeKeep(u8 cmd);
//给1302设置时间
void Ds1302SetTime(u8 set_hour, u8 set_min);

void Ds1302SetTime_s(u8 set_hour, u8 set_min, u8 set_sec);
#endif




