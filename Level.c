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
* ������ڣ�2016/9/22
* ���뻷����C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* ��ʷ��Ϣ��
*******************************************************************************/
#include "Level.h"

#define LEVEL_BU    PD_IDR_IDR0

/**********************************************��������***************************************************** 
* ��������: void LevelInit(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/22
************************************************************************************************************/ 
void LevelInit(void) { 
    PD_DDR_DDR0 = 0;
    PD_CR1_C10 = 0;
    PD_CR2_C20 = 0;
}
/**********************************************��������***************************************************** 
* ��������: u8 LevelRead(void) 
* �������: void 
* ���ز���: u8  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/22
************************************************************************************************************/ 
u8 LevelRead(void) { 
    if(LEVEL_BU == 0) {
        return 0x01;
    } else {
        return 0x00;
    }
}








