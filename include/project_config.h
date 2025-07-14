#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

#include <gd32f4xx.h>
#include "common.h"
#define SOFTWARE_V_NUM           DCDC_SOFT_VERSION  /* 软件版本号具体值在这里修改 */
#define SOFTWARE_NAME_V          "BMS_16S_V"        /* 版本号名字，字符串部分，具体值在common中修改DCDC_SOFT_VERSION*/


/* 以下参数待修改 */
#define DEBUG_PRINTF 
// #define ERROR_PRINTF
#define RAM_PRINTF
#define AFE_DEBUG_PRINTF
// #define AFE_STATUS
#define AFE_READ_INIT_CONFIG

#define AFE_STACK_SIZE 1024
#define AFE_TASK_DELAY 500

/* 电芯数量 */
#define CELL_NUM 16
/* 单体温度采集数量 */
#define CELL_TEMPERATURE_NUM 9
/* 温度偏移量 */
#define TEMPERATURE_OFFSET 40
/* 电流采样电阻值，单位0.1mΩ */
#define SH367309_CURRENT_SAMPLING_RESISTANCE (2u)

#endif