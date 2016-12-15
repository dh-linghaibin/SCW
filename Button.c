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
#include "Button.h"

#define BUT_POW     PA_IDR_IDR1
#define BUT_DOWN    PA_IDR_IDR2
#define BUT_UP      PB_IDR_IDR4
#define BUT_SET     PB_IDR_IDR5

#define BUT_LED     PC_ODR_ODR1

static u8 but_flash = 0;
/**********************************************函数定义***************************************************** 
* 函数名称: void ButtonInit(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/19
************************************************************************************************************/ 
void ButtonInit(void) { 
    PB_DDR_DDR5 = 0;
    PB_CR1_C15 = 1;
    PB_CR2_C25 = 0;
    
    PB_DDR_DDR4 = 0;
    PB_CR1_C14 = 1;
    PB_CR2_C24 = 0;
    
    PA_DDR_DDR2 = 0;
    PA_CR1_C12 = 1;
    PA_CR2_C22 = 0;
    
    PA_DDR_DDR1 = 0;
    PA_CR1_C11 = 1;
    PA_CR2_C21 = 0;
    
    PC_DDR_DDR1 = 1;
    PC_CR1_C11 = 1;
    PC_CR2_C21 = 0;
    
//    TIM2_IER = 0x00;         /*PWM中断主要在这里进行调整*/
//	TIM2_CCMR3 = 0x60;  //pwm模式2
//	//TIM2_CCMR2 = 0x78;  //pwm模式2
//	//TIM2_CCMR1 = 0x78;  //pwm模式2
//	
//	//TIM2_CCER1 |= 0x13;
//	TIM2_CCER2 = 0x07;  		//CC2引脚使能
//	
//	TIM2_PSCR = 0x05;         //8^1 分频为1M 
//	TIM2_ARRH = 0x00;
//	TIM2_ARRL = 0xFE;        //每500us中断一次1F4-2k,100-10k
//	
//	TIM2_CCR3H = 0x00;
//	TIM2_CCR3L = 0X00;        //设置占空比，即计数器到多少后
//	
//	TIM2_CR1 = 0x04;
//	TIM2_CR1 |= BIT(0);              //计数器使能，开始计数
}
/**********************************************函数定义***************************************************** 
* 函数名称: u8 ButtonRead(void) 
* 输入参数: void 
* 返回参数: u8  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/19
************************************************************************************************************/ 
#define BUT_TIME 100
u8 ButtonRead(void) {
    static u16 but_pow_count = 0;
    static u16 but_set_count = 0;
    static u8 but_set_small = 0;
    static u16 but_down_count = 0;
    static u16 but_up_count = 0;
    //电源按钮
    if(BUT_POW == 0) {
        if(but_pow_count < 2000) {
            but_pow_count++;
            if(but_pow_count == BUT_TIME) {
                return 0x02;
            }
        } else {
            //but_pow_count = 0;
        }
    } else {
        //按键放开后才可以进行操作
        if(but_pow_count > BUT_TIME) {
            but_pow_count = 0;
            return 0x01;
        }
        but_pow_count = 0; 
    }
    //设置按钮
    if(BUT_SET == 0) {
        if(but_set_small < 30) {
            but_set_small++;
        } else {
            but_set_small = 0;
            if(but_set_count < 65000) {
                but_set_count++;
                if(but_set_count == 8000) {
                    return 0x12;
                }
            }
        }
    } else {
        if( (but_set_count > 30) && (but_set_count < 7900) ){
            but_set_count = 0;
            return 0x11;
        }
        but_set_count = 0;
    }
    //加按钮
    if(BUT_DOWN == 0) {
        if(but_down_count < 2000) {
            but_down_count++;
            if(but_down_count == BUT_TIME) {
                return 0x22;
            }
        } else {
            
        }
    } else {
        if(but_down_count > BUT_TIME) {
            but_down_count = 0;
            return 0x21;
        }
        but_down_count = 0;
    }
    //减按钮
    if(BUT_UP == 0) {
        if(but_up_count < 2000) {
            but_up_count++;
            if(but_up_count == BUT_TIME) {
                return 0x32;
            }
        } else {
            
        }
    } else {
        if(but_up_count > BUT_TIME) {
            but_up_count = 0;
            return 0x31;
        }
        but_up_count = 0;
    }
    return 0x00;
}
/**********************************************函数定义***************************************************** 
* 函数名称: void ButtonFlash(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/22
************************************************************************************************************/ 
void ButtonFlash(void) { 
    static u16 T1msFlg = 0;
    static u8 run_Falg = 0;
    static u8 Duty_Val = 0;
    if(but_flash == 0) {
        T1msFlg++;
        if(run_Falg == 0) {
            if(Duty_Val < 220) {
                if(T1msFlg >= 200) {
                    T1msFlg = 0;
                    Duty_Val++;
                    TIM2_CCR3L = Duty_Val;
                }
            } else {
                run_Falg = 1;
                T1msFlg = 0;
            }
        } else if(run_Falg == 1) {
            if(T1msFlg >= 3000) {
                run_Falg = 2;
            }
        } else if(run_Falg == 2) {
            if(Duty_Val > 0) {
                if(T1msFlg >= 200) {
                    T1msFlg = 0;
                    Duty_Val--;
                    TIM2_CCR3L = Duty_Val;
                }
            } else {
                run_Falg = 3;
                T1msFlg = 0;
            }
        } else if(run_Falg == 3) {
            if(T1msFlg >= 3000) {
                run_Falg = 0;
            }
        }
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void ButtonSetFlash(u8 cmd) 
* 输入参数: u8 cmd 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/22
************************************************************************************************************/ 
void ButtonSetFlash(u8 cmd) { 
    if(cmd == 0) {
        but_flash = 1;
        BUT_LED = 0;
        //TIM2_CCR3L = 255;
    } else {
        but_flash = 0;
        BUT_LED = 1;
    }
}










