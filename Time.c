/*******************************************************************************
* Copyright 2016 HangZhou ���ݾ���Ƽ����޹�˾
* All right reserved
*
* �ļ����ƣ�time,c
*
* �ļ���ʶ��ds
* ժ    Ҫ����ʱ������������
*
*
* ��ǰ�汾��v1.0
* ��    �ߣ�lhb Steven
* ������ڣ�2016/3/18
* ���뻷����C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* ��ʷ��Ϣ��
*******************************************************************************/
#include "Time.h"

void TimerInit(void) {
    TIM4_IER = 0x00;//       
    TIM4_EGR = 0x01;// 
    TIM4_PSCR = 0x0f;// ������ʱ��=��ʱ��/128=16MHZ/128
    TIM4_ARR = 0xff;// �趨��װ��ʱ�ļĴ���ֵ��255�����ֵ
    //TIM4_CNTR = 0x00;// �趨�������ĳ�ֵ
    // ��ʱ����=(ARR+1)*64=16320uS
    TIM4_IER = 0x01;//   
    TIM4_CR1 = 0x01;  
}

static u16 sec_flag = 0;

u16 TimerGetSec(void) {
    return sec_flag;
}

void TimerSetSec(u8 data) {
    sec_flag = data;
}

#pragma vector=0x19
__interrupt void TIM4_UPD_OVF_IRQHandler(void)
{
    static u16 t4_count = 0;
    TIM4_SR = 0x00;
    if(t4_count < 50) {
        t4_count++;
    } else {
        t4_count = 0;
        sec_flag = 0x01;
    }
    return;
}



