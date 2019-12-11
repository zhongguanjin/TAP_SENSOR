
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
void Task_State_Hal(void);
void Task_Chk_Man(void);
uint16 PS_AD_AVG(uint16 *p,uint8 len);
void hal248_check(void);
void led2_flash(void);


TASK_COMPONENTS TaskComps[] =
{
    {0, 10, 10, Task_State_Hal},            // 检测         10mS检测一次
    {0, 50, 50,  Task_Chk_Man},            // 检测人        50ms检测一次
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
    //TMR0IE	= 0;
    if(mode == CLOSE_8837 )
    {
        drv8837_flg =OFF;
        DRV_SLEEP_PIN = 1;
        DRV_IN1_PIN =0;
        DRV_IN2_PIN =1;
        delay_ms(20);
        DRV_IN1_PIN =0;
        DRV_IN2_PIN =0;
	    DRV_SLEEP_PIN = 0; //进入sleep
    }
    if(mode == OPEN_8837)
	{
        drv8837_flg =ON;
        DRV_SLEEP_PIN = 1;
        DRV_IN1_PIN =1;
        DRV_IN2_PIN =0;
        delay_ms(20);
        DRV_IN1_PIN =0;
        DRV_IN2_PIN =0;
		DRV_SLEEP_PIN = 0; //进入sleep
	}
	//delay_ms(5);
	//TMR0IE	= 1;
}

/*****************************************************************************
 函 数 名  : iabs
 功能描述  : 求绝对值函数
 输入参数  : int a
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年1月3日
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
int16 iabs(int16 a)
{
    if(a<0)
    {
        return -a;
    }
    else
    {
        return a;
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
    static uint16 Timer_Stay =0;
    if(Man_Stay == MAN_HERE)
    {
        Timer_Stay ++;
        if(check_first_flg == 1)
        {
           check_first_flg =0;
           if(iabs(tab1-BAT_AD_VAL)<80)   //
           {
                DRV_8837_CTR(OPEN_8837);
           }
        }
        if(Timer_Stay >= TIME_OUT)//60s
        {
            DRV_8837_CTR(CLOSE_8837);
            Man_Stay = MAN_STAY_CUT;
        }
    }
    if(Man_Stay == MAN_LEAVE)
    {
         Timer_Stay = 0;
         Man_Stay = MAN_IDLE;
         DRV_8837_CTR(CLOSE_8837);
         ps_contr_mode(STANDY_MODE);
         //delay_ms(20);
         asm("sleep");
         _nop();
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
    ad_max = temp[0];
    ad_min = temp[1];
    for ( uint8 i=0;i<len;i++)
    {
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
 函 数 名  : led2_flash
 功能描述  : LED2调节闪烁函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年2月5日
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void led2_flash(void)
{
   static uint8 led2_time=0;
    led2_time++;
    if((led2_time%100)==40)//400ms
    {
        LED2_PIN =ON;
    }
    if((led2_time%100)==90)//900ms
    {
        LED2_PIN =OFF;
    }

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
void Auto_ADJ_PS(void) //10ms
{
	static uint16 PS_ADJ_DATA=0;
    static uint8 index =0;
    static uint16 stay_time=0;
    led2_flash();
    PS_BUF[index++]= Get_PS_DATA();
    if(index == N) // 100ms
    {
        index=0;
        PS_ADJ_DATA = PS_AD_AVG(PS_BUF, N);
        if((PS_ADJ_DATA>=PS_MIN_DAT)&&(PS_ADJ_DATA<=PS_MAX_DAT))
        {
            if(dif_val<=35)//
            {
                if((stay_time++)>=50) // 50*100 =5s
                {
                    dbg("adj ok \r\n");
                    stay_time = 0;
                    PS_DATA_H = (PS_ADJ_DATA + PS_ADJ_DATA/20);
                    PS_DATA_L = (PS_ADJ_DATA + PS_ADJ_DATA/40);
                    buf_cnt[0] =(PS_DATA_L&0xff00)>>8;
                    buf_cnt[1] = PS_DATA_L&0x00ff;
                    buf_cnt[2] =(PS_DATA_H&0xff00)>>8;
                    buf_cnt[3] = PS_DATA_H&0x00ff;
                    EepromWriteByte(eeprom_addr, buf_cnt[0]);
                    EepromWriteByte(eeprom_addr+0x01, buf_cnt[1]);
                    EepromWriteByte(eeprom_addr+0x02, buf_cnt[2]);
                    EepromWriteByte(eeprom_addr+0x03, buf_cnt[3]);
                    adj_ok_flg =1;
                    state = MODE_WORK;
                    LED2_PIN =OFF;
                }
            }
            else
            {
                stay_time = 0;
            }
        }
        else
        {
            PS_ADJ_DATA = 0;
        }
    }
}
/*****************************************************************************
 函 数 名  : man_state_update
 功能描述  : 人状态更新函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年1月30日
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void man_state_update(void)
{
    static uint8 TmpA =0,TmpB=0,TmpC = 0;
    if((PS_DATA >= PS_DATA_H)&& (Man_Stay==MAN_IDLE))
    {
        TmpA++;
        TmpB = 0;
        if(TmpA>=TIMER_Sensitive) //0.2s+延时周期
        {
            TmpA = 0;
            TmpC =0;
            check_first_flg = 1;
            Man_Stay = MAN_HERE;    //人在洗手
            dbg("man get,%d\r\n",PS_DATA);
        }
    }
    if((PS_DATA <= PS_DATA_L)&&((Man_Stay==MAN_HERE)||(Man_Stay==MAN_STAY_CUT)))
    {
        TmpB++;
        TmpA = 0;
        if(TmpB >= 8)// 人离开时间为0.4s+延时周期
        {
            TmpB = 0;
            TmpC =0;
            Man_Stay = MAN_LEAVE; //人洗手离开
            dbg("man leave,%d\r\n",PS_DATA);
        }
    }
    if((PS_DATA <= PS_DATA_L)&&(drv8837_flg==OFF))
    {
        TmpC++;
        if(TmpC>=1)//2     // 50ms
        {
            TmpC =0;
            ps_contr_mode(STANDY_MODE);
            //delay_ms(20);
            asm("sleep");
            _nop();
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
        //PS_DATA =Get_PS_DATA();
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
        man_state_update();
        drain_check();
    }
}
/*****************************************************************************
 函 数 名  : hal248_check
 功能描述  : 霍尔开关检测函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年1月30日
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void hal248_check(void)
{
    static uint8 stay_time= 0,tamB=0;
    if((HAL248_PIN == 0)&&(hal248_adj_flg ==0))
    {
        tamB=0;
        if((stay_time++)>=200)
        {
             dbg("go to adj psd\r\n");
             stay_time= 0;
             hal248_adj_flg =1;
             state = MODE_ADJ_PS;
             PS_DATA_H = 0;
             PS_DATA_L = 0;
        }
    }
    else if((HAL248_PIN == 1)&&(hal248_adj_flg ==1))
    {
        stay_time = 0;
        if((tamB++)>=200)
        {
            tamB= 0;
            hal248_adj_flg =0;
        }
    }
}
/*****************************************************************************
 函 数 名  : Task_State_Hal
 功能描述  : 状态处理函数
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
void Task_State_Hal(void)
{
    BAT_AD_VAL =Read_BAT();
    if(flg_tab0 ==0)
    {
        flg_tab0=1;
        tab1=BAT_AD_VAL;
    }
    switch ( state )
    {
        case MODE_INIT:
            {
                ps_adj_user_l= ((EepromReadByte(eeprom_addr)&0x00ff)<<8)+(EepromReadByte(eeprom_addr+0x01)&0x00ff);
                ps_adj_user_h=((EepromReadByte(eeprom_addr+0x02)&0x00ff)<<8)+(EepromReadByte(eeprom_addr+0x03)&0x00ff);
                if((ps_adj_user_l==0xFFFF)||(ps_adj_user_h==0xFFFF)) //初次使用
                {
                    dbg("use def val\r\n");
                    PS_DATA_H = (PS_DEF_DAT + PS_DEF_DAT/20);
                    PS_DATA_L = (PS_DEF_DAT + PS_DEF_DAT/80);//60

                }
                else
                {
                    dbg("use adj val\r\n");
                    PS_DATA_L = ps_adj_user_l;
                    PS_DATA_H = ps_adj_user_h;
                }
                LTR507_Write_Byte( PS_THRES_UP_0,PS_DATA_H&0X00FF);    // 设置PS_data 上限阀值
                LTR507_Write_Byte( PS_THRES_UP_1,((PS_DATA_H&0XFF00)>>8));
                state = MODE_WORK;
                LED2_PIN = OFF;
            }
            break;
        case MODE_ADJ_PS :
            {
                Auto_ADJ_PS();
            }
            break;
        case MODE_WORK:
            {
                static uint8 power_cnt=0;
    			if(iabs(tab1-BAT_AD_VAL)>=80) //电压值低于/高于基准值(6v)超过0.6v时
    			{
    				power_cnt++;
    				if(power_cnt>=3)
    				{
    					dbg("go to adj power\r\n");
    					state =MODE_ADJ_POWER;            //进入低电压状态 关闭脉冲阀
    					if(drv8837_flg == ON)
                		{
                			 DRV_8837_CTR(CLOSE_8837);
                		}
    				}
    			}
    			else
    			{
    				power_cnt=0;
    			}
#if(vision!=1)
                hal248_check();
#endif
            }
            break;
        case MODE_ADJ_POWER:
            {
                static uint16 low_power_time = 0;
                if(iabs(tab1-BAT_AD_VAL)<=50)//如果电压值又恢复到基准值(6v)范围内时，切换成正常模式，说明是电压不稳定
                {
                    low_power_time = 0;
                    dbg("low power->work\r\n");
                    state =MODE_WORK;
                }
                low_power_time++;
                if(low_power_time>=500)   // 5s 检测电压值稳定在一定范围时，就说明不是真的断电
                {
                    if(BAT_AD_VAL>=640) //电压还在4v左右
                    {
                        flg_tab0 = 0;     //重新确定阀值
                        dbg("new bat ->work\r\n");
                        state =MODE_WORK;
                    }
                    low_power_time=0;
                }
            }
            break;
        default:
            break;
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



