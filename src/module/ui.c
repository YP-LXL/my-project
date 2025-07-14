#include "ui.h"
/* led参数-电量指示 */
LED_POWER_TYPEDEF led_power = {
    .led_handle = {{0}},
    .led_switch = 1,
    .led_light  = 50,
};

/* led参数-报警指示 */
LED_ALARM_TYPEDEF led_alarm = {
    .led_alarm    = NULL,
    .led_switch   = 1,
    .alarm_status = 0,
};

/* led参数-运行指示 */
LED_RUN_TYPEDEF led_run = {
    .on         = NULL,
    .off        = NULL,
    .run_status = LED_RUN_NONE,
};

/* beep参数-蜂鸣器 */
BEEP_TYPEDEF beep = {
    .set_hz      = NULL,
    .set_duty    = NULL,
    .beep_switch = 1,
};

/**
 * @函数名称: led_power_show()
 * @功能描述: led电量显示
 * @输入: power 电量百分比 0~100；
 * @输出: 无
 * @返回值: 无
 */
void led_power_show(uint8_t power) 
{
    if (LED_POWER_SHOW_MUM == 0) {
        return; 
    }

    /* 计算每个 LED 的开关状态 */
    uint8_t led_to_on = (power * LED_POWER_SHOW_MUM + 99) / 100; 

    /* 通过 `power * LED_POWER_SHOW_NUM / 100` 均分电量 */
    for (int i = 0; i < LED_POWER_SHOW_MUM; i++) {
        if (i < led_to_on) {
            /* 点亮 LED */
            if (NULL != led_power.led_handle[i].handle_on ) {
                led_power.led_handle[i].handle_on(led_power.led_light);
            }
        } else {
            /* 熄灭 LED */
            if (NULL != led_power.led_handle[i].handle_off) {
                led_power.led_handle[i].handle_off(0);
            }
        }
    }
}

/**
 * @函数名称: led_power_process()
 * @功能描述: led电量显示周期处理 默认100ms
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void led_power_process(void) {}

/**
 * @函数名称: led_alarm_on()
 * @功能描述: 报警-亮红灯
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void led_alarm_on(void)
{
    if (NULL != led_alarm.led_alarm) {
        led_alarm.led_alarm(50);
    }
}

/**
 * @函数名称: led_alarm_off()
 * @功能描述: 无报警-亮绿灯
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void led_alarm_off(void)
{
    if (NULL != led_alarm.led_alarm) {
        led_alarm.led_alarm(0);
    }
}

/**
 * @函数名称: led_orange_red_on()
 * @功能描述: RGB灯橙红色
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void led_orange_red_on(void)
{

}

/**
 * @函数名称: led_green_on()
 * @功能描述: RGB灯绿色
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void led_green_on(void)
{

}

/**
 * @函数名称: led_yellow_on()
 * @功能描述: RGB灯黄色
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void led_yellow_on(void)
{

}

/**
 * @函数名称: led_purple_on()
 * @功能描述: RGB灯紫色
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void led_purple_on(void)
{

}

/**
 * @函数名称: led_alarm_process()
 * @功能描述: 报警灯周期处理 默认100ms
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void led_alarm_process(void) {}


#if 0 /* 按时用不到 */
/**
 * @函数名称: led_run_set_blink_interval()
 * @功能描述: 设置运行灯闪烁间隔时间 单位ms
 * @输入: interval_ms 闪烁建个时间
 * @输出: 无
 * @返回值: 无
 */
static void led_run_set_blink_interval(uint32_t interval_ms)
{
    if (0 == interval_ms) {
        led_run.interval = 0;
        led_run.blink_hz = 0;
    } else {
        led_run.interval = interval_ms;
        led_run.blink_hz = 1000.0 / (interval_ms << 1);
    }
    
}
#endif
/**
 * @函数名称: led_run_set_status()
 * @功能描述: 设置运行状态 设置led运状态(运行状态-1hz闪烁,报警状态-常亮,异常错误-常灭,升级状态-5hz闪烁)
 * @输入: status 运行状态
 * @输出: 无
 * @返回值: 无
 */
static void led_run_set_hz(float hz) 
{
    if (0.001 > hz) {
        led_run.blink_hz = 0;
        led_run.interval = 0;
    }
    else {
        led_run.blink_hz = hz;
        led_run.interval = 1000 / (hz * 2);
    }

}
/**
 * @函数名称: led_run_set_status()
 * @功能描述: 设置运行状态 设置led运状态(运行状态-1hz闪烁,报警状态-常亮,异常错误-常灭,升级状态-5hz闪烁)
 * @输入: status 运行状态
 * @输出: 无
 * @返回值: 无
 */
void led_run_set_status(LED_RUN_STATUS_TYPDEF status)
{
    if (status >= LED_RUN_STATUS_NUM_ALL) {
        return;
    }

    led_run.run_status = status;

    switch (status) {
        case LED_RUN_NORMAL: {
            led_run_set_hz(LED_RUN_NORMAL_BLINK_HZ);
            led_run.timer_tick = 0;
        } break;
        case LED_RUN_OTA: {
            led_run_set_hz(LED_RUN_OTA_BLINK_HZ);
            led_run.timer_tick = 0;
        } break;
        default: {
            led_run_set_hz(0);
            led_run.timer_tick = 0;
        } break;
    }
}

/**
 * @函数名称: led_run_process()
 * @功能描述: 运行灯周期处理 默认100ms
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void led_run_process(void) 
{
    static uint8_t led_state = 0;

    switch (led_run.run_status) {
        case LED_RUN_NONE: {
            /* 不做任何处理 */
        } break;
        case LED_RUN_ALARM: {
            /* 报警常亮 */
            if (NULL != led_run.on) {
                led_run.on(101);
                led_state = 1;
            }
        } break;
        case LED_RUN_ERR: {
            /* 错误常灭 */
            if (NULL != led_run.off) {
                led_run.off(0);
                led_state = 0;
            }
        } break;
        default:  {
            /* 其他按照闪烁参数闪烁 */
            if ((led_run.blink_hz >= 0.0001) && (led_run.timer_tick >= led_run.interval)) {
                if (led_state) {
                    if (NULL != led_run.on) {
                        led_run.on(101);
                    }
                } else {
                    if (NULL != led_run.off) {
                        led_run.off(0);
                    }
                }
                led_state          = !led_state;
                led_run.timer_tick = 0;
            }
        }
        break;
    }
    led_run.timer_tick += LED_RUN_PROCESS_PERIOD_TIME;
}

/**
 * @函数名称: beep_off()
 * @功能描述: 蜂蜜器 -功能关
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void beep_off(void)
{
    if (NULL != beep.set_hz) {
        beep.set_hz(0);
    }
    if (NULL != beep.set_duty) {
        beep.set_duty(0);
    }
    beep.beep_switch     = 0;
    beep.beep_run_status = BEEP_STATUS_OFF;
}

/**
 * @函数名称: beep_off()
 * @功能描述: 蜂蜜器 -功能开
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void beep_on(void)
{
    if (0 == beep.beep_switch) {
        if (NULL != beep.set_hz) {
            beep.set_hz(BEEP_HZ_DEFAULT);
        }
        /* 开启后默认不蜂鸣 */
        if (NULL != beep.set_duty) {
            beep.set_duty(0);
        }
        beep.beep_switch = 1;
    }
}

/**
 * @函数名称: beep_stop()
 * @功能描述: 蜂蜜器 -蜂鸣停止
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void beep_stop(void)
{
    beep.beep_on_time_count = beep.beep_on_time;
    beep.beep_run_status    = BEEP_STATUS_OFF;

    /* 关蜂鸣 */
    if (NULL != beep.set_duty) {
        beep.set_duty(0);
    }
}

/**
 * @函数名称: beep_on_time_max()
 * @功能描述: 蜂蜜器-蜂鸣最长时间ms
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void beep_on_time_max(uint32_t time) 
{
    beep.beep_on_time = time;
    beep.beep_on_time_count = 0;
}

/**
 * @函数名称: beep_process()
 * @功能描述: 蜂蜜器周期处理 默认100ms
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void beep_set_status(BEEP_STATUS_TYPDEF beep_status)
{
    if (beep_status >= BEEP_STATUS_ALL_NUM) {
        return;
    }

    switch (beep_status) {
        case BEEP_STATUS_OFF: {
            beep.beep_on_time_count = beep.beep_on_time;
            if (NULL != beep.set_duty) {
                beep.set_duty(0);
            }
        } break;
        case BEEP_STATUS_SHORT_1: {
            beep.beep_short.times              = 1;
            beep.beep_short.on_time            = BEEP_SHORT_ON_TIME_DEFAULT;
            beep.beep_short.off_time           = BEEP_SHORT_OFF_TIME_DEFAULT;
            beep.beep_short.priodic_time       = BEEP_SHORT_PRIODIC_TIME_DEFAULT;
            beep.beep_short.times_count        = 0;
            beep.beep_short.on_time_count      = 0;
            beep.beep_short.off_time_count     = 0;
            beep.beep_short.priodic_time_count = 0;
            beep.beep_on_time_count            = 0;
        } break;
        case BEEP_STATUS_SHORT_2: {
            beep.beep_short.times              = 2;
            beep.beep_short.on_time            = BEEP_SHORT_ON_TIME_DEFAULT;
            beep.beep_short.off_time           = BEEP_SHORT_OFF_TIME_DEFAULT;
            beep.beep_short.priodic_time       = BEEP_SHORT_PRIODIC_TIME_DEFAULT;
            beep.beep_short.times_count        = 0;
            beep.beep_short.on_time_count      = 0;
            beep.beep_short.off_time_count     = 0;
            beep.beep_short.priodic_time_count = 0;
            beep.beep_on_time_count            = 0;
        } break;
        case BEEP_STATUS_SHORT_3: {
            beep.beep_short.times              = 3;
            beep.beep_short.on_time            = BEEP_SHORT_ON_TIME_DEFAULT;
            beep.beep_short.off_time           = BEEP_SHORT_OFF_TIME_DEFAULT;
            beep.beep_short.priodic_time       = BEEP_SHORT_PRIODIC_TIME_DEFAULT;
            beep.beep_short.times_count        = 0;
            beep.beep_short.on_time_count      = 0;
            beep.beep_short.off_time_count     = 0;
            beep.beep_short.priodic_time_count = 0;
            beep.beep_on_time_count            = 0;
        } break;
        case BEEP_STATUS_CONTINUOUS: {
            if (NULL != beep.set_duty) {
                beep.set_duty(BEEP_DUTY_DEFAULT);
            }
            beep.beep_short.times              = 0;
            beep.beep_short.times_count        = 0;
            beep.beep_short.on_time_count      = 0;
            beep.beep_short.off_time_count     = 0;
            beep.beep_short.priodic_time_count = 0;
        } break;
        default: break;
    }

    beep.beep_run_status = beep_status;
}

/**
 * @函数名称: beep_short_prpcess()
 * @功能描述: 蜂蜜器-短蜂鸣处理-周期处理
 * @输入: pt_beep_short 短蜂鸣结构体 
 * @输出: cycle_time 周期时间
 * @返回值: 无
 */
static void beep_short_prpcess(BEEP_SHORT_TYPDEF* pt_beep_short, uint32_t cycle_time)
{
    /* 无效指针判断 */
    if (NULL == pt_beep_short) {
        return;
    }
    
    static uint8_t beep_state_last = 0;

    /* 短蜂鸣次数统计 */
    if (pt_beep_short->times_count < pt_beep_short->times) {
        if (pt_beep_short->beep_state) {
            pt_beep_short->on_time_count += cycle_time;
            if (pt_beep_short->on_time_count >= pt_beep_short->on_time) {
                pt_beep_short->beep_state     = 0;
                pt_beep_short->off_time_count = 0;
            }
        }
        else {
            pt_beep_short->off_time_count += cycle_time;
            if (pt_beep_short->off_time_count >= pt_beep_short->off_time) {
                pt_beep_short->beep_state    = 1;
                pt_beep_short->on_time_count = 0;
            }
        }
    } else {
        pt_beep_short->beep_state     = 0;
    }

    /* 蜂鸣 开关 */
    if (beep_state_last != pt_beep_short->beep_state) {
        beep_state_last = pt_beep_short->beep_state;
        if (pt_beep_short->beep_state) {
            if (NULL != beep.set_duty) {
                beep.set_duty(BEEP_DUTY_DEFAULT);
            }

            pt_beep_short->times_count += 1;
        }
        else {
            if (NULL != beep.set_duty) {
                beep.set_duty(0);
            }
        }
    }



    /* 短蜂鸣周期统计 */
    pt_beep_short->priodic_time_count += cycle_time;
    if (pt_beep_short->priodic_time_count >= pt_beep_short->priodic_time) {
        beep.beep_short.beep_state         = 1;
        beep.beep_short.times_count        = 0;
        beep.beep_short.on_time_count      = 0;
        beep.beep_short.off_time_count     = 0;
        beep.beep_short.priodic_time_count = 0;
    }
}

/**
 * @函数名称: beep_process()
 * @功能描述: 蜂蜜器周期处理 默认100ms
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void beep_process(void)
{
    if (0 == beep.beep_switch) {
        return;
    }

    /* 设置了最长蜂鸣时间 */
    if (beep.beep_on_time > 0) {
        if (beep.beep_on_time_count >= beep.beep_on_time) {
            beep_stop();
            beep.beep_on_time = 0;
        }
        beep.beep_on_time_count += BEEP_RUN_PROCESS_PERIOD_TIME;
    }

    switch (beep.beep_run_status) {
        case BEEP_STATUS_OFF: break;
        case BEEP_STATUS_SHORT_1:
        case BEEP_STATUS_SHORT_2:
        case BEEP_STATUS_SHORT_3: {
            beep_short_prpcess(&beep.beep_short, BEEP_RUN_PROCESS_PERIOD_TIME);
        } break;
        case BEEP_STATUS_CONTINUOUS: break;
        default: break;
    }
}

/**
 * @函数名称: beep_process()
 * @功能描述: 蜂蜜器周期处理 默认100ms
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void led_show_init(void)
{
    /* 硬件初始 */
    /* 驱动初始化 */
    // led_power.led_handle[0].handle_on  = LED_POWER_ONE_HANDLE;
    // led_power.led_handle[0].handle_off = LED_POWER_ONE_HANDLE;

    // led_power.led_handle[1].handle_on  = LED_POWER_TWO_HANDLE;
    // led_power.led_handle[1].handle_off = LED_POWER_TWO_HANDLE;

    // led_power.led_handle[2].handle_on  = LED_POWER_THREE_HANDLE;
    // led_power.led_handle[2].handle_off = LED_POWER_THREE_HANDLE;

    // led_power.led_handle[3].handle_on  = LED_POWER_FOUR_HANDLE;
    // led_power.led_handle[3].handle_off = LED_POWER_FOUR_HANDLE;

    led_alarm.led_alarm = LED_ALARM_HANDLE;

    led_run.on  = LED_RUN_HANDLE;
    led_run.off = LED_RUN_HANDLE;    

    beep.set_hz = BEEP_SET_HZ_HANDLE; 
    beep.set_duty = BEEP_SET_DUTY_HANDLE;
}
