#include    <pic.h>
#include	"I2C.h"
#include    "config.h"

/*******************************************************************************
* Function Name  : I2C_delay
* Description    : Simulation IIC Timing series delay
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_delay(void)
{
    uint8 loops = 0;
	while (loops > 0)
	{
        loops--;
    }
}

uint8	SDA_read(void )
{
	uint8 reData;
	SDA_IN();
	reData = RC3;
	return	reData ;
}


/*******************************************************************************
* Function Name  : I2C_Start
* Description    : Master Start Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : Wheather	 Start
****************************************************************************** */
uint8 I2C_Start( void )
{
	SDA_OUT();
	SDA_H();
	SCL_H( );
	I2C_delay();
//	if( !SDA_read() )return FALSE;	//SDA线为低电平则总线忙,退出
	SDA_L();
	I2C_delay();
//	if( SDA_read() ) return FALSE;		//SDA线为高电平则总线出错,退出
	SCL_L( );
	I2C_delay();
	return 1;
}
/*******************************************************************************
* Function Name  : I2C_Stop
* Description    : Master Stop Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Stop( void )
{
    SDA_OUT();
    SCL_L();
	SDA_L();
	I2C_delay();
	SCL_H();
	SDA_H();
	I2C_delay();
}
/*******************************************************************************
* Function Name  : I2C_Ack
* Description    : Master Send Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Ack( void )
{
	SCL_L( );
	I2C_delay();
	SDA_OUT();
	SDA_L();
	I2C_delay();
	SCL_H();
	I2C_delay();
	SCL_L();
//	I2C_delay();
}
/*******************************************************************************
* Function Name  : I2C_NoAck
* Description    : Master Send No Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_NoAck( void )
{
	SCL_L();
	SDA_OUT();
	I2C_delay();
	SDA_H();
	I2C_delay();
	SCL_H();
	I2C_delay();
	SCL_L();
}
/*******************************************************************************
* Function Name  : I2C_Wait_Ack
* Description    : Master Reserive Slave Acknowledge Single
* Input          : None
* Output         : None
* Return         : Wheather	 Reserive Slave Acknowledge Single
****************************************************************************** */
uint8 I2C_Wait_Ack( void ) 	 //返回为:1=ACK失败,=0无ACK
{
    uint8 ucErrTime = 0;
    SDA_IN();
    SDA_H();
	SCL_H();
	I2C_delay();
	while( SDA_read() )
	{
    	ucErrTime ++ ;
    	if( ucErrTime > 250)
    	{
            I2C_Stop();
            return 1;
        }
	}
	SCL_L();
	return 0;
}
/*******************************************************************************
* Function Name  : I2C_Send_Byte
* Description    : Master Send a Byte to Slave
* Input          : Will Send Date
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Send_Byte(uint8 txd)
{
    uint8 t;
	SDA_OUT();
    SCL_L();
    for(t=0;t<8;t++)
    {
        RC3 = (txd&0x80)>>7;
        txd<<=1;
		I2C_delay();
		SCL_H();
		I2C_delay();
		SCL_L();
		I2C_delay();
    }
}
/*******************************************************************************
* Function Name  : I2C_Read_Byte
* Description    : Master Reserive a Byte From Slave
* Input          : None
* Output         : None
* Return         : Date From Slave
****************************************************************************** */

uint8 I2C_Read_Byte(uint8 ack)
{
	unsigned char i,receive=0;
	SDA_IN();
    for(i=0;i<8;i++ )
	{
        SCL_L(  );
        I2C_delay();
		SCL_H(  );
        receive<<=1;
        if( SDA_read() )receive++;
		I2C_delay();
    }
    if (!ack)
        I2C_NoAck();
    else
        I2C_Ack();
    return receive;
}

//单字节写入*******************************************

uint8 Single_Write(uint8 SlaveAddress,uint16 REG_Address,uint8 REG_data )
{
  	if(!I2C_Start(  ))return 0;
    I2C_Send_Byte(SlaveAddress );   //发送设备地址+写信号//I2C_Send_Byte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//设置高起始地址+器件地址
    if(I2C_Wait_Ack(  )){I2C_Stop( ); return 0;}
    I2C_Send_Byte(  ( REG_Address >> 8 ) & 0xFF );   //设置低起始地址
    I2C_Wait_Ack( );
    I2C_Send_Byte(  REG_Address & 0xFF );   //设置低起始地址
    I2C_Wait_Ack( );
    I2C_Send_Byte(REG_data );
    I2C_Wait_Ack( );
    I2C_Stop( );
 //   delay5ms();
    return 1;
}

uint8 Word_Write(uint8 SlaveAddress,uint16 REG_Address,uint16 REG_data )
{
  	if(!I2C_Start(  ))return 0;
    I2C_Send_Byte(SlaveAddress );   //发送设备地址+写信号//I2C_Send_Byte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//设置高起始地址+器件地址
    if(!I2C_Wait_Ack(  )){I2C_Stop( ); return 0;}
    I2C_Send_Byte(  ( REG_Address >> 8 ) & 0xFF );   //设置低起始地址
    I2C_Wait_Ack( );
    I2C_Send_Byte(  REG_Address & 0xFF );   //设置低起始地址
    I2C_Wait_Ack( );
    I2C_Send_Byte( ( REG_data  >> 8 ) & 0xFF );
    I2C_Wait_Ack( );
    I2C_Send_Byte( REG_data & 0xFF );
    I2C_Wait_Ack( );
    I2C_Stop( );
 //   delay5ms();
    return 1;
}
//多字节写入*****************************************
uint8 Multi_Write(uint8 SlaveAddress,uint8 REG_Address,uint8 len,uint8 *buf)
{
	uint8 i;
    I2C_Start();
	I2C_Send_Byte(SlaveAddress);         //发送器件地址+写命令
	if(I2C_Wait_Ack())	                //等待应答
	{
		I2C_Stop();
		return 1;
	}
    I2C_Send_Byte(REG_Address);	        //写寄存器地址
    I2C_Wait_Ack();		                //等待应答
	for(i=0;i<len;i++)
	{
		I2C_Send_Byte(buf[i]);	        //发送数据
		if(I2C_Wait_Ack())		        //等待ACK
		{
			I2C_Stop();
			return 1;
		}
	}
    I2C_Stop();
	return 0;
}
//单字节读取*****************************************
uint8 Single_Read(uint8 SlaveAddress,uint16 REG_Address )
{
    uint16 REG_data = 0;
	if(!I2C_Start( ))return 0;
    I2C_Send_Byte(SlaveAddress  ); //I2C_Send_Byte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//设置高起始地址+器件地址
    if(!I2C_Wait_Ack( )){I2C_Stop( ); return 0;}
    I2C_Send_Byte(  ( REG_Address >> 8 ) & 0xFF );   //设置低起始地址
    I2C_Wait_Ack( );
    I2C_Send_Byte(  REG_Address & 0xFF );   //设置低起始地址
    I2C_Wait_Ack( );
    I2C_Start( );
    I2C_Send_Byte(SlaveAddress+1 );
    I2C_Wait_Ack( );

	REG_data= I2C_Read_Byte(0 );
    I2C_NoAck( );
    I2C_Stop( );
	return REG_data;

}
//双字节读取*****************************************
uint16 Word_Read(uint8 SlaveAddress,uint16 REG_Address )
{
    uint16 REG_data = 0;
    uint8 reg_data_tmp = 0;
	if(!I2C_Start( ))return 0;
    I2C_Send_Byte(SlaveAddress  ); //I2C_Send_Byte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//设置高起始地址+器件地址
    if(!I2C_Wait_Ack( )){I2C_Stop( ); return 0;}
    I2C_Send_Byte(  ( REG_Address >> 8 ) & 0xFF );   //设置低起始地址
    I2C_Wait_Ack( );
    I2C_Send_Byte(  REG_Address & 0xFF );   //设置低起始地址
    I2C_Wait_Ack( );
    I2C_Start( );
    I2C_Send_Byte(SlaveAddress+1 );
    I2C_Wait_Ack( );

	reg_data_tmp= I2C_Read_Byte(0 );
	I2C_Wait_Ack( );
	//REG_data <<= 8;
	REG_data = ( I2C_Read_Byte( 0 ) << 8 ) | reg_data_tmp ;
    I2C_NoAck( );
    I2C_Stop( );
	return REG_data;
}

//多字节读取*****************************************
void Multi_Read( uint8 SlaveAddress,uint16 REG_Address,uint8 len,uint8 RegData[] )
{
    uint8 REG_data ,i;
	if(!I2C_Start( ))
		return;
    I2C_Send_Byte(SlaveAddress); //I2C_Send_Byte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//设置高起始地址+器件地址
    if(!I2C_Wait_Ack( ))
    {
    	I2C_Stop( );
    	return ;
    }
 //   I2C_Send_Byte(  ( REG_Address >> 8 ) & 0xFF );   //设置低起始地址
 //   I2C_Wait_Ack( );
    I2C_Send_Byte(  REG_Address & 0xFF );   //设置低起始地址
    I2C_Wait_Ack( );
    I2C_Start( );
    I2C_Send_Byte(SlaveAddress+1 );
    I2C_Wait_Ack( );
	for( i = 0; i < ( len - 1 ) ;i ++ )
	{
		RegData[i] = I2C_Read_Byte(0 );
		I2C_Ack();
	}
	RegData[len -1 ] = I2C_Read_Byte(0 );
    I2C_NoAck( );
    I2C_Stop( );
}




