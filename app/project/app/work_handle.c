#include "work_handle.h"
#include "FWG2_handle.h"
#include "PID_operation.h"
#include "beep_handle.h"

void check_handle_position(void)
{

    /* check direct handle position */
    if (gpio_input_data_bit_read(GPIOC, GPIO_PINS_4 ))
    {
        sFWG2_t.Direct_handle_position = NOT_IN_POSSITION;
    }
    else
    {
        sFWG2_t.Direct_handle_position = IN_POSSITION;
    }
}


void Direct_handle_switch(void)
{
    check_handle_position();

    if (sFWG2_t.general_parameter.work_mode == NORMAL)
    {
        switch (sFWG2_t.Direct_handle_state)
        {
        case HANDLE_SLEEP:
            if (sFWG2_t.Direct_handle_position == NOT_IN_POSSITION)
            {
                sFWG2_t.Direct_handle_state = HANDLE_WORKING;
            }
            else
            {
                sFWG2_t.Direct_handle_state = HANDLE_SLEEP;
            }

            sFWG2_t.Direct_handle_parameter.last_state = HANDLE_SLEEP;
            sFWG2_t.Direct_handle_parameter.error_time = 0;
            break;

        case HANDLE_WORKING:
#if 1
            if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE || sFWG2_t.Direct_handle_work_mode == EN_WORKING_MODE)
            {
                if (sFWG2_t.Direct_handle_work_mode == EN_WORKING_MODE && sFWG2_t.Direct_handle_position == IN_POSSITION)
                {
                    sFWG2_t.Direct_handle_work_mode = NORMAL_MODE;
                }

//                if (((sFWG2_t.Direct_handle_parameter.actual_temp <= LOW_TEMP_VAL  || \
//                        sFWG2_t.Direct_handle_parameter.actual_temp >= OVER_TEMP_VAL || \
//                        sFWG2_t.Direct_handle_parameter.actual_wind < LOW_WIND_RATE)) && \
//                        sFWG2_t.Direct_handle_position == NOT_IN_POSSITION)
				                if (((sFWG2_t.Direct_handle_parameter.actual_temp <= LOW_TEMP_VAL  || \
                        sFWG2_t.Direct_handle_parameter.actual_temp >= OVER_TEMP_VAL)) && \
                        sFWG2_t.Direct_handle_position == NOT_IN_POSSITION)
                {
                    sFWG2_t.Direct_handle_parameter.error_time++;

                    if (sFWG2_t.Direct_handle_parameter.error_time >= ERROR_TIME_OUT / 2)
                    {
//                        if (sFWG2_t.Direct_handle_parameter.actual_temp >= OVER_TEMP_VAL && \
//                                sFWG2_t.Direct_handle_parameter.actual_wind >= LOW_WIND_RATE)
						if (sFWG2_t.Direct_handle_parameter.actual_temp >= OVER_TEMP_VAL)
                        {
                            sFWG2_t.Direct_handle_error_state = HANDLE_OVER_TEMP_ERR;
                            sFWG2_t.Direct_handle_parameter.last_state = HANDLE_WORKING;
                            sFWG2_t.Direct_handle_parameter.error_time = 0;
                            break;
                        }
                    }

                    if (sFWG2_t.Direct_handle_parameter.error_time >= ERROR_TIME_OUT)
                    {
//                        if (sFWG2_t.Direct_handle_parameter.actual_temp <= LOW_TEMP_VAL && \
//                                sFWG2_t.Direct_handle_parameter.actual_wind >= LOW_WIND_RATE)
						if (sFWG2_t.Direct_handle_parameter.actual_temp <= LOW_TEMP_VAL)
                        {
                            sFWG2_t.Direct_handle_error_state = HANDLE_LOW_TEMP_ERR;
                            sFWG2_t.Direct_handle_parameter.last_state = HANDLE_WORKING;
                            sFWG2_t.Direct_handle_parameter.error_time = 0;
                            break;
                        }
                        else if (sFWG2_t.Direct_handle_parameter.actual_wind < LOW_WIND_RATE)
                        {
                            sFWG2_t.Direct_handle_error_state = HANDLE_FAN_ERR;
                        }

                        sFWG2_t.Direct_handle_parameter.last_state = HANDLE_WORKING;
                        sFWG2_t.Direct_handle_parameter.error_time = 0;
                        break;
                    }
                }
                else
                {
                    sFWG2_t.Direct_handle_parameter.error_time = 0;
                }

                if (sFWG2_t.Direct_handle_position == IN_POSSITION)
                {
                    if (sFWG2_t.Direct_handle_parameter.actual_temp <= 70)
                    {
                        sFWG2_t.Direct_handle_parameter.sleep_time++;

                        if (sFWG2_t.Direct_handle_parameter.sleep_time > SLEEP_TIME_OUT)
                        {
                            sFWG2_t.Direct_handle_state = HANDLE_SLEEP;
                            sFWG2_t.Direct_handle_parameter.sleep_time = 0;
                            sbeep.status = BEEP_LONG;
                            sFWG2_t.Direct_handle_parameter.last_state = HANDLE_WORKING;
                            break;
                        }
                    }
                }
            }

#endif
            else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
            {
                /* only check fan state */
//                if ((sFWG2_t.Direct_handle_parameter.actual_wind < LOW_WIND_RATE))
//                {
//                    sFWG2_t.Direct_handle_parameter.error_time++;

//                    if (sFWG2_t.Direct_handle_parameter.error_time >= ERROR_TIME_OUT)
//                    {
//                        if (sFWG2_t.Direct_handle_parameter.actual_wind <= LOW_WIND_RATE)
//                        {
//                            sFWG2_t.Direct_handle_error_state = HANDLE_FAN_ERR;
//                            break;
//                        }
//                    }
//                }
            }

            break;
        }
    }
    else if (sFWG2_t.general_parameter.work_mode == CODE)
    {
        if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_START)
        {
			if(sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
			{
			    if (((sFWG2_t.Direct_handle_parameter.actual_temp <= LOW_TEMP_VAL  || \
                    sFWG2_t.Direct_handle_parameter.actual_temp >= OVER_TEMP_VAL || \
                    sFWG2_t.Direct_handle_parameter.actual_wind < LOW_WIND_RATE)) && \
                    sFWG2_t.Direct_handle_position == NOT_IN_POSSITION)
            {
                sFWG2_t.Direct_handle_parameter.error_time++;

                if (sFWG2_t.Direct_handle_parameter.error_time >= ERROR_TIME_OUT / 2)
                {
                    if (sFWG2_t.Direct_handle_parameter.actual_temp >= OVER_TEMP_VAL && \
                            sFWG2_t.Direct_handle_parameter.actual_wind >= LOW_WIND_RATE)
                    {
                        sFWG2_t.Direct_handle_error_state = HANDLE_OVER_TEMP_ERR;
                        sFWG2_t.Direct_handle_parameter.last_state = HANDLE_WORKING;
                        sFWG2_t.Direct_handle_parameter.error_time = 0;
                    }
                }

                if (sFWG2_t.Direct_handle_parameter.error_time >= ERROR_TIME_OUT)
                {
                    if (sFWG2_t.Direct_handle_parameter.actual_temp <= LOW_TEMP_VAL && \
                            sFWG2_t.Direct_handle_parameter.actual_wind >= LOW_WIND_RATE)
                    {
                        sFWG2_t.Direct_handle_error_state = HANDLE_LOW_TEMP_ERR;
                        sFWG2_t.Direct_handle_parameter.last_state = HANDLE_WORKING;
                        sFWG2_t.Direct_handle_parameter.error_time = 0;
                    }
                    else if (sFWG2_t.Direct_handle_parameter.actual_wind < LOW_WIND_RATE)
                    {
                        sFWG2_t.Direct_handle_error_state = HANDLE_FAN_ERR;
                    }

                    sFWG2_t.Direct_handle_parameter.last_state = HANDLE_WORKING;
                    sFWG2_t.Direct_handle_parameter.error_time = 0;
                }
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.error_time = 0;
            }
			}
            
        }
    }
}




