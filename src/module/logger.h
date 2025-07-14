#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdint.h>
#include "common.h"
/* **驱动接口函数--开始** */
#include "gd32f4x.h"    /* 驱动头文件 */
/* **驱动接口函数--结束** */

/* 日志功能开关 */
#define LOG_ENABLE        1
#define LOG_DISABLE       0

/* 定义日志级别 */
#define LOG_LEVEL_ERROR   1 
#define LOG_LEVEL_WARNING 2 
#define LOG_LEVEL_INFO    3 
#define LOG_LEVEL_DEBUG   4 


extern uint8_t logger_enable;
extern uint8_t logger_level;
const char* log_level_to_string(int level);
/* 定义日志打印宏 */
#define LOG(level, format, ...) do {if(logger_enable && level <= logger_level) {printf("[%s] " format, log_level_to_string(level), ##__VA_ARGS__);}} while (0)


void log_set_enable(uint8_t enable);
void log_set_level(uint8_t level);
#endif
