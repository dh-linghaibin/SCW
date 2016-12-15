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
* ������ڣ�2016/9/21
* ���뻷����C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* ��ʷ��Ϣ��
*******************************************************************************/
#include "Menu.h"
#include "Ht1621b.h"
#include "Ds1302.h"
#include "Button.h"
#include "Eeprom.h"
#include "Moto.h"
#include "Level.h"
#include "wifi.h"
#include "Delay.h"

menu menu_n;
/**********************************************��������***************************************************** 
* ��������: void MenuInit(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/21
************************************************************************************************************/ 
void MenuInit(void) {
    
    if(EepromRead(10) == 0x55) {
        menu_n.time_time = EepromRead(0x11);
        menu_n.tume_to = EepromRead(0x12);
        menu_n.time_run = EepromRead(0x13);
        menu_n.time_stop = EepromRead(0x14);
        menu_n.conti_run_time_min = EepromRead(0x17);
        menu_n.conti_run_time = EepromRead(0x15);
        menu_n.conti_run_time |= (EepromRead(0x16) << 8);
        
        menu_n.time_time_min = EepromRead(0x18);
        menu_n.time_to_min = EepromRead(0x19);
    } else {
        EepromWrite(10,0x55);
        MenuSetWork(8,20,2,8,0,0);
    }
    //menu_n.power = EepromRead(0x20);
    
    Ds1302GetTime();
    Ht1622bSetTime(Ds1302GetTimeKeep(2),Ds1302GetTimeKeep(1));
    
    Ht1662bSetConterTime(menu_n.time_time,menu_n.tume_to,
                            menu_n.time_run,menu_n.time_stop);
    ButtonSetFlash(menu_n.power);
    Ht1621bPowerSet(menu_n.power);
    
    Ht1621bFlashMoto(3);
    
    Ht1621Setwifi(1);
    Ht1621SetLevel(1);
    Ht1621bSetWork(1,1);

    menu_n.last_minute = Ds1302GetTimeKeep(1);//����
    
    menu_n.power = 1;
    ButtonSetFlash(1);
    Ht1621bPowerSet(1);
    //�رյ��
    MotoSet(0);
    menu_n.menu_set= 0;
    
    MenuSetPower(1);
    
    menu_n.uptime = 0;
    //�ͼ��
    if(LevelRead() == 0x00) {
        menu_n.level = 0;
    } else {
        menu_n.level = 0;
    }
}
/**********************************************��������***************************************************** 
* ��������: void MenuGetPower(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/23
************************************************************************************************************/ 
u8 MenuGetPower(void) { 
    return menu_n.power;
}
/**********************************************��������***************************************************** 
* ��������: u8 MenuGetLevel(void) 
* �������: void 
* ���ز���: u8  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/23
************************************************************************************************************/ 
u8 MenuGetLevel(void) { 
    if(menu_n.level == 0) {
        return 0;
    } else {
        return 1;
    }
}
/**********************************************��������***************************************************** 
* ��������: void MenuSetPower(u8 cmd) 
* �������: u8 cmd 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/23
************************************************************************************************************/ 
void MenuSetPower(u8 cmd) { 
    if(cmd == 0){
        menu_n.power = 0;
        ButtonSetFlash(1);
        Ht1621bPowerSet(1);
        //�رյ��
        MotoSet(0);
        menu_n.menu_set= 0;
       // menu_n.run_minute = 0;
    } else {
        menu_n.power = 1;
        ButtonSetFlash(0);
        Ht1621bPowerSet(0);
        
    }
    menu_n.uptime = 0;
    menu_n.run_minute = 0;
}
/**********************************************��������***************************************************** 
* ��������: void MenuSetWork(u8 start,u8 end,u8 run,u8 stop) 
* �������: u8 start,u8 end,u8 run,u8 stop 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/23
************************************************************************************************************/ 
void MenuSetWork(u8 start,u8 end,u8 run,u8 stop,u8 start_min,u8 end_min) { 
    menu_n.time_time = start;
    menu_n.tume_to = end;
    menu_n.time_run = run;
    menu_n.time_stop = stop;
    menu_n.time_time_min = start_min;
    menu_n.time_to_min = end_min;
    
    EepromWrite(0x11,menu_n.time_time);
    EepromWrite(0x12,menu_n.tume_to);
    EepromWrite(0x13,menu_n.time_run);
    EepromWrite(0x14,menu_n.time_stop);
    
    EepromWrite(0x18,menu_n.time_time_min);
    EepromWrite(0x19,menu_n.time_to_min);
    menu_n.run_minute = 0;
    Ht1662bSetConterTime(menu_n.time_time,menu_n.tume_to,
                            menu_n.time_run,menu_n.time_stop);
}
/**********************************************��������***************************************************** 
* ��������: u8 MenuGetWorkTime(u8 cmd) 
* �������: u8 cmd 
* ���ز���: u8  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/26
************************************************************************************************************/ 
u8 MenuGetWorkTime(u8 cmd) { 
    u8 time  = 0;
    if(cmd == 0) {
        time = menu_n.time_time;
    } else if(cmd == 1) {
        time = menu_n.tume_to;
    } else if(cmd == 2) {
        time =  menu_n.time_run;
    } else {
        time = menu_n.time_stop;
    } 
    return time;
} 
/**********************************************��������***************************************************** 
* ��������: u16 MenuGetRunTime(void) 
* �������: void 
* ���ز���: u16  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/27
************************************************************************************************************/ 
u16 MenuGetRunTime(void) { 
    return menu_n.conti_run_time;
}
/**********************************************��������***************************************************** 
* ��������: void MenuBut(u8 cmd) 
* �������: u8 cmd 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/21
************************************************************************************************************/ 
void MenuBut(u8 cmd) { 
    switch( cmd ) {
    case 0x01://����
        Ht1621bMusicSet(10);
        if(menu_n.power == 0) {
            menu_n.power = 1;
        } else {
            menu_n.power = 0;
        }
        all_data_update();//�ϴ�����
        MenuSetPower(menu_n.power);
    break;
    case 0x21://�Ӱ�ť
        menu_n.time_out = 0;
        Ht1621bMusicSet(10);
        switch( menu_n.menu_set ) {
            case 0:
                
            break;
            case 1://����Сʱ
                if(menu_n.time_hour < 23) {
                    menu_n.time_hour++;
                } else {
                    menu_n.time_hour = 0;
                }
            break;
            case 2://���÷���
                if(menu_n.time_minnte < 59) {
                    menu_n.time_minnte++;
                } else {
                    menu_n.time_minnte = 0;
                }
            break;
            case 3://���ÿ�ʼʱ��
                if(menu_n.time_time < 23) {
                    menu_n.time_time++;
                } else {
                    menu_n.time_time = 0;
                }
            break;
            case 4://���ùر�ʱ��
                if(menu_n.tume_to < 23) {
                    menu_n.tume_to++;
                } else {
                    menu_n.tume_to = 0;
                }
            break;
            case 5://��������ʱ��
                if(menu_n.time_run < 59) {
                    menu_n.time_run++;
                } else {
                    menu_n.time_run = 0;
                }
            break;
            case 6://����ֹͣʱ��
                if(menu_n.time_stop < 59) {
                    menu_n.time_stop++;
                } else {
                    menu_n.time_stop = 0;
                }
            break;
            default:
            break;
         }
    break;
    case 0x31://����ť
        menu_n.time_out = 0;
        Ht1621bMusicSet(10);
        switch( menu_n.menu_set ) {
            case 0:
            
            break;
            case 1://����Сʱ
                if(menu_n.time_hour > 0) {
                    menu_n.time_hour--;
                } else {
                    menu_n.time_hour = 23;
                }
            break;
            case 2://���÷���
                if(menu_n.time_minnte > 0) {
                    menu_n.time_minnte--;
                } else {
                    menu_n.time_minnte = 59;
                }
            break;
            case 3://���ÿ�ʼʱ��
                if(menu_n.time_time > 0) {
                    menu_n.time_time--;
                } else {
                    menu_n.time_time = 23;
                }
            break;
            case 4://���ùر�ʱ��
                if(menu_n.tume_to > 0) {
                    menu_n.tume_to--;
                } else {
                    menu_n.tume_to = 23;
                }
            break;
            case 5://��������ʱ��
                if(menu_n.time_run > 0) {
                    menu_n.time_run--;
                } else {
                    menu_n.time_run = 59;
                }
            break;
            case 6://����ֹͣʱ��
                if(menu_n.time_stop > 0) {
                    menu_n.time_stop--;
                } else {
                    menu_n.time_stop = 59;
                }
            break;
            default:
            break;
         }
    break;
    case 0x11:
        Ht1621bMusicSet(10);
        switch( menu_n.menu_set ) {
            case 0:
                menu_n.menu_set= 1;
                menu_n.time_hour = Ds1302GetTimeKeep(2);
                menu_n.time_minnte = Ds1302GetTimeKeep(1);
                MotoSet(0);//�رյ��
                Ht1621bFlashMoto(0);
                Ht1621bSetWork(1,1);
                menu_n.run_minute = 0;
            break;
            case 1://
                menu_n.menu_set= 2;
                Ds1302SetTime(menu_n.time_hour,menu_n.time_minnte);
                Ht1622bSetTime(Ds1302GetTimeKeep(2),Ds1302GetTimeKeep(1));
            break;
            case 2:
                menu_n.menu_set= 3;
                Ds1302SetTime(menu_n.time_hour,menu_n.time_minnte);
                Ht1622bSetTime(menu_n.time_hour,menu_n.time_minnte);
            break;
            case 3:
                menu_n.menu_set= 4;
                EepromWrite(0x11,menu_n.time_time);
            break;
            case 4:
                menu_n.menu_set= 5;
                EepromWrite(0x12,menu_n.tume_to);
            break;
            case 5:
                menu_n.menu_set= 6;
                EepromWrite(0x13,menu_n.time_run);
            break;
            case 6:
                menu_n.menu_set= 0;
                EepromWrite(0x14,menu_n.time_stop);
                all_data_update();
                mcu_get_system_time();
            break;
            default:
            break;
         }
    break;
//    case 0x02://������Ч
//       // Ht1621bMusicSet(10);
//    break;
    case 0x12://��λwifi
       // Ht1621bMusicSet(10);
        Ht1621bMusicSet(80);
        mcu_reset_wifi();
        menu_n.uptime = 0;
    break;
//    case 0x22:
//      //  Ht1621bMusicSet(10);
//    break;
//    case 0x32:
//      //  Ht1621bMusicSet(10);
//    break;
    default:
        break;
    }
}
/**********************************************��������***************************************************** 
* ��������: u8 MenuAnaTime(void) 
* �������: void 
* ���ز���: u8  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/22
************************************************************************************************************/ 
u8 MenuAnaTime(void) { 
    if( (menu_n.time_time < menu_n.tume_to) || ( (menu_n.time_time == menu_n.tume_to) && (menu_n.time_time_min <= menu_n.time_to_min) ) ) {
        //��ʼʱ��С�ڽ���ʱ��
        if( ( (Ds1302GetTimeKeep(2) > menu_n.time_time) || (Ds1302GetTimeKeep(2) == menu_n.time_time) && (Ds1302GetTimeKeep(1) >= menu_n.time_time_min) )
        && ( (Ds1302GetTimeKeep(2) < menu_n.tume_to) || (Ds1302GetTimeKeep(2) == menu_n.tume_to) && (Ds1302GetTimeKeep(1) <= menu_n.time_to_min) ) ) {
        //��ʱ�����
            return 0x01;
        } else {
        //����ʱ�����
            return 0x00;
        }
    } else {
        // ��ʼʱ����ڽ���ʱ��, ��������
        if( ( (Ds1302GetTimeKeep(2) > menu_n.time_time) || (Ds1302GetTimeKeep(2) == menu_n.time_time) && (Ds1302GetTimeKeep(1) >= menu_n.time_time_min) )
        || ( (Ds1302GetTimeKeep(2) < menu_n.tume_to) || (Ds1302GetTimeKeep(2) == menu_n.tume_to) && (Ds1302GetTimeKeep(1) <= menu_n.time_to_min) ) ) {
        //��ʱ�����
            return 0x01;
        } else {
        //����ʱ�����
            return 0x00;
        }
    }
}
/**********************************************��������***************************************************** 
* ��������: void MenuWifiSer(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/29
************************************************************************************************************/ 
void MenuWifiSer(void) { 

}
/**********************************************��������***************************************************** 
* ��������: void MenuUpdata(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/22
************************************************************************************************************/ 
void MenuUpdata(void) {
    static u8 time_count = 0;
    static u8 hopping = 0;
    
    if(mcu_get_wifi_work_state() == WIFI_CONNECTED) {//���ӳɹ�
        menu_n.wifi_bit = 1;
        Ht1621Setwifi(1);
        if(menu_n.uptime == 0) {
            menu_n.uptime = 1;
            DelayMs(600);
            all_data_update();
            DelayMs(600);
            mcu_get_system_time();
        }
    } else if(mcu_get_wifi_work_state() == AP_STATE){//���óɹ�δ������·����
        menu_n.wifi_bit = 0;
        Ht1621Setwifi(hopping);
        menu_n.uptime = 0;
    } else {
        static u8 hopping2 = 0;
        if(hopping2 == 0) {
            hopping2 = 1;
        } else {
            hopping2 = 0;
        }
        menu_n.uptime = 0;
        menu_n.wifi_bit = 0;
        Ht1621Setwifi(hopping2);
        Ht1622bShow();
    }
    if(time_count < 3) {
        time_count++;
    } else {
        time_count = 0;
        if(menu_n.power == 1) {
            if(hopping == 0) {
                hopping = 1;
            } else {
                hopping = 0;
            }
            //�ͼ��
            if(LevelRead() == 1) {
                Ht1621SetLevel(hopping);
                if(menu_n.level == 0) {
                    menu_n.level = 1;
                    all_data_update();//�ϴ�����
                }
            } else {
                Ht1621SetLevel(1);
                if(menu_n.level == 1) {
                    menu_n.level = 0;
                    all_data_update();//�ϴ�����
                }
            }
            //���ó�ʱ
            if(menu_n.menu_set > 0) {
                if(menu_n.time_out < 16) {
                    menu_n.time_out++;
                } else {
                    
                    menu_n.time_out = 0;
                    menu_n.menu_set = 0;
                    
                    Ds1302SetTime(menu_n.time_hour,menu_n.time_minnte);
                    Ht1622bSetTime(Ds1302GetTimeKeep(2),Ds1302GetTimeKeep(1));
                    Ds1302SetTime(menu_n.time_hour,menu_n.time_minnte);
                    Ht1622bSetTime(menu_n.time_hour,menu_n.time_minnte);
                    EepromWrite(0x11,menu_n.time_time);
                    EepromWrite(0x12,menu_n.tume_to);
                    EepromWrite(0x13,menu_n.time_run);
                    EepromWrite(0x14,menu_n.time_stop);
                }
            }
            switch( menu_n.menu_set ) {
                case 0://����ģʽ
                    Ht1662bSetConterTime(menu_n.time_time,menu_n.tume_to,
                                        menu_n.time_run,menu_n.time_stop);
                    Ht1621bSetpoint(hopping);
                    Ds1302GetTime();
                    Ht1622bSetTime(Ds1302GetTimeKeep(2),Ds1302GetTimeKeep(1));
                    //�ж��Ƿ��ڹ���ʱ��
                    if(MenuAnaTime() == 0x01) {//�ڹ���ʱ��
                        if( menu_n.last_minute != Ds1302GetTimeKeep(1) ) {
                            menu_n.last_minute = Ds1302GetTimeKeep(1);
                            menu_n.run_minute++;
                            if(menu_n.run_minute < menu_n.time_run) {
                                if(menu_n.conti_run_time_min < 60) {
                                    menu_n.conti_run_time_min++;
                                } else {
                                    menu_n.conti_run_time_min = 0;
                                    menu_n.conti_run_time++;
                                    EepromWrite(0x15,menu_n.conti_run_time);
                                    EepromWrite(0x16,(menu_n.conti_run_time >> 8));
                                    all_data_update();//�ϴ�����
                                    mcu_get_system_time();
                                }
                            }
                            EepromWrite(0x17,menu_n.conti_run_time_min);
                        }
                        if(menu_n.run_minute < menu_n.time_run) {
                            MotoSet(1);//�򿪵��
                            if(menu_n.moto_flash < 6) {
                                menu_n.moto_flash++;
                            } else {
                                menu_n.moto_flash = 0;
                            }
                            Ht1621bFlashMoto(menu_n.moto_flash);
                            Ht1621bSetWork(hopping,1);
                        } else if(menu_n.run_minute < (menu_n.time_run+menu_n.time_stop)) {
                            MotoSet(0);//�رյ��
                            Ht1621bFlashMoto(0);
                            Ht1621bSetWork(1,hopping);
                            all_data_update();//�ϴ�����
                            mcu_get_system_time();
                        } else {
                            menu_n.run_minute = 0;
                            MotoSet(1);//�򿪵��
                            if(menu_n.moto_flash < 6) {
                                menu_n.moto_flash++;
                            } else {
                                menu_n.moto_flash = 0;
                            }
                            Ht1621bFlashMoto(menu_n.moto_flash);
                            Ht1621bSetWork(hopping,1);
                        }
                    } else {
                        MotoSet(0);//�رյ��
                        Ht1621bFlashMoto(0);
                        Ht1621bSetWork(1,1);
                    }
                break;
                case 1://ʱ������ģʽ
                    Ht1621bSetpoint(1);
                    if(hopping == 0) {
                        Ht1622bSetTime(100,menu_n.time_minnte);
                    } else {
                        Ht1622bSetTime(menu_n.time_hour,menu_n.time_minnte);
                    }
                break;
                case 2://ʱ������ģʽ
                    if(hopping == 0) {
                        Ht1622bSetTime(menu_n.time_hour,100);
                    } else {
                        Ht1622bSetTime(menu_n.time_hour,menu_n.time_minnte);
                    }
                break;
                case 3://����ʱ������
                    if(hopping == 0) {
                        Ht1662bSetConterTime(100,menu_n.tume_to,
                                        menu_n.time_run,menu_n.time_stop);
                    } else {   
                        Ht1662bSetConterTime(menu_n.time_time,menu_n.tume_to,
                                        menu_n.time_run,menu_n.time_stop);
                    }
                break;
                case 4://����ʱ������
                    if(hopping == 0) {
                        Ht1662bSetConterTime(menu_n.time_time,100,
                                        menu_n.time_run,menu_n.time_stop);
                    } else {   
                        Ht1662bSetConterTime(menu_n.time_time,menu_n.tume_to,
                                        menu_n.time_run,menu_n.time_stop);
                    }
                break;
                case 5:
                    if(hopping == 0) {
                        Ht1662bSetConterTime(menu_n.time_time,menu_n.tume_to,
                                        100,menu_n.time_stop);
                    } else {   
                        Ht1662bSetConterTime(menu_n.time_time,menu_n.tume_to,
                                        menu_n.time_run,menu_n.time_stop);
                    }
                break;
                case 6:
                    if(hopping == 0) {
                        Ht1662bSetConterTime(menu_n.time_time,menu_n.tume_to,
                                        menu_n.time_run,100);
                    } else {   
                        Ht1662bSetConterTime(menu_n.time_time,menu_n.tume_to,
                                        menu_n.time_run,menu_n.time_stop);
                    }
                break;
                default:
                break;
             }
            Ht1622bShow();
        } else {
            //�رյ��
            MotoSet(0);
        }
    }
}










