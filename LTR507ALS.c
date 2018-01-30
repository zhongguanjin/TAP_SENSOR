
#include "LTR507ALS.h"
#include "I2C_Config.h"
#include "uart.h"
#include "timer.h"

//#define reg_len 20
//uint8 reg_dat[reg_len];

uint16 Get_PS_DATA(void)
{
    uint8 data0 = 0,data1 =0;
    uint16 PS_DAT = 0;
    data0 = LTR507_Read_Byte(PS_DATA_0);
    data1 = LTR507_Read_Byte(PS_DATA_1);
    PS_DAT=((data1&0x07)<<8)+data0;
    return PS_DAT;
}

uint16 Get_ALS_DATA(void)
{
    uint8 data0 = 0,data1 =0;
    uint16 ALS_DATA = 0;
    data0 = LTR507_Read_Byte(ALS_DATA_0);
    data1 = LTR507_Read_Byte(ALS_DATA_1);
    ALS_DATA=(data1<<8)+data0;
    return ALS_DATA;
}

//IIC写一个字节
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
uint8 LTR507_Write_Byte(uint8 reg,uint8 data)
{
    IIC_Start();
	IIC_Send_Byte( LTR507_Add );       //发送器件地址+写命令
	if(IIC_Wait_Ack())	                //等待应答
	{
		IIC_Stop();
		return 1;
	}
    IIC_Send_Byte(reg);	                //写寄存器地址
    IIC_Wait_Ack();		                //等待应答
	IIC_Send_Byte(data);                //发送数据
	if(IIC_Wait_Ack())	                //等待ACK
	{
		IIC_Stop();
		return 1;
	}
    IIC_Stop();
	return 0;
}

//IIC读一个字节
//reg:寄存器地址
//返回值:读到的数据
uint8 LTR507_Read_Byte(uint8 reg)
{
	uint8 res;
    IIC_Start();
	IIC_Send_Byte(LTR507_Add);             //发送器件地址+写命令
	IIC_Wait_Ack();		                    //等待应答
    IIC_Send_Byte(reg);	                    //写寄存器地址
    IIC_Wait_Ack();		                    //等待应答
    IIC_Start();
	IIC_Send_Byte(LTR507_Add+1 );          //发送器件地址+读命令
    IIC_Wait_Ack();		                    //等待应答
	res=IIC_Read_Byte(0);                   //读取数据,发送nACK
    IIC_Stop();			                    //产生一个停止条件
	return res;
}

//ps_led 发射功率设置
void ps_led_ctr(uint8 freq, uint8 cur)
{
    uint8 dat;
    dat= ((freq<<5)&0xE0)+0X08+cur;
    LTR507_Write_Byte(PS_LED,dat);
}
//als 使能
void als_contr_mode( uint8 mode,uint8 gain)
{
    if( ACTIVE_MODE == mode )
    {
        LTR507_Write_Byte( ALS_CONTR,0x02+(gain<<3)); //active mode
    }
    else
    {
        LTR507_Write_Byte( ALS_CONTR,(0x00+(gain<<3))); //standy mode
    }
}

// PS 使能
void ps_contr_mode( uint8 mode )
{
    if( ACTIVE_MODE == mode )
    {
        LTR507_Write_Byte( PS_CONTR,0x0E); //active mode
    }
    else
    {
        LTR507_Write_Byte( PS_CONTR,0x0C); //standy mode
    }
}


uint8 ltr507_init(void)
{
    //ps_contr_mode(STANDY_MODE);
    //LTR507_Write_Byte( ALS_CONTR,0x00); //standy mode
    ps_contr_mode(ACTIVE_MODE);
    delay_ms(20);
    LTR507_Write_Byte( ALS_CONTR,0x00);
    delay_ms(20);
    //设置led的电流及频率 DEFAULT
    ps_led_ctr(LED_FREQ_60K, LED_CUR_50MA);
    LTR507_Write_Byte( PS_N_PULSES,0x7F);       // 设置LED发射脉冲数 127
    LTR507_Write_Byte( PS_MEAS_RATE,0x03);      //设置PS测量速率 100ms
    LTR507_Write_Byte( ALS_MEAS_RATE,0X82);     //设置ALS ADC 分辨率 16bit 500ms
    LTR507_Write_Byte( ALS_COEFF1_DATA_0,0X80);    // 设置ALS_DATA_CH1的序数
    LTR507_Write_Byte( ALS_COEFF1_DATA_1,0X03);    // 设置ALS_DATA_CH1的序数
    LTR507_Write_Byte( ALS_COEFF2_DATA_0,0XC8);    // 设置ALS_DATA_CH2的序数
    LTR507_Write_Byte( ALS_COEFF2_DATA_1,0XFB);    // 设置ALS_DATA_CH2的序数
    LTR507_Write_Byte( ALS_IRF_CUT_OFF,0XD0);    // 设置ALS_DATA的极限值
    LTR507_Write_Byte( INTERRUPT,0X08);         // 设置中断
    LTR507_Write_Byte( PS_THRES_UP_0,0XFF);    // 设置PS_data 上限阀值
    LTR507_Write_Byte( PS_THRES_UP_1,0X07);
    LTR507_Write_Byte( PS_THRES_LOW_0,0X00);    // 设置PS_data 下限阀值
    LTR507_Write_Byte( PS_THRES_LOW_1,0X00);
    delay_ms(5);
    if(Get_PS_DATA()!=0)
    {
        return OK;
    }
    else
    {
        return ERR;
    }
}




