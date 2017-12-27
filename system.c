/*************************************************************************************
* system.c
*
*
***************************************************************************************/

#include "timer.h"
#include "uart.h"
#include "system.h"
#define  SYSCLK_FREQ_8MHz 1


/**************************************************************************************************/
static void system_clock_init(void)
{
	OSCSTAT = 0;
	#if(SYSCLK_FREQ_32MHz == 1)
		OSCCON = 0b11110010;		//使用内部8M晶振,使能PLL
	#elif(SYSCLK_FREQ_16MHz== 1)
		OSCCON = 0b01111010;		//使用内部16M晶振
	#elif(SYSCLK_FREQ_8MHz == 1)
		OSCCON = 0b01110010;		//使用内部8M晶振
	#elif(SYSCLK_FREQ_4MHz == 1)
		OSCCON = 0b01101010;		//使用内部4M晶振
    #elif( SYSCLK_FREQ_2MHz  == 1)
		OSCCON = 0b01100010; 		//使用内部2M晶振
    #else
		OSCCON = 0b01011010;		//使用内部1M晶振
	#endif
 	while(!OSCSTATbits.HFIOFR);		//等待晶振稳定
}


/*****************************************************************************
 函 数 名  : Init_MCU
 功能描述  : mcu初始化函数
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
void Init_MCU(void)
{
	system_clock_init(); //使用内部4M晶振
	//APFCON0
	TRISA = 0b11111111;	//PORTA 输入
	PORTA = 0x00;
	ANSELA = 0b11111111;//模拟输入
	WPUA = 0x00;		//禁止上拉



	TRISC = 0b00000000;
	PORTC = 0b00000000;
	WPUC = 0x00;

	IOCAP = 0x00;		//0 = 禁止关联引脚的电平变化中?
	IOCAN = 0x00;		//0 = 禁止关联引脚的电平变化中?

	FVRCON=0b11000010;              //使能固定参考电压，ADC参考电?2.048V
	//01 = 比较器和DAC 固定参考电压外设输出为1x ?.024V?
	//01 = ADC 固定参考电压外设输出为1x ?.024V?

	//disable unused module
	DACCON0 = 0x00;		//0 = 禁止DAC
	DACCON1 = 0x00;		//

	SRCON0 = 0x00;		//0 = 禁止SR 锁存?

	CCP1CON = 0x00;
	CCP2CON = 0x00;

	CM1CON0 = 0x00; 	//0 = 禁止比较?
	CM2CON0 = 0x00; 	//0 = 禁止比较?

	OPTION_REG = 0b10000011;
	//1 = 禁止所有弱上拉（MCLR 除外，如果已使能?
	//Timer0 时钟源选择?0 = 内部指令周期时钟（FOSC/4?
	//011 预分频比选择?1 : 16
	T1CON = 0x00;
	T2CON = 0x00;
	T4CON = 0x00;
	T6CON = 0x00;
	MDCON = 0x00;		//0 = 禁止调制器模块，不产生任何输?
}


