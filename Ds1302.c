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
#include "Ds1302.h"
#include "Delay.h"


#define DS13CLK 	PC_ODR_ODR7		    //1302 CLK
#define DS13IO	    PD_DDR_DDR2		    //IO方向0为输入，1为输出
#define DS13O		PD_ODR_ODR2			//out
#define DS13I		PD_IDR_IDR2         //in
#define DS13CS	    PD_ODR_ODR3		    //RST
#define NOP()	    asm("nop;")

/**********************************************函数定义***************************************************** 
* 函数名称: void Ds1302Init(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/19
************************************************************************************************************/ 
void Ds1302Init(void) { 
    PC_DDR_DDR7 = 1;//sclk
    PC_CR1_C17 = 1;
    PC_CR2_C27 = 0;
    
    PD_DDR_DDR2 = 1;//i/o
    PD_CR1_C12 = 1;
    PD_CR2_C22 = 0;
    
    PD_DDR_DDR3 = 1;//rst
    PD_CR1_C13 = 1;
    PD_CR2_C23 = 0;
}

/*向DS1302写一个字节*/
static void _wds13byte(u8 _code) {
    u8 i;
    DS13CLK =0;
    DS13IO=1;
    NOP();
    for(i=0;i<8;i++) {
        if(_code&0x01) { 
            DS13O =1;
        } else {
            DS13O =0;
        }
        DelayUs(10);
        DS13CLK =1;
        DelayUs(10);
        DS13CLK =0;
        _code >>=1; 
    }
}
/*从DS1302读一个字节*/
static u8 _rds13byte(void) {
    u8 i,_code=0;
    DS13CLK =0;
    //PB1_CR2=0;//静止中断
    DS13IO=0;//设为输入
    DelayUs(2);
    for(i=0;i<8;i++) {
        _code>>=1;
        DelayUs(10);
        if(DS13I) {
            _code|=0x80;
        } else { 
            _code&=0x7f;
        }
        DS13CLK =1;
        DelayUs(10);
        DS13CLK =0;
    }
    DS13IO=0;//设为输入
    return _code;
}
/*读功能 _code读功能命令   */
static u8 readds1302(u8 _code) {
    DS13CS =0;         /*关闭DS1302*/ 
    DS13CLK =0;
    NOP();NOP();NOP();NOP();NOP();NOP();NOP();
    DS13CS =1;          /*使能DS1302*/
    NOP();NOP();NOP();NOP();NOP();NOP();
    _wds13byte(_code);   /*读代码*/
    _code=_rds13byte(); /*返回读取数字*/
    DS13CLK =1;
    DS13CS =0;         /*关闭DS1302*/ 
    return _code;
}
/*写功能 fp写的地址，_code写的内容*/
static void writeds1302(u8 fp,u8 _code) {
    DS13CS =0;         /*关闭DS1302*/ 
    DS13CLK =0;
    DS13CS =1;          /*使能DS1302*/
    NOP();NOP();NOP();NOP();NOP();NOP();NOP();
    _wds13byte(fp);     /*写控制命令*/
    _wds13byte(_code); /*写入数据*/
    DS13CLK=1;
    DS13CS =0;          /*关闭DS1302*/ 
}

static u8 cur_time[3];
void Ds1302GetTime(void) {
	u8 i,t;
	u8 add=0x81;
	for(i=0;i<3;i++) {
		t=readds1302(add);	
		cur_time[i]=((t&0x70)>>4)*10+(t&0x0F);//将读出数据转化						 
		add+=2;
	}
}

u8 Ds1302GetTimeKeep(u8 cmd) {
    return cur_time[cmd];
}

//给1302设置时间
void Ds1302SetTime(u8 set_hour, u8 set_min) {
    u8 tempdata = 0;
    writeds1302(0x8e,0x00); //解除写保护
    writeds1302(0x80,0);//秒给0，第一次启动时钟用,下了这条才能启动1302
    tempdata=set_min/10;
    tempdata<<=4;
    tempdata|=(set_min%10);
    writeds1302(0x82,tempdata);//写之前要对时分做十位个位分离，10位放高3位，个位放低4位。
    tempdata=set_hour/10;
    tempdata<<=4;
    tempdata|=(set_hour%10);
    writeds1302(0x84,tempdata);
    writeds1302(0x8e,0x80); //置位写保护
}

//给1302设置时间
void Ds1302SetTime_s(u8 set_hour, u8 set_min, u8 set_sec) {
    u8 tempdata = 0;
    writeds1302(0x8e,0x00); //解除写保护
    writeds1302(0x80,0);//秒给0，第一次启动时钟用,下了这条才能启动1302
    tempdata=set_min/10;
    tempdata<<=4;
    tempdata|=(set_min%10);
    writeds1302(0x82,tempdata);//写之前要对时分做十位个位分离，10位放高3位，个位放低4位。
    
    tempdata=set_hour/10;
    tempdata<<=4;
    tempdata|=(set_hour%10);
    writeds1302(0x84,tempdata);
    
    tempdata=set_sec/10;
    tempdata<<=4;
    tempdata|=(set_sec%10);
    writeds1302(0x80,tempdata);
    
    writeds1302(0x8e,0x80); //置位写保护
}







