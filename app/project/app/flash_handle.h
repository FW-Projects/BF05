#ifndef __OUTPUT_H
#define __OUTPUT_H
#include "flash.h"
#define FLASH_HANDLE_TIME          2
#define DATA_SIZE                  2
#define FLASH_MENBER               103



/* a area data address */
#define A_LAST_DIRECT_SET_TEMP_ADDRESS           	       (0x08000000 + 1024 * 120)
#define A_LAST_DIRECT_SET_WIND_ADDRESS                     A_LAST_DIRECT_SET_TEMP_ADDRESS + DATA_SIZE          
#define A_LAST_DIRECT_SET_COLD_MODE_WIND_ADDRESS           A_LAST_DIRECT_SET_WIND_ADDRESS + DATA_SIZE            
#define A_LAST_DIRECT_SET_CALIBRATION_TEMP                 A_LAST_DIRECT_SET_COLD_MODE_WIND_ADDRESS + DATA_SIZE 
#define A_LAST_SET_COUNTDOWN_TIME                          A_LAST_DIRECT_SET_CALIBRATION_TEMP + DATA_SIZE
#define A_LAST_TEMP_UINT                                   A_LAST_SET_COUNTDOWN_TIME + DATA_SIZE               
#define A_LAST_SPEAK_STATE                                 A_LAST_TEMP_UINT + DATA_SIZE                          
#define A_LAST_DISPLAY_LOCK_STATE                          A_LAST_SPEAK_STATE + DATA_SIZE                        
#define A_LAST_FN_KEY_SET                                  A_LAST_DISPLAY_LOCK_STATE + DATA_SIZE    
#define A_LAST_ADJUST_KEY_SET                              A_LAST_FN_KEY_SET + DATA_SIZE  
#define A_LAST_OTA_STATE                                   A_LAST_ADJUST_KEY_SET + DATA_SIZE 
#define A_LAST_TOUCH_KEY_SET                               A_LAST_OTA_STATE + DATA_SIZE 
#define A_LAST_UART_STATE                                  A_LAST_TOUCH_KEY_SET + DATA_SIZE 
#define A_LAST_CH1_SET_TEMP                                A_LAST_UART_STATE + DATA_SIZE 
#define A_LAST_CH1_SET_WIND                                A_LAST_CH1_SET_TEMP + DATA_SIZE 
#define A_LAST_CH1_SET_TIME                                A_LAST_CH1_SET_WIND + DATA_SIZE 
#define A_LAST_CH2_SET_TEMP                                A_LAST_CH1_SET_TIME + DATA_SIZE 
#define A_LAST_CH2_SET_WIND                                A_LAST_CH2_SET_TEMP + DATA_SIZE 
#define A_LAST_CH2_SET_TIME                                A_LAST_CH2_SET_WIND + DATA_SIZE 
#define A_LAST_CH3_SET_TEMP                                A_LAST_CH2_SET_TIME + DATA_SIZE 
#define A_LAST_CH3_SET_WIND                                A_LAST_CH3_SET_TEMP + DATA_SIZE 
#define A_LAST_CH3_SET_TIME                                A_LAST_CH3_SET_WIND + DATA_SIZE  
#define A_LAST_CH4_SET_TEMP                                A_LAST_CH3_SET_TIME + DATA_SIZE 
#define A_LAST_CH4_SET_WIND                                A_LAST_CH4_SET_TEMP + DATA_SIZE 
#define A_LAST_CH4_SET_TIME                                A_LAST_CH4_SET_WIND + DATA_SIZE 
#define A_LAST_CODE0_PRE_TEMP                              A_LAST_CH4_SET_TIME + DATA_SIZE 
#define A_LAST_CODE0_PRE_WIND                              A_LAST_CODE0_PRE_TEMP + DATA_SIZE 
#define A_LAST_CODE0_PRE_TIME                              A_LAST_CODE0_PRE_WIND + DATA_SIZE 
#define A_LAST_CODE0_TEMP_1                                A_LAST_CODE0_PRE_TIME + DATA_SIZE 
#define A_LAST_CODE0_WIND_1                                A_LAST_CODE0_TEMP_1 + DATA_SIZE 
#define A_LAST_CODE0_TIME_1                                A_LAST_CODE0_WIND_1 + DATA_SIZE 
#define A_LAST_CODE0_TEMP_2                                A_LAST_CODE0_TIME_1 + DATA_SIZE 
#define A_LAST_CODE0_WIND_2                                A_LAST_CODE0_TEMP_2 + DATA_SIZE 
#define A_LAST_CODE0_TIME_2                                A_LAST_CODE0_WIND_2 + DATA_SIZE 
#define A_LAST_CODE0_TEMP_3                                A_LAST_CODE0_TIME_2 + DATA_SIZE 
#define A_LAST_CODE0_WIND_3                                A_LAST_CODE0_TEMP_3 + DATA_SIZE 
#define A_LAST_CODE0_TIME_3                                A_LAST_CODE0_WIND_3 + DATA_SIZE 
#define A_LAST_CODE0_TEMP_4                                A_LAST_CODE0_TIME_3 + DATA_SIZE 
#define A_LAST_CODE0_WIND_4                                A_LAST_CODE0_TEMP_4 + DATA_SIZE 
#define A_LAST_CODE0_TIME_4                                A_LAST_CODE0_WIND_4 + DATA_SIZE 
#define A_LAST_CODE1_PRE_TEMP                              A_LAST_CODE0_TIME_4 + DATA_SIZE 
#define A_LAST_CODE1_PRE_WIND                              A_LAST_CODE1_PRE_TEMP + DATA_SIZE 
#define A_LAST_CODE1_PRE_TIME                              A_LAST_CODE1_PRE_WIND + DATA_SIZE 
#define A_LAST_CODE1_TEMP_1                                A_LAST_CODE1_PRE_TIME + DATA_SIZE 
#define A_LAST_CODE1_WIND_1                                A_LAST_CODE1_TEMP_1 + DATA_SIZE 
#define A_LAST_CODE1_TIME_1                                A_LAST_CODE1_WIND_1 + DATA_SIZE 
#define A_LAST_CODE1_TEMP_2                                A_LAST_CODE1_TIME_1 + DATA_SIZE 
#define A_LAST_CODE1_WIND_2                                A_LAST_CODE1_TEMP_2 + DATA_SIZE 
#define A_LAST_CODE1_TIME_2                                A_LAST_CODE1_WIND_2 + DATA_SIZE 
#define A_LAST_CODE1_TEMP_3                                A_LAST_CODE1_TIME_2 + DATA_SIZE 
#define A_LAST_CODE1_WIND_3                                A_LAST_CODE1_TEMP_3 + DATA_SIZE 
#define A_LAST_CODE1_TIME_3                                A_LAST_CODE1_WIND_3 + DATA_SIZE 
#define A_LAST_CODE1_TEMP_4                                A_LAST_CODE1_TIME_3 + DATA_SIZE 
#define A_LAST_CODE1_WIND_4                                A_LAST_CODE1_TEMP_4 + DATA_SIZE 
#define A_LAST_CODE1_TIME_4                                A_LAST_CODE1_WIND_4 + DATA_SIZE 
#define A_LAST_CODE2_PRE_TEMP                              A_LAST_CODE1_TIME_4 + DATA_SIZE 
#define A_LAST_CODE2_PRE_WIND                              A_LAST_CODE2_PRE_TEMP + DATA_SIZE 
#define A_LAST_CODE2_PRE_TIME                              A_LAST_CODE2_PRE_WIND + DATA_SIZE 
#define A_LAST_CODE2_TEMP_1                                A_LAST_CODE2_PRE_TIME + DATA_SIZE 
#define A_LAST_CODE2_WIND_1                                A_LAST_CODE2_TEMP_1 + DATA_SIZE 
#define A_LAST_CODE2_TIME_1                                A_LAST_CODE2_WIND_1 + DATA_SIZE 
#define A_LAST_CODE2_TEMP_2                                A_LAST_CODE2_TIME_1 + DATA_SIZE 
#define A_LAST_CODE2_WIND_2                                A_LAST_CODE2_TEMP_2 + DATA_SIZE 
#define A_LAST_CODE2_TIME_2                                A_LAST_CODE2_WIND_2 + DATA_SIZE 
#define A_LAST_CODE2_TEMP_3                                A_LAST_CODE2_TIME_2 + DATA_SIZE 
#define A_LAST_CODE2_WIND_3                                A_LAST_CODE2_TEMP_3 + DATA_SIZE 
#define A_LAST_CODE2_TIME_3                                A_LAST_CODE2_WIND_3 + DATA_SIZE 
#define A_LAST_CODE2_TEMP_4                                A_LAST_CODE2_TIME_3 + DATA_SIZE 
#define A_LAST_CODE2_WIND_4                                A_LAST_CODE2_TEMP_4 + DATA_SIZE 
#define A_LAST_CODE2_TIME_4                                A_LAST_CODE2_WIND_4 + DATA_SIZE 
#define A_LAST_CODE3_PRE_TEMP                              A_LAST_CODE2_TIME_4 + DATA_SIZE 
#define A_LAST_CODE3_PRE_WIND                              A_LAST_CODE3_PRE_TEMP + DATA_SIZE 
#define A_LAST_CODE3_PRE_TIME                              A_LAST_CODE3_PRE_WIND + DATA_SIZE 
#define A_LAST_CODE3_TEMP_1                                A_LAST_CODE3_PRE_TIME + DATA_SIZE 
#define A_LAST_CODE3_WIND_1                                A_LAST_CODE3_TEMP_1 + DATA_SIZE 
#define A_LAST_CODE3_TIME_1                                A_LAST_CODE3_WIND_1 + DATA_SIZE 
#define A_LAST_CODE3_TEMP_2                                A_LAST_CODE3_TIME_1 + DATA_SIZE 
#define A_LAST_CODE3_WIND_2                                A_LAST_CODE3_TEMP_2 + DATA_SIZE 
#define A_LAST_CODE3_TIME_2                                A_LAST_CODE3_WIND_2 + DATA_SIZE 
#define A_LAST_CODE3_TEMP_3                                A_LAST_CODE3_TIME_2 + DATA_SIZE 
#define A_LAST_CODE3_WIND_3                                A_LAST_CODE3_TEMP_3 + DATA_SIZE 
#define A_LAST_CODE3_TIME_3                                A_LAST_CODE3_WIND_3 + DATA_SIZE 
#define A_LAST_CODE3_TEMP_4                                A_LAST_CODE3_TIME_3 + DATA_SIZE 
#define A_LAST_CODE3_WIND_4                                A_LAST_CODE3_TEMP_4 + DATA_SIZE 
#define A_LAST_CODE3_TIME_4                                A_LAST_CODE3_WIND_4 + DATA_SIZE 
#define A_LAST_CODE4_PRE_TEMP                              A_LAST_CODE3_TIME_4 + DATA_SIZE 
#define A_LAST_CODE4_PRE_WIND                              A_LAST_CODE4_PRE_TEMP + DATA_SIZE 
#define A_LAST_CODE4_PRE_TIME                              A_LAST_CODE4_PRE_WIND + DATA_SIZE 
#define A_LAST_CODE4_TEMP_1                                A_LAST_CODE4_PRE_TIME + DATA_SIZE 
#define A_LAST_CODE4_WIND_1                                A_LAST_CODE4_TEMP_1 + DATA_SIZE 
#define A_LAST_CODE4_TIME_1                                A_LAST_CODE4_WIND_1 + DATA_SIZE 
#define A_LAST_CODE4_TEMP_2                                A_LAST_CODE4_TIME_1 + DATA_SIZE 
#define A_LAST_CODE4_WIND_2                                A_LAST_CODE4_TEMP_2 + DATA_SIZE 
#define A_LAST_CODE4_TIME_2                                A_LAST_CODE4_WIND_2 + DATA_SIZE 
#define A_LAST_CODE4_TEMP_3                                A_LAST_CODE4_TIME_2 + DATA_SIZE 
#define A_LAST_CODE4_WIND_3                                A_LAST_CODE4_TEMP_3 + DATA_SIZE 
#define A_LAST_CODE4_TIME_3                                A_LAST_CODE4_WIND_3 + DATA_SIZE 
#define A_LAST_CODE4_TEMP_4                                A_LAST_CODE4_TIME_3 + DATA_SIZE 
#define A_LAST_CODE4_WIND_4                                A_LAST_CODE4_TEMP_4 + DATA_SIZE 
#define A_LAST_CODE4_TIME_4                                A_LAST_CODE4_WIND_4 + DATA_SIZE 
#define A_LAST_FWG2_WORK_MODE                              A_LAST_CODE4_TIME_4 + DATA_SIZE 
#define A_LAST_CYCLONE_SET_CALIBRATION_TEMP                A_LAST_FWG2_WORK_MODE + DATA_SIZE 
#define A_FLASH_VERSION_ADDRESS                            A_LAST_CYCLONE_SET_CALIBRATION_TEMP + DATA_SIZE   
                                                                            
/* b area data address */      
#define B_LAST_DIRECT_SET_TEMP_ADDRESS           	       (0x08000000 + 1024 * 125)
#define B_LAST_DIRECT_SET_WIND_ADDRESS                     B_LAST_DIRECT_SET_TEMP_ADDRESS + DATA_SIZE           
#define B_LAST_DIRECT_SET_COLD_MODE_WIND_ADDRESS           B_LAST_DIRECT_SET_WIND_ADDRESS + DATA_SIZE                                                                 
#define B_LAST_DIRECT_SET_CALIBRATION_TEMP                 B_LAST_DIRECT_SET_COLD_MODE_WIND_ADDRESS + DATA_SIZE
#define B_LAST_SET_COUNTDOWN_TIME                          B_LAST_DIRECT_SET_CALIBRATION_TEMP + DATA_SIZE
#define B_LAST_TEMP_UINT                                   B_LAST_SET_COUNTDOWN_TIME + DATA_SIZE
#define B_LAST_SPEAK_STATE                                 B_LAST_TEMP_UINT + DATA_SIZE
#define B_LAST_DISPLAY_LOCK_STATE                          B_LAST_SPEAK_STATE + DATA_SIZE
#define B_LAST_FN_KEY_SET                                  B_LAST_DISPLAY_LOCK_STATE + DATA_SIZE
#define B_LAST_ADJUST_KEY_SET                              B_LAST_FN_KEY_SET + DATA_SIZE
#define B_LAST_OTA_STATE                                   B_LAST_ADJUST_KEY_SET + DATA_SIZE
#define B_LAST_TOUCH_KEY_SET                               B_LAST_OTA_STATE + DATA_SIZE
#define B_LAST_UART_STATE                                  B_LAST_TOUCH_KEY_SET + DATA_SIZE                                           
#define B_LAST_CH1_SET_TEMP                                B_LAST_UART_STATE + DATA_SIZE
#define B_LAST_CH1_SET_WIND                                B_LAST_CH1_SET_TEMP + DATA_SIZE
#define B_LAST_CH1_SET_TIME                                B_LAST_CH1_SET_WIND + DATA_SIZE
#define B_LAST_CH2_SET_TEMP                                B_LAST_CH1_SET_TIME + DATA_SIZE
#define B_LAST_CH2_SET_WIND                                B_LAST_CH2_SET_TEMP + DATA_SIZE
#define B_LAST_CH2_SET_TIME                                B_LAST_CH2_SET_WIND + DATA_SIZE
#define B_LAST_CH3_SET_TEMP                                B_LAST_CH2_SET_TIME + DATA_SIZE
#define B_LAST_CH3_SET_WIND                                B_LAST_CH3_SET_TEMP + DATA_SIZE 
#define B_LAST_CH3_SET_TIME                                B_LAST_CH3_SET_WIND + DATA_SIZE
#define B_LAST_CH4_SET_TEMP                                B_LAST_CH3_SET_TIME + DATA_SIZE
#define B_LAST_CH4_SET_WIND                                B_LAST_CH4_SET_TEMP + DATA_SIZE
#define B_LAST_CH4_SET_TIME                                B_LAST_CH4_SET_WIND + DATA_SIZE
#define B_LAST_CODE0_PRE_TEMP                              B_LAST_CH4_SET_TIME + DATA_SIZE
#define B_LAST_CODE0_PRE_WIND                              B_LAST_CODE0_PRE_TEMP + DATA_SIZE
#define B_LAST_CODE0_PRE_TIME                              B_LAST_CODE0_PRE_WIND + DATA_SIZE
#define B_LAST_CODE0_TEMP_1                                B_LAST_CODE0_PRE_TIME + DATA_SIZE
#define B_LAST_CODE0_WIND_1                                B_LAST_CODE0_TEMP_1 + DATA_SIZE
#define B_LAST_CODE0_TIME_1                                B_LAST_CODE0_WIND_1 + DATA_SIZE
#define B_LAST_CODE0_TEMP_2                                B_LAST_CODE0_TIME_1 + DATA_SIZE
#define B_LAST_CODE0_WIND_2                                B_LAST_CODE0_TEMP_2 + DATA_SIZE
#define B_LAST_CODE0_TIME_2                                B_LAST_CODE0_WIND_2 + DATA_SIZE
#define B_LAST_CODE0_TEMP_3                                B_LAST_CODE0_TIME_2 + DATA_SIZE
#define B_LAST_CODE0_WIND_3                                B_LAST_CODE0_TEMP_3 + DATA_SIZE
#define B_LAST_CODE0_TIME_3                                B_LAST_CODE0_WIND_3 + DATA_SIZE
#define B_LAST_CODE0_TEMP_4                                B_LAST_CODE0_TIME_3 + DATA_SIZE
#define B_LAST_CODE0_WIND_4                                B_LAST_CODE0_TEMP_4 + DATA_SIZE
#define B_LAST_CODE0_TIME_4                                B_LAST_CODE0_WIND_4 + DATA_SIZE
#define B_LAST_CODE1_PRE_TEMP                              B_LAST_CODE0_TIME_4 + DATA_SIZE
#define B_LAST_CODE1_PRE_WIND                              B_LAST_CODE1_PRE_TEMP + DATA_SIZE
#define B_LAST_CODE1_PRE_TIME                              B_LAST_CODE1_PRE_WIND + DATA_SIZE
#define B_LAST_CODE1_TEMP_1                                B_LAST_CODE1_PRE_TIME + DATA_SIZE
#define B_LAST_CODE1_WIND_1                                B_LAST_CODE1_TEMP_1 + DATA_SIZE
#define B_LAST_CODE1_TIME_1                                B_LAST_CODE1_WIND_1 + DATA_SIZE
#define B_LAST_CODE1_TEMP_2                                B_LAST_CODE1_TIME_1 + DATA_SIZE
#define B_LAST_CODE1_WIND_2                                B_LAST_CODE1_TEMP_2 + DATA_SIZE
#define B_LAST_CODE1_TIME_2                                B_LAST_CODE1_WIND_2 + DATA_SIZE
#define B_LAST_CODE1_TEMP_3                                B_LAST_CODE1_TIME_2 + DATA_SIZE
#define B_LAST_CODE1_WIND_3                                B_LAST_CODE1_TEMP_3 + DATA_SIZE
#define B_LAST_CODE1_TIME_3                                B_LAST_CODE1_WIND_3 + DATA_SIZE
#define B_LAST_CODE1_TEMP_4                                B_LAST_CODE1_TIME_3 + DATA_SIZE
#define B_LAST_CODE1_WIND_4                                B_LAST_CODE1_TEMP_4 + DATA_SIZE
#define B_LAST_CODE1_TIME_4                                B_LAST_CODE1_WIND_4 + DATA_SIZE
#define B_LAST_CODE2_PRE_TEMP                              B_LAST_CODE1_TIME_4 + DATA_SIZE
#define B_LAST_CODE2_PRE_WIND                              B_LAST_CODE2_PRE_TEMP + DATA_SIZE
#define B_LAST_CODE2_PRE_TIME                              B_LAST_CODE2_PRE_WIND + DATA_SIZE
#define B_LAST_CODE2_TEMP_1                                B_LAST_CODE2_PRE_TIME + DATA_SIZE
#define B_LAST_CODE2_WIND_1                                B_LAST_CODE2_TEMP_1 + DATA_SIZE
#define B_LAST_CODE2_TIME_1                                B_LAST_CODE2_WIND_1 + DATA_SIZE
#define B_LAST_CODE2_TEMP_2                                B_LAST_CODE2_TIME_1 + DATA_SIZE
#define B_LAST_CODE2_WIND_2                                B_LAST_CODE2_TEMP_2 + DATA_SIZE
#define B_LAST_CODE2_TIME_2                                B_LAST_CODE2_WIND_2 + DATA_SIZE
#define B_LAST_CODE2_TEMP_3                                B_LAST_CODE2_TIME_2 + DATA_SIZE
#define B_LAST_CODE2_WIND_3                                B_LAST_CODE2_TEMP_3 + DATA_SIZE
#define B_LAST_CODE2_TIME_3                                B_LAST_CODE2_WIND_3 + DATA_SIZE
#define B_LAST_CODE2_TEMP_4                                B_LAST_CODE2_TIME_3 + DATA_SIZE
#define B_LAST_CODE2_WIND_4                                B_LAST_CODE2_TEMP_4 + DATA_SIZE
#define B_LAST_CODE2_TIME_4                                B_LAST_CODE2_WIND_4 + DATA_SIZE
#define B_LAST_CODE3_PRE_TEMP                              B_LAST_CODE2_TIME_4 + DATA_SIZE
#define B_LAST_CODE3_PRE_WIND                              B_LAST_CODE3_PRE_TEMP + DATA_SIZE
#define B_LAST_CODE3_PRE_TIME                              B_LAST_CODE3_PRE_WIND + DATA_SIZE
#define B_LAST_CODE3_TEMP_1                                B_LAST_CODE3_PRE_TIME + DATA_SIZE
#define B_LAST_CODE3_WIND_1                                B_LAST_CODE3_TEMP_1 + DATA_SIZE
#define B_LAST_CODE3_TIME_1                                B_LAST_CODE3_WIND_1 + DATA_SIZE
#define B_LAST_CODE3_TEMP_2                                B_LAST_CODE3_TIME_1 + DATA_SIZE
#define B_LAST_CODE3_WIND_2                                B_LAST_CODE3_TEMP_2 + DATA_SIZE
#define B_LAST_CODE3_TIME_2                                B_LAST_CODE3_WIND_2 + DATA_SIZE
#define B_LAST_CODE3_TEMP_3                                B_LAST_CODE3_TIME_2 + DATA_SIZE
#define B_LAST_CODE3_WIND_3                                B_LAST_CODE3_TEMP_3 + DATA_SIZE
#define B_LAST_CODE3_TIME_3                                B_LAST_CODE3_WIND_3 + DATA_SIZE
#define B_LAST_CODE3_TEMP_4                                B_LAST_CODE3_TIME_3 + DATA_SIZE
#define B_LAST_CODE3_WIND_4                                B_LAST_CODE3_TEMP_4 + DATA_SIZE
#define B_LAST_CODE3_TIME_4                                B_LAST_CODE3_WIND_4 + DATA_SIZE
#define B_LAST_CODE4_PRE_TEMP                              B_LAST_CODE3_TIME_4 + DATA_SIZE
#define B_LAST_CODE4_PRE_WIND                              B_LAST_CODE4_PRE_TEMP + DATA_SIZE
#define B_LAST_CODE4_PRE_TIME                              B_LAST_CODE4_PRE_WIND + DATA_SIZE
#define B_LAST_CODE4_TEMP_1                                B_LAST_CODE4_PRE_TIME + DATA_SIZE
#define B_LAST_CODE4_WIND_1                                B_LAST_CODE4_TEMP_1 + DATA_SIZE
#define B_LAST_CODE4_TIME_1                                B_LAST_CODE4_WIND_1 + DATA_SIZE
#define B_LAST_CODE4_TEMP_2                                B_LAST_CODE4_TIME_1 + DATA_SIZE
#define B_LAST_CODE4_WIND_2                                B_LAST_CODE4_TEMP_2 + DATA_SIZE
#define B_LAST_CODE4_TIME_2                                B_LAST_CODE4_WIND_2 + DATA_SIZE
#define B_LAST_CODE4_TEMP_3                                B_LAST_CODE4_TIME_2 + DATA_SIZE
#define B_LAST_CODE4_WIND_3                                B_LAST_CODE4_TEMP_3 + DATA_SIZE
#define B_LAST_CODE4_TIME_3                                B_LAST_CODE4_WIND_3 + DATA_SIZE
#define B_LAST_CODE4_TEMP_4                                B_LAST_CODE4_TIME_3 + DATA_SIZE
#define B_LAST_CODE4_WIND_4                                B_LAST_CODE4_TEMP_4 + DATA_SIZE
#define B_LAST_CODE4_TIME_4                                B_LAST_CODE4_WIND_4 + DATA_SIZE
#define B_LAST_FWG2_WORK_MODE                              B_LAST_CODE4_TIME_4 + DATA_SIZE
#define B_LAST_CYCLONE_SET_CALIBRATION_TEMP                B_LAST_FWG2_WORK_MODE + DATA_SIZE
#define B_FLASH_VERSION_ADDRESS                            B_LAST_CYCLONE_SET_CALIBRATION_TEMP + DATA_SIZE
						 
typedef enum
{
    FLASH_START = 0,
    FLASH_DIRECT_DATA,   
	FLASH_GENERAL_DATA,
    FLASH_GENERAL_CH_DATA,
	FLASH_GENERAL_CODE0_DATA,
	FLASH_GENERAL_CODE1_DATA,
	FLASH_GENERAL_CODE2_DATA,
	FLASH_GENERAL_CODE3_DATA,
	FLASH_GENERAL_CODE4_DATA,
    FLASH_VER,
	FLASH_FINSH,
} flash_state_t;


typedef struct
{
    flash_state_t state;
} flash_handle_t;

void FlashProc(void);

#endif
