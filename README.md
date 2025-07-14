# GD32F427 Project 空白模板

## 项目简介
该项目是基于 GD32F427 微控制器的BMS开发项目，使用 PlatformIO 作为开发环境，并整合了 FreeRTOS 实时操作系统。

## 目录结构
<pre>
├── src                  # 源代码-目录
│   ├── driver             # 硬件驱动目录
│   │   ├── gd32f4x.c/h            # gd32f427 用到的外设接口驱动(系统时钟、uart、GPIO、I2C、CAN、ADC、RTC、flash、sleep、watchdog...)
│   │   ├── gd25q32esigr.c/h       # spi_flash gd25q32esigr驱动
│   │   ├── sh367309.c/h           # SH367309 AFE芯片驱动
│   │   ├── at8563t.c/h            # 时钟芯片at8563t驱动
│   │   ...
│   │
│   ├── module           # 功能模块-目录
│   │   ├── board.c/h              # 板级支持-功能模块（系统时钟配置 日期时间 睡眠、看门狗、系统复位、板子信息、ADC采集、io读取）
│   │   ├── logger.c/h             # 日志-功能模块
│   │   ├── ui.c/h                 # led-LED指示-功能模块(电池电量、报警、蜂鸣器) 
│   │   ├── hard_time.c/h          # 时间模块-功能模块 
│   │   ├── gateway_communicate.c/h            # 网关通信-功能模块（收据收发）
│   │   ├── battery_parallel_communicate.c/h   # 电池包并机通信-功能模块（收据收发）
│   │   ├── pc_communicate.c/h                 # 上位机通信-功能模块（收据收发）
│   │   ├── bms_dcdc_communicate.c/h           # 内部与dcdc模块通信-功能模块（收据收发）
│   │   ├── parameter.c/h          # 参数管理-功能模块
│   │   ├── ota.c/h                # ota-功能模块
│   │   ├── battery.c/h            # mcu处理电池-功能模块(AFE管理(电池保护数据读写配置，电池参数读取：电压、电流、温度)、温度采集（mcu采集）、温度控制、保护（可自恢复保护、不可自恢复保护）、充放电控制、SOC、SOH)
│   │   ...
│   │
│   ├── application      # 应用层代码目录
│   │   ├── protect_task.c/h       # 保护任务-2（未激活、电流、电压、温度、其他、保护执行）
│   │   ├── period_task.c/h        # 周期检测任务-3（按键，母线高压、报警、睡眠，喂狗、电池温度管理）
│   │   ├── collect_task.c/h       # 数据采集任务-4（AFE:电芯电压、电流、温度监测、报警信息,MCU:电流、电池电压、can电压,soc、soh、sod）
│   │   ├── communicate_task.c/h   # 通信任务-5（网关通信任务、电池包并机通信任务、上位机通信任务、DCDC通信任务）
│   │   ├── ui_task.c/h            # 显示任务-7（外接：电量指示-LED*4,报警指示-LED*2）
│   │   ...
│   │
│   │── main.c           # 主程序入口文件
│   │
│   │
│   └── util             # 工具函数目录
│       ├── pid.c/h        # PID 控制算法
│       ├── sort.c/h       # sort 排序算法
│       ├── filter.c/h     # 滤波算法
│       ├── crc8.c/h       # cec8校验算法
│       ├── crc16.c/h      # crc16校验算法
│       ...
│
├── test                 # 测试代码目录
│   ├── driver_tests                # 驱动层的测试
│   │   ├── gpio_test.c             # GPIO 驱动功能测试
│   │   ...
│   │
│   ├── module_tests     # 功能模块的测试
│   │   ...
│   │
│   ├── application_tests # 应用层的测试
│   │   ...
│   │
│   └── util_tests       # 工具函数的测试
│       ...
│
├── include              # 公共头文件目录
│   ├── common.h         # 通用宏和类型定义
│   └── project_config.h # 项目配置选项
│
├── lib                  # 库文件目录
│   ├── gd32             # GD32 库文件夹
│   ├── freertos         # FreeRTOS 库文件夹
│   └── msgpack          # Msgpack 序列化库
│
├── build                # 编译输出目录
│
├── README.md            # 项目说明文
│
└── LICENSE              # 许可证文件
<pre>
## 构建和上传
1. 使用 PlatformIO 打开该项目。
2. 在终端运行进行编译和上传。

