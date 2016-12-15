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
#include "Ht1621b.h"
#include "Delay.h"

#define HT1621_WR   PC_ODR_ODR4
#define HT1621_CS   PC_ODR_ODR6
#define HT1621_DAT  PC_ODR_ODR5
#define HT1621_LIGHT  PC_ODR_ODR3

static const u8 ht_num[12] = {0xfd,0x61,0xdb,0xf3,0x67,0xb7,0xbf,0xe1,0xff,0xf7,0x01,0x00};
static const u8 ht_num_bit[15] = {1,3,5,7,11,13,15,17,19,21,23,25,0};
static u8 ht_cach[15];
static u8 ht_flash[11];
/**********************************************��������***************************************************** 
* ��������: void Ht1621bInit(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/19
************************************************************************************************************/ 
void Ht1621bInit(void) { 
    PC_DDR_DDR6 = 1;
    PC_CR1_C16 = 1;
    PC_CR2_C26 = 1;
    
    PC_DDR_DDR5 = 1;
    PC_CR1_C15 = 1;
    PC_CR2_C25 = 1;
    
    PC_DDR_DDR4 = 1;
    PC_CR1_C14 = 1;
    PC_CR2_C24 = 1;
    
    PC_DDR_DDR3 = 1;
    PC_CR1_C13 = 1;
    PC_CR2_C23 = 1;
    
    HT1621_LIGHT = 0;
    
    HT1621_CS=1;
    HT1621_WR=0;
    HT1621_DAT=1;
    DelayMs(100);       //��ʱʹLCD������ѹ�ȶ�
    Ht1621bWrCmd(BIAS);
    Ht1621bWrCmd(RC256);  //ʹ���ڲ�����
    Ht1621bWrCmd(SYSDIS);
    Ht1621bWrCmd(WDTDIS);
    Ht1621bWrCmd(TONE2k);//�򿪷�����4k
    Ht1621bWrCmd(SYSEN);
    Ht1621bWrCmd(LCDON);
    Ht1621bWrCmd(TONEON); //�رշ�����
    DelayMs(50);
    Ht1621bWrCmd(TONEOFF); //�رշ�����  
    HT1621_CS=0;
    Ht1621bWrDataCmd(0);
    for(u8 i = 0;i < 15;i++) {
        Ht1621bWrByte(0x00);
        ht_cach[i] = 0x00;
    }
    HT1621_CS=1;
}
/**********************************************��������***************************************************** 
* ��������: void Ht1621bWrByte(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/19
************************************************************************************************************/ 
void Ht1621bWrByte(u8 byte) { 
    u8 i;
    for(i=0;i<8;i++) {
        if(byte&0x80) {
            HT1621_DAT=1;
        } else {
            HT1621_DAT=0;
        }
        HT1621_WR=0;
        DelayUs(20);
        HT1621_WR=1;
        DelayUs(20);
        byte<<=1;
    }
}
/**********************************************��������***************************************************** 
* ��������: void Ht1621bWrCmd(u8 uda) 
* �������: u8 uda 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/19
************************************************************************************************************/ 
void Ht1621bWrDataCmd(u8 uda) { 
    u8 i,ucDataCmd=0x05;    //101
    ucDataCmd<<=5;
    for(i=0;i<3;i++) {
        if(ucDataCmd&0x80) {
            HT1621_DAT=1;
        } else {
            HT1621_DAT=0;
        }
        HT1621_WR=0;
        DelayUs(20);
        HT1621_WR=1;
        DelayUs(20);
        ucDataCmd<<=1;
    }
    uda<<=2;
    for(i=0;i<6;i++) {
        if(uda&0x80) {
            HT1621_DAT=1;
        } else {
            HT1621_DAT=0;
        }
        HT1621_WR=0;
        DelayUs(20);
        HT1621_WR=1;
        DelayUs(20);
        uda<<=1;
    }
}
/**********************************************��������***************************************************** 
* ��������: void Ht1621bWrCmd(u8 cmd) 
* �������: u8 cmd 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/19
************************************************************************************************************/ 
void Ht1621bWrCmd(u8 cmd) { 
    u8 i,ucCmdByte=0x80;
    HT1621_CS=0;
    DelayUs(40);
    for (i=0;i<4;i++) //<4
    {
        if(ucCmdByte&0x80) {
            HT1621_DAT=1;
        } else {
            HT1621_DAT=0;
        }
        HT1621_WR=0;
        DelayUs(40);
        HT1621_WR=1;
        DelayUs(20);
        ucCmdByte<<=1;
    }
    for (i=0;i<8;i++)
    {
        if(cmd&0x80) {
            HT1621_DAT=1;
        } else {
            HT1621_DAT=0;
        }
        HT1621_WR=0;
        DelayUs(40);
        HT1621_WR=1;
        DelayUs(20);
        cmd<<=1;
    }
    HT1621_CS=1;
    DelayUs(20);
}
/**********************************************��������***************************************************** 
* ��������: void Ht1621bMusicSet(u8 time) 
* �������: u8 time 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/21
************************************************************************************************************/ 
static u8 ht_mu_time = 0;
void Ht1621bMusicSet(u8 time) { 
    Ht1621bWrCmd(TONEON); //�رշ�����
    ht_mu_time = time;
    DelayMs(30*time);
    Ht1621bWrCmd(TONEOFF); //�رշ�����
}
/**********************************************��������***************************************************** 
* ��������: void Ht1621Service(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/21
************************************************************************************************************/ 
void Ht1621Service(void) { 
    static u16 ht_count = 0;
    if(ht_mu_time > 0) {
        if(ht_count < 1500) {
            ht_count++;
        } else {
            ht_count = 0;
            ht_mu_time--;
        }
        if(ht_mu_time == 1) {
           // Ht1621bWrCmd(TONEOFF); //�رշ�����  
        }
    }
}

/**********************************************��������***************************************************** 
* ��������: void void Ht1622bShow(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/22
************************************************************************************************************/ 
void Ht1622bShow(void) { 
    HT1621_CS=0;
    Ht1621bWrDataCmd(25);   //��RAM�ĵ�ַiд����  ����Ĳ�����ΧΪ:0-31
    Ht1621bWrByte((ht_cach[11]&0xf0));
    HT1621_CS=1;
    
    HT1621_CS=0;
    Ht1621bWrDataCmd(0);   //��RAM�ĵ�ַiд����  ����Ĳ�����ΧΪ:0-31
    Ht1621bWrByte(((ht_cach[11]&0x0f) << 4));
    HT1621_CS=1;
    
    for(u8 i = 0;i < 11;i++) {
        HT1621_CS=0;
        Ht1621bWrDataCmd(ht_num_bit[i]);   //��RAM�ĵ�ַiд����  ����Ĳ�����ΧΪ:0-31
        Ht1621bWrByte(ht_cach[i]);
        HT1621_CS=1;
    }
}
/**********************************************��������***************************************************** 
* ��������: void Ht1622bSetTime(u8 hour,u8 minute) 
* �������: u8 hour,u8 minute 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/22
************************************************************************************************************/ 
void Ht1622bSetTime(u8 hour,u8 minute) {
    u8 temp[4];
    
    if(hour == 100) {
        for(u8 i = 0;i < 2;i++) {
            temp[i] = ht_num[10];
        }
    } else {
        temp[0] = ht_num[hour/10];
        temp[1] = ht_num[hour%10];
    }
    if(minute == 100) {
         for(u8 i = 2;i < 4;i++) {
            temp[i] = ht_num[10];
        }
    } else {
        temp[2] = ht_num[minute/10];
        temp[3] = ht_num[minute%10];
    }
    if(ht_flash[7] == 0) {//С����
        temp[0] &= 0xfe;
    }
    if(ht_flash[8] == 0) {//Һλ
        temp[1] &= 0xfe; 
    }
    if(ht_flash[9] == 0) {//����
        temp[2] &= 0xfe;
    }
    if(ht_flash[10] == 0) {//����
        temp[3] &= 0xfe;
    }
    ht_cach[4] = temp[0];
    ht_cach[5] = temp[1];
    
    ht_cach[6] = temp[2];
    ht_cach[7] = temp[3];
}
/**********************************************��������***************************************************** 
* ��������: void Ht1662bSetConterTime(u8 time, u8 to,u8 run,u8 stop) 
* �������: u8 time, u8 to,u8 run,u8 stop 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/22
************************************************************************************************************/ 
void Ht1662bSetConterTime(u8 time, u8 to,u8 run,u8 stop) { 
    u8 temp[8];
    
    if(time == 100) {
        temp[0] = ht_num[10];
        temp[1] = ht_num[10];
    } else {
        temp[0] = ht_num[time/10];
        temp[1] = ht_num[time%10];
    }
    
    if(to == 100) {
        temp[2] = ht_num[10];
        temp[3] = ht_num[10];
    } else {
        temp[2] = ht_num[to/10];
        temp[3] = ht_num[to%10];
    }
    
    if(run == 100) {
        temp[4] = ht_num[10];
        temp[5] = ht_num[10];
    } else {
        temp[4] = ht_num[run/10];
        temp[5] = ht_num[run%10];
    }
    
    if(stop == 100) {
        temp[6] = ht_num[10];
        temp[7] = ht_num[10];
    } else {
        temp[6] = ht_num[stop/10];
        temp[7] = ht_num[stop%10];
    }
    
    if(ht_flash[0] == 0) {//������һ��
        temp[0] &= 0xfe;
    }
    if(ht_flash[1] == 0) {//�����ڶ���
        temp[1] &= 0xfe;
    }
    if(ht_flash[2] == 0) {//����������
        temp[2] &= 0xfe;
    }
    if(ht_flash[3] == 0) {//����������
        temp[3] &= 0xfe;
    }
    if(ht_flash[4] == 0) {//����������
        temp[4] &= 0xfe;
    }
    if(ht_flash[5] == 0) {//����������
        temp[5] &= 0xfe;
    }
    if(ht_flash[6] == 0) {//wifi
        temp[6] &= 0xfe;
    }
    
    ht_cach[8] = temp[0];
    ht_cach[9] = temp[1];
    
    ht_cach[10] = temp[2];
    ht_cach[11] = temp[3];
    
    ht_cach[0] = temp[4];
    ht_cach[1] = temp[5];
    
    ht_cach[2] = temp[6];
    ht_cach[3] = temp[7];
}
/**********************************************��������***************************************************** 
* ��������: void Ht1621bSetpoint(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/22
************************************************************************************************************/ 
void Ht1621bSetpoint(u8 cmd) { 
    //static u8 ht_point = 0;
    if(cmd == 0) {
        //ht_point = 1;
        ht_flash[7] = 0; //С����
    } else {
        //ht_point = 0;
        ht_flash[7] = 1;
    }
}
/**********************************************��������***************************************************** 
* ��������: void Ht1621bFlashMoto(u8 num) 
* �������: u8 num 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/22
************************************************************************************************************/ 
void Ht1621bFlashMoto(u8 num) { 
    switch( num ) {
        case 0x00:
            ht_flash[0] = 0;
            ht_flash[1] = 0;
            ht_flash[2] = 0;
            ht_flash[3] = 0;
            ht_flash[4] = 0;
            ht_flash[5] = 0;
            ht_cach[8] &= 0xfe;
            ht_cach[9] &= 0xfe;
            
            ht_cach[10] &= 0xfe;
            ht_cach[11] &= 0xfe;
            
            ht_cach[0] &= 0xfe;
            ht_cach[1] &= 0xfe;
        break;
        case 0x01:
            ht_flash[0] = 0;
            ht_flash[1] = 0;
            ht_flash[2] = 0;
            ht_flash[3] = 0;
            ht_flash[4] = 0;
            ht_flash[5] = 1;
            ht_cach[8] &= 0xfe;
            ht_cach[9] &= 0xfe;
            
            ht_cach[10] &= 0xfe;
            ht_cach[11] &= 0xfe;
            
            ht_cach[0] &= 0xfe;
            ht_cach[1] |= 0x01;
        break;
        case 0x02:
            ht_flash[0] = 0;
            ht_flash[1] = 0;
            ht_flash[2] = 0;
            ht_flash[3] = 0;
            ht_flash[4] = 1;
            ht_flash[5] = 1;
            ht_cach[8] &= 0xfe;
            ht_cach[9] &= 0xfe;
            
            ht_cach[10] &= 0xfe;
            ht_cach[11] &= 0xfe;
            
            ht_cach[0] |= 0x01;
            ht_cach[1] |= 0x01;
        break;
        case 0x03:
            ht_flash[0] = 0;
            ht_flash[1] = 0;
            ht_flash[2] = 0;
            ht_flash[3] = 1;
            ht_flash[4] = 1;
            ht_flash[5] = 1;
            ht_cach[8] &= 0xfe;
            ht_cach[9] &= 0xfe;
            
            ht_cach[10] &= 0xfe;
            ht_cach[11] |= 0x01;
            
            ht_cach[0] |= 0x01;
            ht_cach[1] |= 0x01;
        break;
        case 0x04:
            ht_flash[0] = 0;
            ht_flash[1] = 0;
            ht_flash[2] = 1;
            ht_flash[3] = 1;
            ht_flash[4] = 1;
            ht_flash[5] = 1;
            ht_cach[8] &= 0xfe;
            ht_cach[9] &= 0xfe;
            
            ht_cach[10] |= 0x01;
            ht_cach[11] |= 0x01;
            
            ht_cach[0] |= 0x01;
            ht_cach[1] |= 0x01;
        break;
        case 0x05:
            ht_flash[0] = 0;
            ht_flash[1] = 1;
            ht_flash[2] = 1;
            ht_flash[3] = 1;
            ht_flash[4] = 1;
            ht_flash[5] = 1;
            ht_cach[8] &= 0xfe;
            ht_cach[9] |= 0x01;
            
            ht_cach[10] |= 0x01;
            ht_cach[11] |= 0x01;
            
            ht_cach[0] |= 0x01;
            ht_cach[1] |= 0x01;
        break;
        case 0x06:
            ht_flash[0] = 1;
            ht_flash[1] = 1;
            ht_flash[2] = 1;
            ht_flash[3] = 1;
            ht_flash[4] = 1;
            ht_flash[5] = 1;
            ht_cach[8] |= 0x01;
            ht_cach[9] |= 0x01;
            
            ht_cach[10] |= 0x01;
            ht_cach[11] |= 0x01;
            
            ht_cach[0] |= 0x01;
            ht_cach[1] |= 0x01;
        break;
        default:
        break;
    }
}
/**********************************************��������***************************************************** 
* ��������: void Ht1621Setwifi(u8 cmd,u8 cmd) 
* �������: u8 cmd,u8 cmd 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/22
************************************************************************************************************/ 
void Ht1621Setwifi(u8 cmd) { 
    if(cmd == 0) {
        ht_flash[6] = 0;//wifi
        ht_cach[2] &= 0xfe;
    } else {
        ht_flash[6] = 1;//wifi 
        ht_cach[2] |= 0x01;
    }
}
/**********************************************��������***************************************************** 
* ��������: void Ht1621SetLevel(u8 cmd) 
* �������: u8 cmd 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/27
************************************************************************************************************/ 
void Ht1621SetLevel(u8 cmd) { 
    if(cmd == 0) {//Һλ
        ht_flash[8] = 0;//Һλ
        ht_cach[5] &= 0xfe; 
    } else {
        ht_flash[8] = 1;//Һλ
        ht_cach[5] |= 0x01; 
    }
}
/**********************************************��������***************************************************** 
* ��������: void Ht1621bSetWork(u8 cmd,u8 cmd2) 
* �������: u8 cmd,u8 cmd2 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/22
************************************************************************************************************/ 
void Ht1621bSetWork(u8 cmd,u8 cmd2) { 
    if(cmd == 0) {
        ht_flash[9] = 0;
        ht_cach[6] &= 0xfe;
    } else {
        ht_flash[9] = 1;
        ht_cach[6] |= 0x01;
    }
    if(cmd2 == 0) {
        ht_flash[10] = 0;
        ht_cach[7] &= 0xfe;
    } else {
        ht_flash[10] = 1;
        ht_cach[7] |= 0x01;
    }
}



/**********************************************��������***************************************************** 
* ��������: void Ht1621bPowerSet(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/22
************************************************************************************************************/ 
void Ht1621bPowerSet(u8 cmd) { 
    HT1621_CS=0;
    if(cmd == 0) {
        HT1621_LIGHT = 1;
        Ht1621bWrCmd(LCDON);
    } else {
        HT1621_LIGHT = 0;
        Ht1621bWrCmd(LCDOFF);
    }
    HT1621_CS=1;
}


















