#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <pic.h>
#include <stdio.h>



typedef     char	            int8;
typedef     int		            int16;
typedef     long	            int32;
typedef     unsigned char		uint8;
typedef     unsigned int		uint16;
typedef     unsigned long		uint32;
typedef     unsigned long		ulong;
typedef     unsigned long long	ullong;

typedef union {
	void *	ptr;
	uint32		u;		// u=0x01020304
	int32		i;
	uint16		ush[2];
	int16		sh[2];
	uint8		uch[4];	// ch[0]=0x04 ch[1]=03
	int8		ch[4];
} UN32;

typedef union {
	uint16		ush;
	int16		sh;
	uint8		uch[2];
	int8		ch[2];
} UN16;

#define ON         1
#define OFF        0
#define LED_ON     0 //低电平有效
#define LED_OFF    1

#define OK         1
#define ERR        0




#define DEBUG 					1//串口调试

/*0-初始化，1-电源检测，2-红外自动调准，3-工作检测模式 */
typedef enum
{
   MODE_INIT = 0,
   MODE_ADJ_PS,
   MODE_WORK,
   MODE_ADJ_POWER,
   MODE_ERR,
   MODE_MAX
};

volatile uint8 state;                    //工作状态标志

#endif

