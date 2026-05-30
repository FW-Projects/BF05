#include "output_handle.h"
#include "work_handle.h"
#include "FWG2_handle.h"
int8_t direct_handle_temp_cal_buff[41] = {8, 2, 7, 6, 6, 5, 5, 4, 4, 4,
                                           3, 3, 3, 2, 2, 2, 2, 2, 1, 1,
                                           2, 2, 1, 1, 1, 0,-1,-1,(-1),-2,
	                                      -2,-2,-2,-4, -5, -5, -6,-7,-8,-9,-15 
                                        };


int8_t direct_handle_wind_20_cal_buff[41] = {12, 7, 7, 6, 4, 4, 3, 2, 0, -1,
                                             -2, -3, -4, -3, -3, -3, -4, -4, -4, -4,
                                             -5, -9, -11, -11, -11, -13, -14, -16, (-18), -19,
                                             -21, -22, -23, -24, -24, -24, -25, -27, -30, -33, -34
                                            };

/*
    Direct  handle FAN: TMR9 CH2

	Direct  handle HOT: TMR2 CH1

*/



void fan_control(void)
{
    /* start the Direct handle fan output of  */
    if (sFWG2_t.general_parameter.work_mode == NORMAL && sFWG2_t.Direct_handle_error_state == HANDLE_OK)
    {
        /* fan control with normal mode */
        if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
        {
            if (sFWG2_t.Direct_handle_state == HANDLE_SLEEP)
            {
//				if (sFWG2_t.Direct_handle_parameter.actual_temp >= 250)
//                {
//                    sFWG2_t.Direct_handle_parameter.stop_set_wind = 100;
//                    /* open fan output with a half of max set val*/
//                    tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, MAX_SET_WIND_VAL / 2 * 1.13 + 30);
//                }
//                else if (sFWG2_t.Direct_handle_parameter.actual_temp >= 70 && sFWG2_t.Direct_handle_parameter.actual_temp < 250)
//                {
//                    /* open fan output with actual temp change*/
//                    sFWG2_t.Direct_handle_parameter.stop_set_wind = sFWG2_t.Direct_handle_parameter.actual_temp * 0.4;
//                    tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.Direct_handle_parameter.stop_set_wind * 1.13 + 30);
//                }
				
				
                /* keep fan output until the temp below 60 */
                 if (sFWG2_t.Direct_handle_parameter.actual_temp >= 60 && sFWG2_t.Direct_handle_parameter.actual_temp < 70)
                {
                    tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, 20 * 1.13 + 30);
                }
                else
                {
                    /* close fan output */
                    tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, 0);
                }
            }
            else if (sFWG2_t.Direct_handle_state == HANDLE_WORKING && sFWG2_t.Direct_handle_position == IN_POSSITION)
            {
                if (sFWG2_t.Direct_handle_parameter.actual_temp >= 250)
                {
                    sFWG2_t.Direct_handle_parameter.stop_set_wind = 100;
                    /* open fan output with a half of max set val*/
                    tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, MAX_SET_WIND_VAL / 2 * 1.13 + 30);
                }
                else if (sFWG2_t.Direct_handle_parameter.actual_temp >= 70 && sFWG2_t.Direct_handle_parameter.actual_temp < 250)
                {
                    /* open fan output with actual temp change*/
                    sFWG2_t.Direct_handle_parameter.stop_set_wind = sFWG2_t.Direct_handle_parameter.actual_temp * 0.4;
                    tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.Direct_handle_parameter.stop_set_wind * 1.13 + 30);
                }
                else
                {
                    /* close fan output */
                    tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, 0);
                }
            }
            else if (sFWG2_t.Direct_handle_state == HANDLE_WORKING && sFWG2_t.Direct_handle_position == NOT_IN_POSSITION)
            {
                /* open fan output with user set val */
                tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.Direct_handle_parameter.set_wind * 1.13 + 30);
            }
            
        }
        else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
        {
            tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.Direct_handle_parameter.cold_mode_set_wind * 1.13 + 30);
        }
    }
    else if (sFWG2_t.general_parameter.work_mode == CODE && sFWG2_t.Direct_handle_error_state == HANDLE_OK)
    {
        if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
        {
            if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_START)
            {
                if (sFWG2_t.general_parameter.code_ch == 0)
                {
                    if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
                    {
                        tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.general_parameter.code0_pre_wind * 1.13 + 30);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
                    {
                        tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.general_parameter.code0_wind_1 * 1.13 + 30);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
                    {
                        tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.general_parameter.code0_wind_2 * 1.13 + 30);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
                    {
                        tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.general_parameter.code0_wind_3 * 1.13 + 30);
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 1)
                {
                    if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
                    {
                        tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.general_parameter.code1_pre_wind * 1.13 + 30);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
                    {
                        tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.general_parameter.code1_wind_1 * 1.13 + 30);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
                    {
                        tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.general_parameter.code1_wind_2 * 1.13 + 30);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
                    {
                        tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.general_parameter.code1_wind_3 * 1.13 + 30);
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 2)
                {
                    if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
                    {
                        tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.general_parameter.code2_pre_wind * 1.13 + 30);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
                    {
                        tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.general_parameter.code2_wind_1 * 1.13 + 30);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
                    {
                        tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.general_parameter.code2_wind_2 * 1.13 + 30);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
                    {
                        tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.general_parameter.code2_wind_3 * 1.13 + 30);
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 3)
                {
                    if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
                    {
                        tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.general_parameter.code3_pre_wind * 1.13 + 30);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
                    {
                        tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.general_parameter.code3_wind_1 * 1.13 + 30);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
                    {
                        tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.general_parameter.code3_wind_2 * 1.13 + 30);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
                    {
                        tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.general_parameter.code3_wind_3 * 1.13 + 30);
                    }
                }
            }
            else if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_STOP)
            {
                if (sFWG2_t.Direct_handle_parameter.actual_temp >= 250)
                {
                    sFWG2_t.Direct_handle_parameter.stop_set_wind = 100;
                    /* open fan output with a half of max set val*/
                    tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, MAX_SET_WIND_VAL / 2 * 1.13 + 30);
                }
                else if (sFWG2_t.Direct_handle_parameter.actual_temp >= 70 && sFWG2_t.Direct_handle_parameter.actual_temp < 250)
                {
                    /* open fan output with actual temp change*/
                    sFWG2_t.Direct_handle_parameter.stop_set_wind = sFWG2_t.Direct_handle_parameter.actual_temp * 0.4;
                    tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.Direct_handle_parameter.stop_set_wind * 1.13 + 30);
                }
                /* keep fan output until the temp below 60 */
                else if (sFWG2_t.Direct_handle_parameter.actual_temp >= 60&& sFWG2_t.Direct_handle_parameter.actual_temp < 70 )
                {
                    tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, 20 * 1.13 + 30);
                }
                else
                {
                    /* close fan output */
                    tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, 0);
                }
				
				
            }
        }
        else
        {
            if (sFWG2_t.Direct_handle_parameter.actual_temp >= 250)
            {
                sFWG2_t.Direct_handle_parameter.stop_set_wind = 100;
                /* open fan output with a half of max set val*/
                tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, MAX_SET_WIND_VAL / 2 * 1.13 + 30);
            }
            else if (sFWG2_t.Direct_handle_parameter.actual_temp >= 70 && sFWG2_t.Direct_handle_parameter.actual_temp < 250)
            {
                /* open fan output with actual temp change*/
                sFWG2_t.Direct_handle_parameter.stop_set_wind = sFWG2_t.Direct_handle_parameter.actual_temp * 0.4;
                tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, sFWG2_t.Direct_handle_parameter.stop_set_wind * 1.13 + 30);
            }
            else
            {
                /* close fan output */
                tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, 0);
            }
        }
    }
	else if (sFWG2_t.Direct_handle_error_state != HANDLE_OK)
            {
                /* open fan output with max set val */
                tmr_channel_value_set(TMR9, TMR_SELECT_CHANNEL_2, MAX_SET_WIND_VAL * 1.13 + 30);
            }

    /* end the Direct handle fan output of  */
}




void hot_control(void)
{
    static  uint16_t relay_open_delay = RELAY_OPEN_TIME;

    if (sFWG2_t.general_parameter.work_mode == NORMAL)
    {
        /* heating control with normal mode  */
        if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
        {
            if ((sFWG2_t.Direct_handle_state == HANDLE_SLEEP) || sFWG2_t.Direct_handle_error_state != HANDLE_OK)
            {
                /* close relay */
                gpio_bits_reset(GPIOB, GPIO_PINS_14);
                /* close Direct handle pwm output */
                tmr_counter_enable(TMR2, FALSE);
                tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_1, 0);
 
                /* reset relay open flag */
                sFWG2_t.general_parameter.relay_open_flag = false;
                relay_open_delay = RELAY_OPEN_TIME;
            }
            else if ((sFWG2_t.Direct_handle_state == HANDLE_WORKING) && 
				(sFWG2_t.Direct_handle_error_state == HANDLE_OK  && 
				sFWG2_t.Direct_handle_position == NOT_IN_POSSITION))
            {
                /* open relay */
                gpio_bits_set(GPIOB, GPIO_PINS_14);

                /* wait 300ms*/
                if (sFWG2_t.general_parameter.relay_open_flag == FALSE)
                {
                    relay_open_delay --;
                }

                /* 继电器开启完成标志位置1 */
                if (!relay_open_delay)
                {
                    relay_open_delay = 0;
                    sFWG2_t.general_parameter.relay_open_flag = true;
                }
            }
        }
        /* heating control with wind cold  mode  */
        else if ((sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE) || \
                 (sFWG2_t.Direct_handle_work_mode == HANDLE_SLEEP))
        {
            /* close relay */
            gpio_bits_reset(GPIOB, GPIO_PINS_14);
            /* close Direct handle pwm output */
            tmr_counter_enable(TMR2, FALSE);
            tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_1, 0);

            /* reset relay open flag */
            sFWG2_t.general_parameter.relay_open_flag = false;
            relay_open_delay = RELAY_OPEN_TIME;
        }
    }
    else if (sFWG2_t.general_parameter.work_mode == CODE)
    {
        if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_START)
        {
            if (sFWG2_t.Direct_handle_error_state == HANDLE_OK)
            {
                /* open relay */
                gpio_bits_set(GPIOB, GPIO_PINS_14);

                /* wait 300ms*/
                if (sFWG2_t.general_parameter.relay_open_flag == FALSE)
                {
                    relay_open_delay --;
                }

                /* 继电器开启完成标志位置1 */
                if (!relay_open_delay)
                {
                    relay_open_delay = 0;
                    sFWG2_t.general_parameter.relay_open_flag = true;
                }
            }
            else if (sFWG2_t.Direct_handle_error_state != HANDLE_OK)
            {
                /* close relay */
                gpio_bits_reset(GPIOB, GPIO_PINS_14);
                /* close Direct handle pwm output */
                tmr_counter_enable(TMR2, FALSE);
                tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_1, 0);
  
                /* reset relay open flag */
                sFWG2_t.general_parameter.relay_open_flag = false;
                relay_open_delay = RELAY_OPEN_TIME;
            }
        }
        else if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_STOP)
        {
			if(sFWG2_t.Direct_handle_parameter.actual_temp <70)
			{
			    /* close relay */
                gpio_bits_reset(GPIOB, GPIO_PINS_14);
                /* close Direct handle pwm output */
                tmr_counter_enable(TMR2, FALSE);
                tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_1, 0);
                
                /* reset relay open flag */
                sFWG2_t.general_parameter.relay_open_flag = false;
                relay_open_delay = RELAY_OPEN_TIME;
			}
        }
    }
}


int8_t direct_linear_correction(uint16_t user_set_temp)
{
    volatile uint8_t n = 0;
    volatile uint16_t check_start = 0;
    volatile uint8_t check_value = 0;

    if (sFWG2_t.Direct_handle_parameter.set_wind > 30)
    {
        if (user_set_temp < 300)
        {
            check_value = 0;
            check_start = 100;

            for (n = 0; n < 20; n++)
            {
                if (user_set_temp <= (check_start + check_value))
                {
                    return (direct_handle_temp_cal_buff[n]);
                }
                else
                {
                    check_start += 10;
                }
            }
        }
        else
        {
            check_value = 0;
            check_start = 300;

            for (n = 20; n < 40; n++)
            {
                if (user_set_temp <= (check_start + check_value))
                {
                    return (direct_handle_temp_cal_buff[n]);
                }
                else
                {
                    check_start += 10;
                }
            }
        }
		return (direct_handle_temp_cal_buff[n]);
    }
	
	else if(sFWG2_t.Direct_handle_parameter.set_wind<=20 && sFWG2_t.Direct_handle_parameter.set_wind>0)
	{
        if (user_set_temp < 300)
        {
            check_value = 0;
            check_start = 100;

            for (n = 0; n < 20; n++)
            {
                if (user_set_temp <= (check_start + check_value))
                {
                    return (direct_handle_wind_20_cal_buff[n]);
                }
                else
                {
                    check_start += 10;
                }
            }
        }
        else
        {
            check_value = 0;
            check_start = 300;

            for (n = 20; n < 40; n++)
            {
                if (user_set_temp <= (check_start + check_value))
                {
                    return (direct_handle_wind_20_cal_buff[n]);
                }
                else
                {
                    check_start += 10;
                }
            }
			return (direct_handle_wind_20_cal_buff[n]);
        }	    
	}

	return 0;
}

uint16_t temp_get(void)
{
    static uint16_t temp;
    temp = get_adcval(ADC_CHANNEL) >> 2;
    return temp;
}

uint16_t temp_get_filter_average(void)
{
    static uint16_t temp;
    temp = (get_adcval_average(ADC_CHANNEL, 10) >> 2);
    return temp;
}

uint16_t temp_get_filter_middleValue(void)
{
    static uint16_t temp;
    temp =  middleValueFilter() >> 2;
    return temp;
}

uint16_t temp_get_filter_move_average(adc_channel_select_type adc_channel)
{
    static uint16_t temp;
    temp =  moveAverageFilter(adc_channel) >> 2;

    if (temp >= 1020)
    {
        temp = 1020;
    }

    return temp;
}

uint16_t temp_get_filter_mode(void)
{
    static uint16_t temp;
    temp = modeFilter() >> 2;
    return temp;
}

uint16_t temp_get_filter_max_count(void)
{
    static uint16_t temp;
    temp = max_count_filter() >> 2;
    return temp;
}

uint16_t temp_get_average(void)
{
    static uint16_t temp;
    temp = average(10) >> 2;
    return temp;
}



