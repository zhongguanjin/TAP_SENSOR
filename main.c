#include "Task.h"
#include "main.h"
#include "uart.h"
#include "timer.h"
#include "system.h"
#include "adc.h"
#include "LTR507ALS.h"
#include "I2C_Config.h"

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
    delay_ms(100);
    ltr507_init();
    delay_ms(20);
	Init_TMR0();
    //state = MODE_WORK;
    state = MODE_ADJ_PS;
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
uint8 flg_tab0 = 1;
int16 tab1;

/*****************************************************************************
 �� �� ��  : iabs
 ��������  : �����ֵ����
 �������  : int a
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��1��3��
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
int16 iabs(int16 a)
{
    if(a<0)
    {
        return -a;
    }
    else
    {
        return a;
    }
}
void main(void)
{
	Init_Sys();
	//asm("sleep"); //����
	while(1)
	{
        if(adj_ok_flg ==1)
        {
           adj_ok_flg =0;
           DRV_8837_CTR(OPEN_8837);
           delay_ms(2000);
           DRV_8837_CTR(CLOSE_8837);
        }
        /* BEGIN: Added by zgj, 2018/1/3 */
        if(f5ms ==1)
        {
            f5ms =0;
            BAT_AD_VAL =Read_BAT();
            if(flg_tab0 ==1){
                flg_tab0=0;
                tab1=BAT_AD_VAL;
            }
            if(state == MODE_WORK)
            {
                if(iabs(tab1-BAT_AD_VAL)>=25)
                {
                    //ps_contr_mode(STANDY_MODE);
                    state =MODE_LOW_POWER;
                    DRV_8837_CTR(CLOSE_8837);
                }
            }
            if(state ==MODE_LOW_POWER)
            {
                if(iabs(tab1-BAT_AD_VAL)<25)
                {
                    state =MODE_WORK;
                }
            }
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
	static uint8 t_5ms=0;
	if(TMR0IF && TMR0IE)
	{
		TMR0IF = 0;
		t_5ms++;
		TMR0 = TMR0+TMR0_VALUE;
		if(t_5ms >= 10)
		{
			t_5ms = 0;
			f5ms = 1;
		}
	    TaskRemarks();  //��������ѯ����
	}
}
