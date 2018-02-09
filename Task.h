#ifndef __TASK_H__
#define __TASK_H__

#include "config.h"

#define vision 2
uint16 tab1;


#define TIMER_Sensitive    4  //人体感应时间0.2s，时基100ms
#define TIME_OUT		  1200	//智能止水时间1min,时基100ms


#define PS_MAX_DAT          2000      // 5cm
#if(vision ==1)
#define PS_DEF_DAT          600      //15cm左右
#define PS_MIN_DAT          440     //35cm
#else
#define PS_DEF_DAT          800      //15cm左右
#define PS_MIN_DAT          510     //35cm

#endif

#define DRV_IN2_PIN				LATC0
#define DRV_IN1_PIN				LATC1
#define DRV_SLEEP_PIN           LATC2   // 1=NSLEEP,0-SLEEP
#define LED2_PIN                LATA4

#define     LED2_OUT()              { ANSA4 = 0 ;TRISA4 = 0; }



#define     DRV_IN1_OUT()              { ANSC1 = 0 ;TRISC1 = 0; }
#define     DRV_IN2_OUT()              { ANSC0 = 0 ;TRISC0 = 0; }
#define     DRV_SLEEP_OUT()            { ANSC2 = 0 ;TRISC2 = 0; }


#define   eeprom_addr       0x02

/* BEGIN: Added by zgj, 2018/1/22 */
#define   HAL248_PIN    RC5
#define   HAL248_IN()         {TRISC5 = 1;} //输入
/* END:   Added by zgj, 2018/1/22 */
/*标记位*/
volatile bit f2s;
volatile bit f1s ;
volatile uint8 Man_Stay = 0;                   //人状态
volatile bit check_first_flg =0;
volatile bit drv8837_flg;
volatile bit flg_tab0 = 0;
volatile bit hal248_adj_flg;

uint16  BAT_AD_VAL;
uint8 adj_ok_flg=0;

volatile uint16 PS_DATA;

volatile uint16 PS_DATA_L;
volatile uint16 PS_DATA_H;
volatile uint16 ps_adj_user_h;
volatile uint16 ps_adj_user_l;



/*定义输出IO口管脚  */

enum{
    CLOSE_8837 = 0,
    OPEN_8837,
    MAX_8837
};

enum{
    MAN_IDLE =0,
    MAN_HERE,
    MAN_LEAVE,
    MAN_STAY_CUT, //止水时间
    MAN_MAX
};

// 任务结构
typedef struct _TASK_COMPONENTS{
    uint8 Run;                 // 程序运行标记：0-不运行，1运行
    uint8 Timer;              // 计时器
    uint8 ItvTime;              // 任务运行间隔时间
    void (*TaskHook)(void);    // 要运行的任务函数
} TASK_COMPONENTS;       // 任务定义

extern void DRV_8837_CTR(uint8 mode);
extern void  DRV8837_Init( void );

extern void TaskProcess(void);
extern void TaskRemarks(void);
extern int16 iabs(int16 a);

#endif

