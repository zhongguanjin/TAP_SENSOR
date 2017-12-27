#include "adc.h"
#include "system.h"
#include "Task.h"
#include "uart.h"

/*****************************************************************************
 函 数 名  : Init_ADC
 功能描述  : adc初始化函数
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
void Init_ADC(void)
{
    /*A/D 控制寄存器0*/
	ADCON0 = 0b00001001; //bit0-ADON ADC 使能位, bit1-GO/DONE A/D 转换状态位 通道选择AN2
	/*A/D 控制寄存器1*/          //001 fosc/8  bit7:A/D 结果格式选择位
	ADCON1 = 0b10010011;		//bit0-1:A/D 正参考电压配置位,bit2:A/D 负参考电压配置位,bit 6-4:A/D 转换时钟选择位 FOSC/8
	/*A/D结果寄存器*/
	ADRESH = 0x00;
	ADRESL = 0x00;
	ADIE	= 0;
	ADIF	= 0;
	TRISA2 = 1;
    ANSA2 = 1;
}


/*****************************************************************************
 函 数 名  : Read_BAT
 功能描述  : 读取电源AD值函数
 输入参数  : AD_VAL
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月18日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
uint16 Read_BAT(void)
{
	uint8 ADDelay=0;
    uint16  AD_VAL=0;
	asm("NOP");
	asm("NOP");
	asm("NOP");
	GO = 1;
	ADDelay = 0;
	while(GO)
	{
		ADDelay ++;
		if(ADDelay>80)
		{
			break;
		}
	}
	if(GO != 1)
	{
		AD_VAL = (uint16)(ADRESH<<8) + ADRESL;
	}
	return AD_VAL;
}

