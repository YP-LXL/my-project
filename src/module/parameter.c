#include "parameter.h"

/* 全局变量：偏移地址 */
uint16_t global_offset = 0;
HASH_NODE* hash_table[HASH_TABLE_SIZE] = {0};
/* RAM全局参数 */
/* 参数表数组 */
RAM_DATA param_table[MAX_RAM_PARAMETERS];
/* 当前参数计数 */
uint8_t param_count = 0;

uint8_t erase_flag = 0;

/**
 * @函数名称: hash_string()
 `* @功能描述: 计算哈希值,将字符串转换成一个整数值
 * @输入: str ：字符串
 * @输出: 无
 * @返回值: 无
 */
unsigned int hash_string(const char* str) 
{
    unsigned int hash = 0;
    while (*str) {
        hash = (hash * 31) + *str++;
    }
    return hash % HASH_TABLE_SIZE;
}

/**
 * @函数名称: get_data_size()
 * @功能描述: 计算数据大小
 * @输入: type ：数据类型
 *       default_value ：默认值
 * @输出: 无
 * @返回值: 数据大小
 */
uint16_t get_data_size(DATA_TYPE type, void* default_value) 
{
    uint16_t size = 0;
    switch (type) {
        case TYPE_INT: size = sizeof(int); break;
        case TYPE_FLOAT: size = sizeof(float); break;
        case TYPE_BOOL: size = sizeof(uint8_t); break;
        case TYPE_STRING: size = MAX_STRING_LEN; break;
        case TYPE_UINT8: size = sizeof(uint8_t); break;
        case TYPE_UINT16: size = sizeof(uint16_t); break;
        case TYPE_UINT32: size = sizeof(uint32_t); break;
        default: size = 0; break;
    }
    return size;
}

/**
 * @函数名称: get_type_name()
 * @功能描述: 获取类型名称
 * @输入: type ：数据类型
 * @输出: 无
 * @返回值: 类型字符串
 */
char* get_type_name(DATA_TYPE type) 
{
    switch (type) {
        case TYPE_INT:    return "int";
        case TYPE_FLOAT:  return "float";
        case TYPE_BOOL:   return "bool";
        case TYPE_STRING: return "string";
        case TYPE_UINT8:  return "uint8";
        case TYPE_UINT16: return "uint16";
        case TYPE_UINT32: return "uint32";
        default:          return "unsupported";
    }
}


/**
 * @函数名称: register_flash_parameter()
 * @功能描述: 参数注册函数
 * @输入: id ：查找的id
 *		type ：参数类型
 *		default_value：参数默认值
 * @输出: 无
 * @返回值:  0：失败 1 ：成功
 */
uint8_t register_flash_parameter(const char* id, DATA_TYPE type, void* default_value) 
{
    /* 检查是否已存在 */
    unsigned int index = hash_string(id);
    HASH_NODE* existing = hash_table[index];
    while (existing) {
        if (strcmp(existing->id, id) == 0) {
            #ifdef ERROR_PRINTF
            PARAMTER_LOG("Error: Parameter ID '%s' is already registered.\r\n", id);
            #endif

            /* 返回 0 表示参数已存在 */
            return 0;  
        }
        existing = existing->next;
    }

    /* 计算数据大小 */
    uint16_t size = get_data_size(type, default_value);

    if (size == 0) {
        #ifdef ERROR_PRINTF
        PARAMTER_LOG("Error: Unsupported type for ID '%s'.\r\n", id);
        #endif
        /* 返回 0 表示类型不支持 */
        return 0;  
    }

    /* 构造 flash 数据格式 */
    /* 标志位 */
    uint8_t flag = (default_value != NULL) ? 1 : 0;
    /* 字符串长度（仅 string 类型有效） */
    uint8_t length = 0; 
    /* 为 flag 和可能的长度预留空间 */                            
    uint8_t* flash_data = (uint8_t*)malloc(size + 2); 
    flash_data[0] = flag;

    if (type == TYPE_STRING) {
        length = (uint8_t)strlen((char*)default_value) + 1; 
        flash_data[1] = length;
        /* 写入字符串 */
        memcpy(&flash_data[2], default_value, size); 
    } else {
        /* 非字符串，直接写入数据 */
        memcpy(&flash_data[1], default_value, size); 
    }

    /* 写入 flash */
    // eeprom_bl24cxx_write(global_offset, flash_data, size + (type == TYPE_STRING ? 2 : 1));

    /* 插入哈希表 */
    HASH_NODE* new_node = (HASH_NODE*)malloc(sizeof(HASH_NODE));
    /* 复制 id 字符串 */
    strcpy(new_node->id, id); 
    new_node->type = type;
    new_node->default_value = malloc(size);
    memcpy(new_node->default_value, default_value, size);
    new_node->address = global_offset;
    new_node->size = size;
    new_node->next = hash_table[index];
    hash_table[index] = new_node;
    #ifdef DEBUG_PRINTF
    PARAMTER_LOG("Registered ID: '%s', Type: %s, Address: 0x%04X, Size: %d\r\n",
           id, get_type_name(type), global_offset, size + (type == TYPE_STRING ? 2 : 1));
    #endif

    /* 更新全局偏移地址 */
    global_offset += size + (type == TYPE_STRING ? 2 : 1);

    /* 释放临时缓冲区 */
    free(flash_data);
    /* 返回 1 表示成功 */
    return 1;  
}

/**
 * @函数名称: read_flash_parameter()
 * @功能描述: flash读函数
 * @输入: id ：查找的id
 *		output_value ：读取到的输出值
 * @输出: 无
 * @返回值: 0 ：成功 1：失败
 */
uint8_t read_flash_parameter(const char* id, void* output_value) 
{
    /* 计算哈希值 */
    unsigned int index = hash_string(id); 
    HASH_NODE* node = hash_table[index];

    /* 查找哈希表中是否存在该 ID */
    while (node) {
        if (strcmp(node->id, id) == 0) {
            /* 找到对应的参数，读取 EEPROM 中的数据 */
            uint8_t flash_data[node->size + (node->type == TYPE_STRING ? 2 : 1)];
            if (spi_flash_gd25q32esig_read(node->address, flash_data, sizeof(flash_data)) != 0) {
                /* 读取失败 */
                return 1; 
            }
            /* 解析 FLASH 数据 */
            /* 获取标志位 */
            uint8_t flag = flash_data[0];
            /* 如果标志位为 1，则表示有默认值 */ 
            if (flag == 1) {  
                uint8_t* data = &flash_data[1];
                /* 字符串类型 */
                if (node->type == TYPE_STRING) {
                    /* 获取字符串长度 */
                    uint8_t length = flash_data[1]; 
                    /* 读取字符串数据 */ 
                    memcpy(output_value, &data[1], length);
                } else {
                    /* 非字符串类型 */
                    /* 读取其他数据类型 */
                    memcpy(output_value, data, node->size); 
                }
                /* 成功读取 */
                return 0; 
            } else {
                /* 如果标志位为 0，则没有默认值 */
                #ifdef ERROR_PRINTF
                PARAMTER_LOG("Warning: Parameter ID '%s' has no default value.\r\n", id);
                #endif
                /* 没有默认值，读取失败 */
                printf("Warning: Parameter ID '%s' has no default value.\r\n", id);
                return 1; 
            }
        }
        /* 查找下一个节点 */
        node = node->next; 
    }
    #ifdef ERROR_PRINTF
    PARAMTER_LOG("Error: Parameter ID '%s' not found.\r\n", id);
    #endif
    /* 找不到该 ID，读取失败 */
    return 1; 
}

/**
 * @函数名称: write_flash_parameter()
 * @功能描述: flash写函数
 * @输入: id ：查找的id
 *		new_value ：要写入的值
 * @输出: 无
 * @返回值: 0 ：成功 1：失败
 */
uint8_t write_flash_parameter(const char* id, void* new_value)
{
    /* 计算哈希值 */
    unsigned int index = hash_string(id); 
    HASH_NODE* node = hash_table[index];
    /* 查找哈希表中是否存在该 ID */
    while (node) {
        if (strcmp(node->id, id) == 0) {
            /* 找到对应的参数节点，更新默认值 */
            uint16_t size = get_data_size(node->type, new_value);
            /* 读取参数的值用于进行比较对重复数值的过滤 */
            uint8_t current_data[node->size + (node->type == TYPE_STRING ? 2 : 1)];
            // printf("node->address = %d\r\n",node->address);
            if (spi_flash_gd25q32esig_read(node->address, current_data, sizeof(current_data)) != 0) {
                /* 读取失败 */
                return 1; 
            }
            /* 解析 flash 数据 */
            /* 获取标志位 */
            uint8_t flag = current_data[0];
            // printf("new_value = 0x%X\n", *(uint16_t*)new_value);
            // printf("1111  %X   %X",current_data[1],current_data[2]);
            // printf("flag = %d\r\n",flag);
            /* 如果标志位为 1，则表示有默认值 */ 
            if (node->type == TYPE_STRING) {
                uint8_t length = strlen((char*)new_value) + 1;
                /* 比较字符串 */
                if (flag == 1 && memcmp(current_data + 2, new_value, length) == 0) {
                    /* 数据相同，不进行写入 */
                    #ifdef DEBUG_PRINTF
                    PARAMTER_LOG("No changes detected for parameter '%s', skipping write.\r\n", id);
                    #endif
                    return 0;
                }
            } else {
                /* 非字符串类型，比较数据 */
                if (flag == 1 && memcmp(current_data + 1, new_value, node->size) == 0) {
                    /* 数据相同，不进行写入 */
                    #ifdef DEBUG_PRINTF
                    PARAMTER_LOG("No changes detected for parameter '%s', skipping write.\r\n", id);
                    #endif
                    return 0;
                }
            }
            /* 如果是字符串类型，先写入标志位 + 字符串长度 + 数据 */
            /* 为标志位和长度预留空间 */
            uint8_t* flash_data = (uint8_t*)malloc(size + 2);
            /* 设置标志位为 1，表示有默认值 */
            flash_data[0] = 1; 
            if (node->type == TYPE_STRING) {
                /* 字符串长度 + '\0' */
                uint8_t length = strlen((char*)new_value) + 1; 
                /* 写入长度 */
                flash_data[1] = length; 
                /* 写入字符串内容 */
                memcpy(&flash_data[2], new_value, length); 
            } else {
                /* 非字符串类型，直接写入数据 */
                memcpy(&flash_data[1], new_value, size); 
                // printf("flash_data[1] = 0x%X, flash_data[2] = 0x%X\r\n", flash_data[1], flash_data[2]);
                
             }
            /* 写入 flash */
            if (erase_and_write_address(node->address, flash_data, size + (node->type == TYPE_STRING ? 2 : 1)) != 0) {
                free(flash_data);
                /* 写入失败 */
                return 1; 
            }
            /* 更新哈希表中的默认值 */
            // free(node->default_value);
            // node->default_value = malloc(size);
            // memcpy(node->default_value, new_value, size);
            #ifdef DEBUG_PRINTF
            PARAMTER_LOG("Parameter ID '%s' updated successfully.\r\n", id);
            #endif
            /* 释放临时内存 */
            free(flash_data);
            /* 写入成功 */
            return 0; 
        }
        /* 查找下一个节点 */
        node = node->next; 
    }
    #ifdef ERROR_PRINTF
    PARAMTER_LOG("Error: Parameter ID '%s' not found.\r\n", id);
    #endif
    /* 找不到该 ID，写入失败 */
    printf("write faulse33333\r\n");
    return 1; 
}

/**
 * @函数名称: reset_flash_parameter()
 * @功能描述: 重置单个参数到默认值
 * @输入: id ：查找的id
 * @输出: 无
 * @返回值: 0 ：成功 1：失败
 */
uint8_t reset_flash_parameter(const char* id) 
{
    /* 计算 id 对应的哈希表索引 */
    unsigned int index = hash_string(id);
    
    /* 查找哈希表中是否存在该参数 */
    HASH_NODE* node = hash_table[index];
    while (node) {
        if (strcmp(node->id, id) == 0) {
            /* 找到对应的节点，恢复默认值 */
            /* 计算默认值的大小 */
            uint16_t size = get_data_size(node->type, node->default_value);
            if (size == 0) {
                #ifdef ERROR_PRINTF
                PARAMTER_LOG("Error: Unsupported type for ID '%s'.\r\n", id);
                #endif
                /* 返回错误 */
                return 1; 
            }
            /* 表示有默认值 */
            uint8_t flag = 1;  
            /* 字符串类型的长度，其他类型为 0 */
            uint8_t length = 0; 

            /* 如果是字符串类型，需要写入长度 */
            if (node->type == TYPE_STRING) {
                length = (uint8_t)strlen((char*)node->default_value) + 1;
            }

            /* 构造 FLASH 写入缓冲区，包含 flag、长度（如果是字符串）和默认值 */
            /* 为 flag 和长度预留空间 */
            uint8_t* flash_data = (uint8_t*)malloc(size + 2); 
             
            flash_data[0] = flag;
            if (node->type == TYPE_STRING) {
                flash_data[1] = length;
                memcpy(&flash_data[2], node->default_value, size);
            } else {
                memcpy(&flash_data[1], node->default_value, size);
            }

            /* 写入默认值到 FLASH */
            uint8_t result = spi_flash_buffer_write(node->address, flash_data, size + (node->type == TYPE_STRING ? 2 : 1));

            /* 释放缓冲区并返回结果 */
            free(flash_data);
            /* 如果写入成功，返回 0，失败返回非 0 */
            return result; 
        }
        node = node->next;
    }
    /* 如果没有找到该参数 */
    #ifdef ERROR_PRINTF
    PARAMTER_LOG("Error: Parameter ID '%s' not found.\r\n", id);
    #endif
    /* 返回错误，未找到该参数 */
    return 1; 
}

/**
 * @函数名称: reset_flash_all_parameters()
 * @功能描述: 重置所有参数到默认值
 * @输入: 无
 * @输出: 无
 * @返回值: 0 ：成功 1：失败
 */
uint8_t reset_flash_all_parameters(void) 
{
    uint8_t revalue;
    
    /* 遍历哈希表中的所有节点 */
    for (unsigned int i = 0; i < HASH_TABLE_SIZE; i++) {
        HASH_NODE* node = hash_table[i];
        while (node) {
            /* 重置当前参数 */
            revalue = reset_flash_parameter(node->id);
            if (revalue != 0) {
                /* 如果重置失败，打印错误信息并返回 */
                #ifdef ERROR_PRINTF
                PARAMTER_LOG("Error: Parameter ID '%s' reset failed.\r\n", node->id);
                #endif
                return 1;
            }
            #ifdef DEBUG_PRINTF
            PARAMTER_LOG("Parameter ID '%s' has been reset.\r\n", node->id);
            #endif
            /* 查找下一个节点 */
            node = node->next; 
        }
    }
    /* 所有参数重置成功 */
    #ifdef DEBUG_PRINTF
    PARAMTER_LOG("All parameters have been reset.\r\n");
    #endif
    /* 如果所有参数都重置成功，返回0 */
    return 0;
}

/**
 * @函数名称: register_ram_parameter()
 * @功能描述: 在RAM中参数注册函数
 * @输入: id ：查找的id
 *		type ：参数类型
 *		default_value：参数默认值
 * @输出: 无
 * @返回值: 0 ：成功 1：失败 2: ID已存在 3：内存分配失败
 */
uint8_t register_ram_parameter(const char* id, DATA_TYPE type, void* default_value) 
{
    if (param_count >= MAX_RAM_PARAMETERS) {
        /* 参数表已满 */
        return 1; 
    }

    /* 检查ID是否已存在 */
    for (uint8_t i = 0; i < param_count; i++) {
        if (strcmp(param_table[i].id, id) == 0) {
            /* ID已存在 */
            return 2; 
        }
    }

    /* 创建新参数 */
    RAM_DATA* param = &param_table[param_count++];
    strncpy(param->id, id, sizeof(param->id) - 1);
    param->type = type;

    /* 分配内存并复制默认值 */
    switch (type) {
        case TYPE_INT:
            param->value = malloc(sizeof(int));
            param->default_value = malloc(sizeof(int));
            /* 内存分配失败 */
            if (!param->value || !param->default_value) return 3; 
            memcpy(param->value, default_value, sizeof(int));
            memcpy(param->default_value, default_value, sizeof(int));
            #ifdef RAM_PRINTF
            PARAMTER_LOG("Parameter registered: ID = %s, Type = INT, Value = %d, Default Value = %d, Value Address = %p, Default Value Address = %p\r\n",
                   param->id, *(int*)param->value, *(int*)param->default_value, param->value, param->default_value);
            #endif
            break;
        case TYPE_FLOAT:
            param->value = malloc(sizeof(float));
            param->default_value = malloc(sizeof(float));
            /* 内存分配失败 */
            if (!param->value || !param->default_value) return 3; 
            memcpy(param->value, default_value, sizeof(float));
            memcpy(param->default_value, default_value, sizeof(float));
            #ifdef RAM_PRINTF
            PARAMTER_LOG("Parameter registered: ID = %s, Type = FLOAT, Value = %f, Default Value = %f, Value Address = %p, Default Value Address = %p\r\n",
                   param->id, *(float*)param->value, *(float*)param->default_value, param->value, param->default_value);
            #endif
            break;
        case TYPE_BOOL:
            param->value = malloc(sizeof(bool));
            param->default_value = malloc(sizeof(bool));
            /* 内存分配失败 */
            if (!param->value || !param->default_value) return 3; 
            memcpy(param->value, default_value, sizeof(bool));
            memcpy(param->default_value, default_value, sizeof(bool));
            #ifdef RAM_PRINTF
            PARAMTER_LOG("Parameter registered: ID = %s, Type = BOOL, Value = %d, Default Value = %d, Value Address = %p, Default Value Address = %p\r\n",
                   param->id, *(bool*)param->value, *(bool*)param->default_value, param->value, param->default_value);
            #endif
            break;
        case TYPE_STRING:
            param->value = malloc(strlen((char*)default_value) + 1);
            param->default_value = malloc(strlen((char*)default_value) + 1);
            /* 内存分配失败 */
            if (!param->value || !param->default_value) return 3; 
            strcpy((char*)param->value, (char*)default_value);
            strcpy((char*)param->default_value, (char*)default_value);
            #ifdef RAM_PRINTF
            PARAMTER_LOG("Parameter registered: ID = %s, Type = STRING, Value = %s, Default Value = %s, Value Address = %p, Default Value Address = %p\r\n",
                   param->id, (char*)param->value, (char*)param->default_value, param->value, param->default_value);
            #endif
            break;
        case TYPE_UINT8:
            param->value = malloc(sizeof(uint8_t));
            param->default_value = malloc(sizeof(uint8_t));
            /* 内存分配失败 */
            if (!param->value || !param->default_value) return 3; 
            memcpy(param->value, default_value, sizeof(uint8_t));
            memcpy(param->default_value, default_value, sizeof(uint8_t));
            #ifdef RAM_PRINTF
            PARAMTER_LOG("Parameter registered: ID = %s, Type = UINT8, Value = %x, Default Value = %x, Value Address = %p, Default Value Address = %p\r\n",
                   param->id, *(uint8_t*)param->value, *(uint8_t*)param->default_value, param->value, param->default_value);
            #endif
            break;
        case TYPE_UINT16:
            param->value = malloc(sizeof(uint16_t));
            param->default_value = malloc(sizeof(uint16_t));
            /* 内存分配失败 */
            if (!param->value || !param->default_value) return 3; 
            memcpy(param->value, default_value, sizeof(uint16_t));
            memcpy(param->default_value, default_value, sizeof(uint16_t));
            #ifdef RAM_PRINTF
            PARAMTER_LOG("Parameter registered: ID = %s, Type = UINT16, Value = %x, Default Value = %x, Value Address = %p, Default Value Address = %p\r\n",
                   param->id, *(uint16_t*)param->value, *(uint16_t*)param->default_value, param->value, param->default_value);
            #endif
            break;
        case TYPE_UINT32:
            param->value = malloc(sizeof(uint32_t));
            param->default_value = malloc(sizeof(uint32_t));
            /* 内存分配失败 */
            if (!param->value || !param->default_value) return 3; 
            memcpy(param->value, default_value, sizeof(uint32_t));
            memcpy(param->default_value, default_value, sizeof(uint32_t));
            #ifdef RAM_PRINTF
            PARAMTER_LOG("Parameter registered: ID = %s, Type = UINT32, Value = %lx, Default Value = %lx, Value Address = %p, Default Value Address = %p\r\n",
                   param->id, *(uint32_t*)param->value, *(uint32_t*)param->default_value, param->value, param->default_value);
            #endif
            break;
    }
    /* 注册成功 */
    return 0; 
}

/**
 * @函数名称: write_ram_parameter()
 * @功能描述: RAM写函数
 * @输入: id ：查找的id
 *		new_value ：要写入的值
 * @输出: 无
 * @返回值: 0 ：成功 1：失败
 */
uint8_t write_ram_parameter(const char* id, void* new_value) 
{
    for (uint8_t i = 0; i < param_count; i++) {
        if (strcmp(param_table[i].id, id) == 0) {
            RAM_DATA* param = &param_table[i];
            switch (param->type) {
                case TYPE_INT:
                    memcpy(param->value, new_value, sizeof(int));
                    break;
                case TYPE_FLOAT:
                    memcpy(param->value, new_value, sizeof(float));
                    break;
                case TYPE_BOOL:
                    memcpy(param->value, new_value, sizeof(bool));
                    break;
                case TYPE_STRING:
                    strcpy((char*)param->value, (char*)new_value);
                    break;
                case TYPE_UINT8:
                    memcpy(param->value, new_value, sizeof(uint8_t));
                    break;
                case TYPE_UINT16:
                    memcpy(param->value, new_value, sizeof(uint16_t));
                    break;
                case TYPE_UINT32:
                    memcpy(param->value, new_value, sizeof(uint32_t));
                    break;
            }
            /* 写入成功 */
            return 0; 
        }
    }
    /* 参数未找到 */
    return 1; 
}

/**
 * @函数名称: read_ram_parameter()
 * @功能描述: RAM读函数
 * @输入: id ：查找的id
 *		output_value ：读取到的输出值
 * @输出: 无
 * @返回值: 0 ：成功 1：失败
 */
uint8_t read_ram_parameter(const char* id, void* output_value) 
{
    for (uint8_t i = 0; i < param_count; i++) {
        if (strcmp(param_table[i].id, id) == 0) {
            RAM_DATA* param = &param_table[i];
            switch (param->type) {
                case TYPE_INT:
                    memcpy(output_value, param->value, sizeof(int));
                    break;
                case TYPE_FLOAT:
                    memcpy(output_value, param->value, sizeof(float));
                    break;
                case TYPE_BOOL:
                    memcpy(output_value, param->value, sizeof(bool));
                    break;
                case TYPE_STRING:
                    strcpy((char*)output_value, (char*)param->value);
                    break;
                case TYPE_UINT8:
                    memcpy(output_value, param->value, sizeof(uint8_t));
                    break;
                case TYPE_UINT16:
                    memcpy(output_value, param->value, sizeof(uint16_t));
                    break;
                case TYPE_UINT32:
                    memcpy(output_value, param->value, sizeof(uint32_t));
                    break;
            }
            /* 读取成功 */
            return 0; 
        }
    }
    /* 参数未找到 */
    return 1; 
}

/**
 * @函数名称: free_ram_specified_parameter()
 * @功能描述: 释放指定参数的内存
 * @输入: id ：查找的id
 * @输出: 无
 * @返回值: 0 ：成功 1：失败
 */
uint8_t free_ram_specified_parameter(const char* id) 
{
    for (uint8_t i = 0; i < param_count; i++) {
        if (strcmp(param_table[i].id, id) == 0) {
            RAM_DATA* param = &param_table[i];
            
            /* 释放 value 和 default_value 指向的内存 */
            if (param->value != NULL) {
                free(param->value);
                /* 防止空指针 */
                param->value = NULL;  
            }
            if (param->default_value != NULL) {
                free(param->default_value);
                /* 防止空指针 */
                param->default_value = NULL;  
            }

            /* 清空当前参数信息 清除 id */
            memset(param->id, 0, sizeof(param->id)); 
            /* 重置参数类型 */ 
            param->type = 0;  

            /* 将后续的参数向前移动 */
            for (uint8_t j = i; j < param_count - 1; j++) {
                param_table[j] = param_table[j + 1];
            }
            /* 减少参数数量 */
            param_count--;  
            #ifdef RAM_PRINTF
            PARAMTER_LOG("Parameter freed: ID = %s\r\n", id);
            #endif
            /* 成功 */
            return 0;  
        }
    }
    #ifdef RAM_PRINTF
    PARAMTER_LOG("Parameter with ID = %s not found.\r\n", id);
    #endif
    /* 参数未找到 */
    return 1;  
}

/**
 * @函数名称: free_ram_parameters()
 * @功能描述: 释放RAM参数内存
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void free_ram_parameters(void) 
{
    for (uint8_t i = 0; i < param_count; i++) {
        free(param_table[i].value);
        free(param_table[i].default_value);
    }
    param_count = 0;
}

/* 用户实现***********MCU保护参数的实现******************* */
#if 1
ROM_MCU_PARAMETER_TYPDEF rom_parameter = {0};

/**
 * @函数名称: parameter_rom_init()
 * @功能描述: 关于MCU保护的 flash 存储参数注册
 * @输入: ROM_MCU_PARAMETER_TYPDEF：MCU保护结构体参数
 * @输出: 无
 * @返回值: 0成功 1失败
 */
uint8_t parameter_rom_register(ROM_MCU_PARAMETER_TYPDEF* param_rom_mcu)
{
    uint8_t err = 0;

    if (param_rom_mcu == NULL) {
        /* 防止空指针访问 */
        return 1;
    }
    
    /* 将 init_parameter 的所有字段初始化为 0 */
    memset(param_rom_mcu, 0, sizeof(ROM_MCU_PARAMETER_TYPDEF));

    err |= register_flash_parameter("cell_ov1_alarm",                      TYPE_UINT16,    &param_rom_mcu->cell_ov1_alarm);
    err |= register_flash_parameter("cell_iv1_alars",                      TYPE_UINT16,    &param_rom_mcu->cell_iv1_alars);
    // err |= register_flash_parameter("cell_ov1_alarm_delay_time",           TYPE_UINT16,    &param_rom_mcu->cell_ov1_alarm_delay_time);
    // err |= register_flash_parameter("cell_ov1_release",                    TYPE_UINT16,    &param_rom_mcu->cell_ov1_release);
    // err |= register_flash_parameter("cell_ov1_release_delay_time",         TYPE_UINT16,    &param_rom_mcu->cell_ov1_release_delay_time);

    // err |= register_flash_parameter("cell_ov2_alarm",                      TYPE_UINT16,    &param_rom_mcu->cell_ov2_alarm);
    // err |= register_flash_parameter("cell_ov2_alarm_delay_time",           TYPE_UINT16,    &param_rom_mcu->cell_ov2_alarm_delay_time);
    // err |= register_flash_parameter("cell_ov2_release",                    TYPE_UINT16,    &param_rom_mcu->cell_ov2_release);
    // err |= register_flash_parameter("cell_ov2_release_delay_time",         TYPE_UINT16,    &param_rom_mcu->cell_ov2_release_delay_time);    
    
    // err |= register_flash_parameter("cell_ov3_alarm",                      TYPE_UINT16,    &param_rom_mcu->cell_ov3_alarm);
    // err |= register_flash_parameter("cell_ov3_alarm_delay_time",           TYPE_UINT16,    &param_rom_mcu->cell_ov3_alarm_delay_time);
    // err |= register_flash_parameter("cell_ov3_release",                    TYPE_UINT16,    &param_rom_mcu->cell_ov3_release);
    // err |= register_flash_parameter("cell_ov3_release_delay_time",         TYPE_UINT16,    &param_rom_mcu->cell_ov3_release_delay_time);    
    // err |= register_flash_parameter("cell_ov3_unlock_soc",                 TYPE_UINT16,    &param_rom_mcu->cell_ov3_unlock_soc);
    // err |= register_flash_parameter("cell_ov3_unlock_charge_current",      TYPE_UINT16,    &param_rom_mcu->cell_ov3_unlock_charge_current);


    // err |= register_flash_parameter("cell_uv1_alarm",                      TYPE_UINT16,    &param_rom_mcu->cell_uv1_alarm);
    // err |= register_flash_parameter("cell_uv1_alarm_delay_time",           TYPE_UINT16,    &param_rom_mcu->cell_uv1_alarm_delay_time);
    // err |= register_flash_parameter("cell_uv1_release",                    TYPE_UINT16,    &param_rom_mcu->cell_uv1_release);
    // err |= register_flash_parameter("cell_uv1_release_delay_time",         TYPE_UINT16,    &param_rom_mcu->cell_uv1_release_delay_time);

    // err |= register_flash_parameter("cell_uv2_alarm",                      TYPE_UINT16,    &param_rom_mcu->cell_uv2_alarm);
    // err |= register_flash_parameter("cell_uv2_alarm_delay_time",           TYPE_UINT16,    &param_rom_mcu->cell_uv2_alarm_delay_time);
    // err |= register_flash_parameter("cell_uv2_release",                    TYPE_UINT16,    &param_rom_mcu->cell_uv2_release);
    // err |= register_flash_parameter("cell_uv2_release_delay_time",         TYPE_UINT16,    &param_rom_mcu->cell_uv2_release_delay_time);    
    
    // err |= register_flash_parameter("cell_uv3_alarm",                      TYPE_UINT16,    &param_rom_mcu->cell_uv3_alarm);
    // err |= register_flash_parameter("cell_uv3_alarm_delay_time",           TYPE_UINT16,    &param_rom_mcu->cell_uv3_alarm_delay_time);
    // err |= register_flash_parameter("cell_uv3_release",                    TYPE_UINT16,    &param_rom_mcu->cell_uv3_release);
    // err |= register_flash_parameter("cell_uv3_release_delay_time",         TYPE_UINT16,    &param_rom_mcu->cell_uv3_release_delay_time);    
    // err |= register_flash_parameter("cell_uv3_unlock_charge_current",      TYPE_UINT16,    &param_rom_mcu->cell_uv3_unlock_charge_current);


    // err |= register_flash_parameter("total_ov1_alarm",                     TYPE_UINT16,    &param_rom_mcu->total_ov1_alarm);
    // err |= register_flash_parameter("total_ov1_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->total_ov1_alarm_delay_time);
    // err |= register_flash_parameter("total_ov1_release",                   TYPE_UINT16,    &param_rom_mcu->total_ov1_release);
    // err |= register_flash_parameter("total_ov1_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->total_ov1_release_delay_time);

    // err |= register_flash_parameter("total_ov2_alarm",                     TYPE_UINT16,    &param_rom_mcu->total_ov2_alarm);
    // err |= register_flash_parameter("total_ov2_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->total_ov2_alarm_delay_time);
    // err |= register_flash_parameter("total_ov2_release",                   TYPE_UINT16,    &param_rom_mcu->total_ov2_release);
    // err |= register_flash_parameter("total_ov2_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->total_ov2_release_delay_time);    
    
    // err |= register_flash_parameter("total_ov3_alarm",                     TYPE_UINT16,    &param_rom_mcu->total_ov3_alarm);
    // err |= register_flash_parameter("total_ov3_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->total_ov3_alarm_delay_time);
    // err |= register_flash_parameter("total_ov3_release",                   TYPE_UINT16,    &param_rom_mcu->total_ov3_release);
    // err |= register_flash_parameter("total_ov3_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->total_ov3_release_delay_time);    
    // err |= register_flash_parameter("total_ov3_unlock_soc",                TYPE_UINT16,    &param_rom_mcu->total_ov3_unlock_soc);
    // err |= register_flash_parameter("total_ov3_unlock_charge_current",     TYPE_UINT16,    &param_rom_mcu->total_ov3_unlock_charge_current);

    // err |= register_flash_parameter("total_uv1_alarm",                     TYPE_UINT16,    &param_rom_mcu->total_uv1_alarm);
    // err |= register_flash_parameter("total_uv1_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->total_uv1_alarm_delay_time);
    // err |= register_flash_parameter("total_uv1_release",                   TYPE_UINT16,    &param_rom_mcu->total_uv1_release);
    // err |= register_flash_parameter("total_uv1_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->total_uv1_release_delay_time);

    // err |= register_flash_parameter("total_uv2_alarm",                     TYPE_UINT16,    &param_rom_mcu->total_uv2_alarm);
    // err |= register_flash_parameter("total_uv2_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->total_uv2_alarm_delay_time);
    // err |= register_flash_parameter("total_uv2_release",                   TYPE_UINT16,    &param_rom_mcu->total_uv2_release);
    // err |= register_flash_parameter("total_uv2_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->total_uv2_release_delay_time);    
    
    // err |= register_flash_parameter("total_uv3_alarm",                     TYPE_UINT16,    &param_rom_mcu->total_uv3_alarm);
    // err |= register_flash_parameter("total_uv3_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->total_uv3_alarm_delay_time);
    // err |= register_flash_parameter("total_uv3_release",                   TYPE_UINT16,    &param_rom_mcu->total_uv3_release);
    // err |= register_flash_parameter("total_uv3_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->total_uv3_release_delay_time);    
    // err |= register_flash_parameter("total_uv3_unlock_charge_current",     TYPE_UINT16,    &param_rom_mcu->total_uv3_unlock_charge_current);


    // err |= register_flash_parameter("cell_diff_ov1_alarm",                     TYPE_UINT16,    &param_rom_mcu->cell_diff_ov1_alarm);
    // err |= register_flash_parameter("cell_diff_ov1_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->cell_diff_ov1_alarm_delay_time);
    // err |= register_flash_parameter("cell_diff_ov1_release",                   TYPE_UINT16,    &param_rom_mcu->cell_diff_ov1_release);
    // err |= register_flash_parameter("cell_diff_ov1_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->cell_diff_ov1_release_delay_time);

    // err |= register_flash_parameter("cell_diff_ov2_alarm",                     TYPE_UINT16,    &param_rom_mcu->cell_diff_ov2_alarm);
    // err |= register_flash_parameter("cell_diff_ov2_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->cell_diff_ov2_alarm_delay_time);
    // err |= register_flash_parameter("cell_diff_ov2_release",                   TYPE_UINT16,    &param_rom_mcu->cell_diff_ov2_release);
    // err |= register_flash_parameter("cell_diff_ov2_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->cell_diff_ov2_release_delay_time);  

    // /* ============================= 电流相关参数 ============================= */
    // err |= register_flash_parameter("charge_oc1_alarm",                         TYPE_UINT16,    &param_rom_mcu->charge_oc1_alarm);
    // err |= register_flash_parameter("charge_oc1_alarm_delay_time",              TYPE_UINT16,    &param_rom_mcu->charge_oc1_alarm_delay_time);
    // err |= register_flash_parameter("charge_oc2_alarm",                         TYPE_UINT16,    &param_rom_mcu->charge_oc2_alarm);
    // err |= register_flash_parameter("charge_oc2_alarm_delay_time",              TYPE_UINT16,    &param_rom_mcu->charge_oc2_alarm_delay_time);    
    // err |= register_flash_parameter("charge_oc3_alarm",                         TYPE_UINT16,    &param_rom_mcu->charge_oc3_alarm);
    // err |= register_flash_parameter("charge_oc3_alarm_delay_time",              TYPE_UINT16,    &param_rom_mcu->charge_oc3_alarm_delay_time);

    // err |= register_flash_parameter("discharge_oc1_alarm",                      TYPE_UINT16,    &param_rom_mcu->discharge_oc1_alarm);
    // err |= register_flash_parameter("discharge_oc1_alarm_delay_time",           TYPE_UINT16,    &param_rom_mcu->discharge_oc1_alarm_delay_time);
    // err |= register_flash_parameter("discharge_oc2_alarm",                      TYPE_UINT16,    &param_rom_mcu->discharge_oc2_alarm);
    // err |= register_flash_parameter("discharge_oc2_alarm_delay_time",           TYPE_UINT16,    &param_rom_mcu->discharge_oc2_alarm_delay_time);    
    // err |= register_flash_parameter("discharge_oc3_alarm",                      TYPE_UINT16,    &param_rom_mcu->discharge_oc3_alarm);
    // err |= register_flash_parameter("discharge_oc3_alarm_delay_time",           TYPE_UINT16,    &param_rom_mcu->discharge_oc3_alarm_delay_time);
    
    // err |= register_flash_parameter("short_circuit_alarm",                      TYPE_UINT16,    &param_rom_mcu->short_circuit_alarm);
    // err |= register_flash_parameter("short_circuit_alarm_delay_time",           TYPE_UINT16,    &param_rom_mcu->short_circuit_alarm_delay_time);

    // /* ============================= 温度相关参数 ============================= */
    // err |= register_flash_parameter("charge_cell_ot1_alarm",                     TYPE_UINT16,    &param_rom_mcu->charge_cell_ot1_alarm);
    // err |= register_flash_parameter("charge_cell_ot1_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->charge_cell_ot1_alarm_delay_time);
    // err |= register_flash_parameter("charge_cell_ot1_release",                   TYPE_UINT16,    &param_rom_mcu->charge_cell_ot1_release);
    // err |= register_flash_parameter("charge_cell_ot1_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->charge_cell_ot1_release_delay_time);

    // err |= register_flash_parameter("charge_cell_ot2_alarm",                     TYPE_UINT16,    &param_rom_mcu->charge_cell_ot2_alarm);
    // err |= register_flash_parameter("charge_cell_ot2_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->charge_cell_ot2_alarm_delay_time);
    // err |= register_flash_parameter("charge_cell_ot2_release",                   TYPE_UINT16,    &param_rom_mcu->charge_cell_ot2_release);
    // err |= register_flash_parameter("charge_cell_ot2_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->charge_cell_ot2_release_delay_time);    
    
    // err |= register_flash_parameter("charge_cell_ot3_alarm",                     TYPE_UINT16,    &param_rom_mcu->charge_cell_ot3_alarm);
    // err |= register_flash_parameter("charge_cell_ot3_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->charge_cell_ot3_alarm_delay_time);
    // err |= register_flash_parameter("charge_cell_ot3_release",                   TYPE_UINT16,    &param_rom_mcu->charge_cell_ot3_release);
    // err |= register_flash_parameter("charge_cell_ot3_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->charge_cell_ot3_release_delay_time);  
    
    // err |= register_flash_parameter("charge_cell_ut1_alarm",                     TYPE_UINT16,    &param_rom_mcu->charge_cell_ut1_alarm);
    // err |= register_flash_parameter("charge_cell_ut1_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->charge_cell_ut1_alarm_delay_time);
    // err |= register_flash_parameter("charge_cell_ut1_release",                   TYPE_UINT16,    &param_rom_mcu->charge_cell_ut1_release);
    // err |= register_flash_parameter("charge_cell_ut1_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->charge_cell_ut1_release_delay_time);

    // err |= register_flash_parameter("charge_cell_ut2_alarm",                     TYPE_UINT16,    &param_rom_mcu->charge_cell_ut2_alarm);
    // err |= register_flash_parameter("charge_cell_ut2_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->charge_cell_ut2_alarm_delay_time);
    // err |= register_flash_parameter("charge_cell_ut2_release",                   TYPE_UINT16,    &param_rom_mcu->charge_cell_ut2_release);
    // err |= register_flash_parameter("charge_cell_ut2_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->charge_cell_ut2_release_delay_time);    
    
    // err |= register_flash_parameter("charge_cell_ut3_alarm",                     TYPE_UINT16,    &param_rom_mcu->charge_cell_ut3_alarm);
    // err |= register_flash_parameter("charge_cell_ut3_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->charge_cell_ut3_alarm_delay_time);
    // err |= register_flash_parameter("charge_cell_ut3_release",                   TYPE_UINT16,    &param_rom_mcu->charge_cell_ut3_release);
    // err |= register_flash_parameter("charge_cell_ut3_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->charge_cell_ut3_release_delay_time);     


    // err |= register_flash_parameter("discharge_cell_ot1_alarm",                     TYPE_UINT16,    &param_rom_mcu->discharge_cell_ot1_alarm);
    // err |= register_flash_parameter("discharge_cell_ot1_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->discharge_cell_ot1_alarm_delay_time);
    // err |= register_flash_parameter("discharge_cell_ot1_release",                   TYPE_UINT16,    &param_rom_mcu->discharge_cell_ot1_release);
    // err |= register_flash_parameter("discharge_cell_ot1_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->discharge_cell_ot1_release_delay_time);

    // err |= register_flash_parameter("discharge_cell_ot2_alarm",                     TYPE_UINT16,    &param_rom_mcu->discharge_cell_ot2_alarm);
    // err |= register_flash_parameter("discharge_cell_ot2_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->discharge_cell_ot2_alarm_delay_time);
    // err |= register_flash_parameter("discharge_cell_ot2_release",                   TYPE_UINT16,    &param_rom_mcu->discharge_cell_ot2_release);
    // err |= register_flash_parameter("discharge_cell_ot2_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->discharge_cell_ot2_release_delay_time);    
   
    // err |= register_flash_parameter("discharge_cell_ot3_alarm",                     TYPE_UINT16,    &param_rom_mcu->discharge_cell_ot3_alarm);
    // err |= register_flash_parameter("discharge_cell_ot3_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->discharge_cell_ot3_alarm_delay_time);
    // err |= register_flash_parameter("discharge_cell_ot3_release",                   TYPE_UINT16,    &param_rom_mcu->discharge_cell_ot3_release);
    // err |= register_flash_parameter("discharge_cell_ot3_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->discharge_cell_ot3_release_delay_time);  
    
    // err |= register_flash_parameter("discharge_cell_ut1_alarm",                     TYPE_UINT16,    &param_rom_mcu->discharge_cell_ut1_alarm);
    // err |= register_flash_parameter("discharge_cell_ut1_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->discharge_cell_ut1_alarm_delay_time);
    // err |= register_flash_parameter("discharge_cell_ut1_release",                   TYPE_UINT16,    &param_rom_mcu->discharge_cell_ut1_release);
    // err |= register_flash_parameter("discharge_cell_ut1_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->discharge_cell_ut1_release_delay_time);

    // err |= register_flash_parameter("discharge_cell_ut2_alarm",                     TYPE_UINT16,    &param_rom_mcu->discharge_cell_ut2_alarm);
    // err |= register_flash_parameter("discharge_cell_ut2_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->discharge_cell_ut2_alarm_delay_time);
    // err |= register_flash_parameter("discharge_cell_ut2_release",                   TYPE_UINT16,    &param_rom_mcu->discharge_cell_ut2_release);
    // err |= register_flash_parameter("discharge_cell_ut2_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->discharge_cell_ut2_release_delay_time);    
 
    // err |= register_flash_parameter("discharge_cell_ut3_alarm",                     TYPE_UINT16,    &param_rom_mcu->discharge_cell_ut3_alarm);
    // err |= register_flash_parameter("discharge_cell_ut3_alarm_delay_time",          TYPE_UINT16,    &param_rom_mcu->discharge_cell_ut3_alarm_delay_time);
    // err |= register_flash_parameter("discharge_cell_ut3_release",                   TYPE_UINT16,    &param_rom_mcu->discharge_cell_ut3_release);
    // err |= register_flash_parameter("discharge_cell_ut3_release_delay_time",        TYPE_UINT16,    &param_rom_mcu->discharge_cell_ut3_release_delay_time);

    
    // err |= register_flash_parameter("cell_ot1_alarm",                               TYPE_UINT16,    &param_rom_mcu->cell_ot1_alarm);
    // err |= register_flash_parameter("cell_ot1_alarm_delay_time",                    TYPE_UINT16,    &param_rom_mcu->cell_ot1_alarm_delay_time);
    // err |= register_flash_parameter("cell_ot1_release",                             TYPE_UINT16,    &param_rom_mcu->cell_ot1_release);
    // err |= register_flash_parameter("cell_ot1_release_delay_time",                  TYPE_UINT16,    &param_rom_mcu->cell_ot1_release_delay_time);

    // err |= register_flash_parameter("cell_ot2_alarm",                               TYPE_UINT16,    &param_rom_mcu->cell_ot2_alarm);
    // err |= register_flash_parameter("cell_ot2_alarm_delay_time",                    TYPE_UINT16,    &param_rom_mcu->cell_ot2_alarm_delay_time);
    // err |= register_flash_parameter("cell_ot2_release",                             TYPE_UINT16,    &param_rom_mcu->cell_ot2_release);
    // err |= register_flash_parameter("cell_ot2_release_delay_time",                  TYPE_UINT16,    &param_rom_mcu->cell_ot2_release_delay_time);  

    // err |= register_flash_parameter("cell_diff_temp1_alarm",                        TYPE_UINT16,    &param_rom_mcu->cell_diff_temp1_alarm);
    // err |= register_flash_parameter("cell_diff_temp1_alarm_delay_time",             TYPE_UINT16,    &param_rom_mcu->cell_diff_temp1_alarm_delay_time);
    // err |= register_flash_parameter("cell_diff_temp1_release",                      TYPE_UINT16,    &param_rom_mcu->cell_diff_temp1_release);
    // err |= register_flash_parameter("cell_diff_temp1_release_delay_time",           TYPE_UINT16,    &param_rom_mcu->cell_diff_temp1_release_delay_time);
    
    // err |= register_flash_parameter("cell_diff_temp2_alarm",                        TYPE_UINT16,    &param_rom_mcu->cell_diff_temp2_alarm);
    // err |= register_flash_parameter("cell_diff_temp2_alarm_delay_time",             TYPE_UINT16,    &param_rom_mcu->cell_diff_temp2_alarm_delay_time);
    // err |= register_flash_parameter("cell_diff_temp2_release",                      TYPE_UINT16,    &param_rom_mcu->cell_diff_temp2_release);
    // err |= register_flash_parameter("cell_diff_temp2_release_delay_time",           TYPE_UINT16,    &param_rom_mcu->cell_diff_temp2_release_delay_time);  


    // err |= register_flash_parameter("env_ot_high1_alarm",                           TYPE_UINT16,    &param_rom_mcu->env_ot_high1_alarm);
    // err |= register_flash_parameter("env_ot_high1_alarm_delay_time",                TYPE_UINT16,    &param_rom_mcu->env_ot_high1_alarm_delay_time);
    // err |= register_flash_parameter("env_ot_high1_release",                         TYPE_UINT16,    &param_rom_mcu->env_ot_high1_release);
    // err |= register_flash_parameter("env_ot_high1_release_delay_time",              TYPE_UINT16,    &param_rom_mcu->env_ot_high1_release_delay_time);
    
    // err |= register_flash_parameter("env_ot_high2_alarm",                           TYPE_UINT16,    &param_rom_mcu->env_ot_high2_alarm);
    // err |= register_flash_parameter("env_ot_high2_alarm_delay_time",                TYPE_UINT16,    &param_rom_mcu->env_ot_high2_alarm_delay_time);
    // err |= register_flash_parameter("env_ot_high2_release",                         TYPE_UINT16,    &param_rom_mcu->env_ot_high2_release);
    // err |= register_flash_parameter("env_ot_high2_release_delay_time",              TYPE_UINT16,    &param_rom_mcu->env_ot_high2_release_delay_time);  

    // err |= register_flash_parameter("env_ut_low1_alarm",                            TYPE_UINT16,    &param_rom_mcu->env_ut_low1_alarm);
    // err |= register_flash_parameter("env_ut_low1_alarm_delay_time",                 TYPE_UINT16,    &param_rom_mcu->env_ut_low1_alarm_delay_time);
    // err |= register_flash_parameter("env_ut_low1_release",                          TYPE_UINT16,    &param_rom_mcu->env_ut_low1_release);
    // err |= register_flash_parameter("env_ut_low1_release_delay_time",               TYPE_UINT16,    &param_rom_mcu->env_ut_low1_release_delay_time);

    // err |= register_flash_parameter("env_ut_low2_alarm",                            TYPE_UINT16,    &param_rom_mcu->env_ut_low2_alarm);
    // err |= register_flash_parameter("env_ut_low2_alarm_delay_time",                 TYPE_UINT16,    &param_rom_mcu->env_ut_low2_alarm_delay_time);
    // err |= register_flash_parameter("env_ut_low2_release",                          TYPE_UINT16,    &param_rom_mcu->env_ut_low2_release);
    // err |= register_flash_parameter("env_ut_low2_release_delay_time",               TYPE_UINT16,    &param_rom_mcu->env_ut_low2_release_delay_time);  


    // err |= register_flash_parameter("cell_balance_start_voltage",                   TYPE_UINT16,    &param_rom_mcu->cell_balance_start_voltage);
    // err |= register_flash_parameter("cell_balance_start_diff",                      TYPE_UINT16,    &param_rom_mcu->cell_balance_start_diff);
    // err |= register_flash_parameter("cell_sleep_voltage",                           TYPE_UINT16,    &param_rom_mcu->cell_sleep_voltage);
    // err |= register_flash_parameter("cell_sleep_duration",                          TYPE_UINT16,    &param_rom_mcu->cell_sleep_duration);  
    // err |= register_flash_parameter("cell_full_charge_voltage",                     TYPE_UINT16,    &param_rom_mcu->cell_full_charge_voltage);
    // err |= register_flash_parameter("full_charge_current",                          TYPE_UINT16,    &param_rom_mcu->full_charge_current);  

    // err |= register_flash_parameter("battery_capacity_initial",                     TYPE_UINT16,    &param_rom_mcu->battery_capacity_initial);
    // err |= register_flash_parameter("battery_capacity_current",                     TYPE_UINT16,    &param_rom_mcu->battery_capacity_current);  
    // err |= register_flash_parameter("cycle_count",                                  TYPE_UINT16,    &param_rom_mcu->cycle_count);
    // err |= register_flash_parameter("total_runtime",                                TYPE_UINT16,    &param_rom_mcu->total_runtime);  

    // err |= register_flash_parameter("soc_initial",                                  TYPE_UINT16,    &param_rom_mcu->soc_initial);
    // err |= register_flash_parameter("soc_low_alarm",                                TYPE_UINT16,    &param_rom_mcu->soc_low_alarm);
    // err |= register_flash_parameter("soh_initial",                                  TYPE_UINT16,    &param_rom_mcu->soh_initial);  
    // err |= register_flash_parameter("soh_alarm",                                    TYPE_UINT16,    &param_rom_mcu->soh_alarm);
    // err |= register_flash_parameter("sod_deep_discharge",                           TYPE_UINT16,    &param_rom_mcu->sod_deep_discharge); 

    // err |= register_flash_parameter("heater_off_temp",                              TYPE_UINT16,    &param_rom_mcu->heater_off_temp);
    // err |= register_flash_parameter("device_sn_high",                               TYPE_UINT16,    &param_rom_mcu->device_sn_high);
    // err |= register_flash_parameter("device_sn_low",                                TYPE_UINT16,    &param_rom_mcu->device_sn_low);
    // err |= register_flash_parameter("device_id",                                    TYPE_UINT16,    &param_rom_mcu->device_id);
    // err |= register_flash_parameter("buzzer_duration",                              TYPE_UINT16,    &param_rom_mcu->buzzer_duration);
    // err |= register_flash_parameter("power_on_activation",                          TYPE_UINT16,    &param_rom_mcu->power_on_activation);
    // err |= register_flash_parameter("shutdown_cmd",                                 TYPE_UINT16,    &param_rom_mcu->shutdown_cmd);
    
    // err |= register_flash_parameter("heater_on_cmd",                                TYPE_UINT16,    &param_rom_mcu->heater_on_cmd);
    // err |= register_flash_parameter("heater_off_cmd",                               TYPE_UINT16,    &param_rom_mcu->heater_off_cmd);
    // err |= register_flash_parameter("heater_auto_off_temp",                         TYPE_UINT16,    &param_rom_mcu->heater_auto_off_temp);

    // err |= register_flash_parameter("indicator_on_cmd",                             TYPE_UINT16,    &param_rom_mcu->indicator_on_cmd);
    // err |= register_flash_parameter("indicator_off_cmd",                            TYPE_UINT16,    &param_rom_mcu->indicator_off_cmd);

    // err |= register_flash_parameter("alarm_light_on_cmd",                           TYPE_UINT16,    &param_rom_mcu->alarm_light_on_cmd);
    // err |= register_flash_parameter("alarm_light_off_cmd",                          TYPE_UINT16,    &param_rom_mcu->alarm_light_off_cmd);

    // err |= register_flash_parameter("buzzer_enable_cmd",                            TYPE_UINT16,    &param_rom_mcu->buzzer_enable_cmd);
    // err |= register_flash_parameter("buzzer_disable_cmd",                           TYPE_UINT16,    &param_rom_mcu->buzzer_disable_cmd);
    // err |= register_flash_parameter("buzzer_status",                                TYPE_UINT16,    &param_rom_mcu->buzzer_status);
    // err |= register_flash_parameter("buzzer_stop_cmd",                              TYPE_UINT16,    &param_rom_mcu->buzzer_stop_cmd);

    // err |= register_flash_parameter("current_calib_prepare",                        TYPE_UINT16,    &param_rom_mcu->current_calib_prepare);
    // err |= register_flash_parameter("current_calib_set_value",                      TYPE_UINT16,    &param_rom_mcu->current_calib_set_value);
    // err |= register_flash_parameter("current_calib_start",                          TYPE_UINT16,    &param_rom_mcu->current_calib_start);
    // err |= register_flash_parameter("current_calib_verify",                         TYPE_UINT16,    &param_rom_mcu->current_calib_verify);

    // err |= register_flash_parameter("short_circuit_release_on",                     TYPE_UINT16,    &param_rom_mcu->short_circuit_release_on);
    // err |= register_flash_parameter("short_circuit_release_off",                    TYPE_UINT16,    &param_rom_mcu->short_circuit_release_off);

    // err |= register_flash_parameter("pre_discharge_on",                             TYPE_UINT16,    &param_rom_mcu->pre_discharge_on);
    // err |= register_flash_parameter("pre_discharge_off",                            TYPE_UINT16,    &param_rom_mcu->pre_discharge_off);

    // err |= register_flash_parameter("discharge_mos_on",                             TYPE_UINT16,    &param_rom_mcu->discharge_mos_on);
    // err |= register_flash_parameter("discharge_mos_off",                            TYPE_UINT16,    &param_rom_mcu->discharge_mos_off);

    // err |= register_flash_parameter("charge_mos_on",                                TYPE_UINT16,    &param_rom_mcu->charge_mos_on);
    // err |= register_flash_parameter("charge_mos_off",                               TYPE_UINT16,    &param_rom_mcu->charge_mos_off);

    // err |= register_flash_parameter("fuse_blow_cmd",                                TYPE_UINT16,    &param_rom_mcu->fuse_blow_cmd);
    // err |= register_flash_parameter("fuse_reset_cmd",                               TYPE_UINT16,    &param_rom_mcu->fuse_reset_cmd);

    // err |= register_flash_parameter("afe_eeprom_save_cmd",                          TYPE_UINT16,    &param_rom_mcu->afe_eeprom_save_cmd);
    // err |= register_flash_parameter("mcu_protection_save_cmd",                      TYPE_UINT16,    &param_rom_mcu->mcu_protection_save_cmd);

    // err |= register_flash_parameter("charge_discharge_ctrl",                        TYPE_UINT16,    &param_rom_mcu->charge_discharge_ctrl);
    // err |= register_flash_parameter("mcu_init_flag",                                TYPE_UINT16,    &param_rom_mcu->mcu_init_flag);
    printf("err=%d\r\n",err);
    /* --其他 */
    if (err) {
        return 1;
    }
    return 0;
}


/**
 * @函数名称: parameter_rom_default_init()
 * @功能描述: eeprom 存储参数初始化默认值
 * @输入: 无
 * @输出: 无
 * @返回值: 0成功 1失败
 */
uint8_t parameter_rom_default_init(ROM_MCU_PARAMETER_TYPDEF* param_rom_mcu)
{
    if (param_rom_mcu == NULL) {
        /* 防止空指针访问 */
        return 1;
    }
    /* --MCU */
    param_rom_mcu->cell_ov1_alarm                                           = 4015;//3600;

    /*验证测试数据*/
    param_rom_mcu->cell_iv1_alars                                           = 3500;//3600;

    param_rom_mcu->cell_ov1_alarm_delay_time                                = 1565;//1000;
    param_rom_mcu->cell_ov1_release                                         = 3848;//3600;
    param_rom_mcu->cell_ov1_release_delay_time                              = 6000;

    param_rom_mcu->cell_ov2_alarm                                           = 3850;
    param_rom_mcu->cell_ov2_alarm_delay_time                                = 1000; 
    param_rom_mcu->cell_ov2_release                                         = 3600; 
    param_rom_mcu->cell_ov2_release_delay_time                              = 1000; 
    param_rom_mcu->cell_ov3_alarm                                           = 3670; 
    param_rom_mcu->cell_ov3_alarm_delay_time                                = 1000; 
    param_rom_mcu->cell_ov3_release                                         = 3600; 
    param_rom_mcu->cell_ov3_release_delay_time                              = 1000; 
    param_rom_mcu->cell_ov3_unlock_soc                                      = 85; 
    param_rom_mcu->cell_ov3_unlock_charge_current                           = 1000; 
    param_rom_mcu->cell_uv1_alarm                                           = 2900; 
    param_rom_mcu->cell_uv1_alarm_delay_time                                = 1000; 
    param_rom_mcu->cell_uv1_release                                         = 2900;
    param_rom_mcu->cell_uv1_release_delay_time                              = 6000;
    param_rom_mcu->cell_uv2_alarm                                           = 2800; 
    param_rom_mcu->cell_uv2_alarm_delay_time                                = 1000; 
    param_rom_mcu->cell_uv2_release                                         = 2900; 
    param_rom_mcu->cell_uv2_release_delay_time                              = 1000; 
    param_rom_mcu->cell_uv3_alarm                                           = 2500; 
    param_rom_mcu->cell_uv3_alarm_delay_time                                = 1000; 
    param_rom_mcu->cell_uv3_release                                         = 2900; 
    param_rom_mcu->cell_uv3_release_delay_time                              = 1000; 
    param_rom_mcu->cell_uv3_unlock_charge_current                           = 1000; 
    param_rom_mcu->total_ov1_alarm                                          = 58400; 
    param_rom_mcu->total_ov1_alarm_delay_time                               = 1000; 
    param_rom_mcu->total_ov1_release                                        = 54000; 
    param_rom_mcu->total_ov1_release_delay_time                             = 6000; 
    param_rom_mcu->total_ov2_alarm                                          = 60000; 
    param_rom_mcu->total_ov2_alarm_delay_time                               = 1000; 
    param_rom_mcu->total_ov2_release                                        = 54000; 
    param_rom_mcu->total_ov2_release_delay_time                             = 1000; 
    param_rom_mcu->total_ov3_alarm                                          = 60500; 
    param_rom_mcu->total_ov3_alarm_delay_time                               = 1000;
    param_rom_mcu->total_ov3_release                                        = 54000;
    param_rom_mcu->total_ov3_release_delay_time                             = 1000; 
    param_rom_mcu->total_ov3_unlock_soc                                     = 85;
    param_rom_mcu->total_ov3_unlock_charge_current                          = 1000; 
    param_rom_mcu->total_uv1_alarm                                          = 43200;
    param_rom_mcu->total_uv1_alarm_delay_time                               = 1000; 
    param_rom_mcu->total_uv1_release                                        = 46400; 
    param_rom_mcu->total_uv1_release_delay_time                             = 6000; 
    param_rom_mcu->total_uv2_alarm                                          = 38000; 
    param_rom_mcu->total_uv2_alarm_delay_time                               = 2000;
    param_rom_mcu->total_uv2_release                                        = 46400;
    param_rom_mcu->total_uv2_release_delay_time                             = 2000;
    param_rom_mcu->total_uv3_alarm                                          = 37000;
    param_rom_mcu->total_uv3_alarm_delay_time                               = 1000;
    param_rom_mcu->total_uv3_release                                        = 46400;
    param_rom_mcu->total_uv3_release_delay_time                             = 1000;
    param_rom_mcu->total_uv3_unlock_charge_current                          = 1000;
    param_rom_mcu->cell_diff_ov1_alarm                                      = 300;
    param_rom_mcu->cell_diff_ov1_alarm_delay_time                           = 3000;
    param_rom_mcu->cell_diff_ov1_release                                    = 150;
    param_rom_mcu->cell_diff_ov1_release_delay_time                         = 5000;
    param_rom_mcu->cell_diff_ov2_alarm                                      = 1000;
    param_rom_mcu->cell_diff_ov2_alarm_delay_time                           = 10000;
    param_rom_mcu->cell_diff_ov2_release                                    = 150;
    param_rom_mcu->cell_diff_ov2_release_delay_time                         = 1000;

    param_rom_mcu->charge_oc1_alarm                                         = 55;
    param_rom_mcu->charge_oc1_alarm_delay_time                              = 1000;
    param_rom_mcu->charge_oc2_alarm                                         = 60;
    param_rom_mcu->charge_oc2_alarm_delay_time                              = 500;
    param_rom_mcu->charge_oc3_alarm                                         = 120;
    param_rom_mcu->charge_oc3_alarm_delay_time                              = 500;
    param_rom_mcu->discharge_oc1_alarm                                      = 75;
    param_rom_mcu->discharge_oc1_alarm_delay_time                           = 1000;
    param_rom_mcu->discharge_oc2_alarm                                      = 80;
    param_rom_mcu->discharge_oc2_alarm_delay_time                           = 1000;
    param_rom_mcu->discharge_oc3_alarm                                      = 100;
    param_rom_mcu->discharge_oc3_alarm_delay_time                           = 100;
    param_rom_mcu->short_circuit_alarm                                      = 400;
    param_rom_mcu->short_circuit_alarm_delay_time                           = 128;

    param_rom_mcu->charge_cell_ot1_alarm                                    = 50;
    param_rom_mcu->charge_cell_ot1_alarm_delay_time                         = 2000;
    param_rom_mcu->charge_cell_ot1_release                                  = 50;
    param_rom_mcu->charge_cell_ot1_release_delay_time                       = 5000;
    param_rom_mcu->charge_cell_ot2_alarm                                    = 55;
    param_rom_mcu->charge_cell_ot2_alarm_delay_time                         = 2000;
    param_rom_mcu->charge_cell_ot2_release                                  = 50;
    param_rom_mcu->charge_cell_ot2_release_delay_time                       = 2000;
    param_rom_mcu->charge_cell_ot3_alarm                                    = 57;
    param_rom_mcu->charge_cell_ot3_alarm_delay_time                         = 2000;
    param_rom_mcu->charge_cell_ot3_release                                  = 50;
    param_rom_mcu->charge_cell_ot3_release_delay_time                       = 2000;
    param_rom_mcu->charge_cell_ut1_alarm                                    = 5;
    param_rom_mcu->charge_cell_ut1_alarm_delay_time                         = 2000;
    param_rom_mcu->charge_cell_ut1_release                                  = 5;
    param_rom_mcu->charge_cell_ut1_release_delay_time                       = 5000;
    param_rom_mcu->charge_cell_ut2_alarm                                    = 0;
    param_rom_mcu->charge_cell_ut2_alarm_delay_time                         = 2000;
    param_rom_mcu->charge_cell_ut2_release                                  = 5;
    param_rom_mcu->charge_cell_ut2_release_delay_time                       = 2000;
    param_rom_mcu->charge_cell_ut3_alarm                                    = -5;
    param_rom_mcu->charge_cell_ut3_alarm_delay_time                         = 2000;
    param_rom_mcu->charge_cell_ut3_release                                  = 0;
    param_rom_mcu->charge_cell_ut3_release_delay_time                       = 2000;

    param_rom_mcu->discharge_cell_ot1_alarm                                 = 50;
    param_rom_mcu->discharge_cell_ot1_alarm_delay_time                      = 2000;
    param_rom_mcu->discharge_cell_ot1_release                               = 50;
    param_rom_mcu->discharge_cell_ot1_release_delay_time                    = 5000;
    param_rom_mcu->discharge_cell_ot2_alarm                                 = 55;
    param_rom_mcu->discharge_cell_ot2_alarm_delay_time                      = 2000;
    param_rom_mcu->discharge_cell_ot2_release                               = 50;
    param_rom_mcu->discharge_cell_ot2_release_delay_time                    = 2000;
    param_rom_mcu->discharge_cell_ot3_alarm                                 = 57;
    param_rom_mcu->discharge_cell_ot3_alarm_delay_time                      = 2000;
    param_rom_mcu->discharge_cell_ot3_release                               = 50;
    param_rom_mcu->discharge_cell_ot3_release_delay_time                    = 2000;
    param_rom_mcu->discharge_cell_ut1_alarm                                 = 0;
    param_rom_mcu->discharge_cell_ut1_alarm_delay_time                      = 2000;
    param_rom_mcu->discharge_cell_ut1_release                               = 0;
    param_rom_mcu->discharge_cell_ut1_release_delay_time                    = 5000;
    param_rom_mcu->discharge_cell_ut2_alarm                                 = -20;
    param_rom_mcu->discharge_cell_ut2_alarm_delay_time                      = 2000;
    param_rom_mcu->discharge_cell_ut2_release                               = -10;
    param_rom_mcu->discharge_cell_ut2_release_delay_time                    = 2000;
    param_rom_mcu->discharge_cell_ut3_alarm                                 = -25;
    param_rom_mcu->discharge_cell_ut3_alarm_delay_time                      = 2000;
    param_rom_mcu->discharge_cell_ut3_release                               = -10;
    param_rom_mcu->discharge_cell_ut3_release_delay_time                    = 2000;

    param_rom_mcu->cell_ot1_alarm                                           = 55;
    param_rom_mcu->cell_ot1_alarm_delay_time                                = 2000;
    param_rom_mcu->cell_ot1_release                                         = 55;
    param_rom_mcu->cell_ot1_release_delay_time                              = 5000;
    param_rom_mcu->cell_ot2_alarm                                           = 65;
    param_rom_mcu->cell_ot2_alarm_delay_time                                = 2000;
    param_rom_mcu->cell_ot2_release                                         = 55;
    param_rom_mcu->cell_ot2_release_delay_time                              = 2000;
    param_rom_mcu->cell_diff_temp1_alarm                                    = 8;
    param_rom_mcu->cell_diff_temp1_alarm_delay_time                         = 3000;
    param_rom_mcu->cell_diff_temp1_release                                  = 6;
    param_rom_mcu->cell_diff_temp1_release_delay_time                       = 5000;
    param_rom_mcu->cell_diff_temp2_alarm                                    = 15;
    param_rom_mcu->cell_diff_temp2_alarm_delay_time                         = 1000;
    param_rom_mcu->cell_diff_temp2_release                                  = 8;
    param_rom_mcu->cell_diff_temp2_release_delay_time                       = 1000;
    param_rom_mcu->env_ot_high1_alarm                                       = 60;
    param_rom_mcu->env_ot_high1_alarm_delay_time                            = 2000;
    param_rom_mcu->env_ot_high1_release                                     = 55;
    param_rom_mcu->env_ot_high1_release_delay_time                          = 5000;
    param_rom_mcu->env_ot_high2_alarm                                       = 65;
    param_rom_mcu->env_ot_high2_alarm_delay_time                            = 2000;
    param_rom_mcu->env_ot_high2_release                                     = 55;
    param_rom_mcu->env_ot_high2_release_delay_time                          = 2000;
    param_rom_mcu->env_ut_low1_alarm                                        = -15;
    param_rom_mcu->env_ut_low1_alarm_delay_time                             = 2000;
    param_rom_mcu->env_ut_low1_release                                      = -15;
    param_rom_mcu->env_ut_low1_release_delay_time                           = 5000;
    param_rom_mcu->env_ut_low2_alarm                                        = -20;
    param_rom_mcu->env_ut_low2_alarm_delay_time                             = 2000;
    param_rom_mcu->env_ut_low2_release                                      = -15;
    param_rom_mcu->env_ut_low2_release_delay_time                           = 2000;

    param_rom_mcu->cell_balance_start_voltage                               = 3400;
    param_rom_mcu->cell_balance_start_diff                                  = 30;
    param_rom_mcu->cell_sleep_voltage                                       = 3000;
    param_rom_mcu->cell_sleep_duration                                      = 15;
    param_rom_mcu->cell_full_charge_voltage                                 = 3550;
    param_rom_mcu->full_charge_current                                      = 1000;

    param_rom_mcu->battery_capacity_initial                                 = 100;   
    param_rom_mcu->battery_capacity_current                                 = 100;   
    param_rom_mcu->cycle_count                                              = 0;   
    param_rom_mcu->total_runtime                                            = 0;   

    param_rom_mcu->soc_initial                                              = 50;   
    param_rom_mcu->soc_low_alarm                                            = 5;   
    param_rom_mcu->soh_initial                                              = 100;   
    param_rom_mcu->soh_alarm                                                = 80;   
    param_rom_mcu->sod_deep_discharge                                       = 80;   

    param_rom_mcu->heater_off_temp                                          = 56;//25;  
    param_rom_mcu->device_sn_high                                           = 0;    
    param_rom_mcu->device_sn_low                                            = 0;    
    param_rom_mcu->device_id                                                = 0;    
    param_rom_mcu->buzzer_duration                                          = 1000;   
    
    param_rom_mcu->power_on_activation                                      = 1; 
    param_rom_mcu->sleep_on                                                 = 0; 
    param_rom_mcu->shutdown_cmd                                             = 0;   

    param_rom_mcu->heater_on_cmd                                            = 1;    
    param_rom_mcu->heater_off_cmd                                           = 0;    
    param_rom_mcu->heater_auto_off_temp                                     = 250;    

    param_rom_mcu->indicator_on_cmd                                         = 1;    
    param_rom_mcu->indicator_off_cmd                                        = 0;    
    param_rom_mcu->alarm_light_on_cmd                                       = 1;   
    param_rom_mcu->alarm_light_off_cmd                                      = 0;   

    param_rom_mcu->buzzer_enable_cmd                                        = 1;
    param_rom_mcu->buzzer_disable_cmd                                       = 0;
    param_rom_mcu->buzzer_status                                            = 1;
    param_rom_mcu->buzzer_stop_cmd                                          = 0;

    param_rom_mcu->current_calib_prepare                                    = 0;
    param_rom_mcu->current_calib_set_value                                  = 1;
    param_rom_mcu->current_calib_start                                      = 1;
    param_rom_mcu->current_calib_verify                                     = 1;

    param_rom_mcu->short_circuit_release_on                                 = 1;
    param_rom_mcu->short_circuit_release_off                                = 0;

    param_rom_mcu->pre_discharge_on                                         = 1;
    param_rom_mcu->pre_discharge_off                                        = 0;
    param_rom_mcu->discharge_mos_on                                         = 1; 
    param_rom_mcu->discharge_mos_off                                        = 0;
    param_rom_mcu->charge_mos_on                                            = 1; 
    param_rom_mcu->charge_mos_off                                           = 0; 
    param_rom_mcu->fuse_blow_cmd                                            = 1;     
    param_rom_mcu->fuse_reset_cmd                                           = 0;

    param_rom_mcu->afe_eeprom_save_cmd                                      = 1;     
    param_rom_mcu->mcu_protection_save_cmd                                  = 1; 

    param_rom_mcu->charge_discharge_ctrl                                    = 10;//0;

    param_rom_mcu->mcu_init_flag                                            = 0;                 

    return 0;
}

/**
 * @函数名称: parameter_rom_write()
 * @功能描述: 写关于MCU保护的eeprom 存储参数
 * @输入: ROM_MCU_PARAMETER_TYPDEF：MCU保护结构体参数
 * @输出: 无
 * @返回值: 0成功 1失败
 */
uint8_t parameter_rom_write(ROM_MCU_PARAMETER_TYPDEF* param_rom_mcu)
{
    uint8_t err = 0;
    uint16_t cell_ov1_alarm = 100;
    if (param_rom_mcu == NULL) {
        /* 防止空指针访问 */
        return 1;
    }
    
    printf("parameter = %d\r\n", param_rom_mcu->cell_ov1_alarm);
    err |= write_flash_parameter("cell_ov1_alarm",                              &param_rom_mcu->cell_ov1_alarm);
    read_flash_parameter("cell_ov1_alarm",                                      &cell_ov1_alarm);
    printf("cell_ov1_alarm = %d\r\n", cell_ov1_alarm);   
    err |= write_flash_parameter("cell_iv1_alars",                              &param_rom_mcu->cell_iv1_alars);
    // err |= write_flash_parameter("cell_ov1_alarm_delay_time",                   &param_rom_mcu->cell_ov1_alarm_delay_time);

    // err |= write_flash_parameter("cell_ov1_release",                            &param_rom_mcu->cell_ov1_release);
    // err |= write_flash_parameter("cell_ov1_release_delay_time",                 &param_rom_mcu->cell_ov1_release_delay_time);

    // err |= write_flash_parameter("cell_ov2_alarm",                              &param_rom_mcu->cell_ov2_alarm);
    // err |= write_flash_parameter("cell_ov2_alarm_delay_time",                   &param_rom_mcu->cell_ov2_alarm_delay_time);
    // err |= write_flash_parameter("cell_ov2_release",                            &param_rom_mcu->cell_ov2_release);
    // err |= write_flash_parameter("cell_ov2_release_delay_time",                 &param_rom_mcu->cell_ov2_release_delay_time);    
    
    // err |= write_flash_parameter("cell_ov3_alarm",                              &param_rom_mcu->cell_ov3_alarm);
    // err |= write_flash_parameter("cell_ov3_alarm_delay_time",                   &param_rom_mcu->cell_ov3_alarm_delay_time);
    // err |= write_flash_parameter("cell_ov3_release",                            &param_rom_mcu->cell_ov3_release);
    // err |= write_flash_parameter("cell_ov3_release_delay_time",                 &param_rom_mcu->cell_ov3_release_delay_time);    
    // err |= write_flash_parameter("cell_ov3_unlock_soc",                         &param_rom_mcu->cell_ov3_unlock_soc);
    // err |= write_flash_parameter("cell_ov3_unlock_charge_current",              &param_rom_mcu->cell_ov3_unlock_charge_current);


    // err |= write_flash_parameter("cell_uv1_alarm",                              &param_rom_mcu->cell_uv1_alarm);
    // err |= write_flash_parameter("cell_uv1_alarm_delay_time",                   &param_rom_mcu->cell_uv1_alarm_delay_time);
    // err |= write_flash_parameter("cell_uv1_release",                            &param_rom_mcu->cell_uv1_release);
    // err |= write_flash_parameter("cell_uv1_release_delay_time",                 &param_rom_mcu->cell_uv1_release_delay_time);

    // err |= write_flash_parameter("cell_uv2_alarm",                              &param_rom_mcu->cell_uv2_alarm);
    // err |= write_flash_parameter("cell_uv2_alarm_delay_time",                   &param_rom_mcu->cell_uv2_alarm_delay_time);
    // err |= write_flash_parameter("cell_uv2_release",                            &param_rom_mcu->cell_uv2_release);
    // err |= write_flash_parameter("cell_uv2_release_delay_time",                 &param_rom_mcu->cell_uv2_release_delay_time);    
    
    // err |= write_flash_parameter("cell_uv3_alarm",                              &param_rom_mcu->cell_uv3_alarm);
    // err |= write_flash_parameter("cell_uv3_alarm_delay_time",                   &param_rom_mcu->cell_uv3_alarm_delay_time);
    // err |= write_flash_parameter("cell_uv3_release",                            &param_rom_mcu->cell_uv3_release);
    // err |= write_flash_parameter("cell_uv3_release_delay_time",                 &param_rom_mcu->cell_uv3_release_delay_time);    
    // err |= write_flash_parameter("cell_uv3_unlock_charge_current",              &param_rom_mcu->cell_uv3_unlock_charge_current);


    // err |= write_flash_parameter("total_ov1_alarm",                             &param_rom_mcu->total_ov1_alarm);
    // err |= write_flash_parameter("total_ov1_alarm_delay_time",                  &param_rom_mcu->total_ov1_alarm_delay_time);
    // err |= write_flash_parameter("total_ov1_release",                           &param_rom_mcu->total_ov1_release);
    // err |= write_flash_parameter("total_ov1_release_delay_time",                &param_rom_mcu->total_ov1_release_delay_time);

    // err |= write_flash_parameter("total_ov2_alarm",                             &param_rom_mcu->total_ov2_alarm);
    // err |= write_flash_parameter("total_ov2_alarm_delay_time",                  &param_rom_mcu->total_ov2_alarm_delay_time);
    // err |= write_flash_parameter("total_ov2_release",                           &param_rom_mcu->total_ov2_release);
    // err |= write_flash_parameter("total_ov2_release_delay_time",                &param_rom_mcu->total_ov2_release_delay_time);    
    
    // err |= write_flash_parameter("total_ov3_alarm",                             &param_rom_mcu->total_ov3_alarm);
    // err |= write_flash_parameter("total_ov3_alarm_delay_time",                  &param_rom_mcu->total_ov3_alarm_delay_time);
    // err |= write_flash_parameter("total_ov3_release",                           &param_rom_mcu->total_ov3_release);
    // err |= write_flash_parameter("total_ov3_release_delay_time",                &param_rom_mcu->total_ov3_release_delay_time);    
    // err |= write_flash_parameter("total_ov3_unlock_soc",                        &param_rom_mcu->total_ov3_unlock_soc);
    // err |= write_flash_parameter("total_ov3_unlock_charge_current",             &param_rom_mcu->total_ov3_unlock_charge_current);

    // err |= write_flash_parameter("total_uv1_alarm",                             &param_rom_mcu->total_uv1_alarm);
    // err |= write_flash_parameter("total_uv1_alarm_delay_time",                  &param_rom_mcu->total_uv1_alarm_delay_time);
    // err |= write_flash_parameter("total_uv1_release",                           &param_rom_mcu->total_uv1_release);
    // err |= write_flash_parameter("total_uv1_release_delay_time",                &param_rom_mcu->total_uv1_release_delay_time);

    // err |= write_flash_parameter("total_uv2_alarm",                             &param_rom_mcu->total_uv2_alarm);
    // err |= write_flash_parameter("total_uv2_alarm_delay_time",                  &param_rom_mcu->total_uv2_alarm_delay_time);
    // err |= write_flash_parameter("total_uv2_release",                           &param_rom_mcu->total_uv2_release);
    // err |= write_flash_parameter("total_uv2_release_delay_time",                &param_rom_mcu->total_uv2_release_delay_time);    
    
    // err |= write_flash_parameter("total_uv3_alarm",                             &param_rom_mcu->total_uv3_alarm);
    // err |= write_flash_parameter("total_uv3_alarm_delay_time",                  &param_rom_mcu->total_uv3_alarm_delay_time);
    // err |= write_flash_parameter("total_uv3_release",                           &param_rom_mcu->total_uv3_release);
    // err |= write_flash_parameter("total_uv3_release_delay_time",                &param_rom_mcu->total_uv3_release_delay_time);    
    // err |= write_flash_parameter("total_uv3_unlock_charge_current",             &param_rom_mcu->total_uv3_unlock_charge_current);


    // err |= write_flash_parameter("cell_diff_ov1_alarm",                         &param_rom_mcu->cell_diff_ov1_alarm);
    // err |= write_flash_parameter("cell_diff_ov1_alarm_delay_time",              &param_rom_mcu->cell_diff_ov1_alarm_delay_time);
    // err |= write_flash_parameter("cell_diff_ov1_release",                       &param_rom_mcu->cell_diff_ov1_release);
    // err |= write_flash_parameter("cell_diff_ov1_release_delay_time",            &param_rom_mcu->cell_diff_ov1_release_delay_time);

    // err |= write_flash_parameter("cell_diff_ov2_alarm",                         &param_rom_mcu->cell_diff_ov2_alarm);
    // err |= write_flash_parameter("cell_diff_ov2_alarm_delay_time",              &param_rom_mcu->cell_diff_ov2_alarm_delay_time);
    // err |= write_flash_parameter("cell_diff_ov2_release",                       &param_rom_mcu->cell_diff_ov2_release);
    // err |= write_flash_parameter("cell_diff_ov2_release_delay_time",            &param_rom_mcu->cell_diff_ov2_release_delay_time);  

    // /* ====write_flash_parameter= 电流相关参数 ============================= */
    // err |= write_flash_parameter("charge_oc1_alarm",                            &param_rom_mcu->charge_oc1_alarm);
    // err |= write_flash_parameter("charge_oc1_alarm_delay_time",                 &param_rom_mcu->charge_oc1_alarm_delay_time);
    // err |= write_flash_parameter("charge_oc2_alarm",                            &param_rom_mcu->charge_oc2_alarm);
    // err |= write_flash_parameter("charge_oc2_alarm_delay_time",                 &param_rom_mcu->charge_oc2_alarm_delay_time);    
    // err |= write_flash_parameter("charge_oc3_alarm",                            &param_rom_mcu->charge_oc3_alarm);
    // err |= write_flash_parameter("charge_oc3_alarm_delay_time",                 &param_rom_mcu->charge_oc3_alarm_delay_time);

    // err |= write_flash_parameter("discharge_oc1_alarm",                         &param_rom_mcu->discharge_oc1_alarm);
    // err |= write_flash_parameter("discharge_oc1_alarm_delay_time",              &param_rom_mcu->discharge_oc1_alarm_delay_time);
    // err |= write_flash_parameter("discharge_oc2_alarm",                         &param_rom_mcu->discharge_oc2_alarm);
    // err |= write_flash_parameter("discharge_oc2_alarm_delay_time",              &param_rom_mcu->discharge_oc2_alarm_delay_time);    
    // err |= write_flash_parameter("discharge_oc3_alarm",                         &param_rom_mcu->discharge_oc3_alarm);
    // err |= write_flash_parameter("discharge_oc3_alarm_delay_time",              &param_rom_mcu->discharge_oc3_alarm_delay_time);
    
    // err |= write_flash_parameter("short_circuit_alarm",                         &param_rom_mcu->short_circuit_alarm);
    // err |= write_flash_parameter("short_circuit_alarm_delay_time",              &param_rom_mcu->short_circuit_alarm_delay_time);

    // /* ====write_flash_parameter= 温度相关参数 ============================= */
    // err |= write_flash_parameter("charge_cell_ot1_alarm",                       &param_rom_mcu->charge_cell_ot1_alarm);
    // err |= write_flash_parameter("charge_cell_ot1_alarm_delay_time",            &param_rom_mcu->charge_cell_ot1_alarm_delay_time);
    // err |= write_flash_parameter("charge_cell_ot1_release",                     &param_rom_mcu->charge_cell_ot1_release);
    // err |= write_flash_parameter("charge_cell_ot1_release_delay_time",          &param_rom_mcu->charge_cell_ot1_release_delay_time);

    // err |= write_flash_parameter("charge_cell_ot2_alarm",                       &param_rom_mcu->charge_cell_ot2_alarm);
    // err |= write_flash_parameter("charge_cell_ot2_alarm_delay_time",            &param_rom_mcu->charge_cell_ot2_alarm_delay_time);
    // err |= write_flash_parameter("charge_cell_ot2_release",                     &param_rom_mcu->charge_cell_ot2_release);
    // err |= write_flash_parameter("charge_cell_ot2_release_delay_time",          &param_rom_mcu->charge_cell_ot2_release_delay_time);    

    // err |= write_flash_parameter("charge_cell_ot3_alarm",                       &param_rom_mcu->charge_cell_ot3_alarm);
    // err |= write_flash_parameter("charge_cell_ot3_alarm_delay_time",            &param_rom_mcu->charge_cell_ot3_alarm_delay_time);
    // err |= write_flash_parameter("charge_cell_ot3_release",                     &param_rom_mcu->charge_cell_ot3_release);
    // err |= write_flash_parameter("charge_cell_ot3_release_delay_time",          &param_rom_mcu->charge_cell_ot3_release_delay_time);  

    // err |= write_flash_parameter("charge_cell_ut1_alarm",                       &param_rom_mcu->charge_cell_ut1_alarm);
    // err |= write_flash_parameter("charge_cell_ut1_alarm_delay_time",            &param_rom_mcu->charge_cell_ut1_alarm_delay_time);
    // err |= write_flash_parameter("charge_cell_ut1_release",                     &param_rom_mcu->charge_cell_ut1_release);
    // err |= write_flash_parameter("charge_cell_ut1_release_delay_time",          &param_rom_mcu->charge_cell_ut1_release_delay_time);

    // err |= write_flash_parameter("charge_cell_ut2_alarm",                       &param_rom_mcu->charge_cell_ut2_alarm);
    // err |= write_flash_parameter("charge_cell_ut2_alarm_delay_time",            &param_rom_mcu->charge_cell_ut2_alarm_delay_time);
    // err |= write_flash_parameter("charge_cell_ut2_release",                     &param_rom_mcu->charge_cell_ut2_release);
    // err |= write_flash_parameter("charge_cell_ut2_release_delay_time",          &param_rom_mcu->charge_cell_ut2_release_delay_time);    

    // err |= write_flash_parameter("charge_cell_ut3_alarm",                       &param_rom_mcu->charge_cell_ut3_alarm);
    // err |= write_flash_parameter("charge_cell_ut3_alarm_delay_time",            &param_rom_mcu->charge_cell_ut3_alarm_delay_time);
    // err |= write_flash_parameter("charge_cell_ut3_release",                     &param_rom_mcu->charge_cell_ut3_release);
    // err |= write_flash_parameter("charge_cell_ut3_release_delay_time",          &param_rom_mcu->charge_cell_ut3_release_delay_time);     


    // err |= write_flash_parameter("discharge_cell_ot1_alarm",                    &param_rom_mcu->discharge_cell_ot1_alarm);
    // err |= write_flash_parameter("discharge_cell_ot1_alarm_delay_time",         &param_rom_mcu->discharge_cell_ot1_alarm_delay_time);
    // err |= write_flash_parameter("discharge_cell_ot1_release",                  &param_rom_mcu->discharge_cell_ot1_release);
    // err |= write_flash_parameter("discharge_cell_ot1_release_delay_time",       &param_rom_mcu->discharge_cell_ot1_release_delay_time);

    // err |= write_flash_parameter("discharge_cell_ot2_alarm",                    &param_rom_mcu->discharge_cell_ot2_alarm);
    // err |= write_flash_parameter("discharge_cell_ot2_alarm_delay_time",         &param_rom_mcu->discharge_cell_ot2_alarm_delay_time);
    // err |= write_flash_parameter("discharge_cell_ot2_release",                  &param_rom_mcu->discharge_cell_ot2_release);
    // err |= write_flash_parameter("discharge_cell_ot2_release_delay_time",       &param_rom_mcu->discharge_cell_ot2_release_delay_time);    
   
    // err |= write_flash_parameter("discharge_cell_ot3_alarm",                    &param_rom_mcu->discharge_cell_ot3_alarm);
    // err |= write_flash_parameter("discharge_cell_ot3_alarm_delay_time",         &param_rom_mcu->discharge_cell_ot3_alarm_delay_time);
    // err |= write_flash_parameter("discharge_cell_ot3_release",                  &param_rom_mcu->discharge_cell_ot3_release);
    // err |= write_flash_parameter("discharge_cell_ot3_release_delay_time",       &param_rom_mcu->discharge_cell_ot3_release_delay_time);  
    
    // err |= write_flash_parameter("discharge_cell_ut1_alarm",                    &param_rom_mcu->discharge_cell_ut1_alarm);
    // err |= write_flash_parameter("discharge_cell_ut1_alarm_delay_time",         &param_rom_mcu->discharge_cell_ut1_alarm_delay_time);
    // err |= write_flash_parameter("discharge_cell_ut1_release",                  &param_rom_mcu->discharge_cell_ut1_release);
    // err |= write_flash_parameter("discharge_cell_ut1_release_delay_time",       &param_rom_mcu->discharge_cell_ut1_release_delay_time);

    // err |= write_flash_parameter("discharge_cell_ut2_alarm",                    &param_rom_mcu->discharge_cell_ut2_alarm);
    // err |= write_flash_parameter("discharge_cell_ut2_alarm_delay_time",         &param_rom_mcu->discharge_cell_ut2_alarm_delay_time);
    // err |= write_flash_parameter("discharge_cell_ut2_release",                  &param_rom_mcu->discharge_cell_ut2_release);
    // err |= write_flash_parameter("discharge_cell_ut2_release_delay_time",       &param_rom_mcu->discharge_cell_ut2_release_delay_time);    
 
    // err |= write_flash_parameter("discharge_cell_ut3_alarm",                    &param_rom_mcu->discharge_cell_ut3_alarm);
    // err |= write_flash_parameter("discharge_cell_ut3_alarm_delay_time",         &param_rom_mcu->discharge_cell_ut3_alarm_delay_time);
    // err |= write_flash_parameter("discharge_cell_ut3_release",                  &param_rom_mcu->discharge_cell_ut3_release);
    // err |= write_flash_parameter("discharge_cell_ut3_release_delay_time",       &param_rom_mcu->discharge_cell_ut3_release_delay_time);

    
    // err |= write_flash_parameter("cell_ot1_alarm",                              &param_rom_mcu->cell_ot1_alarm);
    // err |= write_flash_parameter("cell_ot1_alarm_delay_time",                   &param_rom_mcu->cell_ot1_alarm_delay_time);
    // err |= write_flash_parameter("cell_ot1_release",                            &param_rom_mcu->cell_ot1_release);
    // err |= write_flash_parameter("cell_ot1_release_delay_time",                 &param_rom_mcu->cell_ot1_release_delay_time);

    // err |= write_flash_parameter("cell_ot2_alarm",                              &param_rom_mcu->cell_ot2_alarm);
    // err |= write_flash_parameter("cell_ot2_alarm_delay_time",                   &param_rom_mcu->cell_ot2_alarm_delay_time);
    // err |= write_flash_parameter("cell_ot2_release",                            &param_rom_mcu->cell_ot2_release);
    // err |= write_flash_parameter("cell_ot2_release_delay_time",                 &param_rom_mcu->cell_ot2_release_delay_time);  

    // err |= write_flash_parameter("cell_diff_temp1_alarm",                       &param_rom_mcu->cell_diff_temp1_alarm);
    // err |= write_flash_parameter("cell_diff_temp1_alarm_delay_time",            &param_rom_mcu->cell_diff_temp1_alarm_delay_time);
    // err |= write_flash_parameter("cell_diff_temp1_release",                     &param_rom_mcu->cell_diff_temp1_release);
    // err |= write_flash_parameter("cell_diff_temp1_release_delay_time",          &param_rom_mcu->cell_diff_temp1_release_delay_time);
    
    // err |= write_flash_parameter("cell_diff_temp2_alarm",                       &param_rom_mcu->cell_diff_temp2_alarm);
    // err |= write_flash_parameter("cell_diff_temp2_alarm_delay_time",            &param_rom_mcu->cell_diff_temp2_alarm_delay_time);
    // err |= write_flash_parameter("cell_diff_temp2_release",                     &param_rom_mcu->cell_diff_temp2_release);
    // err |= write_flash_parameter("cell_diff_temp2_release_delay_time",          &param_rom_mcu->cell_diff_temp2_release_delay_time);  


    // err |= write_flash_parameter("env_ot_high1_alarm",                          &param_rom_mcu->env_ot_high1_alarm);
    // err |= write_flash_parameter("env_ot_high1_alarm_delay_time",               &param_rom_mcu->env_ot_high1_alarm_delay_time);
    // err |= write_flash_parameter("env_ot_high1_release",                        &param_rom_mcu->env_ot_high1_release);
    // err |= write_flash_parameter("env_ot_high1_release_delay_time",             &param_rom_mcu->env_ot_high1_release_delay_time);
    
    // err |= write_flash_parameter("env_ot_high2_alarm",                          &param_rom_mcu->env_ot_high2_alarm);
    // err |= write_flash_parameter("env_ot_high2_alarm_delay_time",               &param_rom_mcu->env_ot_high2_alarm_delay_time);
    // err |= write_flash_parameter("env_ot_high2_release",                        &param_rom_mcu->env_ot_high2_release);
    // err |= write_flash_parameter("env_ot_high2_release_delay_time",             &param_rom_mcu->env_ot_high2_release_delay_time);  

    // err |= write_flash_parameter("env_ut_low1_alarm",                           &param_rom_mcu->env_ut_low1_alarm);
    // err |= write_flash_parameter("env_ut_low1_alarm_delay_time",                &param_rom_mcu->env_ut_low1_alarm_delay_time);
    // err |= write_flash_parameter("env_ut_low1_release",                         &param_rom_mcu->env_ut_low1_release);
    // err |= write_flash_parameter("env_ut_low1_release_delay_time",              &param_rom_mcu->env_ut_low1_release_delay_time);

    // err |= write_flash_parameter("env_ut_low2_alarm",                           &param_rom_mcu->env_ut_low2_alarm);
    // err |= write_flash_parameter("env_ut_low2_alarm_delay_time",                &param_rom_mcu->env_ut_low2_alarm_delay_time);
    // err |= write_flash_parameter("env_ut_low2_release",                         &param_rom_mcu->env_ut_low2_release);
    // err |= write_flash_parameter("env_ut_low2_release_delay_time",              &param_rom_mcu->env_ut_low2_release_delay_time);  


    // err |= write_flash_parameter("cell_balance_start_voltage",                  &param_rom_mcu->cell_balance_start_voltage);
    // err |= write_flash_parameter("cell_balance_start_diff",                     &param_rom_mcu->cell_balance_start_diff);
    // err |= write_flash_parameter("cell_sleep_voltage",                          &param_rom_mcu->cell_sleep_voltage);
    // err |= write_flash_parameter("cell_sleep_duration",                         &param_rom_mcu->cell_sleep_duration);  
    // err |= write_flash_parameter("cell_full_charge_voltage",                    &param_rom_mcu->cell_full_charge_voltage);
    // err |= write_flash_parameter("full_charge_current",                         &param_rom_mcu->full_charge_current);  

    // err |= write_flash_parameter("battery_capacity_initial",                    &param_rom_mcu->battery_capacity_initial);
    // err |= write_flash_parameter("battery_capacity_current",                    &param_rom_mcu->battery_capacity_current);  
    // err |= write_flash_parameter("cycle_count",                                 &param_rom_mcu->cycle_count);
    // err |= write_flash_parameter("total_runtime",                               &param_rom_mcu->total_runtime);  

    // err |= write_flash_parameter("soc_initial",                                 &param_rom_mcu->soc_initial);
    // err |= write_flash_parameter("soc_low_alarm",                               &param_rom_mcu->soc_low_alarm);
    // err |= write_flash_parameter("soh_initial",                                 &param_rom_mcu->soh_initial);  
    // err |= write_flash_parameter("soh_alarm",                                   &param_rom_mcu->soh_alarm);
    // err |= write_flash_parameter("sod_deep_discharge",                          &param_rom_mcu->sod_deep_discharge); 

    // err |= write_flash_parameter("heater_off_temp",                             &param_rom_mcu->heater_off_temp);
    // err |= write_flash_parameter("device_sn_high",                              &param_rom_mcu->device_sn_high);
    // err |= write_flash_parameter("device_sn_low",                               &param_rom_mcu->device_sn_low);
    // err |= write_flash_parameter("device_id",                                   &param_rom_mcu->device_id);
    // err |= write_flash_parameter("buzzer_duration",                             &param_rom_mcu->buzzer_duration);
    // err |= write_flash_parameter("power_on_activation",                         &param_rom_mcu->power_on_activation);
    // err |= write_flash_parameter("shutdown_cmd",                                &param_rom_mcu->shutdown_cmd);
    
    // err |= write_flash_parameter("heater_on_cmd",                               &param_rom_mcu->heater_on_cmd);
    // err |= write_flash_parameter("heater_off_cmd",                              &param_rom_mcu->heater_off_cmd);
    // err |= write_flash_parameter("heater_auto_off_temp",                        &param_rom_mcu->heater_auto_off_temp);

    // err |= write_flash_parameter("indicator_on_cmd",                            &param_rom_mcu->indicator_on_cmd);
    // err |= write_flash_parameter("indicator_off_cmd",                           &param_rom_mcu->indicator_off_cmd);

    // err |= write_flash_parameter("alarm_light_on_cmd",                          &param_rom_mcu->alarm_light_on_cmd);
    // err |= write_flash_parameter("alarm_light_off_cmd",                         &param_rom_mcu->alarm_light_off_cmd);

    // err |= write_flash_parameter("buzzer_enable_cmd",                           &param_rom_mcu->buzzer_enable_cmd);
    // err |= write_flash_parameter("buzzer_disable_cmd",                          &param_rom_mcu->buzzer_disable_cmd);
    // err |= write_flash_parameter("buzzer_status",                               &param_rom_mcu->buzzer_status);
    // err |= write_flash_parameter("buzzer_stop_cmd",                             &param_rom_mcu->buzzer_stop_cmd);

    // err |= write_flash_parameter("current_calib_prepare",                       &param_rom_mcu->current_calib_prepare);
    // err |= write_flash_parameter("current_calib_set_value",                     &param_rom_mcu->current_calib_set_value);
    // err |= write_flash_parameter("current_calib_start",                         &param_rom_mcu->current_calib_start);
    // err |= write_flash_parameter("current_calib_verify",                        &param_rom_mcu->current_calib_verify);

    // err |= write_flash_parameter("short_circuit_release_on",                    &param_rom_mcu->short_circuit_release_on);
    // err |= write_flash_parameter("short_circuit_release_off",                   &param_rom_mcu->short_circuit_release_off);

    // err |= write_flash_parameter("pre_discharge_on",                            &param_rom_mcu->pre_discharge_on);
    // err |= write_flash_parameter("pre_discharge_off",                           &param_rom_mcu->pre_discharge_off);

    // err |= write_flash_parameter("discharge_mos_on",                            &param_rom_mcu->discharge_mos_on);
    // err |= write_flash_parameter("discharge_mos_off",                           &param_rom_mcu->discharge_mos_off);

    // err |= write_flash_parameter("charge_mos_on",                               &param_rom_mcu->charge_mos_on);
    // err |= write_flash_parameter("charge_mos_off",                              &param_rom_mcu->charge_mos_off);

    // err |= write_flash_parameter("fuse_blow_cmd",                               &param_rom_mcu->fuse_blow_cmd);
    // err |= write_flash_parameter("fuse_reset_cmd",                              &param_rom_mcu->fuse_reset_cmd);

    // err |= write_flash_parameter("afe_eeprom_save_cmd",                         &param_rom_mcu->afe_eeprom_save_cmd);
    // err |= write_flash_parameter("mcu_protection_save_cmd",                     &param_rom_mcu->mcu_protection_save_cmd);

    // err |= write_flash_parameter("charge_discharge_ctrl",                       &param_rom_mcu->charge_discharge_ctrl);

    // err |= write_flash_parameter("mcu_init_flag",                               &param_rom_mcu->mcu_init_flag);
    /* --其他 */
    if (err) {
        return 1;
    }
    return 0;
}

/**
 * @函数名称: parameter_rom_read()
 * @功能描述: 写关于MCU保护的eeprom 存储参数
 * @输入: ROM_MCU_PARAMETER_TYPDEF：MCU保护结构体参数
 * @输出: 无
 * @返回值: 0成功 1失败
 */
uint8_t parameter_rom_read(ROM_MCU_PARAMETER_TYPDEF* param_rom_mcu)
{
    uint8_t err = 0;

    if (param_rom_mcu == NULL) {
        /* 防止空指针访问 */
        return 1;
    }
        /* 初始化结构体中的所有字段为 0 */
    memset(param_rom_mcu, 0, sizeof(ROM_MCU_PARAMETER_TYPDEF));

    err |= read_flash_parameter("cell_ov1_alarm",                              &param_rom_mcu->cell_ov1_alarm);
    printf("11111err = %d rom_parameter = %d\r\n",err, param_rom_mcu->cell_ov1_alarm);
    
    err |= read_flash_parameter("cell_iv1_alars",                              &param_rom_mcu->cell_iv1_alars);
    printf("11111err = %d rom_parameter = %d\r\n",err, param_rom_mcu->cell_iv1_alars);
    // err |= read_flash_parameter("cell_ov1_alarm_delay_time",                   &param_rom_mcu->cell_ov1_alarm_delay_time);
    // err |= read_flash_parameter("cell_ov1_release",                            &param_rom_mcu->cell_ov1_release);
    // printf("22222err = %d rom_parameter = %d\r\n",err, param_rom_mcu->cell_ov1_alarm_delay_time);
    // printf("33333err = %d rom_parameter = %d\r\n",err, param_rom_mcu->cell_ov1_release);
    // err |= read_flash_parameter("cell_ov1_release_delay_time",                 &param_rom_mcu->cell_ov1_release_delay_time);

    // err |= read_flash_parameter("cell_ov2_alarm",                              &param_rom_mcu->cell_ov2_alarm);
    // printf("888888err = %d rom_parameter = %d\r\n",err, param_rom_mcu->cell_ov2_alarm);
    // err |= read_flash_parameter("cell_ov2_alarm_delay_time",                   &param_rom_mcu->cell_ov2_alarm_delay_time);
    // err |= read_flash_parameter("cell_ov2_release",                            &param_rom_mcu->cell_ov2_release);
    // err |= read_flash_parameter("cell_ov2_release_delay_time",                 &param_rom_mcu->cell_ov2_release_delay_time);    
    
    // err |= read_flash_parameter("cell_ov3_alarm",                              &param_rom_mcu->cell_ov3_alarm);
    // err |= read_flash_parameter("cell_ov3_alarm_delay_time",                   &param_rom_mcu->cell_ov3_alarm_delay_time);
    // err |= read_flash_parameter("cell_ov3_release",                            &param_rom_mcu->cell_ov3_release);
    // err |= read_flash_parameter("cell_ov3_release_delay_time",                 &param_rom_mcu->cell_ov3_release_delay_time);    
    // err |= read_flash_parameter("cell_ov3_unlock_soc",                         &param_rom_mcu->cell_ov3_unlock_soc);
    // err |= read_flash_parameter("cell_ov3_unlock_charge_current",              &param_rom_mcu->cell_ov3_unlock_charge_current);


    // err |= read_flash_parameter("cell_uv1_alarm",                              &param_rom_mcu->cell_uv1_alarm);
    // err |= read_flash_parameter("cell_uv1_alarm_delay_time",                   &param_rom_mcu->cell_uv1_alarm_delay_time);
    // err |= read_flash_parameter("cell_uv1_release",                            &param_rom_mcu->cell_uv1_release);
    // err |= read_flash_parameter("cell_uv1_release_delay_time",                 &param_rom_mcu->cell_uv1_release_delay_time);

    // err |= read_flash_parameter("cell_uv2_alarm",                              &param_rom_mcu->cell_uv2_alarm);
    // err |= read_flash_parameter("cell_uv2_alarm_delay_time",                   &param_rom_mcu->cell_uv2_alarm_delay_time);
    // err |= read_flash_parameter("cell_uv2_release",                            &param_rom_mcu->cell_uv2_release);
    // err |= read_flash_parameter("cell_uv2_release_delay_time",                 &param_rom_mcu->cell_uv2_release_delay_time);    
    
    // err |= read_flash_parameter("cell_uv3_alarm",                              &param_rom_mcu->cell_uv3_alarm);
    // err |= read_flash_parameter("cell_uv3_alarm_delay_time",                   &param_rom_mcu->cell_uv3_alarm_delay_time);
    // err |= read_flash_parameter("cell_uv3_release",                            &param_rom_mcu->cell_uv3_release);
    // err |= read_flash_parameter("cell_uv3_release_delay_time",                 &param_rom_mcu->cell_uv3_release_delay_time);    
    // err |= read_flash_parameter("cell_uv3_unlock_charge_current",              &param_rom_mcu->cell_uv3_unlock_charge_current);


    // err |= read_flash_parameter("total_ov1_alarm",                             &param_rom_mcu->total_ov1_alarm);
    // err |= read_flash_parameter("total_ov1_alarm_delay_time",                  &param_rom_mcu->total_ov1_alarm_delay_time);
    // err |= read_flash_parameter("total_ov1_release",                           &param_rom_mcu->total_ov1_release);
    // err |= read_flash_parameter("total_ov1_release_delay_time",                &param_rom_mcu->total_ov1_release_delay_time);

    // err |= read_flash_parameter("total_ov2_alarm",                             &param_rom_mcu->total_ov2_alarm);
    // err |= read_flash_parameter("total_ov2_alarm_delay_time",                  &param_rom_mcu->total_ov2_alarm_delay_time);
    // err |= read_flash_parameter("total_ov2_release",                           &param_rom_mcu->total_ov2_release);
    // err |= read_flash_parameter("total_ov2_release_delay_time",                &param_rom_mcu->total_ov2_release_delay_time);    
    
    // err |= read_flash_parameter("total_ov3_alarm",                             &param_rom_mcu->total_ov3_alarm);
    // err |= read_flash_parameter("total_ov3_alarm_delay_time",                  &param_rom_mcu->total_ov3_alarm_delay_time);
    // err |= read_flash_parameter("total_ov3_release",                           &param_rom_mcu->total_ov3_release);
    // err |= read_flash_parameter("total_ov3_release_delay_time",                &param_rom_mcu->total_ov3_release_delay_time);    
    // err |= read_flash_parameter("total_ov3_unlock_soc",                        &param_rom_mcu->total_ov3_unlock_soc);
    // err |= read_flash_parameter("total_ov3_unlock_charge_current",             &param_rom_mcu->total_ov3_unlock_charge_current);

    // err |= read_flash_parameter("total_uv1_alarm",                             &param_rom_mcu->total_uv1_alarm);
    // err |= read_flash_parameter("total_uv1_alarm_delay_time",                  &param_rom_mcu->total_uv1_alarm_delay_time);
    // err |= read_flash_parameter("total_uv1_release",                           &param_rom_mcu->total_uv1_release);
    // err |= read_flash_parameter("total_uv1_release_delay_time",                &param_rom_mcu->total_uv1_release_delay_time);

    // err |= read_flash_parameter("total_uv2_alarm",                             &param_rom_mcu->total_uv2_alarm);
    // err |= read_flash_parameter("total_uv2_alarm_delay_time",                  &param_rom_mcu->total_uv2_alarm_delay_time);
    // err |= read_flash_parameter("total_uv2_release",                           &param_rom_mcu->total_uv2_release);
    // err |= read_flash_parameter("total_uv2_release_delay_time",                &param_rom_mcu->total_uv2_release_delay_time);    
    
    // err |= read_flash_parameter("total_uv3_alarm",                             &param_rom_mcu->total_uv3_alarm);
    // err |= read_flash_parameter("total_uv3_alarm_delay_time",                  &param_rom_mcu->total_uv3_alarm_delay_time);
    // err |= read_flash_parameter("total_uv3_release",                           &param_rom_mcu->total_uv3_release);
    // err |= read_flash_parameter("total_uv3_release_delay_time",                &param_rom_mcu->total_uv3_release_delay_time);    
    // err |= read_flash_parameter("total_uv3_unlock_charge_current",             &param_rom_mcu->total_uv3_unlock_charge_current);


    // err |= read_flash_parameter("cell_diff_ov1_alarm",                         &param_rom_mcu->cell_diff_ov1_alarm);
    // err |= read_flash_parameter("cell_diff_ov1_alarm_delay_time",              &param_rom_mcu->cell_diff_ov1_alarm_delay_time);
    // err |= read_flash_parameter("cell_diff_ov1_release",                       &param_rom_mcu->cell_diff_ov1_release);
    // err |= read_flash_parameter("cell_diff_ov1_release_delay_time",            &param_rom_mcu->cell_diff_ov1_release_delay_time);

    // err |= read_flash_parameter("cell_diff_ov2_alarm",                         &param_rom_mcu->cell_diff_ov2_alarm);
    // err |= read_flash_parameter("cell_diff_ov2_alarm_delay_time",              &param_rom_mcu->cell_diff_ov2_alarm_delay_time);
    // err |= read_flash_parameter("cell_diff_ov2_release",                       &param_rom_mcu->cell_diff_ov2_release);
    // err |= read_flash_parameter("cell_diff_ov2_release_delay_time",            &param_rom_mcu->cell_diff_ov2_release_delay_time);  

    // /* ===== 电流相关参数 ============================= */
    // err |= read_flash_parameter("charge_oc1_alarm",                            &param_rom_mcu->charge_oc1_alarm);
    // err |= read_flash_parameter("charge_oc1_alarm_delay_time",                 &param_rom_mcu->charge_oc1_alarm_delay_time);
    // err |= read_flash_parameter("charge_oc2_alarm",                            &param_rom_mcu->charge_oc2_alarm);
    // err |= read_flash_parameter("charge_oc2_alarm_delay_time",                 &param_rom_mcu->charge_oc2_alarm_delay_time);    
    // err |= read_flash_parameter("charge_oc3_alarm",                            &param_rom_mcu->charge_oc3_alarm);
    // err |= read_flash_parameter("charge_oc3_alarm_delay_time",                 &param_rom_mcu->charge_oc3_alarm_delay_time);

    // err |= read_flash_parameter("discharge_oc1_alarm",                         &param_rom_mcu->discharge_oc1_alarm);
    // err |= read_flash_parameter("discharge_oc1_alarm_delay_time",              &param_rom_mcu->discharge_oc1_alarm_delay_time);
    // err |= read_flash_parameter("discharge_oc2_alarm",                         &param_rom_mcu->discharge_oc2_alarm);
    // err |= read_flash_parameter("discharge_oc2_alarm_delay_time",              &param_rom_mcu->discharge_oc2_alarm_delay_time);    
    // err |= read_flash_parameter("discharge_oc3_alarm",                         &param_rom_mcu->discharge_oc3_alarm);
    // err |= read_flash_parameter("discharge_oc3_alarm_delay_time",              &param_rom_mcu->discharge_oc3_alarm_delay_time);
    
    // err |= read_flash_parameter("short_circuit_alarm",                         &param_rom_mcu->short_circuit_alarm);
    // err |= read_flash_parameter("short_circuit_alarm_delay_time",              &param_rom_mcu->short_circuit_alarm_delay_time);

    // /* ===== 温度相关参数 ============================= */
    // err |= read_flash_parameter("charge_cell_ot1_alarm",                       &param_rom_mcu->charge_cell_ot1_alarm);
    // err |= read_flash_parameter("charge_cell_ot1_alarm_delay_time",            &param_rom_mcu->charge_cell_ot1_alarm_delay_time);
    // err |= read_flash_parameter("charge_cell_ot1_release",                     &param_rom_mcu->charge_cell_ot1_release);
    // err |= read_flash_parameter("charge_cell_ot1_release_delay_time",          &param_rom_mcu->charge_cell_ot1_release_delay_time);

    // err |= read_flash_parameter("charge_cell_ot2_alarm",                       &param_rom_mcu->charge_cell_ot2_alarm);
    // err |= read_flash_parameter("charge_cell_ot2_alarm_delay_time",            &param_rom_mcu->charge_cell_ot2_alarm_delay_time);
    // err |= read_flash_parameter("charge_cell_ot2_release",                     &param_rom_mcu->charge_cell_ot2_release);
    // err |= read_flash_parameter("charge_cell_ot2_release_delay_time",          &param_rom_mcu->charge_cell_ot2_release_delay_time);    

    // err |= read_flash_parameter("charge_cell_ot3_alarm",                       &param_rom_mcu->charge_cell_ot3_alarm);
    // err |= read_flash_parameter("charge_cell_ot3_alarm_delay_time",            &param_rom_mcu->charge_cell_ot3_alarm_delay_time);
    // err |= read_flash_parameter("charge_cell_ot3_release",                     &param_rom_mcu->charge_cell_ot3_release);
    // err |= read_flash_parameter("charge_cell_ot3_release_delay_time",          &param_rom_mcu->charge_cell_ot3_release_delay_time);  

    // err |= read_flash_parameter("charge_cell_ut1_alarm",                       &param_rom_mcu->charge_cell_ut1_alarm);
    // err |= read_flash_parameter("charge_cell_ut1_alarm_delay_time",            &param_rom_mcu->charge_cell_ut1_alarm_delay_time);
    // err |= read_flash_parameter("charge_cell_ut1_release",                     &param_rom_mcu->charge_cell_ut1_release);
    // err |= read_flash_parameter("charge_cell_ut1_release_delay_time",          &param_rom_mcu->charge_cell_ut1_release_delay_time);

    // err |= read_flash_parameter("charge_cell_ut2_alarm",                       &param_rom_mcu->charge_cell_ut2_alarm);
    // err |= read_flash_parameter("charge_cell_ut2_alarm_delay_time",            &param_rom_mcu->charge_cell_ut2_alarm_delay_time);
    // err |= read_flash_parameter("charge_cell_ut2_release",                     &param_rom_mcu->charge_cell_ut2_release);
    // err |= read_flash_parameter("charge_cell_ut2_release_delay_time",          &param_rom_mcu->charge_cell_ut2_release_delay_time);    

    // err |= read_flash_parameter("charge_cell_ut3_alarm",                       &param_rom_mcu->charge_cell_ut3_alarm);
    // err |= read_flash_parameter("charge_cell_ut3_alarm_delay_time",            &param_rom_mcu->charge_cell_ut3_alarm_delay_time);
    // err |= read_flash_parameter("charge_cell_ut3_release",                     &param_rom_mcu->charge_cell_ut3_release);
    // err |= read_flash_parameter("charge_cell_ut3_release_delay_time",          &param_rom_mcu->charge_cell_ut3_release_delay_time);     


    // err |= read_flash_parameter("discharge_cell_ot1_alarm",                    &param_rom_mcu->discharge_cell_ot1_alarm);
    // err |= read_flash_parameter("discharge_cell_ot1_alarm_delay_time",         &param_rom_mcu->discharge_cell_ot1_alarm_delay_time);
    // err |= read_flash_parameter("discharge_cell_ot1_release",                  &param_rom_mcu->discharge_cell_ot1_release);
    // err |= read_flash_parameter("discharge_cell_ot1_release_delay_time",       &param_rom_mcu->discharge_cell_ot1_release_delay_time);

    // err |= read_flash_parameter("discharge_cell_ot2_alarm",                    &param_rom_mcu->discharge_cell_ot2_alarm);
    // err |= read_flash_parameter("discharge_cell_ot2_alarm_delay_time",         &param_rom_mcu->discharge_cell_ot2_alarm_delay_time);
    // err |= read_flash_parameter("discharge_cell_ot2_release",                  &param_rom_mcu->discharge_cell_ot2_release);
    // err |= read_flash_parameter("discharge_cell_ot2_release_delay_time",       &param_rom_mcu->discharge_cell_ot2_release_delay_time);    
   
    // err |= read_flash_parameter("discharge_cell_ot3_alarm",                    &param_rom_mcu->discharge_cell_ot3_alarm);
    // err |= read_flash_parameter("discharge_cell_ot3_alarm_delay_time",         &param_rom_mcu->discharge_cell_ot3_alarm_delay_time);
    // err |= read_flash_parameter("discharge_cell_ot3_release",                  &param_rom_mcu->discharge_cell_ot3_release);
    // err |= read_flash_parameter("discharge_cell_ot3_release_delay_time",       &param_rom_mcu->discharge_cell_ot3_release_delay_time);  
    
    // err |= read_flash_parameter("discharge_cell_ut1_alarm",                    &param_rom_mcu->discharge_cell_ut1_alarm);
    // err |= read_flash_parameter("discharge_cell_ut1_alarm_delay_time",         &param_rom_mcu->discharge_cell_ut1_alarm_delay_time);
    // err |= read_flash_parameter("discharge_cell_ut1_release",                  &param_rom_mcu->discharge_cell_ut1_release);
    // err |= read_flash_parameter("discharge_cell_ut1_release_delay_time",       &param_rom_mcu->discharge_cell_ut1_release_delay_time);

    // err |= read_flash_parameter("discharge_cell_ut2_alarm",                    &param_rom_mcu->discharge_cell_ut2_alarm);
    // err |= read_flash_parameter("discharge_cell_ut2_alarm_delay_time",         &param_rom_mcu->discharge_cell_ut2_alarm_delay_time);
    // err |= read_flash_parameter("discharge_cell_ut2_release",                  &param_rom_mcu->discharge_cell_ut2_release);
    // err |= read_flash_parameter("discharge_cell_ut2_release_delay_time",       &param_rom_mcu->discharge_cell_ut2_release_delay_time);    
 
    // err |= read_flash_parameter("discharge_cell_ut3_alarm",                    &param_rom_mcu->discharge_cell_ut3_alarm);
    // err |= read_flash_parameter("discharge_cell_ut3_alarm_delay_time",         &param_rom_mcu->discharge_cell_ut3_alarm_delay_time);
    // err |= read_flash_parameter("discharge_cell_ut3_release",                  &param_rom_mcu->discharge_cell_ut3_release);
    // err |= read_flash_parameter("discharge_cell_ut3_release_delay_time",       &param_rom_mcu->discharge_cell_ut3_release_delay_time);

    
    // err |= read_flash_parameter("cell_ot1_alarm",                              &param_rom_mcu->cell_ot1_alarm);
    // err |= read_flash_parameter("cell_ot1_alarm_delay_time",                   &param_rom_mcu->cell_ot1_alarm_delay_time);
    // err |= read_flash_parameter("cell_ot1_release",                            &param_rom_mcu->cell_ot1_release);
    // err |= read_flash_parameter("cell_ot1_release_delay_time",                 &param_rom_mcu->cell_ot1_release_delay_time);

    // err |= read_flash_parameter("cell_ot2_alarm",                              &param_rom_mcu->cell_ot2_alarm);
    // err |= read_flash_parameter("cell_ot2_alarm_delay_time",                   &param_rom_mcu->cell_ot2_alarm_delay_time);
    // err |= read_flash_parameter("cell_ot2_release",                            &param_rom_mcu->cell_ot2_release);
    // err |= read_flash_parameter("cell_ot2_release_delay_time",                 &param_rom_mcu->cell_ot2_release_delay_time);  

    // err |= read_flash_parameter("cell_diff_temp1_alarm",                       &param_rom_mcu->cell_diff_temp1_alarm);
    // err |= read_flash_parameter("cell_diff_temp1_alarm_delay_time",            &param_rom_mcu->cell_diff_temp1_alarm_delay_time);
    // err |= read_flash_parameter("cell_diff_temp1_release",                     &param_rom_mcu->cell_diff_temp1_release);
    // err |= read_flash_parameter("cell_diff_temp1_release_delay_time",          &param_rom_mcu->cell_diff_temp1_release_delay_time);
    
    // err |= read_flash_parameter("cell_diff_temp2_alarm",                       &param_rom_mcu->cell_diff_temp2_alarm);
    // err |= read_flash_parameter("cell_diff_temp2_alarm_delay_time",            &param_rom_mcu->cell_diff_temp2_alarm_delay_time);
    // err |= read_flash_parameter("cell_diff_temp2_release",                     &param_rom_mcu->cell_diff_temp2_release);
    // err |= read_flash_parameter("cell_diff_temp2_release_delay_time",          &param_rom_mcu->cell_diff_temp2_release_delay_time);  


    // err |= read_flash_parameter("env_ot_high1_alarm",                          &param_rom_mcu->env_ot_high1_alarm);
    // err |= read_flash_parameter("env_ot_high1_alarm_delay_time",               &param_rom_mcu->env_ot_high1_alarm_delay_time);
    // err |= read_flash_parameter("env_ot_high1_release",                        &param_rom_mcu->env_ot_high1_release);
    // err |= read_flash_parameter("env_ot_high1_release_delay_time",             &param_rom_mcu->env_ot_high1_release_delay_time);
    
    // err |= read_flash_parameter("env_ot_high2_alarm",                          &param_rom_mcu->env_ot_high2_alarm);
    // err |= read_flash_parameter("env_ot_high2_alarm_delay_time",               &param_rom_mcu->env_ot_high2_alarm_delay_time);
    // err |= read_flash_parameter("env_ot_high2_release",                        &param_rom_mcu->env_ot_high2_release);
    // err |= read_flash_parameter("env_ot_high2_release_delay_time",             &param_rom_mcu->env_ot_high2_release_delay_time);  

    // err |= read_flash_parameter("env_ut_low1_alarm",                           &param_rom_mcu->env_ut_low1_alarm);
    // err |= read_flash_parameter("env_ut_low1_alarm_delay_time",                &param_rom_mcu->env_ut_low1_alarm_delay_time);
    // err |= read_flash_parameter("env_ut_low1_release",                         &param_rom_mcu->env_ut_low1_release);
    // err |= read_flash_parameter("env_ut_low1_release_delay_time",              &param_rom_mcu->env_ut_low1_release_delay_time);

    // err |= read_flash_parameter("env_ut_low2_alarm",                           &param_rom_mcu->env_ut_low2_alarm);
    // err |= read_flash_parameter("env_ut_low2_alarm_delay_time",                &param_rom_mcu->env_ut_low2_alarm_delay_time);
    // err |= read_flash_parameter("env_ut_low2_release",                         &param_rom_mcu->env_ut_low2_release);
    // err |= read_flash_parameter("env_ut_low2_release_delay_time",              &param_rom_mcu->env_ut_low2_release_delay_time);  


    // err |= read_flash_parameter("cell_balance_start_voltage",                  &param_rom_mcu->cell_balance_start_voltage);
    // err |= read_flash_parameter("cell_balance_start_diff",                     &param_rom_mcu->cell_balance_start_diff);
    // err |= read_flash_parameter("cell_sleep_voltage",                          &param_rom_mcu->cell_sleep_voltage);
    // err |= read_flash_parameter("cell_sleep_duration",                         &param_rom_mcu->cell_sleep_duration);  
    // err |= read_flash_parameter("cell_full_charge_voltage",                    &param_rom_mcu->cell_full_charge_voltage);
    // err |= read_flash_parameter("full_charge_current",                         &param_rom_mcu->full_charge_current);  

    // err |= read_flash_parameter("battery_capacity_initial",                    &param_rom_mcu->battery_capacity_initial);
    // err |= read_flash_parameter("battery_capacity_current",                    &param_rom_mcu->battery_capacity_current);  
    // err |= read_flash_parameter("cycle_count",                                 &param_rom_mcu->cycle_count);
    // err |= read_flash_parameter("total_runtime",                               &param_rom_mcu->total_runtime);  

    // err |= read_flash_parameter("soc_initial",                                 &param_rom_mcu->soc_initial);
    // err |= read_flash_parameter("soc_low_alarm",                               &param_rom_mcu->soc_low_alarm);
    // err |= read_flash_parameter("soh_initial",                                 &param_rom_mcu->soh_initial);  
    // err |= read_flash_parameter("soh_alarm",                                   &param_rom_mcu->soh_alarm);
    // err |= read_flash_parameter("sod_deep_discharge",                          &param_rom_mcu->sod_deep_discharge); 

    // err |= read_flash_parameter("heater_off_temp",                             &param_rom_mcu->heater_off_temp);
    // printf("44444err = %d rom_parameter = %d\r\n",err, param_rom_mcu->heater_off_temp);
    // err |= read_flash_parameter("device_sn_high",                              &param_rom_mcu->device_sn_high);
    // err |= read_flash_parameter("device_sn_low",                               &param_rom_mcu->device_sn_low);
    // err |= read_flash_parameter("device_id",                                   &param_rom_mcu->device_id);
    // err |= read_flash_parameter("buzzer_duration",                             &param_rom_mcu->buzzer_duration);
    // err |= read_flash_parameter("power_on_activation",                         &param_rom_mcu->power_on_activation);
    // err |= read_flash_parameter("shutdown_cmd",                                &param_rom_mcu->shutdown_cmd);
    
    // err |= read_flash_parameter("heater_on_cmd",                               &param_rom_mcu->heater_on_cmd);
    // err |= read_flash_parameter("heater_off_cmd",                              &param_rom_mcu->heater_off_cmd);
    // err |= read_flash_parameter("heater_auto_off_temp",                        &param_rom_mcu->heater_auto_off_temp);

    // err |= read_flash_parameter("indicator_on_cmd",                            &param_rom_mcu->indicator_on_cmd);
    // err |= read_flash_parameter("indicator_off_cmd",                           &param_rom_mcu->indicator_off_cmd);

    // err |= read_flash_parameter("alarm_light_on_cmd",                          &param_rom_mcu->alarm_light_on_cmd);
    // err |= read_flash_parameter("alarm_light_off_cmd",                         &param_rom_mcu->alarm_light_off_cmd);

    // err |= read_flash_parameter("buzzer_enable_cmd",                           &param_rom_mcu->buzzer_enable_cmd);
    // err |= read_flash_parameter("buzzer_disable_cmd",                          &param_rom_mcu->buzzer_disable_cmd);
    // err |= read_flash_parameter("buzzer_status",                               &param_rom_mcu->buzzer_status);
    // err |= read_flash_parameter("buzzer_stop_cmd",                             &param_rom_mcu->buzzer_stop_cmd);

    // err |= read_flash_parameter("current_calib_prepare",                       &param_rom_mcu->current_calib_prepare);
    // err |= read_flash_parameter("current_calib_set_value",                     &param_rom_mcu->current_calib_set_value);
    // err |= read_flash_parameter("current_calib_start",                         &param_rom_mcu->current_calib_start);
    // err |= read_flash_parameter("current_calib_verify",                        &param_rom_mcu->current_calib_verify);

    // err |= read_flash_parameter("short_circuit_release_on",                    &param_rom_mcu->short_circuit_release_on);
    // err |= read_flash_parameter("short_circuit_release_off",                   &param_rom_mcu->short_circuit_release_off);

    // err |= read_flash_parameter("pre_discharge_on",                            &param_rom_mcu->pre_discharge_on);
    // err |= read_flash_parameter("pre_discharge_off",                           &param_rom_mcu->pre_discharge_off);

    // err |= read_flash_parameter("discharge_mos_on",                            &param_rom_mcu->discharge_mos_on);
    // err |= read_flash_parameter("discharge_mos_off",                           &param_rom_mcu->discharge_mos_off);

    // err |= read_flash_parameter("charge_mos_on",                               &param_rom_mcu->charge_mos_on);
    // err |= read_flash_parameter("charge_mos_off",                              &param_rom_mcu->charge_mos_off);

    // err |= read_flash_parameter("fuse_blow_cmd",                               &param_rom_mcu->fuse_blow_cmd);
    // err |= read_flash_parameter("fuse_reset_cmd",                              &param_rom_mcu->fuse_reset_cmd);

    // err |= read_flash_parameter("afe_eeprom_save_cmd",                         &param_rom_mcu->afe_eeprom_save_cmd);
    // err |= read_flash_parameter("mcu_protection_save_cmd",                     &param_rom_mcu->mcu_protection_save_cmd);

    // err |= read_flash_parameter("charge_discharge_ctrl",                       &param_rom_mcu->charge_discharge_ctrl);
    // printf("55555err = %d rom_parameter = %d\r\n",err, param_rom_mcu->charge_discharge_ctrl);
    // err |= read_flash_parameter("mcu_init_flag",                               &param_rom_mcu->mcu_init_flag);
    /* --其他 */
    if (err) {
        return 1;
    }
    return 0;
}

// /**
//  * @函数名称: parameter_compare_rom_mcu_parameters()
//  * @功能描述: 比较 ROM_MCU_PARAMETER_TYPDEF 初始化结构体参数
//  * @输入: parameter: 结构体值
//  *        compare_parameter: 比较目标结构体值
//  * @输出: 无
//  * @返回值: true: 参数一致
//  *          false: 参数不一致
//  */
// static bool parameter_compare_rom_mcu_parameters(ROM_MCU_PARAMETER_TYPDEF* parameter, ROM_MCU_PARAMETER_TYPDEF* compare_parameter)
// {
//     /* 比较所有类型数据 */
//     // if (parameter->active_flag != compare_parameter->active_flag) return false;
//     return true;
// }

/**
 * @函数名称: parameter_ram_init()
 * @功能描述: ram存储参数注册
 * @输入: 无
 * @输出: 无
 * @返回值: 0成果 1是吧
 */
uint8_t parameter_rom_init(ROM_MCU_PARAMETER_TYPDEF* parameter)
{
    read_flash_parameter("mcu_init_flag", &parameter->mcu_init_flag);
    printf("mcu_init_flag = %x\r\n",parameter->mcu_init_flag);
   /* 1.如果上电后flash中没有值 */
    if (parameter->mcu_init_flag == 0x00) { 
        PARAMTER_LOG("flash is empty, loading default parameters...\r\n");
        /* 将默认值写入flash中 */
        parameter_rom_default_init(parameter);

        PARAMTER_LOG("*******************************\r\n");
        parameter_rom_write(parameter);
        PARAMTER_LOG("*******************************\r\n");
        /* 从flash进行读取 */
        parameter_rom_read(parameter);
    } else {
        /* 2.如果上电后flash有值，读取flash，如果代码中的默认参数被修改了需要和flash中对比进行更新flash */
        // PARAMTER_LOG("Loading parameters from flash...\r\n");
        // /* 从flash进行读取 */
        // if(can_mcu_set_flag == 1) {
        //     /* 如果有CAN数据则进行写flash，然后读取加载 */
        //     parameter_rom_write(parameter);
        //     parameter_rom_read(parameter);
        //    can_mcu_set_flag = 0;
        // } else {
        //     /* 从flash进行读取 */
        //     parameter_rom_read(parameter);
        // }
    } 
    return 0;  
}

/* ram全集参数 */
RAM_PARAMETER_TYPDEF param_ram = {0};

/**
 * @函数名称: parameter_ram_init()
 * @功能描述: ram存储参数注册
 * @输入: 无
 * @输出: 无
 * @返回值: 0成果 1是吧
 */
uint8_t parameter_ram_init(void)
{
    uint8_t err = 0;
    param_ram.uninitialized_flag = TASK_UNINITIALIZED_FLAG_MASK; /* 未初始化标志位 */
    /* --mcu */
    
    param_ram.ui_run_sattus = 1;
    param_ram.ui_alarm_status = 1;
    param_ram.ui_beep_status = 1;

    param_ram.protect_charge_flag       = 0;//0xffff;
    param_ram.protect_discharge_flag    = 0;//0xffff;
    param_ram.protect_fuse_flag         = 0;//0xffff;
    param_ram.protect_nonactivated_flag = 0;// 0xffffffff;
    param_ram.protect_voltage_flag      = 0;//0xffffffff;
    param_ram.protect_current_flag      = 0;//0xffffffff;
    param_ram.protect_temperature_high_flag  = 0;//0xffffffff;
    param_ram.protect_temperature_low_flag  = 0;//0xffffffff;
    param_ram.protect_charge_status     = 0;//0xffff;
    param_ram.protect_discharge_status  = 0;//0xffff;
    param_ram.protect_fuse_status       = 0;//0xffff;

    param_ram.bms_can0_active_flag     = 1;/* 临时测试用1 默认0 */
    param_ram.bms_can1_active_flag     = 1;/* 临时测试用1 默认0 */
    param_ram.bms_usart0_active_flag   = 1;/* 临时测试用1 默认0 */
    param_ram.bms_usart2_active_flag   = 1;/* 临时测试用1 默认0 */
    param_ram.bms_key_active_err_flag  = 0;
    param_ram.bms_key_active_flag      = 0;
    param_ram.bms_enclosure_status     = 0;
    param_ram.bms_watchdog_status      = 0;
    param_ram.bms_alarm_flag           = 0;
    param_ram.bms_alarm_rgb_status     = 0;
    param_ram.bms_alarm_beep_status    = 0;
    param_ram.bms_alarm_run_led_status = 0;
    param_ram.bms_sleep_status         = 0;

    param_ram.battery_current = 0;
    param_ram.voltage_diff_max = 0;
    param_ram.voltage_cell_max = 0;
    param_ram.voltage_cell_max_pos = 0;
    param_ram.voltage_cell_min = 0;
    param_ram.voltage_cell_min_pos = 0;
    param_ram.voltage_cell_total = 0;
    param_ram.voltage_cell[0] = 0;
    param_ram.voltage_cell[1] = 0;
    param_ram.voltage_cell[2] = 0;
    param_ram.voltage_cell[3] = 0;
    param_ram.voltage_cell[4] = 0;
    param_ram.voltage_cell[5] = 0;
    param_ram.voltage_cell[6] = 0;
    param_ram.voltage_cell[7] = 0;
    param_ram.voltage_cell[8] = 0;
    param_ram.voltage_cell[9] = 0;
    param_ram.voltage_cell[10] = 0;
    param_ram.voltage_cell[11] = 0;
    param_ram.voltage_cell[12] = 0;
    param_ram.voltage_cell[13] = 0;
    param_ram.voltage_cell[14] = 0;
    param_ram.voltage_cell[15] = 0;
    param_ram.temp_ambient = 0;
    param_ram.temp_diff_max = 0;
    param_ram.temp_charge_mos = 0;
    param_ram.temp_discharge_mos = 0;
    param_ram.temp_cell_max = 0;
    param_ram.temp_cell_max_pos = 0;
    param_ram.temp_cell_min = 0;
    param_ram.temp_cell_min_pos = 0;
    param_ram.temp_cell_1 = 0;
    param_ram.temp_cell_2 = 0;
    param_ram.temp_cell_3 = 0;
    param_ram.temp_cell_4 = 0;
    param_ram.status_charge_discharge = 0;
    param_ram.soc = 50;
    param_ram.sod = 0;
    param_ram.soh = 100;
    param_ram.voltage_can_rs485_power = 0;
    param_ram.voltage_battery_total = 0;
    param_ram.current_sense_value = 0;
    param_ram.alarm_level = 0;
    param_ram.protection_flags_voltage = 0;
    param_ram.protection_flags_current = 0;
    param_ram.protection_high_flags_temperature = 0;
    param_ram.protection_low_flags_temperature = 0;
    param_ram.protection_flags_others = 0;

    if (err) {
        return 1;
    }

    return 0;
}
#endif

// PARAMETER_DCDC dcdc_param = {0};

/**
 * @函数名称: parameter_dcdc_init()
 * @功能描述: parameter_dcdc存储参数注册
 * @输入: 无
 * @输出: 无
 * @返回值: 0成果 1是吧
 */
uint8_t parameter_dcdc_init(void)
{
    uint8_t err = 0;
    /* -trifle-mcu */
    
    // dcdc_param.mcu_param.battery_current = 0;


    // dcdc_param.dcdc_param->dcdc_device_address = 1;
    if (err) {
        return 1;
    }

    return 0;
}


AFE_INIT* parameter = NULL;
ROM_MCU_PARAMETER_TYPDEF* mcu_rom_data = NULL;
AFE_INIT* afe_init_data = NULL;

void parameter_register_and_init(void)
{
    /* 注册AFE初始化，MCU初始化参数所需参数 */
    parameter = malloc(sizeof(AFE_INIT));
    mcu_rom_data = malloc(sizeof(ROM_MCU_PARAMETER_TYPDEF));
    afe_init_data = malloc(sizeof(AFE_INIT));

    // battery_afe_register_flash_parameter(parameter);
    // parameter_rom_register(&rom_parameter);

    /* 初始化写入FLASH将参数值 */
    battery_afe_init(parameter); 
    
    parameter_rom_default_init(&rom_parameter);

    // parameter_rom_init(&rom_parameter);
    /* 此处读取用于CAN设置ROM参数的结构体 */
    // parameter_rom_read(mcu_rom_data); 
    // battery_afe_read_init_register_config(afe_init_data);
}
