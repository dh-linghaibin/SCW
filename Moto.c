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
* 完成日期：2016/9/22
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
*******************************************************************************/
#include "Moto.h"

#define MOTO_M1 PD_ODR_ODR4

/**********************************************函数定义***************************************************** 
* 函数名称: void MotoInit(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/22
************************************************************************************************************/ 
void MotoInit(void) { 
    PD_DDR_DDR4 = 1;
    PD_CR1_C14 = 1;
    PD_CR2_C24 = 1;
    
    MOTO_M1 = MOTO_OFF;
}

/**********************************************函数定义***************************************************** 
* 函数名称: void MotoSet(u8 cmd) 
* 输入参数: u8 cmd 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/22
************************************************************************************************************/ 
void MotoSet(u8 cmd) { 
    MOTO_M1 = cmd;
}





