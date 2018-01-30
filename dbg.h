#ifndef _DBG_H
#define _DBG_H
#include <stdio.h>
#include <stdarg.h>
#include "uart.h"
#include "config.h"



void  my_printf(const char *pFormat, ...);
void  my_dbg(const char *pFormat, ...);




//void my_print(uint8 *Data,...);


#define dbg  (my_dbg("[%d]",__LINE__),my_dbg)


#endif

