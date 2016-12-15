/*******************************************************************************
* Copyright 2016 HangZhou ���ݾ���Ƽ����޹�˾
* All right reserved
*
* �ļ����ƣ�
*
* �ļ���ʶ��
* ժ    Ҫ��
*
*
* ��ǰ�汾��
* ��    �ߣ�lhb Steven
* ������ڣ�2016/8/23
* ���뻷����C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* ��ʷ��Ϣ��
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

//3���õ�ָ����ַ�ϵ�һ���ֽڻ���

#define  MEM_B( x )  ( *( (byte *) (x) ) )
#define  MEM_W( x )  ( *( (word *) (x) ) )


#define INTEN   asm("rim");//open interrupt
#define INTOFF  asm("sim");//closs interrupt
#define MCUSLEEP asm("halt");//sleep

//include file
#include "iostm8s105k4.h"

#endif

