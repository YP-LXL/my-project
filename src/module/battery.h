#ifndef BATTERY_H
#define BATTERY_H




#include "sh367309.h"
#include "parameter.h"
#include "gd25q32esig.h"
#include "board.h"
#include "project_config.h"
#include "common.h"




/* 打印信息控制 */
#define BATTERY_DEBUG_EN    0 /* 电池模块调试使能 */
#if BATTERY_DEBUG_EN
#define BATTERY_LOG(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define BATTERY_LOG(fmt, ...)
#endif
//
/* AFE************************************************************************** */
typedef struct{
    /* 00H寄存器 */
    uByteBits System_config_register1_00H;
    /* 空余寄存器 */
    uByteBits spare_register;
    /* 01H寄存器 */
    uByteBits System_config_register2_01H;
    /* 02H,03H寄存器 */
    /* 单体过压告警持续时间，最小单位0.1秒 */
    uint16_t cell_overvoltage_value_time;
    /* 单体过压告警值，最小单位1mV */
    uint16_t cell_overvoltage_value;
    /* 单体过压负载释放延时 */
    uint16_t cell_overvoltage_Load_release_delay;
    /* 04H,05H寄存器 */
    /* 单体欠压告警持续时间，最小单位0.1秒 */
    uint16_t cell_undervoltage_value_time;
    /* 单体过压恢复告警值，最小单位1mV */
    uint16_t cell_overvoltage_recovery_value;
    /* 06H寄存器 */
    /* 单体欠压告警值 最小单位1mV */
    uint16_t cell_undervoltage_value;
    /* 07H寄存器 */
    /* 单体欠压恢复值 最小单位1mV */
    uint16_t cell_undervoltage_recovery_value;
    /* 08H寄存器 */
    /* 平衡开启电压设置值 */
    uint16_t balance_voltage_value;
    /* 09H寄存器 */
    /* 预充电电压设置值(禁用预充) */
    uint16_t precharge_voltage_value;
    /* 0AH寄存器 */
    /* 低电压禁止充电电压值 */
    uint16_t low_voltage_stop_charging_voltage_value;
    /* 0BH寄存器 */
    /* 二次过充电保护电压值 */
    uint16_t secondary_overcharge_protection_voltage_value;
    /* 0CH寄存器 */
    /* 放电过流1保护电压值 */
    uint16_t discharge_overcurrent1_protection_voltage_value;
    /* 放电过流1保护延时设置 */
    uint16_t discharge_overcurrent1_protection_delay_value;
    /* 0DH寄存器 */
    /* 放电过流2保护电压值 */
    uint16_t discharge_overcurrent2_protection_voltage_value;
    /* 放电过流2保护延时设置 */
    uint16_t discharge_overcurrent2_protection_delay_value;
    /* 0EH寄存器 */
    /* 短路保护电压值 */
    uint16_t short_circuit_protection_voltage_value;
    /* 短路保护延时设置 */
    uint16_t short_circuit_protection_delay_value;
    /* 0FH寄存器 */
    /* 充电过流保护电压值 */
    uint16_t charge_overcurrent_protection_voltage_value;
    /* 充电过流保护延时设置 */
    uint16_t charge_overcurrent_protection_delay_value;
    /* 10H寄存器 */
    /* 充放电状态检测电压设置 */
    uint16_t charge_discharge_state_detection_voltage;
    /* 充放电MOS开启延时设置 */
    uint16_t charging_discharg_mos_opening_delay;
    /* 充放电过流自恢复延时设置 */
    uint16_t charge_discharge_overcurrent_self_recovery_delay;
    /* 二次过充电保护延时设置 */
    uint16_t secondary_overcharge_protection_delay;
    /* 11H寄存器 */
    /* 充电高温保护值 */
    uint16_t charge_over_temperature_value;
    /* 12H寄存器 */
    /* 充电高温保护释放值 */
    uint16_t charge_over_temperature_recovery_value;
    /* 13H寄存器 */
    /* 充电低温保护值 */
    uint16_t charge_under_temperature_value;
    /* 14H寄存器 */
    /* 充电低温保护释放值 */
    uint16_t charge_under_temperature_recovery_value;
    /* 15H寄存器 */
    /* 放电高温保护值 */
    uint16_t discharge_over_temperature_value;
    /* 16H寄存器 */
    /* 放电高温保护释放值 */
    uint16_t discharge_over_temperature_recovery_value;
    /* 17H寄存器 */
    /* 放电低温保护值 */
    uint16_t discharge_under_temperature_value;
    /* 18H寄存器 */
    /* 放电低温保护释放值 */
    uint16_t discharge_under_temperature_recovery_value;

    /* 40H RAM寄存器 */
    uByteBits System_config_ram_register_40H;
    /* 41H RAM寄存器 */
    uByteBits System_config_ram_register_41H;
    /* 42H RAM寄存器 */
    uByteBits System_config_ram_register_42H;
    /* CADC偏移值 */
    uint16_t CADC_offset_value;
    /* CADC 电流校准增益 */
    float CADC_current_calibration_gain;
    /* AFE EEPROM 初始化完成标志位 */
    uint8_t afe_init_flag;
}AFE_INIT;

/* 温度查表取阻值 */
/* 定义温度和Rnor值数组 */
typedef struct {
    int   temp; /* 温度值 单位:℃ */
    float Rnor; /* 电阻值 单位:kR */
}TEMP_RNOR_DATA;

/* 告警结构体 */
typedef union
{
    /* 43H */
    uint8_t Byte;
    struct
    {
        /* 位0-7，从上到下 */
        uint8_t overvoltage_protect_flag :1;
        uint8_t undervoltage_protect_flag :1;
        uint8_t discharge_overcurrent1_protect_flag :1;
        uint8_t discharge_overcurrent2_protect_flag :1;
        uint8_t charge_overcurrent_protect_flag :1;
        uint8_t short_circuit_protect_flag :1;
        uint8_t secondary_overcharge_protect_flag :1;
        uint8_t watchdog_flag :1;
    }Bits;
}ALARM_STATUS1;

typedef union
{
    /* 44H */
    uint8_t Byte;
    struct
    {
        /* 位0-7，从上到下 */
        uint8_t charge_under_temperature_protect_flag :1;
        uint8_t charge_over_temperature_protect_flag :1;
        uint8_t discharge_under_temperature_protect_flag :1;
        uint8_t discharge_over_temperature_protect_flag :1;
        uint8_t Bit4 :1;
        uint8_t Bit5 :1;
        uint8_t Bit6 :1;
        uint8_t Bit7 :1;
    }Bits;
}ALARM_STATUS2;

typedef union
{
    /* 45H */
    uint8_t Byte;
    struct
    {
        /* 位0-7，从上到下 */
        uint8_t discharge_mos_status_flag :1;
        uint8_t charge_mos_status_flag :1;
        uint8_t precharge_mos_switch_status_flag :1;
        uint8_t low_voltage_stop_charging_flag :1;
        uint8_t eeprom_write_status_flag :1;
        uint8_t Bit5 :1;
        uint8_t discharge_status_flag :1;
        uint8_t charge_status_flag :1;
    }Bits;
}ALARM_STATUS3;

typedef union
{
    /* 70H */
    uint8_t Byte;
    struct
    {
        /* 位0-7，从上到下 */
        uint8_t overvoltage_protect_flag :1;
        uint8_t undervoltage_protect_flag :1;
        uint8_t discharge_overcurrent_protect_flag :1;
        uint8_t LODO3_overcurrent_protect_flag :1;
        uint8_t charge_overcurrent_protect_flag :1;
        uint8_t short_circuit_protect_flag :1;
        uint8_t secondary_overcharge_protect_flag :1;
        uint8_t watchdog_flag :1;
    }Bits;
}ALARM_HISTORY_STATUS1;

typedef union
{
    /* 71H */
    uint8_t Byte;
    struct
    {
        /* 位0-7，从上到下 */
        uint8_t charge_under_temperature_protect_flag :1;
        uint8_t charge_over_temperature_protect_flag :1;
        uint8_t discharge_under_temperature_protect_flag :1;
        uint8_t discharge_over_temperature_protect_flag :1;
        uint8_t VADC_interrupt_flag :1;
        uint8_t CADC_interrupt_flag :1;
        uint8_t weakup_interrupt_flag :1;
        uint8_t reset_flag :1;
    }Bits;
}ALARM_HISTORY_STATUS2;

typedef struct {
    ALARM_STATUS1 alarm_state1;
    ALARM_STATUS2 alarm_state2;
    ALARM_STATUS3 alarm_state3;
    ALARM_HISTORY_STATUS1 history_alarm1;
    ALARM_HISTORY_STATUS2 history_alarm2;
}ALARM_DATA;

/* 温度计算公式宏定义 */
/* 内部参考电阻计算公式 */
#define SH367309_R_REF(x)   (6.8f+0.05f*x)
/* 使用B值法计算温度，这个宏用于防止除数为0的情况发生 */
#define NTC_B_VALUE         3435
#define B_VALUE_TEMP_METHOD_L(x) ((1.0f / NTC_B_VALUE) * log((x) / 10) + (1.0f / (25+273.15f)))
#define B_VALUE_TEMP_METHOD(x)   ((B_VALUE_TEMP_METHOD_L(x)!= 0)?((1.0f / B_VALUE_TEMP_METHOD_L(x)) - 273.15f):(6553.5))
/* 采样电阻阻值--AFE用 */
#define CURRENT_RSENSE    (0.00025f)
/* AFE芯片的充放电过温保护设置寄存器的值 */
#define CHA_DIS_HIGH_TEMP_VAL(Rt1,x) ((Rt1/(SH367309_R_REF(x)+Rt1))*512)         
#define CHA_DIS_LOW_TEMP_VAL(Rt1,x) (((Rt1/(SH367309_R_REF(x)+Rt1))-0.5f)*512)

extern AFE_INIT* parameter;
extern AFE_INIT* afe_init_data;
extern ALARM_DATA alarm_state;

#define BATTERY_AFE_GET_GAIN_PARAM()                    (0)//(param_afe_init_data.CADC_current_calibration_gain) /* 获取 增益值 */
#define BATTERY_AFE_GET_OFFSET_REGISTER_VALUE_PARAM()   (0)//(param_afe_init_data.CADC_offset_value) /* 获取偏移值 */


uint8_t battery_afe_init(AFE_INIT* init_parameter);
void battery_afe_alarm_callback_function(void);
uint8_t battery_afe_get_cell_voltage_value(uint8_t cell_num, uint16_t* cell_voltage_buff);
uint8_t battery_afe_get_other_voltage_value(const uint16_t *cell_voltage_value, uint8_t cell_num, 
                                uint16_t *max_value, uint8_t *max_index, 
                                uint16_t *min_value, uint8_t *min_index, 
                                uint32_t *total_voltage);
uint8_t battery_afe_get_current_value(int32_t* current_value);
uint8_t battery_afe_get_cell_temperature_value(uint8_t temperature_num, int16_t* cell_tempertaure_buff);
uint8_t battery_afe_get_alarm_state(ALARM_DATA* state_value);
uint8_t battery_afe_get_history_alarm_state(ALARM_DATA* state_value);
uint8_t battery_afe_clear_history_alarm_state(void);
uint8_t battery_afe_get_calibration_offset_current(uint16_t* offset_current);
float battery_afe_get_calibration_current_gain(uint16_t actual_cadc_value, uint16_t offset_cadc_value, float calibration_actual_current);
void battery_afe_read_init_register_config(AFE_INIT *afe_init);
void battery_afe_register_flash_parameter(AFE_INIT* init_parameter);
void battery_afe_write_default_flash_parameter(AFE_INIT* init_parameter);

/* SOC************************************************************************** */
typedef struct
{
    float   use_age;              /* 使用时间-单位秒 */
    float   nominal_capacity;     /* 电池标称容量-当前使用 */
    float   series_battery;       /* 串联电池组串数 */
    float   current_capacity;     /* 电池当前容量-修正后 */
    float   current_temperature;  /* 当前温度 */
    float   soc;                  /* 电池电量 */
    float   min_voltage;          /* 最小电压 */
    float   max_voltage;          /* 最大电压 */
    uint8_t battery_diff_flag;    /* 电池压差过大标志位 */
    uint8_t ocv_calibration_flag; /* ocv标定标志位 */
} SOC_PARAM_TYPEDEF;

uint8_t battery_soc_init(SOC_PARAM_TYPEDEF* p_soc_param);
uint8_t battery_soc_calculate(float current, float pack_voltage, float* cell_voltages);
uint8_t battery_soc_get_value(float* p_soc_out);

uint8_t battery_soc_update_nominal_capacity(float full_capacity);
uint8_t battery_soc_update_temperature(float temperature);
uint8_t battery_soc_get_age(float* age);

uint8_t soc_reset(void);

/* SOH************************************************************************** */
#include <math.h>

#define Q_INITIAL                   (100.0)             /* 初始容量 */
#define SAMPLING_TIME               (0.1f)              /* 采样周期，单位：秒 */
#define INTEGRAL_CURRENT_AH_CONVER(current)      (current*SAMPLING_TIME/3600.0)    /* 积分电流AH转换 */

#define SOH_LIMIT_MIN_DEFAULT        (60.0f)            /* SOH最小默认60% */
#define DOD_LIMIT_MAX_DEFAULT        (80.0f)            /* 放电深度最大默认80% */
#define SOH_CHARGE_CURRENT_THRESHOLD (0.5f)             /* 充电判断电流阈值 */

typedef struct
{
    float factory_capacity;   /* 出厂标称容量 单位AH*/
    float surplus_capacity;   /* 当前可用容量-用soh计算得到 单位AH*/
    float discharge_capacity; /* 放电容量 单位AH*/
    float cycle_count;        /* 循环使用次数 */
    float soh;                /* 电池健康状态 % */
    float soh_limit;          /* 电池健康极限值 */
    float dod;                /* 放电深度 % */
    float dod_limit_max;      /* 深度放电阈值 % */
    float dod_count;          /* 深度放电统计 */

    uint8_t soh_err_flag; /* soh 异常标志位 */
    uint8_t dod_err_flag; /* dod 超阈值标志位 */
} SOH_PARAM_TYPEDEF;

uint8_t battery_soh_init(SOH_PARAM_TYPEDEF* p_soh_param);
uint8_t battery_soh_cycle_calculate(float current, float soc);
uint8_t battery_soh_get_value(float* soh_value);
uint8_t battery_soh_reset(void);


/***************温度采集*****************/
typedef struct
{
    uint8_t NTC_RESISTANCE_ERROR;
} NTC_ERROR_MSG_TYPAEDEF;

typedef struct
{
    uint8_t HEAT_FILM_LOSS_TEMPERATURE;
} HEAT_FILM_ERROR_TYPAEDEF;
float* battery_mcu_cells_tempetature_get(void);
float battery_charge_temperature_get(void);
float battery_discharge_temperature_get(void);
float battery_ambient_temperature_get(void);
void battery_temperature_init(void);
void battery_temperature_periodic_process(void);


/*预放电MOS***************************************************/
/* 预放电ADC最大值 */
#define PREDISCHARGE_ADC_MAX_VALUE          4096.0
/* 基准电压3.3V */
#define PREDISCHARGE_V_REF                  3.3
/* 放大倍率11 */
#define PREDISCHARGE_MULTIPLE               11
/* 放大三级管压降 0.6v */
// #define PREDISCHARGE_TRIODE_VOLTAGE      600
#define P_PLUS_VOLTAGE_THRESHOLD            10     
#define P_PLUS_VOLTAGE_TOLERANCE            150    
#define VOLTAGE_DIFF_THRESHOLD              2        
#define PRECHARGE_MOS_ON_TIME               50        
#define PRECHARGE_MOS_OFF_TIME              200      
#define DISCHARGE_MOS_ON_TIME               2000      
#define MAX_PRECHARGE_ATTEMPTS              10       
#define MAX_PRECHARGE_ROUNDS                5          
#define B_PLUS_VOLTAGE                      25000
typedef struct 
{
  uint32_t* get_b_plus_voltage;
  uint8_t predischarge_time_out_flag;
  uint8_t predischarge_short_circuit_flag;
}PREDISCHARE_FUNC_TYPEDEFINE;
extern PREDISCHARE_FUNC_TYPEDEFINE discharge_msg;
uint8_t battery_predischarge_mos_func(void);
uint32_t get_p_plus_voltage(void);
uint32_t get_b_plus_voltage(void);


#define pwm_max                     100
#define pwm_hz                      2000
#define ADC_MAX_VALUE               4096.0
#define V_REF                       (3.3f)
#define NTC_REF_RESISTANCE          10
// #define BETA_COEFFICIENT            3950
#define BETA_COEFFICIENT            3435
#define ROOM_TEMP_KELVIN            (25 + 273.15)
#define TEMP_CORRECTION             40

#define	REALL_DATA(SAMP)	(float)( ( ( (float)SAMP )  * 3.3 ) / 4096 ) //GLF
#define	STLM20_V25      1.8663
#define	STLM20_AVG_SLOPE        0.01169


float calculate_battery_current(uint32_t adc_value); 
float calculate_can_battery_vbat_voltage(uint32_t adc_value);
float calculate_battery_total_vbat_voltage(uint32_t adc_value);
float calculate_mcu_ntc4(uint32_t adc_value);
float calculate_mcu_board_temperture(uint32_t adc_value);
void battery_temperature_init(void);
float battery_mcu_current_get(void);
float can_vbat_mcu_voltage_get(void);
float battery_charge_temperature_get(void);
void mcu_adc_value_refresh(void);
extern int16_t usr_temperatures[5];

#endif 
