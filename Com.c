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
* 完成日期：2016/7/11
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
*******************************************************************************/

#include "Com.h"
#include "Delay.h"
#include "Time.h"
#include "wifi.h"
/*串口数据*/
void ComInit(void) {
    PD_DDR_DDR5 = 1;
    PD_CR1_C15 = 1;
    PD_CR2_C25 = 1;

    UART2_CR1=0x00;
    UART2_CR2=0x00;
    UART2_CR3=0x00; 
    UART2_BRR2=0x02;//02 0a
    UART2_BRR1=0x68;//68 08
    UART2_CR2=0x2c;//允许接收，发送，开接收中断
}



u8 data;
#pragma vector=0x17
__interrupt void UART2_RX_IRQHandler(void)
{
    data = UART2_DR;
    /*等待数据接受完成*/
    while((UART2_SR & 0x80) == 0x00);
    uart_receive_input(data);

    return;
}


