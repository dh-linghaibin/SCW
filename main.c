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
    wifi_protocol_init();//wifi��ʼ��
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
