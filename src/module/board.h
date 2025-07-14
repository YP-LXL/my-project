#ifndef BOARD_H
#define	BOARD_H
/* 板级支持-模块项 */
/* AFE************************************************************************** */
/* 功能键************************************************************************** */
/* 箱体检测************************************************************************** */
/* 低功耗睡眠************************************************************************** */
/* 看门狗************************************************************************** */
/* 时间RTC************************************************************************** */
/* AFE************************************************************************** */
/* 数据类型枚举 */

/* 功能键************************************************************************** */
/* **驱动接口函数--开始** */
#if 1
#include "gd32f4x.h"    /* 驱动头文件 */
#include "common.h" 
/* 正式使用-修改接口 */
#define GPIO_KEY1_READ()      gd32f4x_key_read()
#else
/* 测试使用 */
#define GPIO_KEY1_READ()      (0)
#endif
/* **驱动接口函数--结束** */

/* **key 模块参数宏配置--开始** */
#define TICKS_INTERVAL      (10)	/* ms */
#define DEBOUNCE_TICKS      (1)     /* 消抖次数 0~7 */
#define SHORT_TICKS         (300 / TICKS_INTERVAL)
#define LONG_TICKS          (3000 / TICKS_INTERVAL)
#define DOUBLE_CLICK_TICKS  (500 / TICKS_INTERVAL)

/* **key 模块参数宏配置--结束** */
#define ENCLOSURE_INTERVAL                  (10)	/* 周期时间ms */
#define ENCLOSURE_DETECT_TIME               (500)	/* 有效检测时间ms */
#define ENCLOSURE_POWER_TIME                (30000)	/* 有效检测时间ms 上电完成后30s内检测到in==1,out=1*/

typedef enum {
    RED_LED_RUN,
    WHITE_LED_RUN,
    EN_SW1,
    EN_SW2,
    CAN_RS485_POWER,
    LOW_VBAT_DRIVE,
    RS485_RE,
    POWER_VPRO,
    RELAY_POWER,
    AFE_MOS,
    SHIP,
    SELF_OUT,
    HIGH_VBAT_DRIVE,
    SWITCH_TYPE_MAX 
} SWITCH_TYPE;

uint8_t board_switch_on_or_off(SWITCH_TYPE switch_name,char* action);
uint8_t board_short_circuit_protect_detection(void) ;
uint8_t board_reverse_connect_detection(void) ;

typedef void (*button_call_back)(void*);

typedef enum {
    PRESS_DOWN = 0,   /* 按键按下，每次按下都触发 */
    PRESS_UP,         /* 按键弹起，每次松开都触发 */
    PRESS_REPEAT,     /* 重复按下触发，变量repeat计数连击次数 */
    SINGLE_CLICK,     /* 单击按键事件 */
    DOUBLE_CLICK,     /* 双击按键事件 */
    LONG_PRESS_START, /* 达到长按时间阈值时触发一次 */
    LONG_PRESS_HOLD,  /* 长按期间一直触发 */
    NUMBER_OF_EVENT,  /* 有效按键事件总数 */
    NONE_PRESS        /* 无按键事件 */
} PRESS_EVENT_TYPDEF;

typedef struct Button {
    uint16_t ticks;                   // 按键的计时变量（单位可能为毫秒，用于去抖动或长按检测）
    uint8_t  repeat : 4;              // 重复按键计数（4位，最大值为 15）
    uint8_t  event : 4;               // 按键事件类型（4位，表示事件枚举值，最大值为 15）
    uint8_t  state : 3;               // 按键状态（3位，最大值为 7，用于指示当前按键状态）
    uint8_t  debounce_cnt : 3;        // 抖动计数器（3位，用于去抖动逻辑）
    uint8_t  active_level : 1;        // 按键的有效电平类型（1位，0 表示低有效，1 表示高有效）
    uint8_t  button_level : 1;        // 当前按键的实际电平（1位，0 或 1）
    uint8_t  button_id;               // 按键的唯一标识 ID（用于区分多个按键）
    uint8_t  (*hal_button_Level)(uint8_t button_id); // 函数指针，用于读取硬件按键电平状态
    button_call_back  cb[NUMBER_OF_EVENT];          // 事件回调函数数组，用于响应不同事件
    struct Button* next;              // 指向下一个按键的指针（多按键管理时形成链表）
} BUTTON_TYPDEF;

/* ------用户-私有实现部分-开始------ */
void boart_buttons_atach(PRESS_EVENT_TYPDEF event, button_call_back cb);
void boart_buttons_init(void);
void boart_buttons_process(void);
/* ------用户-私有实现部分-结束------ */

/* 箱体检测************************************************************************** */
/* ---  VCC
    |
    [] 10K
    |____---V_ADC
    |
    [] BOX_R:station=20k,suitcase=10k;
    |
   --- GND
    -
 */
/* **驱动接口函数--开始** */
#if 1
#include "gd32f4x.h"    /* 驱动头文件 */
/* 正式使用-修改接口 */
#else
/* 测试使用 */
extern uint16_t test_adc_value;


#endif
/* **驱动接口函数--结束** */

typedef enum {
        
    ENCLOSURE_OFF,    
    ENCLOSURE_ON,       
} ENCLOSURE_STATUS_TYPEDEF;

typedef struct
{
    ENCLOSURE_STATUS_TYPEDEF status;
    uint32_t                detect_time_count;
    uint32_t                poweron_time_count;

} ENCLOSURE_TYPDEF;
void board_enclosure_detect_process(void);
uint8_t board_enclosure_detect_read(uint16_t* out);
/* 低功耗睡眠************************************************************************** */
#define BOARD_MODE_SLEEP        0 /* 睡眠-功率会偏大-不好用 */
#define BOARD_MODE_DEEP_SLEEP   1 /* 深度睡眠-好用 */
#define BOARD_MODE_STANDBY      2 /* 待机-会重启-好用 */

#define BOARD_WAKUP_MODE_CAN0 0
#define BOARD_WAKUP_MODE_KEY  1

#define SLEEP_FLAG_MANUAL                   (1<<0)  /* 睡眠-手动 */
#define SLEEP_FLAG_UNDERVOLTAGE             (1<<1)  /* 睡眠-欠压 */
#define SLEEP_FLAG_TIMEOUT_DEACTIVATED      (1<<2)  /* 睡眠-超时未激活 */

void   board_sleep_init(void);
int8_t board_sleep_enter(uint8_t sleep_mode, uint8_t wakeup_mode);
void   board_sleep_exit(void);
int8_t board_sleep_set_flag(uint32_t flag_value);
uint8_t board_sleep_flag_check(void);

/* 看门狗************************************************************************** */
#define WATCHDOG_PERIOD_BOX_TASK                        (1<<0)
#define WATCHDOG_PERIOD_KEY_TASK                        (1<<1)
#define WATCHDOG_PERIOD_SLEEP_TASK                      (1<<2)
#define WATCHDOG_PERIOD_ALARM_TASK                      (1<<3)
#define WATCHDOG_PERIOD_CELL_TEMPERATURE_TASK           (1<<4)

#define WATCHDOT_UI_TASK                                (1<<5)

#define WATCHDOG_COLLECT_SOC_SOH_TASK                   (1<<6)
#define WATCHDOG_COLLECT_MCU_TEMPERATURE_TASK           (1<<7)
#define WATCHDOG_COLLECT_AFE_TASK                       (1<<8)

#define WATCHDOG_PROTECT_NONACTIVATED_TASK              (1<<9)
#define WATCHDOG_PROTECT_VOLTAGE_TASK                   (1<<10)
#define WATCHDOG_PROTECT_CURRENT_TASK                   (1<<11)
#define WATCHDOG_PROTECT_TEMPERATURE_TASK               (1<<12)
#define WATCHDOG_DEALT_TASK                             (1<<13)

#define WATCHDOG_FLAG_ALL (WATCHDOG_PERIOD_BOX_TASK  \
                           | WATCHDOG_PERIOD_KEY_TASK  \
                           | WATCHDOG_PERIOD_SLEEP_TASK  \
                           | WATCHDOG_PERIOD_ALARM_TASK  \
                           | WATCHDOG_PERIOD_CELL_TEMPERATURE_TASK \
                           | WATCHDOT_UI_TASK  \
                           | WATCHDOG_COLLECT_SOC_SOH_TASK  \
                           | WATCHDOG_COLLECT_MCU_TEMPERATURE_TASK  \
                           | WATCHDOG_COLLECT_AFE_TASK  \
                           | WATCHDOG_PROTECT_NONACTIVATED_TASK  \
                           | WATCHDOG_PROTECT_VOLTAGE_TASK  \
                           | WATCHDOG_PROTECT_CURRENT_TASK \
                           | WATCHDOG_PROTECT_TEMPERATURE_TASK \
                           | WATCHDOG_DEALT_TASK \
                           )

void board_soft_watchdog_init(uint32_t times);
void board_soft_watchdog_feed(void);
int8_t board_soft_watchdog_set_flag(uint32_t flag_value);
int8_t board_soft_watchdog_check(void);

/* 时间RTC************************************************************************** */
/* 根据项目需求定义RTC数据结构 */
typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t week;
} RTC_DATA_TYPEDEF;

uint8_t board_datatime_set(RTC_DATA_TYPEDEF* rtc_data);
int  board_datatime_initial_set(RTC_DATA_TYPEDEF* rtc_data);
void board_datatime_get(RTC_DATA_TYPEDEF* rtc_data);
void board_alarmtime_set(uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);
rtc_alarm_struct board_alarmtime_get(void);

#endif
