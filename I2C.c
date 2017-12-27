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
//	if( !SDA_read() )return FALSE;	//SDA��Ϊ�͵�ƽ������æ,�˳�
	SDA_L();
	I2C_delay();
//	if( SDA_read() ) return FALSE;		//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
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
uint8 I2C_Wait_Ack( void ) 	 //����Ϊ:1=ACKʧ��,=0��ACK
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

//���ֽ�д��*******************************************

uint8 Single_Write(uint8 SlaveAddress,uint16 REG_Address,uint8 REG_data )
{
  	if(!I2C_Start(  ))return 0;
    I2C_Send_Byte(SlaveAddress );   //�����豸��ַ+д�ź�//I2C_Send_Byte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//���ø���ʼ��ַ+������ַ
    if(I2C_Wait_Ack(  )){I2C_Stop( ); return 0;}
    I2C_Send_Byte(  ( REG_Address >> 8 ) & 0xFF );   //���õ���ʼ��ַ
    I2C_Wait_Ack( );
    I2C_Send_Byte(  REG_Address & 0xFF );   //���õ���ʼ��ַ
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
    I2C_Send_Byte(SlaveAddress );   //�����豸��ַ+д�ź�//I2C_Send_Byte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//���ø���ʼ��ַ+������ַ
    if(!I2C_Wait_Ack(  )){I2C_Stop( ); return 0;}
    I2C_Send_Byte(  ( REG_Address >> 8 ) & 0xFF );   //���õ���ʼ��ַ
    I2C_Wait_Ack( );
    I2C_Send_Byte(  REG_Address & 0xFF );   //���õ���ʼ��ַ
    I2C_Wait_Ack( );
    I2C_Send_Byte( ( REG_data  >> 8 ) & 0xFF );
    I2C_Wait_Ack( );
    I2C_Send_Byte( REG_data & 0xFF );
    I2C_Wait_Ack( );
    I2C_Stop( );
 //   delay5ms();
    return 1;
}
//���ֽ�д��*****************************************
uint8 Multi_Write(uint8 SlaveAddress,uint8 REG_Address,uint8 len,uint8 *buf)
{
	uint8 i;
    I2C_Start();
	I2C_Send_Byte(SlaveAddress);         //����������ַ+д����
	if(I2C_Wait_Ack())	                //�ȴ�Ӧ��
	{
		I2C_Stop();
		return 1;
	}
    I2C_Send_Byte(REG_Address);	        //д�Ĵ�����ַ
    I2C_Wait_Ack();		                //�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		I2C_Send_Byte(buf[i]);	        //��������
		if(I2C_Wait_Ack())		        //�ȴ�ACK
		{
			I2C_Stop();
			return 1;
		}
	}
    I2C_Stop();
	return 0;
}
//���ֽڶ�ȡ*****************************************
uint8 Single_Read(uint8 SlaveAddress,uint16 REG_Address )
{
    uint16 REG_data = 0;
	if(!I2C_Start( ))return 0;
    I2C_Send_Byte(SlaveAddress  ); //I2C_Send_Byte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//���ø���ʼ��ַ+������ַ
    if(!I2C_Wait_Ack( )){I2C_Stop( ); return 0;}
    I2C_Send_Byte(  ( REG_Address >> 8 ) & 0xFF );   //���õ���ʼ��ַ
    I2C_Wait_Ack( );
    I2C_Send_Byte(  REG_Address & 0xFF );   //���õ���ʼ��ַ
    I2C_Wait_Ack( );
    I2C_Start( );
    I2C_Send_Byte(SlaveAddress+1 );
    I2C_Wait_Ack( );

	REG_data= I2C_Read_Byte(0 );
    I2C_NoAck( );
    I2C_Stop( );
	return REG_data;

}
//˫�ֽڶ�ȡ*****************************************
uint16 Word_Read(uint8 SlaveAddress,uint16 REG_Address )
{
    uint16 REG_data = 0;
    uint8 reg_data_tmp = 0;
	if(!I2C_Start( ))return 0;
    I2C_Send_Byte(SlaveAddress  ); //I2C_Send_Byte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//���ø���ʼ��ַ+������ַ
    if(!I2C_Wait_Ack( )){I2C_Stop( ); return 0;}
    I2C_Send_Byte(  ( REG_Address >> 8 ) & 0xFF );   //���õ���ʼ��ַ
    I2C_Wait_Ack( );
    I2C_Send_Byte(  REG_Address & 0xFF );   //���õ���ʼ��ַ
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

//���ֽڶ�ȡ*****************************************
void Multi_Read( uint8 SlaveAddress,uint16 REG_Address,uint8 len,uint8 RegData[] )
{
    uint8 REG_data ,i;
	if(!I2C_Start( ))
		return;
    I2C_Send_Byte(SlaveAddress); //I2C_Send_Byte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//���ø���ʼ��ַ+������ַ
    if(!I2C_Wait_Ack( ))
    {
    	I2C_Stop( );
    	return ;
    }
 //   I2C_Send_Byte(  ( REG_Address >> 8 ) & 0xFF );   //���õ���ʼ��ַ
 //   I2C_Wait_Ack( );
    I2C_Send_Byte(  REG_Address & 0xFF );   //���õ���ʼ��ַ
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




