
#include "uart.h"


/*****************************************************************************
 函 数 名  : Init_Uart
 功能描述  : uart初始化函数
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
void Init_Uart(void)
{
	//配置RA0/RA1作为UART通信?
	RXDTSEL = 1;		//1 = RA1 上具有RX/DT 功能
	TXCKSEL = 1;		//1 = RA0 上具有TX/CK 功能
	//设置波特?600 @ 8MHZ, 8bit, NC
	BRGH = 0;
	BRG16= 0;
	SPBRGH=0X00;
	SPBRGL=12;
    //发送模式配?
    TXSTA = 0x20;
    //接收模式配置
    RCSTA =0x90;
    TXIF = 0;
    TXIE = 0;           // 关闭发送中断
	RCIF = 0;
	RCIE = 0;			// 接收中断

}
/*****************************************************************************
 函 数 名  : uart_send_byte
 功能描述  : 串口发送一字节函数
 输入参数  : uint8 dat
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月18日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void uart_send_byte(uint8 dat)
{
	while(!TRMT)		//TRMT=0:正在发送，TRMT=1:发送已完成
	{
		continue;
	}
	TXREG=dat;
}


/*****************************************************************************
 函 数 名  : uart_send_str
 功能描述  : 发送字符函数
 输入参数  : uint8 *s
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月27日 星期六
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void uart_send_str( uint8 *s)
{
    while(*s!='\0')   // \0表示字符串结束标志，通过检测是否字符串末尾
    {
       uart_send_byte(*s++);
    }
}

void uart_send_data(void *p,uint8 len)
{
    uint8 *temp = p;
    for(uint8 j=0;j<len;j++)
    {
       uart_send_byte(*temp++);
    }
}


