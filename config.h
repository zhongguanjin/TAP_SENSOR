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
#define LED_ON     0 //�͵�ƽ��Ч
#define LED_OFF    1

#define OK         1
#define ERR        0




#define DEBUG 					1//���ڵ���

/*0-��ʼ����1-��Դ��⣬2-�����Զ���׼��3-�������ģʽ */
typedef enum
{
   MODE_INIT = 0,
   MODE_ADJ_PS,
   MODE_WORK,
   MODE_ADJ_POWER,
   MODE_ERR,
   MODE_MAX
};

volatile uint8 state;                    //����״̬��־

#endif

