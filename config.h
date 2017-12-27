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
#define LED_ON     0 //�͵�ƽ��Ч
#define LED_OFF    1





#define DEBUG 					1//���ڵ���

/*0-��ʼ����1-��Դ��⣬2-�����Զ���׼��3-�������ģʽ */
typedef enum
{
   MODE_INIT = 0,
   MODE_ADJ_PS,
   MODE_WORK,
   MODE_ERR,
   MODE_MAX
};

volatile uint8 state;                    //����״̬��־

#endif

