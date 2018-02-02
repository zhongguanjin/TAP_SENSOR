#ifndef _LTR507ALS_H
#define _LTR507ALS_H


#include    "config.h"

#define     LTR507_Add          0x46

#define     ALS_CONTR           0x80
#define     PS_CONTR            0x81
#define     PS_LED              0x82
#define     PS_N_PULSES         0x83
#define     PS_MEAS_RATE        0x84
#define     ALS_MEAS_RATE       0x85
#define     PART_ID             0x86
#define     MANUFAC_ID          0x87
#define     ALS_DATA_0          0x88
#define     ALS_DATA_1          0x89
#define     ALS_PS_STATUS       0x8A
#define     PS_DATA_0           0x8B
#define     PS_DATA_1           0x8C
#define     ALS_DATA_CH1_0      0x8D
#define     ALS_DATA_CH1_1      0x8E
#define     ALS_DATA_CH1_2      0x8F
#define     ALS_DATA_CH2_0      0x90
#define     ALS_DATA_CH2_1      0x91
#define     ALS_DATA_CH2_2      0x92
#define     ALS_COEFF1_DATA_0   0x93
#define     ALS_COEFF1_DATA_1   0x94
#define     ALS_COEFF2_DATA_0   0x95
#define     ALS_COEFF2_DATA_1   0x96
#define     ALS_IRF_CUT_OFF     0x97
#define     INTERRUPT           0x98
#define     PS_THRES_UP_0       0x99
#define     PS_THRES_UP_1       0x9A
#define     PS_THRES_LOW_0      0x9B
#define     PS_THRES_LOW_1      0x9C
#define     ALS_THRES_UP_0      0x9E
#define     ALS_THRES_UP_1      0x9F
#define     ALS_THRES_LOW_0     0xA0
#define     ALS_THRES_LOW_1     0xA1
#define     INTERRUPT_PERSIST   0xA4


typedef enum
{
    STANDY_MODE= 0,
    ACTIVE_MODE,
    MAX_MODE
} ;


enum
{
    LED_FREQ_30K = 0,
    LED_FREQ_40K,
    LED_FREQ_50K,
    LED_FREQ_60K,
    LED_FREQ_70K,
    LED_FREQ_80K,
    LED_FREQ_90K,
    LED_FREQ_100K,
    LED_FREQ_MAX
};

enum
{
    LED_CUR_5MA = 0,
    LED_CUR_10MA,
    LED_CUR_20MA,
    LED_CUR_50MA,
    LED_CUR_100MA,
    LED_CUR_MAX
};

extern uint8 ltr507_init(void);

extern  uint16 Get_PS_DATA(void);
//extern  uint16 Get_ALS_DATA(void);

extern uint8 LTR507_Read_Byte(uint8 reg);
extern uint8 LTR507_Write_Byte(uint8 reg,uint8 data);
// void als_contr_mode( uint8 mode,uint8 gain);
 void ps_contr_mode( uint8 mode );

extern void ps_led_ctr(uint8 freq, uint8 cur);


#endif

