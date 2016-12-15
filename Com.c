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
* ������ڣ�2016/7/11
* ���뻷����C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* ��ʷ��Ϣ��
*******************************************************************************/

#include "Com.h"
#include "Delay.h"
#include "Time.h"
#include "wifi.h"
/*��������*/
void ComInit(void) {
    PD_DDR_DDR5 = 1;
    PD_CR1_C15 = 1;
    PD_CR2_C25 = 1;

    UART2_CR1=0x00;
    UART2_CR2=0x00;
    UART2_CR3=0x00; 
    UART2_BRR2=0x02;//02 0a
    UART2_BRR1=0x68;//68 08
    UART2_CR2=0x2c;//������գ����ͣ��������ж�
}



u8 data;
#pragma vector=0x17
__interrupt void UART2_RX_IRQHandler(void)
{
    data = UART2_DR;
    /*�ȴ����ݽ������*/
    while((UART2_SR & 0x80) == 0x00);
    uart_receive_input(data);

    return;
}


