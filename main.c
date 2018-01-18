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
uint16 low_power_time = 0;
uint16 low_bat_ad=0;
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
            if(flg_tab0 ==1)
            {
                flg_tab0=0;
                tab1=BAT_AD_VAL;
            }
            if(state == MODE_WORK)//����״̬
            {
                if((tab1>BAT_AD_VAL)&&(tab1-BAT_AD_VAL)>=120) //��ѹֵ���ڻ�׼ֵ(6v)����1vʱ
                {
                    state =MODE_LOW_POWER;            //����͵�ѹ״̬ �ر����巧
                    low_bat_ad=BAT_AD_VAL;    //�ݴ�
                    if(drv8837_flg == ON)
                    {
                        DRV_8837_CTR(CLOSE_8837);
                    }
                }
            }
            if(state ==MODE_LOW_POWER) //�͵�ѹ״̬
            {
                low_power_time++;
                if(low_power_time>=500)   // 5s �����ѹֵ�ȶ���һ����Χʱ����˵��������Ķϵ�
                {
                    if(iabs(low_bat_ad-BAT_AD_VAL)<=80) //
                    {
                        flg_tab0 = 1;     //����ȷ����ֵ
                        low_power_time =0;
                        state =MODE_WORK;
                    }
                }
                if(iabs(tab1-BAT_AD_VAL)<=50)//�����ѹֵ�ָֻ�����׼ֵ(6v)��Χ��ʱ���л�������ģʽ��˵���ǵ�ѹ���ȶ�
                {
                    low_power_time = 0;
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
