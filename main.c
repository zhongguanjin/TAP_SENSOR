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
 函 数 名  : Init_Sys
 功能描述  : 系统初始化函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月18日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

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
 函 数 名  : main
 功能描述  : 主函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月18日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
uint8 flg_tab0 = 1;
uint16 low_power_time = 0;
uint16 low_bat_ad=0;
/*****************************************************************************
 函 数 名  : iabs
 功能描述  : 求绝对值函数
 输入参数  : int a
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年1月3日
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/

void main(void)
{
	Init_Sys();
	//asm("sleep"); //休眠
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
            if(state == MODE_WORK)//工作状态
            {
                if((tab1>BAT_AD_VAL)&&(tab1-BAT_AD_VAL)>=120) //电压值低于基准值(6v)超过1v时
                {
                    state =MODE_LOW_POWER;            //进入低电压状态 关闭脉冲阀
                    low_bat_ad=BAT_AD_VAL;    //暂存
                    if(drv8837_flg == ON)
                    {
                        DRV_8837_CTR(CLOSE_8837);
                    }
                }
            }
            if(state ==MODE_LOW_POWER) //低电压状态
            {
                low_power_time++;
                if(low_power_time>=500)   // 5s 后检测电压值稳定在一定范围时，就说明不是真的断电
                {
                    if(iabs(low_bat_ad-BAT_AD_VAL)<=80) //
                    {
                        flg_tab0 = 1;     //重新确定阀值
                        low_power_time =0;
                        state =MODE_WORK;
                    }
                }
                if(iabs(tab1-BAT_AD_VAL)<=50)//如果电压值又恢复到基准值(6v)范围内时，切换成正常模式，说明是电压不稳定
                {
                    low_power_time = 0;
                    state =MODE_WORK;
                }
            }
        }
        /* END:   Added by zgj, 2018/1/3 */
	   TaskProcess();            // 任务处理
	   CLRWDT();
	}
}

/*****************************************************************************
 函 数 名  : ISR
 功能描述  : 中断服务函数
 输入参数  : void
 输出参数  : 无
 返 回 值  : void
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月18日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

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
	    TaskRemarks();  //任务标记轮询处理
	}
}
