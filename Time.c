/*******************************************************************************
* Copyright 2016 HangZhou 杭州久天科技有限公司
* All right reserved
*
* 文件名称：time,c
*
* 文件标识：ds
* 摘    要：定时器，用于休眠
*
*
* 当前版本：v1.0
* 作    者：lhb Steven
* 完成日期：2016/3/18
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
*******************************************************************************/
#include "Time.h"

void TimerInit(void) {
    TIM4_IER = 0x00;//       
    TIM4_EGR = 0x01;// 
    TIM4_PSCR = 0x0f;// 计数器时钟=主时钟/128=16MHZ/128
    TIM4_ARR = 0xff;// 设定重装载时的寄存器值，255是最大值
    //TIM4_CNTR = 0x00;// 设定计数器的初值
    // 定时周期=(ARR+1)*64=16320uS
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



