#include <stdio.h>
#include <stdarg.h>

#include "uart.h"




#define DEBUG_ENABLE

#ifndef DEBUG_ENABLE
#define my_printf(...)
#define dbg(...)
#else
void my_printf(const char *, ...);
void dbg(const char *, ...);
#endif

/*
#define DEBUG_ENABLE

#ifndef DEBUG_ENABLE
#define dbg(pFormat,...)	    my_dbg("[%s]:" pFormat "\r\n",__func__,...)
#define chk(x);				    if(x)my_dbg("[Chk]%s:%d\r\n",__FILE__,(int)__LINE__);
#else
#define dbg(pFormat,...)	    my_dbg("[%s]:" pFormat "\r\n",__func__,...)
#define chk(x);				    if(x)my_dbg("[Chk]%s:%d\r\n",__FILE__,(int)__LINE__);
#endif
*/




