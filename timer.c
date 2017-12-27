/***************************************************************************************
* timer.c
*
*
*****************************************************************************************/

#include "timer.h"

/*****************************************************************************
 函 数 名  : Init_TMR0
 功能描述  : 定时器0初始化函数
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
void Init_TMR0(void)
{
	TMR0CS	= 0;				//内部指令周期时钟（FOSC/4=2MHz 0.5us
	TMR0SE	= 0;				//在T0CKI 引脚信号从低至高跳变时，递增计数
	PSA		= 0;				//预分频器分配给Timer0 模块
	PS2		= 0;
	PS1		= 1;
	PS0		= 1;				//预分频比1:16 0.5us*16 = 8us
	TMR0IF	= 0;
	TMR0IE	= 1;				//打开TMR0中断
	TMR0	= 0x7D;				//TMR0计数255(0xff)-(1ms/8us) = 130 =0X82定时1ms产生一个中断
	GIE		= 1;
	PEIE	= 1;
}





