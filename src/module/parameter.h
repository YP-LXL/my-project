#ifndef PARAMETER_H
#define	PARAMETER_H

// #include "bl24cxx.h"
// #include "battery.h"
// #include "communicate.h"
#include "gd32f4x.h"    /* 驱动头文件 */
#include "gd25q32esig.h"
#include "common.h"
#include "battery.h"
#include "stdbool.h"
#include "project_config.h"
/* 打印信息控制 */
#define PARAMTER_DEBUG_EN    0 /* 电池模块调试使能 */
#if PARAMTER_DEBUG_EN
#define PARAMTER_LOG(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define PARAMTER_LOG(fmt, ...)
#endif



/* 数据类型枚举 */
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOL,
    TYPE_STRING,
    TYPE_UINT8,
    TYPE_UINT16,
    TYPE_UINT32,
} DATA_TYPE;

/* 哈希表节点结构 */
typedef struct HASH_NODE {
    /* 参数 ID */
    char id[256];
    /* 数据类型 */            
    DATA_TYPE  type; 
    /* 默认值 */           
    void* default_value; 
    /* EEPROM 起始地址 */    
    uint32_t address; 
    /* 数据大小 */
    uint16_t size;
    /* 链表指针 */       
    struct HASH_NODE* next;   
} HASH_NODE;

/* RAM参数结构体 */
typedef struct {
    /* 参数唯一标识 */
    char id[256]; 
    /* 数据类型 */          
    DATA_TYPE type; 
    /* 当前值 */       
    void* value; 
    /* 默认值 */          
    void* default_value;   
} RAM_DATA;

/*最大字符串长度*/
#define MAX_STRING_LEN 32
/* 最大RAM参数数量 */
#define MAX_RAM_PARAMETERS  256
/* 哈希表大小和表头数组 */
#define HASH_TABLE_SIZE 512

/* 用户实现****************************** */
typedef struct
{
    /* ============================= 电压相关参数 ============================= */
    uint16_t cell_ov1_alarm;                      /* 设置BMS电池的单体过压1告警值（两字节） (mV) 默认值：3600 */ 

    /*验证测试数据*/
    uint16_t cell_iv1_alars;                      /* 设置BMS电池的单体过压1告警值（两字节） (mV) 默认值：3600 */ 

    uint16_t cell_ov1_alarm_delay_time;           /* 设置BMS电池的单体过压1告警值触发时间（两字节） (ms) 默认值：1000 */
    uint16_t cell_ov1_release;                    /* 设置BMS电池的单体过压1告警恢复值（两字节） (mV) 默认值：3600 */
    uint16_t cell_ov1_release_delay_time;         /* 设置BMS电池的单体过压1告警恢复值触发时间（两字节） (ms) 默认值：6000 */

    uint16_t cell_ov2_alarm;                      /* 设置BMS电池的单体过压2告警值（两字节） (mV) 默认值：3600 */ 
    uint16_t cell_ov2_alarm_delay_time;           /* 设置BMS电池的单体过压2告警值触发时间（两字节） (ms) 默认值：1000 */
    uint16_t cell_ov2_release;                    /* 设置BMS电池的单体过压2告警值触发时间（两字节） (mV) 默认值：3600 */
    uint16_t cell_ov2_release_delay_time;         /* 设置BMS电池的单体过压2告警值触发时间（两字节） (ms) 默认值：6000 */

    uint16_t cell_ov3_alarm;                      /* 设置BMS电池的单体过压3告警值（两字节） (mV) 默认值：3600 */ 
    uint16_t cell_ov3_alarm_delay_time;           /* 设置BMS电池的单体过压3告警值触发时间（两字节           ms      默认值：1000 */
    uint16_t cell_ov3_release;                    /* 设置BMS电池的单体过压3告警恢复值（两字节）            mV       默认值：3600 */
    uint16_t cell_ov3_release_delay_time;         /* 设置BMS电池的单体过压3告警恢复值触发时间（两字节）     ms       默认值：6000 */
    uint16_t cell_ov3_unlock_soc;                 /* 设置BMS电池的单体过压3解锁SOC值（两字节）             %        默认值：85 */
    uint16_t cell_ov3_unlock_charge_current;      /* 设置BMS电池的单体过压3解锁充电电流值（两字节）         mA       默认值：1000 */


    uint16_t cell_uv1_alarm;                      /* 设置BMS电池的单体欠压1告警值（两字节） (mV) 默认值：2900 */
    uint16_t cell_uv1_alarm_delay_time;           /* 设置BMS电池的单体欠压1告警值触发时间（两字节） (ms) 默认值：1000 */
    uint16_t cell_uv1_release;                    /* 设置BMS电池的单体欠压1告警恢复值（两字节） (mV) 默认值：2900 */
    uint16_t cell_uv1_release_delay_time;         /* 设置BMS电池的单体欠压1告警恢复值触发时间（两字节） (ms) 默认值：1000 */

    uint16_t cell_uv2_alarm;                      /* 设置BMS电池的单体欠压2告警值（两字节） 默认值：3600 */ 
    uint16_t cell_uv2_alarm_delay_time;           /* 设置BMS电池的单体欠压2告警值触发时间（两字节） 默认值：1000 */
    uint16_t cell_uv2_release;                    /* 设置BMS电池的单体欠压2告警恢复值（两字节） 默认值：3600 */
    uint16_t cell_uv2_release_delay_time;         /* 设置BMS电池的单体欠压2告警恢复值触发时间（两字节） 默认值：6000 */

    uint16_t cell_uv3_alarm;                      /* 设置BMS电池的单体欠压3告警值（两字节）   默认值：3600 */ 
    uint16_t cell_uv3_alarm_delay_time;           /* 设置BMS电池的单体欠压3告警值触发时间（两字节）   默认值：1000 */
    uint16_t cell_uv3_release;                    /* 设置BMS电池的单体欠压3告警恢复值（两字节）   默认值：3600 */
    uint16_t cell_uv3_release_delay_time;         /* 设置BMS电池的单体欠压3告警恢复值触发时间（两字节）   默认值：6000 */
    uint16_t cell_uv3_unlock_charge_current;      /* 设置BMS电池的单体欠压3锁恢复充电电流值（两字节）   默认值：1000 */

    uint16_t total_ov1_alarm;                     /* 设置BMS电池的总压过压1告警值（两字节） 默认值：58400 */
    uint16_t total_ov1_alarm_delay_time;          /* 设置BMS电池的总压过压1告警值触发时间（两字节） 默认值：1000 */
    uint16_t total_ov1_release;                   /* 设置BMS电池的总压过压1告警恢复值（两字节） 默认值：58400 */
    uint16_t total_ov1_release_delay_time;        /* 设置BMS电池的总压过压1告警恢复值触发时间（两字节） 默认值：1000 */    

    uint16_t total_ov2_alarm;                     /* 设置BMS电池的总压过压2告警值（两字节） 默认值：58400 */
    uint16_t total_ov2_alarm_delay_time;          /* 设置BMS电池的总压过压2告警值触发时间（两字节） 默认值：1000 */
    uint16_t total_ov2_release;                   /* 设置BMS电池的总压过压2告警恢复值（两字节） 默认值：58400 */
    uint16_t total_ov2_release_delay_time;        /* 设置BMS电池的总压过压2告警恢复值触发时间（两字节） 默认值：1000 */  

    uint16_t total_ov3_alarm;                     /* 设置BMS电池的总压过压3告警值（两字节） 默认值：58400 */
    uint16_t total_ov3_alarm_delay_time;          /* 设置BMS电池的总压过压3告警值触发时间（两字节） 默认值：1000 */
    uint16_t total_ov3_release;                   /* 设置BMS电池的总压过压3告警恢复值（两字节） 默认值：58400 */
    uint16_t total_ov3_release_delay_time;        /* 设置BMS电池的总压过压3告警恢复值触发时间（两字节） 默认值：1000 */  
    uint16_t total_ov3_unlock_soc;                /* 设置BMS电池的总压过压3解锁SOC值（两字节）默认值：85 */
    uint16_t total_ov3_unlock_charge_current;     /* 设置BMS电池的总压过压3解锁充电电流值（两字节） 默认值：1000 */

    uint16_t total_uv1_alarm;                     /* 设置BMS电池的总压欠压1告警值（两字节） 默认值：43200 */
    uint16_t total_uv1_alarm_delay_time;          /* 设置BMS电池的总压欠压1告警值触发时间（两字节） 默认值：1000 */
    uint16_t total_uv1_release;                   /* 设置BMS电池的总压欠压1告警恢复值（两字节） 默认值：58400 */
    uint16_t total_uv1_release_delay_time;        /* 设置BMS电池的总压欠压1告警恢复值触发时间（两字节） 默认值：1000 */    

    uint16_t total_uv2_alarm;                     /* 设置BMS电池的总压欠压2告警值（两字节） 默认值：58400 */
    uint16_t total_uv2_alarm_delay_time;          /* 设置BMS电池的总压欠压2告警值触发时间（两字节） 默认值：1000 */
    uint16_t total_uv2_release;                   /* 设置BMS电池的总压欠压2告警恢复值（两字节） 默认值：58400 */
    uint16_t total_uv2_release_delay_time;        /* 设置BMS电池的总压欠压2告警恢复值触发时间（两字节） 默认值：1000 */  

    uint16_t total_uv3_alarm;                     /* 设置BMS电池的总压欠压3告警值（两字节） 默认值：58400 */
    uint16_t total_uv3_alarm_delay_time;          /* 设置BMS电池的总压欠压3告警值触发时间（两字节） 默认值：1000 */
    uint16_t total_uv3_release;                   /* 设置BMS电池的总压欠压3告警恢复值（两字节） 默认值：58400 */
    uint16_t total_uv3_release_delay_time;        /* 设置BMS电池的总压欠压3告警恢复值触发时间（两字节） 默认值：1000 */  
    uint16_t total_uv3_unlock_charge_current;     /* 设置BMS电池的总压欠压3锁恢复充电电流值（两字节） 默认值：1000 */


    uint16_t cell_diff_ov1_alarm;                 /* 设置BMS电池的单体压差过压1告警值（两字节） 默认值：300 */
    uint16_t cell_diff_ov1_alarm_delay_time;      /* 设置BMS电池的单体压差过压1告警值触发时间（两字节） 默认值：3000 */
    uint16_t cell_diff_ov1_release;               /* 设置BMS电池的单体压差过压1告警恢复值（两字节） 默认值：58400 */
    uint16_t cell_diff_ov1_release_delay_time;    /* 设置BMS电池的单体压差过压1告警恢复值触发时间（两字节） 默认值：1000 */
    
    uint16_t cell_diff_ov2_alarm;                 /* 设置BMS电池的单体压差过压2告警值（两字节） 默认值：300 */
    uint16_t cell_diff_ov2_alarm_delay_time;      /* 设置BMS电池的单体压差过压2告警值触发时间（两字节） 默认值：3000 */
    uint16_t cell_diff_ov2_release;               /* 设置BMS电池的单体压差过压2告警恢复值（两字节） 默认值：58400 */
    uint16_t cell_diff_ov2_release_delay_time;    /* 设置BMS电池的单体压差过压2告警恢复值触发时间（两字节） 默认值：1000 */

    /* ============================= 电流相关参数 ============================= */
    uint16_t charge_oc1_alarm;                    /* 设置BMS电池的电流充电过流1告警值（两字节）   默认值：55 */
    uint16_t charge_oc1_alarm_delay_time;         /* 设置BMS电池的电流充电过流1告警值触发时间（两字节）   默认值：1000 */
    uint16_t charge_oc2_alarm;                    /* 设置BMS电池的电流充电过流2告警值（两字节）   默认值：55 */
    uint16_t charge_oc2_alarm_delay_time;         /* 设置BMS电池的电流充电过流2告警值触发时间（两字节）   默认值：1000 */
    uint16_t charge_oc3_alarm;                    /* 设置BMS电池的电流充电过流3告警值（两字节）   默认值：55 */
    uint16_t charge_oc3_alarm_delay_time;         /* 设置BMS电池的电流充电过流3告警值触发时间（两字节）   默认值：1000 */

    uint16_t discharge_oc1_alarm;                 /* 设置BMS电池的电流放电过流1告警值（两字节）   默认值：75 */
    uint16_t discharge_oc1_alarm_delay_time;      /* 设置BMS电池的电流放电过流1告警值触发时间（两字节）   默认值：1000 */
    uint16_t discharge_oc2_alarm;                 /* 设置BMS电池的电流放电过流2告警值（两字节）   默认值：55 */
    uint16_t discharge_oc2_alarm_delay_time;      /* 设置BMS电池的电流放电过流2告警值触发时间（两字节）   默认值：1000 */
    uint16_t discharge_oc3_alarm;                 /* 设置BMS电池的电流放电过流3告警值（两字节）   默认值：55 */
    uint16_t discharge_oc3_alarm_delay_time;      /* 设置BMS电池的电流放电过流3告警值触发时间（两字节）   默认值：1000 */

    uint16_t short_circuit_alarm;                 /* 设置BMS电池的电流放电过流3告警值（两字节）   默认值：55 */
    uint16_t short_circuit_alarm_delay_time;      /* 设置BMS电池的电流放电过流3告警值触发时间（两字节）   默认值：55 */

    /* ============================= 温度相关参数 ============================= */
    // 充电温度
    uint16_t charge_cell_ot1_alarm;               /* 设置BMS电池的充电电芯过温1告警值（两字节）   默认值：50 */
    uint16_t charge_cell_ot1_alarm_delay_time;    /* 设置BMS电池的充电电芯过温1告警值触发时间（两字节）    默认值：2000 */
    uint16_t charge_cell_ot1_release;             /* 设置BMS电池的充电电芯过温1告警恢复值（两字节）   默认值：50 */
    uint16_t charge_cell_ot1_release_delay_time;  /* 设置BMS电池的充电电芯过温1告警恢复值触发时间（两字节）    默认值：2000 */
 
    uint16_t charge_cell_ot2_alarm;               /* 设置BMS电池的充电电芯过温2告警值（两字节）   默认值：50 */
    uint16_t charge_cell_ot2_alarm_delay_time;    /* 设置BMS电池的充电电芯过温2告警值触发时间（两字节）    默认值：2000 */
    uint16_t charge_cell_ot2_release;             /* 设置BMS电池的充电电芯过温2告警恢复值（两字节）   默认值：50 */
    uint16_t charge_cell_ot2_release_delay_time;  /* 设置BMS电池的充电电芯过温2告警恢复值触发时间（两字节）    默认值：2000 */
 
    uint16_t charge_cell_ot3_alarm;               /* 设置BMS电池的充电电芯过温3告警值（两字节）   默认值：50 */
    uint16_t charge_cell_ot3_alarm_delay_time;    /* 设置BMS电池的充电电芯过温3告警值触发时间（两字节）    默认值：2000 */
    uint16_t charge_cell_ot3_release;             /* 设置BMS电池的充电电芯过温3告警恢复值（两字节）   默认值：50 */
    uint16_t charge_cell_ot3_release_delay_time;  /* 设置BMS电池的充电电芯过温3告警恢复值触发时间（两字节）    默认值：2000 */
 
    uint16_t charge_cell_ut1_alarm;               /* 设置BMS电池的充电电芯欠温1告警值（两字节）   默认值：50 */
    uint16_t charge_cell_ut1_alarm_delay_time;    /* 设置BMS电池的充电电芯欠温1告警值触发时间（两字节）    默认值：2000 */
    uint16_t charge_cell_ut1_release;             /* 设置BMS电池的充电电芯欠温1告警恢复值（两字节）   默认值：50 */
    uint16_t charge_cell_ut1_release_delay_time;  /* 设置BMS电池的充电电芯欠温1告警恢复值触发时间（两字节）    默认值：2000 */
 
    uint16_t charge_cell_ut2_alarm;               /* 设置BMS电池的充电电芯欠温2告警值（两字节）   默认值：50 */
    uint16_t charge_cell_ut2_alarm_delay_time;    /* 设置BMS电池的充电电芯欠温2告警值触发时间（两字节）    默认值：2000 */
    uint16_t charge_cell_ut2_release;             /* 设置BMS电池的充电电芯欠温2告警恢复值（两字节）   默认值：50 */
    uint16_t charge_cell_ut2_release_delay_time;  /* 设置BMS电池的充电电芯欠温2告警恢复值触发时间（两字节）    默认值：2000 */
 
    uint16_t charge_cell_ut3_alarm;               /* 设置BMS电池的充电电芯欠温3告警值（两字节）   默认值：50 */
    uint16_t charge_cell_ut3_alarm_delay_time;    /* 设置BMS电池的充电电芯欠温3告警值触发时间（两字节）    默认值：2000 */
    uint16_t charge_cell_ut3_release;             /* 设置BMS电池的充电电芯欠温3告警恢复值（两字节）   默认值：50 */
    uint16_t charge_cell_ut3_release_delay_time;  /* 设置BMS电池的充电电芯欠温3告警恢复值触发时间（两字节）    默认值：2000 */
 

    // 放电温度
    uint16_t discharge_cell_ot1_alarm;               /* 设置BMS电池的放电电芯过温1告警值（两字节）   默认值：50 */
    uint16_t discharge_cell_ot1_alarm_delay_time;    /* 设置BMS电池的放电电芯过温1告警值触发时间（两字节）    默认值：2000 */
    uint16_t discharge_cell_ot1_release;             /* 设置BMS电池的放电电芯过温1告警恢复值（两字节）   默认值：50 */
    uint16_t discharge_cell_ot1_release_delay_time;  /* 设置BMS电池的放电电芯过温1告警恢复值触发时间（两字节）    默认值：2000 */
 
    uint16_t discharge_cell_ot2_alarm;               /* 设置BMS电池的放电电芯过温2告警值（两字节）   默认值：50 */
    uint16_t discharge_cell_ot2_alarm_delay_time;    /* 设置BMS电池的放电电芯过温2告警值触发时间（两字节）    默认值：2000 */
    uint16_t discharge_cell_ot2_release;             /* 设置BMS电池的放电电芯过温2告警恢复值（两字节）   默认值：50 */
    uint16_t discharge_cell_ot2_release_delay_time;  /* 设置BMS电池的放电电芯过温2告警恢复值触发时间（两字节）    默认值：2000 */
 
    uint16_t discharge_cell_ot3_alarm;               /* 设置BMS电池的放电电芯过温3告警值（两字节）   默认值：50 */
    uint16_t discharge_cell_ot3_alarm_delay_time;    /* 设置BMS电池的放电电芯过温3告警值触发时间（两字节）    默认值：2000 */
    uint16_t discharge_cell_ot3_release;             /* 设置BMS电池的放电电芯过温3告警恢复值（两字节）   默认值：50 */
    uint16_t discharge_cell_ot3_release_delay_time;  /* 设置BMS电池的放电电芯过温3告警恢复值触发时间（两字节）    默认值：2000 */
 
    uint16_t discharge_cell_ut1_alarm;               /* 设置BMS电池的放电电芯欠温1告警值（两字节）   默认值：50 */
    uint16_t discharge_cell_ut1_alarm_delay_time;    /* 设置BMS电池的放电电芯欠温1告警值触发时间（两字节）    默认值：2000 */
    uint16_t discharge_cell_ut1_release;             /* 设置BMS电池的放电电芯欠温1告警恢复值（两字节）   默认值：50 */
    uint16_t discharge_cell_ut1_release_delay_time;  /* 设置BMS电池的放电电芯欠温1告警恢复值触发时间（两字节）    默认值：2000 */
 
    uint16_t discharge_cell_ut2_alarm;               /* 设置BMS电池的放电电芯欠温2告警值（两字节）   默认值：50 */
    uint16_t discharge_cell_ut2_alarm_delay_time;    /* 设置BMS电池的放电电芯欠温2告警值触发时间（两字节）    默认值：2000 */
    uint16_t discharge_cell_ut2_release;             /* 设置BMS电池的放电电芯欠温2告警恢复值（两字节）   默认值：50 */
    uint16_t discharge_cell_ut2_release_delay_time;  /* 设置BMS电池的放电电芯欠温2告警恢复值触发时间（两字节）    默认值：2000 */
 
    uint16_t discharge_cell_ut3_alarm;               /* 设置BMS电池的放电电芯欠温3告警值（两字节）   默认值：50 */
    uint16_t discharge_cell_ut3_alarm_delay_time;    /* 设置BMS电池的放电电芯欠温3告警值触发时间（两字节）    默认值：2000 */
    uint16_t discharge_cell_ut3_release;             /* 设置BMS电池的放电电芯欠温3告警恢复值（两字节）   默认值：50 */
    uint16_t discharge_cell_ut3_release_delay_time;  /* 设置BMS电池的放电电芯欠温3告警恢复值触发时间（两字节）    默认值：2000 */
 

    /* 任意状态 */
    uint16_t cell_ot1_alarm;                      /* 设置BMS电池的电芯过温1告警值（两字节）   默认值：50 */
    uint16_t cell_ot1_alarm_delay_time;           /* 设置BMS电池的电芯过温1告警值触发时间（两字节）    默认值：2000 */
    uint16_t cell_ot1_release;                    /* 设置BMS电池的电芯过温1告警恢复值（两字节）   默认值：50 */
    uint16_t cell_ot1_release_delay_time;         /* 设置BMS电池的电芯过温1告警恢复值触发时间（两字节）    默认值：2000 */
          
    uint16_t cell_ot2_alarm;                      /* 设置BMS电池的电芯过温2告警值（两字节）   默认值：50 */
    uint16_t cell_ot2_alarm_delay_time;           /* 设置BMS电池的电芯过温2告警值触发时间（两字节）    默认值：2000 */
    uint16_t cell_ot2_release;                    /* 设置BMS电池的电芯过温2告警恢复值（两字节）   默认值：50 */
    uint16_t cell_ot2_release_delay_time;         /* 设置BMS电池的电芯过温2告警恢复值触发时间（两字节）    默认值：2000 */
 

    uint16_t cell_diff_temp1_alarm;               /* 设置BMS电池的电芯温差过大1告警值（两字节）    默认值：2000 */
    uint16_t cell_diff_temp1_alarm_delay_time;    /* 设置BMS电池的电芯温差过大1告警值触发时间（两字节）    默认值：2000 */
    uint16_t cell_diff_temp1_release;             /* 设置BMS电池的电芯温差过大1告警恢复值（两字节）   默认值：50 */
    uint16_t cell_diff_temp1_release_delay_time;  /* 设置BMS电池的电芯温差过大1告警恢复值触发时间（两字节）    默认值：2000 */

    uint16_t cell_diff_temp2_alarm;               /* 设置BMS电池的电芯温差过大2告警值（两字节）    默认值：2000 */
    uint16_t cell_diff_temp2_alarm_delay_time;    /* 设置BMS电池的电芯温差过大2告警值触发时间（两字节）    默认值：2000 */
    uint16_t cell_diff_temp2_release;             /* 设置BMS电池的电芯温差过大2告警恢复值（两字节）   默认值：50 */
    uint16_t cell_diff_temp2_release_delay_time;  /* 设置BMS电池的电芯温差过大2告警恢复值触发时间（两字节）    默认值：2000 */


    uint16_t env_ot_high1_alarm;                  /* 设置BMS电池的环境温度高温1告警值（两字节） 默认值：60 */
    uint16_t env_ot_high1_alarm_delay_time;       /* 设置BMS电池的环境温度高温1告警值触发时间（两字节）  默认值：2000 */
    uint16_t env_ot_high1_release;                /* 设置BMS电池的环境温度高温1告警恢复值（两字节） 默认值：50 */
    uint16_t env_ot_high1_release_delay_time;     /* 设置BMS电池的环境温度高温1告警恢复值触发时间（两字节）  默认值：2000 */

    uint16_t env_ot_high2_alarm;                  /* 设置BMS电池的环境温度高温2告警值（两字节） 默认值：60 */
    uint16_t env_ot_high2_alarm_delay_time;       /* 设置BMS电池的环境温度高温2告警值触发时间（两字节）  默认值：2000 */
    uint16_t env_ot_high2_release;                /* 设置BMS电池的环境温度高温2告警恢复值（两字节） 默认值：50 */
    uint16_t env_ot_high2_release_delay_time;     /* 设置BMS电池的环境温度高温2告警恢复值触发时间（两字节）  默认值：2000 */

    uint16_t env_ut_low1_alarm;                   /* 设置BMS电池的环境温度低温1告警值（两字节） 默认值：60 */
    uint16_t env_ut_low1_alarm_delay_time;        /* 设置BMS电池的环境温度低温1告警值触发时间（两字节）  默认值：2000 */
    uint16_t env_ut_low1_release;                 /* 设置BMS电池的环境温度低温1告警恢复值（两字节） 默认值：50 */
    uint16_t env_ut_low1_release_delay_time;      /* 设置BMS电池的环境温度低温1告警恢复值触发时间（两字节）  默认值：2000 */

    uint16_t env_ut_low2_alarm;                   /* 设置BMS电池的环境温度低温2告警值（两字节） 默认值：60 */
    uint16_t env_ut_low2_alarm_delay_time;        /* 设置BMS电池的环境温度低温2告警值触发时间（两字节）  默认值：2000 */
    uint16_t env_ut_low2_release;                 /* 设置BMS电池的环境温度低温2告警恢复值（两字节）  默认值：50 */
    uint16_t env_ut_low2_release_delay_time;      /* 设置BMS电池的环境温度低温2告警恢复值触发时间（两字节）   默认值：2000 */

    /* ============================== 电池状态参数 ============================== */
    uint16_t cell_balance_start_voltage;          /* 电芯均衡开启电压值 (mV) 默认值：3400 */
    uint16_t cell_balance_start_diff;             /* 电芯均衡开启压差值 (mV) 默认值：30 */
    uint16_t cell_sleep_voltage;                  /* 电芯休眠电压 (mV) 默认值：3000 */
    uint16_t cell_sleep_duration;                 /* 电芯休眠判定时间 (min) 默认值：15 */
    uint16_t cell_full_charge_voltage;            /* 电芯满充电压 (mV) 默认值：3550 */
    uint16_t full_charge_current;                 /* 满充电流 (mA) 默认值：1000 */

    /* ============================== 电池信息参数 ============================== */
    uint16_t battery_capacity_initial;            /* 电池出厂容量 (AH) 默认值：100 */
    uint16_t battery_capacity_current;            /* 电池当前容量 (AH) 默认值：100 */
    uint16_t cycle_count;                         /* 循环次数 默认值：0 */
    uint16_t total_runtime;                       /* 累计运行时间 (s) 默认值：0 */

    /* ============================== 状态估算参数 ============================== */
    uint16_t soc_initial;                         /* SOC初始值 (%) 默认值：50 */
    uint16_t soc_low_alarm;                       /* SOC低电量告警值 (%) 默认值：5 */
    uint16_t soh_initial;                         /* SOH初始值 (%) 默认值：100 */
    uint16_t soh_alarm;                           /* SOH告警值 (%) 默认值：80 */
    uint16_t sod_deep_discharge;                  /* SOD深度放阈值 (%) 默认值：80 */

    /* ============================== 设备控制参数 ============================== */
    uint16_t heater_off_temp;                     /* 加热膜关闭温度 (℃) 默认值：25 */
    uint16_t device_sn_high;                      /* 设备SN-H 默认值：0 */
    uint16_t device_sn_low;                       /* 设备SN-L 默认值：0 */
    uint16_t device_id;                           /* 设备ID 默认值：0 */ 
    uint16_t buzzer_duration;                     /* 蜂鸣时间 (ms) 默认值：1000 */
    uint16_t power_on_activation;                 /* 开机激活 默认值：1 */
    uint16_t shutdown_cmd;                        /* 关机命令 默认值：0 */
    uint16_t sleep_on;                            /* 开机激活 默认值：1 */

   
    /* ============================= 加热膜控制 ============================= */
    uint16_t heater_on_cmd;                   /* 加热膜开 (1=开启, 0=关闭) 默认值：1 */ 
    uint16_t heater_off_cmd;                  /* 加热膜关 (1=关闭, 0=无操作) 默认值：0 */
    uint16_t heater_auto_off_temp;            /* 加热膜自动关闭温度值 (℃) 默认值：25 */

    /* ============================= 指示灯控制 ============================= */
    uint16_t indicator_on_cmd;                /* 电量指示灯开 (1=开启) 默认值：1 */
    uint16_t indicator_off_cmd;               /* 电量指示灯关 (1=关闭) 默认值：0 */

    uint16_t alarm_light_on_cmd;              /* 报警灯开 (1=volatilein开启) 默认值：1 */
    uint16_t alarm_light_off_cmd;             /* 报警灯关 (1=关闭) 默认值：0 */

    /* ============================= 蜂鸣器控制 ============================= */
    uint16_t buzzer_enable_cmd;               /* 蜂鸣器开启蜂鸣功能 (1=启用) 默认值：1 */
    uint16_t buzzer_disable_cmd;              /* 蜂鸣器关闭蜂鸣功能 (1=禁用) 默认值：0 */
    uint16_t buzzer_status;                   /* 蜂鸣状态 (1=正在鸣响) 默认值：1 */
    uint16_t buzzer_stop_cmd;                 /* 停止当前蜂鸣 (1=立即停止) 默认值：0 */

    /* ======================== 电流采集标定控制 ======================== */
    uint16_t current_calib_prepare;           /* 电流采集标定-准备 (1=启动准备) 默认值：1 */ 
    uint16_t current_calib_set_value;         /* 电流采集标定-设定电流标定值 (A) 默认值：2 */ 
    uint16_t current_calib_start;             /* 电流采集标定-开始 (1=启动标定) 默认值：1 */ 
    uint16_t current_calib_verify;            /* 电流采集标定-验证测试 (1=启动验证) 默认值：1 */

    /* ======================== 硬件保护控制 ======================== */
    uint16_t short_circuit_release_on;        /* 硬件短路释放-开 (1=释放保护) 默认值：1 */
    uint16_t short_circuit_release_off;       /* 硬件短路释放-关 (1=保持保护) 默认值：0 */

    /* ======================== 充放电控制 ======================== */
    uint16_t pre_discharge_on;                /* 预放电-开 (1=启动预放电) 默认值：1 */
    uint16_t pre_discharge_off;               /* 预放电-关 (1=停止预放电) 默认值：0 */

    uint16_t discharge_mos_on;                /* 放电MOS-开 (1=导通) 默认值：1 */
    uint16_t discharge_mos_off;               /* 放电MOS-关 (1=关断) 默认值：0 */

    uint16_t charge_mos_on;                   /* 充电MOS-开 (1=导通) 默认值：1 */
    uint16_t charge_mos_off;                  /* 充电MOS-关 (1=关断) 默认值：0 */

    uint16_t fuse_blow_cmd;                   /* 保险丝熔断-开 (1=触发熔断) 默认值：1 */
    uint16_t fuse_reset_cmd;                  /* 保险丝熔断-关 (1=复位状态) 默认值：0 */

    /* ======================== 数据存储控制 ======================== */
    uint16_t afe_eeprom_save_cmd;             /* AFE设置数据到EEPROM (1=触发保存) 默认值：1 */
    uint16_t mcu_protection_save_cmd;         /* MCU保护数据存储 (1=触发保存) 默认值：1 */

    /* ======================== 系统总控 ======================== */
    uint16_t charge_discharge_ctrl;           /* 电池放电/充电总控 (1=允许) 默认值：1 */


    /* ======================== 2025.06.10->bms用 ======================== */
    uint16_t parallel_can_receive_num;          /* 并机can指令接收计数 */
    uint16_t parallel_can_send_num;             /* 并机can指令应答计数 */
    uint16_t erase_parallel_can_num_clt;        /* 并机清can指令计数值 */
    uint16_t dcdc_comm_send_num;                /* dcdc串口指令发送计数 */
    uint16_t dcdc_comm_receive_num;             /* dcdc串口指令应答计数 */
    uint16_t erase_dcdc_comm_num_clt;           /* 清dcdc串口指令计数值 */

    uint16_t mcu_init_flag;                         
                           /* MCU EEPROM 初始化完成标志位 */
} ROM_MCU_PARAMETER_TYPDEF;


extern ROM_MCU_PARAMETER_TYPDEF rom_parameter;
uint8_t parameter_rom_register(ROM_MCU_PARAMETER_TYPDEF* param_rom_mcu);
uint8_t parameter_rom_init(ROM_MCU_PARAMETER_TYPDEF* parameter);
uint8_t parameter_rom_write(ROM_MCU_PARAMETER_TYPDEF* param_rom_mcu);
uint8_t parameter_rom_read(ROM_MCU_PARAMETER_TYPDEF* param_rom_mcu);

#define PARAMETER_BATTERY_CELL_NUM                  (16u)  /* 电池串数 */
#define PARAMETER_BATTERY_TEMPERATURE_NUM           (4u)  /* 电池温度温度总数*/

#define PARAMETER_BATTERY_NONE_STATUS               (0u) /* 电池充放电状态 0-无充放 */
#define PARAMETER_BATTERY_CHARGE_STATUS             (1u) /* 电池充放电状态 1-充电 */
#define PARAMETER_BATTERY_DISCHARGE_STATUS          (2u) /* 电池充放电状态 2-放电 */

/* afe状态 */
typedef union {
    /* 43H */
    uint8_t Byte;
    struct {
        /* 位0-7，从上到下 */
        uint8_t overvoltage_protect_flag :1;
        uint8_t undervoltage_protect_flag :1;
        uint8_t discharge_overcurrent1_protect_flag :1;
        uint8_t discharge_overcurrent2_protect_flag :1;
        uint8_t charge_overcurrent_protect_flag :1;
        uint8_t short_circuit_protect_flag :1;
        uint8_t secondary_overcharge_protect_flag :1;
        uint8_t watchdog_flag :1;
    } Bits;
} U_AFE_STATUS1;
typedef union {
    /* 44H */
    uint8_t Byte;
    struct {
        /* 位0-7，从上到下 */
        uint8_t charge_under_temperature_protect_flag :1;
        uint8_t charge_over_temperature_protect_flag :1;
        uint8_t discharge_under_temperature_protect_flag :1;
        uint8_t discharge_over_temperature_protect_flag :1;
        uint8_t Bit4 :1;
        uint8_t Bit5 :1;
        uint8_t Bit6 :1;
        uint8_t Bit7 :1;
    } Bits;
} U_AFE_STATUS2;
typedef union {
    /* 45H */
    uint8_t Byte;
    struct {
        /* 位0-7，从上到下 */
        uint8_t discharge_mos_status_flag :1;
        uint8_t charge_mos_status_flag :1;
        uint8_t precharge_mos_switch_status_flag :1;
        uint8_t low_voltage_stop_charging_flag :1;
        uint8_t eeprom_write_status_flag :1;
        uint8_t Bit5 :1;
        uint8_t discharge_status_flag :1;
        uint8_t charge_status_flag :1;
    } Bits;
} U_AFE_STATUS3;

typedef union {
    /* 70H */
    uint8_t Byte;
    struct {
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
} U_AFE_HISTORY_STATUS1;
typedef union {
    /* 71H */
    uint8_t Byte;
    struct {
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
} U_AFE_HISTORY_STATUS2;

#define TASK_UNINITIALIZED_FLAG_HARDWARE            (1u<<0)  /* 任务-未初始化标志位-硬件 */
#define TASK_UNINITIALIZED_FLAG_COLLECT_AFE         (1u<<1)  /* 任务-未初始化标志位-AFE采集 */
#define TASK_UNINITIALIZED_FLAG_COLLECT_MCU         (1u<<2)  /* 任务-未初始化标志位-MCU采集 */
#define TASK_UNINITIALIZED_FLAG_COLLECT_SOC_SOH     (1u<<3)  /* 任务-未初始化标志位-SOC采集 */
#define TASK_UNINITIALIZED_FLAG_DCDC_COMMUNICATE    (1u<<4)  /* 任务-未初始化标志位-DCDC通信 */
#define TASK_UNINITIALIZED_FLAG_GATEWAY_COMMUNICATE (1u<<5)  /* 任务-未初始化标志位-网关通信 */
#define TASK_UNINITIALIZED_FLAG_PROTECT_VOLTAGE     (1u<<6)  /* 任务-未初始化标志位-电压保护 */
#define TASK_UNINITIALIZED_FLAG_PROTECT_CURRENT     (1u<<7)  /* 任务-未初始化标志位-电流保护 */
#define TASK_UNINITIALIZED_FLAG_PROTECT_TEMPERATURE (1u<<8)  /* 任务-未初始化标志位-温度保护 */
#define TASK_UNINITIALIZED_FLAG_PROTECT_DEALT       (1u<<9)  /* 任务-未初始化标志位-保护执行 */
#define TASK_UNINITIALIZED_FLAG_PERIOD_KEY          (1u<<10) /* 任务-未初始化标志位-周期检测按键 */
#define TASK_UNINITIALIZED_FLAG_PERIOD_ALARM        (1u<<11) /* 任务-未初始化标志位-周期检测报警 */

#define TASK_UNINITIALIZED_FLAG_MASK                \
    (TASK_UNINITIALIZED_FLAG_HARDWARE | \
     TASK_UNINITIALIZED_FLAG_COLLECT_AFE | \
     TASK_UNINITIALIZED_FLAG_COLLECT_MCU | \
     TASK_UNINITIALIZED_FLAG_COLLECT_SOC_SOH | \
     TASK_UNINITIALIZED_FLAG_DCDC_COMMUNICATE | \
     TASK_UNINITIALIZED_FLAG_GATEWAY_COMMUNICATE | \
     TASK_UNINITIALIZED_FLAG_PROTECT_VOLTAGE | \
     TASK_UNINITIALIZED_FLAG_PROTECT_CURRENT | \
     TASK_UNINITIALIZED_FLAG_PROTECT_TEMPERATURE | \
     TASK_UNINITIALIZED_FLAG_PROTECT_DEALT | \
     TASK_UNINITIALIZED_FLAG_PERIOD_KEY | \
     TASK_UNINITIALIZED_FLAG_PERIOD_ALARM)

/* 保护标志位-未激活 */
/* --保护标志位-未激活-掩码 */
/* --保护标志位-未激活-保护-上电后恢复 */
/* --保护标志位-未激活-保护 */

/* --报警标志位-未激活-报警-warning */
/* --报警标志位-未激活-报警-error */
/* --报警标志位-未激活-报警-fault */


/* 保护标志位-电压 */
#define PROTECT_OVERVOLTAGE_1_FLAG                (1<<0) /* 电压保护标志位-超压-1 */
#define PROTECT_OVERVOLTAGE_2_FLAG                (1<<1) /* 电压保护标志位-超压-2 */
#define PROTECT_OVERVOLTAGE_3_FLAG                (1<<2) /* 电压保护标志位-超压-3 */
#define PROTECT_UNDERVOLTAGE_1_FLAG               (1<<3) /* 电压保护标志位-欠压-1 */
#define PROTECT_UNDERVOLTAGE_2_FLAG               (1<<4) /* 电压保护标志位-欠压-2 */
#define PROTECT_UNDERVOLTAGE_3_FLAG               (1<<5) /* 电压保护标志位-欠压-3 */

#define PROTECT_TOTAL_OVERVOLTAGE_1_FLAG          (1<<6) /* 电压保护标志位-超压-1 */
#define PROTECT_TOTAL_OVERVOLTAGE_2_FLAG          (1<<7) /* 电压保护标志位-超压-2 */
#define PROTECT_TOTAL_OVERVOLTAGE_3_FLAG          (1<<8) /* 电压保护标志位-超压-3 */
#define PROTECT_TOTAL_UNDERVOLTAGE_1_FLAG         (1<<9) /* 电压保护标志位-欠压-1 */
#define PROTECT_TOTAL_UNDERVOLTAGE_2_FLAG         (1<<10) /* 电压保护标志位-欠压-2 */
#define PROTECT_TOTAL_UNDERVOLTAGE_3_FLAG         (1<<11) /* 电压保护标志位-欠压-3 */

#define PROTECT_OVERVOLDIFF_1_FLAG                (1<<12) /* 电压保护标志位-压差过大-1 */
#define PROTECT_OVERVOLDIFF_2_FLAG                (1<<13) /* 电压保护标志位-压差过大-2 */
/* --保护标志位-电压-掩码 */
#define PROTECT_VOLTAGE_MASK                      \
    (PROTECT_OVERVOLTAGE_1_FLAG | PROTECT_OVERVOLTAGE_2_FLAG | PROTECT_OVERVOLTAGE_3_FLAG | \
     PROTECT_UNDERVOLTAGE_1_FLAG | PROTECT_UNDERVOLTAGE_2_FLAG | PROTECT_UNDERVOLTAGE_3_FLAG | \
     PROTECT_TOTAL_OVERVOLTAGE_1_FLAG | PROTECT_TOTAL_OVERVOLTAGE_2_FLAG | PROTECT_TOTAL_OVERVOLTAGE_3_FLAG | \
     PROTECT_TOTAL_UNDERVOLTAGE_1_FLAG | PROTECT_TOTAL_UNDERVOLTAGE_2_FLAG | PROTECT_TOTAL_UNDERVOLTAGE_3_FLAG | \
     PROTECT_OVERVOLDIFF_1_FLAG | PROTECT_OVERVOLDIFF_2_FLAG)
     
/* --保护标志位-电压-保护-上电后恢复 */
/* --保护标志位-电压-保护 */

/* --报警标志位-电压-报警-warning */
/* --报警标志位-电压-报警-error */
/* --报警标志位-电压-报警-fault */

/* 保护标志位-电流 */
/* --保护标志位-电流-掩码 */
/* --保护标志位-电流-保护-上电后恢复 */
/* --保护标志位-电流-保护 */

/* --报警标志位-电流-报警-warning */
/* --报警标志位-电流-报警-error */
/* --报警标志位-电流-报警-fault */
/* 保护标志位-温度 */
/* --保护标志位-温度-掩码 */
/* --保护标志位-温度-保护-上电后恢复 */
/* --保护标志位-温度-保护 */

/* --报警标志位-温度-报警-warning */
/* --报警标志位-温度-报警-error */
/* --报警标志位-温度-报警-fault */
/* 保护标志位-其他 */
/* --保护标志位-其他-掩码 */
/* --保护标志位-其他-保护-上电后恢复 */
/* --保护标志位-其他-保护 */

/* --报警标志位-其他-报警-warning */
/* --报警标志位-其他-报警-error */
/* --报警标志位-其他-报警-fault */

typedef struct
{
    uint32_t uninitialized_flag;    /* 未初始化标志位 */
    uint16_t ui_run_sattus;         /* 运行状态 */
    uint16_t ui_alarm_status;       /* 报警灯状态 */
    uint16_t ui_beep_status;        /* 蜂鸣状态 */

    uint16_t protect_charge_flag;        /* 充电保护标志位 0无保护 非0有保护 */
    uint16_t protect_discharge_flag;     /* 放电保护标志位 0无保护 非0有保护 */
    uint16_t protect_fuse_flag;          /* 保险丝熔断保护标志位 0无保护 非0有保护 */
    uint16_t protect_nonactivated_flag;  /* 保护标志位-未激活（无箱体、无按键激活、按键失效、无can应答） */
    uint16_t protect_voltage_flag;       /* 保护标志位-电压（过压n、欠压n） */
    uint16_t protect_current_flag;       /* 保护标志位-电流（过流，短路） */
    uint16_t protect_temperature_high_flag;   /* 保护标志位-温度（充电过温、欠温，放电过温、欠温，环境过温、欠温，充电mos过温，放电mos过温） */
    uint16_t protect_temperature_low_flag;
    uint16_t protect_charge_status;      /* 保护标状态-充电保护 */
    uint16_t protect_discharge_status;   /* 保护标状态-放电保护 */
    uint16_t protect_fuse_status;        /* 保护标状态-保险丝熔断保护 */

    uint16_t  bms_can0_active_flag;      /* 周期检测-CAN0激活-结果 */
    uint16_t  bms_can1_active_flag;      /* 周期检测-CAN1激活-结果 */
    uint16_t  bms_usart0_active_flag;    /* 周期检测-USART0激活-结果 */
    uint16_t  bms_usart2_active_flag;    /* 周期检测-USART2激活-结果 */            
    uint16_t  bms_key_active_err_flag;   /* 周期检测-按键-异常结果 */
    uint16_t  bms_key_active_flag;       /* 周期检测-按键-激活结果 */
    uint16_t  bms_enclosure_status;      /* 周期检测-箱体检测结果 */
    uint16_t  bms_watchdog_status;       /* 周期检测-喂狗结果 */
    uint16_t  bms_alarm_flag;            /* 周期检测-报警-标志位 */
    uint16_t  bms_alarm_rgb_status;      /* 周期检测-报警-报警指示灯状态 */
    uint16_t  bms_alarm_beep_status;     /* 周期检测-报警-报警蜂鸣器状态 */
    uint16_t  bms_alarm_run_led_status;  /* 周期检测-报警-报警运行灯状态 */
    uint16_t  bms_sleep_status;          /* 周期检测-睡眠-状态 */
    uint16_t  bms_can_active_flag;

    uint16_t  afe_current_offset_current; /* AFE采集-电流采集增益临时使用 */ 
    uint8_t   battery_afe_off_line_state; /* AFE离线状态0在线 1离线 */
    U_AFE_STATUS1 u_battery_afe_alarm_status1;                  /* afe报警状态1 */
    U_AFE_STATUS2 u_battery_afe_alarm_status2;                  /* afe报警状态2 */
    U_AFE_STATUS3 u_battery_afe_alarm_status3;                  /* afe报警状态3 */
    U_AFE_HISTORY_STATUS1 u_battery_afe_history_alarm_status1;  /* afe报警历史1 */
    U_AFE_HISTORY_STATUS2 u_battery_afe_history_alarm_status2;  /* afe报警历史2 */

    /* ========================= 电流/电压监测参数 (只读) ======================== */
    int32_t  battery_current;                /* AFE采集-充放电电流 (单位：mA) */
    uint16_t voltage_diff_max;               /* AFE采集-最大压差 (单位：mV) */
    uint16_t voltage_cell_max;               /* AFE采集-最大电压 (单位：mV) */
    uint16_t voltage_cell_max_pos;           /* AFE采集-最大电压位置 (电芯索引，1-based) */
    uint16_t voltage_cell_min;               /* AFE采集-最小电压 (单位：mV) */
    uint16_t voltage_cell_min_pos;           /* AFE采集-最小电压位置 (电芯索引，1-based) */
    uint16_t voltage_cell_total;             /* AFE采集-电池总电压-单体总合 (单位：mV) */
    uint16_t voltage_cell[CELL_NUM];         /* AFE采集-单体电压-(1-16号电芯) (mV) */
    
    uint16_t voltage_can_rs485_power;        /* MCU采集-CAN，485的电源电压 */
    uint16_t voltage_battery_total;          /* MCU采集-电池总电压 (单位：mV) */
    int16_t  current_sense_value;            /* MCU采集-I_SENSE的采集 */
    /* ========================= 温度监测参数 (只读) ======================== */
    /* BMS一共5路 ：NTC1,2,3,4电芯，板载温度作为环境温度。MOS温度(由DCDC去采集，通过通信读取做保护) 2025/06/11*/
    int16_t temp_ambient;                    /* 环境温度 (单位：0.01℃) 实际值=raw*0.01℃ */
    /* 单体温度传感器数据 */
    int16_t temp_cell_1;                     /* 单体温度1 (0.01℃) */ 
    int16_t temp_cell_2;                     /* 单体温度2 (0.01℃) */
    int16_t temp_cell_3;                     /* 单体温度3 (0.01℃) */
    int16_t temp_cell_4;                     /* 单体温度4 (0.01℃) */

    int16_t temp_charge_mos;                /* 充电MOS温度 (0.01℃) */
    int16_t temp_discharge_mos;             /* 放电MOS温度 (0.01℃) */


    uint16_t temp_diff_max;                  /* 电池最大温差 (0.01℃) */

    int16_t  temp_cell_max;                  /* 单体温度最高值 (0.01℃) */
    uint16_t temp_cell_max_pos;              /* 最高温度位置 (传感器索引) */
    int16_t  temp_cell_min;                  /* 单体温度最低值 (0.01℃) */
    uint16_t temp_cell_min_pos;              /* 最低温度位置 (传感器索引) */

    /* ========================= 状态信息参数 (只读) ======================== */
    uint16_t status_charge_discharge;        /* 充放电状态 
                                            bit0: 充电使能
                                            bit1: 放电使能 */
    uint16_t soc;                            /* SOC (剩余电量百分比 0-100%) */
    uint16_t sod;                            /* SOD (放电深度百分比) */
    uint16_t soh;                            /* SOH (健康状态百分比 0-100%) */
    
    /* ========================= 保护标志参数 (只读) ======================== */
    uint16_t alarm_level;                    /* 告警等级 */
    uint16_t protection_flags_voltage;       /* 电压保护标志位 */
    uint16_t protection_flags_current;       /* 电流保护标志位 */
    uint16_t protection_high_flags_temperature;   /* 温度保护标志位  */
    uint16_t protection_low_flags_temperature;   /* 温度保护标志位  */
    uint16_t protection_flags_others;        /* 其他保护标志位  */
} RAM_PARAMETER_TYPDEF;


extern RAM_PARAMETER_TYPDEF param_ram;
extern ROM_MCU_PARAMETER_TYPDEF* mcu_rom_data;
// extern PARAMETER_DCDC dcdc_param;
unsigned int hash_string(const char* str); 
uint16_t get_data_size(DATA_TYPE type, void* default_value); 
char* get_type_name(DATA_TYPE type); 
uint8_t register_flash_parameter(const char* id, DATA_TYPE type, void* default_value); 
uint8_t read_flash_parameter(const char* id, void* output_value); 
uint8_t write_flash_parameter(const char* id, void* new_value);
uint8_t reset_flash_parameter(const char* id); 
uint8_t reset_flash_all_parameters(void);
uint8_t register_ram_parameter(const char* id, DATA_TYPE type, void* default_value); 
uint8_t write_ram_parameter(const char* id, void* new_value);
uint8_t read_ram_parameter(const char* id, void* output_value);
uint8_t free_ram_specified_parameter(const char* id); 
void free_ram_parameters(void); 
uint8_t parameter_rom_register(ROM_MCU_PARAMETER_TYPDEF* param_rom_mcu);
uint8_t parameter_rom_default_init(ROM_MCU_PARAMETER_TYPDEF* param_rom_mcu);
uint8_t parameter_rom_write(ROM_MCU_PARAMETER_TYPDEF* param_rom_mcu);
uint8_t parameter_rom_read(ROM_MCU_PARAMETER_TYPDEF* param_rom_mcu);
uint8_t parameter_rom_init(ROM_MCU_PARAMETER_TYPDEF* parameter);
uint8_t parameter_ram_init(void);
void parameter_register_and_init(void);

#endif

