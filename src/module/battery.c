#include "battery.h"
/* AFE************************************************************************** */
/* 中断回调函数指针AFE与MCU之间通信引脚，采用中断方式，在main函数中挂载中断回调函数 */
ALARM_DATA alarm_state = {0};
/* 表格数据 B = 3950*/
// TEMP_RNOR_DATA temp_rnor_table[] = {
//     {-40, 345.275}, {-39, 322.791}, {-38, 301.925},
//     {-37, 282.549}, {-36, 264.549}, {-35, 247.816},
//     {-34, 232.254}, {-33, 217.774}, {-32, 204.292},
//     {-31, 191.735}, {-30, 180.032}, {-29, 169.120},
//     {-28, 158.941}, {-27, 149.441}, {-26, 140.571},
//     {-25, 132.284}, {-24, 124.522}, {-23, 117.266},
//     {-22, 110.480}, {-21, 104.130}, {-20, 98.185},
//     {-19, 92.618}, {-18, 87.402}, {-17, 82.513},
//     {-16, 77.927}, {-15, 73.626}, {-14, 69.588},
//     {-13, 65.797}, {-12, 62.237}, {-11, 58.890},
//     {-10, 55.744}, {-9, 52.786}, {-8, 50.002},
//     {-7, 47.382}, {-6, 44.916}, {-5, 42.592},
//     {-4, 40.400}, {-3, 38.333}, {-2, 36.385},
//     {-1, 34.548}, {0, 32.814}, {1, 31.179},
//     {2, 29.636}, {3, 28.178}, {4, 26.800},
//     {5, 25.497}, {6, 24.263}, {7, 23.096},
//     {8, 21.992}, {9, 20.947}, {10, 19.958},
//     {11, 19.022}, {12, 18.135}, {13, 17.294},
//     {14, 16.498}, {15, 15.742}, {16, 15.025},
//     {17, 14.345}, {18, 13.699}, {19, 13.086},
//     {20, 12.504}, {21, 11.951}, {22, 11.426},
//     {23, 10.926}, {24, 10.452}, {25, 10},
//     {26, 9.570}, {27, 9.162}, {28, 8.773},
//     {29, 8.402}, {30, 8.049}, {31, 7.773},
//     {32, 7.393}, {33, 7.088}, {34, 6.797},
//     {35, 6.520}, {36, 6.255}, {37, 6.003},
//     {38, 5.762}, {39, 5.532}, {40, 5.313},
//     {41, 5.103}, {42, 4.903}, {43, 4.711},
//     {44, 4.529}, {45, 4.354}, {46, 4.187},
//     {47, 4.027}, {48, 3.874}, {49, 3.728},
//     {50, 3.588}, {51, 3.454}, {52, 3.326},
//     {53, 3.203}, {54, 3.086}, {55, 2.973},
//     {56, 2.865}, {57, 2.761}, {58, 2.662},
//     {59, 2.567}, {60, 2.476}, {61, 2.388},
//     {62, 2.304}, {63, 2.224}, {64, 2.146},
//     {65, 2.072}, {66, 2.001}, {67, 1.932},
//     {68, 1.866}, {69, 1.803}, {70, 1.742},
//     {71, 1.684}, {72, 1.628}, {73, 1.574},
//     {74, 1.522}, {75, 1.472}, {76, 1.424},
//     {77, 1.387}, {78, 1.333}, {79, 1.290},
//     {80, 1.249}, {81, 1.209}, {82, 1.171},
//     {83, 1.134}, {84, 1.099}, {85, 1.065},
//     {86, 1.032}, {87, 1.000}, {88, 0.969},
//     {89, 0.940}, {90, 0.911}, {91, 0.884},
//     {92, 0.857}, {93, 0.831}, {94, 0.807},
//     {95, 0.783}, {96, 0.760}, {97, 0.738},
//     {98, 0.716}, {99, 0.695}, {100, 0.675},
//     {101, 0.656}, {102, 0.637}, {103, 0.619},
//     {104, 0.602}, {105, 0.585}, {106, 0.569},
//     {107, 0.553}, {108, 0.538}, {109, 0.523},
//     {110, 0.508}, {111, 0.495}, {112, 0.481},
//     {113, 0.468}, {114, 0.456}, {115, 0.443},
//     {116, 0.432}, {117, 0.420}, {118, 0.409},
//     {119, 0.399}, {120, 0.388}, {121, 0.378},
//     {122, 0.368}, {123, 0.359}, {124, 0.350},
//     {125, 0.341}
// };
/* 表格数据 B = 3435*/
#if 0 /* 对错未知 */
TEMP_RNOR_DATA temp_rnor_table[] = {
    {-55, 500.13}, {-54, 471.961}, {-53, 444.955},
    {-52, 419.191}, {-51, 394.713}, {-50, 371.534},
    {-49, 349.645}, {-48, 329.02}, {-47, 309.621},
    {-46, 291.402}, {-45, 274.309}, {-44, 258.287},
    {-43, 243.278}, {-42, 229.223}, {-41, 216.066},
    {-40, 203.75}, {-39, 192.22}, {-38, 181.427},
    {-37, 171.32}, {-36, 161.854}, {-35, 152.984},
    {-34, 144.67}, {-33, 136.874}, {-32, 129.559},

    {-31, 122.694}, {-30, 116.247}, {-29, 110.189},
    {-28, 104.494}, {-27, 99.137}, {-26, 94.096},
    {-25, 89.35}, {-24, 84.877}, {-23, 80.662},
    {-22, 76.687}, {-21, 72.935}, {-20, 69.394},
    {-19, 66.049}, {-18, 62.888}, {-17, 59.9},
    {-16, 57.073}, {-15, 54.398}, {-14, 51.866},
    {-13, 49.468}, {-12, 47.196}, {-11, 45.042},
    {-10, 43}, {-9, 41.062}, {-8, 39.224},
    {-7, 37.479}, {-6, 35.822}, {-5, 34.247},
    {-4, 32.751}, {-3, 31.329}, {-2, 29.977},

    {-1, 28.691}, {0, 27.513}, {1, 26.303},
    {2, 25.195}, {3, 24.139}, {4, 23.134},
    {5, 22.176}, {6, 21.263}, {7, 20.392},
    {8, 19.562}, {9, 18.771}, {10, 18.016},
    {11, 17.295}, {12, 16.607}, {13, 15.95},
    {14, 15.323}, {15, 14.724}, {16, 14.151},
    {17, 13.604}, {18, 13.081}, {19, 12.581},
    {20, 12.103}, {21, 11.646}, {22, 11.208},
    {23, 10.789}, {24, 10.389}, {25, 10},

    {26, 9.637}, {27, 9.285}, {28, 8.948},
    {29, 8.624}, {30, 8.315}, {31, 8.018},
    {32, 7.733}, {33, 7.459}, {34, 7.197},
    {35, 6.946}, {36, 6.704}, {37, 6.473},
    {38, 6.25}, {39, 6.036}, {40, 5.831},
    {41, 5.634}, {42, 5.444}, {43, 5.262},
    {44, 5.087}, {45, 4.918}, {46, 4.756},
    {47, 4.601}, {48, 4.451}, {49, 4.306},
    {50, 4.168}, {51, 4.034}, {52, 3.905},
    {53, 3.781}, {54, 3.662}, {55, 3.546},

    {56, 3.436}, {57, 3.329}, {58, 3.226},
    {59, 3.126}, {60, 3.03 }, {61, 2.938},
    {62, 2.849}, {63, 2.763}, {64, 2.68},
    {65, 2.6},   {66, 2.522}, {67, 2.448},
    {68, 2.376}, {69, 2.306}, {70, 2.239},
    {71, 2.174}, {72, 2.111}, {73, 2.05},
    {74, 1.992}, {75, 1.935}, {76, 1.88},
    {77, 1.827}, {78, 1.776}, {79, 1.726},
    {80, 1.678}, {81, 1.632}, {82, 1.587},
    {83, 1.543}, {84, 1.501}, {85, 1.461},

    {86, 1.421}, {87, 1.383}, {88, 1.346},
    {89, 1.310}, {90, 1.275}, {91, 1.242},
    {92, 1.209}, {93, 1.178}, {94, 1.147},
    {95, 1.118}, {96, 1.089}, {97, 1.061},
    {98, 1.034}, {99, 1.008}, {100, 0.983},
    {101, 0.958}, {102, 0.934}, {103, 0.911},
    {104, 0.888}, {105, 0.867}, {106, 0.846},
    {107, 0.825}, {108, 0.805}, {109, 0.786},
    {110, 0.767}, {111, 0.749}, {112, 0.731},

    {113, 0.714}, {114, 0.698}, {115, 0.681},
    {116, 0.666}, {117, 0.651}, {118, 0.636},
    {119, 0.621}, {120, 0.607}, {121, 0.594},
    {122, 0.581}, {123, 0.568}, {124, 0.556},
    {125, 0.544}, {126, 0.532}, {127, 0.52},
    {128, 0.509}, {129, 0.499}, {130, 0.488},
    {131, 0.478}, {132, 0.468}, {133, 0.459},
    {134, 0.449}, {135, 0.44}, {136, 0.431},
    {137, 0.423}, {138, 0.414}, {139, 0.406},
    {140, 0.398}, {141, 0.391}, {142, 0.383},
    {143, 0.376}, {144, 0.369}, {145, 0.362},
    {146, 0.356}, {147, 0.349}, {148, 0.343},
    {149, 0.337}, {150, 0.331}
};
#endif
/* 表格数据 B = 3435*/
const TEMP_RNOR_DATA temp_rnor_table[] = {
{-55, 500.13},
{-54, 471.961},
{-53, 444.955},
{-52, 419.191},
{-51, 394.713},
{-50, 371.534},
{-49, 349.645},
{-48, 329.02},
{-47, 309.621},
{-46, 291.402},
{-45, 274.309},
{-44, 258.287},
{-43, 243.278},
{-42, 229.223},
{-41, 216.066},
{-40, 203.75},
{-39, 192.22},
{-38, 181.427},
{-37, 171.32},
{-36, 161.854},
{-35, 152.984},
{-34, 144.67},
{-33, 136.874},
{-32, 129.559},
{-31, 122.694},
{-30, 116.247},
{-29, 110.189},
{-28, 104.494},
{-27, 99.137},
{-26, 94.096},
{-25, 89.35},
{-24, 84.877},
{-23, 80.662},
{-22, 76.687},
{-21, 72.935},
{-20, 69.394},
{-19, 66.049},
{-18, 62.888},
{-17, 59.9},
{-16, 57.073},
{-15, 54.398},
{-14, 51.866},
{-13, 49.468},
{-12, 47.196},
{-11, 45.042},
{-10, 43},
{-9, 41.062},
{-8, 39.224},
{-7, 37.479},
{-6, 35.822},
{-5, 34.247},
{-4, 32.751},
{-3, 31.329},
{-2, 29.977},
{-1, 28.691},
{0, 27.513},
{1, 26.303},
{2, 25.195},
{3, 24.139},
{4, 23.134},
{5, 22.176},
{6, 21.263},
{7, 20.392},
{8, 19.562},
{9, 18.771},
{10, 18.016},
{11, 17.295},
{12, 16.607},
{13, 15.95},
{14, 15.323},
{15, 14.724},
{16, 14.151},
{17, 13.604},
{18, 13.081},
{19, 12.581},
{20, 12.103},
{21, 11.646},
{22, 11.208},
{23, 10.789},
{24, 10.389},
{25, 10.1},
{26, 9.637},
{27, 9.285},
{28, 8.948},
{29, 8.624},
{30, 8.315},
{31, 8.018},
{32, 7.733},
{33, 7.459},
{34, 7.197},
{35, 6.946},
{36, 6.704},
{37, 6.473},
{38, 6.25},
{39, 6.036},
{40, 5.831},
{41, 5.634},
{42, 5.444},
{43, 5.262},
{44, 5.087},
{45, 4.918},
{46, 4.756},
{47, 4.601},
{48, 4.451},
{49, 4.306},
{50, 4.168},
{51, 4.034},
{52, 3.905},
{53, 3.781},
{54, 3.662},
{55, 3.546},
{56, 3.436},
{57, 3.329},
{58, 3.226},
{59, 3.126},
{60, 3.03},
{61, 2.938},
{62, 2.849},
{63, 2.763},
{64, 2.68},
{65, 2.6},
{66, 2.522},
{67, 2.448},
{68, 2.376},
{69, 2.306},
{70, 2.239},
{71, 2.174},
{72, 2.111},
{73, 2.05},
{74, 1.992},
{75, 1.935},
{76, 1.88},
{77, 1.827},
{78, 1.776},
{79, 1.726},
{80, 1.678},
{81, 1.632},
{82, 1.587},
{83, 1.543},
{84, 1.501},
{85, 1.461},
{86, 1.421},
{87, 1.383},
{88, 1.346},
{89, 1.31},
{90, 1.275},
{91, 1.242},
{92, 1.209},
{93, 1.178},
{94, 1.147},
{95, 1.118},
{96, 1.089},
{97, 1.061},
{98, 1.034},
{99, 1.008},
{100, 0.983},
{101, 0.958},
{102, 0.934},
{103, 0.911},
{104, 0.888},
{105, 0.867},
{106, 0.846},
{107, 0.825},
{108, 0.805},
{109, 0.786},
{110, 0.767},
{111, 0.749},
{112, 0.731},
{113, 0.714},
{114, 0.698},
{115, 0.681},
{116, 0.666},
{117, 0.651},
{118, 0.636},
{119, 0.621},
{120, 0.607},
{121, 0.594},
{122, 0.581},
{123, 0.568},
{124, 0.556},
{125, 0.544},
{126, 0.532},
{127, 0.52},
{128, 0.509},
{129, 0.499},
{130, 0.488},
{131, 0.478},
{132, 0.468},
{133, 0.459},
{134, 0.449},
{135, 0.44},
{136, 0.431},
{137, 0.423},
{138, 0.414},
{139, 0.406},
{140, 0.398},
{141, 0.391},
{142, 0.383},
{143, 0.392},
{144, 0.385},
{145, 0.378},
{146, 0.371},
{147, 0.364},
{148, 0.358},
{149, 0.352},
{150, 0.345},
};
/**
 * @函数名称: battery_afe_register_flash_parameter()
 * @功能描述: 注册所有参数值
 * @输入: init_parameter: 初始化结构体参数
 * @输出: 无
 * @返回值: 无
 */
void battery_afe_register_flash_parameter(AFE_INIT* init_parameter)
{
    if (init_parameter == NULL) {
        /* 防止空指针访问 */
        return;
    }
    /* 将 init_parameter 的所有字段初始化为 0 */
    memset(init_parameter, 0, sizeof(AFE_INIT));
    /* FLASH */
    register_flash_parameter("cell_overvoltage_value_time", TYPE_UINT16, &init_parameter->cell_overvoltage_value_time);
    register_flash_parameter("cell_overvoltage_value", TYPE_UINT16, &init_parameter->cell_overvoltage_value);
    register_flash_parameter("cell_overvoltage_Load_release_delay", TYPE_UINT16, &init_parameter->cell_overvoltage_Load_release_delay);
    register_flash_parameter("cell_undervoltage_value_time", TYPE_UINT16, &init_parameter->cell_undervoltage_value_time);
    register_flash_parameter("cell_overvoltage_recovery_value", TYPE_UINT16, &init_parameter->cell_overvoltage_recovery_value);
    register_flash_parameter("cell_undervoltage_value", TYPE_UINT16, &init_parameter->cell_undervoltage_value);
    register_flash_parameter("cell_undervoltage_recovery_value", TYPE_UINT16, &init_parameter->cell_undervoltage_recovery_value);
    register_flash_parameter("balance_voltage_value", TYPE_UINT16, &init_parameter->balance_voltage_value);
    register_flash_parameter("precharge_voltage_value", TYPE_UINT16, &init_parameter->precharge_voltage_value);
    register_flash_parameter("low_voltage_stop_charging_voltage_value", TYPE_UINT16, &init_parameter->low_voltage_stop_charging_voltage_value);
    register_flash_parameter("secondary_overcharge_protection_voltage_value", TYPE_UINT16, &init_parameter->secondary_overcharge_protection_voltage_value);
    register_flash_parameter("discharge_overcurrent1_protection_voltage_value", TYPE_UINT16, &init_parameter->discharge_overcurrent1_protection_voltage_value);
    register_flash_parameter("discharge_overcurrent1_protection_delay_value", TYPE_UINT16, &init_parameter->discharge_overcurrent1_protection_delay_value);
    register_flash_parameter("discharge_overcurrent2_protection_voltage_value", TYPE_UINT16, &init_parameter->discharge_overcurrent2_protection_voltage_value);
    register_flash_parameter("discharge_overcurrent2_protection_delay_value", TYPE_UINT16, &init_parameter->discharge_overcurrent2_protection_delay_value);
    register_flash_parameter("short_circuit_protection_voltage_value", TYPE_UINT16, &init_parameter->short_circuit_protection_voltage_value);
    register_flash_parameter("short_circuit_protection_delay_value", TYPE_UINT16, &init_parameter->short_circuit_protection_delay_value);
    register_flash_parameter("charge_overcurrent_protection_voltage_value", TYPE_UINT16, &init_parameter->charge_overcurrent_protection_voltage_value);
    register_flash_parameter("charge_overcurrent_protection_delay_value", TYPE_UINT16, &init_parameter->charge_overcurrent_protection_delay_value);
    register_flash_parameter("charge_discharge_state_detection_voltage", TYPE_UINT16, &init_parameter->charge_discharge_state_detection_voltage);
    register_flash_parameter("charging_discharg_mos_opening_delay", TYPE_UINT16, &init_parameter->charging_discharg_mos_opening_delay);
    register_flash_parameter("charge_discharge_overcurrent_self_recovery_delay", TYPE_UINT16, &init_parameter->charge_discharge_overcurrent_self_recovery_delay);
    register_flash_parameter("secondary_overcharge_protection_delay", TYPE_UINT16, &init_parameter->secondary_overcharge_protection_delay);
    register_flash_parameter("charge_over_temperature_value", TYPE_UINT16, &init_parameter->charge_over_temperature_value);
    register_flash_parameter("charge_over_temperature_recovery_value", TYPE_UINT16, &init_parameter->charge_over_temperature_recovery_value);
    register_flash_parameter("charge_under_temperature_value", TYPE_UINT16, &init_parameter->charge_under_temperature_value);
    register_flash_parameter("charge_under_temperature_recovery_value", TYPE_UINT16, &init_parameter->charge_under_temperature_recovery_value);
    register_flash_parameter("discharge_over_temperature_value", TYPE_UINT16, &init_parameter->discharge_over_temperature_value);
    register_flash_parameter("discharge_over_temperature_recovery_value", TYPE_UINT16, &init_parameter->discharge_over_temperature_recovery_value);
    register_flash_parameter("discharge_under_temperature_value", TYPE_UINT16, &init_parameter->discharge_under_temperature_value);
    register_flash_parameter("discharge_under_temperature_recovery_value", TYPE_UINT16, &init_parameter->discharge_under_temperature_recovery_value);
    register_flash_parameter("CADC_offset_value", TYPE_UINT16, &init_parameter->CADC_offset_value);
    register_flash_parameter("CADC_current_calibration_gain", TYPE_FLOAT, &init_parameter->CADC_current_calibration_gain);
    register_flash_parameter("afe_systerm1_config", TYPE_UINT8, &init_parameter->System_config_register1_00H);

    // register_flash_parameter("spare_register", TYPE_UINT8, &init_parameter->spare_register);

    register_flash_parameter("afe_systerm2_config", TYPE_UINT8, &init_parameter->System_config_register2_01H);
    register_flash_parameter("afe_systerm_ram40_config", TYPE_UINT8, &init_parameter->System_config_ram_register_40H);
    register_flash_parameter("afe_systerm_ram41_config", TYPE_UINT8, &init_parameter->System_config_ram_register_41H);
    register_flash_parameter("afe_systerm_ram42_config", TYPE_UINT8, &init_parameter->System_config_ram_register_42H);
    register_flash_parameter("afe_init_flag", TYPE_UINT8, &init_parameter->afe_init_flag);

}

/**
 * @函数名称: sh367309_init_default_flash_parameter()
 * @功能描述: 寄存器参数阈值初始化默认参数
 * @输入: init_parameter: 初始化结构体参数
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_init_default_flash_parameter(AFE_INIT* init_parameter)
{
    if (init_parameter == NULL) {
        /* 防止空指针访问 */
        return;
    }
    /* 时间单位ms，电压单位mv */
    init_parameter->cell_overvoltage_value_time = 1000;
    
    init_parameter->cell_overvoltage_value = 3800;
    init_parameter->cell_overvoltage_Load_release_delay = 200;

    init_parameter->cell_undervoltage_value_time = 1000;
    init_parameter->cell_overvoltage_recovery_value = 3380;

    init_parameter->cell_undervoltage_value = 2320;

    init_parameter->cell_undervoltage_recovery_value = 3020;

    init_parameter->balance_voltage_value = 3460;

    /* 不使用AFE的预充功能 */
    init_parameter->precharge_voltage_value = 2200;

    init_parameter->low_voltage_stop_charging_voltage_value = 1900;
    init_parameter->secondary_overcharge_protection_voltage_value = 3800;
    /* 单位0.01A 10000*0.01 = 100A  */
    init_parameter->discharge_overcurrent1_protection_voltage_value = 10000;
    init_parameter->discharge_overcurrent1_protection_delay_value = 100;
    /* 单位0.01A  15000*0.01 = 150A */
    init_parameter->discharge_overcurrent2_protection_voltage_value = 15000; 
    init_parameter->discharge_overcurrent2_protection_delay_value = 10;
    /* 单位0.01A  40000*0.01 = 400A */
    init_parameter->short_circuit_protection_voltage_value = 40000;
    init_parameter->short_circuit_protection_delay_value = 128;
    /* 单位0.01A 10000*0.01 = 100A  */
    init_parameter->charge_overcurrent_protection_voltage_value = 10000;
    init_parameter->charge_overcurrent_protection_delay_value = 100;
    /* 单位：mv */
    init_parameter->charge_discharge_state_detection_voltage = 200;
    /* 单位：us */
    init_parameter->charging_discharg_mos_opening_delay = 128;
    /* 单位：ms */
    init_parameter->charge_discharge_overcurrent_self_recovery_delay = 3200;
    /* 单位：ms */
    init_parameter->secondary_overcharge_protection_delay = 800;

    init_parameter->charge_over_temperature_value = (63 + TEMPERATURE_OFFSET);
    init_parameter->charge_over_temperature_recovery_value = (55 + TEMPERATURE_OFFSET);
    init_parameter->charge_under_temperature_value = (2 + TEMPERATURE_OFFSET);
    init_parameter->charge_under_temperature_recovery_value = (6 + TEMPERATURE_OFFSET);
    init_parameter->discharge_over_temperature_value = (63 + TEMPERATURE_OFFSET);
    init_parameter->discharge_over_temperature_recovery_value = (55 + TEMPERATURE_OFFSET);
    init_parameter->discharge_under_temperature_value = (-20 + TEMPERATURE_OFFSET);
    init_parameter->discharge_under_temperature_recovery_value = (-15 + TEMPERATURE_OFFSET);
    init_parameter->CADC_offset_value = 0;
    init_parameter->CADC_current_calibration_gain = 0;

    /* 00H寄存器默认值 */
    /* 电芯串数的配置 */
    init_parameter->System_config_register1_00H.Bits.Bit0 = (CELL_NUM & 0x1);
    init_parameter->System_config_register1_00H.Bits.Bit1 = (CELL_NUM >> 1) & 0x1;
    init_parameter->System_config_register1_00H.Bits.Bit2 = (CELL_NUM >> 2) & 0x1;
    init_parameter->System_config_register1_00H.Bits.Bit3 = (CELL_NUM >> 3) & 0x1;
    /* 开启均衡由AFE芯片逻辑控制 */
    init_parameter->System_config_register1_00H.Bits.Bit4 = 1;
    /* 充放电关闭充放电MOS */
    init_parameter->System_config_register1_00H.Bits.Bit5 = 0;
    /* 启用充电MOS恢复控制功能 */
    init_parameter->System_config_register1_00H.Bits.Bit6 = 1;
    /* 禁用预充电功能 */
    init_parameter->System_config_register1_00H.Bits.Bit7 = 0;
    // write_eeprom_parameter("afe_systerm1_config", &init_parameter->System_config_register1_00H.Byte); 
    /* 01H寄存器默认值 */
    /* 过放电保护状态释放和负载无关 */
    init_parameter->System_config_register2_01H.Bits.Bit0 = 0;
    /* 允许电流恢复保护定时恢复 */
    init_parameter->System_config_register2_01H.Bits.Bit1 = 1;
    /* CTL管脚功能设置控制位 */
    init_parameter->System_config_register2_01H.Bits.Bit2 = 1;
    init_parameter->System_config_register2_01H.Bits.Bit3 = 1;
    /* 二次过充电模块使能控制 */
    init_parameter->System_config_register2_01H.Bits.Bit4 = 1;
    /* 过放电时MOS控制位 */
    init_parameter->System_config_register2_01H.Bits.Bit5 = 0;
    /* reserved */
    init_parameter->System_config_register2_01H.Bits.Bit6 = 0;
    /* 禁止低压电芯充电功能控制位 */
    init_parameter->System_config_register2_01H.Bits.Bit7 = 1;
    // write_eeprom_parameter("afe_systerm2_config", &init_parameter->System_config_register2_01H.Byte);

    /* RAM寄存器40H */
    /* IDLE设置控制位 */
    init_parameter->System_config_ram_register_40H.Bits.Bit0 = 0;
    /* SLEEP设置控制位 */
    init_parameter->System_config_ram_register_40H.Bits.Bit1 = 0;
    /* 看门口设置控制位 */
    init_parameter->System_config_ram_register_40H.Bits.Bit2 = 0;
    /* CADC设置控制位 */
    init_parameter->System_config_ram_register_40H.Bits.Bit3 = 1;
    /* 充电MOS控制位 */
    init_parameter->System_config_ram_register_40H.Bits.Bit4 = 1;
    /* 放电MOS控制位 */
    init_parameter->System_config_ram_register_40H.Bits.Bit5 = 1;
    /* 预充电MOS控制位 */
    init_parameter->System_config_ram_register_40H.Bits.Bit6 = 1;
    /* 过流保护控制位 */
    init_parameter->System_config_ram_register_40H.Bits.Bit7 = 0;
    // write_eeprom_parameter("afe_systerm_ram40_config", &init_parameter->System_config_ram_register_40H.Byte);
    /* RAM寄存器41H */
    /* 第九节电芯的均衡状态控制 */
    init_parameter->System_config_ram_register_41H.Bits.Bit0 = 0;
    /* 第十节电芯的均衡状态控制 */
    init_parameter->System_config_ram_register_41H.Bits.Bit1 = 0;
    /* 第十一节电芯的均衡状态控制 */
    init_parameter->System_config_ram_register_41H.Bits.Bit2 = 0;
    /* 第十二节电芯的均衡状态控制 */
    init_parameter->System_config_ram_register_41H.Bits.Bit3 = 0;
    /* 第十三节电芯的均衡状态控制 */
    init_parameter->System_config_ram_register_41H.Bits.Bit4 = 0;
    /* 第十四节电芯的均衡状态控制 */
    init_parameter->System_config_ram_register_41H.Bits.Bit5 = 0;
    /* 第十五节电芯的均衡状态控制 */
    init_parameter->System_config_ram_register_41H.Bits.Bit6 = 0;
    /* 第十六节电芯的均衡状态控制 */
    init_parameter->System_config_ram_register_41H.Bits.Bit7 = 0;
    // write_eeprom_parameter("afe_systerm_ram41_config", &init_parameter->System_config_ram_register_41H.Byte);

    /* RAM寄存器42H */
    /* 第一节电芯的均衡状态控制 */
    init_parameter->System_config_ram_register_42H.Bits.Bit0 = 0;
    /* 第二节电芯的均衡状态控制 */
    init_parameter->System_config_ram_register_42H.Bits.Bit1 = 0;
    /* 第三节电芯的均衡状态控制 */
    init_parameter->System_config_ram_register_42H.Bits.Bit2 = 0;
    /* 第四节电芯的均衡状态控制 */
    init_parameter->System_config_ram_register_42H.Bits.Bit3 = 0;
    /* 第五节电芯的均衡状态控制 */
    init_parameter->System_config_ram_register_42H.Bits.Bit4 = 0;
    /* 第六节电芯的均衡状态控制 */
    init_parameter->System_config_ram_register_42H.Bits.Bit5 = 0;
    /* 第七节电芯的均衡状态控制 */
    init_parameter->System_config_ram_register_42H.Bits.Bit6 = 0;
    /* 第八节电芯的均衡状态控制 */
    init_parameter->System_config_ram_register_42H.Bits.Bit7 = 0;
    // write_eeprom_parameter("afe_systerm_ram42_config", &init_parameter->System_config_ram_register_42H.Byte);
    
    /* AFE初始化完成标志位，标志着AFE已经有默认参数配置了 */
    init_parameter->afe_init_flag = 0x00;
}

/**
 * @函数名称: battery_afe_write_default_flash_parameter()
 * @功能描述: 寄存器参数阈值初始化写入默认参数到FLASH中
 * @输入: init_parameter: 初始化结构体参数
 * @输出: 无
 * @返回值: 无
 */
void battery_afe_write_default_flash_parameter(AFE_INIT* init_parameter)
{
    if (init_parameter == NULL) {
        /* 防止空指针访问 */
        return;
    }

    uint8_t err = 0;

    /* 存储到flash中 */
    err |= write_flash_parameter("afe_systerm1_config", &init_parameter->System_config_register1_00H.Byte);

    err |= write_flash_parameter("afe_systerm2_config", &init_parameter->System_config_register2_01H.Byte);
    printf("WRITE FLASH  afe_systerm2_config = %d\r\n", init_parameter->System_config_register2_01H.Byte);
    err |= write_flash_parameter("afe_systerm_ram40_config", &init_parameter->System_config_ram_register_40H.Byte);
    err |= write_flash_parameter("afe_systerm_ram41_config", &init_parameter->System_config_ram_register_41H.Byte);
    err |= write_flash_parameter("afe_systerm_ram42_config", &init_parameter->System_config_ram_register_42H.Byte);
    err |= write_flash_parameter("cell_overvoltage_value_time", &init_parameter->cell_overvoltage_value_time);
    printf("cell_overvoltage_value_time = %d\r\n", init_parameter->cell_overvoltage_value_time);
    err |= write_flash_parameter("cell_overvoltage_value", &init_parameter->cell_overvoltage_value);
    err |= write_flash_parameter("cell_overvoltage_Load_release_delay",  &init_parameter->cell_overvoltage_Load_release_delay);
    err |= write_flash_parameter("cell_undervoltage_value_time",  &init_parameter->cell_undervoltage_value_time);
    err |= write_flash_parameter("cell_overvoltage_recovery_value",  &init_parameter->cell_overvoltage_recovery_value);
    err |= write_flash_parameter("cell_undervoltage_value",  &init_parameter->cell_undervoltage_value);
    err |= write_flash_parameter("cell_undervoltage_recovery_value", &init_parameter->cell_undervoltage_recovery_value);
    err |= write_flash_parameter("balance_voltage_value", &init_parameter->balance_voltage_value);
    err |= write_flash_parameter("precharge_voltage_value", &init_parameter->precharge_voltage_value);
    err |= write_flash_parameter("low_voltage_stop_charging_voltage_value", &init_parameter->low_voltage_stop_charging_voltage_value);
    err |= write_flash_parameter("secondary_overcharge_protection_voltage_value", &init_parameter->secondary_overcharge_protection_voltage_value);
    err |= write_flash_parameter("discharge_overcurrent1_protection_voltage_value", &init_parameter->discharge_overcurrent1_protection_voltage_value);
    err |= write_flash_parameter("discharge_overcurrent1_protection_delay_value", &init_parameter->discharge_overcurrent1_protection_delay_value);
    err |= write_flash_parameter("discharge_overcurrent2_protection_voltage_value", &init_parameter->discharge_overcurrent2_protection_voltage_value);
    err |= write_flash_parameter("discharge_overcurrent2_protection_delay_value", &init_parameter->discharge_overcurrent2_protection_delay_value);
    err |= write_flash_parameter("short_circuit_protection_voltage_value", &init_parameter->short_circuit_protection_voltage_value);
    err |= write_flash_parameter("short_circuit_protection_delay_value", &init_parameter->short_circuit_protection_delay_value);
    err |= write_flash_parameter("charge_overcurrent_protection_voltage_value", &init_parameter->charge_overcurrent_protection_voltage_value);
    err |= write_flash_parameter("charge_overcurrent_protection_delay_value", &init_parameter->charge_overcurrent_protection_delay_value);
    err |= write_flash_parameter("charge_discharge_state_detection_voltage", &init_parameter->charge_discharge_state_detection_voltage);
    err |= write_flash_parameter("charging_discharg_mos_opening_delay", &init_parameter->charging_discharg_mos_opening_delay);
    err |= write_flash_parameter("charge_discharge_overcurrent_self_recovery_delay", &init_parameter->charge_discharge_overcurrent_self_recovery_delay);
    err |= write_flash_parameter("secondary_overcharge_protection_delay", &init_parameter->secondary_overcharge_protection_delay);
    err |= write_flash_parameter("charge_over_temperature_value", &init_parameter->charge_over_temperature_value);
    err |= write_flash_parameter("charge_over_temperature_recovery_value", &init_parameter->charge_over_temperature_recovery_value);
    err |= write_flash_parameter("charge_under_temperature_value", &init_parameter->charge_under_temperature_value);
    err |= write_flash_parameter("charge_under_temperature_recovery_value", &init_parameter->charge_under_temperature_recovery_value);
    err |= write_flash_parameter("discharge_over_temperature_value", &init_parameter->discharge_over_temperature_value);
    err |= write_flash_parameter("discharge_over_temperature_recovery_value", &init_parameter->discharge_over_temperature_recovery_value);
    err |= write_flash_parameter("discharge_under_temperature_value", &init_parameter->discharge_under_temperature_value);
    err |= write_flash_parameter("discharge_under_temperature_recovery_value", &init_parameter->discharge_under_temperature_recovery_value);
    err |= write_flash_parameter("CADC_offset_value", &init_parameter->CADC_offset_value);
    err |= write_flash_parameter("CADC_current_calibration_gain", &init_parameter->CADC_current_calibration_gain);
    err |= write_flash_parameter("afe_init_flag", &init_parameter->afe_init_flag);

    if(err) {
        printf("err battery_afe_write_default_eeprom_parameter!!!\r\n");
    }
#if 0
    /* 打印调试信息 */
    BATTERY_LOG("AFE_INIT configuration/*********************************************************/:\r\n");
    BATTERY_LOG("System_config_register1_00H = %x\r\n", init_parameter->System_config_register1_00H.Byte);
    BATTERY_LOG("System_config_register2_01H = %x\r\n", init_parameter->System_config_register2_01H.Byte);
    BATTERY_LOG("cell_overvoltage_value_time = %d\r\n", init_parameter->cell_overvoltage_value_time);
    BATTERY_LOG("cell_overvoltage_value = %d\r\n", init_parameter->cell_overvoltage_value);
    BATTERY_LOG("cell_overvoltage_Load_release_delay = %d\r\n", init_parameter->cell_overvoltage_Load_release_delay);
    BATTERY_LOG("cell_undervoltage_value_time = %d\r\n", init_parameter->cell_undervoltage_value_time);
    BATTERY_LOG("cell_overvoltage_recovery_value = %d\r\n", init_parameter->cell_overvoltage_recovery_value);
    BATTERY_LOG("cell_undervoltage_value = %d\r\n", init_parameter->cell_undervoltage_value);
    BATTERY_LOG("cell_undervoltage_recovery_value = %d\r\n", init_parameter->cell_undervoltage_recovery_value);
    BATTERY_LOG("balance_voltage_value = %d\r\n", init_parameter->balance_voltage_value);
    BATTERY_LOG("precharge_voltage_value = %d\r\n", init_parameter->precharge_voltage_value);
    BATTERY_LOG("low_voltage_stop_charging_voltage_value = %d\r\n", init_parameter->low_voltage_stop_charging_voltage_value);
    BATTERY_LOG("secondary_overcharge_protection_voltage_value = %d\r\n", init_parameter->secondary_overcharge_protection_voltage_value);
    BATTERY_LOG("discharge_overcurrent1_protection_voltage_value = %d\r\n", init_parameter->discharge_overcurrent1_protection_voltage_value);
    BATTERY_LOG("discharge_overcurrent1_protection_delay_value = %d\r\n", init_parameter->discharge_overcurrent1_protection_delay_value);
    BATTERY_LOG("discharge_overcurrent2_protection_voltage_value = %d\r\n", init_parameter->discharge_overcurrent2_protection_voltage_value);
    BATTERY_LOG("discharge_overcurrent2_protection_delay_value = %d\r\n", init_parameter->discharge_overcurrent2_protection_delay_value);
    BATTERY_LOG("short_circuit_protection_voltage_value = %d\r\n", init_parameter->short_circuit_protection_voltage_value);
    BATTERY_LOG("short_circuit_protection_delay_value = %d\r\n", init_parameter->short_circuit_protection_delay_value);
    BATTERY_LOG("charge_overcurrent_protection_voltage_value = %d\r\n", init_parameter->charge_overcurrent_protection_voltage_value);
    BATTERY_LOG("charge_overcurrent_protection_delay_value = %d\r\n", init_parameter->charge_overcurrent_protection_delay_value);
    BATTERY_LOG("charge_discharge_state_detection_voltage = %d\r\n", init_parameter->charge_discharge_state_detection_voltage);
    BATTERY_LOG("charging_discharg_mos_opening_delay = %d\r\n", init_parameter->charging_discharg_mos_opening_delay);
    BATTERY_LOG("charge_discharge_overcurrent_self_recovery_delay = %d\r\n", init_parameter->charge_discharge_overcurrent_self_recovery_delay);
    BATTERY_LOG("secondary_overcharge_protection_delay = %d\r\n", init_parameter->secondary_overcharge_protection_delay);
    BATTERY_LOG("charge_over_temperature_value = %d\r\n", init_parameter->charge_over_temperature_value);
    BATTERY_LOG("charge_over_temperature_recovery_value = %d\r\n", init_parameter->charge_over_temperature_recovery_value);
    BATTERY_LOG("charge_under_temperature_value = %d\r\n", init_parameter->charge_under_temperature_value);
    BATTERY_LOG("charge_under_temperature_recovery_value = %d\r\n", init_parameter->charge_under_temperature_recovery_value);
    BATTERY_LOG("discharge_over_temperature_value = %d\r\n", init_parameter->discharge_over_temperature_value);
    BATTERY_LOG("discharge_over_temperature_recovery_value = %d\r\n", init_parameter->discharge_over_temperature_recovery_value);
    BATTERY_LOG("discharge_under_temperature_value = %d\r\n", init_parameter->discharge_under_temperature_value);
    BATTERY_LOG("discharge_under_temperature_recovery_value = %d\r\n", init_parameter->discharge_under_temperature_recovery_value);
    BATTERY_LOG("System_config_ram_register_40H = %d\r\n", init_parameter->System_config_ram_register_40H.Byte);
    BATTERY_LOG("System_config_ram_register_41H = %d\r\n", init_parameter->System_config_ram_register_41H.Byte);
    BATTERY_LOG("System_config_ram_register_42H = %d\r\n", init_parameter->System_config_ram_register_42H.Byte);
    BATTERY_LOG("CADC_offset_value = %d\r\n", init_parameter->CADC_offset_value);
    BATTERY_LOG("CADC_current_calibration_gain = %f\r\n", init_parameter->CADC_current_calibration_gain);

#endif
}

/**
 * @函数名称: battery_afe_read_init_register_config()
 * @功能描述: 从FLASH中读取AFE初始化寄存器配置
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void battery_afe_read_init_register_config(AFE_INIT *afe_init)
{
    if (afe_init == NULL) {
        /* 防止空指针访问 */
        return;
    }
    uint8_t err = 0;
    /* 初始化结构体中的所有字段为 0 */
    memset(afe_init, 0, sizeof(AFE_INIT));

    /* 从 FLASH 中读取参数，并赋值到结构体的相应字段 */
    // read_flash_parameter("afe_systerm1_config", &afe_init->System_config_register1_00H.Byte);
    err |= read_flash_parameter("afe_systerm1_config", &afe_init->System_config_register1_00H.Byte);

    err |= read_flash_parameter("afe_systerm2_config", &afe_init->System_config_register2_01H.Byte);
    err |= read_flash_parameter("cell_overvoltage_value_time", &afe_init->cell_overvoltage_value_time);

    err |= read_flash_parameter("cell_overvoltage_value", &afe_init->cell_overvoltage_value);
    err |= read_flash_parameter("cell_overvoltage_Load_release_delay", &afe_init->cell_overvoltage_Load_release_delay);
    err |= read_flash_parameter("cell_undervoltage_value_time", &afe_init->cell_undervoltage_value_time);
    err |= read_flash_parameter("cell_overvoltage_recovery_value", &afe_init->cell_overvoltage_recovery_value);
    err |= read_flash_parameter("cell_undervoltage_value", &afe_init->cell_undervoltage_value);
    err |= read_flash_parameter("cell_undervoltage_recovery_value", &afe_init->cell_undervoltage_recovery_value);
    err |= read_flash_parameter("balance_voltage_value", &afe_init->balance_voltage_value);
    err |= read_flash_parameter("precharge_voltage_value", &afe_init->precharge_voltage_value);
    err |= read_flash_parameter("low_voltage_stop_charging_voltage_value", &afe_init->low_voltage_stop_charging_voltage_value);
    err |= read_flash_parameter("secondary_overcharge_protection_voltage_value", &afe_init->secondary_overcharge_protection_voltage_value);
    err |= read_flash_parameter("discharge_overcurrent1_protection_voltage_value", &afe_init->discharge_overcurrent1_protection_voltage_value);
    err |= read_flash_parameter("discharge_overcurrent1_protection_delay_value", &afe_init->discharge_overcurrent1_protection_delay_value);
    err |= read_flash_parameter("discharge_overcurrent2_protection_voltage_value", &afe_init->discharge_overcurrent2_protection_voltage_value);
    err |= read_flash_parameter("discharge_overcurrent2_protection_delay_value", &afe_init->discharge_overcurrent2_protection_delay_value);
    err |= read_flash_parameter("short_circuit_protection_voltage_value", &afe_init->short_circuit_protection_voltage_value);
    err |= read_flash_parameter("short_circuit_protection_delay_value", &afe_init->short_circuit_protection_delay_value);
    err |= read_flash_parameter("charge_overcurrent_protection_voltage_value", &afe_init->charge_overcurrent_protection_voltage_value);
    err |= read_flash_parameter("charge_overcurrent_protection_delay_value", &afe_init->charge_overcurrent_protection_delay_value);
    err |= read_flash_parameter("charge_discharge_state_detection_voltage", &afe_init->charge_discharge_state_detection_voltage);
    err |= read_flash_parameter("charging_discharg_mos_opening_delay", &afe_init->charging_discharg_mos_opening_delay);
    err |= read_flash_parameter("charge_discharge_overcurrent_self_recovery_delay", &afe_init->charge_discharge_overcurrent_self_recovery_delay);
    err |= read_flash_parameter("secondary_overcharge_protection_delay", &afe_init->secondary_overcharge_protection_delay);
    err |= read_flash_parameter("charge_over_temperature_value", &afe_init->charge_over_temperature_value);
    err |= read_flash_parameter("charge_over_temperature_recovery_value", &afe_init->charge_over_temperature_recovery_value);
    err |= read_flash_parameter("charge_under_temperature_value", &afe_init->charge_under_temperature_value);
    err |= read_flash_parameter("charge_under_temperature_recovery_value", &afe_init->charge_under_temperature_recovery_value);
    err |= read_flash_parameter("discharge_over_temperature_value", &afe_init->discharge_over_temperature_value);
    err |= read_flash_parameter("discharge_over_temperature_recovery_value", &afe_init->discharge_over_temperature_recovery_value);
    err |= read_flash_parameter("discharge_under_temperature_value", &afe_init->discharge_under_temperature_value);
    err |= read_flash_parameter("discharge_under_temperature_recovery_value", &afe_init->discharge_under_temperature_recovery_value);
    err |= read_flash_parameter("afe_systerm_ram40_config", &afe_init->System_config_ram_register_40H.Byte);
    err |= read_flash_parameter("afe_systerm_ram41_config", &afe_init->System_config_ram_register_41H.Byte);
    err |= read_flash_parameter("afe_systerm_ram42_config", &afe_init->System_config_ram_register_42H.Byte);
    err |= read_flash_parameter("CADC_offset_value", &afe_init->CADC_offset_value);
    err |= read_flash_parameter("CADC_current_calibration_gain", &afe_init->CADC_current_calibration_gain);
    if(err) {
        printf("err battery_afe_read_init_register_config!!!\r\n");
    }
#if 0
    /* 打印调试信息 */
    BATTERY_LOG("AFE_INIT configuration:\r\n");
    BATTERY_LOG("System_config_register1_00H = %x\r\n", afe_init->System_config_register1_00H.Byte);
    BATTERY_LOG("System_config_register2_01H = %x\r\n", afe_init->System_config_register2_01H.Byte);
    BATTERY_LOG("cell_overvoltage_value_time = %d\r\n", afe_init->cell_overvoltage_value_time);
    BATTERY_LOG("cell_overvoltage_value = %d\r\n", afe_init->cell_overvoltage_value);
    BATTERY_LOG("cell_overvoltage_Load_release_delay = %d\r\n", afe_init->cell_overvoltage_Load_release_delay);
    BATTERY_LOG("cell_undervoltage_value_time = %d\r\n", afe_init->cell_undervoltage_value_time);
    BATTERY_LOG("cell_overvoltage_recovery_value = %d\r\n", afe_init->cell_overvoltage_recovery_value);
    BATTERY_LOG("cell_undervoltage_value = %d\r\n", afe_init->cell_undervoltage_value);
    BATTERY_LOG("cell_undervoltage_recovery_value = %d\r\n", afe_init->cell_undervoltage_recovery_value);
    BATTERY_LOG("balance_voltage_value = %d\r\n", afe_init->balance_voltage_value);
    BATTERY_LOG("precharge_voltage_value = %d\r\n", afe_init->precharge_voltage_value);
    BATTERY_LOG("low_voltage_stop_charging_voltage_value = %d\r\n", afe_init->low_voltage_stop_charging_voltage_value);
    BATTERY_LOG("secondary_overcharge_protection_voltage_value = %d\r\n", afe_init->secondary_overcharge_protection_voltage_value);
    BATTERY_LOG("discharge_overcurrent1_protection_voltage_value = %d\r\n", afe_init->discharge_overcurrent1_protection_voltage_value);
    BATTERY_LOG("discharge_overcurrent1_protection_delay_value = %d\r\n", afe_init->discharge_overcurrent1_protection_delay_value);
    BATTERY_LOG("discharge_overcurrent2_protection_voltage_value = %d\r\n", afe_init->discharge_overcurrent2_protection_voltage_value);
    BATTERY_LOG("discharge_overcurrent2_protection_delay_value = %d\r\n", afe_init->discharge_overcurrent2_protection_delay_value);
    BATTERY_LOG("short_circuit_protection_voltage_value = %d\r\n", afe_init->short_circuit_protection_voltage_value);
    BATTERY_LOG("short_circuit_protection_delay_value = %d\r\n", afe_init->short_circuit_protection_delay_value);
    BATTERY_LOG("charge_overcurrent_protection_voltage_value = %d\r\n", afe_init->charge_overcurrent_protection_voltage_value);
    BATTERY_LOG("charge_overcurrent_protection_delay_value = %d\r\n", afe_init->charge_overcurrent_protection_delay_value);
    BATTERY_LOG("charge_discharge_state_detection_voltage = %d\r\n", afe_init->charge_discharge_state_detection_voltage);
    BATTERY_LOG("charging_discharg_mos_opening_delay = %d\r\n", afe_init->charging_discharg_mos_opening_delay);
    BATTERY_LOG("charge_discharge_overcurrent_self_recovery_delay = %d\r\n", afe_init->charge_discharge_overcurrent_self_recovery_delay);
    BATTERY_LOG("secondary_overcharge_protection_delay = %d\r\n", afe_init->secondary_overcharge_protection_delay);
    BATTERY_LOG("charge_over_temperature_value = %d\r\n", afe_init->charge_over_temperature_value);
    BATTERY_LOG("charge_over_temperature_recovery_value = %d\r\n", afe_init->charge_over_temperature_recovery_value);
    BATTERY_LOG("charge_under_temperature_value = %d\r\n", afe_init->charge_under_temperature_value);
    BATTERY_LOG("charge_under_temperature_recovery_value = %d\r\n", afe_init->charge_under_temperature_recovery_value);
    BATTERY_LOG("discharge_over_temperature_value = %d\r\n", afe_init->discharge_over_temperature_value);
    BATTERY_LOG("discharge_over_temperature_recovery_value = %d\r\n", afe_init->discharge_over_temperature_recovery_value);
    BATTERY_LOG("discharge_under_temperature_value = %d\r\n", afe_init->discharge_under_temperature_value);
    BATTERY_LOG("discharge_under_temperature_recovery_value = %d\r\n", afe_init->discharge_under_temperature_recovery_value);
    BATTERY_LOG("System_config_ram_register_40H = %d\r\n", afe_init->System_config_ram_register_40H.Byte);
    BATTERY_LOG("System_config_ram_register_41H = %d\r\n", afe_init->System_config_ram_register_41H.Byte);
    BATTERY_LOG("System_config_ram_register_42H = %d\r\n", afe_init->System_config_ram_register_42H.Byte);
    BATTERY_LOG("CADC_offset_value = %d\r\n", afe_init->CADC_offset_value);
    BATTERY_LOG("CADC_current_calibration_gain = %f\r\n", afe_init->CADC_current_calibration_gain);

#endif
}

/**
 * @函数名称: sh367309_chip_systerm1_config()
 * @功能描述: 系统配置1设置
 * @输入: register_value: 寄存器值 init_parameter: 初始化结构体参数
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_chip_systerm1_config(uByteBits* register_value, AFE_INIT* init_parameter)
{
    /* 串数配置控制 */
    register_value->Bits.Bit0 = init_parameter->System_config_register1_00H.Bits.Bit0;
    register_value->Bits.Bit1 = init_parameter->System_config_register1_00H.Bits.Bit1;
    register_value->Bits.Bit2 = init_parameter->System_config_register1_00H.Bits.Bit2;
    register_value->Bits.Bit3 = init_parameter->System_config_register1_00H.Bits.Bit3;
    /* 平衡功能模块是能控制位 */
    register_value->Bits.Bit4 = init_parameter->System_config_register1_00H.Bits.Bit4;
    /* 充放电过流MOS控制位 */
    register_value->Bits.Bit5 = init_parameter->System_config_register1_00H.Bits.Bit5;
    /* 充电MOS恢复控制位 */
    register_value->Bits.Bit6 = init_parameter->System_config_register1_00H.Bits.Bit6;
    /* 预充电模块控制位 */
    register_value->Bits.Bit7 = init_parameter->System_config_register1_00H.Bits.Bit7;
}

/**
 * @函数名称: sh367309_chip_systerm2_config()
 * @功能描述: 系统配置2设置
 * @输入: register_value: 寄存器值 init_parameter: 初始化结构体参数
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_chip_systerm2_config(uByteBits* register_value, AFE_INIT* init_parameter)
{
    /* 过放电保护状态释放和负载无关 */
    register_value->Bits.Bit0 = init_parameter->System_config_register2_01H.Bits.Bit0;
    /* 允许电流恢复保护定时恢复 */
    register_value->Bits.Bit1 = init_parameter->System_config_register2_01H.Bits.Bit1;
    /* CTL管脚功能设置控制位 */
    register_value->Bits.Bit2 = init_parameter->System_config_register2_01H.Bits.Bit2;
    register_value->Bits.Bit3 = init_parameter->System_config_register2_01H.Bits.Bit3;
    /* 二次过充电模块使能控制 */
    register_value->Bits.Bit4 = init_parameter->System_config_register2_01H.Bits.Bit4;
    /* 过放电时MOS控制位 */
    register_value->Bits.Bit5 = init_parameter->System_config_register2_01H.Bits.Bit5;
    /* reserved */
    register_value->Bits.Bit6 = init_parameter->System_config_register2_01H.Bits.Bit6;
    /* 禁止低压电芯充电功能控制位 */
    register_value->Bits.Bit7 = init_parameter->System_config_register2_01H.Bits.Bit7;
}

/**
 * @函数名称: sh367309_chip_system40_config()
 * @功能描述: 系统配置40设置
 * @输入: register_value: 寄存器值 init_parameter: 初始化结构体参数
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_chip_system40_config(uByteBits* register_value, AFE_INIT* init_parameter)
{
    /* IDLE设置控制位 */
    register_value->Bits.Bit0 = (init_parameter->System_config_ram_register_40H.Bits.Bit0);
    /* SLEEP设置控制位 */
    register_value->Bits.Bit1 = (init_parameter->System_config_ram_register_40H.Bits.Bit1);
    /* 看门口设置控制位 */
    register_value->Bits.Bit2 = (init_parameter->System_config_ram_register_40H.Bits.Bit2);
    /* CADC设置控制位 */
    register_value->Bits.Bit3 = (init_parameter->System_config_ram_register_40H.Bits.Bit3);
    /* 充电MOS控制位 */
    register_value->Bits.Bit4 = (init_parameter->System_config_ram_register_40H.Bits.Bit4);
    /* 放电MOS控制位 */
    register_value->Bits.Bit5 = (init_parameter->System_config_ram_register_40H.Bits.Bit5);
    /* 预充电MOS控制位 */
    register_value->Bits.Bit6 = (init_parameter->System_config_ram_register_40H.Bits.Bit6);
    /* 过流保护控制位 */
    register_value->Bits.Bit7 = (init_parameter->System_config_ram_register_40H.Bits.Bit7);

    // write_eeprom_parameter("afe_systerm_ram40_config", &register_value->Byte);
}

/**
 * @函数名称: sh367309_chip_system41_config()
 * @功能描述: 系统配置41设置
 * @输入: register_value: 寄存器值 init_parameter: 初始化结构体参数
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_chip_system41_config(uByteBits* register_value, AFE_INIT* init_parameter)
{
    /* 第九节电芯的均衡状态控制 */
    register_value->Bits.Bit0 = (init_parameter->System_config_ram_register_41H.Bits.Bit0);
    /* 第十节电芯的均衡状态控制 */
    register_value->Bits.Bit1 = (init_parameter->System_config_ram_register_41H.Bits.Bit1);
    /* 第十一节电芯的均衡状态控制 */
    register_value->Bits.Bit2 = (init_parameter->System_config_ram_register_41H.Bits.Bit2);
    /* 第十二节电芯的均衡状态控制 */
    register_value->Bits.Bit3 = (init_parameter->System_config_ram_register_41H.Bits.Bit3);
    /* 第十三节电芯的均衡状态控制 */
    register_value->Bits.Bit4 = (init_parameter->System_config_ram_register_41H.Bits.Bit4);
    /* 第十四节电芯的均衡状态控制 */
    register_value->Bits.Bit5 = (init_parameter->System_config_ram_register_41H.Bits.Bit5);
    /* 第十五节电芯的均衡状态控制 */
    register_value->Bits.Bit6 = (init_parameter->System_config_ram_register_41H.Bits.Bit6);
    /* 第十六节电芯的均衡状态控制 */
    register_value->Bits.Bit7 = (init_parameter->System_config_ram_register_41H.Bits.Bit7);
}

/**
 * @函数名称: sh367309_chip_system42_config()
 * @功能描述: 系统配置42设置
 * @输入: register_value: 寄存器值 init_parameter: 初始化结构体参数
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_chip_system42_config(uByteBits* register_value, AFE_INIT* init_parameter)
{
    /* 第一节电芯的均衡状态控制 */
    register_value->Bits.Bit0 = (init_parameter->System_config_ram_register_42H.Bits.Bit0);
    /* 第二节电芯的均衡状态控制 */
    register_value->Bits.Bit1 = (init_parameter->System_config_ram_register_42H.Bits.Bit1);
    /* 第三节电芯的均衡状态控制 */
    register_value->Bits.Bit2 = (init_parameter->System_config_ram_register_42H.Bits.Bit2);
    /* 第四节电芯的均衡状态控制 */
    register_value->Bits.Bit3 = (init_parameter->System_config_ram_register_42H.Bits.Bit3);
    /* 第五节电芯的均衡状态控制 */
    register_value->Bits.Bit4 = (init_parameter->System_config_ram_register_42H.Bits.Bit4);
    /* 第六节电芯的均衡状态控制 */
    register_value->Bits.Bit5 = (init_parameter->System_config_ram_register_42H.Bits.Bit5);
    /* 第七节电芯的均衡状态控制 */
    register_value->Bits.Bit6 = (init_parameter->System_config_ram_register_42H.Bits.Bit6);
    /* 第八节电芯的均衡状态控制 */
    register_value->Bits.Bit7 = (init_parameter->System_config_ram_register_42H.Bits.Bit7);
}

/**
 * @函数名称: sh367309_charge_discharge_overvoltage_protect_time_set()
 * @功能描述: 充放电过压告警持续时间设置
 * @输入: register_value: 寄存器值 time: 延时时间
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_charge_discharge_overvoltage_protect_time_set(uByteBits* register_value, uint16_t time)
{
    if(time <= 100) {
        register_value->Byte = 0x00;
    } else if((time > 100)&&(time <= 200)) {
        register_value->Byte = 0x01 << 4;
    } else if((time > 200)&&(time <= 300)) {
        register_value->Byte = 0x02 << 4;
    } else if((time > 300)&&(time <= 400)) {
        register_value->Byte = 0x03 << 4;
    } else if((time > 400)&&(time <= 600)) {
        register_value->Byte = 0x04 << 4;
    } else if((time > 600)&&(time <= 800)) {
        register_value->Byte = 0x05<<4;
    } else if((time > 800)&&(time <= 1000)) {
        register_value->Byte = 0x06<<4;
    } else if((time > 1000)&&(time <= 2000)) {
        register_value->Byte = 0x07<<4;
    } else if((time > 2000)&&(time <= 3000)) {
        register_value->Byte = 0x08<<4;
    } else if((time > 3000)&&(time <= 4000)) {
        register_value->Byte = 0x09<<4;
    } else if((time > 4000)&&(time <= 6000)) {
        register_value->Byte = 0x0A<<4;
    } else if((time > 6000)&&(time <= 8000)) {
        register_value->Byte = 0x0B<<4;
    } else if((time > 8000)&&(time <= 10000)) {
        register_value->Byte = 0x0C<<4;
    } else if((time > 10000)&&(time <= 20000)) {
        register_value->Byte = 0x0D<<4;
    } else if((time > 20000)&&(time <= 30000)) {
        register_value->Byte = 0x0E<<4;
    } else if(time > 30000) {
        register_value->Byte = 0x0F<<4;
    }
}

/**
 * @函数名称: sh367309_overvoltage_Load_release_delay_set()
 * @功能描述: 单体过压负载释放延时设置
 * @输入: register_value: 寄存器值 time: 延时时间
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_overvoltage_Load_release_delay_set(uByteBits* register_value, uint16_t time)
{
    if(time <= 100) {
        register_value->Bits.Bit2 = 0;
        register_value->Bits.Bit3 = 0;
    } else if((time > 100)&&(time <= 500)) {
        register_value->Bits.Bit2 = 1;
        register_value->Bits.Bit3 = 0;
    } else if((time > 500)&&(time <= 1000)) {
        register_value->Bits.Bit2 = 0;
        register_value->Bits.Bit3 = 1;
    } else if(time > 1000) {
        register_value->Bits.Bit2 = 1;
        register_value->Bits.Bit3 = 1;
    } 
}

/**
 * @函数名称: sh367309_discharge_overcurrent1_protection_voltage_value_set()
 * @功能描述: 放电过流1保护电压值设置
 * @输入: register_value: 寄存器值 current_value: 电流值
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_discharge_overcurrent1_protection_voltage_value_set(uByteBits* register_value, uint16_t current_value)
{
    uint32_t voltage_value = 0;
    /* 使用欧姆定律计算电压 */
    voltage_value = SH367309_CURRENT_SAMPLING_RESISTANCE*current_value;
    voltage_value = voltage_value/1000;
    if(register_value != NULL) {
        if(voltage_value < 30) {
            register_value->Byte |= 0 << 4;
        } else if((voltage_value >= 30)&&(voltage_value < 40)) {
            register_value->Byte = 0x01<<4;
        } else if((voltage_value >= 40)&&(voltage_value < 50)) {
            register_value->Byte = 0x02<<4;
        } else if((voltage_value >= 50)&&(voltage_value < 60)) {
            register_value->Byte = 0x03<<4;
        } else if((voltage_value >= 60)&&(voltage_value < 70)) {
            register_value->Byte = 0x04<<4;
        } else if((voltage_value >= 70)&&(voltage_value < 80)) {
            register_value->Byte = 0x05<<4;
        } else if((voltage_value >= 80)&&(voltage_value < 90)) {
            register_value->Byte = 0x06<<4;
        } else if((voltage_value >= 90)&&(voltage_value < 100)) {
            register_value->Byte = 0x07<<4;
        } else if((voltage_value >= 100)&&(voltage_value < 110)) {
            register_value->Byte = 0x08<<4;
        } else if((voltage_value >= 110)&&(voltage_value < 120)) {
            register_value->Byte = 0x09<<4;
        } else if((voltage_value >= 120)&&(voltage_value < 130)) {
            register_value->Byte = 0x0A<<4;
        } else if((voltage_value >= 130)&&(voltage_value < 140)) {
            register_value->Byte = 0x0B<<4;
        } else if((voltage_value >= 140)&&(voltage_value < 160)) {
            register_value->Byte = 0x0C<<4;
        } else if((voltage_value >= 160)&&(voltage_value < 180)) {
            register_value->Byte = 0x0D<<4;
        } else if((voltage_value >= 180)&&(voltage_value < 200)) {
            register_value->Byte = 0x0E<<4;
        } else if(voltage_value >= 200) {
            register_value->Byte = 0x0F<<4;
        }
    }
}

/**
 * @函数名称: sh367309_discharge_overcurrent1_protection_delay_value_set()
 * @功能描述: 放电过流1保护延时设置
 * @输入: register_value: 寄存器值 time: 延时时间
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_discharge_overcurrent1_protection_delay_value_set(uByteBits* register_value, uint16_t time)
{ 
    if(time <= 50) {
        register_value->Byte |= 0; 
    } else if((time > 50)&&(time <= 100)) {
        register_value->Byte |= 0x01; 
    } else if((time > 100)&&(time <= 200)) {
        register_value->Byte |= 0x02; 
    } else if((time > 200)&&(time <= 400)) {
        register_value->Byte |= 0x03; 
    } else if((time > 400)&&(time <= 600)) {
        register_value->Byte |= 0x04; 
    } else if((time > 600)&&(time <= 800)) {
        register_value->Byte |= 0x05; 
    } else if((time > 800)&&(time <= 1000)) {
        register_value->Byte |= 0x06; 
    } else if((time > 1000)&&(time <= 2000)) {
        register_value->Byte |= 0x07; 
    } else if((time > 2000)&&(time <= 4000)) {
        register_value->Byte |= 0x08; 
    } else if((time > 4000)&&(time <= 6000)) {
        register_value->Byte |= 0x09; 
    } else if((time > 6000)&&(time <= 8000)) {
        register_value->Byte |= 0x0A; 
    } else if((time > 8000)&&(time <= 10000)) {
        register_value->Byte |= 0x0B; 
    } else if((time > 10000)&&(time <= 15000)) {
        register_value->Byte |= 0x0C; 
    } else if((time > 15000)&&(time <= 20000)) {
        register_value->Byte |= 0x0D; 
    } else if((time > 20000)&&(time <= 30000)) {
        register_value->Byte |= 0x0E; 
    } else if(time > 30000) {
        register_value->Byte |= 0x0F; 
    }
}

/**
 * @函数名称: sh367309_discharge_overcurrent2_protection_voltage_value_set()
 * @功能描述: 放电过流2保护电压值设置
 * @输入: register_value: 寄存器值 current_value: 电流值
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_discharge_overcurrent2_protection_voltage_value_set(uByteBits* register_value, uint16_t current_value)
{
    uint32_t voltage_value = 0;
    voltage_value = SH367309_CURRENT_SAMPLING_RESISTANCE*current_value;
    voltage_value = voltage_value/1000;
    if(register_value != NULL) { 
        if(voltage_value < 40) {
            register_value->Byte = 0; 
        } else if((voltage_value >= 40)&&(voltage_value < 50)) {
            register_value->Byte = 0x01<<4; 
        } else if((voltage_value >= 50)&&(voltage_value < 60)) {
            register_value->Byte = 0x02<<4; 
        } else if((voltage_value >= 60)&&(voltage_value < 70)) {
            register_value->Byte = 0x03<<4; 
        } else if((voltage_value >= 70)&&(voltage_value < 80)) {
            register_value->Byte = 0x04<<4; 
        } else if((voltage_value >= 80)&&(voltage_value < 90)) {
            register_value->Byte = 0x05<<4; 
        } else if((voltage_value >= 90)&&(voltage_value < 100)) {
            register_value->Byte = 0x06<<4; 
        } else if((voltage_value >= 100)&&(voltage_value < 120)) {
            register_value->Byte = 0x07<<4; 
        } else if((voltage_value >= 120)&&(voltage_value < 140)) {
            register_value->Byte = 0x08<<4; 
        } else if((voltage_value >= 140)&&(voltage_value < 160)) {
            register_value->Byte = 0x09<<4; 
        } else if((voltage_value >= 160)&&(voltage_value < 180)) {
            register_value->Byte = 0x0A<<4; 
        } else if((voltage_value >= 180)&&(voltage_value < 200)) {
            register_value->Byte = 0x0B<<4; 
        } else if((voltage_value >= 200)&&(voltage_value < 300)) {
            register_value->Byte = 0x0C<<4; 
        } else if((voltage_value >= 300)&&(voltage_value < 400)) {
            register_value->Byte = 0x0D<<4; 
        } else if((voltage_value >= 400)&&(voltage_value < 500)) {
            register_value->Byte = 0x0E<<4; 
        }else if(voltage_value >= 500) {
            register_value->Byte = 0x0F<<4; 
        }
    }
}

/**
 * @函数名称: sh367309_discharge_overcurrent2_protection_delay_value_set()
 * @功能描述: 放电过流2保护延时设置
 * @输入: register_value: 寄存器值 time: 延时时间
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_discharge_overcurrent2_protection_delay_value_set(uByteBits* register_value, uint16_t time)
{ 
    if(time < 20) {
        register_value->Byte |= 0; 
    } else if((time >= 20)&&(time < 40)) {
        register_value->Byte |= 0x01; 
    } else if((time >= 40)&&(time < 60)) {
        register_value->Byte |= 0x02; 
    } else if((time >= 60)&&(time < 80)) {
        register_value->Byte |= 0x03; 
    } else if((time >= 80)&&(time < 100)) {
        register_value->Byte |= 0x04; 
    } else if((time >= 100)&&(time < 200)) {
        register_value->Byte |= 0x05; 
    } else if((time >= 200)&&(time < 400)) {
        register_value->Byte |= 0x06; 
    } else if((time >= 400)&&(time < 600)) {
        register_value->Byte |= 0x07; 
    } else if((time >= 600)&&(time < 800)) {
        register_value->Byte |= 0x08; 
    } else if((time >= 800)&&(time < 1000)) {
        register_value->Byte |= 0x09; 
    } else if((time >= 1000)&&(time < 2000)) {
        register_value->Byte |= 0x0A; 
    } else if((time >= 2000)&&(time < 4000)) {
        register_value->Byte |= 0x0B; 
    } else if((time >= 4000)&&(time < 8000)) {
        register_value->Byte |= 0x0C; 
    } else if((time >= 8000)&&(time < 10000)) {
        register_value->Byte |= 0x0D; 
    } else if((time >= 10000)&&(time < 20000)) {
        register_value->Byte |= 0x0E; 
    } else if(time >= 20000) {
        register_value->Byte |= 0x0F; 
    }
}

/**
 * @函数名称: sh367309_short_circuit_protection_voltage_value_set()
 * @功能描述: 短路保护电压值设置
 * @输入: register_value: 寄存器值 current_value: 电流值
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_short_circuit_protection_voltage_value_set(uByteBits* register_value, uint16_t current_value)
{
    uint32_t voltage_value = 0;
    voltage_value = SH367309_CURRENT_SAMPLING_RESISTANCE*current_value;
    voltage_value = voltage_value/1000;
    if(register_value != NULL) { 
        if((voltage_value >= 50)&&(voltage_value < 80)) {
            register_value->Byte = 0; 
        } else if((voltage_value >= 80)&&(voltage_value < 110)) {
            register_value->Byte = 0x01<<4; 
        } else if((voltage_value >= 110)&&(voltage_value < 140)) {
            register_value->Byte = 0x02<<4; 
        } else if((voltage_value >= 140)&&(voltage_value < 170)) {
            register_value->Byte = 0x03<<4; 
        } else if((voltage_value >= 170)&&(voltage_value < 200)) {
            register_value->Byte = 0x04<<4; 
        } else if((voltage_value >= 200)&&(voltage_value < 230)) {
            register_value->Byte = 0x05<<4; 
        } else if((voltage_value >= 230)&&(voltage_value < 260)) {
            register_value->Byte = 0x06<<4; 
        } else if((voltage_value >= 260)&&(voltage_value < 290)) {
            register_value->Byte = 0x07<<4; 
        } else if((voltage_value >= 290)&&(voltage_value < 320)) {
            register_value->Byte = 0x08<<4; 
        } else if((voltage_value >= 320)&&(voltage_value < 350)) {
            register_value->Byte = 0x09<<4; 
        } else if((voltage_value >= 350)&&(voltage_value < 400)) {
            register_value->Byte = 0x0A<<4; 
        } else if((voltage_value >= 400)&&(voltage_value < 500)) {
            register_value->Byte = 0x0B<<4; 
        } else if((voltage_value >= 500)&&(voltage_value < 600)) {
            register_value->Byte = 0x0C<<4; 
        } else if((voltage_value >= 600)&&(voltage_value < 800)) {
            register_value->Byte = 0x0D<<4; 
        } else if((voltage_value >= 500)&&(voltage_value < 1000)) {
            register_value->Byte = 0x0E<<4; 
        } else if(voltage_value >= 1000) {
            register_value->Byte = 0x0F<<4; 
        }
    }
}

/**
 * @函数名称: sh367309_short_circuit_protection_delay_value_set()
 * @功能描述: 短路保护延时设置
 * @输入: register_value: 寄存器值 time: 延时时间
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_short_circuit_protection_delay_value_set(uByteBits* register_value, uint16_t time)
{ 
    if(time <64) {
        register_value->Byte |= 0; 
    } else if((time >= 64)&&(time < 128)) {
        register_value->Byte |= 0x01; 
    } else if((time >= 128)&&(time < 192)) {
        register_value->Byte |= 0x02; 
    } else if((time >= 192)&&(time < 256)) {
        register_value->Byte |= 0x03; 
    } else if((time >= 256)&&(time < 320)) {
        register_value->Byte |= 0x04; 
    } else if((time >= 320)&&(time < 384)) {
        register_value->Byte |= 0x05; 
    } else if((time >= 384)&&(time < 448)) {
        register_value->Byte |= 0x06; 
    } else if((time >= 448)&&(time < 512)) {
        register_value->Byte |= 0x07; 
    } else if((time >= 512)&&(time < 576)) {
        register_value->Byte |= 0x08; 
    } else if((time >= 576)&&(time < 640)) {
        register_value->Byte |= 0x09; 
    } else if((time >= 640)&&(time < 704)) {
        register_value->Byte |= 0x0A; 
    } else if((time >= 704)&&(time < 768)) {
        register_value->Byte |= 0x0B; 
    } else if((time >= 768)&&(time < 832)) {
        register_value->Byte |= 0x0C; 
    } else if((time >= 832)&&(time < 896)) {
        register_value->Byte |= 0x0D; 
    } else if((time >= 896)&&(time < 960)) {
        register_value->Byte |= 0x0E; 
    } else if(time >= 960) {
        register_value->Byte |= 0x0F; 
    }
}

/**
 * @函数名称: sh367309_charge_overcurrent_protection_voltage_value_set()
 * @功能描述: 充电过流保护电压值设置
 * @输入: register_value: 寄存器值 current_value: 电流值
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_charge_overcurrent_protection_voltage_value_set(uByteBits* register_value, uint16_t current_value)
{
    uint32_t voltage_value = 0;
    voltage_value = SH367309_CURRENT_SAMPLING_RESISTANCE*current_value;
    voltage_value = voltage_value/1000;
    if(register_value != NULL) { 
        if(voltage_value < 30) {
            register_value->Byte |= 0 << 4; 
        } else if((voltage_value >= 30)&&(voltage_value < 40)) {
            register_value->Byte = 0x01<<4; 
        } else if((voltage_value >= 40)&&(voltage_value < 50)) {
            register_value->Byte = 0x02<<4; 
        } else if((voltage_value >= 50)&&(voltage_value < 60)) {
            register_value->Byte = 0x03<<4; 
        } else if((voltage_value >= 60)&&(voltage_value < 70)) {
            register_value->Byte = 0x04<<4; 
        } else if((voltage_value >= 70)&&(voltage_value < 80)) {
            register_value->Byte = 0x05<<4; 
        } else if((voltage_value >= 80)&&(voltage_value < 90)) {
            register_value->Byte = 0x06<<4; 
        } else if((voltage_value >= 90)&&(voltage_value < 100)) {
            register_value->Byte = 0x07<<4; 
        } else if((voltage_value >= 100)&&(voltage_value < 110)) {
            register_value->Byte = 0x08<<4; 
        } else if((voltage_value >= 110)&&(voltage_value < 120)) {
            register_value->Byte = 0x09<<4; 
        } else if((voltage_value >= 120)&&(voltage_value < 130)) {
            register_value->Byte = 0x0A<<4; 
        } else if((voltage_value >= 130)&&(voltage_value < 140)) {
            register_value->Byte = 0x0B<<4; 
        } else if((voltage_value >= 140)&&(voltage_value < 160)) {
            register_value->Byte = 0x0C<<4; 
        } else if((voltage_value >= 160)&&(voltage_value < 180)) {
            register_value->Byte = 0x0D<<4; 
        } else if((voltage_value >= 180)&&(voltage_value < 200)) {
            register_value->Byte = 0x0E<<4; 
        }else if(voltage_value >= 200) {
            register_value->Byte = 0x0F<<4; 
        } 
    }
}

/**
 * @函数名称: sh367309_charge_overcurrent_protection_delay_value_set()
 * @功能描述: 充电过流保护延时设置
 * @输入: register_value: 寄存器值 time: 延时时间
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_charge_overcurrent_protection_delay_value_set(uByteBits* register_value, uint16_t time)
{ 
    if(time < 20) {
        register_value->Byte |= 0; 
    } else if((time >= 20)&&(time < 40)) {
        register_value->Byte |= 0x01; 
    } else if((time >= 40)&&(time < 60)) {
        register_value->Byte |= 0x02; 
    } else if((time >= 60)&&(time < 80)) {
        register_value->Byte |= 0x03; 
    } else if((time >= 80)&&(time < 100)) {
        register_value->Byte |= 0x04; 
    } else if((time >= 100)&&(time < 200)) {
        register_value->Byte |= 0x05; 
    } else if((time >= 200)&&(time < 400)) {
        register_value->Byte |= 0x06; 
    } else if((time >= 400)&&(time < 600)) {
        register_value->Byte |= 0x07; 
    } else if((time >= 600)&&(time < 800)) {
        register_value->Byte |= 0x08; 
    } else if((time >= 800)&&(time < 1000)) {
        register_value->Byte |= 0x09; 
    } else if((time >= 1000)&&(time < 2000)) {
        register_value->Byte |= 0x0A; 
    } else if((time >= 2000)&&(time < 4000)) {
        register_value->Byte |= 0x0B; 
    } else if((time >= 4000)&&(time < 8000)) {
        register_value->Byte |= 0x0C; 
    } else if((time >= 8000)&&(time < 10000)) {
        register_value->Byte |= 0x0D; 
    } else if((time >= 10000)&&(time < 20000)) {
        register_value->Byte |= 0x0E; 
    } else if(time >= 20000) {
        register_value->Byte |= 0x0F; 
    }
}

/**
 * @函数名称: sh367309_secondary_overcharge_protection_delay_set()
 * @功能描述: 二次过充电保护延时设置
 * @输入: register_value: 寄存器值 time: 延时时间
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_secondary_overcharge_protection_delay_set(uByteBits* register_value, uint16_t time)
{ 
    if((time > 0)&&(time < 1600)) {
        register_value->Bits.Bit0 = 0;
        register_value->Bits.Bit1 = 0; 
    } else if((time >= 1600)&&(time < 3200)) {
        register_value->Bits.Bit0 = 1;
        register_value->Bits.Bit1 = 0; 
    } else if((time >= 3200)&&(time < 6400)) {
        register_value->Bits.Bit0 = 0;
        register_value->Bits.Bit1 = 1; 
    } else if(time >= 6400) {
        register_value->Bits.Bit0 = 1;
        register_value->Bits.Bit1 = 1; 
    }
}

/**
 * @函数名称: sh367309_charge_discharge_overcurrent_self_recovery_delay_set()
 * @功能描述: 充放电过流自恢复延时设置
 * @输入: register_value: 寄存器值 time: 延时时间
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_charge_discharge_overcurrent_self_recovery_delay_set(uByteBits* register_value, uint16_t time)
{ 
    if((time > 0)&&(time < 1600)) {
        register_value->Bits.Bit2 = 0;
        register_value->Bits.Bit3 = 0; 
    } else if((time >= 1600)&&(time < 3200)) {
        register_value->Bits.Bit2 = 1;
        register_value->Bits.Bit3 = 0; 
    } else if((time >= 3200)&&(time < 6400)) {
        register_value->Bits.Bit2 = 0;
        register_value->Bits.Bit3 = 1; 
    } else if(time >= 6400) {
        register_value->Bits.Bit2 = 1;
        register_value->Bits.Bit3 = 1; 
    }
}

/**
 * @函数名称: sh367309_charging_discharg_mos_opening_delay_set()
 * @功能描述: 充放电MOS开启延时设置
 * @输入: register_value: 寄存器值 time: 延时时间
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_charging_discharg_mos_opening_delay_set(uByteBits* register_value, uint16_t time)
{ 
    if(time < 128) {
        register_value->Bits.Bit4 = 0;
        register_value->Bits.Bit5 = 0; 
    } else if((time >= 128)&&(time < 256)) {
        register_value->Bits.Bit4 = 1;
        register_value->Bits.Bit5 = 0; 
    } else if((time >= 256)&&(time < 512)) {
        register_value->Bits.Bit4 = 0;
        register_value->Bits.Bit5 = 1; 
    } else if(time >= 512) {
        register_value->Bits.Bit4 = 1;
        register_value->Bits.Bit5 = 1; 
    }
}

/**
 * @函数名称: sh367309_charge_discharge_state_detection_voltage_set()
 * @功能描述: 充放电状态检测电压设置
 * @输入: register_value: 寄存器值 voltage_value: 电压值
 * @输出: 无
 * @返回值: 无
 */
static void sh367309_charge_discharge_state_detection_voltage_set(uByteBits* register_value, uint16_t voltage_value)
{ 
    if(voltage_value < 500) {
        register_value->Bits.Bit6 = 0;
        register_value->Bits.Bit7 = 0; 
    } else if((voltage_value >= 500)&&(voltage_value < 1000)) {
        register_value->Bits.Bit6 = 1;
        register_value->Bits.Bit7 = 0; 
    } else if((voltage_value >= 1000)&&(voltage_value < 2000)) {
        register_value->Bits.Bit6 = 0;
        register_value->Bits.Bit7 = 1; 
    } else if(voltage_value >= 2000) {
        register_value->Bits.Bit6 = 1;
        register_value->Bits.Bit7 = 1; 
    }
}

/**
 * @函数名称: sh367309_get_rnor_value()
 * @功能描述: 查表根据温度获取阻值
 * @输入: temp：温度值
 * @输出: 无
 * @返回值: 无
 */
static float sh367309_get_rnor_value(int temp) 
{
    int size = sizeof(temp_rnor_table) / sizeof(temp_rnor_table[0]);
    for (int i = 0; i < size; i++)  {
        if (temp_rnor_table[i].temp == temp) {
            return temp_rnor_table[i].Rnor;
        } else if (temp_rnor_table[i].temp > temp && i > 0) {
            /* 线性插值 */
            float slope = (temp_rnor_table[i].Rnor - temp_rnor_table[i - 1].Rnor) /(temp_rnor_table[i].temp - temp_rnor_table[i - 1].temp);
            return temp_rnor_table[i - 1].Rnor + slope * (temp - temp_rnor_table[i - 1].temp);
        }
    }
    /* 未找到合适的温度，返回错误值 */
    return -1.0;
}

#define TABLE_SIZE (sizeof(temp_rnor_table)/sizeof(temp_rnor_table[0]))

/**
 * @函数名称: resistance_to_temp()
 * @功能描述: 查表法计算温度，使用线性插值
 * @输入: resistance_ohm 电阻值 单位kΩ; 
 * @输出: 无
 * @返回值: 温度值
 */
float resistance_to_temp(float resistance) {
    for (int i = 0; i < TABLE_SIZE - 1; ++i) {
        float r1 = temp_rnor_table[i].Rnor;
        float r2 = temp_rnor_table[i + 1].Rnor;

        // 查找插值范围
        if ((resistance <= r1 && resistance >= r2) || (resistance >= r1 && resistance <= r2)) {
            int t1 = temp_rnor_table[i].temp;
            int t2 = temp_rnor_table[i + 1].temp;
            // 线性插值
            float temp = t1 + (resistance - r1) * (t2 - t1) / (r2 - r1);
            return temp;
        }
    }

    // 超出查表范围，返回无效标记
    if(resistance > temp_rnor_table[TABLE_SIZE-1].Rnor) {
        return -999.0;
    } else {
        return 999.0;
    }
}

/**
 * @函数名称: battery_afe_init()
 * @功能描述: AFE芯片初始化
 * @输入: init_parameter: 初始化结构体的参数
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
uint8_t battery_afe_init(AFE_INIT* init_parameter)
{
    float rnor[10] ={0};
    
    /* 用来提取TR寄存器的值 */
    uint8_t TR_buff = 0;
    /* 用来向AFE的充放电高温保护阈值寄存器里写如算好的值 */
    uint8_t temperature_register_value = 0;
    /* 充电保护，放电保护恢复值 */
    uWordBits cell_charge_discharge_volatge_recover[4] = {0};
    /* 对应SH367309的flash寄存器0至19 */
    uByteBits register_buff[0x19] = {0};
    /* 对应SH367309的RAM寄存器 */
    uByteBits ram_buff[3] = {0};
    /* 定义一个标志位，用于标记是否需要重新写入flash */
    // bool flash_needs_update = false; 
    // bool parameter_compare= true; 
    BATTERY_LOG("AFE_init_flag\r\n");
    board_switch_on_or_off(POWER_VPRO,"ON");
    gd32f4x_delay_ms(70);

    read_flash_parameter("afe_init_flag", &init_parameter->afe_init_flag);
    BATTERY_LOG("afe_init_flag = %x\r\n",init_parameter->afe_init_flag);
    /********* 上电初始化参数值逻辑 ***********/
    /* 1.如果上电后flash中没有值 */
    // if (init_parameter->afe_init_flag == 0x00) { 
    //     BATTERY_LOG("flash is empty, loading default parameters...\r\n");
    //     /* 将默认值写入flash中 */
    //     sh367309_init_default_flash_parameter(init_parameter);
    //     battery_afe_write_default_flash_parameter(init_parameter);
    //     /* 从flash进行读取 */ 
    //     battery_afe_read_init_register_config(init_parameter);
    // } else {
    //     // /* 2.如果上电后flash有值，读取flash，如果代码中的默认参数被修改了需要和flash中对比进行更新flash */ 
    //     // BATTERY_LOG("Loading parameters from flash...\r\n");
    //     // if(can_set_flag == 1) {
    //     //     /* 如果有CAN数据则进行写flash，然后读取加载 */
    //     //    battery_afe_write_default_flash_parameter(init_parameter); 
    //     //    battery_afe_read_init_register_config(init_parameter);
    //     //    can_set_flag = 0;
    //     // } else {
    //     //     /* 从flash进行读取 */
    //     //     battery_afe_read_init_register_config(init_parameter);
    //     // }
    // }   


    sh367309_init_default_flash_parameter(init_parameter);
    /********* 将参数值写入AFE寄存器中 ***********/
    /* 要想AFE的EEPROM写东西需要开启这个电源，写完之后要关闭 */
    /* 配置系统参数1-00H */
    BATTERY_LOG("/********************************************/\r\n");
    sh367309_chip_systerm1_config(&register_buff[0],init_parameter);
    BATTERY_LOG("register_buff[0] = %x\r\n",register_buff[0].Byte);
    sh367309_write(0x00,register_buff[0].Byte);
    gd32f4x_delay_ms(36);

    /* 配置系统参数2-01H */
    sh367309_chip_systerm2_config(&register_buff[1],init_parameter);
    BATTERY_LOG("register_buff[1] = %x\r\n",register_buff[1].Byte);
    sh367309_write(0x01,register_buff[1].Byte);
    gd32f4x_delay_ms(36);

    /* 02H,03H */
    sh367309_charge_discharge_overvoltage_protect_time_set(&register_buff[2],init_parameter->cell_overvoltage_value_time);
    sh367309_overvoltage_Load_release_delay_set(&register_buff[2],init_parameter->cell_overvoltage_Load_release_delay);
    /* 充电过压保护值 */
    cell_charge_discharge_volatge_recover[0].W = init_parameter->cell_overvoltage_value/5;
    register_buff[2].Bits.Bit0 = cell_charge_discharge_volatge_recover[0].bits.hbyte.bit.b8;
    register_buff[2].Bits.Bit1 = cell_charge_discharge_volatge_recover[0].bits.hbyte.bit.b9;
    register_buff[3].Byte = cell_charge_discharge_volatge_recover[0].bits.lbyte.L;
    BATTERY_LOG("register_buff[2] = %x\r\n",register_buff[2].Byte);
    BATTERY_LOG("register_buff[3] = %x\r\n",register_buff[3].Byte);
    sh367309_write(0x02,register_buff[2].Byte);
    gd32f4x_delay_ms(36);
    sh367309_write(0x03,register_buff[3].Byte);

    /* 04H,05H */
    /* 因为SH367309芯片充电过压保护后，恢复的时间是定死的2*100mS，因此无需设置充电过压保护恢复时间 */
    /* 放电欠压保护延时时间 */
    sh367309_charge_discharge_overvoltage_protect_time_set(&register_buff[4],init_parameter->cell_undervoltage_value_time);
    /* 充电过压告警恢复值 */
    cell_charge_discharge_volatge_recover[1].W = init_parameter->cell_overvoltage_recovery_value/5;
    register_buff[4].Bits.Bit0 = cell_charge_discharge_volatge_recover[0].bits.hbyte.bit.b8;
    register_buff[4].Bits.Bit1 = cell_charge_discharge_volatge_recover[0].bits.hbyte.bit.b9;
    register_buff[5].Byte = cell_charge_discharge_volatge_recover[1].bits.lbyte.L;
    BATTERY_LOG("register_buff[4] = %x\r\n",register_buff[4].Byte);
    BATTERY_LOG("register_buff[5] = %x\r\n",register_buff[5].Byte);
    sh367309_write(0x04,register_buff[4].Byte);
    gd32f4x_delay_ms(36);
    sh367309_write(0x05,register_buff[5].Byte);

    /* 06H */
    /* 单体放电欠压告警值 */
    register_buff[6].Byte = init_parameter->cell_undervoltage_value/20;
    BATTERY_LOG("register_buff[6] = %x\r\n",register_buff[6].Byte);
    sh367309_write(0x06,register_buff[6].Byte);
    gd32f4x_delay_ms(36);

    /* 07H */
    /* 单体欠压告警恢复值 */
    register_buff[7].Byte = init_parameter->cell_undervoltage_recovery_value/20;
    BATTERY_LOG("register_buff[7] = %x\r\n",register_buff[7].Byte);
    sh367309_write(0x07,register_buff[7].Byte);
    gd32f4x_delay_ms(36);

    /* 08H */
    /* 电池均衡开启电压设置 */
    register_buff[8].Byte = init_parameter->balance_voltage_value/20;
    BATTERY_LOG("register_buff[8] = %x\r\n",register_buff[8].Byte);
    sh367309_write(0x08,register_buff[8].Byte);
    gd32f4x_delay_ms(36);

    /* 0AH */
    /* 低电压禁止充电电压设置 */
    register_buff[0x0A].Byte = init_parameter->low_voltage_stop_charging_voltage_value/20;
    BATTERY_LOG("register_buff[A] = %x\r\n",register_buff[0x0A].Byte);
    sh367309_write(0x0A,register_buff[0x0A].Byte);
    gd32f4x_delay_ms(36);

    /* 0BH */
    /* 二次过充电保护电压值 */
    register_buff[0x0B].Byte = init_parameter->secondary_overcharge_protection_voltage_value/20;
    BATTERY_LOG("register_buff[B] = %x\r\n",register_buff[0X0B].Byte);
    sh367309_write(0x0B,register_buff[0x0B].Byte);
    gd32f4x_delay_ms(36);

    /* 0CH */
    /* 放电过流1保护电压值 */
    sh367309_discharge_overcurrent1_protection_voltage_value_set(&register_buff[0x0C],init_parameter->secondary_overcharge_protection_voltage_value);
    sh367309_discharge_overcurrent1_protection_delay_value_set(&register_buff[0x0C],init_parameter->discharge_overcurrent1_protection_delay_value);
    BATTERY_LOG("register_buff[C] = %x\r\n",register_buff[0X0C].Byte);
    sh367309_write(0x0C,register_buff[0x0C].Byte);
    gd32f4x_delay_ms(36);

    /* 0DH */
    sh367309_discharge_overcurrent2_protection_voltage_value_set(&register_buff[0x0D],init_parameter->discharge_overcurrent2_protection_voltage_value);
    sh367309_discharge_overcurrent2_protection_delay_value_set(&register_buff[0x0D],init_parameter->discharge_overcurrent2_protection_delay_value);
    BATTERY_LOG("register_buff[D] = %x\r\n",register_buff[0X0D].Byte);
    sh367309_write(0x0D,register_buff[0x0D].Byte);
    gd32f4x_delay_ms(36);

    /* 0EH */
    sh367309_short_circuit_protection_voltage_value_set(&register_buff[0x0E],init_parameter->short_circuit_protection_voltage_value);
    sh367309_short_circuit_protection_delay_value_set(&register_buff[0x0E],init_parameter->short_circuit_protection_delay_value);
    BATTERY_LOG("register_buff[E] = %x\r\n",register_buff[0X0E].Byte);
    sh367309_write(0x0E,register_buff[0x0E].Byte);
    gd32f4x_delay_ms(36);

    /* 0FH */
    sh367309_charge_overcurrent_protection_voltage_value_set(&register_buff[0x0F],init_parameter->charge_overcurrent_protection_voltage_value);
    sh367309_charge_overcurrent_protection_delay_value_set(&register_buff[0x0F],init_parameter->charge_overcurrent_protection_delay_value);
    BATTERY_LOG("register_buff[F] = %x\r\n",register_buff[0X0F].Byte);
    sh367309_write(0x0F,register_buff[0x0F].Byte);
    gd32f4x_delay_ms(36);

    /* 10H */
    sh367309_secondary_overcharge_protection_delay_set(&register_buff[0x10],init_parameter->secondary_overcharge_protection_delay);
    sh367309_charge_discharge_overcurrent_self_recovery_delay_set(&register_buff[0x10],init_parameter->charge_discharge_overcurrent_self_recovery_delay);
    sh367309_charging_discharg_mos_opening_delay_set(&register_buff[0x10],init_parameter->charging_discharg_mos_opening_delay);
    sh367309_charge_discharge_state_detection_voltage_set(&register_buff[0x10],init_parameter->charge_discharge_state_detection_voltage);
    BATTERY_LOG("register_buff[10] = %x\r\n",register_buff[0X10].Byte);
    sh367309_write(0x10,register_buff[0x10].Byte);
    gd32f4x_delay_ms(36);

    /* 11H */
    /* 充放电MOS充放电过温保护和恢复阈值计算公式：TR是TR寄存器的值
    公式：Rref = 6.8+0.05*TR；
        过温保护和释放阈值 = (Rt1/(Rref+Rt1))*512
    充放电低温保护和恢复阈值计算公式：TR是TR寄存器的值
    公式：Rref = 6.8+0.05*TR；
        低温保护和释放阈值 = ((Rt1/(Rref+Rt1)) - 0.5)*512 */
    sh367309_read(0x19,1,&TR_buff);
    rnor[0] = sh367309_get_rnor_value(init_parameter->charge_over_temperature_value - TEMPERATURE_OFFSET);
    temperature_register_value = CHA_DIS_HIGH_TEMP_VAL(rnor[0],TR_buff);
    BATTERY_LOG("register_buff[11] = %x\r\n",temperature_register_value);
    sh367309_write(0x11,temperature_register_value);
    gd32f4x_delay_ms(36);
    /* 单体充电过温告警恢复值设定  充电高温保护恢复值 */
    rnor[1] = sh367309_get_rnor_value(init_parameter->charge_over_temperature_recovery_value - TEMPERATURE_OFFSET);
    temperature_register_value = CHA_DIS_HIGH_TEMP_VAL(rnor[1],TR_buff);
    BATTERY_LOG("register_buff[12] = %x\r\n",temperature_register_value);
    sh367309_write(0x12,temperature_register_value);
    gd32f4x_delay_ms(36);
    /* 充电低温保护设置 */
    rnor[2] = sh367309_get_rnor_value(init_parameter->charge_under_temperature_value - TEMPERATURE_OFFSET);
    temperature_register_value = CHA_DIS_LOW_TEMP_VAL(rnor[2],TR_buff);
    BATTERY_LOG("register_buff[13] = %x\r\n",temperature_register_value);
    sh367309_write(0x13,temperature_register_value);
    gd32f4x_delay_ms(36);
    /* 充电低温保护恢复 */
    rnor[3] = sh367309_get_rnor_value(init_parameter->charge_under_temperature_recovery_value - TEMPERATURE_OFFSET);
    temperature_register_value = CHA_DIS_LOW_TEMP_VAL(rnor[3],TR_buff);
    BATTERY_LOG("register_buff[14] = %x\r\n",temperature_register_value);
    sh367309_write(0x14,temperature_register_value);
    gd32f4x_delay_ms(36);
    /* 放电过温保护设置 */
    rnor[4] = sh367309_get_rnor_value(init_parameter->discharge_over_temperature_value - TEMPERATURE_OFFSET);
    temperature_register_value = CHA_DIS_HIGH_TEMP_VAL(rnor[4],TR_buff);
    BATTERY_LOG("register_buff[15] = %x\r\n",temperature_register_value);
    sh367309_write(0x15,temperature_register_value);
    gd32f4x_delay_ms(36);    
    /* 放电过温恢复 */
    rnor[5] = sh367309_get_rnor_value(init_parameter->discharge_over_temperature_recovery_value - TEMPERATURE_OFFSET);
    temperature_register_value = CHA_DIS_HIGH_TEMP_VAL(rnor[5],TR_buff);
    BATTERY_LOG("register_buff[16] = %x\r\n",temperature_register_value);
    sh367309_write(0x16,temperature_register_value);
    gd32f4x_delay_ms(36);    
    /* 放电低温保护 */
    rnor[6] = sh367309_get_rnor_value(init_parameter->discharge_under_temperature_value - TEMPERATURE_OFFSET);
    temperature_register_value = CHA_DIS_LOW_TEMP_VAL(rnor[6],TR_buff);
    BATTERY_LOG("register_buff[17] = %x\r\n",temperature_register_value);
    sh367309_write(0x17,temperature_register_value);
    gd32f4x_delay_ms(36);    
    /* 放电低温保护恢复 */
    rnor[7] = sh367309_get_rnor_value(init_parameter->discharge_under_temperature_recovery_value - TEMPERATURE_OFFSET);    
    temperature_register_value = CHA_DIS_LOW_TEMP_VAL(rnor[7],TR_buff);
    BATTERY_LOG("register_buff[18] = %x\r\n",temperature_register_value);
    sh367309_write(0x18,temperature_register_value);
    gd32f4x_delay_ms(36);

    /* 对sh367309 RAM寄存器的写入 */
    sh367309_chip_system40_config(&ram_buff[0],init_parameter);
    BATTERY_LOG("register_buff[40] = %x\r\n",ram_buff[0].Byte);
    sh367309_write(0x40,ram_buff[0].Byte);
    gd32f4x_delay_ms(36); 

    sh367309_chip_system41_config(&ram_buff[1],init_parameter);
    BATTERY_LOG("register_buff[41] = %x\r\n",ram_buff[1].Byte);
    sh367309_write(0x41,ram_buff[1].Byte);
    gd32f4x_delay_ms(36);    
    
    sh367309_chip_system42_config(&ram_buff[2],init_parameter);
    BATTERY_LOG("register_buff[42] = %x\r\n",ram_buff[2].Byte);
    sh367309_write(0x42,ram_buff[2].Byte);
    gd32f4x_delay_ms(36);
    /* AFE 软件复位 */
    sh367309_reset();
    #ifdef AFE_DEBUG_PRINTF
    BATTERY_LOG("AFE INIT RESET SUCCESS\r\n");
    #endif
    /* 关闭烧写vpro */
    board_switch_on_or_off(POWER_VPRO,"OFF");
    gd32f4x_delay_ms(50);
    BATTERY_LOG("/**********************************************/\r\n");
    return 0;
}

/**
 * @函数名称: battery_afe_alarm_callback_function()
 * @功能描述: 告警的中断回调函数
 * @输入: 无
 * @输出: 无
 * @返回值: 无
 */
void battery_afe_alarm_callback_function(void)
{
    /* 获取实时告警状态，读取状态寄存器 */
    battery_afe_get_alarm_state(&alarm_state);
    /* 获取历史告警状态，读取状态寄存器 */
    battery_afe_get_history_alarm_state(&alarm_state);
}

/**
 * @函数名称: battery_afe_get_alarm_state()
 * @功能描述: AFE芯片实时告警中断状态获取
 * @输入: ALARM_DATA 类型的结构体指针
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
uint8_t battery_afe_get_alarm_state(ALARM_DATA* state_value)
{
    uint8_t return_value = 0;
    uByteBits state_buff[3] = {0};

    /* 43h连读三个状态寄存器 */
    return_value = sh367309_read(0x43,3,(uint8_t*)&state_buff[0].Byte);
    if(return_value == 0) {   
        if((&state_buff[0] != NULL)&&(&state_buff[1] != NULL)&&(&state_buff[2] != NULL)) {
            /* 发生过AFE通信异常，看门狗标志位 */
            state_value->alarm_state1.Bits.watchdog_flag = state_buff[0].Bits.Bit7;
            /* 发生二次过充电过压保护 */        
            state_value->alarm_state1.Bits.secondary_overcharge_protect_flag = state_buff[0].Bits.Bit6;
            /* 发生过短路保护 */ 
            state_value->alarm_state1.Bits.short_circuit_protect_flag = state_buff[0].Bits.Bit5; 
            /* 发生过充电过流保护 */
            state_value->alarm_state1.Bits.charge_overcurrent_protect_flag = state_buff[0].Bits.Bit4; 
            /* 发生过放电过流2保护 */       
            state_value->alarm_state1.Bits.discharge_overcurrent2_protect_flag = state_buff[0].Bits.Bit3;
            /* 发生过放电过流1保护 */        
            state_value->alarm_state1.Bits.discharge_overcurrent1_protect_flag = state_buff[0].Bits.Bit2; 
            /* 发生过欠压保护 */       
            state_value->alarm_state1.Bits.undervoltage_protect_flag = state_buff[0].Bits.Bit1;
            /* 发生过充电过压保护 */        
            state_value->alarm_state1.Bits.overvoltage_protect_flag = state_buff[0].Bits.Bit0;            
            /* 发生过放电高温保护 */  
            state_value->alarm_state2.Bits.discharge_over_temperature_protect_flag = state_buff[1].Bits.Bit3; 
            /* 发生过放电低温保护 */    
            state_value->alarm_state2.Bits.discharge_under_temperature_protect_flag = state_buff[1].Bits.Bit2;  
            /* 发生过充电高温保护 */  
            state_value->alarm_state2.Bits.charge_over_temperature_protect_flag = state_buff[1].Bits.Bit1;
            /* 发生过充电低温保护 */   
            state_value->alarm_state2.Bits.charge_under_temperature_protect_flag = state_buff[1].Bits.Bit0;
            /* 充电状态 */
            state_value->alarm_state3.Bits.charge_status_flag = state_buff[2].Bits.Bit7;
            /* 放电状态 */        
            state_value->alarm_state3.Bits.discharge_status_flag = state_buff[2].Bits.Bit6;
            /* EEPROM写操作状态位 */
            state_value->alarm_state3.Bits.eeprom_write_status_flag = state_buff[2].Bits.Bit4; 
            /* 低压禁止充电状态位 */       
            state_value->alarm_state3.Bits.low_voltage_stop_charging_flag = state_buff[2].Bits.Bit3;
            /* 预充电MOS开关状态位 */        
            state_value->alarm_state3.Bits.precharge_mos_switch_status_flag = state_buff[2].Bits.Bit2; 
            /* 充电MOS开关状态位 */       
            state_value->alarm_state3.Bits.charge_mos_status_flag = state_buff[2].Bits.Bit1;
            /* 放电MOS开关状态位 */        
            state_value->alarm_state3.Bits.discharge_mos_status_flag = state_buff[2].Bits.Bit0;                     
            #if 0
            BATTERY_LOG("alarm_state1 = %x\r\n",state_value->alarm_state1.Byte);
            BATTERY_LOG("alarm_state2 = %x\r\n",state_value->alarm_state2.Byte);
            BATTERY_LOG("alarm_state3 = %x\r\n",state_value->alarm_state3.Byte);
            #endif
        }  
    } else {
        return_value = 1;
    }
    return return_value;    
}

/**
 * @函数名称: battery_afe_get_history_alarm_state()
 * @功能描述: AFE芯片历史告警中断状态获取
 * @输入: ALARM_DATA 类型的结构体指针
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
uint8_t battery_afe_get_history_alarm_state(ALARM_DATA* history_state_value)
{
    uint8_t return_value = 0;
    uByteBits state_buff[2] = {0};
   
    /* 70h连读两个寄存器 */
    return_value = sh367309_read(0x70,2,(uint8_t*)&state_buff[0].Byte);
    if(return_value == 0) {   
        if((&state_buff[0] != NULL)&&(&state_buff[1] != NULL)) {
            /* 发生过AFE通信异常，看门狗标志位 */
            history_state_value->history_alarm1.Bits.watchdog_flag = state_buff[0].Bits.Bit7;
            /* 发生二次过充电过压保护 */        
            history_state_value->history_alarm1.Bits.secondary_overcharge_protect_flag = state_buff[0].Bits.Bit6;
            /* 发生过短路保护 */ 
            history_state_value->history_alarm1.Bits.short_circuit_protect_flag = state_buff[0].Bits.Bit5; 
            /* 发生过充电过流保护 */
            history_state_value->history_alarm1.Bits.charge_overcurrent_protect_flag = state_buff[0].Bits.Bit4; 
            /* 发生过放电过流2保护 */       
            history_state_value->history_alarm1.Bits.LODO3_overcurrent_protect_flag = state_buff[0].Bits.Bit3;
            /* 发生过放电过流保护 */        
            history_state_value->history_alarm1.Bits.discharge_overcurrent_protect_flag = state_buff[0].Bits.Bit2; 
            /* 发生过欠压保护 */       
            history_state_value->history_alarm1.Bits.undervoltage_protect_flag = state_buff[0].Bits.Bit1;
            /* 发生过充电过压保护 */        
            history_state_value->history_alarm1.Bits.overvoltage_protect_flag = state_buff[0].Bits.Bit0;        
            /* AFE复位标志 */   
            history_state_value->history_alarm2.Bits.reset_flag = state_buff[1].Bits.Bit7; 
            /* AFE唤醒标志 */    
            history_state_value->history_alarm2.Bits.weakup_interrupt_flag = state_buff[1].Bits.Bit6;  
            /* AFE发生过CADC中断，电流高精度采集完成中断 */   
            history_state_value->history_alarm2.Bits.CADC_interrupt_flag = state_buff[1].Bits.Bit5; 
            /* AFE发生过VADC中断，电压，温度，低精度电流采集完成中断 */    
            history_state_value->history_alarm2.Bits.VADC_interrupt_flag = state_buff[1].Bits.Bit4;     
            /* 发生过放电高温保护 */  
            history_state_value->history_alarm2.Bits.discharge_over_temperature_protect_flag = state_buff[1].Bits.Bit3; 
            /* 发生过放电低温保护 */    
            history_state_value->history_alarm2.Bits.discharge_under_temperature_protect_flag = state_buff[1].Bits.Bit2;  
            /* 发生过充电高温保护 */  
            history_state_value->history_alarm2.Bits.charge_over_temperature_protect_flag = state_buff[1].Bits.Bit1;
            /* 发生过充电低温保护 */   
            history_state_value->history_alarm2.Bits.charge_under_temperature_protect_flag = state_buff[1].Bits.Bit0;
            #if 0
            BATTERY_LOG("history_alarm1 = %x\r\n",history_state_value->history_alarm1.Byte);
            BATTERY_LOG("history_alarm2 = %x\r\n",history_state_value->history_alarm2.Byte);
            #endif
        }  
    } else {
        return_value = 1;
    }
    return return_value;
}

/**
 * @函数名称: battery_afe_clear_history_alarm_state()
 * @功能描述: 清除AFE芯片历史告警状态
 * @输入: 无
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
uint8_t battery_afe_clear_history_alarm_state(void)
{
    uint8_t return_value = 0;;
    uint8_t state[2] = {0};

    state[0] = sh367309_write(0x70,0);
    vTaskDelay(35);
    state[1] = sh367309_write(0x71,0);
    vTaskDelay(35);
    if((state[0] == 1) || (state[1] == 1)) {
        return_value = 1;
    }
    return return_value;
}

/**
 * @函数名称: battery_afe_get_cell_voltage_value()
 * @功能描述: AFE芯片电压采集
 * @输入: cell_num: 单体电芯个数 cell_voltage_buff：存储电芯电压数据缓存
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
uint8_t battery_afe_get_cell_voltage_value(uint8_t cell_num, uint16_t* cell_voltage_buff)
{
    /* 增加这个变量，防止采集到的单体电压是0V. */
    uint16_t cell_voltage_value = 0;
    uint8_t uci = 0;
    uint8_t return_value = 0;
    /* 用于提取电芯电压 */
    uint8_t voltage_buff[32] = {0};

    /* 读取电芯电压 */
    return_value = sh367309_read(0x4E,cell_num <= 16 ? cell_num*2 : 0,voltage_buff);


    for(uci = 0;(uci < cell_num)&&(uci < 16);uci++) {
        if(&cell_voltage_buff[uci] != NULL) {

            cell_voltage_value = LOGICAL_LEFT((uint16_t)voltage_buff[uci*2],8,0xFF00) + LOGICAL_LEFT((uint16_t)voltage_buff[uci*2+1],0,0x00FF);
            /* 采集到的单体电压不是0V才能赋值 */
            if(cell_voltage_value != 0) {
                cell_voltage_buff[uci] = cell_voltage_value;
                /* AFE采集电芯电压最小单位是0.6mV,而电芯电压值的最小单位是1mV */
                cell_voltage_buff[uci] = (cell_voltage_buff[uci]*5)/32;
                // printf("cell_voltage_buff[%d] = %d\r\n",uci,cell_voltage_buff[uci]);
            }
        }
    }

    return return_value;
}

/**
 * @函数名称: battery_afe_get_other_voltage_value()
 * @功能描述: 计算电芯电压的统计信息，包括最大单体电压、最小单体电压、压差、最大最小单体电压编号以及总电压。
 * @输入: 
 *        cell_voltage_value: 电芯电压数组指针
 *        cell_num: 单体电芯个数 
 * @输出: 
 *        max_value: 最大单体电压 
 *        max_index: 最大单体电压编号
 *        min_value: 最小单体电压
 *        min_index: 最小单体电压编号
 *        total_voltage: 总电压
 * @返回值: 
 *        0: 成功
 *        1: 参数无效
 */
uint8_t battery_afe_get_other_voltage_value(const uint16_t *cell_voltage_value, uint8_t cell_num, 
                                uint16_t *max_value, uint8_t *max_index, 
                                uint16_t *min_value, uint8_t *min_index, 
                                uint32_t *total_voltage) {
    if (cell_voltage_value == NULL || cell_num == 0 || max_value == NULL || max_index == NULL || 
        min_value == NULL || min_index == NULL || total_voltage == NULL) {
        return 1; // 参数无效，直接返回
    }

    *max_value = cell_voltage_value[0];
    *min_value = cell_voltage_value[0];
    *total_voltage = cell_voltage_value[0];
    *max_index = 0;
    *min_index = 0;

    for (size_t i = 1; i < cell_num; i++) {
        uint16_t voltage = cell_voltage_value[i];
        *total_voltage += voltage;

        if (voltage > *max_value) {
            *max_value = voltage;
            *max_index = i;
        }

        if (voltage < *min_value) {
            *min_value = voltage;
            *min_index = i;
        }
    }

    return 0; // 成功
}

/**
 * @函数名称: battery_afe_get_current_value()
 * @功能描述: AFE芯片电流采集
 * @输入: current_value: 获取的电流值
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
uint8_t battery_afe_get_current_value(int32_t* current_value)
{
    uint8_t return_value = 0;
    int32_t calculate_current_value = 0;
    int16_t register_value = 0;
    uint8_t CADC_value[2] = {0};
    float gain = BATTERY_AFE_GET_GAIN_PARAM();
    int16_t read_offset_register_value = (int16_t)param_ram.afe_current_offset_current; //BATTERY_AFE_GET_OFFSET_REGISTER_VALUE_PARAM();

    /* 读取电流寄存器的值 */
    return_value = sh367309_read(0x6E,2,CADC_value);
    if((return_value == 0)&&(CURRENT_RSENSE != 0)) {

        register_value = (int16_t)(LOGICAL_LEFT((uint16_t)CADC_value[0],8,0xFF00) + LOGICAL_LEFT((uint16_t)CADC_value[1],0,0x00FF));
        

        /* 检查 gain 是否为 0 或未初始化(需要标定电流)，设置默认值 */
        // if (gain == 0) {
        //     /* 参数报警 */
        //     // param_ram.protect_other_flag |= PROTECT_CURRENT_CALIBRATION_ERR_FLAG;
        //     gain = 0.00001f;                        /* 设置一个合理的默认值 */
        //     read_offset_register_value = param_ram.afe_current_offset_current;    /* 设置一个合理的默认值 */
        //     printf("Gain not initialized, using default value: gain=%f off_reg=%x\r\n", gain,read_offset_register_value);
        // }

        // if(((register_value >> 15) & 0x1) == 1) {
        //     /* 放电 */
        //     calculate_current_value = (gain*(register_value - read_offset_register_value));
        //     // calculate_current_value = (gain*(register_value-0XFFE5));
        //     *current_value = calculate_current_value;
        // } else {
        //     /* 充电 */
        //     calculate_current_value = (gain*(register_value + ((0xFFFF - read_offset_register_value))));
        //     // calculate_current_value = (gain*(register_value-0XFFE5));
        //     *current_value = calculate_current_value;           
        // }

        // printf("register_value =%04X = %d %d\r\n",register_value, register_value, (int16_t)register_value);

        // if(((register_value >> 15) & 0x1) == 1) {
        //     /* 放电 */ //(0xffff - register_value)
        //     calculate_current_value = 0 - (int32_t)(((register_value - read_offset_register_value) * 200.0) / (21470 * CURRENT_RSENSE));//计算出来的是mA值

        //     *current_value = calculate_current_value;
        //     #if 1
        //     printf("discharge CADC_value_current_value = %dmA %dmA\r\n",calculate_current_value, *current_value);
        //     #endif
        // } else {
        //     /* 充电 */
        //     calculate_current_value = (int32_t)((((register_value + (0xFFFF - read_offset_register_value)) * 200.0) / (21470 * CURRENT_RSENSE)));
        //     *current_value = calculate_current_value;    
        //     #if 1  
        //     printf("charge CADC_value_current_value = %dmA %dmA\r\n",calculate_current_value, *current_value);
        //     #endif  
        // }

        int16_t current_value_temp = register_value - read_offset_register_value;

        if(current_value_temp < 0) {
            /* 放电 */
            calculate_current_value = (((current_value_temp * 200.0) / (21470 * CURRENT_RSENSE)));
            *current_value = calculate_current_value; // 负值表示放电
            // printf("discharge CADC_value_current_value = %dmA %dmA offset=%04X\r\n",calculate_current_value, *current_value, read_offset_register_value);
        } else {
            /* 充电 */
            calculate_current_value = (((current_value_temp * 200.0) / (21470 * CURRENT_RSENSE)));
            *current_value = calculate_current_value; // 正值表示充电

            // printf("charge CADC_value_current_value = %dmA %dmA offset=%04X\r\n",calculate_current_value, *current_value, read_offset_register_value);
        }


    }
    return return_value;
}

/**
 * @函数名称: battery_afe_get_cell_temperature_value()
 * @功能描述: AFE芯片温度采集
 * @输入: temperature_num: 温度采集个数 cell_tempertaure_buff：获取的温度值
 * @输出: cell_tempertaure_buff : 存储温度值的缓存 温度单位0.01℃，例如25.0℃存储为250
 * @返回值: 0:正确。1：错误
 */
uint8_t battery_afe_get_cell_temperature_value(uint8_t temperature_num, int16_t* cell_tempertaure_buff)
{
    uint8_t return_value = 0;
    uint8_t tempertaure_buff[3*2] = {0};
    /* TEMP1至TEMP3寄存器的值 */
    uint16_t tempertaure_value[3] = {0};
    uint8_t uci = 0;
    /* 用于提取AFE内部的温度参考电阻 */
    uint8_t resistivity = 0;
    float reference_resistance_value = 0;
    /* 用于温度采集的外部热敏电阻阻值，单位KΩ */
    float external_thermistor_resistance[3] = {0};
    /* 获取温度内部参考电阻系数 */
    return_value = sh367309_read(0x19,1,&resistivity);
    if(return_value == 0) {
        /* 计算出参考电阻值 */
        reference_resistance_value = SH367309_R_REF(resistivity);
        
        return_value = sh367309_read(0x46,temperature_num <= 3 ? (temperature_num*2) : 0,tempertaure_buff);//提取温度值
        if(return_value == 0) {
            for(uci = 0;uci < 3;uci++) {
                tempertaure_value[uci] = LOGICAL_LEFT((uint16_t)tempertaure_buff[2*uci],8,0xFF00) +  LOGICAL_LEFT((uint16_t)tempertaure_buff[2*uci+1],0,0x00FF);
            }
            for(uci = 0;uci < 3;uci++) {
                external_thermistor_resistance[uci] = ((float)tempertaure_value[uci]/((float)32768 - (float)tempertaure_value[uci]))*reference_resistance_value;
                
           }
        //    /* 测试 3个电阻值 */
        //    printf("tempertaure_value[0] = %0.4f [1]= %0.4f [2]= %0.4f\r\n",external_thermistor_resistance[0],external_thermistor_resistance[1],external_thermistor_resistance[2]);

             
            for(uci = 0;(uci < temperature_num)&&(uci < 3);uci++) {
                if(&cell_tempertaure_buff[uci] != NULL) {
                    #if 0
                    /* 公式法计算温度值 */
                    cell_tempertaure_buff[uci] = B_VALUE_TEMP_METHOD(external_thermistor_resistance[uci])*10;
                    #else
                    /* 查表法计算温度值 */
                    cell_tempertaure_buff[uci] = (int16_t)(100 * resistance_to_temp(external_thermistor_resistance[uci]));
                    #endif
                }
            }
        }
    }
    return return_value;
}

/**
 * @函数名称: battery_afe_get_calibration_offset_current()
 * @功能描述: AFE芯片空载电流校准偏移值获取
 * @输入: offset_current: 空载获取到的电流偏移值
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
uint8_t battery_afe_get_calibration_offset_current(uint16_t* offset_current)
{
    uint8_t return_value = 0;
    uint8_t CADC_value[2] = {0};
    uint16_t read_offset_register_value = 0;
    gd32f4x_delay_ms(500);
    return_value = sh367309_read(0x6E,2,CADC_value);
    if(return_value == 0) {
        *offset_current = LOGICAL_LEFT((uint16_t)CADC_value[0],8,0xFF00) + LOGICAL_LEFT((uint16_t)CADC_value[1],0,0x00FF);
        if(read_flash_parameter("CADC_offset_value", &read_offset_register_value) == 0) {
            write_flash_parameter("CADC_offset_value", offset_current);
        } else {
            BATTERY_LOG("CADC_offset_value is store\r\n");
        }
    }

    return return_value;
}

/**
 * @函数名称: battery_afe_get_calibration_current_gain()
 * @功能描述: AFE芯片空载电流校准增益值获取
 * @输入: actual_cadc_value: 有负载实际CADC值 offset_cadc_value：CADC偏移值 calibration_actual_current：校准的电流值（单位：A）
 * @输出: 无
 * @返回值: 0:正确。1：错误
 */
float battery_afe_get_calibration_current_gain(uint16_t actual_cadc_value, uint16_t offset_cadc_value, float calibration_actual_current)
{
    /* 假设的增益值 */
    float gain = 0.04;
    float actual_gain = 0;
    float get_value = 0;

    actual_gain = (gain * calibration_actual_current) / (gain * (actual_cadc_value - offset_cadc_value));
    
    /* 如果实际增益为负，则取其绝对值 */
    if (actual_gain < 0) {
        actual_gain = -actual_gain;
    }

    if(read_flash_parameter("CADC_current_calibration_gain", &get_value) == 0) {
        write_flash_parameter("CADC_current_calibration_gain", &actual_gain);
    } else {
        BATTERY_LOG("CADC_offset_value is store\r\n");
    }
    return actual_gain;
}

/* SOC************************************************************************** */
/* 电池参数定义 */
#define Q_NOM                     (100.0f)      /* 电池标称容量，单位：Ah */
#define SERIES_COUNT              (8u)          /* 串联电池数量 */
#define DEFAULT_TEMPERATURE       (25.0f)       /* 默认温度 */

#define SAMPLING_TIME             (0.1f)        /* 采样周期，单位：秒 */
#define EFFICIENCY                (0.99f)       /* 库仑效率（假设充电和放电效率一致） */
#define OCV_SOC_POINTS            (11u)         /* OCV-SOC 曲线点数 */
#define VOLT_DIFF_THRESHOLD       (0.1f)        /* 电池一致性电压差阈值，单位：V */
#define SOC_DEFAULT               (40.0f)       /* 默认电池电量 */

#define CELL_FULL_VOLTAGE         (3.65)        /* 单电池满电压 */
#define CHARGE_FULL_CURRENT(cap)  (0.05*cap)    /* 充满电流计算 */  

#define INTEGRAL_CURRENT_AH_CONVER(current)      (current*SAMPLING_TIME/3600.0)    /* 积分电流AH转换 */

/* 变量 */

/* soc参数默认初始化 */
SOC_PARAM_TYPEDEF soc_param = {
    .use_age              = 0,
    .nominal_capacity     = Q_NOM,
    .series_battery       = SERIES_COUNT,
    .current_capacity     = Q_NOM,
    .current_temperature  = DEFAULT_TEMPERATURE,
    .soc                  = SOC_DEFAULT,
    .min_voltage          = 0,
    .max_voltage          = 0,
    .battery_diff_flag    = 0,
    .ocv_calibration_flag = 0,
};

/* 磷酸铁锂电池 OCV-SOC 曲线数据 */
const float ocv_table[OCV_SOC_POINTS] = {3.65, 3.50, 3.42, 3.37, 3.33, 3.30, 3.27, 3.22, 3.18, 3.10, 2.50};
const float soc_table[OCV_SOC_POINTS] = {100, 90, 80, 70, 60, 50, 40, 30, 20, 10, 0};

/* 温度补偿表 温度-补偿容量 */
const float temp_comp_table[][2] = {{-20, -20.0}, {-10, -10.0}, {0, 0.0}, {25, 0.0}, {40, 5.0}, {60, -10.0}};

/**
 * @函数名称: ocv_to_soc()
 * @功能描述: 查表函数 通过线性插值从 OCV 获取 SOC
 * @输入: voltage 电压
 * @输出: 无
 * @返回值: 电量百分比
 */
static float ocv_to_soc(float voltage)
{
    float soc = 0;
    if (voltage >= ocv_table[0]) {
        soc = 100;
    }
    else if (voltage <= ocv_table[OCV_SOC_POINTS - 1]) {
        soc = 0;
    }
    else {
        for (int i = 0; i < OCV_SOC_POINTS - 1; i++) {
            if (voltage <= ocv_table[i] && voltage > ocv_table[i + 1]) {
                /* 线性插值计算 */
                soc = soc_table[i] + (soc_table[i + 1] - soc_table[i]) *
                                         (voltage - ocv_table[i]) /
                                         (ocv_table[i + 1] - ocv_table[i]);
            }
        }
    }

    return soc;
}

/**
 * @函数名称: get_capacity_correction()
 * @功能描述: 根据温度修正容量
 * @输入: temperature
 * @输出: 无
 * @返回值: 修正容量
 */
float get_capacity_correction(float temperature)
{
    uint32_t temp_comp_table_size = sizeof(temp_comp_table) / sizeof(temp_comp_table[0]);
    for (int i = 0; i < temp_comp_table_size - 1; i++) {
        if (temperature >= temp_comp_table[i][0] &&  temperature <= temp_comp_table[i + 1][0]) {
            // 线性插值计算修正值
            float t1    = temp_comp_table[i][0];
            float comp1 = temp_comp_table[i][1];
            float t2    = temp_comp_table[i + 1][0];
            float comp2 = temp_comp_table[i + 1][1];
            return comp1 + (comp2 - comp1) * (temperature - t1) / (t2 - t1);
        }
    }
    return 0.0;  // 如果超出范围，返回 0
}

/**
 * @函数名称: check_battery_voltage_consistency()
 * @功能描述: 电压一致性判断
 * @输入: num 串数
 * @输入: p_in_cells 电池电压首地址
 * @输入: diff_value 一致性判断差值
 * @输出: p_out_min 输出最小值
 * @输出: p_out_max 输出最大值
 * @返回值: 0 一致， 1 不一致
 */
static uint8_t check_battery_voltage_consistency(uint8_t num, float* p_in_cells, float diff_value, float* p_out_min, float* p_out_max)
{
    if (NULL == p_in_cells) {
        return 0;
    }

    float min  = p_in_cells[0];
    float max  = p_in_cells[0];
    float diff = 0;

    for (uint8_t i = 0; i < num; i++) {
        if (min > p_in_cells[i]) {
            min = p_in_cells[i];
        }

        if (max < p_in_cells[i]) {
            max = p_in_cells[i];
        }
    }

    diff = max - min;

    if (diff > diff_value) {
        if (NULL != p_out_min) {
            *p_out_min = min;
        }

        if (NULL != p_out_max) {
            *p_out_max = max;
        }
        return 1;
    }

    return 0;
}

/**
 * @函数名称: battery_soc_init()
 * @功能描述: SOC参数初始化
 * @输入: p_soc_param 配置参数结构体地址
 * @输出: 无
 * @返回值: 0 成功 非0 失败
 */
uint8_t battery_soc_init(SOC_PARAM_TYPEDEF* p_soc_param)
{
    if (NULL == p_soc_param) {
        return 1;
    }
    soc_param.use_age              = p_soc_param->use_age;
    soc_param.nominal_capacity     = p_soc_param->nominal_capacity;
    soc_param.series_battery       = p_soc_param->series_battery;
    soc_param.current_capacity     = p_soc_param->current_capacity;
    soc_param.current_temperature  = p_soc_param->current_temperature;
    soc_param.soc                  = p_soc_param->soc;
    soc_param.min_voltage          = p_soc_param->min_voltage;
    soc_param.max_voltage          = p_soc_param->max_voltage;
    soc_param.battery_diff_flag    = p_soc_param->battery_diff_flag;
    soc_param.ocv_calibration_flag = p_soc_param->ocv_calibration_flag;

    return 0;
}

/**
 * @函数名称: battery_soc_calculate()
 * @功能描述: 更新函数
 * @输入: current 电流A
 * @输入: pack_voltage 包电压V
 * @输入: cell_voltages 单体电池电压V首地址
 * @输出: 无
 * @返回值: 0 成功 非0 失败
 */
uint8_t battery_soc_calculate(float current, float pack_voltage, float* cell_voltages)
{
    float soc_cell = 0;
    float voltage = 0;
    float calibration_soc = 0;
    float calibration_capacity = 0;

    /* 0. 温度修正容量 */
    calibration_capacity = get_capacity_correction(soc_param.current_temperature);
    soc_param.current_capacity = soc_param.nominal_capacity * (1 + calibration_capacity / 100.0);
    
    /* 1. 定时积分法更新整包 SOC  转为小时单位 */
    float delta_soc = INTEGRAL_CURRENT_AH_CONVER(current) / (soc_param.current_capacity / 100) * EFFICIENCY;

    /* 限制 SOC 范围在 0%～100% */
    float soc = soc_param.soc + delta_soc;
    soc_param.soc = (soc > 100.0) ? 100 : ((soc < 0) ? 0 : soc);

    /* 使用时间累计 */
    soc_param.use_age += SAMPLING_TIME;

    /* 2. 检测电池一致性 计算最小电压 和 最大电压*/
    if (check_battery_voltage_consistency(soc_param.series_battery, cell_voltages, 0.1, &soc_param.min_voltage, &soc_param.max_voltage)) {
        /* 置位不一致, OCV-SOC 放电用最小电压计算电量，充电用最大电压计算电量 */
        soc_param.battery_diff_flag = 1;
    }
    else {
        soc_param.battery_diff_flag = 0;
    }

    /*  3. OCV 校准 */
    if ((current < 0.1) && (current > -0.1)) {
        soc_param.ocv_calibration_flag = 1;
    }
    else {
        soc_param.ocv_calibration_flag = 0;
    }

    if (soc_param.ocv_calibration_flag) {
        if (soc_param.battery_diff_flag) {
            /* 使用最小值 */
            voltage = soc_param.min_voltage;
        }
        else {
            /* 充放电-使用平均值 */
            voltage = pack_voltage / soc_param.series_battery;
        }

        /* OCV-SOC查询 */
        soc_cell        = ocv_to_soc(voltage);
        calibration_soc = soc_param.soc;

        if (soc_param.battery_diff_flag) {
            /* 不一致时融合算法 */
            calibration_soc = 0.7 * calibration_soc + 0.3 * soc_cell;
        }
        else {
            /* 一致时融合算法 */
            calibration_soc = 0.8 * calibration_soc + 0.2 * soc_cell;
        }

        /* 融合后限制SOC范围0~100 */
        soc_param.soc = calibration_soc > 100 ? 100 : (calibration_soc < 0 ? 0 : calibration_soc);
    }

    /* 4. 判断是否充满 */
    if ((soc_param.soc > 98) && (soc_param.max_voltage >= CELL_FULL_VOLTAGE) &&
        (current > 0) && (current < CHARGE_FULL_CURRENT(soc_param.current_capacity))) {
        soc_param.soc = 100;
    }

    return 0;
}

/**
 * @函数名称: battery_soc_get_value()
 * @功能描述: 获取soc
 * @输出: p_soc_out 输出soc地址
 * @返回值: 0 成功 非0 失败
 */
uint8_t battery_soc_get_value(float* p_soc_out)
{
    if (NULL != p_soc_out) {
        *p_soc_out = soc_param.soc;
        return 0;
    }

    return 1;
}

/**
 * @函数名称: battery_soc_update_nominal_capacity()
 * @功能描述: 更新标称容量
 * @输入: full_capacity 当前使用的满量程容量
 * @输出: 无
 * @返回值: 0 成功 非0 失败
 */
uint8_t battery_soc_update_nominal_capacity(float full_capacity)
{
    if (full_capacity <= 0) {
        return 1;
    }

    soc_param.nominal_capacity = full_capacity;
    return 0;
}

/**
 * @函数名称: battery_soc_update_parameters()
 * @功能描述: 更新温度
 * @输入: temperature 温度
 * @输出: 无
 * @返回值: 0 成功 非0 失败
 */
uint8_t battery_soc_update_temperature(float temperature) 
{
    soc_param.current_temperature = temperature;

    return 0;
}

/**
 * @函数名称: battery_soc_get_age()
 * @功能描述: 获取使用时间 单位s
 * @输入: 无
 * @输出: age 使用时间
 * @返回值: 0 成功 非0 失败
 */
uint8_t battery_soc_get_age(float* age)
{
    if (!age) {
        return 1;
    }

    *age = soc_param.use_age;
    return 0;
}
/**
 * @函数名称: soc_reset()
 * @功能描述: 复位所有soc参数到默认参数
 * @输入: 无
 * @输出: 无
 * @返回值: 0 成功 非0 失败
 */
uint8_t soc_reset(void)
{
    soc_param.use_age              = 0;
    soc_param.nominal_capacity     = Q_NOM;
    soc_param.series_battery       = SERIES_COUNT;
    soc_param.current_capacity     = Q_NOM;
    soc_param.current_temperature  = DEFAULT_TEMPERATURE;
    soc_param.soc                  = SOC_DEFAULT;
    soc_param.min_voltage          = 0;
    soc_param.max_voltage          = 0;
    soc_param.battery_diff_flag    = 0;
    soc_param.ocv_calibration_flag = 0;

    return 0;
}

/* SOH************************************************************************** */
/* 电池参数定义 */

/* 变量 */
#define N_POINTS                     2                  /* 数据点（循环次数和容量） */
const int   cycles[N_POINTS]     = {3500, 6000};        /* 循环次数（对应剩余容量） */
const float capacities[N_POINTS] = {80.0, 70.0};        /* 剩余容量百分比（对应使用次数） */
static float decay_k = 0.000053;                        /* 容量衰减系数 */

SOH_PARAM_TYPEDEF soh_param = {
    .factory_capacity   = Q_INITIAL,
    .surplus_capacity   = Q_INITIAL,
    .discharge_capacity = Q_INITIAL,
    .cycle_count        = 0,
    .soh                = 100,
    .soh_limit          = SOH_LIMIT_MIN_DEFAULT,
    .dod                = 0,
    .dod_limit_max      = DOD_LIMIT_MAX_DEFAULT,
    .dod_count          = 0,
    .soh_err_flag       = 0,
    .dod_err_flag       = 0,
};

/**
 * @函数名称: fit_decay_coefficient()
 * @功能描述: 根据使用次数和容量衰减 拟合衰减系数 k。
 * @输入: 无
 * @输出: 无
 * @返回值: 衰减系数k
 */
static float fit_decay_coefficient(void)
{
    float sum_x  = 0.0;
    float sum_y  = 0.0;
    float sum_xy = 0.0;
    float sum_x2 = 0.0;

    for (int i = 0; i < N_POINTS; i++) {
        float x = cycles[i];
        /* 取对数以线性化 */
        float y = log(capacities[i] / Q_INITIAL);  
        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_x2 += x * x;
    }

    /* 计算斜率 k */
    float k = -(sum_xy - (sum_x * sum_y) / N_POINTS) / (sum_x2 - (sum_x * sum_x) / N_POINTS);
    return k;
}

/**
 * @函数名称: battery_soh_exp_model()
 * @功能描述: 指数模型: Q(t) = Q_INITIAL * exp(-k * t)
 * @输入: t 使用次数（时间）
 * @输入: k 衰减系数
 * @输出: 无
 * @返回值: 剩余容量
 */
static float battery_soh_exp_model(uint32_t t, float k)
{
    soh_param.surplus_capacity = soh_param.factory_capacity * exp(-k * t);
    return soh_param.surplus_capacity;
}

/**
 * @函数名称: battery_soh_calculate()
 * @功能描述: 计算 SOH, 通过使用次数（cycle）计算剩余容量并转换为SOH百分比。
 * @输入: cycle 使用次数
 * @输入: k 衰减系数
 * @输出: 无
 * @返回值: SOH百分比
 */
static float battery_soh_calculate(uint32_t cycle, float k)
{
    float capacity = battery_soh_exp_model(cycle, k);
    return (capacity / soh_param.factory_capacity) * 100;
}

/**
 * @函数名称: battery_soh_cycle_count()
 * @功能描述: 充放电循环次数统计
 * @输入: current 充放电电流
 * @输出: 无
 * @返回值: 有效累计返回1 无效计数返回0
 */
static uint8_t battery_soh_cycle_count(float current)
{
    static uint8_t dod_deep_flag = 0;

    if (current < 0) {
        /* 放电判断-统计放电-按标定满容量统计深度放电次数 */
        soh_param.discharge_capacity += INTEGRAL_CURRENT_AH_CONVER(-current);
        if (soh_param.discharge_capacity >= soh_param.surplus_capacity) {
            dod_deep_flag += 1;
            soh_param.discharge_capacity = 0;
        }
    }
    else if (current > SOH_CHARGE_CURRENT_THRESHOLD) {
        /* 充电判断-统计循环次数*/
        if (dod_deep_flag) {
            soh_param.cycle_count += dod_deep_flag;
            dod_deep_flag = 0;
            return 1;
        }
    }
    return 0;
}

/**
 * @函数名称: battery_dod_count()
 * @功能描述: 按阈值深度放电统计
 * @输入: soc 当前soc值
 * @输入: 
 * @输出: 无
 * @返回值: soh
 */
void battery_dod_count(float soc)
{
    static float   dod_laste     = 0;
    static float   dod_now       = 0;
    static uint8_t dod_deep_flag = 0;

    /* 深度放电计算 */
    dod_now = 100 - soc;
    soh_param.dod = dod_now;

    /* 深度放电判断 */
    if ((dod_laste < dod_now) && (dod_now > soh_param.dod_limit_max)) {
        dod_deep_flag = 1;

        soh_param.dod_err_flag = 1;
    }
    else if ((dod_deep_flag) && (dod_laste > dod_now)) {
        soh_param.dod_count += 1;
        dod_deep_flag = 0;
    }

    dod_laste = dod_now;
}

/**
 * @函数名称: battery_soh_init()
 * @功能描述: 初始化SOh模块。
 * @输入: soh_param_init 初始化数据结构体地址
 * @输出: 无
 * @返回值: 0 成功 非0 失败
 */
uint8_t battery_soh_init(SOH_PARAM_TYPEDEF* p_soh_param)
{
    if (!p_soh_param) {
        return 1;
    }

    /* 参数初始化 */
    soh_param.factory_capacity   = p_soh_param->factory_capacity;
    soh_param.surplus_capacity   = p_soh_param->surplus_capacity;
    soh_param.discharge_capacity = p_soh_param->discharge_capacity;
    soh_param.cycle_count        = p_soh_param->cycle_count;
    soh_param.soh                = p_soh_param->soh;
    soh_param.soh_limit          = p_soh_param->soh_limit;
    soh_param.dod                = p_soh_param->dod;
    soh_param.dod_limit_max      = p_soh_param->dod_limit_max;
    soh_param.dod_count          = p_soh_param->dod_count;
    soh_param.soh_err_flag       = p_soh_param->soh_err_flag;
    soh_param.dod_err_flag       = p_soh_param->dod_err_flag;

    /* 计算衰减系数 */
    decay_k = fit_decay_coefficient();
    return 0;
}

/**
 * @函数名称: battery_soh_cycle_calculate()
 * @功能描述: soh 循环使用次数算法。周期调用 SAMPLING_TIME 单位s
 * @输入: current 充放电电流
 * @输入: soc 当前soc
 * @输入: 无
 * @输出: p_soh_out 指向接收SOH值的指针。
 * @返回值: 0 成功 非0 失败
 */
uint8_t battery_soh_cycle_calculate(float current, float soc)
{
    if (soc < 0 ) {
        return 1;
    }

    /* 计算循环次数 */
    if(battery_soh_cycle_count(current))
    {
        /* 更新soh,更新剩余容量*/
        soh_param.soh = battery_soh_calculate(soh_param.cycle_count, decay_k);

        /* soh极限值判断 置位异常标志位 */
        if (soh_param.soh < soh_param.soh_limit) {
            soh_param.soh_err_flag = 1;
        }
    }

    /* 计算dod 和统计深度放电次数 */
    battery_dod_count(soc);

    return 0;
}

/**
 * @函数名称: battery_soh_get_value()
 * @功能描述: 初始化SOh模块。
 * @输入: 无
 * @输出: soh_value 指向接收SOH值的指针。
 * @返回值: 0 成功 非0 失败
 */
uint8_t battery_soh_get_value(float* soh_value)
{
    if (!soh_value) {
        return 1;
    }
    *soh_value = soh_param.soh;
    return 0;
}

/**
 * @函数名称: uint8_t battery_soh_reset()
 * @功能描述: 复位接口
 * @输入: 无
 * @输出: 无
 * @返回值: 0 成功 非0 失败
 */
uint8_t battery_soh_reset(void)
{
    soh_param.factory_capacity   = Q_INITIAL;
    soh_param.surplus_capacity   = Q_INITIAL;
    soh_param.discharge_capacity = Q_INITIAL;
    soh_param.cycle_count        = 0;
    soh_param.soh                = 100;
    soh_param.soh_limit          = SOH_LIMIT_MIN_DEFAULT;
    soh_param.dod                = 0;
    soh_param.dod_limit_max      = DOD_LIMIT_MAX_DEFAULT;
    soh_param.dod_count          = 0;
    soh_param.soh_err_flag       = 0;
    soh_param.dod_err_flag       = 0;

    return 0;
}

/***********************************************温度采集********************************************** */
int16_t usr_temperatures[5]={0};/*存储的是MCU采集到的电压值*/
NTC_ERROR_MSG_TYPAEDEF NTC_ERROR_MSG;


/**
 * @函数名称: calculate_battery_current()
 * @功能描述: 计算电芯电流值 A
 * @输入: 输入ADC采样值 ：adc_value
 * @输出: 无
 * @返回值: 计算到的电流值 A
 */
float calculate_battery_current(uint32_t adc_value) 
{
    #if 0 /* 旧设备 电流采样电阻 0.125mR */
    float adc_voltage = (adc_value / ADC_MAX_VALUE) * V_REF;
    /* 使用Beta系数公式计算温度，并进行校正 */
    float battery_current = adc_voltage * 8000.0;
    return battery_current;
    #else /* 新设备 电流采样电阻 0.25mR */
    
    float adc_voltage = (adc_value * V_REF) / ADC_MAX_VALUE;
    /* 使用Beta系数公式计算温度，并进行校正 */
    float battery_current = adc_voltage / CURRENT_RSENSE;
    // printf("adc_value = %ld,adc_voltage=%0.4f curr=%0.4f\r\n", adc_value, adc_voltage, battery_current);
    return battery_current;
    #endif
}

/**
 * @函数名称: calculate_battery_vbat_voltage()
 * @功能描述: CAN，485通讯芯片供电电源的电压值
 * @输入: 输入ADC采样值 ：adc_value
 * @输出: 无
 * @返回值: 计算到的电压值
 */
float calculate_can_battery_vbat_voltage(uint32_t adc_value) 
{
    float adc_voltage = 0;
    adc_voltage = (adc_value / ADC_MAX_VALUE) * V_REF;
    /* 使用Beta系数公式计算温度，并进行校正 */
    float can_vbat_voltage = adc_voltage * (3382 / 82);
    return can_vbat_voltage;
}

/**
 * @函数名称: calculate_battery_total_vbat_voltage()
 * @功能描述: 采集电池总电压(BAT+)
 * @输入: 输入ADC采样值 ：adc_value
 * @输出: 无
 * @返回值: 计算到的电压值
 */
float calculate_battery_total_vbat_voltage(uint32_t adc_value) 
{
    float adc_voltage = (adc_value / ADC_MAX_VALUE) * V_REF;

    /* 使用Beta系数公式计算温度，并进行校正 */
    float battery_total_voltage = (adc_voltage * 103) / 2 +(0.2 - 0.5);
    return battery_total_voltage;
}

/**
 * @函数名称: calculate_battery_current()
 * @功能描述: 计算电芯电流值
 * @输入: 输入ADC采样值 ：adc_value
 * @输出: 无
 * @返回值: 计算温度值摄氏度
 */
float calculate_mcu_ntc4(uint32_t adc_value) 
{
    float mcu_ntc4 = 0;
    float rntc = 0;
    float adc_voltage = ((adc_value * 1.0) / ADC_MAX_VALUE) * V_REF;
    rntc = (10 * adc_voltage) / (3.3 - adc_voltage);
    // /* 测试 */
    // printf("rntc=%0.4f\r\n", rntc);
    #if 0
    /* 使用Beta系数公式计算温度，并进行校正 */
    mcu_ntc4 = ((1.0 / ((1.0 / BETA_COEFFICIENT) * log((rntc) / 10) + (1.0 / (25+273.15))) - 273.15));
    #else
    /* 使用查表法计算温度 */
    mcu_ntc4 = resistance_to_temp(rntc);
    #endif
    
    return mcu_ntc4;
}

/**
 * @函数名称: calculate_mcu_board_temperture()
 * @功能描述: 计算板载温度
 * @输入: 输入ADC采样值 ：adc_value
 * @输出: 无
 * @返回值: 计算到的电流值
 */
float calculate_mcu_board_temperture(uint32_t adc_value) 
{
    float nborad_num = 0;
    float quadratic_formula = 0;
    float adc_voltage = ((adc_value * 1.0) / ADC_MAX_VALUE) * V_REF;
    /* 使用Beta系数公式计算温度，并进行校正 */
    /* IO对应OUT输出的电压系数 1.51 */
    nborad_num = adc_voltage * 1.51;
    // printf("voltage =  %lf\r\n", nborad_num);
    /* 使用手册中的一阶公式 */
    float mcu_board_temperture = (STLM20_V25 - nborad_num)/STLM20_AVG_SLOPE;
    // printf("One formula result =  %lf\r\n", mcu_board_temperture);
    /* 使用手册中的二阶公式 */
    quadratic_formula = sqrt(2196200 + ((1.8639 - nborad_num)/(3.88 * 0.000001 )));
    // printf("quadratic_formula =  %lf\r\n", quadratic_formula);
    mcu_board_temperture = quadratic_formula - 1481.96;
    // printf("quadratic_formula result =  %lf\r\n", mcu_board_temperture);
    return mcu_board_temperture;
}

/**
 * @函数名称: battery_temperature_init()
 * @功能描述: mcu温度采集初始化
 * @输入: 当前电芯数组温度：temp_arr 当前电芯数组长度：len
 * @输出: 无
 * @返回值: 无
 */
void battery_temperature_init(void)
{
    /* PD11要使能才能获取环境温度使能环境温度采集 */
    gpio_bit_set(GPIOD, GPIO_PIN_11);
    /* NTC报警标志位清零 */
    memset(&NTC_ERROR_MSG, 0, sizeof(NTC_ERROR_MSG_TYPAEDEF));
}

/**
 * @函数名称: battery_mcu_current_get()
 * @功能描述: mcu采集电流 单位A浮点数
 * @输入: 无
 * @输出: 无
 * @返回值: battery_current 电流值 A
 */
float battery_mcu_current_get(void)
{
    float battery_current = calculate_battery_current(uiDMAData[0]);

    return battery_current;
}

/**
 * @函数名称: can_vbat_mcu_voltage_get()
 * @功能描述: mcu CAN，485通讯芯片供电电源的电压采集
 * @输入: 当前的ADC值：adc_value
 * @输出: 无
 * @返回值: 电压
 */
float can_vbat_mcu_voltage_get(void)
{
    float can_vbat_voltage = calculate_can_battery_vbat_voltage(uiDMAData[1]);
    return can_vbat_voltage;
}

/**
 * @函数名称: mcu_ntc4_get()
 * @功能描述: mcu采集ntc
 * @输入: 无
 * @输出: 无
 * @返回值: 数组指针
 */
float mcu_ntc4_get(void)
{
    float ntc4 = calculate_mcu_ntc4(uiDMAData[2]);

    return ntc4;
}

/**
 * @函数名称: mcu_board_temperture_get()
 * @功能描述: mcu采集电流
 * @输入: 无
 * @输出: 无
 * @返回值: 数组指针
 */
float mcu_board_temperture_get(void)
{
    float board_temperture = calculate_mcu_board_temperture(uiDMAData[3]);

    return board_temperture;
}

/**
 * @函数名称: battery_battery_voltage_total_get()
 * @功能描述: mcu充电温度采集
 * @输入: 当前的ADC值：adc_value
 * @输出: 无
 * @返回值: 充电温度
 */
float battery_battery_voltage_total_get(void)
{
    float battery_voltage_total = calculate_battery_total_vbat_voltage(uiDMAData[4]);
    return battery_voltage_total;
}

/**
 * @函数名称: float_to_int16_round()
 * @功能描述: 浮点转换为int16_t并四舍五入
 * @输入: val 浮点值
 * @输出: 无
 * @返回值: 转换后的int16_t值
 */
static int16_t float_to_int16_round(float val)
{
    if (val >= 0.0f)
        return (int16_t)(val + 0.5f);
    else
        return (int16_t)(val - 0.5f);
}


/**
 * @函数名称: battery_temperature_filter_read()
 * @功能描述: 周期性检测函数
 * @输入: 当前的ADC值：adc_value
 * @输出: 无
 * @返回值: 无
 */
void mcu_adc_value_refresh(void)
{
    float battery_current       = 0; /* 电池电流 单位0.01A */
    float can_vbat_voltage      = 0; /* 通信CAN RS485电源电压 单位0.01V */
    float ntc4_temperature      = 0; /* 电芯4温度 单位0.01℃ 其余3个在AFE */
    float board_temperature     = 0; /* 环境温度(bms板载温度) 单位0.01℃ */
    float battery_total_voltage = 0; /* 电池总电压 单位0.01V */

    battery_current       = battery_mcu_current_get() * 100.0;        /* 电池电流 单位0.01A */
    can_vbat_voltage      = can_vbat_mcu_voltage_get() * 100.0;       /* 通信CAN RS485电源电压 单位0.01V */
    ntc4_temperature      = mcu_ntc4_get() * 100.0;                   /* 电芯4温度 单位0.01℃ 其余3个在AFE */
    board_temperature     = mcu_board_temperture_get() * 100.0;       /* 环境温度(bms板载温度) 单位0.01℃ */
    battery_total_voltage = battery_battery_voltage_total_get() * 100.0; /* 电池总电压 单位0.01V */

    usr_temperatures[0] = float_to_int16_round(battery_current);    /* 电池电流 单位10mA */
    usr_temperatures[1] = float_to_int16_round(can_vbat_voltage);   /* 通信CAN RS485电源电压 单位10mV */
    usr_temperatures[2] = float_to_int16_round(ntc4_temperature);               /* 电芯4温度 单位0.01℃ 其余3个在AFE */
    usr_temperatures[3] = float_to_int16_round(board_temperature);   /* 环境温度(bms板载温度) 单位0.01℃ */
    usr_temperatures[4] = float_to_int16_round(battery_total_voltage); /* 电池总电压 单位10mV */
}
