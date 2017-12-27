#ifndef __TIMER_H__
#define __TIMER_H__

#include "config.h"


extern void Init_TMR0(void);

//extern void Init_TMR2(void);
#define  _XTAL_FREQ   ((double)8000000)

#define delay_us(x) __delay_us(x)

#define delay_ms(x) __delay_ms(x)



#endif
