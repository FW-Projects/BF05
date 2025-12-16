#include "dwin_handle.h"
#include "beep_handle.h"
#include "FWG2_handle.h"
#include "stdio.h"
DwinObjectType sdwin;
// channel number

static uint8_t direct_temp_buff[14] = {0x5a, 0xa5, 0x0b, 0x82, 0x03, 0x10, 0x5a, 0xa5, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00};
static uint8_t direct_wind_buff[14] = {0x5a, 0xa5, 0x0b, 0x82, 0x03, 0x10, 0x5a, 0xa5, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00};

uint8_t page_switch[10] = {0x5a, 0xa5, 0x07, 0x82, 0x00, 0x84, 0x5a, 0x01, 0x00, 0x01};

static uint8_t software_buff[9] = {0x5A, 0xA5, 0x06, 0x82, 0x50, 0x39, 0x00, 0x00, 0x00};
static uint8_t hardtware_buff[9] = {0x5A, 0xA5, 0x06, 0x82, 0x50, 0x47, 0x00, 0x00, 0x00};

uint16_t show_direct_set_temp_time;
uint16_t show_direct_set_wind_time;

uint16_t show_direct_curve_set_temp_time;
uint16_t show_direct_curve_set_wind_time;

bool show_popup_flag = false;

static void WriteDataToLCD(DwinObjectType *dwim, uint16_t startAddress, uint16_t length, uint16_t data);
static void RecvDataFromLCD(DwinObjectType *dwim);
/* CRC16校验（X16+X15+X2+1）*/
static uint16_t CalcDwinCRC16(uint8_t *buf, uint16_t length);

static void WriteDataToLCD(DwinObjectType *dwim, uint16_t startAddress, uint16_t length, uint16_t data)
{
    /*命令的长度由帧头（2个字节）+数据长度（1个字节）+指令（1个字节）+起始地址（2个字节）+数据（长度为length）*/
    dwim->tx_buff[0] = 0x5a;
    dwim->tx_buff[1] = 0xa5;
    dwim->tx_buff[2] = (uint8_t)length + 3;
    dwim->tx_buff[3] = FC_VAR_Write;
    dwim->tx_buff[4] = (uint8_t)((startAddress >> 8) & 0xff); // 起始地址
    dwim->tx_buff[5] = (uint8_t)(startAddress & 0XFF);        // 起始地址
    dwim->tx_buff[6] = (uint8_t)((data >> 8) & 0xff);
    dwim->tx_buff[7] = (uint8_t)(data & 0XFF);
    /* send data */
    usart_sendData(DWIN_USART, dwim->tx_buff, MAX_SEND_LEN);
}

static void RecvDataFromLCD(DwinObjectType *dwim)
{
    static uint16_t cal_temp = 0;
    static uint16_t ch1_press_time = 0;
    static uint16_t ch1_long_press_time = 0;
    static uint16_t ch2_press_time = 0;
    static uint16_t ch2_long_press_time = 0;
    static uint16_t ch3_press_time = 0;
    static uint16_t ch3_long_press_time = 0;
    static uint16_t ch4_press_time = 0;
    static uint16_t ch4_long_press_time = 0;
    static bool ch1_short_press_flag = false;
    static bool ch1_long_press_flag = false;
    static bool ch2_short_press_flag = false;
    static bool ch2_long_press_flag = false;
    static bool ch3_short_press_flag = false;
    static bool ch3_long_press_flag = false;
    static bool ch4_short_press_flag = false;
    static bool ch4_long_press_flag = false;
    static fwg2_fn_key_long_set_e   last_fn_key_long_set;
    static fwg2_fn_key_short_set_e  last_fn_key_short_set;
    dwim->read_size = usart_receiveData(DWIN_USART, dwim->rx_buff);
#if 1

    if (dwim->read_size == MAX_RECV_LEN)
    {
        // sbeep.status = BEEP_SHORT;

        /* check head */
        if (CHECK_HEAD(dwim->rx_buff[FRAME_HEAD1], dwim->rx_buff[FRAME_HEAD2]))
        {
            /* get data */
            switch (dwim->rx_buff[FRAME_Address_L])
            {
            /* start the page 1 data revc of */
            case SHOW_DIRECT_ERROR_CODE:
                break;

            case SHOW_DIRECT_HANDLE_MODE:
                break;

            case SHOW_DIRECT_WORKING_STATE:
                break;

            case SHOW_DIRECT_FAN_GIF_H:
                break;

            case SHOW_DIRECT_FAN_GIF_L:
                break;

            case SHOW_DIRECT_CURRENT_TEMP_BAR:
#if 1
                sFWG2_t.general_parameter.adjust_key_temporary_set = TEMPORARY_SELECT_TEMP;

                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                {
                    sFWG2_t.Direct_handle_parameter.set_temp = (dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L] * 2.5);

                    if (sFWG2_t.Direct_handle_parameter.set_temp <= MIN_SET_TEMP_VAL)
                    {
                        sFWG2_t.Direct_handle_parameter.set_temp = MIN_SET_TEMP_VAL;
                    }

                    sFWG2_t.Direct_handle_parameter.set_temp_f_display = 9 * sFWG2_t.Direct_handle_parameter.set_temp / 5 + 32;
                    break;
                }

#endif
                break;

            case SHOW_DIRECT_CURRENT_WIND_BAR:
#if 1
                sFWG2_t.general_parameter.adjust_key_temporary_set = TEMPORARY_SELECT_WIND;

                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                {
                    sFWG2_t.Direct_handle_parameter.set_wind = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.Direct_handle_parameter.set_wind <= MIN_SET_WIND_VAL)
                    {
                        sFWG2_t.Direct_handle_parameter.set_wind = MIN_SET_WIND_VAL;
                    }

                    break;
                }
                else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
                {
                    sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.Direct_handle_parameter.cold_mode_set_wind <= 1)
                    {
                        sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = 1;
                    }

                    break;
                }

#endif
                break;

            case SHOW_COUNTDOWN:
                break;

            /* direct handle temp set */
            case SHOW_DIRECT_SET_TEMP:
#if 1
                sFWG2_t.Direct_handle_parameter.last_set_temp = 0;

                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                {
                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.Direct_handle_parameter.set_temp_f_display = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];
                        sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                    }
                    else
                    {
                        sFWG2_t.Direct_handle_parameter.set_temp = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];
                        sFWG2_t.Direct_handle_parameter.set_temp_f_display = 9 * sFWG2_t.Direct_handle_parameter.set_temp / 5 + 32;
                    }

                    if (sFWG2_t.Direct_handle_parameter.set_temp <= MIN_SET_TEMP_VAL)
                    {
                        sFWG2_t.Direct_handle_parameter.set_temp = MIN_SET_TEMP_VAL;
                    }

                    if (sFWG2_t.Direct_handle_parameter.set_temp >= MAX_SET_TEMP_VAL)
                    {
                        sFWG2_t.Direct_handle_parameter.set_temp = MAX_SET_TEMP_VAL;
                    }

                    if (sFWG2_t.Direct_handle_parameter.set_temp_f_display <= 212)
                    {
                        sFWG2_t.Direct_handle_parameter.set_temp_f_display = 212;
                    }

                    if (sFWG2_t.Direct_handle_parameter.set_temp_f_display >= 932)
                    {
                        sFWG2_t.Direct_handle_parameter.set_temp_f_display = 932;
                    }
                }

#endif
                break;

            /* direct handle wind set */
            case SHOW_DIRECT_SET_WIND:
#if 1
                sFWG2_t.Direct_handle_parameter.last_set_wind = 0;

                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                {
                    sFWG2_t.Direct_handle_parameter.set_wind = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.Direct_handle_parameter.set_wind <= MIN_SET_WIND_VAL)
                    {
                        sFWG2_t.Direct_handle_parameter.set_wind = MIN_SET_WIND_VAL;
                    }

                    if (sFWG2_t.Direct_handle_parameter.set_wind >= 200)
                    {
                        sFWG2_t.Direct_handle_parameter.set_wind = 200;
                    }
                }
                else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
                {
                    sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.Direct_handle_parameter.cold_mode_set_wind <= 1)
                    {
                        sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = 1;
                    }

                    if (sFWG2_t.Direct_handle_parameter.cold_mode_set_wind >= 200)
                    {
                        sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = 200;
                    }
                }

#endif
                break;

            /* set handle channel */
            case CHANNEL1_PRESS:
                sFWG2_t.general_parameter.countdown_flag = false;

                if (sFWG2_t.Direct_handle_state == HANDLE_SLEEP)
                {
                    sFWG2_t.Direct_handle_state = HANDLE_WORKING;
                }
                else if (sFWG2_t.Direct_handle_state == HANDLE_WORKING)
                {
                    sFWG2_t.Direct_handle_parameter.sleep_time = 0;
                }

                ch1_short_press_flag = true;
                break;

            case CHANNEL1_LONG_PRESS:
                if (dwim->rx_buff[FRAME_VAL_L] == 0)
                {
                    if (ch1_long_press_flag == FALSE)
                    {
                        ch1_short_press_flag = false;
                        ch1_long_press_time++;
                    }

                    if (ch1_long_press_time >= 10)
                    {
                        ch1_long_press_time = 0;
                        ch1_long_press_flag = true;
                        sbeep.status = BEEP_LONG;
                        sFWG2_t.general_parameter.ch1_set_wind = sFWG2_t.Direct_handle_parameter.set_wind;
                        sFWG2_t.general_parameter.ch1_set_time = sFWG2_t.general_parameter.countdown_time;

                        if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                        {
                            sFWG2_t.general_parameter.ch1_set_temp = sFWG2_t.Direct_handle_parameter.set_temp;
                            sFWG2_t.general_parameter.ch1_set_temp_f_display = 9 * sFWG2_t.general_parameter.ch1_set_temp / 5 + 32;
                            /* show ch1 set val */
                            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH1_SET_TEMP), DWIN_DATA_BITS,
                                            sFWG2_t.general_parameter.ch1_set_temp);
                        }
                        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                        {
                            sFWG2_t.general_parameter.ch1_set_temp_f_display = sFWG2_t.Direct_handle_parameter.set_temp_f_display;
                            sFWG2_t.general_parameter.ch1_set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                            /* show ch1 set val */
                            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH1_SET_TEMP), DWIN_DATA_BITS,
                                            sFWG2_t.general_parameter.ch1_set_temp_f_display);
                        }

                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH1_SET_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.ch1_set_wind);
                        /* show save channel mes */
                        show_popup_flag = true;
                    }
                }

                if (dwim->rx_buff[FRAME_VAL_L] == 1)
                {
                    ch1_long_press_time = 0;
                    ch1_long_press_flag = false;
                }

                break;

            /* set handle channel */
            case CHANNEL2_PRESS:
                sFWG2_t.general_parameter.countdown_flag = false;

                if (sFWG2_t.Direct_handle_state == HANDLE_SLEEP)
                {
                    sFWG2_t.Direct_handle_state = HANDLE_WORKING;
                }
                else if (sFWG2_t.Direct_handle_state == HANDLE_WORKING)
                {
                    sFWG2_t.Direct_handle_parameter.sleep_time = 0;
                }

                ch2_short_press_flag = true;
                break;

            case CHANNEL2_LONG_PRESS:
                if (dwim->rx_buff[FRAME_VAL_L] == 0)
                {
                    if (ch2_long_press_flag == FALSE)
                    {
                        ch2_short_press_flag = false;
                        ch2_long_press_time++;
                    }

                    if (ch2_long_press_time >= 10)
                    {
                        ch2_long_press_time = 0;
                        ch2_long_press_flag = true;
                        sbeep.status = BEEP_LONG;
                        sFWG2_t.general_parameter.ch2_set_wind = sFWG2_t.Direct_handle_parameter.set_wind;
                        sFWG2_t.general_parameter.ch2_set_time = sFWG2_t.general_parameter.countdown_time;

                        if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                        {
                            sFWG2_t.general_parameter.ch2_set_temp = sFWG2_t.Direct_handle_parameter.set_temp;
                            sFWG2_t.general_parameter.ch2_set_temp_f_display = 9 * sFWG2_t.general_parameter.ch2_set_temp / 5 + 32;
                            /* show ch2 set val */
                            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH2_SET_TEMP), DWIN_DATA_BITS,
                                            sFWG2_t.general_parameter.ch2_set_temp);
                        }
                        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                        {
                            sFWG2_t.general_parameter.ch2_set_temp_f_display = sFWG2_t.Direct_handle_parameter.set_temp_f_display;
                            sFWG2_t.general_parameter.ch2_set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                            /* show ch2 set val */
                            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH2_SET_TEMP), DWIN_DATA_BITS,
                                            sFWG2_t.general_parameter.ch2_set_temp_f_display);
                        }

                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH2_SET_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.ch2_set_wind);
                        /* show save channel mes */
                        show_popup_flag = true;
                    }
                }

                if (dwim->rx_buff[FRAME_VAL_L] == 1)
                {
                    ch2_long_press_time = 0;
                    ch2_long_press_flag = false;
                }

                break;

            /* set handle channel */
            case CHANNEL3_PRESS:
                sFWG2_t.general_parameter.countdown_flag = false;

                if (sFWG2_t.Direct_handle_state == HANDLE_SLEEP)
                {
                    sFWG2_t.Direct_handle_state = HANDLE_WORKING;
                }
                else if (sFWG2_t.Direct_handle_state == HANDLE_WORKING)
                {
                    sFWG2_t.Direct_handle_parameter.sleep_time = 0;
                }

                ch3_short_press_flag = true;
                break;

            case CHANNEL3_LONG_PRESS:
                if (dwim->rx_buff[FRAME_VAL_L] == 0)
                {
                    if (ch3_long_press_flag == FALSE)
                    {
                        ch3_short_press_flag = false;
                        ch3_long_press_time++;
                    }

                    if (ch3_long_press_time >= 10)
                    {
                        ch3_long_press_time = 0;
                        ch3_long_press_flag = true;
                        sbeep.status = BEEP_LONG;
                        sFWG2_t.general_parameter.ch3_set_wind = sFWG2_t.Direct_handle_parameter.set_wind;
                        sFWG2_t.general_parameter.ch3_set_time = sFWG2_t.general_parameter.countdown_time;

                        if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                        {
                            sFWG2_t.general_parameter.ch3_set_temp = sFWG2_t.Direct_handle_parameter.set_temp;
                            sFWG2_t.general_parameter.ch3_set_temp_f_display = 9 * sFWG2_t.general_parameter.ch3_set_temp / 5 + 32;
                            /* show ch3 set val */
                            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH3_SET_TEMP), DWIN_DATA_BITS,
                                            sFWG2_t.general_parameter.ch3_set_temp);
                        }
                        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                        {
                            sFWG2_t.general_parameter.ch3_set_temp_f_display = sFWG2_t.Direct_handle_parameter.set_temp_f_display;
                            sFWG2_t.general_parameter.ch3_set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                            /* show ch3 set val */
                            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH3_SET_TEMP), DWIN_DATA_BITS,
                                            sFWG2_t.general_parameter.ch3_set_temp_f_display);
                        }

                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH3_SET_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.ch3_set_wind);
                        /* show save channel mes */
                        show_popup_flag = true;
                    }
                }

                if (dwim->rx_buff[FRAME_VAL_L] == 1)
                {
                    ch3_long_press_time = 0;
                    ch3_long_press_flag = false;
                }

                break;

            /* set handle channel */
            case CHANNEL4_PRESS:
                sFWG2_t.general_parameter.countdown_flag = false;

                if (sFWG2_t.Direct_handle_state == HANDLE_SLEEP)
                {
                    sFWG2_t.Direct_handle_state = HANDLE_WORKING;
                }
                else if (sFWG2_t.Direct_handle_state == HANDLE_WORKING)
                {
                    sFWG2_t.Direct_handle_parameter.sleep_time = 0;
                }

                ch4_short_press_flag = true;
                break;

            case CHANNEL4_LONG_PRESS:
                if (dwim->rx_buff[FRAME_VAL_L] == 0)
                {
                    if (ch4_long_press_flag == FALSE)
                    {
                        ch4_short_press_flag = false;
                        ch4_long_press_time++;
                    }

                    if (ch4_long_press_time >= 10)
                    {
                        ch4_long_press_time = 0;
                        ch4_long_press_flag = true;
                        sbeep.status = BEEP_LONG;
                        sFWG2_t.general_parameter.ch4_set_wind = sFWG2_t.Direct_handle_parameter.set_wind;
                        sFWG2_t.general_parameter.ch4_set_time = sFWG2_t.general_parameter.countdown_time;

                        if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                        {
                            sFWG2_t.general_parameter.ch4_set_temp = sFWG2_t.Direct_handle_parameter.set_temp;
                            sFWG2_t.general_parameter.ch4_set_temp_f_display = 9 * sFWG2_t.general_parameter.ch4_set_temp / 5 + 32;
                            /* show ch4 set val */
                            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH4_SET_TEMP), DWIN_DATA_BITS,
                                            sFWG2_t.general_parameter.ch4_set_temp);
                        }
                        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                        {
                            sFWG2_t.general_parameter.ch4_set_temp_f_display = sFWG2_t.Direct_handle_parameter.set_temp_f_display;
                            sFWG2_t.general_parameter.ch4_set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                            /* show ch4 set val */
                            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH4_SET_TEMP), DWIN_DATA_BITS,
                                            sFWG2_t.general_parameter.ch4_set_temp_f_display);
                        }

                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH4_SET_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.ch4_set_wind);
                        /* show save channel mes */
                        show_popup_flag = true;
                    }
                }

                if (dwim->rx_buff[FRAME_VAL_L] == 1)
                {
                    ch4_long_press_time = 0;
                    ch4_long_press_flag = false;
                }

                break;

            /* end the page 1 data revc of */

            /* start the page 3 and page 4 data revc of */
            case PAGE_ID:
                sFWG2_t.general_parameter.fwg2_page = (fwg2_page_e)dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];
                break;

            /* end the page 3 and page 4 data revc of */

            /* start the page 5 data revc of */
            case FWG2_WORK_MODE:
#if 1
                if ((fwg2_work_e)dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L] == 0)
                {
                    sFWG2_t.general_parameter.enhance_state = ENHANCE_OPEN;
                    sFWG2_t.general_parameter.work_mode = NORMAL;
                }
                else if ((fwg2_work_e)dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L] == 1)
                {
                    sFWG2_t.general_parameter.work_mode = NORMAL;
                    sFWG2_t.general_parameter.enhance_state = ENHANCE_CLOSE;
                }
                else if ((fwg2_work_e)dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L] == 2)
                {
                    sFWG2_t.general_parameter.work_mode = CODE;
                    sFWG2_t.general_parameter.enhance_state = ENHANCE_CLOSE;
                    /* jump to page 28 */
                    page_switch[9] = PAGE_SHOW_CODE_WORK;
                    usart_sendData(DWIN_USART, page_switch, 10);
                    sFWG2_t.general_parameter.fwg2_page = PAGE_SHOW_CODE_WORK;
                }

#endif
                break;

            /* end the page 5 data revc of */

            /* start the page 6 data revc of */
            case SHOW_CH1_SET_TEMP:
#if 1
                if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sFWG2_t.general_parameter.ch1_set_temp_f_display = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];
                    sFWG2_t.general_parameter.ch1_set_temp = (sFWG2_t.general_parameter.ch1_set_temp_f_display - 32) * 5 / 9;
                }
                else if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sFWG2_t.general_parameter. ch1_set_temp = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];
                    sFWG2_t.general_parameter. ch1_set_temp_f_display = 9 * sFWG2_t.general_parameter. ch1_set_temp_f_display / 5 + 32;
                }

                if (sFWG2_t.general_parameter.ch1_set_temp <= MIN_SET_TEMP_VAL)
                {
                    sFWG2_t.general_parameter.ch1_set_temp = MIN_SET_TEMP_VAL;
                }

                if (sFWG2_t.general_parameter.ch1_set_temp >= MAX_SET_TEMP_VAL)
                {
                    sFWG2_t.general_parameter.ch1_set_temp = MAX_SET_TEMP_VAL;
                }

                if (sFWG2_t.general_parameter.ch1_set_temp_f_display <= MIN_SET_TEMP_F_VAL)
                {
                    sFWG2_t.general_parameter.ch1_set_temp_f_display = MIN_SET_TEMP_F_VAL;
                }

                if (sFWG2_t.general_parameter.ch1_set_temp_f_display >= MAX_SET_TEMP_F_VAL)
                {
                    sFWG2_t.general_parameter.ch1_set_temp_f_display = MAX_SET_TEMP_F_VAL;
                }

#endif
                break;

            case SHOW_CH1_SET_WIND:
#if 1
                sFWG2_t.general_parameter.ch1_set_wind = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                if (sFWG2_t.general_parameter.ch1_set_wind >= MAX_SET_WIND_VAL)
                {
                    sFWG2_t.general_parameter.ch1_set_wind = MAX_SET_WIND_VAL;
                }

                if (sFWG2_t.general_parameter.ch1_set_wind <= MIN_SET_WIND_VAL)
                {
                    sFWG2_t.general_parameter.ch1_set_wind = MIN_SET_WIND_VAL;
                }

#endif
                break;

            case SHOW_CH1_SET_TIME:
#if 1
                sFWG2_t.general_parameter.ch1_set_time = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                if (sFWG2_t.general_parameter.ch1_set_time >= MAX_SET_TIME_VAL)
                {
                    sFWG2_t.general_parameter.ch1_set_time = MAX_SET_TIME_VAL;
                }

                if (sFWG2_t.general_parameter.ch1_set_time <= MIN_SET_TIME_VAL)
                {
                    sFWG2_t.general_parameter.ch1_set_time = MIN_SET_TIME_VAL;
                }

#endif
                break;

            case SHOW_CH2_SET_TEMP:
#if 1
                if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sFWG2_t.general_parameter.ch2_set_temp_f_display = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];
                    sFWG2_t.general_parameter.ch2_set_temp = (sFWG2_t.general_parameter.ch2_set_temp_f_display - 32) * 5 / 9;
                }
                else if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sFWG2_t.general_parameter. ch2_set_temp = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];
                    sFWG2_t.general_parameter. ch2_set_temp_f_display = 9 * sFWG2_t.general_parameter. ch2_set_temp_f_display / 5 + 32;
                }

                if (sFWG2_t.general_parameter.ch2_set_temp <= MIN_SET_TEMP_VAL)
                {
                    sFWG2_t.general_parameter.ch2_set_temp = MIN_SET_TEMP_VAL;
                }

                if (sFWG2_t.general_parameter.ch2_set_temp >= MAX_SET_TEMP_VAL)
                {
                    sFWG2_t.general_parameter.ch2_set_temp = MAX_SET_TEMP_VAL;
                }

                if (sFWG2_t.general_parameter.ch2_set_temp_f_display <= MIN_SET_TEMP_F_VAL)
                {
                    sFWG2_t.general_parameter.ch2_set_temp_f_display = MIN_SET_TEMP_F_VAL;
                }

                if (sFWG2_t.general_parameter.ch2_set_temp_f_display >= MAX_SET_TEMP_F_VAL)
                {
                    sFWG2_t.general_parameter.ch2_set_temp_f_display = MAX_SET_TEMP_F_VAL;
                }

#endif
                break;

            case SHOW_CH2_SET_WIND:
#if 1
                sFWG2_t.general_parameter.ch2_set_wind = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                if (sFWG2_t.general_parameter.ch2_set_wind >= MAX_SET_WIND_VAL)
                {
                    sFWG2_t.general_parameter.ch2_set_wind = MAX_SET_WIND_VAL;
                }

                if (sFWG2_t.general_parameter.ch2_set_wind <= MIN_SET_WIND_VAL)
                {
                    sFWG2_t.general_parameter.ch2_set_wind = MIN_SET_WIND_VAL;
                }

#endif
                break;

            case SHOW_CH2_SET_TIME:
#if 1
                sFWG2_t.general_parameter.ch2_set_time = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                if (sFWG2_t.general_parameter.ch2_set_time >= MAX_SET_TIME_VAL)
                {
                    sFWG2_t.general_parameter.ch2_set_time = MAX_SET_TIME_VAL;
                }

                if (sFWG2_t.general_parameter.ch2_set_time <= MIN_SET_TIME_VAL)
                {
                    sFWG2_t.general_parameter.ch2_set_time = MIN_SET_TIME_VAL;
                }

#endif
                break;

            case SHOW_CH3_SET_TEMP:
#if 1
                if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sFWG2_t.general_parameter.ch3_set_temp_f_display = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];
                    sFWG2_t.general_parameter.ch3_set_temp = (sFWG2_t.general_parameter.ch3_set_temp_f_display - 32) * 5 / 9;
                }
                else if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sFWG2_t.general_parameter. ch3_set_temp = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];
                    sFWG2_t.general_parameter. ch3_set_temp_f_display = 9 * sFWG2_t.general_parameter. ch3_set_temp_f_display / 5 + 32;
                }

                if (sFWG2_t.general_parameter.ch3_set_temp <= MIN_SET_TEMP_VAL)
                {
                    sFWG2_t.general_parameter.ch3_set_temp = MIN_SET_TEMP_VAL;
                }

                if (sFWG2_t.general_parameter.ch3_set_temp >= MAX_SET_TEMP_VAL)
                {
                    sFWG2_t.general_parameter.ch3_set_temp = MAX_SET_TEMP_VAL;
                }

                if (sFWG2_t.general_parameter.ch3_set_temp_f_display <= MIN_SET_TEMP_F_VAL)
                {
                    sFWG2_t.general_parameter.ch3_set_temp_f_display = MIN_SET_TEMP_F_VAL;
                }

                if (sFWG2_t.general_parameter.ch3_set_temp_f_display >= MAX_SET_TEMP_F_VAL)
                {
                    sFWG2_t.general_parameter.ch3_set_temp_f_display = MAX_SET_TEMP_F_VAL;
                }

#endif
                break;

            case SHOW_CH3_SET_WIND:
#if 1
                sFWG2_t.general_parameter.ch3_set_wind = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                if (sFWG2_t.general_parameter.ch3_set_wind >= MAX_SET_WIND_VAL)
                {
                    sFWG2_t.general_parameter.ch3_set_wind = MAX_SET_WIND_VAL;
                }

                if (sFWG2_t.general_parameter.ch3_set_wind <= MIN_SET_WIND_VAL)
                {
                    sFWG2_t.general_parameter.ch3_set_wind = MIN_SET_WIND_VAL;
                }

#endif
                break;

            case SHOW_CH3_SET_TIME:
#if 1
                sFWG2_t.general_parameter.ch3_set_time = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                if (sFWG2_t.general_parameter.ch3_set_time >= MAX_SET_TIME_VAL)
                {
                    sFWG2_t.general_parameter.ch3_set_time = MAX_SET_TIME_VAL;
                }

                if (sFWG2_t.general_parameter.ch3_set_time <= MIN_SET_TIME_VAL)
                {
                    sFWG2_t.general_parameter.ch3_set_time = MIN_SET_TIME_VAL;
                }

#endif
                break;

            case SHOW_CH4_SET_TEMP:
#if 1
                if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sFWG2_t.general_parameter.ch4_set_temp_f_display = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];
                    sFWG2_t.general_parameter.ch4_set_temp = (sFWG2_t.general_parameter.ch4_set_temp_f_display - 32) * 5 / 9;
                }
                else if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sFWG2_t.general_parameter. ch4_set_temp = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];
                    sFWG2_t.general_parameter. ch4_set_temp_f_display = 9 * sFWG2_t.general_parameter. ch4_set_temp_f_display / 5 + 32;
                }

                if (sFWG2_t.general_parameter.ch4_set_temp <= MIN_SET_TEMP_VAL)
                {
                    sFWG2_t.general_parameter.ch4_set_temp = MIN_SET_TEMP_VAL;
                }

                if (sFWG2_t.general_parameter.ch4_set_temp >= MAX_SET_TEMP_VAL)
                {
                    sFWG2_t.general_parameter.ch4_set_temp = MAX_SET_TEMP_VAL;
                }

                if (sFWG2_t.general_parameter.ch4_set_temp_f_display <= MIN_SET_TEMP_F_VAL)
                {
                    sFWG2_t.general_parameter.ch4_set_temp_f_display = MIN_SET_TEMP_F_VAL;
                }

                if (sFWG2_t.general_parameter.ch4_set_temp_f_display >= MAX_SET_TEMP_F_VAL)
                {
                    sFWG2_t.general_parameter.ch4_set_temp_f_display = MAX_SET_TEMP_F_VAL;
                }

#endif
                break;

            case SHOW_CH4_SET_WIND:
#if 1
                sFWG2_t.general_parameter.ch4_set_wind = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                if (sFWG2_t.general_parameter.ch4_set_wind >= MAX_SET_WIND_VAL)
                {
                    sFWG2_t.general_parameter.ch4_set_wind = MAX_SET_WIND_VAL;
                }

                if (sFWG2_t.general_parameter.ch4_set_wind <= MIN_SET_WIND_VAL)
                {
                    sFWG2_t.general_parameter.ch4_set_wind = MIN_SET_WIND_VAL;
                }

#endif
                break;

            case SHOW_CH4_SET_TIME:
#if 1
                sFWG2_t.general_parameter.ch4_set_time = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                if (sFWG2_t.general_parameter.ch4_set_time >= MAX_SET_TIME_VAL)
                {
                    sFWG2_t.general_parameter.ch4_set_time = MAX_SET_TIME_VAL;
                }

                if (sFWG2_t.general_parameter.ch4_set_time <= MIN_SET_TIME_VAL)
                {
                    sFWG2_t.general_parameter.ch4_set_time = MIN_SET_TIME_VAL;
                }

#endif
                // sFWG2_t.general_parameter.countdown_time  = sFWG2_t.general_parameter.ch4_set_time;
                break;

            /* end the page 6 data revc of */

            /* start the page 7 data revc of */
            case CODE_CHANNEL:
                sFWG2_t.general_parameter.code_ch = dwim->rx_buff[FRAME_VAL_L];
                break;

            case SET_PRE_TEMP:
#if 1
                if (sFWG2_t.general_parameter.code_ch == 0)
                {
                    sFWG2_t.general_parameter.code0_pre_temp = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.general_parameter.code0_pre_temp = (sFWG2_t.general_parameter.code0_pre_temp - 32) * 5 / 9;
                    }

                    if (sFWG2_t.general_parameter.code0_pre_temp <= MIN_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code0_pre_temp = MIN_SET_TEMP_VAL;
                    }

                    if (sFWG2_t.general_parameter.code0_pre_temp >= MAX_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code0_pre_temp = MAX_SET_TEMP_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 1)
                {
                    sFWG2_t.general_parameter.code1_pre_temp = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.general_parameter.code1_pre_temp = (sFWG2_t.general_parameter.code1_pre_temp - 32) * 5 / 9;
                    }

                    if (sFWG2_t.general_parameter.code1_pre_temp <= MIN_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code1_pre_temp = MIN_SET_TEMP_VAL;
                    }

                    if (sFWG2_t.general_parameter.code1_pre_temp >= MAX_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code1_pre_temp = MAX_SET_TEMP_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 2)
                {
                    sFWG2_t.general_parameter.code2_pre_temp = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.general_parameter.code2_pre_temp = (sFWG2_t.general_parameter.code2_pre_temp - 32) * 5 / 9;
                    }

                    if (sFWG2_t.general_parameter.code2_pre_temp <= MIN_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code2_pre_temp = MIN_SET_TEMP_VAL;
                    }

                    if (sFWG2_t.general_parameter.code2_pre_temp >= MAX_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code2_pre_temp = MAX_SET_TEMP_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 3)
                {
                    sFWG2_t.general_parameter.code3_pre_temp = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.general_parameter.code3_pre_temp = (sFWG2_t.general_parameter.code3_pre_temp - 32) * 5 / 9;
                    }

                    if (sFWG2_t.general_parameter.code3_pre_temp <= MIN_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code3_pre_temp = MIN_SET_TEMP_VAL;
                    }

                    if (sFWG2_t.general_parameter.code3_pre_temp >= MAX_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code3_pre_temp = MAX_SET_TEMP_VAL;
                    }
                }

#endif
                break;

            case SET_PRE_WIND:
#if 1
                if (sFWG2_t.general_parameter.code_ch == 0)
                {
                    sFWG2_t.general_parameter.code0_pre_wind = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code0_pre_wind >= MAX_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code0_pre_wind = MAX_SET_WIND_VAL;
                    }

                    if (sFWG2_t.general_parameter.code0_pre_wind <= MIN_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code0_pre_wind = MIN_SET_WIND_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 1)
                {
                    sFWG2_t.general_parameter.code1_pre_wind = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code1_pre_wind >= MAX_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code1_pre_wind = MAX_SET_WIND_VAL;
                    }

                    if (sFWG2_t.general_parameter.code1_pre_wind <= MIN_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code1_pre_wind = MIN_SET_WIND_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 2)
                {
                    sFWG2_t.general_parameter.code2_pre_wind = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code2_pre_wind >= MAX_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code2_pre_wind = MAX_SET_WIND_VAL;
                    }

                    if (sFWG2_t.general_parameter.code2_pre_wind <= MIN_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code2_pre_wind = MIN_SET_WIND_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 3)
                {
                    sFWG2_t.general_parameter.code3_pre_wind = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code3_pre_wind >= MAX_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code3_pre_wind = MAX_SET_WIND_VAL;
                    }

                    if (sFWG2_t.general_parameter.code3_pre_wind <= MIN_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code3_pre_wind = MIN_SET_WIND_VAL;
                    }
                }

#endif
                break;

            case SET_PRE_TIME:
#if 1
                if (sFWG2_t.general_parameter.code_ch == 0)
                {
                    sFWG2_t.general_parameter.code0_pre_time = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code0_pre_time >= MAX_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code0_pre_time = MAX_SET_TIME_VAL;
                    }

                    if (sFWG2_t.general_parameter.code0_pre_time <= MIN_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code0_pre_time = MIN_SET_TIME_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 1)
                {
                    sFWG2_t.general_parameter.code1_pre_time = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code1_pre_time >= MAX_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code1_pre_time = MAX_SET_TIME_VAL;
                    }

                    if (sFWG2_t.general_parameter.code1_pre_time <= MIN_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code1_pre_time = MIN_SET_TIME_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 2)
                {
                    sFWG2_t.general_parameter.code2_pre_time = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code2_pre_time >= MAX_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code2_pre_time = MAX_SET_TIME_VAL;
                    }

                    if (sFWG2_t.general_parameter.code2_pre_time <= MIN_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code2_pre_time = MIN_SET_TIME_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 3)
                {
                    sFWG2_t.general_parameter.code3_pre_time = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code3_pre_time >= MAX_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code3_pre_time = MAX_SET_TIME_VAL;
                    }

                    if (sFWG2_t.general_parameter.code3_pre_time <= MIN_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code3_pre_time = MIN_SET_TIME_VAL;
                    }
                }

#endif
                break;

            case SET_WORK_TEMP1:
#if 1
                if (sFWG2_t.general_parameter.code_ch == 0)
                {
                    sFWG2_t.general_parameter.code0_temp_1 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.general_parameter.code0_temp_1 = (sFWG2_t.general_parameter.code0_temp_1 - 32) * 5 / 9;
                    }

                    if (sFWG2_t.general_parameter.code0_temp_1 <= MIN_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code0_temp_1 = MIN_SET_TEMP_VAL;
                    }

                    if (sFWG2_t.general_parameter.code0_temp_1 >= MAX_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code0_temp_1 = MAX_SET_TEMP_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 1)
                {
                    sFWG2_t.general_parameter.code1_temp_1 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.general_parameter.code1_temp_1 = (sFWG2_t.general_parameter.code1_temp_1 - 32) * 5 / 9;
                    }

                    if (sFWG2_t.general_parameter.code1_temp_1 <= MIN_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code1_temp_1 = MIN_SET_TEMP_VAL;
                    }

                    if (sFWG2_t.general_parameter.code1_temp_1 >= MAX_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code1_temp_1 = MAX_SET_TEMP_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 2)
                {
                    sFWG2_t.general_parameter.code2_temp_1 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.general_parameter.code2_temp_1 = (sFWG2_t.general_parameter.code2_temp_1 - 32) * 5 / 9;
                    }

                    if (sFWG2_t.general_parameter.code2_temp_1 <= MIN_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code2_temp_1 = MIN_SET_TEMP_VAL;
                    }

                    if (sFWG2_t.general_parameter.code2_temp_1 >= MAX_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code2_temp_1 = MAX_SET_TEMP_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 3)
                {
                    sFWG2_t.general_parameter.code3_temp_1 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.general_parameter.code3_temp_1 = (sFWG2_t.general_parameter.code3_temp_1 - 32) * 5 / 9;
                    }

                    if (sFWG2_t.general_parameter.code3_temp_1 <= MIN_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code3_temp_1 = MIN_SET_TEMP_VAL;
                    }

                    if (sFWG2_t.general_parameter.code3_temp_1 >= MAX_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code3_temp_1 = MAX_SET_TEMP_VAL;
                    }
                }

#endif
                break;

            case SET_WORK_WIND1:
#if 1
                if (sFWG2_t.general_parameter.code_ch == 0)
                {
                    sFWG2_t.general_parameter.code0_wind_1 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code0_wind_1 >= MAX_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code0_wind_1 = MAX_SET_WIND_VAL;
                    }

                    if (sFWG2_t.general_parameter.code0_wind_1 <= MIN_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code0_wind_1 = MIN_SET_WIND_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 1)
                {
                    sFWG2_t.general_parameter.code1_wind_1 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code1_wind_1 >= MAX_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code1_wind_1 = MAX_SET_WIND_VAL;
                    }

                    if (sFWG2_t.general_parameter.code1_wind_1 <= MIN_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code1_wind_1 = MIN_SET_WIND_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 2)
                {
                    sFWG2_t.general_parameter.code2_wind_1 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code2_wind_1 >= MAX_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code2_wind_1 = MAX_SET_WIND_VAL;
                    }

                    if (sFWG2_t.general_parameter.code2_wind_1 <= MIN_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code2_wind_1 = MIN_SET_WIND_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 3)
                {
                    sFWG2_t.general_parameter.code3_wind_1 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code3_wind_1 >= MAX_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code3_wind_1 = MAX_SET_WIND_VAL;
                    }

                    if (sFWG2_t.general_parameter.code3_wind_1 <= MIN_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code3_wind_1 = MIN_SET_WIND_VAL;
                    }
                }

#endif
                break;

            case SET_WORK_TIME1:
#if 1
                if (sFWG2_t.general_parameter.code_ch == 0)
                {
                    sFWG2_t.general_parameter.code0_time_1 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code0_time_1 >= MAX_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code0_time_1 = MAX_SET_TIME_VAL;
                    }

                    if (sFWG2_t.general_parameter.code0_time_1 <= MIN_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code0_time_1 = MIN_SET_TIME_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 1)
                {
                    sFWG2_t.general_parameter.code1_time_1 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code1_time_1 >= MAX_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code1_time_1 = MAX_SET_TIME_VAL;
                    }

                    if (sFWG2_t.general_parameter.code1_time_1 <= MIN_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code1_time_1 = MIN_SET_TIME_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 2)
                {
                    sFWG2_t.general_parameter.code2_time_1 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code2_time_1 >= MAX_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code2_time_1 = MAX_SET_TIME_VAL;
                    }

                    if (sFWG2_t.general_parameter.code2_time_1 <= MIN_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code2_time_1 = MIN_SET_TIME_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 3)
                {
                    sFWG2_t.general_parameter.code3_time_1 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code3_time_1 >= MAX_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code3_time_1 = MAX_SET_TIME_VAL;
                    }

                    if (sFWG2_t.general_parameter.code3_time_1 <= MIN_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code3_time_1 = MIN_SET_TIME_VAL;
                    }
                }

#endif
                break;

            case SET_WORK_TEMP2:
#if 1
                if (sFWG2_t.general_parameter.code_ch == 0)
                {
                    sFWG2_t.general_parameter.code0_temp_2 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.general_parameter.code0_temp_2 = (sFWG2_t.general_parameter.code0_temp_2 - 32) * 5 / 9;
                    }

                    if (sFWG2_t.general_parameter.code0_temp_2 <= MIN_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code0_temp_2 = MIN_SET_TEMP_VAL;
                    }

                    if (sFWG2_t.general_parameter.code0_temp_2 >= MAX_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code0_temp_2 = MAX_SET_TEMP_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 1)
                {
                    sFWG2_t.general_parameter.code1_temp_2 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.general_parameter.code1_temp_2 = (sFWG2_t.general_parameter.code1_temp_2 - 32) * 5 / 9;
                    }

                    if (sFWG2_t.general_parameter.code1_temp_2 <= MIN_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code1_temp_2 = MIN_SET_TEMP_VAL;
                    }

                    if (sFWG2_t.general_parameter.code1_temp_2 >= MAX_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code1_temp_2 = MAX_SET_TEMP_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 2)
                {
                    sFWG2_t.general_parameter.code2_temp_2 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.general_parameter.code2_temp_2 = (sFWG2_t.general_parameter.code2_temp_2 - 32) * 5 / 9;
                    }

                    if (sFWG2_t.general_parameter.code2_temp_2 <= MIN_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code2_temp_2 = MIN_SET_TEMP_VAL;
                    }

                    if (sFWG2_t.general_parameter.code2_temp_2 >= MAX_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code2_temp_2 = MAX_SET_TEMP_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 3)
                {
                    sFWG2_t.general_parameter.code3_temp_2 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.general_parameter.code3_temp_2 = (sFWG2_t.general_parameter.code3_temp_2 - 32) * 5 / 9;
                    }

                    if (sFWG2_t.general_parameter.code3_temp_2 <= MIN_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code3_temp_2 = MIN_SET_TEMP_VAL;
                    }

                    if (sFWG2_t.general_parameter.code3_temp_2 >= MAX_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code3_temp_2 = MAX_SET_TEMP_VAL;
                    }
                }

#endif
                break;

            case SET_WORK_WIND2:
#if 1
                if (sFWG2_t.general_parameter.code_ch == 0)
                {
                    sFWG2_t.general_parameter.code0_wind_2 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code0_wind_2 >= MAX_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code0_wind_2 = MAX_SET_WIND_VAL;
                    }

                    if (sFWG2_t.general_parameter.code0_wind_2 <= MIN_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code0_wind_2 = MIN_SET_WIND_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 1)
                {
                    sFWG2_t.general_parameter.code1_wind_2 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code1_wind_2 >= MAX_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code1_wind_2 = MAX_SET_WIND_VAL;
                    }

                    if (sFWG2_t.general_parameter.code1_wind_2 <= MIN_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code1_wind_2 = MIN_SET_WIND_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 2)
                {
                    sFWG2_t.general_parameter.code2_wind_2 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code2_wind_2 >= MAX_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code2_wind_2 = MAX_SET_WIND_VAL;
                    }

                    if (sFWG2_t.general_parameter.code2_wind_2 <= MIN_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code2_wind_2 = MIN_SET_WIND_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 3)
                {
                    sFWG2_t.general_parameter.code3_wind_2 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code3_wind_2 >= MAX_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code3_wind_2 = MAX_SET_WIND_VAL;
                    }

                    if (sFWG2_t.general_parameter.code3_wind_2 <= MIN_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code3_wind_2 = MIN_SET_WIND_VAL;
                    }
                }

#endif
                break;

            case SET_WORK_TIME2:
#if 1
                if (sFWG2_t.general_parameter.code_ch == 0)
                {
                    sFWG2_t.general_parameter.code0_time_2 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code0_time_2 >= MAX_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code0_time_2 = MAX_SET_TIME_VAL;
                    }

                    if (sFWG2_t.general_parameter.code0_time_2 <= MIN_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code0_time_2 = MIN_SET_TIME_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 1)
                {
                    sFWG2_t.general_parameter.code1_time_2 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code1_time_2 >= MAX_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code1_time_2 = MAX_SET_TIME_VAL;
                    }

                    if (sFWG2_t.general_parameter.code1_time_2 <= MIN_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code1_time_2 = MIN_SET_TIME_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 2)
                {
                    sFWG2_t.general_parameter.code2_time_2 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code2_time_2 >= MAX_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code2_time_2 = MAX_SET_TIME_VAL;
                    }

                    if (sFWG2_t.general_parameter.code2_time_2 <= MIN_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code2_time_2 = MIN_SET_TIME_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 3)
                {
                    sFWG2_t.general_parameter.code3_time_2 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code3_time_2 >= MAX_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code3_time_2 = MAX_SET_TIME_VAL;
                    }

                    if (sFWG2_t.general_parameter.code3_time_2 <= MIN_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code3_time_2 = MIN_SET_TIME_VAL;
                    }
                }

#endif
                break;

            case SET_WORK_TEMP3:
#if 1
                if (sFWG2_t.general_parameter.code_ch == 0)
                {
                    sFWG2_t.general_parameter.code0_temp_3 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.general_parameter.code0_temp_3 = (sFWG2_t.general_parameter.code0_temp_3 - 32) * 5 / 9;
                    }

                    if (sFWG2_t.general_parameter.code0_temp_3 <= MIN_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code0_temp_3 = MIN_SET_TEMP_VAL;
                    }

                    if (sFWG2_t.general_parameter.code0_temp_3 >= MAX_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code0_temp_3 = MAX_SET_TEMP_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 1)
                {
                    sFWG2_t.general_parameter.code1_temp_3 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.general_parameter.code1_temp_3 = (sFWG2_t.general_parameter.code1_temp_3 - 32) * 5 / 9;
                    }

                    if (sFWG2_t.general_parameter.code1_temp_3 <= MIN_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code1_temp_3 = MIN_SET_TEMP_VAL;
                    }

                    if (sFWG2_t.general_parameter.code1_temp_3 >= MAX_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code1_temp_3 = MAX_SET_TEMP_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 2)
                {
                    sFWG2_t.general_parameter.code2_temp_3 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.general_parameter.code2_temp_3 = (sFWG2_t.general_parameter.code2_temp_3 - 32) * 5 / 9;
                    }

                    if (sFWG2_t.general_parameter.code2_temp_3 <= MIN_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code2_temp_3 = MIN_SET_TEMP_VAL;
                    }

                    if (sFWG2_t.general_parameter.code2_temp_3 >= MAX_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code2_temp_3 = MAX_SET_TEMP_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 3)
                {
                    sFWG2_t.general_parameter.code3_temp_3 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.general_parameter.code3_temp_3 = (sFWG2_t.general_parameter.code3_temp_3 - 32) * 5 / 9;
                    }

                    if (sFWG2_t.general_parameter.code3_temp_3 <= MIN_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code3_temp_3 = MIN_SET_TEMP_VAL;
                    }

                    if (sFWG2_t.general_parameter.code3_temp_3 >= MAX_SET_TEMP_VAL)
                    {
                        sFWG2_t.general_parameter.code3_temp_3 = MAX_SET_TEMP_VAL;
                    }
                }

#endif
                break;

            case SET_WORK_WIND3:
#if 1
                if (sFWG2_t.general_parameter.code_ch == 0)
                {
                    sFWG2_t.general_parameter.code0_wind_3 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code0_wind_3 >= MAX_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code0_wind_3 = MAX_SET_WIND_VAL;
                    }

                    if (sFWG2_t.general_parameter.code0_wind_3 <= MIN_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code0_wind_3 = MIN_SET_WIND_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 1)
                {
                    sFWG2_t.general_parameter.code1_wind_3 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code1_wind_3 >= MAX_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code1_wind_3 = MAX_SET_WIND_VAL;
                    }

                    if (sFWG2_t.general_parameter.code1_wind_3 <= MIN_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code1_wind_3 = MIN_SET_WIND_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 2)
                {
                    sFWG2_t.general_parameter.code2_wind_3 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code2_wind_3 >= MAX_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code2_wind_3 = MAX_SET_WIND_VAL;
                    }

                    if (sFWG2_t.general_parameter.code2_wind_3 <= MIN_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code2_wind_3 = MIN_SET_WIND_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 3)
                {
                    sFWG2_t.general_parameter.code3_wind_3 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code3_wind_3 >= MAX_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code3_wind_3 = MAX_SET_WIND_VAL;
                    }

                    if (sFWG2_t.general_parameter.code3_wind_3 <= MIN_SET_WIND_VAL)
                    {
                        sFWG2_t.general_parameter.code3_wind_3 = MIN_SET_WIND_VAL;
                    }
                }

#endif
                break;

            case SET_WORK_TIME3:
#if 1
                if (sFWG2_t.general_parameter.code_ch == 0)
                {
                    sFWG2_t.general_parameter.code0_time_3 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code0_time_3 >= MAX_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code0_time_3 = MAX_SET_TIME_VAL;
                    }

                    if (sFWG2_t.general_parameter.code0_time_3 <= MIN_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code0_time_3 = MIN_SET_TIME_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 1)
                {
                    sFWG2_t.general_parameter.code1_time_3 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code1_time_3 >= MAX_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code1_time_3 = MAX_SET_TIME_VAL;
                    }

                    if (sFWG2_t.general_parameter.code1_time_3 <= MIN_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code1_time_3 = MIN_SET_TIME_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 2)
                {
                    sFWG2_t.general_parameter.code2_time_3 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code2_time_3 >= MAX_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code2_time_3 = MAX_SET_TIME_VAL;
                    }

                    if (sFWG2_t.general_parameter.code2_time_3 <= MIN_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code2_time_3 = MIN_SET_TIME_VAL;
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 3)
                {
                    sFWG2_t.general_parameter.code3_time_3 = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.code3_time_3 >= MAX_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code3_time_3 = MAX_SET_TIME_VAL;
                    }

                    if (sFWG2_t.general_parameter.code3_time_3 <= MIN_SET_TIME_VAL)
                    {
                        sFWG2_t.general_parameter.code3_time_3 = MIN_SET_TIME_VAL;
                    }
                }

#endif
                break;

            case SHOW_CODE_STEP:
                break;

            /* end the page 7 data revc of */

            /* start the page 8 data revc of */
            case SET_TEMP_UNITS:
                sFWG2_t.general_parameter.temp_uint = (fwg2_temp_unit_e)dwim->rx_buff[FRAME_VAL_L];
                break;

            /* end the page 8 data revc of */

            /* start the page 9 data revc of */
            case SET_SPEAKER_FUNCTION:
                sFWG2_t.general_parameter.speak_state = (fwg2_speaker_state_e)dwim->rx_buff[FRAME_VAL_L];
                break;

            /* end the page 9 data revc of */
            /* start the page 10 data revc of */
            case SET_DISPLAY_LOCK_MODE:
                sFWG2_t.general_parameter.display_lock_state = (fwg2_temp_lock_state_e)dwim->rx_buff[FRAME_VAL_L];
                break;

            /* end the page 10 data revc of */

            /* start the page 11 data revc of */
            case SET_FN_KEY_LONG_MODE:
                sFWG2_t.general_parameter.fn_key_long_set = (fwg2_fn_key_long_set_e)dwim->rx_buff[FRAME_VAL_H] * 256 +
                    dwim->rx_buff[FRAME_VAL_L];

                if (last_fn_key_long_set != sFWG2_t.general_parameter.fn_key_long_set)
                {
                    sFWG2_t.general_parameter.countdown_flag = false;
                    sFWG2_t.Direct_handle_work_mode = NORMAL_MODE;
                    last_fn_key_long_set = sFWG2_t.general_parameter.fn_key_long_set;
                }

                break;

            case SET_FN_KEY_SHORT_MODE:
                sFWG2_t.general_parameter.fn_key_short_set = (fwg2_fn_key_short_set_e)dwim->rx_buff[FRAME_VAL_H] * 256 +
                    dwim->rx_buff[FRAME_VAL_L];

                if (last_fn_key_short_set != sFWG2_t.general_parameter.fn_key_short_set)
                {
                    sFWG2_t.general_parameter.countdown_flag = false;
                    sFWG2_t.Direct_handle_work_mode = NORMAL_MODE;
                    last_fn_key_short_set = sFWG2_t.general_parameter.fn_key_short_set;
                }

                break;

            case SET_ADJUST_KEY:
                sFWG2_t.general_parameter.adjust_key_set = (fwg2_adjust_key_set_e)dwim->rx_buff[FRAME_VAL_H] * 256 +
                    dwim->rx_buff[FRAME_VAL_L];
                break;

            /* end the page 11 data revc of */
            case SET_ENHANCE_TEMP:
#if 1
                sFWG2_t.Direct_handle_parameter.quick_work_temp = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    if (sFWG2_t.Direct_handle_parameter.quick_work_temp <= 33)
                    {
                        sFWG2_t.Direct_handle_parameter.quick_work_temp = 33;
                    }

                    if (sFWG2_t.Direct_handle_parameter.quick_work_temp >= 212)
                    {
                        sFWG2_t.Direct_handle_parameter.quick_work_temp = 212;
                    }

                    sFWG2_t.Direct_handle_parameter.quick_work_temp = (sFWG2_t.Direct_handle_parameter.quick_work_temp - 32) * 5 / 9;
                }
                else if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    if (sFWG2_t.Direct_handle_parameter.quick_work_temp <= 0)
                    {
                        sFWG2_t.Direct_handle_parameter.quick_work_temp = 0;
                    }

                    if (sFWG2_t.Direct_handle_parameter.quick_work_temp >= 100)
                    {
                        sFWG2_t.Direct_handle_parameter.quick_work_temp = 100;
                    }
                }

#endif
                break;

            case SET_ENHANCE_COUNTDOWN_TIME:
#if 1
                sFWG2_t.Direct_handle_parameter.quick_work_time = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                if (sFWG2_t.Direct_handle_parameter.quick_work_time >= MAX_SET_TIME_VAL)
                {
                    sFWG2_t.Direct_handle_parameter.quick_work_time = MAX_SET_TIME_VAL;
                }

                if (sFWG2_t.Direct_handle_parameter.quick_work_time <= MIN_SET_TIME_VAL)
                {
                    sFWG2_t.Direct_handle_parameter.quick_work_time = MIN_SET_TIME_VAL;
                }

#endif
                break;

            case SET_DIRECT_CALIBRATE_TEMP:
#if 1
                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                {
                    cal_temp = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        cal_temp = (cal_temp - 32) *
                                   5 / 9;
                    }

                    sFWG2_t.Direct_handle_parameter.set_calibration_temp += sFWG2_t.Direct_handle_parameter.set_temp - cal_temp;

                    if (sFWG2_t.Direct_handle_parameter.set_calibration_temp >= 100)
                    {
                        sFWG2_t.Direct_handle_parameter.set_calibration_temp = 100;
                    }

                    if (sFWG2_t.Direct_handle_parameter.set_calibration_temp <= -100)
                    {
                        sFWG2_t.Direct_handle_parameter.set_calibration_temp = -100;
                    }
                }

#endif
                break;

            case SET_TIME:
#if 1
                sFWG2_t.general_parameter.countdown_time = dwim->rx_buff[FRAME_VAL_H] * 256 + dwim->rx_buff[FRAME_VAL_L];

                if (sFWG2_t.general_parameter.countdown_time > MAX_SET_TIME_VAL)
                {
                    sFWG2_t.general_parameter.countdown_time = MAX_SET_TIME_VAL;
                }

                if (sFWG2_t.general_parameter.countdown_time < MIN_SET_TIME_VAL)
                {
                    sFWG2_t.general_parameter.countdown_time = MIN_SET_TIME_VAL;
                }

#endif
                break;

            case SET_TOUCH_FUNCTION:
                sFWG2_t.general_parameter.touch_key_set = (fwg2_touch_key_set_e)dwim->rx_buff[FRAME_VAL_L];
                break;

            case SET_SLEEP_MODE:
                sFWG2_t.general_parameter.fwg2_sleep_state = (fwg2_sleep_state_e)dwim->rx_buff[FRAME_VAL_L];
                break;

            case SET_UART_FUNCTION:
                sFWG2_t.general_parameter.uart_state = (fwg2_uart_state_e)dwim->rx_buff[FRAME_VAL_L];
                break;

            case RESET_FWG2:
#if 1
                sFWG2_t.general_parameter.reset_fwg2_flag = dwim->rx_buff[FRAME_VAL_L];

                if (sFWG2_t.general_parameter.reset_fwg2_flag == true)
                {
                    sFWG2_t.general_parameter.fwg2_sleep_state = SLEEP_OPEN;
                    /* reset direct handle's value */
                    sFWG2_t.Direct_handle_parameter.set_temp = 380;
                    sFWG2_t.Direct_handle_parameter.set_temp_f_display = 716;
                    sFWG2_t.Direct_handle_parameter.set_wind = 40;
                    sFWG2_t.Direct_handle_parameter.set_time = 10;
                    sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = 100;
                    sFWG2_t.Direct_handle_parameter.quick_work_temp = 50;
                    sFWG2_t.Direct_handle_parameter.quick_work_time = 30;
                    sFWG2_t.Direct_handle_parameter.set_calibration_temp = 0;
                    /* reset general value */
                    sFWG2_t.general_parameter.temp_uint = CELSIUS;
                    sFWG2_t.general_parameter.speak_state = SPEAKER_OPEN;
                    sFWG2_t.general_parameter.display_lock_state = LOCK;
                    sFWG2_t.general_parameter.fn_key_long_set = L_COLD_WIN_MODE;
                    sFWG2_t.general_parameter.fn_key_short_set = S_CHANNEL_SWITCH;
                    sFWG2_t.general_parameter.adjust_key_set = SELECT_TEMP;
                    sFWG2_t.general_parameter.ota_state = OTA_OFF;
                    sFWG2_t.general_parameter.touch_key_set = TOUCH_CLOSE;
                    sFWG2_t.general_parameter.fwg2_sleep_state = SLEEP_OPEN;
                    sFWG2_t.general_parameter.uart_state = UART_CLOSE;
                    sFWG2_t.general_parameter.enhance_state = ENHANCE_CLOSE;
					
                    sFWG2_t.general_parameter.countdown_time = 10;
                    sFWG2_t.general_parameter.ch1_set_temp = 350;
                    sFWG2_t.general_parameter.ch1_set_temp_f_display = 662;
                    sFWG2_t.general_parameter.ch1_set_wind = 20;
                    sFWG2_t.general_parameter.ch1_set_time = 10;
                    sFWG2_t.general_parameter.ch2_set_temp = 380;
                    sFWG2_t.general_parameter.ch2_set_temp_f_display = 716;
                    sFWG2_t.general_parameter.ch2_set_wind = 30;
                    sFWG2_t.general_parameter.ch2_set_time = 15;
                    sFWG2_t.general_parameter.ch3_set_temp = 400;
                    sFWG2_t.general_parameter.ch3_set_temp_f_display = 752;
                    sFWG2_t.general_parameter.ch3_set_wind = 40;
                    sFWG2_t.general_parameter.ch3_set_time = 20;
                    sFWG2_t.general_parameter.ch4_set_temp = 450;
                    sFWG2_t.general_parameter.ch4_set_temp_f_display = 842;
                    sFWG2_t.general_parameter.ch4_set_wind = 40;
                    sFWG2_t.general_parameter.ch4_set_time = 10;
#if 1
                    sFWG2_t.general_parameter.code0_pre_temp = 150;
                    sFWG2_t.general_parameter.code0_pre_wind = 40;
                    sFWG2_t.general_parameter.code0_pre_time = 10;
                    sFWG2_t.general_parameter.code0_temp_1 = 260;
                    sFWG2_t.general_parameter.code0_wind_1 = 20;
                    sFWG2_t.general_parameter.code0_time_1 = 3;
                    sFWG2_t.general_parameter.code0_temp_2 = 280;
                    sFWG2_t.general_parameter.code0_wind_2 = 20;
                    sFWG2_t.general_parameter.code0_time_2 = 4;
                    sFWG2_t.general_parameter.code0_temp_3 = 100;
                    sFWG2_t.general_parameter.code0_wind_3 = 20;
                    sFWG2_t.general_parameter.code0_time_3 = 5;
					
                    sFWG2_t.general_parameter.code1_pre_temp = 150;
                    sFWG2_t.general_parameter.code1_pre_wind = 40;
                    sFWG2_t.general_parameter.code1_pre_time = 3;
                    sFWG2_t.general_parameter.code1_temp_1 = 260;
                    sFWG2_t.general_parameter.code1_wind_1 = 20;
                    sFWG2_t.general_parameter.code1_time_1 = 3;
                    sFWG2_t.general_parameter.code1_temp_2 = 380;
                    sFWG2_t.general_parameter.code1_wind_2 = 40;
                    sFWG2_t.general_parameter.code1_time_2 = 10;
                    sFWG2_t.general_parameter.code1_temp_3 = 260;
                    sFWG2_t.general_parameter.code1_wind_3 = 20;
                    sFWG2_t.general_parameter.code1_time_3 = 5;
					
                    sFWG2_t.general_parameter.code2_pre_temp = 400;
                    sFWG2_t.general_parameter.code2_pre_wind = 30;
                    sFWG2_t.general_parameter.code2_pre_time = 4;
                    sFWG2_t.general_parameter.code2_temp_1 = 450;
                    sFWG2_t.general_parameter.code2_wind_1 = 40;
                    sFWG2_t.general_parameter.code2_time_1 = 8;
                    sFWG2_t.general_parameter.code2_temp_2 = 380;
                    sFWG2_t.general_parameter.code2_wind_2 = 40;
                    sFWG2_t.general_parameter.code2_time_2 = 4;
                    sFWG2_t.general_parameter.code2_temp_3 = 260;
                    sFWG2_t.general_parameter.code2_wind_3 = 20;
                    sFWG2_t.general_parameter.code2_time_3 = 5;
					
                    sFWG2_t.general_parameter.code3_pre_temp = 400;
                    sFWG2_t.general_parameter.code3_pre_wind = 30;
                    sFWG2_t.general_parameter.code3_pre_time = 4;
                    sFWG2_t.general_parameter.code3_temp_1 = 500;
                    sFWG2_t.general_parameter.code3_wind_1 = 30;
                    sFWG2_t.general_parameter.code3_time_1 = 6;
                    sFWG2_t.general_parameter.code3_temp_2 = 380;
                    sFWG2_t.general_parameter.code3_wind_2 = 40;
                    sFWG2_t.general_parameter.code3_time_2 = 4;
                    sFWG2_t.general_parameter.code3_temp_3 = 260;
                    sFWG2_t.general_parameter.code3_wind_3 = 40;
                    sFWG2_t.general_parameter.code3_time_3 = 5;
                    sFWG2_t.general_parameter.ch = 0;
#endif
                }

#endif
                break;

            case SET_OTA_FUNCTION:
                sFWG2_t.general_parameter.ota_state = (fwg2_ota_state_e)dwim->rx_buff[FRAME_VAL_L];
                break;

            case EN_HANCE_FUNCTION:
                sFWG2_t.general_parameter.enhance_state = (fwg2_enhance_state_e)dwim->rx_buff[FRAME_VAL_L];
                break;

            case START_STOP_CODE_MODE:
                sFWG2_t.general_parameter.code_mode_state = (fwg2_code_mode_state_e)dwim->rx_buff[FRAME_VAL_H] * 256 +
                    dwim->rx_buff[FRAME_VAL_L];

                if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_START)
                {
                    sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code0_pre_time;
                    sFWG2_t.general_parameter.code_mode_step = CODE_PRE_HEAT;
                }
                else if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_STOP)
                {
                    sFWG2_t.general_parameter.code_mode_step = CODE_WAIT;
                }

                break;

            case LONG_PRESS_RELESS:
                break;

            case TOUCH_CHANGE_TEMP:
                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                {
                    sFWG2_t.Direct_handle_parameter.last_set_temp = 0;

                    /* reduce temp */
                    if (dwim->rx_buff[FRAME_VAL_L] == 0)
                    {
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

                            sFWG2_t.Direct_handle_parameter.set_temp_f_display = 9 * sFWG2_t.Direct_handle_parameter.set_temp / 5 + 32;
                        }
                        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                        {
                            if (sFWG2_t.Direct_handle_parameter.set_temp_f_display <= 212)
                            {
                                sFWG2_t.Direct_handle_parameter.set_temp_f_display = 212;
                            }
                            else
                            {
                                sFWG2_t.Direct_handle_parameter.set_temp_f_display--;
                            }

                            sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                        }
                    }
                    /* increase temp */
                    else if (dwim->rx_buff[FRAME_VAL_L] == 1)
                    {
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

                            sFWG2_t.Direct_handle_parameter.set_temp_f_display = 9 * sFWG2_t.Direct_handle_parameter.set_temp / 5 + 32;
                        }
                        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                        {
                            if (sFWG2_t.Direct_handle_parameter.set_temp_f_display >= 932)
                            {
                                sFWG2_t.Direct_handle_parameter.set_temp_f_display = 932;
                            }
                            else
                            {
                                sFWG2_t.Direct_handle_parameter.set_temp_f_display++;
                            }

                            sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                        }
                    }
                }

                break;

            case LONG_TOUCH_CHANGE_TEMP:
                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                {
                    sFWG2_t.Direct_handle_parameter.last_set_temp = 0;

                    /* reduce temp */
                    if (dwim->rx_buff[FRAME_VAL_L] == 0)
                    {
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

                            sFWG2_t.Direct_handle_parameter.set_temp_f_display = 9 * sFWG2_t.Direct_handle_parameter.set_temp / 5 + 32;
                        }
                        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                        {
                            if (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 38 <= 212)
                            {
                                sFWG2_t.Direct_handle_parameter.set_temp_f_display = 212;
                            }
                            else
                            {
                                sFWG2_t.Direct_handle_parameter.set_temp_f_display -= 38;
                            }

                            sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                        }
                    }
                    /* increase temp */
                    else if (dwim->rx_buff[FRAME_VAL_L] == 1)
                    {
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

                            sFWG2_t.Direct_handle_parameter.set_temp_f_display = 9 * sFWG2_t.Direct_handle_parameter.set_temp / 5 + 32;
                        }
                        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                        {
                            if (sFWG2_t.Direct_handle_parameter.set_temp_f_display + 38 >= 932)
                            {
                                sFWG2_t.Direct_handle_parameter.set_temp_f_display = 932;
                            }
                            else
                            {
                                sFWG2_t.Direct_handle_parameter.set_temp_f_display += 38;
                            }

                            sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
                        }
                    }
                }

                break;

            case TOUCH_CHANGE_WIND:

                /* reduce wind  */
                if (dwim->rx_buff[FRAME_VAL_L] == 0)
                {
                    if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                    {
                        sFWG2_t.Direct_handle_parameter.last_set_wind = 0;

                        if (sFWG2_t.Direct_handle_parameter.set_wind <= MIN_SET_WIND_VAL)
                        {
                            sFWG2_t.Direct_handle_parameter.set_wind = MIN_SET_WIND_VAL;
                        }
                        else
                        {
                            sFWG2_t.Direct_handle_parameter.set_wind--;
                        }
                    }
                    else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
                    {
                        if (sFWG2_t.Direct_handle_parameter.cold_mode_set_wind <= MIN_SET_WIND_VAL)
                        {
                            sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = MIN_SET_WIND_VAL;
                        }
                        else
                        {
                            sFWG2_t.Direct_handle_parameter.cold_mode_set_wind--;
                        }
                    }
                }
                /* increase wind */
                else if (dwim->rx_buff[FRAME_VAL_L] == 1)
                {
                    if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                    {
                        sFWG2_t.Direct_handle_parameter.last_set_wind = 0;

                        if (sFWG2_t.Direct_handle_parameter.set_wind >= MAX_SET_WIND_VAL)
                        {
                            sFWG2_t.Direct_handle_parameter.set_wind = MAX_SET_WIND_VAL;
                        }
                        else
                        {
                            sFWG2_t.Direct_handle_parameter.set_wind++;
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
                            sFWG2_t.Direct_handle_parameter.cold_mode_set_wind++;
                        }
                    }
                }

                break;

            case LONG_TOUCH_CHANGE_WIND:

                /* reduce wind  */
                if (dwim->rx_buff[FRAME_VAL_L] == 0)
                {
                    if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
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
                    else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
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
                }
                /* increase wind */
                else if (dwim->rx_buff[FRAME_VAL_L] == 1)
                {
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
                }

                break;
            }
        }

        dwim->read_size = 0;
    }

    if (ch1_short_press_flag)
    {
        if (ch1_long_press_flag == false)
        {
            ch1_press_time++;

            if (ch1_press_time > 200) // over 200ms
            {
                ch1_short_press_flag = false;
                ch1_press_time = 0;

                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                {
                    sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
                    sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
                    sFWG2_t.general_parameter.ch = 1;

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.Direct_handle_parameter.set_temp_f_display  = sFWG2_t.general_parameter.ch1_set_temp_f_display;
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
                    sFWG2_t.general_parameter.countdown_time = sFWG2_t.general_parameter.ch1_set_time;
                    /* show select channel */
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + CHANNEL_STATE), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.ch);
                }
            }
        }
        else
        {
            ch1_press_time = 0;
        }
    }

    if (ch2_short_press_flag)
    {
        if (ch2_long_press_flag == false)
        {
            ch2_press_time++;

            if (ch2_press_time > 200) // over 200ms
            {
                ch2_short_press_flag = false;
                ch2_press_time = 0;

                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                {
                    sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
                    sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
                    sFWG2_t.general_parameter.ch = 2;

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.Direct_handle_parameter.set_temp_f_display  = sFWG2_t.general_parameter.ch2_set_temp_f_display;
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
                    sFWG2_t.general_parameter.countdown_time = sFWG2_t.general_parameter.ch2_set_time;
                    /* show select channel */
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + CHANNEL_STATE), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.ch);
                }
            }
        }
        else
        {
            ch2_press_time = 0;
        }
    }

    if (ch3_short_press_flag)
    {
        if (ch3_long_press_flag == false)
        {
            ch3_press_time++;

            if (ch3_press_time > 200) // over 200ms
            {
                ch3_short_press_flag = false;
                ch3_press_time = 0;

                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                {
                    sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
                    sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
                    sFWG2_t.general_parameter.ch = 3;

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.Direct_handle_parameter.set_temp_f_display  = sFWG2_t.general_parameter.ch3_set_temp_f_display;
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
                    sFWG2_t.general_parameter.countdown_time = sFWG2_t.general_parameter.ch3_set_time;
                    /* show select channel */
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + CHANNEL_STATE), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.ch);
                }
            }
        }
        else
        {
            ch3_press_time = 0;
        }
    }

    if (ch4_short_press_flag)
    {
        if (ch4_long_press_flag == false)
        {
            ch4_press_time++;

            if (ch4_press_time > 200) // over 200ms
            {
                ch4_short_press_flag = false;
                ch4_press_time = 0;

                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
                {
                    sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
                    sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
                    sFWG2_t.general_parameter.ch = 4;

                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sFWG2_t.Direct_handle_parameter.set_temp_f_display  = sFWG2_t.general_parameter.ch4_set_temp_f_display;
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
                    sFWG2_t.general_parameter.countdown_time = sFWG2_t.general_parameter.ch4_set_time;
                    /* show select channel */
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + CHANNEL_STATE), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.ch);
                }
            }
        }
        else
        {
            ch4_press_time = 0;
        }
    }

#endif
}

/*
 *   case 0:show direct handle's wind value and bar
 *   case 1:show direct handle's set temp and current temp and bar
 *   case 2:show channel's select state
 *   case 3:show direct handle's outpot value and bar
 *   case 4:show countdown time
 *   case 5:show direct handle's work mode
 *   case 6:show direct handle's fan
 */

extern float direct_handle_pid_out;
bool show_direct_currtne_temp_flag = false;
bool show_direct_set_temp_flag = false;
bool show_direct_set_wind_flag = false;

void Page_Direct_Work_Heartbeat_Packet(void)
{
    static uint8_t state = 0;
    static uint8_t time = 0;
    static bool first_in = false;
    static bool exit_cold_mode_flage = false;
    static uint8_t last_channel = 0;
    static uint8_t direct_temp_refirsh_time = DIRECT_TEMP_REFIRSH_TIME;
    static uint8_t direct_wind_refirsh_time = DIRECT_WIND_REFIRSH_TIME;
    static uint16_t last_direct_handle_set_temp_f_display = 0;
    static uint16_t last_direct_handle_current_temp_f_display = 0;
    static uint16_t last_direct_handle_current_temp = 0;
    static uint8_t last_cold_mode_set_wind = 0;
    static handle_state_e last_direct_handle_state = HANDLE_SLEEP;
    time++;
    static uint8_t output_bar = 0;
    static uint16_t show_set_temp = 0;
    static int16_t show_actual_temp = 0;
    static uint8_t temp_bar = 0;

    if (output_bar < (direct_handle_pid_out / 599))
    {
        output_bar++;
    }

    if (output_bar > (direct_handle_pid_out / 599))
    {
        output_bar--;
    }

    if (sFWG2_t.general_parameter.fwg2_page == PAGE_MAIN)
    {
        if (first_in == false)
        {
            time = 0;
            direct_temp_refirsh_time = DIRECT_TEMP_REFIRSH_TIME;
            show_direct_set_temp_flag = false;
            show_direct_set_wind_flag = false;
            last_direct_handle_set_temp_f_display = 0;
            last_direct_handle_current_temp_f_display = 0;
            last_direct_handle_current_temp = 0;
            show_direct_currtne_temp_flag = false;
            last_cold_mode_set_wind = 0;
            last_direct_handle_state = HANDLE_SLEEP;
            sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
            sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
            first_in = true;
        }

        switch (state)
        {
        case 0:
            if (sFWG2_t.general_parameter.work_mode == NORMAL)
            {
                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE || sFWG2_t.Direct_handle_work_mode == QUICK_MODE)
                {
                    // 处理风速显示逻辑
                    if (!show_direct_set_wind_flag)
                    {
                        uint16_t wind_value = sFWG2_t.Direct_handle_parameter.set_wind;
                        bool should_send_wind = false;

                        if ((sFWG2_t.Direct_handle_state == HANDLE_WORKING && sFWG2_t.Direct_handle_position == NOT_IN_POSSITION) ||
                                sFWG2_t.general_parameter.fwg2_sleep_state == SLEEP_CLOSE ||
                                sFWG2_t.Direct_handle_state == HANDLE_SLEEP)
                        {
                            should_send_wind = true;
                        }
                        else if (sFWG2_t.Direct_handle_state == HANDLE_WORKING && sFWG2_t.Direct_handle_position == IN_POSSITION)
                        {
                            if (time % 5 == 0)
                            {
                                wind_value = (sFWG2_t.Direct_handle_parameter.actual_temp < 65) ?
                                             sFWG2_t.Direct_handle_parameter.set_wind :
                                             sFWG2_t.Direct_handle_parameter.stop_set_wind;
                                should_send_wind = true;
                            }
                        }

                        if (should_send_wind)
                        {
                            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_SET_WIND), DWIN_DATA_BITS, wind_value);
                            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_WIND_BAR), DWIN_DATA_BITS, wind_value);
                        }
                    }

                    // 处理风速变化
                    if (sFWG2_t.Direct_handle_parameter.last_set_wind != sFWG2_t.Direct_handle_parameter.set_wind)
                    {
                        sFWG2_t.Direct_handle_parameter.last_set_wind = sFWG2_t.Direct_handle_parameter.set_wind;

                        if (!show_direct_set_wind_flag)
                        {
                            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_WIND), DWIN_DATA_BITS, 1);
                            sdwin.send_data(&sdwin, SHOW_DIRECT_WIND_COLOUR, DWIN_DATA_BITS, BLUE_COLOR);
                            show_direct_set_wind_flag = true;
                        }

                        show_direct_set_wind_time = 0;
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_SET_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.Direct_handle_parameter.set_wind);
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_WIND_BAR), DWIN_DATA_BITS,
                                        sFWG2_t.Direct_handle_parameter.set_wind);
                    }

                    // 处理显示标志超时
                    if (show_direct_set_wind_flag && ++show_direct_set_wind_time >= SHOW_DIRECT_HANDLE_SET_WIND_TIME)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_WIND), DWIN_DATA_BITS, 0);
                        sdwin.send_data(&sdwin, SHOW_DIRECT_WIND_COLOUR, DWIN_DATA_BITS, WHITE_COLOR);
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_SET_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.Direct_handle_parameter.set_wind);
                        show_direct_set_wind_time = 0;
                        show_direct_set_wind_flag = false;
                    }
                }
                else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_WIND), DWIN_DATA_BITS, 1);
                    sdwin.send_data(&sdwin, SHOW_DIRECT_WIND_COLOUR, DWIN_DATA_BITS, BLUE_COLOR);
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_SET_WIND), DWIN_DATA_BITS,
                                    sFWG2_t.Direct_handle_parameter.cold_mode_set_wind);
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_WIND_BAR), DWIN_DATA_BITS,
                                    sFWG2_t.Direct_handle_parameter.cold_mode_set_wind);
                    exit_cold_mode_flage = true;
                }
            }

            // 退出冷风模式处理
            if (sFWG2_t.Direct_handle_work_mode != COLD_WIND_MODE && exit_cold_mode_flage)
            {
                exit_cold_mode_flage = false;
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_WIND), DWIN_DATA_BITS, 0);
                sdwin.send_data(&sdwin, SHOW_DIRECT_WIND_COLOUR, DWIN_DATA_BITS, WHITE_COLOR);
            }

            state++;
            break;

        case 1:
            if (sFWG2_t.general_parameter.enhance_state == ENHANCE_CLOSE ||
                    sFWG2_t.general_parameter.enhance_state == ENHANCE_OPEN)
            {
                // 计算基础温度值（摄氏温度）
                int base_temp_c = sFWG2_t.Direct_handle_parameter.actual_temp +
                                  sFWG2_t.Direct_handle_parameter.linear_calibration_temp -
                                  sFWG2_t.Direct_handle_parameter.set_calibration_temp;

                // 如果开启增强模式，减去增强温度
                if (sFWG2_t.general_parameter.enhance_state == ENHANCE_OPEN)
                {
                    base_temp_c -= ENHANCE_TEMP;
                }

                // 摄氏温度处理
                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    show_actual_temp = (sFWG2_t.Direct_handle_position == IN_POSSITION &&
                                        sFWG2_t.general_parameter.fwg2_sleep_state == SLEEP_OPEN) ?
                                       sFWG2_t.Direct_handle_parameter.actual_temp : base_temp_c;

                    if (show_actual_temp < sFWG2_t.general_parameter.mcu_temp)
                    {
                        show_actual_temp = sFWG2_t.general_parameter.mcu_temp;
                    }

                    temp_bar = show_actual_temp * 0.4;
                    show_set_temp = sFWG2_t.Direct_handle_parameter.set_temp;

                    // 显示锁定检查
                    if (sFWG2_t.general_parameter.display_lock_state == LOCK &&
                            show_actual_temp < sFWG2_t.Direct_handle_parameter.set_temp + LOCK_RANGE &&
                            show_actual_temp > sFWG2_t.Direct_handle_parameter.set_temp - LOCK_RANGE)
                    {
                        show_actual_temp = show_set_temp;
                        temp_bar = show_actual_temp * 0.4;
                    }
                }
                // 华氏温度处理
                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    show_actual_temp = (sFWG2_t.Direct_handle_position == IN_POSSITION &&
                                        sFWG2_t.general_parameter.fwg2_sleep_state == SLEEP_OPEN) ?
                                       sFWG2_t.Direct_handle_parameter.actual_temp * 9 / 5 + 32 : base_temp_c * 9 / 5 + 32;
                    int mcu_temp_f = sFWG2_t.general_parameter.mcu_temp * 9 / 5 + 32;

                    if (show_actual_temp < mcu_temp_f)
                    {
                        show_actual_temp = mcu_temp_f;
                    }

                    temp_bar = show_actual_temp * 0.21;
                    show_set_temp = sFWG2_t.Direct_handle_parameter.set_temp_f_display;

                    // 显示锁定检查
                    if (sFWG2_t.general_parameter.display_lock_state == LOCK &&
                            show_actual_temp < sFWG2_t.Direct_handle_parameter.set_temp_f_display + LOCK_RANGE_F &&
                            show_actual_temp > sFWG2_t.Direct_handle_parameter.set_temp_f_display - LOCK_RANGE_F)
                    {
                        show_actual_temp = show_set_temp;
                        temp_bar = show_actual_temp * 0.21;
                    }
                }
            }

            if (sFWG2_t.general_parameter.work_mode == NORMAL)
            {
                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE ||
                        sFWG2_t.Direct_handle_work_mode == QUICK_MODE  ||
                        sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
                {
                    // 处理设置温度变化
                    if ((sFWG2_t.Direct_handle_parameter.last_set_temp != sFWG2_t.Direct_handle_parameter.set_temp) ||
                            last_direct_handle_set_temp_f_display != sFWG2_t.Direct_handle_parameter.set_temp_f_display)
                    {
                        sFWG2_t.Direct_handle_parameter.last_set_temp = sFWG2_t.Direct_handle_parameter.set_temp;
                        last_direct_handle_set_temp_f_display = sFWG2_t.Direct_handle_parameter.set_temp_f_display;

                        if (!show_direct_set_temp_flag)
                        {
                            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_TEMP), DWIN_DATA_BITS, 1);
                            sdwin.send_data(&sdwin, SHOW_DIRECT_TEMP_COLOUR, DWIN_DATA_BITS, ORANGE_COLOR);
                            show_direct_set_temp_flag = true;
                        }

                        show_direct_set_temp_time = 0;
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP), DWIN_DATA_BITS, show_set_temp);
                        temp_bar = (sFWG2_t.general_parameter.temp_uint == CELSIUS) ?
                                   show_set_temp * 0.4 : show_set_temp * 0.21;
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP_BAR), DWIN_DATA_BITS, temp_bar);
                    }

                    // 处理设置温度显示超时
                    if (show_direct_set_temp_flag && ++show_direct_set_temp_time >= SHOW_DIRECT_HANDLE_SET_TEMP_TIME)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_TEMP), DWIN_DATA_BITS, 0);
                        sdwin.send_data(&sdwin, SHOW_DIRECT_TEMP_COLOUR, DWIN_DATA_BITS, WHITE_COLOR);
                        show_direct_set_temp_time = 0;
                        show_direct_set_temp_flag = false;
                    }
                    // 显示当前温度
                    else if (!show_direct_set_temp_flag && --direct_temp_refirsh_time == 0)
                    {
                        direct_temp_refirsh_time = DIRECT_TEMP_REFIRSH_TIME;

                        if (last_direct_handle_current_temp != sFWG2_t.Direct_handle_parameter.actual_temp ||
                                last_direct_handle_current_temp_f_display != sFWG2_t.Direct_handle_parameter.actual_temp_f_display)
                        {
                            last_direct_handle_current_temp = sFWG2_t.Direct_handle_parameter.actual_temp;
                            last_direct_handle_current_temp_f_display = sFWG2_t.Direct_handle_parameter.actual_temp_f_display;
                            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP), DWIN_DATA_BITS, show_actual_temp);
                            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP_BAR), DWIN_DATA_BITS, temp_bar);
                        }
                    }
                }
            }

            state++;
            break;

        // 状态2-8保持不变...
        case 2:

            // 通道状态显示逻辑（保持不变）
            if (last_channel != sFWG2_t.general_parameter.ch)
            {
                last_channel = sFWG2_t.general_parameter.ch;
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + CHANNEL_STATE), DWIN_DATA_BITS, sFWG2_t.general_parameter.ch);
            }

            // 通道状态检查逻辑（保持不变）
            uint8_t current_ch = sFWG2_t.general_parameter.ch;
            bool need_reset_channel = false;

            if (current_ch == 1)
            {
                need_reset_channel = (sFWG2_t.general_parameter.ch1_set_temp != sFWG2_t.Direct_handle_parameter.set_temp ||
                                      sFWG2_t.general_parameter.ch1_set_wind != sFWG2_t.Direct_handle_parameter.set_wind ||
                                      sFWG2_t.general_parameter.ch1_set_time != sFWG2_t.general_parameter.countdown_time);
            }
            else if (current_ch == 2)
            {
                need_reset_channel = (sFWG2_t.general_parameter.ch2_set_temp != sFWG2_t.Direct_handle_parameter.set_temp ||
                                      sFWG2_t.general_parameter.ch2_set_wind != sFWG2_t.Direct_handle_parameter.set_wind ||
                                      sFWG2_t.general_parameter.ch2_set_time != sFWG2_t.general_parameter.countdown_time);
            }
            else if (current_ch == 3)
            {
                need_reset_channel = (sFWG2_t.general_parameter.ch3_set_temp != sFWG2_t.Direct_handle_parameter.set_temp ||
                                      sFWG2_t.general_parameter.ch3_set_wind != sFWG2_t.Direct_handle_parameter.set_wind ||
                                      sFWG2_t.general_parameter.ch3_set_time != sFWG2_t.general_parameter.countdown_time);
            }
            else if (current_ch == 4)
            {
                need_reset_channel = (sFWG2_t.general_parameter.ch4_set_temp != sFWG2_t.Direct_handle_parameter.set_temp ||
                                      sFWG2_t.general_parameter.ch4_set_wind != sFWG2_t.Direct_handle_parameter.set_wind ||
                                      sFWG2_t.general_parameter.ch4_set_time != sFWG2_t.general_parameter.countdown_time);
            }

            if (need_reset_channel)
            {
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + CHANNEL_STATE), DWIN_DATA_BITS, 0);
            }

            state++;
            break;

        // 状态3-8保持原样...
        case 3:
            if (time % 20 == 0)
            {
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_TPME_OUTPUT_POWER), DWIN_DATA_BITS, output_bar);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_TPME_OUTPUT_POWER_BAR), DWIN_DATA_BITS, output_bar);
            }

            state++;
            break;

        case 4:
            if (sFWG2_t.general_parameter.countdown_flag == true)
            {
                sdwin.send_data(&sdwin, SHOW_DIRECT_TIME_COLOUR, DWIN_DATA_BITS, GREEN_COLOR);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_COUNTDOWN), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.countdown_time_display);
            }
            else
            {
                sdwin.send_data(&sdwin, SHOW_DIRECT_TIME_COLOUR, DWIN_DATA_BITS, WHITE_COLOR);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_COUNTDOWN), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.countdown_time);
            }

            state++;
            break;

        case 5:
            if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
            {
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_HANDLE_MODE), DWIN_DATA_BITS, 0);
            }
            else if (sFWG2_t.Direct_handle_work_mode == QUICK_MODE)
            {
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_HANDLE_MODE), DWIN_DATA_BITS, 1);
            }
            else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
            {
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_HANDLE_MODE), DWIN_DATA_BITS, 2);
            }

            state++;
            break;

        case 6:
            if (sFWG2_t.Direct_handle_parameter.actual_wind <= LOW_WIND_RATE || sFWG2_t.Direct_handle_state == HANDLE_SLEEP)
            {
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_FAN_GIF_H), DWIN_DATA_BITS, 0);
            }
            else
            {
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_FAN_GIF_H), DWIN_DATA_BITS, 1);
            }

            state++;
            break;

        case 7:
            if (sFWG2_t.Direct_handle_error_state != HANDLE_OK)
            {
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_ERROR_CODE), DWIN_DATA_BITS,
                                sFWG2_t.Direct_handle_error_state);
            }

            state++;
            break;

        case 8:
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_WORKING_STATE), DWIN_DATA_BITS,
                            (sFWG2_t.Direct_handle_state == HANDLE_SLEEP) ? 1 : 0);
            state = 0;
            break;
        }
    }
    /* clean data */
    else if (sFWG2_t.general_parameter.fwg2_page != PAGE_MAIN || sFWG2_t.Direct_handle_state == HANDLE_SLEEP)
    {
        first_in = false;
    }

    if (sFWG2_t.general_parameter.fwg2_page == PAGE_DIRECT_CURVE && last_channel != sFWG2_t.general_parameter.ch)
    {
        last_channel = sFWG2_t.general_parameter.ch;
        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + CHANNEL_STATE), DWIN_DATA_BITS, sFWG2_t.general_parameter.ch);
    }
}

/*
 *   case 0:show direct handle's current temp curve
 *   case 1:show direct handle's set wind curve
 *   case 2:show direct handle's channel select state
 *   case 3:show countdown time
 */
void Page_Direct_Curve_Heartbeat_Packet(void)
{
    static uint8_t state = 0;
    static bool first_in = false;
    static bool exit_cold_mode_flage = false;
    //static bool show_direct_set_wind_flag = false;
    static uint8_t direct_wind_refirsh_time = 0;
    static uint8_t last_channel = 0;
    //static bool show_direct_set_temp_flag = false;
    static bool first_in_cold_mode = false;
    static uint16_t last_direct_handle_current_temp = 0;
    static uint16_t last_direct_handle_current_temp_f_display = 0;
    static uint16_t last_direct_handle_set_temp_f_display = 0;
    static uint8_t direct_temp_refirsh_time = 0;
    static uint16_t delay_time = CURVE_REFIRSH_TIME;
    delay_time = CURVE_REFIRSH_TIME;
    static uint16_t show_set_temp = 0;
    static int16_t show_actual_temp = 0;
    static uint8_t temp_bar = 0;

    if (sFWG2_t.general_parameter.fwg2_page == PAGE_DIRECT_CURVE)
    {
        if (first_in == false)
        {
            sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
            show_direct_set_wind_flag = false;
            direct_wind_refirsh_time = 0;
            show_direct_set_temp_flag = false;
            first_in_cold_mode = false;
            last_direct_handle_current_temp = 0;
            last_direct_handle_current_temp_f_display = 0;
            last_direct_handle_set_temp_f_display = 0;
            direct_temp_refirsh_time = 0;
            delay_time = CURVE_REFIRSH_TIME;
            first_in = true;
        }

        switch (state)
        {
        case 0:
#if 1
            if (sFWG2_t.general_parameter.enhance_state == ENHANCE_CLOSE ||
                    sFWG2_t.general_parameter.enhance_state == ENHANCE_OPEN)
            {
                // 计算基础温度值（摄氏温度）
                int base_temp_c = sFWG2_t.Direct_handle_parameter.actual_temp +
                                  sFWG2_t.Direct_handle_parameter.linear_calibration_temp -
                                  sFWG2_t.Direct_handle_parameter.set_calibration_temp;

                // 如果开启增强模式，减去增强温度
                if (sFWG2_t.general_parameter.enhance_state == ENHANCE_OPEN)
                {
                    base_temp_c -= ENHANCE_TEMP;
                }

                // 摄氏温度处理
                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    show_actual_temp = (sFWG2_t.Direct_handle_position == IN_POSSITION &&
                                        sFWG2_t.general_parameter.fwg2_sleep_state == SLEEP_OPEN) ?
                                       sFWG2_t.Direct_handle_parameter.actual_temp : base_temp_c;

                    if (show_actual_temp < sFWG2_t.general_parameter.mcu_temp)
                    {
                        show_actual_temp = sFWG2_t.general_parameter.mcu_temp;
                    }

                    temp_bar = show_actual_temp * 0.4;
                    show_set_temp = sFWG2_t.Direct_handle_parameter.set_temp;

                    // 显示锁定检查
                    if (sFWG2_t.general_parameter.display_lock_state == LOCK &&
                            show_actual_temp < sFWG2_t.Direct_handle_parameter.set_temp + LOCK_RANGE &&
                            show_actual_temp > sFWG2_t.Direct_handle_parameter.set_temp - LOCK_RANGE)
                    {
                        show_actual_temp = show_set_temp;
                        temp_bar = show_actual_temp * 0.4;
                    }
                }
                // 华氏温度处理
                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    show_actual_temp = (sFWG2_t.Direct_handle_position == IN_POSSITION &&
                                        sFWG2_t.general_parameter.fwg2_sleep_state == SLEEP_OPEN) ?
                                       sFWG2_t.Direct_handle_parameter.actual_temp * 9 / 5 + 32 : base_temp_c * 9 / 5 + 32;
                    int mcu_temp_f = sFWG2_t.general_parameter.mcu_temp * 9 / 5 + 32;

                    if (show_actual_temp < mcu_temp_f)
                    {
                        show_actual_temp = mcu_temp_f;
                    }

                    temp_bar = show_actual_temp * 0.21;
                    show_set_temp = sFWG2_t.Direct_handle_parameter.set_temp_f_display;

                    // 显示锁定检查
                    if (sFWG2_t.general_parameter.display_lock_state == LOCK &&
                            show_actual_temp < sFWG2_t.Direct_handle_parameter.set_temp_f_display + LOCK_RANGE_F &&
                            show_actual_temp > sFWG2_t.Direct_handle_parameter.set_temp_f_display - LOCK_RANGE_F)
                    {
                        show_actual_temp = show_set_temp;
                        temp_bar = show_actual_temp * 0.21;
                    }
                }
            }

            if (sFWG2_t.general_parameter.work_mode == NORMAL)
            {
                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE ||
                        sFWG2_t.Direct_handle_work_mode == QUICK_MODE  ||
                        sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
                {
                    first_in_cold_mode = false;

                    // 处理设置温度变化
                    if ((sFWG2_t.Direct_handle_parameter.last_set_temp != sFWG2_t.Direct_handle_parameter.set_temp) ||
                            last_direct_handle_set_temp_f_display != sFWG2_t.Direct_handle_parameter.set_temp_f_display)
                    {
                        sFWG2_t.Direct_handle_parameter.last_set_temp = sFWG2_t.Direct_handle_parameter.set_temp;
                        last_direct_handle_set_temp_f_display = sFWG2_t.Direct_handle_parameter.set_temp_f_display;

                        if (!show_direct_set_temp_flag)
                        {
                            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_TEMP), DWIN_DATA_BITS, 1);
                            sdwin.send_data(&sdwin, SHOW_CURVE_TEMP_VALUE_COLOUR, DWIN_DATA_BITS, ORANGE_COLOR);
                            show_direct_set_temp_flag = true;
                        }

                        show_direct_set_temp_time = 0;
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CURVE_TEMP), DWIN_DATA_BITS, show_set_temp);
                    }

                    // 处理设置温度显示超时
                    if (show_direct_set_temp_flag && ++show_direct_set_temp_time >= SHOW_DIRECT_HANDLE_SET_TEMP_TIME)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_TEMP), DWIN_DATA_BITS, 0);
                        sdwin.send_data(&sdwin, SHOW_CURVE_TEMP_VALUE_COLOUR, DWIN_DATA_BITS, WHITE_COLOR);
                        last_direct_handle_current_temp = 0;
                        direct_temp_refirsh_time = 1;
                        show_direct_set_temp_time = 0;
                        show_direct_set_temp_flag = false;
                    }
                    // 显示当前温度
                    else if (!show_direct_set_temp_flag && --direct_temp_refirsh_time == 0)
                    {
                        direct_temp_refirsh_time = DIRECT_TEMP_REFIRSH_TIME;

                        if (last_direct_handle_current_temp != sFWG2_t.Direct_handle_parameter.actual_temp ||
                                last_direct_handle_current_temp_f_display != sFWG2_t.Direct_handle_parameter.actual_temp_f_display)
                        {
                            last_direct_handle_current_temp = sFWG2_t.Direct_handle_parameter.actual_temp;
                            last_direct_handle_current_temp_f_display = sFWG2_t.Direct_handle_parameter.actual_temp_f_display;
                            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CURVE_TEMP), DWIN_DATA_BITS, show_actual_temp);
                        }
                    }
                }
            }

#endif
            state++;
            break;

        case 1:
#if 1
            if (sFWG2_t.general_parameter.work_mode == NORMAL)
            {
                if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE || sFWG2_t.Direct_handle_work_mode == QUICK_MODE)
                {
                    // 处理风速显示逻辑
                    if (!show_direct_set_wind_flag)
                    {
                        uint16_t wind_value = sFWG2_t.Direct_handle_parameter.set_wind;
                        bool should_send_wind = false;

                        if ((sFWG2_t.Direct_handle_state == HANDLE_WORKING && sFWG2_t.Direct_handle_position == NOT_IN_POSSITION) ||
                                sFWG2_t.general_parameter.fwg2_sleep_state == SLEEP_CLOSE ||
                                sFWG2_t.Direct_handle_state == HANDLE_SLEEP)
                        {
                            should_send_wind = true;
                        }
                        else if (sFWG2_t.Direct_handle_state == HANDLE_WORKING && sFWG2_t.Direct_handle_position == IN_POSSITION)
                        {
                            wind_value = (sFWG2_t.Direct_handle_parameter.actual_temp < 65) ?
                                         sFWG2_t.Direct_handle_parameter.set_wind :
                                         sFWG2_t.Direct_handle_parameter.stop_set_wind;
                            should_send_wind = true;
                        }

                        if (should_send_wind)
                        {
                            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CURVE_WIND), DWIN_DATA_BITS, wind_value);
                            direct_wind_buff[13] =  wind_value;
                        }
                    }

                    // 处理风速变化
                    if (sFWG2_t.Direct_handle_parameter.last_set_wind != sFWG2_t.Direct_handle_parameter.set_wind)
                    {
                        sFWG2_t.Direct_handle_parameter.last_set_wind = sFWG2_t.Direct_handle_parameter.set_wind;

                        if (!show_direct_set_wind_flag)
                        {
                            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_WIND), DWIN_DATA_BITS, 1);
                            sdwin.send_data(&sdwin, SHOW_CURVE_WIND_VALUE_COLOUR, DWIN_DATA_BITS, BLUE_COLOR);
                            show_direct_set_wind_flag = true;
                        }

                        show_direct_set_wind_time = 0;
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CURVE_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.Direct_handle_parameter.set_wind);
                    }

                    // 处理显示标志超时
                    if (show_direct_set_wind_flag && ++show_direct_set_wind_time >= SHOW_DIRECT_HANDLE_SET_WIND_TIME)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_WIND), DWIN_DATA_BITS, 0);
                        sdwin.send_data(&sdwin, SHOW_CURVE_WIND_VALUE_COLOUR, DWIN_DATA_BITS, WHITE_COLOR);
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CURVE_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.Direct_handle_parameter.set_wind);
                        show_direct_set_wind_time = 0;
                        show_direct_set_wind_flag = false;
                    }
                }
                else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_WIND), DWIN_DATA_BITS, 1);
                    sdwin.send_data(&sdwin, SHOW_CURVE_WIND_VALUE_COLOUR, DWIN_DATA_BITS, BLUE_COLOR);
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CURVE_WIND), DWIN_DATA_BITS,
                                    sFWG2_t.Direct_handle_parameter.cold_mode_set_wind);
                    direct_wind_buff[13] =  sFWG2_t.Direct_handle_parameter.cold_mode_set_wind;
                    exit_cold_mode_flage = true;
                }
            }

            // 退出冷风模式处理
            if (sFWG2_t.Direct_handle_work_mode != COLD_WIND_MODE && exit_cold_mode_flage)
            {
                exit_cold_mode_flage = false;
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_WIND), DWIN_DATA_BITS, 0);
                sdwin.send_data(&sdwin, SHOW_CURVE_WIND_VALUE_COLOUR, DWIN_DATA_BITS, WHITE_COLOR);
            }

            if (sFWG2_t.Direct_handle_parameter.actual_wind <= 250)
            {
                direct_wind_buff[13] =  0;
            }

            usart_sendData(DWIN_USART, direct_wind_buff, 14);
#endif
            state++;
            break;

        case 2:
#if 1

            /* show channel select state */
            if (sFWG2_t.general_parameter.ch == 1)
            {
                if (sFWG2_t.general_parameter.ch1_set_temp != sFWG2_t.Direct_handle_parameter.set_temp ||
                        sFWG2_t.general_parameter.ch1_set_wind != sFWG2_t.Direct_handle_parameter.set_wind ||
                        sFWG2_t.general_parameter.ch1_set_time != sFWG2_t.general_parameter.countdown_time)
                {
                    /* show not select channel */
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + CHANNEL_STATE), DWIN_DATA_BITS, 0);
                }
            }
            else if (sFWG2_t.general_parameter.ch == 2)
            {
                if (sFWG2_t.general_parameter.ch2_set_temp != sFWG2_t.Direct_handle_parameter.set_temp ||
                        sFWG2_t.general_parameter.ch2_set_wind != sFWG2_t.Direct_handle_parameter.set_wind ||
                        sFWG2_t.general_parameter.ch2_set_time != sFWG2_t.general_parameter.countdown_time)
                {
                    /* show not select channel */
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + CHANNEL_STATE), DWIN_DATA_BITS,
                                    0);
                }
            }
            else if (sFWG2_t.general_parameter.ch == 3)
            {
                if (sFWG2_t.general_parameter.ch3_set_temp != sFWG2_t.Direct_handle_parameter.set_temp ||
                        sFWG2_t.general_parameter.ch3_set_wind != sFWG2_t.Direct_handle_parameter.set_wind ||
                        sFWG2_t.general_parameter.ch3_set_time != sFWG2_t.general_parameter.countdown_time)
                {
                    /* show not select channel */
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + CHANNEL_STATE), DWIN_DATA_BITS,
                                    0);
                }
            }
            else if (sFWG2_t.general_parameter.ch == 4)
            {
                if (sFWG2_t.general_parameter.ch4_set_temp != sFWG2_t.Direct_handle_parameter.set_temp ||
                        sFWG2_t.general_parameter.ch4_set_wind != sFWG2_t.Direct_handle_parameter.set_wind ||
                        sFWG2_t.general_parameter.ch4_set_time != sFWG2_t.general_parameter.countdown_time)
                {
                    /* show not select channel */
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + CHANNEL_STATE), DWIN_DATA_BITS,
                                    0);
                }
            }

#endif
            state++;
            break;

        case 3:
#if 1

            /* show countdown time */
            if (sFWG2_t.general_parameter.countdown_flag == true)
            {
                sdwin.send_data(&sdwin, SHOW_CURVE_TIME_VALUE_COLOUR, DWIN_DATA_BITS,
                                GREEN_COLOR);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CURVE_TIME), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.countdown_time_display);
            }
            else
            {
                sdwin.send_data(&sdwin, SHOW_CURVE_TIME_VALUE_COLOUR, DWIN_DATA_BITS,
                                WHITE_COLOR);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CURVE_TIME), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.countdown_time);
            }

#endif
            state ++;
            break;

        case 4:
#if 1
            /* send direct handle's temp curve */
            direct_temp_buff[12] = sFWG2_t.Direct_handle_parameter.actual_temp >> 8;
            direct_temp_buff[13] = sFWG2_t.Direct_handle_parameter.actual_temp;
            usart_sendData(DWIN_USART, direct_temp_buff, 14);
#endif
            state = 0;
            break;
        }
    }
    else
    {
        first_in = false;
    }
}

/*
 *   case 0:show channel's select state
 *   case 1:show channel 1 value
 *   case 2:show channel 2 value
 *   case 3:show channel 3 value
 *   case 4:show channel 4 value
 *   case 5:show speaker state
 *   case 6:show enhance mode set val
 *   case 7:show code mode pre set val
 *   case 8:show code mode code1 set val
 *   case 9:show code mode code2 set val
 *   case 10:show enhance mode set value
 *   case 11:show countdown time set val
 *   case 12:show temp unit time select state
 *   case 13:
 */
void Page_General_Heartbeat_Packet(void)
{
    static uint8_t state = 0;
    static fwg2_speaker_state_e last_speaker_state = SPEAKER_CLOSE;
    static fwg2_page_e last_page = 0xff;

    if (last_page != sFWG2_t.general_parameter.fwg2_page || sFWG2_t.general_parameter.fwg2_page == PAGE_SET_WORK_MODE
            || sFWG2_t.general_parameter.fwg2_page == PAGE_SET_CODE_MODE)
    {
        switch (state)
        {
        case 0:
#if 0
#endif
            state++;
            break;

        case 1:
#if 1
            if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
            {
                /* show ch1 set val */
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH1_SET_TEMP), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.ch1_set_temp); // lost data
            }
            else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
            {
                /* show ch1 set val */
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH1_SET_TEMP), DWIN_DATA_BITS,
                                9 * sFWG2_t.general_parameter.ch1_set_temp / 5 + 32); // lost data
            }

            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH1_SET_WIND), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.ch1_set_wind);
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH1_SET_TIME), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.ch1_set_time);
#endif
            state++;
            break;

        case 2:
#if 1
            if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
            {
                /* show ch2 set val */
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH2_SET_TEMP), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.ch2_set_temp);
            }
            else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
            {
                /* show ch2 set val */
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH2_SET_TEMP), DWIN_DATA_BITS,
                                9 * sFWG2_t.general_parameter.ch2_set_temp / 5 + 32);
            }

            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH2_SET_WIND), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.ch2_set_wind);
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH2_SET_TIME), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.ch2_set_time);
#endif
            state++;
            break;

        case 3:
#if 1
            if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
            {
                /* show ch3 set val */
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH3_SET_TEMP), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.ch3_set_temp);
            }
            else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
            {
                /* show ch3 set val */
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH3_SET_TEMP), DWIN_DATA_BITS,
                                9 * sFWG2_t.general_parameter.ch3_set_temp / 5 + 32);
            }

            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH3_SET_WIND), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.ch3_set_wind);
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH3_SET_TIME), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.ch3_set_time);
#endif
            state++;
            break;

        case 4:
#if 1
            if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
            {
                /* show ch4 set val */
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH4_SET_TEMP), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.ch4_set_temp); // lost data
            }
            else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
            {
                /* show ch4 set val */
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH4_SET_TEMP), DWIN_DATA_BITS,
                                9 * sFWG2_t.general_parameter.ch4_set_temp / 5 + 32); // lost data
            }

            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH4_SET_WIND), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.ch4_set_wind);
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH4_SET_TIME), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.ch4_set_time);
#endif
            state++;
            break;

        case 5:
#if 1
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_SPEAKER_FUNCTION), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.speak_state);
#endif
            state++;
            break;

        case 6:
            if (sFWG2_t.general_parameter.code_ch == 0)
            {
                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_PRE_TEMP), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.code0_pre_temp);
                }
                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_PRE_TEMP), DWIN_DATA_BITS,
                                    9 * sFWG2_t.general_parameter.code0_pre_temp / 5 + 32);
                }

                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_PRE_WIND), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code0_pre_wind);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_PRE_TIME), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code0_pre_time);
            }

            if (sFWG2_t.general_parameter.code_ch == 1)
            {
                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_PRE_TEMP), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.code1_pre_temp);
                }
                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_PRE_TEMP), DWIN_DATA_BITS,
                                    9 * sFWG2_t.general_parameter.code1_pre_temp / 5 + 32);
                }

                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_PRE_WIND), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code1_pre_wind);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_PRE_TIME), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code1_pre_time);
            }

            if (sFWG2_t.general_parameter.code_ch == 2)
            {
                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_PRE_TEMP), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.code2_pre_temp);
                }
                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_PRE_TEMP), DWIN_DATA_BITS,
                                    9 * sFWG2_t.general_parameter.code2_pre_temp / 5 + 32);
                }

                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_PRE_WIND), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code2_pre_wind);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_PRE_TIME), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code2_pre_time);
            }

            if (sFWG2_t.general_parameter.code_ch == 3)
            {
                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_PRE_TEMP), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.code3_pre_temp);
                }
                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_PRE_TEMP), DWIN_DATA_BITS,
                                    9 * sFWG2_t.general_parameter.code3_pre_temp / 5 + 32);
                }

                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_PRE_WIND), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code3_pre_wind);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_PRE_TIME), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code3_pre_time);
            }

            state++;
            break;

        case 7:
            if (sFWG2_t.general_parameter.code_ch == 0)
            {
                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP1), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.code0_temp_1);
                }
                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP1), DWIN_DATA_BITS,
                                    9 * sFWG2_t.general_parameter.code0_temp_1 / 5 + 32);
                }

                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_WIND1), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code0_wind_1);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TIME1), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code0_time_1);
            }
            else if (sFWG2_t.general_parameter.code_ch == 1)
            {
                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP1), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.code1_temp_1);
                }
                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP1), DWIN_DATA_BITS,
                                    9 * sFWG2_t.general_parameter.code1_temp_1 / 5 + 32);
                }

                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_WIND1), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code1_wind_1);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TIME1), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code1_time_1);
            }
            else if (sFWG2_t.general_parameter.code_ch == 2)
            {
                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP1), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.code2_temp_1);
                }
                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP1), DWIN_DATA_BITS,
                                    9 * sFWG2_t.general_parameter.code2_temp_1 / 5 + 32);
                }

                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_WIND1), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code2_wind_1);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TIME1), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code2_time_1);
            }
            else if (sFWG2_t.general_parameter.code_ch == 3)
            {
                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP1), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.code3_temp_1);
                }
                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP1), DWIN_DATA_BITS,
                                    9 * sFWG2_t.general_parameter.code3_temp_1 / 5 + 32);
                }

                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_WIND1), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code3_wind_1);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TIME1), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code3_time_1);
            }

            state++;
            break;

        case 8:
            if (sFWG2_t.general_parameter.code_ch == 0)
            {
                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP2), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.code0_temp_2);
                }
                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP2), DWIN_DATA_BITS,
                                    9 * sFWG2_t.general_parameter.code0_temp_2 / 5 + 32);
                }

                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_WIND2), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code0_wind_2);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TIME2), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code0_time_2);
            }
            else if (sFWG2_t.general_parameter.code_ch == 1)
            {
                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP2), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.code1_temp_2);
                }
                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP2), DWIN_DATA_BITS,
                                    9 * sFWG2_t.general_parameter.code1_temp_2 / 5 + 32);
                }

                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_WIND2), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code1_wind_2);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TIME2), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code1_time_2);
            }
            else if (sFWG2_t.general_parameter.code_ch == 2)
            {
                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP2), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.code2_temp_2);
                }
                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP2), DWIN_DATA_BITS,
                                    9 * sFWG2_t.general_parameter.code2_temp_2 / 5 + 32);
                }

                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_WIND2), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code2_wind_2);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TIME2), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code2_time_2);
            }
            else if (sFWG2_t.general_parameter.code_ch == 3)
            {
                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP2), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.code3_temp_2);
                }
                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP2), DWIN_DATA_BITS,
                                    9 * sFWG2_t.general_parameter.code3_temp_2 / 5 + 32);
                }

                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_WIND2), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code3_wind_2);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TIME2), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code3_time_2);
            }

            state++;
            break;

        case 9:
            if (sFWG2_t.general_parameter.code_ch == 0)
            {
                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP3), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.code0_temp_3);
                }
                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP3), DWIN_DATA_BITS,
                                    9 * sFWG2_t.general_parameter.code0_temp_3 / 5 + 32);
                }

                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_WIND3), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code0_wind_3);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TIME3), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code0_time_3);
            }
            else if (sFWG2_t.general_parameter.code_ch == 1)
            {
                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP3), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.code1_temp_3);
                }
                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP3), DWIN_DATA_BITS,
                                    9 * sFWG2_t.general_parameter.code1_temp_3 / 5 + 32);
                }

                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_WIND3), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code1_wind_3);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TIME3), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code1_time_3);
            }
            else if (sFWG2_t.general_parameter.code_ch == 2)
            {
                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP3), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.code2_temp_3);
                }
                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP3), DWIN_DATA_BITS,
                                    9 * sFWG2_t.general_parameter.code2_temp_3 / 5 + 32);
                }

                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_WIND3), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code2_wind_3);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TIME3), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code2_time_3);
            }
            else if (sFWG2_t.general_parameter.code_ch == 3)
            {
                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP3), DWIN_DATA_BITS,
                                    sFWG2_t.general_parameter.code3_temp_3);
                }
                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TEMP3), DWIN_DATA_BITS,
                                    9 * sFWG2_t.general_parameter.code3_temp_3 / 5 + 32);
                }

                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_WIND3), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code3_wind_3);
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_WORK_TIME3), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code3_time_3);
            }

            state++;
            break;

        case 10:

            /* show enhance mode set value */
            if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
            {
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_ENHANCE_TEMP), DWIN_DATA_BITS,
                                sFWG2_t.Direct_handle_parameter.quick_work_temp);
            }
            else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
            {
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_ENHANCE_TEMP), DWIN_DATA_BITS,
                                9 * sFWG2_t.Direct_handle_parameter.quick_work_temp / 5 + 32);
            }

            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_ENHANCE_COUNTDOWN_TIME), DWIN_DATA_BITS,
                            sFWG2_t.Direct_handle_parameter.quick_work_time);
            state++;
            break;

        case 11:
            if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
            {
                /* lost data,no reason */
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH1_SET_TEMP), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.ch1_set_temp); // send again
            }
            else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
            {
                /* lost data,no reason */
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH1_SET_TEMP), DWIN_DATA_BITS,
                                9 * sFWG2_t.general_parameter.ch1_set_temp / 5 + 32); // send again
            }

            state++;
            break;

        case 12:
            if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
            {
                /* lost data,no reason */
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH4_SET_TEMP), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.ch4_set_temp); // send again
            }
            else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
            {
                /* lost data,no reason */
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CH4_SET_TEMP), DWIN_DATA_BITS,
                                9 * sFWG2_t.general_parameter.ch4_set_temp / 5 + 32); // send again
            }

            state++;
            break;

        case 13:
            /* show temp unit select */
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_TEMP_UNITS), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.temp_uint);
            state++;
            break;

        case 14:
            state++;
            break;

        case 15:
            /* show enhance function state */
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + EN_HANCE_FUNCTION), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.enhance_state);
            last_page = sFWG2_t.general_parameter.fwg2_page;
            state = 0;
            break;
        }
    }
}

/*
*   case 0:show work mode select
*   case 1:show temp unit select
*   case 2:show speaker mode select
*   case 3:show display lock mode select
*   case 4:show fn key mode select
*   case 5:show speaker state

*   case 6:show countdown time
*   case 7:show handle touch select
*   case 8:show uart function select / show sleep state
*   case 9:show software version
*   case 10:show hardware version
*   case 11:show ota function select
*/
void Page_Set_Heartbeat_Packet(void)
{
    static uint8_t state = 0;
    static bool first_in = false;
    static bool first_open = false;

    if (sFWG2_t.general_parameter.fwg2_page == PAGE_MENU_1 ||
            sFWG2_t.general_parameter.fwg2_page == PAGE_MENU_2)
    {
        switch (state)
        {
        case 0:

            /* show work mode select */
            if (sFWG2_t.general_parameter.work_mode == CODE)
            {
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + FWG2_WORK_MODE), DWIN_DATA_BITS,
                                2);
            }
            else if (sFWG2_t.general_parameter.work_mode == NORMAL && sFWG2_t.general_parameter.enhance_state == ENHANCE_OPEN)
            {
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + FWG2_WORK_MODE), DWIN_DATA_BITS,
                                0);
            }
            else if (sFWG2_t.general_parameter.work_mode == NORMAL && sFWG2_t.general_parameter.enhance_state == ENHANCE_CLOSE)
            {
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + FWG2_WORK_MODE), DWIN_DATA_BITS,
                                1);
            }

            state++;
            break;

        case 1:
            /* show temp unit select */
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_TEMP_UNITS), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.temp_uint);
            state++;
            break;

        case 2:
            /* show speaker mode select */
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_SPEAKER_FUNCTION), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.speak_state);
            state++;
            break;

        case 3:
            /* show display lock mode select */
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_DISPLAY_LOCK_MODE), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.display_lock_state);
            state++;
            break;

        case 4:
            /* show fn key long mode select */
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_FN_KEY_LONG_MODE), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.fn_key_long_set);
            /* show fn key mode short select */
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_FN_KEY_SHORT_MODE), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.fn_key_short_set);
            /* show adjust key select */
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_ADJUST_KEY), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.adjust_key_set);
            state++;
            break;

        case 5:
            if (first_in == false)
            {
                first_in = true;

                /* show direct handle set temp */
                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_DIRECT_CALIBRATE_TEMP), DWIN_DATA_BITS,
                                    sFWG2_t.Direct_handle_parameter.set_temp);
                }
                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_DIRECT_CALIBRATE_TEMP), DWIN_DATA_BITS,
                                    sFWG2_t.Direct_handle_parameter.set_temp_f_display);
                }
            }

            state++;
            break;

        case 6:
            /* show countdown time */
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_TIME), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.countdown_time);
            state++;
            break;

        case 7:
            /* show handle touch select */
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_TOUCH_FUNCTION), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.touch_key_set);
            state++;
            break;

        case 8:
            /* show uart function select */
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_UART_FUNCTION), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.uart_state);
            /* show uart function select */
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_SLEEP_MODE), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.fwg2_sleep_state);
            state++;
            break;

        case 9:
            /* show software version  */
            software_buff[8] = 1;
            software_buff[7] = 2;
            software_buff[6] = 2;
            usart_sendData(DWIN_USART, software_buff, 9);
            state++;
            break;

        case 10:
            /* show hardware version  */
            hardtware_buff[8] = 0;
            hardtware_buff[7] = 3;
            hardtware_buff[6] = 1;
            usart_sendData(DWIN_USART, hardtware_buff, 9);
            state++;
            break;

        case 11:
            /* show ota function select */
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SET_UART_FUNCTION), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.ota_state);
            state = 0;
            break;
        }
    }
    else
    {
        first_in = false;

        if (first_open == false)
        {
            /* show software version  */
            software_buff[8] = 1;
            software_buff[7] = 2;
            software_buff[6] = 2;
            usart_sendData(DWIN_USART, software_buff, 9);
            first_open = true;
        }
    }
}

/*
 *   case 0:show current temp curve
 *   case 1:show set wind curve
 *   case 2:show countdown time
 *   case 3:show step run state
 */
void Page_Code_Heartbeat_Packet(void)
{
    static uint8_t state = 0;
    static uint8_t show_temp_time = 0;
    static uint8_t show_wind_time = 0;
    static uint16_t actual_temp = 0;

    if (sFWG2_t.general_parameter.fwg2_page == PAGE_SHOW_CODE_WORK)
    {
        switch (state)
        {
        case 0:
#if 1
            actual_temp = sFWG2_t.Direct_handle_parameter.actual_temp +
                          sFWG2_t.Direct_handle_parameter.linear_calibration_temp -
                          sFWG2_t.Direct_handle_parameter.set_calibration_temp;
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + START_STOP_CODE_MODE), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.code_mode_state);

            if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
            {
                /* send direct handle's temp curve */
                direct_temp_buff[12] = actual_temp >> 8;
                direct_temp_buff[13] = actual_temp;
                usart_sendData(DWIN_USART, direct_temp_buff, 14);
                show_temp_time++;

                if (show_temp_time % 12 == 0)
                {
                    if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_TEMP), DWIN_DATA_BITS,
                                        sFWG2_t.Direct_handle_parameter.actual_temp_f_display);
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_TEMP), DWIN_DATA_BITS,
                                        sFWG2_t.Direct_handle_parameter.actual_temp + sFWG2_t.Direct_handle_parameter.linear_calibration_temp);
                    }
                }
            }

#endif
            state++;
            break;

        case 1:
#if 1
            if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_START)
            {
                if (sFWG2_t.general_parameter.code_ch == 0)
                {
                    if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.code0_pre_wind);
                        /* send direct handle's wind curve */
                        direct_wind_buff[13] = sFWG2_t.general_parameter.code0_pre_wind;
                        usart_sendData(DWIN_USART, direct_wind_buff, 14);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.code0_wind_1);
                        /* send direct handle's wind curve */
                        direct_wind_buff[13] = sFWG2_t.general_parameter.code0_wind_1;
                        usart_sendData(DWIN_USART, direct_wind_buff, 14);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.code0_wind_2);
                        /* send direct handle's wind curve */
                        direct_wind_buff[13] = sFWG2_t.general_parameter.code0_wind_2;
                        usart_sendData(DWIN_USART, direct_wind_buff, 14);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.code0_wind_3);
                        /* send direct handle's wind curve */
                        direct_wind_buff[13] = sFWG2_t.general_parameter.code0_wind_3;
                        usart_sendData(DWIN_USART, direct_wind_buff, 14);
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 1)
                {
                    if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.code1_pre_wind);
                        /* send direct handle's wind curve */
                        direct_wind_buff[13] = sFWG2_t.general_parameter.code1_pre_wind;
                        usart_sendData(DWIN_USART, direct_wind_buff, 14);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.code1_wind_1);
                        /* send direct handle's wind curve */
                        direct_wind_buff[13] = sFWG2_t.general_parameter.code1_wind_1;
                        usart_sendData(DWIN_USART, direct_wind_buff, 14);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.code1_wind_2);
                        /* send direct handle's wind curve */
                        direct_wind_buff[13] = sFWG2_t.general_parameter.code1_wind_2;
                        usart_sendData(DWIN_USART, direct_wind_buff, 14);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.code1_wind_3);
                        /* send direct handle's wind curve */
                        direct_wind_buff[13] = sFWG2_t.general_parameter.code1_wind_3;
                        usart_sendData(DWIN_USART, direct_wind_buff, 14);
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 2)
                {
                    if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.code2_pre_wind);
                        /* send direct handle's wind curve */
                        direct_wind_buff[13] = sFWG2_t.general_parameter.code2_pre_wind;
                        usart_sendData(DWIN_USART, direct_wind_buff, 14);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.code2_wind_1);
                        /* send direct handle's wind curve */
                        direct_wind_buff[13] = sFWG2_t.general_parameter.code2_wind_1;
                        usart_sendData(DWIN_USART, direct_wind_buff, 14);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.code2_wind_2);
                        /* send direct handle's wind curve */
                        direct_wind_buff[13] = sFWG2_t.general_parameter.code2_wind_2;
                        usart_sendData(DWIN_USART, direct_wind_buff, 14);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.code2_wind_3);
                        /* send direct handle's wind curve */
                        direct_wind_buff[13] = sFWG2_t.general_parameter.code2_wind_3;
                        usart_sendData(DWIN_USART, direct_wind_buff, 14);
                    }
                }
                else if (sFWG2_t.general_parameter.code_ch == 3)
                {
                    if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.code3_pre_wind);
                        /* send direct handle's wind curve */
                        direct_wind_buff[13] = sFWG2_t.general_parameter.code3_pre_wind;
                        usart_sendData(DWIN_USART, direct_wind_buff, 14);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.code3_wind_1);
                        /* send direct handle's wind curve */
                        direct_wind_buff[13] = sFWG2_t.general_parameter.code3_wind_1;
                        usart_sendData(DWIN_USART, direct_wind_buff, 14);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.code3_wind_2);
                        /* send direct handle's wind curve */
                        direct_wind_buff[13] = sFWG2_t.general_parameter.code3_wind_2;
                        usart_sendData(DWIN_USART, direct_wind_buff, 14);
                    }
                    else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_WIND), DWIN_DATA_BITS,
                                        sFWG2_t.general_parameter.code3_wind_3);
                        /* send direct handle's wind curve */
                        direct_wind_buff[13] = sFWG2_t.general_parameter.code3_wind_3;
                        usart_sendData(DWIN_USART, direct_wind_buff, 14);
                    }
                }
            }
            else if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_STOP)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                {
                    show_wind_time++;

                    if (sFWG2_t.Direct_handle_parameter.actual_temp > 70)
                    {
                        if (show_wind_time % 12 == 0)
                        {
                            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_WIND), DWIN_DATA_BITS,
                                            sFWG2_t.Direct_handle_parameter.stop_set_wind);
                        }

                        /* send direct handle's stop wind curve */
                        direct_wind_buff[13] = sFWG2_t.Direct_handle_parameter.stop_set_wind;
                        usart_sendData(DWIN_USART, direct_wind_buff, 14);
                    }
                    else
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_WIND), DWIN_DATA_BITS,
                                        0);
                        /* send direct handle's stop wind curve */
                        direct_wind_buff[13] = 0;
                        usart_sendData(DWIN_USART, direct_wind_buff, 14);
                    }
                }
            }

#endif
            state++;
            break;

        case 2:
#if 1
            if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_START)
            {
                sdwin.send_data(&sdwin, SHOW_CODE_TIME_COLOUR, DWIN_DATA_BITS,
                                GREEN_COLOR);
                /* show countdown time */
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_COUNTDOWN_TIME), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.code_mode_countdown_time_display);
            }
            else if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_STOP)
            {
                sdwin.send_data(&sdwin, SHOW_CODE_TIME_COLOUR, DWIN_DATA_BITS,
                                WHITE_COLOR);
                /* show countdown time */
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_MODE_COUNTDOWN_TIME), DWIN_DATA_BITS,
                                0);
            }

#endif
            state++;
            break;

        case 3:
#if 1
            if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_START)
            {
                /* show step run state */
                if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
                {
                    /* show select channel */
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_STEP), DWIN_DATA_BITS,
                                    1);
                }
                else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
                {
                    /* show select channel */
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_STEP), DWIN_DATA_BITS,
                                    2);
                }
                else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
                {
                    /* show select channel */
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_STEP), DWIN_DATA_BITS,
                                    3);
                }
                else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
                {
                    /* show select channel */
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_STEP), DWIN_DATA_BITS,
                                    4);
                }
            }
            else if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_STOP)
            {
                /* show select channel */
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_CODE_STEP), DWIN_DATA_BITS,
                                0);
            }

#endif
            state = 0;
            break;
        }
    }
}

void Page_Switch(void)
{
    static bool first_in = false;
    static bool first_in_cal = false;
    static fwg2_enhance_state_e last_fwg2_enhance_state;
    static handle_work_mode_e last_handle_work_mode;
    static uint16_t time = 0;

    if (first_in == false)
    {
        first_in = true;

        /* jump to work page */
        if (sFWG2_t.general_parameter.work_mode == NORMAL)
        {
            /* jump to work page 1*/
            page_switch[9] = PAGE_MAIN;
            usart_sendData(DWIN_USART, page_switch, 10);
            sFWG2_t.general_parameter.fwg2_page = PAGE_MAIN;
        }
        else if (sFWG2_t.general_parameter.work_mode == CODE)
        {
            /* jump to work page 28*/
            page_switch[9] = PAGE_SHOW_CODE_WORK;
            usart_sendData(DWIN_USART, page_switch, 10);
            sFWG2_t.general_parameter.fwg2_page = PAGE_SHOW_CODE_WORK;
        }
    }
    else
    {
        /* page switch */
        if (sFWG2_t.general_parameter.work_mode == NORMAL)
        {
            if (sFWG2_t.general_parameter.fwg2_page == 0x28)
            {
                /* only show direct work page  */
                page_switch[9] = PAGE_MAIN;
                usart_sendData(DWIN_USART, page_switch, 10);
                sFWG2_t.general_parameter.fwg2_page = PAGE_MAIN;
            }
        }
        else if (sFWG2_t.general_parameter.work_mode == CODE)
        {
            if (sFWG2_t.general_parameter.fwg2_page == 0x28)
            {
                /* jump to work page 28*/
                page_switch[9] = PAGE_SHOW_CODE_WORK;
                usart_sendData(DWIN_USART, page_switch, 10);
                sFWG2_t.general_parameter.fwg2_page = PAGE_SHOW_CODE_WORK;
            }
        }

        /* show reset mes */
        if (sFWG2_t.general_parameter.reset_fwg2_flag == true)
        {
            time++;
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + RESET_FWG2_MES), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.reset_fwg2_flag);

            if (time >= MES_SHOW_TIME)
            {
                time = 0;
                sFWG2_t.general_parameter.reset_fwg2_flag = false;
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + RESET_FWG2_MES), DWIN_DATA_BITS,
                                sFWG2_t.general_parameter.reset_fwg2_flag);
            }
        }

        /* show channel save mes */
        if (show_popup_flag == true)
        {
            time++;
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SAVE_CHANNEL_MES), DWIN_DATA_BITS,
                            show_popup_flag);

            if (time >= MES_SHOW_TIME)
            {
                time = 0;
                show_popup_flag = false;
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SAVE_CHANNEL_MES), DWIN_DATA_BITS,
                                show_popup_flag);
            }
        }
    }

    if (sFWG2_t.general_parameter.fwg2_page == PAGE_SET_CAL_TEMP)
    {
        if ((sFWG2_t.Direct_handle_work_mode == QUICK_MODE || sFWG2_t.general_parameter.enhance_state == ENHANCE_OPEN)
                && first_in_cal == false)
        {
            first_in_cal = true;
            last_fwg2_enhance_state = sFWG2_t.general_parameter.enhance_state;
            last_handle_work_mode = sFWG2_t.Direct_handle_work_mode;
            sFWG2_t.Direct_handle_work_mode = NORMAL_MODE;
            sFWG2_t.general_parameter.enhance_state = ENHANCE_CLOSE;
        }
    }

    if (sFWG2_t.general_parameter.fwg2_page != PAGE_SET_CAL_TEMP && first_in_cal == true)
    {
        first_in_cal = false;
        sFWG2_t.Direct_handle_work_mode = last_handle_work_mode;
        sFWG2_t.general_parameter.enhance_state = last_fwg2_enhance_state;
    }
}


void DwinInitialization(DwinObjectType *dwin)
{
    if (dwin == NULL)
    {
        return;
    }

    dwin->checkMode = DwinNone;
    dwin->send_data = WriteDataToLCD;
    dwin->recv_data = RecvDataFromLCD;

    for (int i = 0; i < rxbuff_length; i++)
    {
        dwin->tx_buff[i] = 0x00;
        dwin->rx_buff[i] = 0x00;
    }

    dwin->read_size = 0;
}

/*********** (C) COPYRIGHT 1999-2019 Moonan Technology *********END OF FILE****/

void show_temp_in_page_work(show_state_t *state)
{
    switch (state->show_temp)
    {
    case SHOW_SET_TEMP:
        /* change display */
        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_TEMP), DWIN_DATA_BITS,
                        1);
        /* change color */
        sdwin.send_data(&sdwin, SHOW_DIRECT_TEMP_COLOUR, DWIN_DATA_BITS,
                        ORANGE_COLOR);

        /* show direct handle set temp */
        if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
        {
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP), DWIN_DATA_BITS,
                            sFWG2_t.Direct_handle_parameter.set_temp);
        }
        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
        {
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP), DWIN_DATA_BITS,
                            sFWG2_t.Direct_handle_parameter.set_temp_f_display);
        }

    case SHOW_ACTUAL_TEMP:
        /* change display */
        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_TEMP), DWIN_DATA_BITS,
                        0);
        /* change color */
        sdwin.send_data(&sdwin, SHOW_DIRECT_TEMP_COLOUR, DWIN_DATA_BITS, WHITE_COLOR);

        if (sFWG2_t.general_parameter.display_lock_state == UNLOCK)
        {
            if (sFWG2_t.Direct_handle_parameter.set_temp < 500)
            {
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP_BAR), DWIN_DATA_BITS,
                                sFWG2_t.Direct_handle_parameter.actual_temp * 0.4);
            }
            else
            {
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP_BAR), DWIN_DATA_BITS,
                                200);
            }

            /* show direct handle current temp */
            if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
            {
                if (sFWG2_t.general_parameter.enhance_state == ENHANCE_OPEN)
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP), DWIN_DATA_BITS,
                                    (sFWG2_t.Direct_handle_parameter.actual_temp - sFWG2_t.Direct_handle_parameter.set_calibration_temp - ENHANCE_TEMP));
                }
                else
                {
                    sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP), DWIN_DATA_BITS,
                                    (sFWG2_t.Direct_handle_parameter.actual_temp - sFWG2_t.Direct_handle_parameter.set_calibration_temp));
                }
            }
            else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
            {
                sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP), DWIN_DATA_BITS,
                                sFWG2_t.Direct_handle_parameter.actual_temp_f_display);
            }
        }
        else if (sFWG2_t.general_parameter.display_lock_state == LOCK)
        {
            if (sFWG2_t.general_parameter.enhance_state == ENHANCE_OPEN)
            {
                if ((sFWG2_t.Direct_handle_parameter.actual_temp - sFWG2_t.Direct_handle_parameter.set_calibration_temp - ENHANCE_TEMP)
                        <
                        (sFWG2_t.Direct_handle_parameter.set_temp + LOCK_RANGE) &&
                        (sFWG2_t.Direct_handle_parameter.actual_temp - sFWG2_t.Direct_handle_parameter.set_calibration_temp - ENHANCE_TEMP) >
                        (sFWG2_t.Direct_handle_parameter.set_temp - LOCK_RANGE))
                {
                    /* show direct handle set temp */
                    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP), DWIN_DATA_BITS,
                                        sFWG2_t.Direct_handle_parameter.set_temp);
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP), DWIN_DATA_BITS,
                                        sFWG2_t.Direct_handle_parameter.set_temp_f_display);
                    }

                    /* show direct handle set temp bar */
                    if (sFWG2_t.Direct_handle_parameter.set_temp < 500)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP_BAR), DWIN_DATA_BITS,
                                        sFWG2_t.Direct_handle_parameter.set_temp * 0.4);
                    }
                    else
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP_BAR), DWIN_DATA_BITS,
                                        200);
                    }
                }
                else
                {
                    /* show direct handle current temp */
                    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP), DWIN_DATA_BITS,
                                        sFWG2_t.Direct_handle_parameter.actual_temp);
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP), DWIN_DATA_BITS,
                                        sFWG2_t.Direct_handle_parameter.actual_temp_f_display);
                    }

                    if (sFWG2_t.Direct_handle_parameter.set_temp < 500)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP_BAR), DWIN_DATA_BITS,
                                        sFWG2_t.Direct_handle_parameter.actual_temp * 0.4);
                    }
                    else
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP_BAR), DWIN_DATA_BITS,
                                        200);
                    }
                }
            }
            else if (sFWG2_t.general_parameter.enhance_state == ENHANCE_CLOSE)
            {
                if ((sFWG2_t.Direct_handle_parameter.actual_temp - sFWG2_t.Direct_handle_parameter.set_calibration_temp) <
                        (sFWG2_t.Direct_handle_parameter.set_temp + LOCK_RANGE) &&
                        (sFWG2_t.Direct_handle_parameter.actual_temp - sFWG2_t.Direct_handle_parameter.set_calibration_temp) >
                        (sFWG2_t.Direct_handle_parameter.set_temp - LOCK_RANGE))
                {
                    /* show direct handle set temp */
                    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP), DWIN_DATA_BITS,
                                        sFWG2_t.Direct_handle_parameter.set_temp);
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP), DWIN_DATA_BITS,
                                        sFWG2_t.Direct_handle_parameter.set_temp_f_display);
                    }

                    if (sFWG2_t.Direct_handle_parameter.set_temp < 500)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP_BAR), DWIN_DATA_BITS,
                                        sFWG2_t.Direct_handle_parameter.set_temp * 0.4);
                    }
                    else
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP_BAR), DWIN_DATA_BITS,
                                        200);
                    }
                }
                else
                {
                    /* show direct handle current temp */
                    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP), DWIN_DATA_BITS,
                                        sFWG2_t.Direct_handle_parameter.actual_temp);
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP), DWIN_DATA_BITS,
                                        sFWG2_t.Direct_handle_parameter.actual_temp_f_display);
                    }

                    if (sFWG2_t.Direct_handle_parameter.set_temp < 500)
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP_BAR), DWIN_DATA_BITS,
                                        sFWG2_t.Direct_handle_parameter.actual_temp * 0.4);
                    }
                    else
                    {
                        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + SHOW_DIRECT_CURRENT_TEMP_BAR), DWIN_DATA_BITS,
                                        200);
                    }
                }
            }
        }

        break;

    default:
        break;
    }
}






/*********** (C) COPYRIGHT 1999-2019 Moonan Technology *********END OF FILE****/