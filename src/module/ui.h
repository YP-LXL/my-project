#ifndef UI_H
#define UI_H
/* 
    电量指示使用说明：
        1.根据指示灯数量，修改宏定义指示灯数量LED_POWER_SHOW_MUM；
        2.对接电量指示灯对应的接口驱动；
        3.调用led_power_show 设置电量显示百分比；
    报警灯指示使用说明：
        1.对接报警灯对应的接口驱动；
        2.调用报警灯指示设置led_alarm_on、led_alarm_off
    运行灯指示使用说明：
        1.对接运行灯对应的接口驱动；
        2.把led_run_process放到周期调用中，根据周期时间修改宏定义LED_RUN_PROCESS_PERIOD_TIME
        3.根据状态调用led_run_set_status
    蜂鸣器使用说明：
        1.对接对应的接口驱动；
        2.调用蜂鸣器功能开启beep_on，开启功能模块，（beep_off 关闭功能模块）
        3.设置蜂鸣最长时间beep_on_time_max，最长延时2^32ms，若不适用最长时间（一直蜂鸣）则设置时间为0；
        4.设置蜂鸣状态 0-关 1-短蜂鸣1次每周期，2-短蜂鸣2ci每周期，3-短蜂鸣3次每周期，4-长蜂鸣
        5.把beep_process放到 周期调用中，根据周期调用时间修改BEEP_RUN_PROCESS_PERIOD_TIME宏定义
        注意：目前默认蜂鸣器频率和占空比，需要做成活动的需要修改；蜂鸣器间隔最大为周期时间，可以根据需求修改；
 */
#include <stdio.h>
#include <stdint.h>
#include "common.h"
/* **驱动接口函数--开始** */
#if 1
#include "gd32f4x.h"    /* 驱动头文件 */

/* 正式使用-修改接口 */
// #define LED_POWER_ONE_HANDLE            gd32f4x_timer3_led_j49_2_set_duty/* led_power_one_set_duty */
// #define LED_POWER_TWO_HANDLE            gd32f4x_timer2_led_j49_3_set_duty/* led_power_two_set_duty */
// #define LED_POWER_THREE_HANDLE          gd32f4x_timer2_led_j49_4_set_duty/* led_power_three_set_duty */
// #define LED_POWER_FOUR_HANDLE           gd32f4x_timer2_led_j49_5_set_duty/* led_power_four_set_duty */

#define LED_ALARM_HANDLE                gd32f4x_timer2_led_j49_6_set_duty

#define LED_RUN_HANDLE                  gd32f4x_timer0_led_j49_7_set_duty

#define BEEP_SET_HZ_HANDLE              gd32f4x_timer0ch1_set_hz/* beep_set_hz */
#define BEEP_SET_DUTY_HANDLE            gd32f4x_timer0ch1_set_pwm_duty/* beep_set_duty */
#else
/* 测试使用 */
#define LED_POWER_ONE_HANDLE            NULL
#define LED_POWER_TWO_HANDLE            NULL
#define LED_POWER_THREE_HANDLE          NULL
#define LED_POWER_FOUR_HANDLE           NULL

#define LED_ALARM_R_HANDLE              NULL
#define LED_ALARM_G_HANDLE              NULL
#define LED_ALARM_B_HANDLE              NULL

#define LED_RUN_SET_ON_HANDLE           NULL
#define LED_RUN_SET_OFF_HANDLE          NULL

#define BEEP_SET_HZ_HANDLE              NULL
#define BEEP_SET_DUTY_HANDLE            NULL
#endif
/* **驱动接口函数--结束** */

/* **UI 模块参数宏配置--开始** */
#define LED_POWER_SHOW_MUM                  (0u)    /* led电量指示灯数量 */

#define LED_RUN_NORMAL_BLINK_HZ             (1.0f)  /* 运行灯-正常运行-闪烁频率hz */
#define LED_RUN_OTA_BLINK_HZ                (5.0f)  /* 运行灯-OTA升级-闪烁频率hz */
#define LED_RUN_PROCESS_PERIOD_TIME         (100u)  /* 运行的-周期执行时间-单位ms */

#define BEEP_RUN_PROCESS_PERIOD_TIME        (100U)  /* 蜂鸣器-周期执行时间-单位ms */
#define BEEP_DUTY_DEFAULT                   (50U)   /* 蜂鸣器-默认占空比50%，声音最大 */
#define BEEP_HZ_DEFAULT                     (2000)  /* 蜂鸣器-默认频率2k，根据需要设置2k~5k人体听力范围 */
#define BEEP_SHORT_ON_TIME_DEFAULT          (BEEP_RUN_PROCESS_PERIOD_TIME)  /* 蜂鸣器-短蜂鸣-默认开启时间周期处理时间 */
#define BEEP_SHORT_OFF_TIME_DEFAULT         (BEEP_SHORT_ON_TIME_DEFAULT)    /* 蜂鸣器-短蜂鸣-默认关闭时间=开启时间 */
#define BEEP_SHORT_PRIODIC_TIME_DEFAULT     (1000)                          /* 蜂鸣器-短蜂鸣-默认周期时间 1000ms*/

/* **UI 模块参数宏配置--结束** */
typedef void (*led_power_handle)(uint8_t duty);

typedef struct
{
    led_power_handle handle_on;
    led_power_handle handle_off;
} LED_HANDLE_TYPEDEF;

typedef struct
{
    LED_HANDLE_TYPEDEF led_handle[LED_POWER_SHOW_MUM]; /* 电量指示led数组，0~n：对应电量低~高 */
    uint8_t led_switch; /* 电量指示开关 0不指示 1指示 */
    uint8_t led_light;  /* 电量指示亮度 0%~100% */
} LED_POWER_TYPEDEF;

typedef void (*led_set_pwm)(uint8_t value);

typedef struct
{
    led_set_pwm led_alarm;        /* 报警指示R-设置占空比值， 0~100*/
    uint8_t     led_switch;   /* 报警指示开关 0不指示 1指示 */
    uint8_t     alarm_status; /* 报警状态 0误报警 1有报警 */
} LED_ALARM_TYPEDEF;

typedef enum {
    LED_RUN_NONE,           /* 状态-无操作 */
    LED_RUN_NORMAL,         /* 状态-正常-1hz闪烁 */
    LED_RUN_ALARM,          /* 状态-报警-常亮 */
    LED_RUN_OTA,            /* 状态-ota-5hz闪烁 */
    LED_RUN_ERR,            /* 状态-异常-常灭 */
    LED_RUN_STATUS_NUM_ALL, /* 状态-所有状态统计 */
} LED_RUN_STATUS_TYPDEF;

typedef void (*led_run_handle)(uint8_t value);

typedef struct
{
    led_run_handle        on;               /* 运行指示-亮 */
    led_run_handle        off;              /* 运行指示-灭 */
    LED_RUN_STATUS_TYPDEF run_status;       /* 运行状态 */
    float                 blink_hz;         /* 闪烁频率HZ */
    uint32_t              interval;         /* 计算出来的闪烁间隔 ms */
    uint32_t              timer_tick;       /* 时间控制变量 */
} LED_RUN_TYPEDEF;

typedef enum {
    BEEP_STATUS_OFF,
    BEEP_STATUS_SHORT_1,
    BEEP_STATUS_SHORT_2,
    BEEP_STATUS_SHORT_3,
    BEEP_STATUS_CONTINUOUS,
    BEEP_STATUS_ALL_NUM,
} BEEP_STATUS_TYPDEF;

typedef void (*beep_set_hz_handle)(uint32_t hz);
typedef void (*beep_set_duty_handle)(uint8_t duty);

typedef struct
{
    uint8_t beep_state;  /* 蜂鸣器-状态 0不蜂鸣 1蜂鸣 */
    uint8_t times;       /* 蜂鸣器-短蜂鸣-周期内蜂鸣次数 */
    uint8_t times_count; /* 蜂鸣器-短蜂鸣-周期内蜂鸣次数-统计 */

    uint32_t on_time;       /* 蜂鸣器-短蜂鸣-开时间ms */
    uint32_t on_time_count; /* 蜂鸣器-短蜂鸣-开时间-统计ms */

    uint32_t off_time;       /* 蜂鸣器-短蜂鸣-关时间ms */
    uint32_t off_time_count; /* 蜂鸣器-短蜂鸣-关时间-统计ms */

    uint32_t priodic_time;       /* 蜂鸣器-短蜂鸣-周期时间ms */
    uint32_t priodic_time_count; /* 蜂鸣器-短蜂鸣-周期时间-统计ms */
} BEEP_SHORT_TYPDEF;

typedef struct
{
    beep_set_hz_handle      set_hz;     /* 蜂鸣器-设置频率 2khz~5khz，音调 */
    beep_set_duty_handle    set_duty;   /* 蜂鸣器-设置pwm占空比 0~100% 占空比50%的时候声音最大*/

    uint8_t     beep_switch;                /* 蜂鸣器-开关：0关 1开 */
    uint8_t     beep_run_status;            /* 蜂鸣器-运行状态 0-关， 1-短1声1s，2-短2声1s，3短3声1s， 4-常叫，*/
    BEEP_SHORT_TYPDEF beep_short;           /* 蜂鸣器-短蜂鸣结构体变量 */
    uint32_t    beep_on_time;               /* 蜂鸣器-每次运行最长时间 */
    uint32_t    beep_on_time_count;         /* 蜂鸣器-每次运行时间累计 */
} BEEP_TYPEDEF;

/* 指示初始化 */
void led_show_init(void);

/* 电量指示 */
void led_power_show(uint8_t power);
void led_power_process(void);

/* 报警指示 */
void led_alarm_on(void);
void led_alarm_off(void);
void led_alarm_process(void);

/* RGB指示灯颜色 */
void led_orange_red_on(void);
void led_green_on(void);
void led_yellow_on(void);
void led_purple_on(void);

/* 运行状态指示 */
void led_run_set_status(LED_RUN_STATUS_TYPDEF status);
void led_run_process(void);

/* 蜂鸣器 */
void beep_off(void);
void beep_on(void);
void beep_stop(void);
void beep_on_time_max(uint32_t time_ms);
void beep_set_status(BEEP_STATUS_TYPDEF beep_status);
void beep_process(void);
#endif
