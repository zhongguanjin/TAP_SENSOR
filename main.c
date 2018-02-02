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
    if(ltr507_init()==OK)
    {
        state = MODE_INIT;
    }
    delay_ms(20);
	Init_TMR0();
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
	dbg("init ok\r\n");
	HAL248_IN(); // 霍尔开关配置成输入
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
	    TaskRemarks();  //任务标记轮询处理
	}
}
