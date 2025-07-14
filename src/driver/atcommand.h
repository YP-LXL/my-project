#ifndef ATCOMMAND_H
#define ATCOMMAND_H

#include "common.h"

#define ATCMD_NAME_MAX_LEN   16U
#define ATCMD_PARAMS_MAX_LEN 80U

#define TRANSFER_NUM         100U

#define PARAM_MAX_NUM        6U    //Every AT command must has less than 6 params
#define PER_PARAM_MAX_LEN    12U    //every param must less than 12 charator


typedef struct
{
	char *name;                       //AT指令名称,eg,AT+MOTORR, AT+SENSORW and so on
  uint8_t (*ATfunc)(char params[]);  // AT指令处理函数
}ATCMD_HANDLER_TYPE;                        //用于在系统中注册AT命令及其处理函数的结构体

typedef struct{
	char *name;     /*AT指令名称*/
	char *params;   /*参数*/
}ATCMD_TYPE;				//用于接收存储接收到的AT指令

typedef struct ATCMD_HANDLER_LIST_TYPE
{
	ATCMD_HANDLER_TYPE *ATcmd_handler;
  struct ATCMD_HANDLER_LIST_TYPE *prev;
  struct ATCMD_HANDLER_LIST_TYPE *next;
}ATCMD_HANDLER_LIST_TYPE;


uint8_t AT_check_command_valid(void);
uint8_t AT_get_cmd_len(void);
void ATcmd_get_name(char name[]);
void ATcmd_get_params(char params[]);
void ATcmd_split_params(char params[], char param[][PER_PARAM_MAX_LEN], uint8_t *param_num) ;
void ATcmd_init_cmdData(ATCMD_TYPE *cmd, char name[], char params[]);
void ATcmd_execute(ATCMD_TYPE *cmd);
void ATcmd_register_handler(ATCMD_HANDLER_TYPE *ATcmd_handler);
void ATcmd_register(char *ATcmd_name, uint8_t (*ATfunc)(char params[]));
void ATcmd_init(void);
void ATcmd_server(void);
uint8_t ATcmd_set_parse_data_address(char* p_data);

int String2Int(char *str); 
uint32_t String2Hex(const char *str);
float String2float(char *str);
void String2ASCII(char *str, uint32_t data[]);


uint8_t at_ledon_func(char params[]);
uint8_t at_ledoff_func(char params[]);
uint8_t at_gpioouton_func(char params[]);
uint8_t at_gpiooutoff_func(char params[]);
uint8_t at_gpioinon_func(char params[]);
uint8_t at_gpioinoff_func(char params[]);
uint8_t at_adc_func(char params[]);
uint8_t at_beepwm_func(char params[]);
uint8_t at_fanpwm_func(char params[]);
uint8_t at_iic_func(char params[]);
uint8_t at_spi_func(char params[]);
uint8_t at_canon_func(char params[]);
uint8_t at_canoff_func(char params[]);
uint8_t at_usarton_func(char params[]);
uint8_t at_usartoff_func(char params[]);
uint8_t at_sleep_func(char params[]);
uint8_t at_wdgton_func(char params[]);
uint8_t at_softreset_func(char params[]);
uint8_t at_gdid_func(char params[]);

extern uint16_t atcommand_can_flag;
#endif
