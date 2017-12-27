#ifndef __USART_H__
#define __USART_H__

#include "config.h"

extern void Init_Uart(void);
extern void uart_send_byte(uint8 dat);
extern void uart_send_str( uint8 *s);
extern void uart_send_data(void *p,uint8 len);
#endif


