#include "UART4_comm_handle.h"
#include "dwin_handle.h"
#include "FWG2_handle.h"
#include "beep_handle.h"
UART4_DATA_t UART4_data;
uart4_event_e uart4_event = UART4_END_EVENT;

static float change_temp = 0;
static float change_temp_f_display = 0;
static float change_wind = 0;

void uart4_event_handle(void);
static void RecvDataFromUART4(UART4_DATA_t *uart4);
static void WriteDataToUART4(UART4_DATA_t *uart4,
                             uint16_t cmd_1, uint16_t cmd_2,
                             uint16_t id, uint16_t data_len,
                             uint16_t data_1, uint16_t data_2,
                             uint16_t data_3, uint16_t data_4,
                             uint16_t data_5);

void uart4_comm_handle(void)
{
    RecvDataFromUART4(&UART4_data);
    uart4_event_handle();
}

void RecvDataFromUART4(UART4_DATA_t *uart4)
{
    uint32_t crc_value;
    uart4->read_size = usart_receiveData(UART4_USART, uart4->rx_buff);

    if (uart4->read_size == UART4_MAX_RECV_SIZE)
    {

        if (UART4_CHECK_HEAD(uart4->rx_buff[UART4_HEAD1], uart4->rx_buff[UART4_HEAD2]))
        {
            convert_data(uart4->rx_buff, uart4->check_crc_buff, UART4_CMD1, UART4_DATA5_LEN_L);
            crc_value = crc_block_calculate(uart4->check_crc_buff, UART4_CRC_SIZE);
            crc_data_reset();

            if (uart4->rx_buff[UART4_CRC32_1] == ((crc_value >> 24) & 0xff) &&
                uart4->rx_buff[UART4_CRC32_2] == ((crc_value >> 16) & 0xff) &&
                uart4->rx_buff[UART4_CRC32_3] == ((crc_value >> 8) & 0xff) &&
                uart4->rx_buff[UART4_CRC32_4] == (crc_value & 0xff))
            {
                /* app cmd */
                if (uart4->rx_buff[UART4_CMD1] == UART4_GENERAL)
                {

                    if (uart4->rx_buff[UART4_CMD2] == UART4_GENERAL_WRITE)
                    {
                        /* 433 cmd */
                        if (uart4->rx_buff[UART4_ID_L] == 0x07)
                        {
                            /* get channel 1 data */
                            if (uart4->rx_buff[UART4_DATA4_LEN_L] == 0x01)
                            {
                                uart4_event = UART4_GET_CHANNEL_1_EVENT;
                            }
                            /* get channel 2 data */
                            else if (uart4->rx_buff[UART4_DATA4_LEN_L] == 0x02)
                            {
                                uart4_event = UART4_GET_CHANNEL_2_EVENT;
                            }
                            /* get channel 3 data */
                            else if (uart4->rx_buff[UART4_DATA4_LEN_L] == 0x03)
                            {
                                uart4_event = UART4_GET_CHANNEL_3_EVENT;
                            }
                            /* get channel 4 data */
                            else if (uart4->rx_buff[UART4_DATA4_LEN_L] == 0x04)
                            {
                                uart4_event = UART4_GET_CHANNEL_4_EVENT;
                            }
                            /*  add set temp  */
                            else if (uart4->rx_buff[UART4_DATA4_LEN_L] == 0x05)
                            {
                                uart4_event = UART4_SET_TEMP_ADD_EVENT;
                            }
                            /*  reduce set temp  */
                            else if (uart4->rx_buff[UART4_DATA4_LEN_L] == 0x06)
                            {
                                uart4_event = UART4_SET_TEMP_REDUCE_EVENT;
                            }
                            /*  add set wind  */
                            else if (uart4->rx_buff[UART4_DATA4_LEN_L] == 0x07)
                            {
                                uart4_event = UART4_SET_WIND_ADD_EVENT;
                            }
                            /* reduce set wind  */
                            else if (uart4->rx_buff[UART4_DATA4_LEN_L] == 0x08)
                            {
                                uart4_event = UART4_SET_WIND_REDUCE_EVENT;
                            }
                            /* select handle */
                            else if (uart4->rx_buff[UART4_DATA4_LEN_L] == 0x09)
                            {
                                uart4_event = UART4_SELECT_HANDLE_EVENT;
                            }
                        }
                        /* speaker cmd */
                        else if (uart4->rx_buff[UART4_ID_L] == 0x06)
                        {
                            if (uart4->rx_buff[UART4_DATA3_LEN_H] == 0x01 &&
                                uart4->rx_buff[UART4_DATA3_LEN_L] == 0x01)
                            {
                                /* temp add  */
                                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                                {
                                    change_temp = uart4->rx_buff[UART4_DATA4_LEN_L];
                                    change_temp_f_display = 9 * change_temp / 5 + 32;
                                }
                                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                                {
                                    change_temp_f_display = uart4->rx_buff[UART4_DATA4_LEN_L];
                                    change_temp = (change_temp_f_display - 32) * 5 / 9;
                                }
                                uart4_event = UART4_SET_TEMP_ADD_EVENT;
                            }

                            else if (uart4->rx_buff[UART4_DATA3_LEN_H] == 0x01 &&
                                     uart4->rx_buff[UART4_DATA3_LEN_L] == 0x02)
                            {
                                /* temp recuce */
                                if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                                {
                                    change_temp = (uart4->rx_buff[UART4_DATA4_LEN_L]);
                                    change_temp_f_display = (9 * change_temp / 5 + 32);
                                }
                                else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                                {
                                    change_temp_f_display = (uart4->rx_buff[UART4_DATA4_LEN_L]);
                                    change_temp = ((change_temp_f_display - 32) * 5 / 9);
                                }
                                uart4_event = UART4_SET_TEMP_REDUCE_EVENT;
                            }
                            else if (uart4->rx_buff[UART4_DATA3_LEN_H] == 0x01 &&
                                     uart4->rx_buff[UART4_DATA3_LEN_L] == 0x00)
                            {
                                /* temp change with value*/
                                change_temp = uart4->rx_buff[UART4_DATA4_LEN_H] * 256 +
                                              uart4->rx_buff[UART4_DATA4_LEN_L];

                                change_temp_f_display = uart4->rx_buff[UART4_DATA4_LEN_H] * 256 +
                                                        uart4->rx_buff[UART4_DATA4_LEN_L];
								
								uart4_event = UART4_SET_TEMP_EVENT;
								
                            }
                            else if (uart4->rx_buff[UART4_DATA3_LEN_H] == 0x02 &&
                                     uart4->rx_buff[UART4_DATA3_LEN_L] == 0x01)
                            {
                                /* wind add  */
                                change_wind = (uart4->rx_buff[UART4_DATA4_LEN_L]);
                                uart4_event = UART4_SET_WIND_ADD_EVENT;
                            }

                            else if (uart4->rx_buff[UART4_DATA3_LEN_H] == 0x02 &&
                                     uart4->rx_buff[UART4_DATA3_LEN_L] == 0x02)
                            {
                                /* wind recuce */
                                change_wind = (uart4->rx_buff[UART4_DATA4_LEN_L]);
                                uart4_event = UART4_SET_WIND_REDUCE_EVENT;
                            }
                            else if (uart4->rx_buff[UART4_DATA3_LEN_H] == 0x02 &&
                                     uart4->rx_buff[UART4_DATA3_LEN_L] == 0x00)
                            {
                                /* wind change with value*/
                                change_wind = uart4->rx_buff[UART4_DATA4_LEN_H] * 256 +
                                              uart4->rx_buff[UART4_DATA4_LEN_L];
                                uart4_event = UART4_SET_WIND_EVENT;
                            }

                            else if (uart4->rx_buff[UART4_DATA3_LEN_H] == 0x00 &&
                                     uart4->rx_buff[UART4_DATA4_LEN_L] == 0x01)
                            {
                                /* get channel 1 value */
                                uart4_event = UART4_GET_CHANNEL_1_EVENT;
                            }
                            else if (uart4->rx_buff[UART4_DATA3_LEN_H] == 0x00 &&
                                     uart4->rx_buff[UART4_DATA4_LEN_L] == 0x02)
                            {
                                /* get channel 2 value */
                                uart4_event = UART4_GET_CHANNEL_2_EVENT;
                            }
                            else if (uart4->rx_buff[UART4_DATA3_LEN_H] == 0x00 &&
                                     uart4->rx_buff[UART4_DATA4_LEN_L] == 0x03)
                            {
                                /* get channel 3 value */
                                uart4_event = UART4_GET_CHANNEL_3_EVENT;
                            }
                            else if (uart4->rx_buff[UART4_DATA3_LEN_H] == 0x00 &&
                                     uart4->rx_buff[UART4_DATA4_LEN_L] == 0x04)
                            {
                                /* get channel 4 value */
                                uart4_event = UART4_GET_CHANNEL_4_EVENT;
                            }
                            else if (uart4->rx_buff[UART4_DATA3_LEN_H] == 0x00 &&
                                     uart4->rx_buff[UART4_DATA4_LEN_L] == 0x05)
                            {
                                /* set mode to enhance mode  */
                                uart4_event = UART4_ENTER_ENHANCE_MODE_EVENT;
                            }
                            else if (uart4->rx_buff[UART4_DATA3_LEN_H] == 0x00 &&
                                     uart4->rx_buff[UART4_DATA4_LEN_L] == 0x06)
                            {
                                /* set mode to cold wind mode  */
                                uart4_event = UART4_ENTER_COLD_MODE_EVENT;
                            }
                            else if (uart4->rx_buff[UART4_DATA3_LEN_H] == 0x00 &&
                                     uart4->rx_buff[UART4_DATA4_LEN_L] == 0x07)
                            {
                                /* set mode to normal mode  */
                                uart4_event = UART4_ENTER_NORMAL_MODE_EVENT;
                            }
                            else if (uart4->rx_buff[UART4_DATA3_LEN_H] == 0x00 &&
                                     uart4->rx_buff[UART4_DATA4_LEN_L] == 0x08)
                            {
                                /* set page to curve */
                                uart4_event = UART4_SHOW_CURVE_PAGE_EVENT;
                            }
                            else if (uart4->rx_buff[UART4_DATA3_LEN_H] == 0x00 &&
                                     uart4->rx_buff[UART4_DATA4_LEN_L] == 0x09)
                            {
                                /* set page to value */
                                uart4_event = UART4_SHOW_NORMAL_PAGE_EVENT;
                            }
                            else if (uart4->rx_buff[UART4_DATA3_LEN_H] == 0x00 &&
                                     uart4->rx_buff[UART4_DATA4_LEN_L] == 0x0A)
                            {
                                /* set temp unit to celsius */
                                uart4_event = UART4_SET_TEMP_UNIT_F;
                            }
                            else if (uart4->rx_buff[UART4_DATA3_LEN_H] == 0x00 &&
                                     uart4->rx_buff[UART4_DATA4_LEN_L] == 0x0B)
                            {
                                /* set temp unit to fahrenheit */
                                uart4_event = UART4_SET_TEMP_UNIT_C;
                            }
                            else if (uart4->rx_buff[UART4_DATA3_LEN_H] == 0x00 &&
                                     uart4->rx_buff[UART4_DATA4_LEN_L] == 0x0C)
                            {
                                /* switch select handle */
                                uart4_event = UART4_SELECT_HANDLE_EVENT;
                            }
                        }
                    }
                }

                /* factory cmd */
                if (uart4->rx_buff[UART4_CMD1] == 0x01)
                {
                    if (uart4->rx_buff[UART4_CMD2] == 0x00)
                    {
                        uart4_event = UART4_FACTORY_EVENT;
                    }
                }
                /* other cmd */
                else
                {
                    __NOP();
                }
            }
        }

        uart4->read_size = 0;
    }
}

static void WriteDataToUART4(UART4_DATA_t *uart4,
                             uint16_t cmd_1, uint16_t cmd_2,
                             uint16_t id, uint16_t data_len,
                             uint16_t data_1, uint16_t data_2,
                             uint16_t data_3, uint16_t data_4,
                             uint16_t data_5)
{
    static uint32_t crc_value;
    uart4->tx_buff[UART4_HEAD1] = UART4_HEAD_1;
    uart4->tx_buff[UART4_CMD1] = cmd_1;
    uart4->tx_buff[UART4_CMD2] = cmd_2;
    uart4->tx_buff[UART4_ID_H] = (uint8_t)((id >> 8) & 0xff);
    uart4->tx_buff[UART4_ID_L] = (uint8_t)((id & 0XFF));
    uart4->tx_buff[UART4_DATA_LEN_H] = (uint8_t)((data_len >> 8) & 0xff);
    uart4->tx_buff[UART4_DATA_LEN_L] = (uint8_t)((data_len & 0XFF));
    uart4->tx_buff[UART4_DATA1_LEN_H] = (uint8_t)((data_1 >> 8) & 0xff);
    uart4->tx_buff[UART4_DATA1_LEN_L] = (uint8_t)((data_1 & 0XFF));
    uart4->tx_buff[UART4_DATA2_LEN_H] = (uint8_t)((data_2 >> 8) & 0xff);
    uart4->tx_buff[UART4_DATA2_LEN_L] = (uint8_t)((data_2 & 0XFF));
    uart4->tx_buff[UART4_DATA3_LEN_H] = (uint8_t)((data_3 >> 8) & 0xff);
    uart4->tx_buff[UART4_DATA3_LEN_L] = (uint8_t)((data_3 & 0XFF));
    uart4->tx_buff[UART4_DATA4_LEN_H] = (uint8_t)((data_4 >> 8) & 0xff);
    uart4->tx_buff[UART4_DATA4_LEN_L] = (uint8_t)((data_4 & 0XFF));
    uart4->tx_buff[UART4_DATA5_LEN_H] = (uint8_t)((data_5 >> 8) & 0xff);
    uart4->tx_buff[UART4_DATA5_LEN_L] = (uint8_t)((data_5 & 0XFF));
    /* crc check */
    convert_data(uart4->tx_buff, uart4->check_crc_buff, UART4_CMD1, UART4_DATA5_LEN_L);
    crc_value = crc_block_calculate(uart4->check_crc_buff, UART4_CRC_SIZE);
    crc_data_reset();
    uart4->tx_buff[UART4_CRC32_1] = ((crc_value >> 24) & 0xff);
    uart4->tx_buff[UART4_CRC32_2] = ((crc_value >> 16) & 0xff);
    uart4->tx_buff[UART4_CRC32_3] = ((crc_value >> 8) & 0xff);
    uart4->tx_buff[UART4_CRC32_4] = (crc_value & 0xff);
    uart4->tx_buff[UART4_HEAD2] = UART4_HEAD_2;
    /* send data */
    usart_sendData(UART4_USART, uart4->tx_buff, UART4_MAX_SEND_SIZE);
}

void uart4_event_handle(void)
{
    switch (uart4_event)
    {
    case UART4_SET_TEMP_ADD_EVENT:
        sFWG2_t.general_parameter.key_setting_flag = true;
        sFWG2_t.general_parameter.setting_time = 0;
        sFWG2_t.general_parameter.setting_temp_flag = true;
        sFWG2_t.general_parameter.setting_temp_time = 0;
        sFWG2_t.Direct_handle_parameter.last_set_temp = 0;

        if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
        {
            sFWG2_t.Direct_handle_parameter.set_temp += change_temp;
            if (sFWG2_t.Direct_handle_parameter.set_temp >= MAX_SET_TEMP_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
                sFWG2_t.Direct_handle_parameter.set_temp = MAX_SET_TEMP_VAL;
            }

            sFWG2_t.Direct_handle_parameter.set_temp_f_display = 9 * sFWG2_t.Direct_handle_parameter.set_temp / 5 + 32;
        }
        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
        {
            sFWG2_t.Direct_handle_parameter.set_temp_f_display += change_temp_f_display;
            if (sFWG2_t.Direct_handle_parameter.set_temp_f_display >= MAX_SET_TEMP_F_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = MAX_SET_TEMP_F_VAL;
            }

            sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
        }

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

        sbeep.status = BEEP_SHORT;
        uart4_event = UART4_END_EVENT;
        break;

    case UART4_SET_TEMP_REDUCE_EVENT:
        sFWG2_t.general_parameter.key_setting_flag = true;
        sFWG2_t.general_parameter.setting_time = 0;
        sFWG2_t.general_parameter.setting_temp_flag = true;
        sFWG2_t.general_parameter.setting_temp_time = 0;
        sFWG2_t.Direct_handle_parameter.last_set_temp = 0;

        if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
        {
            sFWG2_t.Direct_handle_parameter.set_temp -= change_temp;
            if (sFWG2_t.Direct_handle_parameter.set_temp <= MIN_SET_TEMP_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
                sFWG2_t.Direct_handle_parameter.set_temp = MIN_SET_TEMP_VAL;
            }
            sFWG2_t.Direct_handle_parameter.set_temp_f_display = 9 * sFWG2_t.Direct_handle_parameter.set_temp / 5 + 32;
        }
        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
        {
            sFWG2_t.Direct_handle_parameter.set_temp_f_display -= change_temp_f_display;

            if ((sFWG2_t.Direct_handle_parameter.set_temp_f_display) <= MIN_SET_TEMP_F_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = MIN_SET_TEMP_F_VAL;
            }

            sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
        }

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

        sbeep.status = BEEP_SHORT;
        uart4_event = UART4_END_EVENT;
        break;
    case UART4_SET_TEMP_EVENT:
        sFWG2_t.general_parameter.key_setting_flag = true;
        sFWG2_t.general_parameter.setting_time = 0;
        sFWG2_t.general_parameter.setting_temp_flag = true;
        sFWG2_t.general_parameter.setting_temp_time = 0;
        sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
        if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
        {
            sFWG2_t.Direct_handle_parameter.set_temp = change_temp;
            if (sFWG2_t.Direct_handle_parameter.set_temp <= MIN_SET_TEMP_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
                sFWG2_t.Direct_handle_parameter.set_temp = MIN_SET_TEMP_VAL;
            }
            else if (sFWG2_t.Direct_handle_parameter.set_temp >= MAX_SET_TEMP_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
                sFWG2_t.Direct_handle_parameter.set_temp = MAX_SET_TEMP_VAL;
            }
            sFWG2_t.Direct_handle_parameter.set_temp_f_display = 9 * sFWG2_t.Direct_handle_parameter.set_temp / 5 + 32;
        }
        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
        {
            sFWG2_t.Direct_handle_parameter.set_temp_f_display = change_temp_f_display;
            if (sFWG2_t.Direct_handle_parameter.set_temp_f_display <= MIN_SET_TEMP_F_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = MIN_SET_TEMP_F_VAL;
            }
            else if (sFWG2_t.Direct_handle_parameter.set_temp_f_display >= MAX_SET_TEMP_F_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = MAX_SET_TEMP_F_VAL;
            }
            sFWG2_t.Direct_handle_parameter.set_temp = (sFWG2_t.Direct_handle_parameter.set_temp_f_display - 32) * 5 / 9;
        }
		sbeep.status = BEEP_SHORT;
		uart4_event = UART4_END_EVENT;
        break;

    case UART4_SET_WIND_ADD_EVENT:
        sFWG2_t.general_parameter.key_setting_flag = true;
        sFWG2_t.general_parameter.setting_time = 0;
        sFWG2_t.general_parameter.setting_wind_flag = true;
        sFWG2_t.general_parameter.setting_wind_time = 0;
        sFWG2_t.Direct_handle_parameter.last_set_wind = 0;

        if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
        {
			sFWG2_t.Direct_handle_parameter.set_wind+=change_wind;
            if ((sFWG2_t.Direct_handle_parameter.set_wind) >= MAX_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
                sFWG2_t.Direct_handle_parameter.set_wind = MAX_SET_WIND_VAL;
            }
             
        }
        else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
        {
			sFWG2_t.Direct_handle_parameter.cold_mode_set_wind += change_wind;
            if ((sFWG2_t.Direct_handle_parameter.cold_mode_set_wind ) >= MAX_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = MAX_SET_WIND_VAL;
            }
             
        }

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

        sbeep.status = BEEP_SHORT;
        uart4_event = UART4_END_EVENT;
        break;

    case UART4_SET_WIND_REDUCE_EVENT:
        sFWG2_t.general_parameter.key_setting_flag = true;
        sFWG2_t.general_parameter.setting_time = 0;
        sFWG2_t.general_parameter.setting_wind_flag = true;
        sFWG2_t.general_parameter.setting_wind_time = 0;
        sFWG2_t.Direct_handle_parameter.last_set_wind = 0;

        if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
        {
			sFWG2_t.Direct_handle_parameter.set_wind -= change_wind;
            if ((sFWG2_t.Direct_handle_parameter.set_wind ) <= MIN_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
                sFWG2_t.Direct_handle_parameter.set_wind = MIN_SET_WIND_VAL;
            }
             
        }
        else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
        {
			sFWG2_t.Direct_handle_parameter.cold_mode_set_wind -= change_wind;
            if ((sFWG2_t.Direct_handle_parameter.cold_mode_set_wind ) <= MIN_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = MIN_SET_WIND_VAL;
            }
             
        }

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

        sbeep.status = BEEP_SHORT;
        uart4_event = UART4_END_EVENT;
        break;

    case UART4_SET_WIND_EVENT:
        sFWG2_t.general_parameter.key_setting_flag = true;
        sFWG2_t.general_parameter.setting_time = 0;
        sFWG2_t.general_parameter.setting_wind_flag = true;
        sFWG2_t.general_parameter.setting_wind_time = 0;
        sFWG2_t.Direct_handle_parameter.last_set_wind = 0;

        if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
        {
            sFWG2_t.Direct_handle_parameter.set_wind = change_wind;
            if ((sFWG2_t.Direct_handle_parameter.set_wind) <= MIN_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
                sFWG2_t.Direct_handle_parameter.set_wind = MIN_SET_WIND_VAL;
            }
            else if ((sFWG2_t.Direct_handle_parameter.set_wind) >= MAX_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
                sFWG2_t.Direct_handle_parameter.set_wind = MAX_SET_WIND_VAL;
            }
        }
        else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
        {
            sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = change_wind;
            if (sFWG2_t.Direct_handle_parameter.cold_mode_set_wind <= MIN_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = MIN_SET_WIND_VAL;
            }
            else if (sFWG2_t.Direct_handle_parameter.cold_mode_set_wind >= MAX_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = MAX_SET_WIND_VAL;
            }
        }

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

        sbeep.status = BEEP_SHORT;
        uart4_event = UART4_END_EVENT;
        break;

    case UART4_GET_CHANNEL_1_EVENT:
        sFWG2_t.general_parameter.key_setting_flag = true;
        sFWG2_t.general_parameter.setting_time = 0;
        sFWG2_t.general_parameter.setting_temp_flag = true;
        sFWG2_t.general_parameter.setting_temp_time = 0;
        sFWG2_t.general_parameter.setting_wind_flag = true;
        sFWG2_t.general_parameter.setting_wind_time = 0;

        if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
        {
            sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
            sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
            sFWG2_t.general_parameter.ch = 1;

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
            sFWG2_t.general_parameter.countdown_time = sFWG2_t.general_parameter.ch1_set_time;
            sFWG2_t.general_parameter.countdown_flag = false;
            /* show select channel */
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + CHANNEL_STATE), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.ch);
        }

        sbeep.status = BEEP_SHORT;
        uart4_event = UART4_END_EVENT;
        break;

    case UART4_GET_CHANNEL_2_EVENT:
        sFWG2_t.general_parameter.key_setting_flag = true;
        sFWG2_t.general_parameter.setting_time = 0;
        sFWG2_t.general_parameter.setting_temp_flag = true;
        sFWG2_t.general_parameter.setting_temp_time = 0;
        sFWG2_t.general_parameter.setting_wind_flag = true;
        sFWG2_t.general_parameter.setting_wind_time = 0;

        if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
        {
            sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
            sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
            sFWG2_t.general_parameter.ch = 2;

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
            sFWG2_t.general_parameter.countdown_time = sFWG2_t.general_parameter.ch2_set_time;
            sFWG2_t.general_parameter.countdown_flag = false;
            /* show select channel */
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + CHANNEL_STATE), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.ch);
        }

        sbeep.status = BEEP_SHORT;
        uart4_event = UART4_END_EVENT;
        break;

    case UART4_GET_CHANNEL_3_EVENT:
        sFWG2_t.general_parameter.key_setting_flag = true;
        sFWG2_t.general_parameter.setting_time = 0;
        sFWG2_t.general_parameter.setting_temp_flag = true;
        sFWG2_t.general_parameter.setting_temp_time = 0;
        sFWG2_t.general_parameter.setting_wind_flag = true;
        sFWG2_t.general_parameter.setting_wind_time = 0;

        if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
        {
            sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
            sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
            sFWG2_t.general_parameter.ch = 3;

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
            sFWG2_t.general_parameter.countdown_time = sFWG2_t.general_parameter.ch3_set_time;
            sFWG2_t.general_parameter.countdown_flag = false;
            /* show select channel */
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + CHANNEL_STATE), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.ch);
        }

        sbeep.status = BEEP_SHORT;
        uart4_event = UART4_END_EVENT;
        break;

    case UART4_GET_CHANNEL_4_EVENT:
        sFWG2_t.general_parameter.key_setting_flag = true;
        sFWG2_t.general_parameter.setting_time = 0;
        sFWG2_t.general_parameter.setting_temp_flag = true;
        sFWG2_t.general_parameter.setting_temp_time = 0;
        sFWG2_t.general_parameter.setting_wind_flag = true;
        sFWG2_t.general_parameter.setting_wind_time = 0;

        if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
        {
            sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
            sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
            sFWG2_t.general_parameter.ch = 4;

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
            sFWG2_t.general_parameter.countdown_time = sFWG2_t.general_parameter.ch4_set_time;
            sFWG2_t.general_parameter.countdown_flag = false;
            /* show select channel */
            sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + CHANNEL_STATE), DWIN_DATA_BITS,
                            sFWG2_t.general_parameter.ch);
        }

        sbeep.status = BEEP_SHORT;
        uart4_event = UART4_END_EVENT;
        break;

    case UART4_ENTER_ENHANCE_MODE_EVENT:
        /* enter normal mode */
	    sFWG2_t.general_parameter.enhance_state = ENHANCE_OPEN;
        //sFWG2_t.Direct_handle_work_mode = QUICK_MODE;
        sbeep.status = BEEP_LONG;
        uart4_event = UART4_END_EVENT;
        break;

    case UART4_ENTER_NORMAL_MODE_EVENT:
        /* enter normal mode */
        sFWG2_t.Direct_handle_work_mode = NORMAL_MODE;
	    sFWG2_t.general_parameter.enhance_state = ENHANCE_CLOSE;
	    
        sbeep.status = BEEP_LONG;
        uart4_event = UART4_END_EVENT;
        break;

    case UART4_ENTER_COLD_MODE_EVENT:
        /* enter cold mode */
        sFWG2_t.Direct_handle_work_mode = COLD_WIND_MODE;
        sbeep.status = BEEP_LONG;
        uart4_event = UART4_END_EVENT;
        break;

    case UART4_SHOW_CURVE_PAGE_EVENT:
        page_switch[9] = PAGE_DIRECT_CURVE;
        usart_sendData(DWIN_USART, page_switch, 10);
        sFWG2_t.general_parameter.fwg2_page = PAGE_DIRECT_CURVE;
        sbeep.status = BEEP_SHORT;
        uart4_event = UART4_END_EVENT;
        break;

    case UART4_SHOW_NORMAL_PAGE_EVENT:
        page_switch[9] = PAGE_MAIN;
        usart_sendData(DWIN_USART, page_switch, 10);
        sFWG2_t.general_parameter.fwg2_page = PAGE_MAIN;
        sbeep.status = BEEP_SHORT;
        uart4_event = UART4_END_EVENT;
        break;

    case UART4_SET_TEMP_UNIT_C:
        sFWG2_t.general_parameter.temp_uint = CELSIUS;
        sbeep.status = BEEP_SHORT;
        uart4_event = UART4_END_EVENT;
        break;

    case UART4_SET_TEMP_UNIT_F:
        sFWG2_t.general_parameter.temp_uint = FAHRENHEIT;
        sbeep.status = BEEP_SHORT;
        uart4_event = UART4_END_EVENT;
        break;

    case UART4_SELECT_HANDLE_EVENT:
        sbeep.status = BEEP_SHORT;
        uart4_event = UART4_END_EVENT;
        break;

    case UART4_FACTORY_EVENT:
        WriteDataToUART4(&UART4_data, 0x01, 0x01, 0x01, 0x0A, 258, 256, 0x00, 0x00, 0x00);
        uart4_event = UART4_END_EVENT;
        break;

    case UART4_END_EVENT:
        break;
    }
}
