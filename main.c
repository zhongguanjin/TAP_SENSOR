#include "Task.h"
#include "main.h"
#include "uart.h"
#include "timer.h"
#include "system.h"
#include "adc.h"
#include "LTR507ALS.h"
#include "I2C_Config.h"
#include "dbg.h"


static uint16 t_2s=0;
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
        LED2_OUT();
        LED2_PIN =ON;
        state = MODE_INIT;
    }
    else
    {
        state = MODE_ERR;
        dbg("state->err\r\n");
    }
    delay_ms(20);
	Init_TMR0();
	HAL248_IN(); // �����������ó�����
	//INTERRUPT_IN();
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
	dbg("init ok\r\n");
	while(1)
	{
	    static uint8 fb_flg = 0;
        //asm("sleep");
	    if(state == MODE_ERR)
	    {
            if(ltr507_init()==OK)
            {
                LED2_OUT();
                LED2_PIN =ON;
                state = MODE_INIT;
            }
            else
            {
                state = MODE_ERR;
                dbg("state->err\r\n");
            }
        }
        if(adj_ok_flg ==1)
        {
           adj_ok_flg =0;
           fb_flg=1;
           t_2s =0;
           f2s = 0;
           DRV_8837_CTR(OPEN_8837);
        }
        /* BEGIN: Added by zgj, 2018/1/3 */
        if(f2s ==1)
        {
            f2s =0;
            if(fb_flg ==1)
            {
               fb_flg=0;
               dbg("auto open tap 2s\r\n");
               DRV_8837_CTR(CLOSE_8837);
            }
        }
        if(f1s  == 1)
        {
            f1s  =0;
            dbg("state:(%d)-bat:(%d)-[%d][%d]-[%d]\r\n",state,BAT_AD_VAL,PS_DATA_H,PS_DATA_L,PS_DATA);
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
	static uint16 f_1s =0;
	if(TMR0IF && TMR0IE)
	{
		TMR0IF = 0;
		t_2s++;
		f_1s ++;
		TMR0 = TMR0+TMR0_VALUE;
		if(t_2s >= 2000)
		{
			t_2s = 0;
			f2s = 1;
		}
		if(f_1s  >= 1000)
		{
            f_1s  = 0;
            f1s  = 1;
		}
	    TaskRemarks();  //��������ѯ����
	}
}
