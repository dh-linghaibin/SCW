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
* 完成日期：2016/8/23
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
*******************************************************************************/

#ifndef TYPE_H
#define TYPE_H

/*Data Types Reconstruction*/
typedef unsigned char    u8;
typedef unsigned int     u16;
typedef unsigned long    u32;
typedef signed char    s8;
typedef signed int     s16;
typedef signed long    s32;


#define BIT(x) (1 << (x))

//3，得到指定地址上的一个字节或字

#define  MEM_B( x )  ( *( (byte *) (x) ) )
#define  MEM_W( x )  ( *( (word *) (x) ) )


#define INTEN   asm("rim");//open interrupt
#define INTOFF  asm("sim");//closs interrupt
#define MCUSLEEP asm("halt");//sleep

//include file
#include "iostm8s105k4.h"

#endif

