
#include "LTR507ALS.h"
#include "I2C_Config.h"
#include "Task.h"
#include "adc.h"
#include "uart.h"
#include "timer.h"
#include "dbg.h"
#include "stdio.h"

#define N  10
uint16 PS_BUF[N];
#define BUF_LEN  11

uint8 BUF_DATA[BUF_LEN];

uint8 buf_cnt[4];
uint8 dif_val;

void EepromWriteByte(uint8 addr,uint8 data);
uint8 EepromReadByte(uint8 addr);
void Task_Chk_Bat(void);
void Task_Chk_Man(void);
uint16 PS_AD_AVG(uint16 *p,uint8 len);


TASK_COMPONENTS TaskComps[] =
{
    {0, 100, 100, Task_Chk_Bat},            // 检测电源         100mS检测一次
    {0, 100, 100,  Task_Chk_Man},            // 检测人           100ms检测一次
};

typedef enum _TASK_LIST
{
    TAST_CHK_BAT,              // 检测电源
    TASK_CHK_MAN,             // 检测人
    TASKS_MAX                // 总的可供分配的定时任务数目
} TASK_LIST;



/*****************************************************************************
 函 数 名  : EepromWriteByte
 功能描述  : 写内存函数
 输入参数  : uint8 addr
             uint8 data
 输出参数  :
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月23日 星期二
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void EepromWriteByte(uint8 addr,uint8 data)
{
    do
    {
        ;
    }while(WR);

    EEADRL = addr;
    EEDATL = data;

    WREN = 1;

    EECON2 = 0x55;
    EECON2 = 0xAA;

    WR = 1;

    while(WR);

    if(EEIF == 1)
    {
        EEIF = 0;
    }
    WREN = 0;
}
/*****************************************************************************
 函 数 名  : EepromReadByte
 功能描述  : 读内存函数
 输入参数  : uint8 addr
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月23日 星期二
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
uint8 EepromReadByte(uint8 addr)
{
	do{
		;
	}while(RD);

	EEADRL = addr;

	EEPGD = 0;
	CFGS = 0;

	RD = 1;

	while(RD);

	return EEDATL;

}

void  DRV8837_Init( void )
{
    DRV_IN1_OUT();
    DRV_IN2_OUT();
    DRV_SLEEP_OUT();
    DRV_8837_CTR(CLOSE_8837);
}

void  DRV_8837_CTR(uint8 mode)
{
    if(mode == CLOSE_8837 )
    {
        DRV_SLEEP_PIN = 1;
        DRV_IN1_PIN =0;
        DRV_IN2_PIN =1;
        delay_ms(50);
        DRV_IN1_PIN =0;
        DRV_IN2_PIN =0;
    }
     if(mode == OPEN_8837)
	{
        DRV_SLEEP_PIN = 1;
        DRV_IN1_PIN =1;
        DRV_IN2_PIN =0;
        delay_ms(50);
        DRV_IN1_PIN =0;
        DRV_IN2_PIN =0;
	}
	if(mode == SLEEP_8837)
	{
		DRV_SLEEP_PIN = 0; //进入sleep
        DRV_IN1_PIN =0;
        DRV_IN2_PIN =0;
	}
}
/*****************************************************************************
 函 数 名  : drain_check
 功能描述  : 判断冲水类别函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年7月14日 星期五
    作    者   : lcd初始化函数
    修改内容   : 新生成函数

*****************************************************************************/
void drain_check (void)
{
    if(Man_Stay == MAN_HERE)
    {
        Timer_Stay ++;
        if(check_first_flg == 1)
        {
           check_first_flg =0;
           DRV_8837_CTR(OPEN_8837);
        }
        if((Timer_Stay >= TIME_OUT)&&(check_60s_flg==0))//60s
        {
            check_60s_flg =1;
            DRV_8837_CTR(CLOSE_8837);
        }
    }
    if(Man_Stay == MAN_LEAVE)
    {
         Timer_Stay = 0;
         check_60s_flg =0;
         Man_Stay = MAN_IDLE;
         DRV_8837_CTR(CLOSE_8837);
    }
}


/*****************************************************************************
 函 数 名  : get_avg
 功能描述  : 计算平均值函数
 输入参数  :

 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年7月14日 星期五
    作    者   : lcd初始化函数
    修改内容   : 新生成函数

*****************************************************************************/
uint16 PS_AD_AVG(uint16 *p,uint8 len)
{
    uint16 ad_max,ad_min;
    uint32  sum = 0;
    uint16 *temp =p;
    for ( uint8 i=0;i<len;i++)
    {
        ad_max = temp[0];
        ad_min = temp[1];
       if(ad_max < temp[i])
       {
           ad_max = temp[i];
       }
       if(ad_min > temp[i])
       {
           ad_min = temp[i];
       }
        dif_val =ad_max-ad_min;
        sum += temp[i];
    }
    return (uint16)(sum/len);
}

/*****************************************************************************
 函 数 名  : Auto_ADJ_PSD
 功能描述  : 初始化红外自动校准函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月18日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/

void Auto_ADJ_PS(void)
{
	static uint16 PS_ADJ_DATA=0;
    static uint8 index =0;
    static uint16 adj_time_cnt=0;
    if((adj_time_cnt++)<=600)//100ms*600=60s
    {
        PS_BUF[index++]= Get_PS_DATA();
        if(index == N)
        {
            index=0;
            PS_ADJ_DATA = PS_AD_AVG(PS_BUF, N);
            if((PS_ADJ_DATA>=PS_MIN_DAT)&&(PS_ADJ_DATA<=PS_MAX_DAT))
            {
                if(dif_val<=5)
                {
                    PS_DATA_H = (PS_ADJ_DATA + PS_ADJ_DATA/30);
                    PS_DATA_L = (PS_ADJ_DATA + PS_ADJ_DATA/50);
                   /* buf_cnt[0] =(PS_DATA_L&0xff00)>>8;
                    buf_cnt[1] = PS_DATA_L&0x00ff;
                    buf_cnt[2] =(PS_DATA_H&0xff00)>>8;
                    buf_cnt[3] = PS_DATA_H&0x00ff;
                    EepromWriteByte(eeprom_addr, buf_cnt[0]);
                    EepromWriteByte(eeprom_addr+0x01, buf_cnt[1]);
                    EepromWriteByte(eeprom_addr+0x02, buf_cnt[2]);
                    EepromWriteByte(eeprom_addr+0x03, buf_cnt[3]);
                    */
                    adj_ok_flg =1;
                    state = MODE_WORK;
                 }
            }
            else
            {
                PS_ADJ_DATA = 0;
            }
        }
    }
    else
    {
        adj_time_cnt = 0;
        PS_DATA_H = (PS_DEF_DAT + PS_DEF_DAT/30);
        PS_DATA_L = (PS_DEF_DAT + PS_DEF_DAT/50);
        /*
        buf_cnt[0] =(PS_DATA_L&0xff00)>>8;
        buf_cnt[1] = PS_DATA_L&0x00ff;
        buf_cnt[2] =(PS_DATA_H&0xff00)>>8;
        buf_cnt[3] = PS_DATA_H&0x00ff;
        EepromWriteByte(eeprom_addr, buf_cnt[0]);
        EepromWriteByte(eeprom_addr+0x01, buf_cnt[1]);
        EepromWriteByte(eeprom_addr+0x02, buf_cnt[2]);
        EepromWriteByte(eeprom_addr+0x03, buf_cnt[3]);
        */
        adj_ok_flg =1;
        state = MODE_WORK;
    }
}

/*****************************************************************************
 函 数 名  : man_sta
 功能描述  : 人状态检测函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年9月9日
    作    者   : man_sta
    修改内容   : 新生成函数

*****************************************************************************/
void man_sta(void)
{
    static uint8 TmpA =0,TmpB=0;
    if((PS_DATA >= PS_DATA_H)&& (Man_Stay==MAN_IDLE))
    {
        TmpA++;
        TmpB = 0;
        if(TmpA>=TIMER_Sensitive) //0.2s+延时周期
        {
            TmpA = 0;
            check_first_flg = 1;
            Man_Stay = MAN_HERE;    //人在洗手
            dbg("man get,%d\r\n",PS_DATA);
        }
    }
    if((PS_DATA <= PS_DATA_L)&&(Man_Stay==MAN_HERE))//
    {
        TmpB++;
        TmpA = 0;
        if(TmpB >= TIMER_Sensitive)// 人离开时间为0.2s+延时周期
        {
            TmpB = 0;
            Man_Stay = MAN_LEAVE; //人洗手离开
            dbg("man leave,%d\r\n",PS_DATA);
        }
    }

}

/*****************************************************************************
 函 数 名  : Task_Chk_Man
 功能描述  : 人检测任务函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月18日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void Task_Chk_Man(void)
{
   if(state == MODE_WORK) //工作模式
    {
        for(uint8 cnt=N;cnt>0;cnt--)
        {
          PS_BUF[cnt]=PS_BUF[cnt-1];
        }
        PS_BUF[0]= Get_PS_DATA();
        PS_DATA = PS_AD_AVG(PS_BUF,N);
        /*
        if((PS_DATA >= 1000)&&(LTR507_Read_Byte(PS_LED)&0x03)==LED_CUR_50MA)//检测距离<15cm ,切换20ma的发射功率
        {
            ps_led_ctr(LED_FREQ_60K, LED_CUR_20MA); //采用20ma档位
            delay_ms(10);
        }
        else if((LTR507_Read_Byte(PS_LED)&0x03)!=LED_CUR_50MA)
        {
            ps_led_ctr(LED_FREQ_60K, LED_CUR_50MA); //采用50ma档位
            delay_ms(10);
        }*/
        BUF_DATA[0]=0x0D;
        BUF_DATA[1]=0x0A;
        BUF_DATA[2]=(PS_DATA&0xFF00)>>8;
        BUF_DATA[3]=PS_DATA&0x00FF;
        BUF_DATA[4]=(PS_DATA_L&0xFF00)>>8;
        BUF_DATA[5]=PS_DATA_L&0x00FF;
        BUF_DATA[6]=(PS_DATA_H&0xFF00)>>8;
        BUF_DATA[7]=PS_DATA_H&0x00FF;
        BUF_DATA[8]=0xFF;
        BUF_DATA[9]=0x0F;
        BUF_DATA[10]=0x0F;
        //uart_send_data(BUF_DATA,BUF_LEN);
        man_sta();
        drain_check();
    }
}

/*****************************************************************************
 函 数 名  : Task_Chk_Bat
 功能描述  : 电源检测任务函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月18日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void Task_Chk_Bat(void)
{
    BAT_AD_VAL =Read_BAT();
    if( BAT_AD_VAL <= 850) //电压 < 5v左右 关闭脉冲阀
   	{
   	   state =MODE_ERR;
       DRV_8837_CTR(CLOSE_8837);
   	}
   	else if(state ==MODE_ERR)
   	{
   	    state =MODE_WORK;
   	}
    if(state == MODE_ADJ_PS)
	{
	/*
	    ps_adj_user_l= ((EepromReadByte(eeprom_addr)&0x00ff)<<8)+(EepromReadByte(eeprom_addr+0x01)&0x00ff);
	    ps_adj_user_h=((EepromReadByte(eeprom_addr+0x02)&0x00ff)<<8)+(EepromReadByte(eeprom_addr+0x03)&0x00ff);
	    if((ps_adj_user_l==0xFFFF)||(ps_adj_user_h==0xFFFF)) //初次使用
	    {
	        Auto_ADJ_PS();
	    }
	    else
	    {
	       PS_DATA_L = ps_adj_user_l;
	       PS_DATA_H =ps_adj_user_h;
           state = MODE_WORK;
	    }
	    */
        //PS_DATA_L = 530;
        //PS_DATA_H = 560;
        //state = MODE_WORK;
        Auto_ADJ_PS(); //每次上电都重新确定阀值

	}
}

/*****************************************************************************
 函 数 名  : TaskRemarks
 功能描述  : 任务标记函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月23日 星期二
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void TaskRemarks(void)
{
    uint8 i;
    for (i=0; i<TASKS_MAX; i++)          // 逐个任务时间处理
    {
         if (TaskComps[i].Timer)          // 时间不为0
        {
            TaskComps[i].Timer--;         // 减去一个节拍
            if (TaskComps[i].Timer == 0)       // 时间减完了
            {
                 TaskComps[i].Timer = TaskComps[i].ItvTime;       // 恢复计时器值，从新下一次
                 TaskComps[i].Run = 1;                           // 任务可以运行
            }
        }
   }
}

/*****************************************************************************
 函 数 名  : TaskProcess
 功能描述  : 任务处理函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月23日 星期二
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void TaskProcess(void)
{
    uint8 i;
    for (i=0; i<TASKS_MAX; i++)           // 逐个任务时间处理
    {
         if (TaskComps[i].Run)            // 时间不为0
        {
             TaskComps[i].TaskHook();      // 运行任务
             TaskComps[i].Run = 0;         // 标志清0
        }
    }
}



