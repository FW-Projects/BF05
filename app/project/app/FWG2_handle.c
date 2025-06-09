#include "FWG2_handle.h"

FWG2_Handle sFWG2_t ;

uint8_t FWG2_Init(FWG2_Handle * FWG2)
{
	FWG2->FWG2_STATE                                    = FWG2_INIT,

	/* start the Direct handle init of  */
	FWG2->Direct_handle_parameter.actual_temp           = 0,      
	FWG2->Direct_handle_parameter.actual_wind           = 0,	      
	FWG2->Direct_handle_parameter.set_temp              = 200, 
	FWG2->Direct_handle_parameter.set_wind              = 50, 
    FWG2->Direct_handle_parameter.cold_mode_set_wind    = 50,   
	FWG2->Direct_handle_parameter.set_time              = 40,      
	FWG2->Direct_handle_parameter.sleep_time            = 0,		      
	FWG2->Direct_handle_parameter.error_time            = 0,      
	FWG2->Direct_handle_parameter.last_state            = HANDLE_SLEEP,      
                                                     
	FWG2->Direct_handle_state                           = HANDLE_SLEEP,      
	FWG2->Direct_handle_position                        = NOT_IN_POSSITION,      
	FWG2->Direct_handle_error_state                     = HANDLE_OK,    

    FWG2->Direct_handle_parameter.quick_work_temp          = 50;
    FWG2->Direct_handle_parameter.quick_work_time          = 60; 
    FWG2->Direct_handle_parameter.quick_work_time_display  = 0;	
	/* end the  Direct handle init of*/              
                                                     
	/* start the fwg2  init of  */                   
	FWG2->general_parameter.fwg2_page                   = PAGE_MAIN,      
	FWG2->general_parameter.code_mode_step              = CODE_PRE_HEAT,      
	FWG2->general_parameter.work_mode                   = NORMAL,      
	/* end the fwg2 page init of  */
	
	
	/* start the general init of  */
	FWG2->general_parameter.temp_uint                   = CELSIUS,
	FWG2->general_parameter.speak_state                 = SPEAKER_OPEN ,
	FWG2->general_parameter.display_lock_state          = UNLOCK,
    FWG2->general_parameter.fn_key_set                  = SELECT_COLD_WIN_MODE,
	FWG2->general_parameter.ota_state                   = OTA_OFF,
	FWG2->general_parameter.touch_key_set               = TOUCH_CLOSE,
	FWG2->general_parameter.uart_state                  = UART_CLOSE,
	
	FWG2->general_parameter.reset_fwg2_flag	            = false;
	FWG2->general_parameter.relay_open_flag             = false;
	FWG2->general_parameter.countdown_flag              = false;
	
	FWG2->general_parameter.software_version[0]         ='1',
	FWG2->general_parameter.software_version[1]         ='.',
	FWG2->general_parameter.software_version[2]         ='0',
	FWG2->general_parameter.software_version[3]         ='.',
	FWG2->general_parameter.software_version[4]         ='0',
	
	
	FWG2->general_parameter.hardware_version[0]         ='1',
	FWG2->general_parameter.hardware_version[1]         ='.',
	FWG2->general_parameter.hardware_version[2]         ='0',
	FWG2->general_parameter.hardware_version[3]         ='.',
	FWG2->general_parameter.hardware_version[4]         ='0',
	

	
	FWG2->general_parameter.code_ch                     = 0,
	FWG2->general_parameter.ch                          = 0,
    FWG2->general_parameter.countdown_time              = 10,
	FWG2->general_parameter.mcu_temp                    = 0,

	/* ch set */
	FWG2->general_parameter.ch1_set_temp                = 300,
	FWG2->general_parameter.ch1_set_wind                = 40,
	FWG2->general_parameter.ch1_set_time                = 30,
	FWG2->general_parameter.ch2_set_temp                = 320,
	FWG2->general_parameter.ch2_set_wind                = 50,
	FWG2->general_parameter.ch2_set_time                = 40,
	FWG2->general_parameter.ch3_set_temp                = 350,
	FWG2->general_parameter.ch3_set_wind                = 60,
	FWG2->general_parameter.ch3_set_time                = 50,
	FWG2->general_parameter.ch4_set_temp                = 380,
	FWG2->general_parameter.ch4_set_wind                = 80,
	FWG2->general_parameter.ch4_set_time                = 60,
	
	/* code 0 set */
	FWG2->general_parameter.code0_pre_temp              = 100,
	FWG2->general_parameter.code0_pre_wind              = 20,
	FWG2->general_parameter.code0_pre_time              = 60,
	FWG2->general_parameter.code0_temp_1                = 200,
	FWG2->general_parameter.code0_wind_1                = 50,
	FWG2->general_parameter.code0_time_1                = 10,
	FWG2->general_parameter.code0_temp_2                = 200,
	FWG2->general_parameter.code0_wind_2                = 50,
	FWG2->general_parameter.code0_time_2                = 10,
	FWG2->general_parameter.code0_temp_3                = 200,
	FWG2->general_parameter.code0_wind_3                = 50,
	FWG2->general_parameter.code0_time_3                = 10,
	FWG2->general_parameter.code0_temp_4                = 200,
	FWG2->general_parameter.code0_wind_4                = 50,
	FWG2->general_parameter.code0_time_4                = 10,

	/* code 1 set */
	FWG2->general_parameter.code1_pre_temp              = 100,
	FWG2->general_parameter.code1_pre_wind              = 20,
	FWG2->general_parameter.code1_pre_time              = 60,
	FWG2->general_parameter.code1_temp_1                = 200,
	FWG2->general_parameter.code1_wind_1                = 50,
	FWG2->general_parameter.code1_time_1                = 10,
	FWG2->general_parameter.code1_temp_2                = 200,
	FWG2->general_parameter.code1_wind_2                = 50,
	FWG2->general_parameter.code1_time_2                = 10,
	FWG2->general_parameter.code1_temp_3                = 200,
	FWG2->general_parameter.code1_wind_3                = 50,
	FWG2->general_parameter.code1_time_3                = 10,
	FWG2->general_parameter.code1_temp_4                = 200,
	FWG2->general_parameter.code1_wind_4                = 50,
	FWG2->general_parameter.code1_time_4                = 10,

	/* code 2 set */
	FWG2->general_parameter.code2_pre_temp              = 100,
	FWG2->general_parameter.code2_pre_wind              = 20,
	FWG2->general_parameter.code2_pre_time              = 60,
	FWG2->general_parameter.code2_temp_1                = 200,
	FWG2->general_parameter.code2_wind_1                = 50,
	FWG2->general_parameter.code2_time_1                = 10,
	FWG2->general_parameter.code2_temp_2                = 200,
	FWG2->general_parameter.code2_wind_2                = 50,
	FWG2->general_parameter.code2_time_2                = 10,
	FWG2->general_parameter.code2_temp_3                = 200,
	FWG2->general_parameter.code2_wind_3                = 50,
	FWG2->general_parameter.code2_time_3                = 10,
	FWG2->general_parameter.code2_temp_4                = 200,
	FWG2->general_parameter.code2_wind_4                = 50,
	FWG2->general_parameter.code2_time_4                = 10,

	/* code 3 set */
	FWG2->general_parameter.code3_pre_temp              = 100,
	FWG2->general_parameter.code3_pre_wind              = 20,
	FWG2->general_parameter.code3_pre_time              = 60,
	FWG2->general_parameter.code3_temp_1                = 200,
	FWG2->general_parameter.code3_wind_1                = 50,
	FWG2->general_parameter.code3_time_1                = 10,
	FWG2->general_parameter.code3_temp_2                = 200,
	FWG2->general_parameter.code3_wind_2                = 50,
	FWG2->general_parameter.code3_time_2                = 10,
	FWG2->general_parameter.code3_temp_3                = 200,
	FWG2->general_parameter.code3_wind_3                = 50,
	FWG2->general_parameter.code3_time_3                = 10,
	FWG2->general_parameter.code3_temp_4                = 200,
	FWG2->general_parameter.code3_wind_4                = 50,
	FWG2->general_parameter.code3_time_4                = 10,

	/* code 4 set */
	FWG2->general_parameter.code4_pre_temp              = 100,
	FWG2->general_parameter.code4_pre_wind              = 20,
	FWG2->general_parameter.code4_pre_time              = 60,
	FWG2->general_parameter.code4_temp_1                = 200,
	FWG2->general_parameter.code4_wind_1                = 50,
	FWG2->general_parameter.code4_time_1                = 10,
	FWG2->general_parameter.code4_temp_2                = 200,
	FWG2->general_parameter.code4_wind_2                = 50,
	FWG2->general_parameter.code4_time_2                = 10,
	FWG2->general_parameter.code4_temp_3                = 200,
	FWG2->general_parameter.code4_wind_3                = 50,
	FWG2->general_parameter.code4_time_3                = 10,
	FWG2->general_parameter.code4_temp_4                = 200,
	FWG2->general_parameter.code4_wind_4                = 50,
	FWG2->general_parameter.code4_time_4                = 10,

	/* en working set */
	FWG2->general_parameter.working_time                = 0;
	/* end the general init of*/

	return 0;
}



