#include "key_handle.h"
#include "work_handle.h"
#include "beep_handle.h"
#include "dwin_handle.h"
#include "FWG2_handle.h"
static handle_event EVENT = END_EVENT;
#define KEY_COUNT 3


KEY_DEFINE(key_ch, GPIOB, GPIO_PINS_1, 0);
KEY_DEFINE(key_up, GPIOC, GPIO_PINS_5, 0);
KEY_DEFINE(key_down, GPIOB, GPIO_PINS_0, 0);
 
key_obj *keys[KEY_COUNT] = {&key_ch, &key_down, &key_up };

key_event_t key_event[KEY_COUNT];

void KeyProc(void)
{
	static bool first_in = false;
    static bool set_done = false;
    static bool channel_switch_flag = false;
    static uint8_t channel_switch_time = 0;

	
	    if (first_in == false)
    {
        first_in = true;
        key_set_scan_interval(KEY_HANDLE_TIME);
        key_set_timing_simple(&key_ch, KEY_HANDLE_TIME, 600, 0, 0);
        
        key_set_timing_simple(&key_down, KEY_HANDLE_TIME, 600, 0,100);
		key_set_timing_simple(&key_up, KEY_HANDLE_TIME, 600, 0, 100);

    }
	

    if (key_event[0] == KEY_NONE &&
        key_event[1] == KEY_NONE &&
        key_event[2] == KEY_NONE)
    {
        set_done = false;
    }

    /* scan key begin */
    for (int i = 0; i < KEY_COUNT; i++)
    {
        key_event[i] = key_event_check(keys[i]);
    }

    /* scan key end */

    /* direct setting time control begin */
    if (sFWG2_t.general_parameter.setting_temp_flag)
    {
        sFWG2_t.general_parameter.setting_temp_time++;

        if (sFWG2_t.general_parameter.setting_temp_time >= 120)
        {
            sFWG2_t.general_parameter.setting_temp_time = 0;
            sFWG2_t.general_parameter.setting_temp_flag = false;

            if (sFWG2_t.general_parameter.adjust_key_temporary_set == TEMPORARY_SELECT_TEMP)
            {
                sFWG2_t.general_parameter.adjust_key_temporary_set = TEMPORARY_SELECT_NONE;
            }
        }
    }

    if (sFWG2_t.general_parameter.setting_wind_flag)
    {
        sFWG2_t.general_parameter.setting_wind_time++;

        if (sFWG2_t.general_parameter.setting_wind_time >= 120)
        {
            sFWG2_t.general_parameter.setting_wind_time = 0;
            sFWG2_t.general_parameter.setting_wind_flag = false;

            if (sFWG2_t.general_parameter.adjust_key_temporary_set == TEMPORARY_SELECT_WIND)
            {
                sFWG2_t.general_parameter.adjust_key_temporary_set = TEMPORARY_SELECT_NONE;
            }
        }
    }

    if (channel_switch_flag == true)
    {
        channel_switch_time ++;

        if (channel_switch_time >= 120)
        {
            channel_switch_flag = false;
            channel_switch_time = 0;
        }
    }

    /* direct setting time control end */

    if (sFWG2_t.general_parameter.work_mode == NORMAL)
    {
        if (!set_done)
        {
            /*get event*/
            if (key_event[0] == KEY_SHORT_PRESS)
            {
                sFWG2_t.general_parameter.key_setting_flag = true;
                sFWG2_t.general_parameter.setting_time = 0;

				if( sFWG2_t.general_parameter.setting_temp_flag == true &&
				    sFWG2_t.general_parameter.setting_wind_flag == true &&
					sFWG2_t.general_parameter.fn_key_short_set == S_CHANNEL_SWITCH)
				   {
				       goto change_channel;
				   }
					else if(sFWG2_t.general_parameter.setting_temp_flag == true &&
				   sFWG2_t.general_parameter.setting_wind_flag == true &&
					sFWG2_t.general_parameter.fn_key_short_set == S_QUICK_MODE)
				   {
				       goto enter_quick_mode;
				   }
				   else if(sFWG2_t.general_parameter.setting_temp_flag == true &&
				   sFWG2_t.general_parameter.setting_wind_flag == true &&
					sFWG2_t.general_parameter.fn_key_short_set == S_COUNTDOWN_MODE)
				   {
				       goto enter_countdown_mode;
				   }
				
				
                if (sFWG2_t.general_parameter.setting_temp_flag && channel_switch_flag == false)
                {
                    if (sFWG2_t.general_parameter.adjust_key_set == SELECT_TEMP)
                    {
                        sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
                        sFWG2_t.general_parameter.setting_wind_flag = true;
                        sFWG2_t.general_parameter.setting_temp_flag = false;
                    }
                    else if (sFWG2_t.general_parameter.adjust_key_set == SELECT_WIND)
                    {
                        sFWG2_t.general_parameter.setting_temp_flag = false;
                    }

                    sbeep.status = BEEP_SHORT;
                    set_done = TRUE;
                }
                else if (sFWG2_t.general_parameter.setting_wind_flag  && channel_switch_flag == false)
                {
                    if (sFWG2_t.general_parameter.adjust_key_set == SELECT_TEMP)
                    {
                        sFWG2_t.general_parameter.setting_wind_flag = false;
                    }
                    else if (sFWG2_t.general_parameter.adjust_key_set == SELECT_WIND)
                    {
                        sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
                        sFWG2_t.general_parameter.setting_wind_flag = false;
                        sFWG2_t.general_parameter.setting_temp_flag = true;
                    }

                    sbeep.status = BEEP_SHORT;
                    set_done = TRUE;
                }
                else if (sFWG2_t.general_parameter.fn_key_short_set == S_CHANNEL_SWITCH)
                {
					change_channel:
                    if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                    {
                        channel_switch_flag = true;
                        channel_switch_time = 0;
                        EVENT = DIRECT_CH_SWITCH_EVENT;
                        sbeep.status = BEEP_LONG;
                        set_done = TRUE;
                    }
                }
                else if (sFWG2_t.general_parameter.fn_key_short_set == S_QUICK_MODE)
                {
					enter_quick_mode:
                    /* enter quick mode */
                    if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                    {
                        sFWG2_t.Direct_handle_work_mode = QUICK_MODE;
						sFWG2_t.general_parameter.countdown_flag = false;
                    }
                    /* exit quick mode */
                    else if (sFWG2_t.Direct_handle_work_mode == QUICK_MODE)
                    {
                        sFWG2_t.Direct_handle_work_mode = NORMAL_MODE;
                    }

                    sbeep.status = BEEP_LONG;
                    set_done = TRUE;
                }
                else if (sFWG2_t.general_parameter.fn_key_short_set == S_COUNTDOWN_MODE && \
                         (sFWG2_t.general_parameter.fwg2_page == PAGE_MAIN || \
                          sFWG2_t.general_parameter.fwg2_page == PAGE_DIRECT_CURVE))
                {
					enter_countdown_mode:
                    /* start countdown  */
                    if (sFWG2_t.general_parameter.countdown_flag == true)
                    {
                        sFWG2_t.general_parameter.countdown_flag = false;
                    }
                    /* stop countdown  */
                    else if (sFWG2_t.general_parameter.countdown_flag == false)
                    {
                        sFWG2_t.general_parameter.countdown_flag = true;
                    }

                    sbeep.status = BEEP_LONG;
                    set_done = TRUE;
                }
            }
            else if (key_event[1] == KEY_SHORT_PRESS)
            {
                sFWG2_t.general_parameter.key_setting_flag = true;
                sFWG2_t.general_parameter.setting_time = 0;

                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                {
                    if (sFWG2_t.general_parameter.setting_temp_flag == false && sFWG2_t.general_parameter.setting_wind_flag == false
                            && sFWG2_t.general_parameter.adjust_key_temporary_set == TEMPORARY_SELECT_NONE)
                    {
                        if (sFWG2_t.general_parameter.adjust_key_set == SELECT_TEMP)
                        {
                            sFWG2_t.general_parameter.setting_temp_flag = true;
                        }
                        else if (sFWG2_t.general_parameter.adjust_key_set == SELECT_WIND)
                        {
                            sFWG2_t.general_parameter.setting_wind_flag = true;
                        }
                        else if (sFWG2_t.general_parameter.adjust_key_set == SELECT_CHANNEL)
                        {
                            if (sFWG2_t.general_parameter.fwg2_page == PAGE_MAIN || sFWG2_t.general_parameter.fwg2_page == PAGE_DIRECT_CURVE)
                            {
                                EVENT = DIRECT_CH_REDUCE_EVENT;
                                set_done = TRUE;
                            }
                        }
                    }
                    else if (sFWG2_t.general_parameter.setting_temp_flag == false && sFWG2_t.general_parameter.setting_wind_flag == false
                             && sFWG2_t.general_parameter.adjust_key_temporary_set == TEMPORARY_SELECT_WIND)
                    {
                        sFWG2_t.general_parameter.setting_wind_flag = true;
                    }
                    else if (sFWG2_t.general_parameter.setting_temp_flag == false && sFWG2_t.general_parameter.setting_wind_flag == false
                             && sFWG2_t.general_parameter.adjust_key_temporary_set == TEMPORARY_SELECT_TEMP)
                    {
                        sFWG2_t.general_parameter.setting_temp_flag = true;
                    }

                    if (sFWG2_t.general_parameter.adjust_key_set == SELECT_CHANNEL)
                    {
                        if (sFWG2_t.general_parameter.fwg2_page == PAGE_MAIN || sFWG2_t.general_parameter.fwg2_page == PAGE_DIRECT_CURVE)
                        {
                            channel_switch_flag = true;
                            channel_switch_time = 0;
                            EVENT = DIRECT_CH_REDUCE_EVENT;
                            set_done = TRUE;
                        }
                    }
                    else
                    {
                        if (channel_switch_flag)
                        {
                            channel_switch_flag = false;
                            channel_switch_time = 0;

                            if (sFWG2_t.general_parameter.adjust_key_set == SELECT_TEMP)
                            {
                                sFWG2_t.general_parameter.setting_temp_flag = true;
                                sFWG2_t.general_parameter.setting_wind_flag = false;
                                EVENT = DIRECT_TEMP_REDUCE_EVENT;
                                set_done = TRUE;
                                sFWG2_t.general_parameter.setting_temp_time = 0;
                            }
                            else if (sFWG2_t.general_parameter.adjust_key_set == SELECT_WIND)
                            {
                                sFWG2_t.general_parameter.setting_temp_flag = false;
                                sFWG2_t.general_parameter.setting_wind_flag = true;
                                EVENT = DIRECT_WIND_REDUCE_EVENT;
                                set_done = TRUE;
                                sFWG2_t.general_parameter.setting_wind_time = 0;
                            }
                        }
                        else
                        {
                            if (sFWG2_t.general_parameter.setting_temp_flag && sFWG2_t.general_parameter.setting_wind_flag)
                            {
                                if (sFWG2_t.general_parameter.adjust_key_set == SELECT_TEMP)
                                {
                                    sFWG2_t.general_parameter.setting_temp_flag = true;
                                    sFWG2_t.general_parameter.setting_wind_flag = false;
                                    EVENT = DIRECT_TEMP_ADD_EVENT;
                                    set_done = TRUE;
                                    sFWG2_t.general_parameter.setting_temp_time = 0;
                                }
                                else if (sFWG2_t.general_parameter.adjust_key_set == SELECT_WIND)
                                {
                                    sFWG2_t.general_parameter.setting_temp_flag = false;
                                    sFWG2_t.general_parameter.setting_wind_flag = true;
                                    EVENT = DIRECT_WIND_ADD_EVENT;
                                    set_done = TRUE;
                                    sFWG2_t.general_parameter.setting_wind_time = 0;
                                }
                            }
                            else
                            {
                                if (sFWG2_t.general_parameter.setting_temp_flag)
                                {
                                    EVENT = DIRECT_TEMP_REDUCE_EVENT;
                                    set_done = TRUE;
                                    sFWG2_t.general_parameter.setting_temp_time = 0;
                                }

                                if (sFWG2_t.general_parameter.setting_wind_flag)
                                {
                                    EVENT = DIRECT_WIND_REDUCE_EVENT;
                                    set_done = TRUE;
                                    sFWG2_t.general_parameter.setting_wind_time = 0;
                                }
                            }
                        }
                    }
                }
                else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
                {
                    EVENT = DIRECT_WIND_REDUCE_EVENT;
                    set_done = TRUE;
                }
            }
            else if (key_event[2] == KEY_SHORT_PRESS)
            {
                sFWG2_t.general_parameter.key_setting_flag = true;
                sFWG2_t.general_parameter.setting_time = 0;

                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                {
                    if (sFWG2_t.general_parameter.setting_temp_flag == false && sFWG2_t.general_parameter.setting_wind_flag == false
                            && sFWG2_t.general_parameter.adjust_key_temporary_set == TEMPORARY_SELECT_NONE)
                    {
                        if (sFWG2_t.general_parameter.setting_temp_flag == false && sFWG2_t.general_parameter.setting_wind_flag == false)
                        {
                            if (sFWG2_t.general_parameter.adjust_key_set == SELECT_TEMP)
                            {
                                sFWG2_t.general_parameter.setting_temp_flag = true;
                            }
                            else if (sFWG2_t.general_parameter.adjust_key_set == SELECT_WIND)
                            {
                                sFWG2_t.general_parameter.setting_wind_flag = true;
                            }
                            else if (sFWG2_t.general_parameter.adjust_key_set == SELECT_CHANNEL)
                            {
                                if (sFWG2_t.general_parameter.fwg2_page == PAGE_MAIN || sFWG2_t.general_parameter.fwg2_page == PAGE_DIRECT_CURVE)
                                {
                                    EVENT = DIRECT_CH_ADD_EVENT;
                                    set_done = TRUE;
                                }
                            }
                        }
                    }
                    else if (sFWG2_t.general_parameter.setting_temp_flag == false && sFWG2_t.general_parameter.setting_wind_flag == false
                             && sFWG2_t.general_parameter.adjust_key_temporary_set == TEMPORARY_SELECT_WIND)
                    {
                        sFWG2_t.general_parameter.setting_wind_flag = true;
                    }
                    else if (sFWG2_t.general_parameter.setting_temp_flag == false && sFWG2_t.general_parameter.setting_wind_flag == false
                             && sFWG2_t.general_parameter.adjust_key_temporary_set == TEMPORARY_SELECT_TEMP)
                    {
                        sFWG2_t.general_parameter.setting_temp_flag = true;
                    }

                    if (sFWG2_t.general_parameter.adjust_key_set == SELECT_CHANNEL)
                    {
                        if (sFWG2_t.general_parameter.fwg2_page == PAGE_MAIN || sFWG2_t.general_parameter.fwg2_page == PAGE_DIRECT_CURVE)
                        {
                            channel_switch_flag = true;
                            channel_switch_time = 0;
                            EVENT = DIRECT_CH_ADD_EVENT;
                            set_done = TRUE;
                        }
                    }
                    else
                    {
                        if (channel_switch_flag)
                        {
                            channel_switch_flag = false;
                            channel_switch_time = 0;

                            if (sFWG2_t.general_parameter.adjust_key_set == SELECT_TEMP)
                            {
                                sFWG2_t.general_parameter.setting_temp_flag = true;
                                sFWG2_t.general_parameter.setting_wind_flag = false;
                                EVENT = DIRECT_TEMP_ADD_EVENT;
                                set_done = TRUE;
                                sFWG2_t.general_parameter.setting_temp_time = 0;
                            }
                            else if (sFWG2_t.general_parameter.adjust_key_set == SELECT_WIND)
                            {
                                sFWG2_t.general_parameter.setting_temp_flag = false;
                                sFWG2_t.general_parameter.setting_wind_flag = true;
                                EVENT = DIRECT_WIND_ADD_EVENT;
                                set_done = TRUE;
                                sFWG2_t.general_parameter.setting_wind_time = 0;
                            }
                        }
                        else
                        {
                            if (sFWG2_t.general_parameter.setting_temp_flag && sFWG2_t.general_parameter.setting_wind_flag)
                            {
                                if (sFWG2_t.general_parameter.adjust_key_set == SELECT_TEMP)
                                {
                                    sFWG2_t.general_parameter.setting_temp_flag = true;
                                    sFWG2_t.general_parameter.setting_wind_flag = false;
                                    EVENT = DIRECT_TEMP_ADD_EVENT;
                                    set_done = TRUE;
                                    sFWG2_t.general_parameter.setting_temp_time = 0;
                                }
                                else if (sFWG2_t.general_parameter.adjust_key_set == SELECT_WIND)
                                {
                                    sFWG2_t.general_parameter.setting_temp_flag = false;
                                    sFWG2_t.general_parameter.setting_wind_flag = true;
                                    EVENT = DIRECT_WIND_ADD_EVENT;
                                    set_done = TRUE;
                                    sFWG2_t.general_parameter.setting_wind_time = 0;
                                }
                            }
                            else
                            {
                                if (sFWG2_t.general_parameter.setting_temp_flag)
                                {
                                    EVENT = DIRECT_TEMP_ADD_EVENT;
                                    set_done = TRUE;
                                    sFWG2_t.general_parameter.setting_temp_time = 0;
                                }

                                if (sFWG2_t.general_parameter.setting_wind_flag)
                                {
                                    EVENT = DIRECT_WIND_ADD_EVENT;
                                    set_done = TRUE;
                                    sFWG2_t.general_parameter.setting_wind_time = 0;
                                }
                            }
                        }
                    }
                }
                else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
                {
                    EVENT = DIRECT_WIND_ADD_EVENT;
                    set_done = TRUE;
                }
            }

            /* key long perss */
            if (key_event[0] == KEY_LONG_PRESS)
            {
                if (sFWG2_t.general_parameter.fn_key_long_set == L_COLD_WIN_MODE)
                {
                    /* enter cold mode */
                    if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                    {
                        sFWG2_t.Direct_handle_work_mode = COLD_WIND_MODE;
                    }
                    /* exit cold mode */
                    else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
                    {
                        sFWG2_t.Direct_handle_work_mode = NORMAL_MODE;
                    }

                    sbeep.status = BEEP_LONG;
                    set_done = TRUE;
                }
                else if (sFWG2_t.general_parameter.fn_key_long_set == L_QUICK_MODE)
                {
                    /* enter quick mode */
                    if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                    {
                        sFWG2_t.Direct_handle_work_mode = QUICK_MODE;
						sFWG2_t.general_parameter.countdown_flag = false;
                    }
                    /* exit quick mode */
                    else if (sFWG2_t.Direct_handle_work_mode == QUICK_MODE)
                    {
                        sFWG2_t.Direct_handle_work_mode = NORMAL_MODE;
                    }

                    sbeep.status = BEEP_LONG;
                    set_done = TRUE;
                }
                else if (sFWG2_t.general_parameter.fn_key_long_set == L_COUNTDOWN_MODE && \
                         (sFWG2_t.general_parameter.fwg2_page == PAGE_MAIN || \
                          sFWG2_t.general_parameter.fwg2_page == PAGE_DIRECT_CURVE))
                {
                    /* start countdown  */
                    if (sFWG2_t.general_parameter.countdown_flag == true)
                    {
                        sFWG2_t.general_parameter.countdown_flag = false;
                    }
                    /* stop countdown  */
                    else if (sFWG2_t.general_parameter.countdown_flag == false)
                    {
                        sFWG2_t.general_parameter.countdown_flag = true;
                    }

                    sbeep.status = BEEP_LONG;
                    set_done = TRUE;
                }
            }
            else  if (key_event[1] == KEY_LONG_PRESS_CONTINUE)
            {
                sFWG2_t.general_parameter.key_setting_flag = true;
                sFWG2_t.general_parameter.setting_time = 0;

                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                {
                    if (sFWG2_t.general_parameter.setting_temp_flag == false && sFWG2_t.general_parameter.setting_wind_flag == false)
                    {
                        if (sFWG2_t.general_parameter.adjust_key_set == SELECT_TEMP)
                        {
                            sFWG2_t.general_parameter.setting_temp_flag = true;
                        }
                        else if (sFWG2_t.general_parameter.adjust_key_set == SELECT_WIND)
                        {
                            sFWG2_t.general_parameter.setting_wind_flag = true;
                        }
                    }

                    if (channel_switch_flag)
                    {
                        channel_switch_flag = false;
                        channel_switch_time = 0;

                        if (sFWG2_t.general_parameter.adjust_key_set == SELECT_TEMP)
                        {
                            sFWG2_t.general_parameter.setting_temp_flag = true;
                            sFWG2_t.general_parameter.setting_wind_flag = false;
                            EVENT = DIRECT_TEMP_REDUCE_FIVE_EVENT;
                            sFWG2_t.general_parameter.setting_temp_time = 0;
                        }
                        else if (sFWG2_t.general_parameter.adjust_key_set == SELECT_WIND)
                        {
                            sFWG2_t.general_parameter.setting_temp_flag = false;
                            sFWG2_t.general_parameter.setting_wind_flag = true;
                            EVENT = DIRECT_WIND_REDUCE_FIVE_EVENT;
                            sFWG2_t.general_parameter.setting_wind_time = 0;
                        }
                    }
                    else
                    {
                        if (sFWG2_t.general_parameter.setting_temp_flag)
                        {
                            EVENT = DIRECT_TEMP_REDUCE_FIVE_EVENT;
                            sFWG2_t.general_parameter.setting_temp_time = 0;
                        }

                        if (sFWG2_t.general_parameter.setting_wind_flag)
                        {
                            EVENT = DIRECT_WIND_REDUCE_FIVE_EVENT;
                            sFWG2_t.general_parameter.setting_wind_time = 0;
                        }
                    }

                    //                    if (sFWG2_t.general_parameter.setting_temp_flag  && channel_switch_flag == false)
                    //                    {
                    //                        //EVENT = DIRECT_TEMP_REDUCE_EVENT;
                    //                        EVENT = DIRECT_TEMP_REDUCE_FIVE_EVENT;
                    //                        sFWG2_t.general_parameter.setting_temp_time = 0;
                    //                    }
                    //                    if (sFWG2_t.general_parameter.setting_wind_flag  && channel_switch_flag == false)
                    //                    {
                    //                        EVENT = DIRECT_WIND_REDUCE_EVENT;
                    //                        //EVENT = DIRECT_WIND_REDUCE_FIVE_EVENT;
                    //                        sFWG2_t.general_parameter.setting_wind_time = 0;
                    //                    }
                }
                else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
                {
                    //EVENT = DIRECT_WIND_REDUCE_EVENT;
                    EVENT = DIRECT_WIND_REDUCE_FIVE_EVENT;
                }
            }
            else if (key_event[2] == KEY_LONG_PRESS_CONTINUE)
            {
                sFWG2_t.general_parameter.key_setting_flag = true;
                sFWG2_t.general_parameter.setting_time = 0;

                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                {
                    if (sFWG2_t.general_parameter.setting_temp_flag == false && sFWG2_t.general_parameter.setting_wind_flag == false)
                    {
                        if (sFWG2_t.general_parameter.adjust_key_set == SELECT_TEMP)
                        {
                            sFWG2_t.general_parameter.setting_temp_flag = true;
                        }
                        else if (sFWG2_t.general_parameter.adjust_key_set == SELECT_WIND)
                        {
                            sFWG2_t.general_parameter.setting_wind_flag = true;
                        }
                    }

                    if (channel_switch_flag)
                    {
                        channel_switch_flag = false;
                        channel_switch_time = 0;

                        if (sFWG2_t.general_parameter.adjust_key_set == SELECT_TEMP)
                        {
                            sFWG2_t.general_parameter.setting_temp_flag = true;
                            sFWG2_t.general_parameter.setting_wind_flag = false;
                            EVENT = DIRECT_TEMP_ADD_FIVE_EVENT;
                            sFWG2_t.general_parameter.setting_temp_time = 0;
                        }
                        else if (sFWG2_t.general_parameter.adjust_key_set == SELECT_WIND)
                        {
                            sFWG2_t.general_parameter.setting_temp_flag = false;
                            sFWG2_t.general_parameter.setting_wind_flag = true;
                            EVENT = DIRECT_WIND_ADD_FIVE_EVENT;
                            sFWG2_t.general_parameter.setting_wind_time = 0;
                        }
                    }
                    else
                    {
                        if (sFWG2_t.general_parameter.setting_temp_flag)
                        {
                            EVENT = DIRECT_TEMP_ADD_FIVE_EVENT;
                            sFWG2_t.general_parameter.setting_temp_time = 0;
                        }

                        if (sFWG2_t.general_parameter.setting_wind_flag)
                        {
                            EVENT = DIRECT_WIND_ADD_FIVE_EVENT;
                            sFWG2_t.general_parameter.setting_wind_time = 0;
                        }
                    }

                    //                    if (sFWG2_t.general_parameter.setting_temp_flag  && channel_switch_flag == false)
                    //                    {
                    //                        //EVENT = DIRECT_TEMP_ADD_EVENT;
                    //                        EVENT = DIRECT_TEMP_ADD_FIVE_EVENT;
                    //                        sFWG2_t.general_parameter.setting_temp_time = 0;
                    //                    }
                    //                    if (sFWG2_t.general_parameter.setting_wind_flag  && channel_switch_flag == false)
                    //                    {
                    //                        //EVENT = DIRECT_WIND_ADD_EVENT;
                    //                        EVENT = DIRECT_WIND_ADD_FIVE_EVENT;
                    //                        sFWG2_t.general_parameter.setting_wind_time = 0;
                    //                    }
                }
                else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
                {
                    //EVENT = DIRECT_WIND_ADD_EVENT;
                    EVENT = DIRECT_WIND_ADD_FIVE_EVENT;
                }
            }
        }
    }
    else if (sFWG2_t.general_parameter.work_mode == CODE)
    {
        if (!set_done)
        {
            if (key_event[0] == KEY_SHORT_PRESS && sFWG2_t.general_parameter.fwg2_page == PAGE_SHOW_CODE_WORK)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                {
                    if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_START)
                    {
                        sFWG2_t.general_parameter.code_mode_state = CODE_MODE_STOP;
                        sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code0_pre_time;
                    }
                    else if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_STOP)
                    {
                        sFWG2_t.general_parameter.code_mode_state = CODE_MODE_START;
                        sFWG2_t.general_parameter.code_mode_step = CODE_PRE_HEAT;
                    }

                    sbeep.status = BEEP_SHORT;
                    set_done = TRUE;
                }
            }
        }
    }
}

static void key_event_handle(void)
{
    switch (EVENT)
    {
    case DIRECT_TEMP_ADD_EVENT:
        sFWG2_t.Direct_handle_parameter.last_set_temp = 0;

        if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
        {
            if (sFWG2_t.Direct_handle_parameter.set_temp >= MAX_SET_TEMP_VAL)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = MAX_SET_TEMP_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_temp++;
            }

            sFWG2_t.Direct_handle_parameter.set_temp_f_display  =  9 * sFWG2_t.Direct_handle_parameter.set_temp  / 5  + 32;
        }
        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
        {
            if (sFWG2_t.Direct_handle_parameter.set_temp_f_display >= MAX_SET_TEMP_F_VAL)
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = MAX_SET_TEMP_F_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display++;
            }

            sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case DIRECT_TEMP_ADD_FIVE_EVENT:
        sFWG2_t.Direct_handle_parameter.last_set_temp = 0;

        if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
        {
            if (sFWG2_t.Direct_handle_parameter.set_temp + 5 >= MAX_SET_TEMP_VAL)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = MAX_SET_TEMP_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_temp += 5;
            }

            sFWG2_t.Direct_handle_parameter.set_temp_f_display  =  9 * sFWG2_t.Direct_handle_parameter.set_temp  / 5  + 32;
        }
        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
        {
            if (sFWG2_t.Direct_handle_parameter.set_temp_f_display + 38 >= MAX_SET_TEMP_F_VAL)
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = MAX_SET_TEMP_F_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display += 38;
            }

            sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case DIRECT_TEMP_REDUCE_EVENT:
        sFWG2_t.Direct_handle_parameter.last_set_temp = 0;

        if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
        {
            if (sFWG2_t.Direct_handle_parameter.set_temp <= MIN_SET_TEMP_VAL)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = MIN_SET_TEMP_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_temp--;
            }

            sFWG2_t.Direct_handle_parameter.set_temp_f_display  =  9 * sFWG2_t.Direct_handle_parameter.set_temp  / 5  + 32;
        }
        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
        {
            if (sFWG2_t.Direct_handle_parameter.set_temp_f_display <= MIN_SET_TEMP_F_VAL)
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = MIN_SET_TEMP_F_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display --;
            }

            sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case DIRECT_TEMP_REDUCE_FIVE_EVENT:
        sFWG2_t.Direct_handle_parameter.last_set_temp = 0;

        if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
        {
            if (sFWG2_t.Direct_handle_parameter.set_temp - 5 <= MIN_SET_TEMP_VAL)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = MIN_SET_TEMP_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_temp -= 5;
            }

            sFWG2_t.Direct_handle_parameter.set_temp_f_display  =  9 * sFWG2_t.Direct_handle_parameter.set_temp  / 5  + 32;
        }
        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
        {
            if (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 38 <= MIN_SET_TEMP_F_VAL)
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = MIN_SET_TEMP_F_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display -= 38;
            }

            sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case DIRECT_WIND_REDUCE_EVENT:
        if (sFWG2_t.Direct_handle_work_mode  ==  NORMAL_MODE)
        {
            sFWG2_t.Direct_handle_parameter.last_set_wind = 0;

            if (sFWG2_t.Direct_handle_parameter.set_wind <= MIN_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.set_wind = MIN_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_wind --;
            }
        }
        else if (sFWG2_t.Direct_handle_work_mode  ==  COLD_WIND_MODE)
        {
            if (sFWG2_t.Direct_handle_parameter.cold_mode_set_wind <= MIN_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = MIN_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind --;
            }
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case DIRECT_WIND_ADD_EVENT:
        if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
        {
            sFWG2_t.Direct_handle_parameter.last_set_wind = 0;

            if (sFWG2_t.Direct_handle_parameter.set_wind >= MAX_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.set_wind = MAX_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_wind ++;
            }
        }
        else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
        {
            if (sFWG2_t.Direct_handle_parameter.cold_mode_set_wind >= MAX_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = MAX_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind ++;
            }
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case DIRECT_WIND_REDUCE_FIVE_EVENT:
        if (sFWG2_t.Direct_handle_work_mode  ==  NORMAL_MODE)
        {
            if (sFWG2_t.Direct_handle_parameter.set_wind - 5 <= MIN_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
                sFWG2_t.Direct_handle_parameter.set_wind = MIN_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_wind -= 5;
            }
        }
        else if (sFWG2_t.Direct_handle_work_mode  ==  COLD_WIND_MODE)
        {
            if (sFWG2_t.Direct_handle_parameter.cold_mode_set_wind - 5 <= MIN_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = MIN_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind -= 5;
            }
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case DIRECT_WIND_ADD_FIVE_EVENT:
        if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
        {
            sFWG2_t.Direct_handle_parameter.last_set_wind = 0;

            if (sFWG2_t.Direct_handle_parameter.set_wind + 5 >= MAX_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.set_wind = MAX_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_wind += 5;
            }
        }
        else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
        {
            if (sFWG2_t.Direct_handle_parameter.cold_mode_set_wind >= MAX_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = MAX_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind += 5;
            }
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case DIRECT_CH_SWITCH_EVENT:
        sFWG2_t.general_parameter.setting_temp_flag =  true;
        sFWG2_t.general_parameter.setting_temp_time =  0;
        sFWG2_t.general_parameter.setting_wind_flag =  true;
        sFWG2_t.general_parameter.setting_wind_time =  0;
        sFWG2_t.general_parameter.countdown_flag = false;
        sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
        sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
	    sFWG2_t.Direct_handle_parameter.last_show_time = 0;

        if (sFWG2_t.general_parameter.ch >= 4)
        {
            sFWG2_t.general_parameter.ch = 1;
        }
        else
        {
            sFWG2_t.general_parameter.ch ++;
        }

        if (sFWG2_t.general_parameter.ch == 1)
        {
            if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = sFWG2_t.general_parameter.ch1_set_temp_f_display;
                sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                sFWG2_t.general_parameter.ch1_set_temp = (sFWG2_t.general_parameter.ch1_set_temp_f_display - 32) * 5 / 9;
            }
            else if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch1_set_temp;
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = 9 * sFWG2_t.Direct_handle_parameter.set_temp / 5 + 32;
                sFWG2_t.general_parameter.ch1_set_temp_f_display = 9 * sFWG2_t.general_parameter.ch1_set_temp / 5 + 32;
            }

            sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch1_set_wind;
            sFWG2_t.Direct_handle_parameter.set_time = sFWG2_t.general_parameter.ch1_set_time;
        }
        else if (sFWG2_t.general_parameter.ch == 2)
        {
            if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = sFWG2_t.general_parameter.ch2_set_temp_f_display;
                sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                sFWG2_t.general_parameter.ch2_set_temp = (sFWG2_t.general_parameter.ch2_set_temp_f_display - 32) * 5 / 9;
            }
            else if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch2_set_temp;
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = 9 * sFWG2_t.Direct_handle_parameter.set_temp / 5 + 32;
                sFWG2_t.general_parameter.ch2_set_temp_f_display = 9 * sFWG2_t.general_parameter.ch2_set_temp / 5 + 32;
            }

            sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch2_set_wind;
            sFWG2_t.Direct_handle_parameter.set_time = sFWG2_t.general_parameter.ch2_set_time;
        }
        else if (sFWG2_t.general_parameter.ch == 3)
        {
            if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = sFWG2_t.general_parameter.ch3_set_temp_f_display;
                sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                sFWG2_t.general_parameter.ch3_set_temp = (sFWG2_t.general_parameter.ch3_set_temp_f_display - 32) * 5 / 9;
            }
            else if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch3_set_temp;
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = 9 * sFWG2_t.Direct_handle_parameter.set_temp / 5 + 32;
                sFWG2_t.general_parameter.ch3_set_temp_f_display = 9 * sFWG2_t.general_parameter.ch3_set_temp / 5 + 32;
            }

            sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch3_set_wind;
            sFWG2_t.Direct_handle_parameter.set_time = sFWG2_t.general_parameter.ch3_set_time;
        }
        else if (sFWG2_t.general_parameter.ch == 4)
        {
            if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = sFWG2_t.general_parameter.ch4_set_temp_f_display;
                sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                sFWG2_t.general_parameter.ch4_set_temp = (sFWG2_t.general_parameter.ch4_set_temp_f_display - 32) * 5 / 9;
            }
            else if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch4_set_temp;
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = 9 * sFWG2_t.Direct_handle_parameter.set_temp / 5 + 32;
                sFWG2_t.general_parameter.ch4_set_temp_f_display = 9 * sFWG2_t.general_parameter.ch4_set_temp / 5 + 32;
            }

            sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch4_set_wind;
            sFWG2_t.Direct_handle_parameter.set_time = sFWG2_t.general_parameter.ch4_set_time;
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case DIRECT_CH_ADD_EVENT:
        sFWG2_t.general_parameter.setting_temp_flag =  true;
        sFWG2_t.general_parameter.setting_temp_time =  0;
        sFWG2_t.general_parameter.setting_wind_flag =  true;
        sFWG2_t.general_parameter.setting_wind_time =  0;
        sFWG2_t.general_parameter.countdown_flag = false;
        sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
        sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
        sFWG2_t.Direct_handle_parameter.last_set_wind = 0;

        if (sFWG2_t.general_parameter.ch >= 4)
        {
            sFWG2_t.general_parameter.ch = 4;
        }
        else
        {
            sFWG2_t.general_parameter.ch ++;
        }

        if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
        {
            if (sFWG2_t.general_parameter.ch == 1)
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = sFWG2_t.general_parameter.ch1_set_temp_f_display;
                sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch1_set_wind;
                sFWG2_t.Direct_handle_parameter.set_time  = sFWG2_t.general_parameter.ch1_set_time;
            }
            else if (sFWG2_t.general_parameter.ch == 2)
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = sFWG2_t.general_parameter.ch2_set_temp_f_display;
                sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch2_set_wind;
                sFWG2_t.Direct_handle_parameter.set_time = sFWG2_t.general_parameter.ch2_set_time;
            }
            else if (sFWG2_t.general_parameter.ch == 3)
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = sFWG2_t.general_parameter.ch3_set_temp_f_display;
                sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch3_set_wind;
                sFWG2_t.Direct_handle_parameter.set_time = sFWG2_t.general_parameter.ch3_set_time;
            }
            else if (sFWG2_t.general_parameter.ch == 4)
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = sFWG2_t.general_parameter.ch4_set_temp_f_display;
                sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch4_set_wind;
                sFWG2_t.Direct_handle_parameter.set_time = sFWG2_t.general_parameter.ch4_set_time;
            }
        }
        else if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
        {
            if (sFWG2_t.general_parameter.ch == 1)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch1_set_temp;
                sFWG2_t.Direct_handle_parameter.set_temp_f_display  =  9 * sFWG2_t.Direct_handle_parameter.set_temp  / 5  + 32;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch1_set_wind;
                sFWG2_t.Direct_handle_parameter.set_time  = sFWG2_t.general_parameter.ch1_set_time;
            }
            else if (sFWG2_t.general_parameter.ch == 2)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch2_set_temp;
                sFWG2_t.Direct_handle_parameter.set_temp_f_display  =  9 * sFWG2_t.Direct_handle_parameter.set_temp  / 5  + 32;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch2_set_wind;
                sFWG2_t.Direct_handle_parameter.set_time = sFWG2_t.general_parameter.ch2_set_time;
            }
            else if (sFWG2_t.general_parameter.ch == 3)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch3_set_temp;
                sFWG2_t.Direct_handle_parameter.set_temp_f_display  =  9 * sFWG2_t.Direct_handle_parameter.set_temp  / 5  + 32;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch3_set_wind;
                sFWG2_t.Direct_handle_parameter.set_time = sFWG2_t.general_parameter.ch3_set_time;
            }
            else if (sFWG2_t.general_parameter.ch == 4)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch4_set_temp;
                sFWG2_t.Direct_handle_parameter.set_temp_f_display  =  9 * sFWG2_t.Direct_handle_parameter.set_temp  / 5  + 32;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch4_set_wind;
                sFWG2_t.Direct_handle_parameter.set_time = sFWG2_t.general_parameter.ch4_set_time;
            }
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case DIRECT_CH_REDUCE_EVENT:
        sFWG2_t.general_parameter.setting_temp_flag =  true;
        sFWG2_t.general_parameter.setting_temp_time =  0;
        sFWG2_t.general_parameter.setting_wind_flag =  true;
        sFWG2_t.general_parameter.setting_wind_time =  0;
        sFWG2_t.general_parameter.countdown_flag = false;
        sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
        sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
        sFWG2_t.Direct_handle_parameter.last_set_wind = 0;

        if (sFWG2_t.general_parameter.ch <= 1)
        {
            sFWG2_t.general_parameter.ch = 1;
        }
        else
        {
            sFWG2_t.general_parameter.ch --;
        }

        if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
        {
            if (sFWG2_t.general_parameter.ch == 1)
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = sFWG2_t.general_parameter.ch1_set_temp_f_display;
                sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch1_set_wind;
                sFWG2_t.Direct_handle_parameter.set_time  = sFWG2_t.general_parameter.ch1_set_time;
            }
            else if (sFWG2_t.general_parameter.ch == 2)
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = sFWG2_t.general_parameter.ch2_set_temp_f_display;
                sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch2_set_wind;
                sFWG2_t.Direct_handle_parameter.set_time = sFWG2_t.general_parameter.ch2_set_time;
            }
            else if (sFWG2_t.general_parameter.ch == 3)
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = sFWG2_t.general_parameter.ch3_set_temp_f_display;
                sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch3_set_wind;
                sFWG2_t.Direct_handle_parameter.set_time = sFWG2_t.general_parameter.ch3_set_time;
            }
            else if (sFWG2_t.general_parameter.ch == 4)
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = sFWG2_t.general_parameter.ch4_set_temp_f_display;
                sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch4_set_wind;
                sFWG2_t.Direct_handle_parameter.set_time = sFWG2_t.general_parameter.ch4_set_time;
            }
        }
        else if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
        {
            if (sFWG2_t.general_parameter.ch == 1)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch1_set_temp;
                sFWG2_t.Direct_handle_parameter.set_temp_f_display  =  9 * sFWG2_t.Direct_handle_parameter.set_temp  / 5  + 32;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch1_set_wind;
                sFWG2_t.Direct_handle_parameter.set_time  = sFWG2_t.general_parameter.ch1_set_time;
            }
            else if (sFWG2_t.general_parameter.ch == 2)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch2_set_temp;
                sFWG2_t.Direct_handle_parameter.set_temp_f_display  =  9 * sFWG2_t.Direct_handle_parameter.set_temp  / 5  + 32;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch2_set_wind;
                sFWG2_t.Direct_handle_parameter.set_time = sFWG2_t.general_parameter.ch2_set_time;
            }
            else if (sFWG2_t.general_parameter.ch == 3)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch3_set_temp;
                sFWG2_t.Direct_handle_parameter.set_temp_f_display  =  9 * sFWG2_t.Direct_handle_parameter.set_temp  / 5  + 32;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch3_set_wind;
                sFWG2_t.Direct_handle_parameter.set_time = sFWG2_t.general_parameter.ch3_set_time;
            }
            else if (sFWG2_t.general_parameter.ch == 4)
            {
                sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch4_set_temp;
                sFWG2_t.Direct_handle_parameter.set_temp_f_display  =  9 * sFWG2_t.Direct_handle_parameter.set_temp  / 5  + 32;
                sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch4_set_wind;
                sFWG2_t.Direct_handle_parameter.set_time = sFWG2_t.general_parameter.ch4_set_time;
            }
        }

        sbeep.status = BEEP_SHORT;
        EVENT = END_EVENT;
        break;

    case TIME_COUNTDOWN_EVENT:
        break;

    case VALUE_RESET_EVENT:
        break;

    case END_EVENT:
        break;
    }
}



void key_handle(void)
{
    static  handle_event event = END_EVENT;
    static  uint8_t set_done = FALSE;
    static  uint8_t time = 0;
#if 1

    if (key_event[0] != KEY_NONE ||
            key_event[1] != KEY_NONE ||
            key_event[2] != KEY_NONE)
    {
        if (sFWG2_t.Direct_handle_state == HANDLE_SLEEP)
        {
            if (sFWG2_t.general_parameter.fwg2_page != PAGE_DIRECT_CURVE)
            {
                sFWG2_t.Direct_handle_state = HANDLE_WORKING;
            }
        }
        else if (sFWG2_t.Direct_handle_state == HANDLE_WORKING)
        {
            sFWG2_t.Direct_handle_parameter.sleep_time = 0;
        }
    }

    if (key_event[0] == KEY_NONE ||
            key_event[1] == KEY_NONE ||
            key_event[2] == KEY_NONE)
    {
        if (sFWG2_t.general_parameter.key_setting_flag)
        {
            sFWG2_t.general_parameter.setting_time ++;

            if (sFWG2_t.general_parameter.setting_time >= 30)
            {
                sFWG2_t.general_parameter.setting_time = 0;
                sFWG2_t.general_parameter.key_setting_flag = false;
            }
        }
    }

#endif
    KeyProc();
    key_event_handle();
}

