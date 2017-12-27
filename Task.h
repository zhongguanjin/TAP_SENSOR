#ifndef __TASK_H__
#define __TASK_H__

#include "config.h"




#define TIMER_Sensitive   2  //人体感应时间0.2s，时基100ms
#define TIME_OUT		  600	//智能止水时间1min,时基100ms


#define PS_MAX_DAT          2000      // 5cm
#define PS_DEF_DAT          610       //30CM
#define PS_MIN_DAT          540      //40CM 白纸板

#define DRV_IN2_PIN				LATC0
#define DRV_IN1_PIN				LATC1
#define DRV_SLEEP_PIN           LATC2   // 1=NSLEEP,0-SLEEP

#define     DRV_IN1_OUT()              { ANSC1 = 0 ;TRISC1 = 0; }
#define     DRV_IN2_OUT()              { ANSC0 = 0 ;TRISC0 = 0; }
#define     DRV_SLEEP_OUT()            { ANSC2 = 0 ;TRISC2 = 0; }

#define   eeprom_addr       0x02

volatile uint16 Timer_Stay;
/*标记位*/
volatile bit f1min;
volatile uint8 Man_Stay = 0;                   //人状态
volatile bit check_first_flg =0;
volatile bit check_60s_flg =0;
volatile uint16 PS_DATA;
volatile uint16 ALS_DATA;
volatile uint16 PS_DATA_L;
volatile uint16 PS_DATA_H;
volatile uint16 PS_DATA_MAX;
volatile uint16 ps_adj_user_h;
volatile uint16 ps_adj_user_l;
uint16  BAT_AD_VAL;

uint8 adj_ok_flg=0;




/*定义输出IO口管脚  */

enum
{
    CLOSE_8837 = 0,
    OPEN_8837,
    SLEEP_8837,
    MAX_8837
};

enum
{
    MAN_IDLE =0,
    MAN_HERE,
    MAN_LEAVE,
    MAN_MAX
};

// 任务结构
typedef struct _TASK_COMPONENTS
{
    uint8 Run;                 // 程序运行标记：0-不运行，1运行
    uint8 Timer;              // 计时器
    uint8 ItvTime;              // 任务运行间隔时间
    void (*TaskHook)(void);    // 要运行的任务函数
} TASK_COMPONENTS;       // 任务定义

extern void DRV_8837_CTR(uint8 mode);
extern void  DRV8837_Init( void );

extern void TaskProcess(void);
extern void TaskRemarks(void);


#endif

