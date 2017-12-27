#ifndef	__I2C_H__
#define	__I2C_H__

#include    "config.h"

/*模拟IIC端口输出输入定义*/

void        I2C_delay(void);





#define		I2CInit()			{	TRISC3 = 0; TRISC4 = 0; TRISA5 = 1; TRISC5 = 0;ANSC3 = 0;\
                                    LATC3 = 1; LATC4 = 1;LATC5 = 1;\
                                }
#define     RESET_HIGH()        {  LATC5 = 1;}

#define 	SCL_H()         	{ 	TRISC4 = 0; RC4 = 1; }
#define 	SCL_L()         	{ 	TRISC4 = 0; RC4 = 0; }


#define 	SDA_IN()         	{ 	TRISC3 = 1; } //C2
#define 	SDA_OUT()         	{ 	TRISC3 = 0; } //C2
#define 	SDA_H()         	{ 	TRISC3 = 0;RC3 = 1; } //1
#define 	SDA_L()         	{ 	TRISC3 = 0;RC3 = 0; }


uint8       Single_Read(uint8 SlaveAddress,uint16 REG_Address );
void 	    Multi_Read( uint8 SlaveAddress,uint16 REG_Address,uint8 len,uint8 RegData[] );
uint8 	    Single_Write(uint8 SlaveAddress,uint16 REG_Address,uint8 REG_data );
uint8       Multi_Write(uint8 SlaveAddress,uint8 REG_Address,uint8 len,uint8 *buf);
uint16 	    Word_Read(uint8 SlaveAddress,uint16 REG_Address );


uint8       I2C_Start( void );
void        I2C_Stop( void );
uint8       I2C_Wait_Ack( void );
void        I2C_Send_Byte(uint8 SendByte);
uint8 I2C_Read_Byte(uint8 ack );
#endif