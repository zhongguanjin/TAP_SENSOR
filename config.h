#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <pic.h>
#include <stdio.h>


typedef char	int8;
typedef int		int16;
typedef long	int32;
typedef unsigned char		uint8;
typedef unsigned int		uint16;
typedef unsigned long		uint32;
typedef unsigned long		ulong;
typedef unsigned long long	ullong;


#define ON         1
#define OFF        0
#define LED_ON     0 //低电平有效
#define LED_OFF    1





#define DEBUG 					1//串口调试

/*0-初始化，1-电源检测，2-红外自动调准，3-工作检测模式 */
typedef enum
{
   MODE_INIT = 0,
   MODE_ADJ_PS,
   MODE_WORK,
   MODE_ERR,
   MODE_MAX
};

volatile uint8 state;                    //工作状态标志

#endif

