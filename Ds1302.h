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
* ������ڣ�2016/9/19
* ���뻷����C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* ��ʷ��Ϣ��
*******************************************************************************/
#ifndef __DS1302_H
#define __DS1302_H

#include "Type.h"
/**********************************************��������***************************************************** 
* ��������: void Ds1302Init(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/19
************************************************************************************************************/ 
void Ds1302Init(void);
/*��DS1302дһ���ֽ�*/
static void _wds13byte(u8 _code);
void Ds1302GetTime(void);
u8 Ds1302GetTimeKeep(u8 cmd);
//��1302����ʱ��
void Ds1302SetTime(u8 set_hour, u8 set_min);

void Ds1302SetTime_s(u8 set_hour, u8 set_min, u8 set_sec);
#endif




