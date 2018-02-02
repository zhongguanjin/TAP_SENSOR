#include "Task.h"
#include "main.h"
#include "uart.h"
#include "timer.h"
#include "system.h"
#include "adc.h"
#include "LTR507ALS.h"
#include "I2C_Config.h"
#include "dbg.h"



/*****************************************************************************
 �� �� ��  : Init_Sys
 ��������  : ϵͳ��ʼ������
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��5��18�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
void Init_Sys(void)
{
	Init_MCU();
	if(DEBUG)
	{
		Init_Uart();
	}
	Init_ADC();
    DRV8837_Init();
	I2CInit();
    delay_ms(1000);
    if(ltr507_init()==OK)
    {
        state = MODE_INIT;
    }
    delay_ms(20);
	Init_TMR0();
}


/*****************************************************************************
 �� �� ��  : main
 ��������  : ������
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��5��18�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
void main(void)
{
	Init_Sys();
	//asm("sleep"); //����
	dbg("init ok\r\n");
	HAL248_IN(); // �����������ó�����
	while(1)
	{
        if(adj_ok_flg ==1)
        {
           DRV_8837_CTR(OPEN_8837);
        }
        /* BEGIN: Added by zgj, 2018/1/3 */
        if(f2s ==1)
        {
            f2s =0;
            if(adj_ok_flg ==1)
            {
               adj_ok_flg =0;
               dbg("auto open tap 2s\r\n");
               DRV_8837_CTR(CLOSE_8837);
            }
        }
        if(f500ms == 1)
        {
            f500ms =0;
            dbg("state:(%d)-bat:(%d)-[%d][%d]->%d\r\n",state,BAT_AD_VAL,PS_DATA_H,PS_DATA_L,HAL248_PIN);
        }
        /* END:   Added by zgj, 2018/1/3 */
	   TaskProcess();            // ������
	   CLRWDT();
	}
}

/*****************************************************************************
 �� �� ��  : ISR
 ��������  : �жϷ�����
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��5��18�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
void interrupt ISR(void)
{
	static uint16 t_2s=0,t_500ms=0;
	if(TMR0IF && TMR0IE)
	{
		TMR0IF = 0;
		t_2s++;
		t_500ms++;
		TMR0 = TMR0+TMR0_VALUE;
		if(t_2s >= 2000)
		{
			t_2s = 0;
			f2s = 1;
		}
		if(t_500ms >= 1000)
		{
            t_500ms = 0;
            f500ms = 1;
		}
	    TaskRemarks();  //��������ѯ����
	}
}
