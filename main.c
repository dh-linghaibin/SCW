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
#include "Type.h"
#include "Eeprom.h"
#include "Sys.h"
#include "Ht1621b.h"
#include "Ds1302.h"
#include "Button.h"
#include "Menu.h"
#include "Time.h"
#include "Level.h"
#include "Moto.h"
#include "wifi.h"
#include "Com.h"

int main( void ) {
    SysInit();
    EeepromInit();
    ButtonInit();
    ComInit();
    Ht1621bInit();
    Ds1302Init();
    TimerInit();
    LevelInit();
    wifi_protocol_init();//wifi初始化
    MotoInit();
    MenuInit();
    INTEN
    while(1) {
        MenuBut(ButtonRead());
        if(TimerGetSec() == 0x01) {
            TimerSetSec(0x00);
            MenuUpdata();
        }
        Ht1621Service();
        ButtonFlash();
        wifi_uart_service();
    }
}
