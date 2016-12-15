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
* 完成日期：2016/9/21
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
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
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuInit(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/21
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

    menu_n.last_minute = Ds1302GetTimeKeep(1);//分钟
    
    menu_n.power = 1;
    ButtonSetFlash(1);
    Ht1621bPowerSet(1);
    //关闭电机
    MotoSet(0);
    menu_n.menu_set= 0;
    
    MenuSetPower(1);
    
    menu_n.uptime = 0;
    //油检测
    if(LevelRead() == 0x00) {
        menu_n.level = 0;
    } else {
        menu_n.level = 0;
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuGetPower(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/23
************************************************************************************************************/ 
u8 MenuGetPower(void) { 
    return menu_n.power;
}
/**********************************************函数定义***************************************************** 
* 函数名称: u8 MenuGetLevel(void) 
* 输入参数: void 
* 返回参数: u8  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/23
************************************************************************************************************/ 
u8 MenuGetLevel(void) { 
    if(menu_n.level == 0) {
        return 0;
    } else {
        return 1;
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuSetPower(u8 cmd) 
* 输入参数: u8 cmd 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/23
************************************************************************************************************/ 
void MenuSetPower(u8 cmd) { 
    if(cmd == 0){
        menu_n.power = 0;
        ButtonSetFlash(1);
        Ht1621bPowerSet(1);
        //关闭电机
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
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuSetWork(u8 start,u8 end,u8 run,u8 stop) 
* 输入参数: u8 start,u8 end,u8 run,u8 stop 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/23
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
/**********************************************函数定义***************************************************** 
* 函数名称: u8 MenuGetWorkTime(u8 cmd) 
* 输入参数: u8 cmd 
* 返回参数: u8  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/26
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
/**********************************************函数定义***************************************************** 
* 函数名称: u16 MenuGetRunTime(void) 
* 输入参数: void 
* 返回参数: u16  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/27
************************************************************************************************************/ 
u16 MenuGetRunTime(void) { 
    return menu_n.conti_run_time;
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuBut(u8 cmd) 
* 输入参数: u8 cmd 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/21
************************************************************************************************************/ 
void MenuBut(u8 cmd) { 
    switch( cmd ) {
    case 0x01://开关
        Ht1621bMusicSet(10);
        if(menu_n.power == 0) {
            menu_n.power = 1;
        } else {
            menu_n.power = 0;
        }
        all_data_update();//上传数据
        MenuSetPower(menu_n.power);
    break;
    case 0x21://加按钮
        menu_n.time_out = 0;
        Ht1621bMusicSet(10);
        switch( menu_n.menu_set ) {
            case 0:
                
            break;
            case 1://设置小时
                if(menu_n.time_hour < 23) {
                    menu_n.time_hour++;
                } else {
                    menu_n.time_hour = 0;
                }
            break;
            case 2://设置分钟
                if(menu_n.time_minnte < 59) {
                    menu_n.time_minnte++;
                } else {
                    menu_n.time_minnte = 0;
                }
            break;
            case 3://设置开始时间
                if(menu_n.time_time < 23) {
                    menu_n.time_time++;
                } else {
                    menu_n.time_time = 0;
                }
            break;
            case 4://设置关闭时间
                if(menu_n.tume_to < 23) {
                    menu_n.tume_to++;
                } else {
                    menu_n.tume_to = 0;
                }
            break;
            case 5://设置运行时间
                if(menu_n.time_run < 59) {
                    menu_n.time_run++;
                } else {
                    menu_n.time_run = 0;
                }
            break;
            case 6://设置停止时间
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
    case 0x31://减按钮
        menu_n.time_out = 0;
        Ht1621bMusicSet(10);
        switch( menu_n.menu_set ) {
            case 0:
            
            break;
            case 1://设置小时
                if(menu_n.time_hour > 0) {
                    menu_n.time_hour--;
                } else {
                    menu_n.time_hour = 23;
                }
            break;
            case 2://设置分钟
                if(menu_n.time_minnte > 0) {
                    menu_n.time_minnte--;
                } else {
                    menu_n.time_minnte = 59;
                }
            break;
            case 3://设置开始时间
                if(menu_n.time_time > 0) {
                    menu_n.time_time--;
                } else {
                    menu_n.time_time = 23;
                }
            break;
            case 4://设置关闭时间
                if(menu_n.tume_to > 0) {
                    menu_n.tume_to--;
                } else {
                    menu_n.tume_to = 23;
                }
            break;
            case 5://设置运行时间
                if(menu_n.time_run > 0) {
                    menu_n.time_run--;
                } else {
                    menu_n.time_run = 59;
                }
            break;
            case 6://设置停止时间
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
                MotoSet(0);//关闭电机
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
//    case 0x02://开关有效
//       // Ht1621bMusicSet(10);
//    break;
    case 0x12://复位wifi
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
/**********************************************函数定义***************************************************** 
* 函数名称: u8 MenuAnaTime(void) 
* 输入参数: void 
* 返回参数: u8  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/22
************************************************************************************************************/ 
u8 MenuAnaTime(void) { 
    if( (menu_n.time_time < menu_n.tume_to) || ( (menu_n.time_time == menu_n.tume_to) && (menu_n.time_time_min <= menu_n.time_to_min) ) ) {
        //开始时间小于结束时间
        if( ( (Ds1302GetTimeKeep(2) > menu_n.time_time) || (Ds1302GetTimeKeep(2) == menu_n.time_time) && (Ds1302GetTimeKeep(1) >= menu_n.time_time_min) )
        && ( (Ds1302GetTimeKeep(2) < menu_n.tume_to) || (Ds1302GetTimeKeep(2) == menu_n.tume_to) && (Ds1302GetTimeKeep(1) <= menu_n.time_to_min) ) ) {
        //在时间段内
            return 0x01;
        } else {
        //不在时间段内
            return 0x00;
        }
    } else {
        // 开始时间大于结束时间, 跨天的情况
        if( ( (Ds1302GetTimeKeep(2) > menu_n.time_time) || (Ds1302GetTimeKeep(2) == menu_n.time_time) && (Ds1302GetTimeKeep(1) >= menu_n.time_time_min) )
        || ( (Ds1302GetTimeKeep(2) < menu_n.tume_to) || (Ds1302GetTimeKeep(2) == menu_n.tume_to) && (Ds1302GetTimeKeep(1) <= menu_n.time_to_min) ) ) {
        //在时间段内
            return 0x01;
        } else {
        //不在时间段内
            return 0x00;
        }
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuWifiSer(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/29
************************************************************************************************************/ 
void MenuWifiSer(void) { 

}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuUpdata(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/22
************************************************************************************************************/ 
void MenuUpdata(void) {
    static u8 time_count = 0;
    static u8 hopping = 0;
    
    if(mcu_get_wifi_work_state() == WIFI_CONNECTED) {//连接成功
        menu_n.wifi_bit = 1;
        Ht1621Setwifi(1);
        if(menu_n.uptime == 0) {
            menu_n.uptime = 1;
            DelayMs(600);
            all_data_update();
            DelayMs(600);
            mcu_get_system_time();
        }
    } else if(mcu_get_wifi_work_state() == AP_STATE){//配置成功未连接上路由器
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
            //油检测
            if(LevelRead() == 1) {
                Ht1621SetLevel(hopping);
                if(menu_n.level == 0) {
                    menu_n.level = 1;
                    all_data_update();//上传数据
                }
            } else {
                Ht1621SetLevel(1);
                if(menu_n.level == 1) {
                    menu_n.level = 0;
                    all_data_update();//上传数据
                }
            }
            //设置超时
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
                case 0://正常模式
                    Ht1662bSetConterTime(menu_n.time_time,menu_n.tume_to,
                                        menu_n.time_run,menu_n.time_stop);
                    Ht1621bSetpoint(hopping);
                    Ds1302GetTime();
                    Ht1622bSetTime(Ds1302GetTimeKeep(2),Ds1302GetTimeKeep(1));
                    //判断是否在工作时间
                    if(MenuAnaTime() == 0x01) {//在工作时间
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
                                    all_data_update();//上传数据
                                    mcu_get_system_time();
                                }
                            }
                            EepromWrite(0x17,menu_n.conti_run_time_min);
                        }
                        if(menu_n.run_minute < menu_n.time_run) {
                            MotoSet(1);//打开电机
                            if(menu_n.moto_flash < 6) {
                                menu_n.moto_flash++;
                            } else {
                                menu_n.moto_flash = 0;
                            }
                            Ht1621bFlashMoto(menu_n.moto_flash);
                            Ht1621bSetWork(hopping,1);
                        } else if(menu_n.run_minute < (menu_n.time_run+menu_n.time_stop)) {
                            MotoSet(0);//关闭电机
                            Ht1621bFlashMoto(0);
                            Ht1621bSetWork(1,hopping);
                            all_data_update();//上传数据
                            mcu_get_system_time();
                        } else {
                            menu_n.run_minute = 0;
                            MotoSet(1);//打开电机
                            if(menu_n.moto_flash < 6) {
                                menu_n.moto_flash++;
                            } else {
                                menu_n.moto_flash = 0;
                            }
                            Ht1621bFlashMoto(menu_n.moto_flash);
                            Ht1621bSetWork(hopping,1);
                        }
                    } else {
                        MotoSet(0);//关闭电机
                        Ht1621bFlashMoto(0);
                        Ht1621bSetWork(1,1);
                    }
                break;
                case 1://时间设置模式
                    Ht1621bSetpoint(1);
                    if(hopping == 0) {
                        Ht1622bSetTime(100,menu_n.time_minnte);
                    } else {
                        Ht1622bSetTime(menu_n.time_hour,menu_n.time_minnte);
                    }
                break;
                case 2://时间设置模式
                    if(hopping == 0) {
                        Ht1622bSetTime(menu_n.time_hour,100);
                    } else {
                        Ht1622bSetTime(menu_n.time_hour,menu_n.time_minnte);
                    }
                break;
                case 3://工作时间设置
                    if(hopping == 0) {
                        Ht1662bSetConterTime(100,menu_n.tume_to,
                                        menu_n.time_run,menu_n.time_stop);
                    } else {   
                        Ht1662bSetConterTime(menu_n.time_time,menu_n.tume_to,
                                        menu_n.time_run,menu_n.time_stop);
                    }
                break;
                case 4://工作时间设置
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
            //关闭电机
            MotoSet(0);
        }
    }
}










