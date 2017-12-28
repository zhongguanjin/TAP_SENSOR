#include "Task.h"
#include "main.h"
#include "uart.h"
#include "timer.h"
#include "system.h"
#include "adc.h"
#include "LTR507ALS.h"
#include "I2C_Config.h"

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
    delay_ms(100);
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
	static uint16 t_1min=0;
	if(TMR0IF && TMR0IE)
	{
		TMR0IF = 0;
		TMR0 = TMR0+TMR0_VALUE;
		if(t_1min >= 60000)
		{
			t_1min = 0;
			f1min = 1;
		}
	    TaskRemarks();  //任务标记轮询处理
	}
}
