
#include "logger.h"

uint8_t logger_enable = LOG_DISABLE;
uint8_t logger_level  = LOG_LEVEL_DEBUG;

/**
 * @函数名称: log_level_to_string()
 * @功能描述: 将日志级别转为字符串
 * @输入: level 日志级别
 * @输出: 无
 * @返回值: 字符串日志级别
 */
const char* log_level_to_string(int level) { 
    switch (level) { 
        case LOG_LEVEL_ERROR:   return "ERROR"; 
        case LOG_LEVEL_WARNING: return "WARNING"; 
        case LOG_LEVEL_INFO:    return "INFO"; 
        case LOG_LEVEL_DEBUG:   return "DEBUG"; 
        default:                return "UNKNOWN"; 
    } 
}


/**
 * @函数名称: log_set_enable()
 * @功能描述: 日志使能设置
 * @输入: enable 1=使能 0=非使能；
 * @输出: 无
 * @返回值: 无
 */
void log_set_enable(uint8_t enable)
{
    logger_enable = enable;
}

/**
 * @函数名称: log_set_level()
 * @功能描述: 日志级别设置
 * @输入: level 1~4；
 * @输出: 无
 * @返回值: 无
 */
void log_set_level(uint8_t level)
{
    if ((level >= LOG_LEVEL_ERROR) && (level <= LOG_LEVEL_DEBUG)) {
        logger_level = level;
    }
}