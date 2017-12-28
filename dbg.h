#ifndef _DBG_H
#define _DBG_H
#include <stdio.h>
#include <stdarg.h>
#include "uart.h"
#include "config.h"

void my_dbg(uint8 *Data,...);



#define dbg (my_dbg("[%s]-[%d]",__FILE__,__LINE__),my_dbg)


#endif

