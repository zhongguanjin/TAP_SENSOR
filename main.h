#ifndef __MAIN_H__
#define __MAIN_H__

#include "config.h"

__CONFIG(0x0EA4);       //ÅäÖÃ×Ó
__CONFIG(0x3EFF);

__EEPROM_DATA( 0xAA,0x55,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF );   //0x00-0x07


#define TMR0_VALUE				0x82



/*º¯ÊýÉùÃ÷  */
void Init_Sys(void);


#endif

