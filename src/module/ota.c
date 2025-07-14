// #include "ota.h"


// #define OTA_STORAGE_ADDRESS  0x08080000  // 固件存储起始地址
// #define OTA_BACKUP_ADDRESS   0x080C0000  // 备份固件地址
// #define OTA_FIRMWARE_SIZE    (256 * 1024) // 固件大小 256KB

// static OTA_CALL_TYPEDEF *ota_succeed_callback = NULL;
// static OTA_CALL_TYPEDEF *ota_fail_callback = NULL;

// // OTA 环境初始化
// uint8_t ota_init(void)
// {
//     // 可以在这里进行 Flash 分区检查、存储空间准备等
//     return 0;
// }

// // OTA 固件写入
// uint8_t ota_write_firmware(uint8_t* p_buf, uint32_t len)
// {
//     if (len > OTA_FIRMWARE_SIZE) {
//         return 1; // 固件超出最大大小
//     }
    
//     // 擦除 Flash 目标区域
//     if (flash_erase(OTA_STORAGE_ADDRESS, OTA_FIRMWARE_SIZE) != 0) {
//         return 2; // Flash 擦除失败
//     }
    
//     // 写入固件
//     if (flash_write(OTA_STORAGE_ADDRESS, p_buf, len) != 0) {
//         return 3; // Flash 写入失败
//     }
    
//     return 0; // 成功
// }

// // OTA 固件验证
// uint8_t ota_verify_firmware(uint8_t* p_firmware, uint32_t len, uint16_t verify_crc16)
// {
//     uint16_t calculated_crc = crc16_calculate(p_firmware, len);
//     if (calculated_crc != verify_crc16) {
//         return 1; // 校验失败
//     }
//     return 0; // 校验成功
// }

// // OTA 应用更新
// uint8_t ota_apply_update(void)
// {
//     // 这里通常涉及 Bootloader 逻辑，例如设置启动标志并复位
//     flash_set_boot_flag(OTA_STORAGE_ADDRESS);
//     system_reset(); // 复位系统以启动新固件
//     return 0;
// }

// // OTA 回滚
// uint8_t ota_rollback(void)
// {
//     // 将备份固件恢复到应用区域
//     if (flash_erase(OTA_STORAGE_ADDRESS, OTA_FIRMWARE_SIZE) != 0) {
//         return 1; // Flash 擦除失败
//     }
//     if (flash_copy(OTA_BACKUP_ADDRESS, OTA_STORAGE_ADDRESS, OTA_FIRMWARE_SIZE) != 0) {
//         return 2; // Flash 复制失败
//     }
    
//     // 复位系统启动旧固件
//     system_reset();
//     return 0;
// }

// // 成功回调注册
// uint8_t ota_set_success_callback(OTA_CALL_TYPEDEF *ota_succeed_callback_fn)
// {
//     ota_succeed_callback = ota_succeed_callback_fn;
//     return 0;
// }

// // 失败回调注册
// uint8_t ota_set_fail_callback(OTA_CALL_TYPEDEF *ota_fail_callback_fn)
// {
//     ota_fail_callback = ota_fail_callback_fn;
//     return 0;
// }


