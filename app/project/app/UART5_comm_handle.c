#include "UART5_comm_handle.h"
#include "dwin_handle.h"
#include "FWG2_handle.h"
#include "beep_handle.h"
UART5_DATA_t UART5_data;
uart5_event_e uart5_event = UART5_END_EVENT;
void uart5_event_handle(void);
static void RecvDataFromUART5(UART5_DATA_t *uart5);
static void WriteDataToUART5(UART5_DATA_t * uart5,
                             uint16_t cmd_1,   uint16_t cmd_2,
                             uint16_t id,     uint16_t data_len,
                             uint16_t data_1, uint16_t data_2,
                             uint16_t data_3, uint16_t data_4,
                             uint16_t data_5);


void uart5_comm_handle(void)
{
    RecvDataFromUART5(&UART5_data);
    uart5_event_handle();
    __NOP();
}

void RecvDataFromUART5(UART5_DATA_t *uart5)
{
    uint32_t crc_value;
    uart5->read_size = usart_receiveData(UART5_USART, uart5->rx_buff);

    if (uart5->read_size == UART5_MAX_RECV_SIZE)
    {
        if (UART5_CHECK_HEAD(uart5->rx_buff[UART5_HEAD1], uart5->rx_buff[UART5_HEAD2]))
        {
            convert_data(uart5->rx_buff, uart5->check_crc_buff, UART5_CMD1, UART5_DATA5_LEN_L);
            crc_value = crc_block_calculate(uart5->check_crc_buff, UART5_CRC_SIZE);
            crc_data_reset();

            if (uart5->rx_buff[UART5_CRC32_1] == ((crc_value >> 24) & 0xff)  &&
                    uart5->rx_buff[UART5_CRC32_2] == ((crc_value >> 16) & 0xff)  &&
                    uart5->rx_buff[UART5_CRC32_3] == ((crc_value >> 8)  & 0xff)   &&
                    uart5->rx_buff[UART5_CRC32_4] == (crc_value & 0xff))
            {
                /* app cmd */
                if (uart5->rx_buff[UART5_CMD1] == UART5_GENERAL)
                {
                    if (uart5->rx_buff[UART5_CMD2] == UART5_GENERAL_WRITE)
                    {
                        /* 433 cmd */
                        if (uart5->rx_buff[UART5_ID_L] == 0x07)
                        {
                            /* get channel 1 data */
                            if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x01)
                            {
                                uart5_event = UART5_GET_CHANNEL_1_EVENT;
                            }
                            /* get channel 2 data */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x02)
                            {
                                uart5_event = UART5_GET_CHANNEL_2_EVENT;
                            }
                            /* get channel 3 data */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x03)
                            {
                                uart5_event = UART5_GET_CHANNEL_3_EVENT;
                            }
                            /* get channel 4 data */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x04)
                            {
                                uart5_event = UART5_GET_CHANNEL_4_EVENT;
                            }
                            /*  add set temp  */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x05)
                            {
                                uart5_event = UART5_SET_TEMP_ADD_EVENT;
                            }
                            /*  reduce set temp  */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x06)
                            {
                                uart5_event = UART5_SET_TEMP_REDUCE_EVENT;
                            }
                            /*  add set wind  */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x07)
                            {
                                uart5_event = UART5_SET_WIND_ADD_EVENT;
                            }
                            /* reduce set wind  */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x08)
                            {
                                uart5_event = UART5_SET_WIND_REDUCE_EVENT;
                            }
                            /* select handle */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x09)
                            {
                                uart5_event = UART5_SELECT_HANDLE_EVENT;
                            }
                        }
                        /* speaker cmd */
                        else if (uart5->rx_buff[UART5_ID_L] == 0x06)
                        {
                            /* get channel 1 data */
                            if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x01)
                            {
                                uart5_event = UART5_GET_CHANNEL_1_EVENT;
                            }
                            /* get channel 2 data */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x02)
                            {
                                uart5_event = UART5_GET_CHANNEL_2_EVENT;
                            }
                            /* get channel 3 data */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x03)
                            {
                                uart5_event = UART5_GET_CHANNEL_3_EVENT;
                            }
                            /* get channel 4 data */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x04)
                            {
                                uart5_event = UART5_GET_CHANNEL_4_EVENT;
                            }
                            /*  add set temp  */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x05)
                            {
                                uart5_event = UART5_SET_TEMP_ADD_EVENT;
                            }
                            /*  reduce set temp  */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x06)
                            {
                                uart5_event = UART5_SET_TEMP_REDUCE_EVENT;
                            }
                            /*  add set wind  */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x07)
                            {
                                uart5_event = UART5_SET_WIND_ADD_EVENT;
                            }
                            /* reduce set wind  */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x08)
                            {
                                uart5_event = UART5_SET_WIND_REDUCE_EVENT;
                            }
                            /* enter cold mode */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x09)
                            {
                                uart5_event = UART5_ENTER_COLD_MODE_EVENT;
                            }
                            /* enter normal mode */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x0A)
                            {
                                uart5_event = UART5_ENTER_NORMAL_MODE_EVENT;
                            }
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x0B)
                            {
                            }
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x0C)
                            {
                            }
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x0D)
                            {
                            }
                            /* show curve page */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x0E)
                            {
                                uart5_event = UART5_SHOW_CURVE_PAGE_EVENT;
                            }
                            /* show normal page */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x0F)
                            {
                                uart5_event = UART5_SHOW_NORMAL_PAGE_EVENT;
                            }
                            /* set temp unit f */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x10)
                            {
                                uart5_event =  UART5_SET_TEMP_UNIT_F;
                            }
                            /* set temp unit c */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x11)
                            {
                                uart5_event =  UART5_SET_TEMP_UNIT_C;
                            }
                            /* select handle */
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x12)
                            {
                                uart5_event = UART5_SELECT_HANDLE_EVENT;
                            }
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x13)
                            {
                            }
                            else if (uart5->rx_buff[UART5_DATA4_LEN_L] == 0x14)
                            {
                            }
                        }
                    }
                }

                if (uart5->rx_buff[UART5_CMD1] == UART5_GENERAL)
                {
                    if (uart5->rx_buff[UART5_CMD2] == 0x00)
                    {
                        uart5_event = UART5_FACTORY_EVENT;
                    }
                }
                /* other cmd */
                else
                {
                    __NOP();
                }
            }
        }

        uart5->read_size = 0;
    }
}


static void WriteDataToUART5(UART5_DATA_t * uart5,
                             uint16_t cmd_1,   uint16_t cmd_2,
                             uint16_t id,     uint16_t data_len,
                             uint16_t data_1, uint16_t data_2,
                             uint16_t data_3, uint16_t data_4,
                             uint16_t data_5)
{
    static uint32_t crc_value;
    uart5->tx_buff[UART5_HEAD1] = UART5_HEAD_1;
    uart5->tx_buff[UART5_CMD1] = cmd_1;
    uart5->tx_buff[UART5_CMD2] = cmd_2;
    uart5->tx_buff[UART5_ID_H] = (uint8_t)((id >> 8) & 0xff);
    uart5->tx_buff[UART5_ID_L] = (uint8_t)((id & 0XFF));
    uart5->tx_buff[UART5_DATA_LEN_H] = (uint8_t)((data_len >> 8) & 0xff);
    uart5->tx_buff[UART5_DATA_LEN_L] = (uint8_t)((data_len & 0XFF));
    uart5->tx_buff[UART5_DATA1_LEN_H] = (uint8_t)((data_1 >> 8) & 0xff);
    uart5->tx_buff[UART5_DATA1_LEN_L] = (uint8_t)((data_1 & 0XFF));
    uart5->tx_buff[UART5_DATA2_LEN_H] = (uint8_t)((data_2 >> 8) & 0xff);
    uart5->tx_buff[UART5_DATA2_LEN_L] = (uint8_t)((data_2 & 0XFF));
    uart5->tx_buff[UART5_DATA3_LEN_H] = (uint8_t)((data_3 >> 8) & 0xff);
    uart5->tx_buff[UART5_DATA3_LEN_L] = (uint8_t)((data_3 & 0XFF));
    uart5->tx_buff[UART5_DATA4_LEN_H] = (uint8_t)((data_4 >> 8) & 0xff);
    uart5->tx_buff[UART5_DATA4_LEN_L] = (uint8_t)((data_4 & 0XFF));
    uart5->tx_buff[UART5_DATA5_LEN_H] = (uint8_t)((data_5 >> 8) & 0xff);
    uart5->tx_buff[UART5_DATA5_LEN_L] = (uint8_t)((data_5 & 0XFF));
    /* crc check */
    memset(uart5->check_crc_buff, 0, 256);
    convert_data(uart5->tx_buff, uart5->check_crc_buff, UART5_CMD1, UART5_DATA5_LEN_L);
    crc_value = crc_block_calculate(uart5->check_crc_buff, UART5_CRC_SIZE);
    crc_data_reset();
    uart5->tx_buff[UART5_CRC32_1] = ((crc_value >> 24) & 0xff);
    uart5->tx_buff[UART5_CRC32_2] = ((crc_value >> 16) & 0xff);
    uart5->tx_buff[UART5_CRC32_3] = ((crc_value >> 8) & 0xff);
    uart5->tx_buff[UART5_CRC32_4] = (crc_value & 0xff);
    uart5->tx_buff[UART5_HEAD2] = UART5_HEAD_2;
    /* send data */
    usart_sendData(UART5_USART, uart5->tx_buff, UART5_MAX_SEND_SIZE);
}

void uart5_event_handle(void)
{
    switch (uart5_event)
    {
    case UART5_SET_TEMP_ADD_EVENT:
        sFWG2_t.general_parameter.key_setting_flag = true;
        sFWG2_t.general_parameter.setting_time = 0;
        sFWG2_t.general_parameter.setting_temp_flag = true;
        sFWG2_t.general_parameter.setting_temp_time = 0;
        sFWG2_t.Direct_handle_parameter.last_set_temp = 0;

        if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
        {
            if ((sFWG2_t.Direct_handle_parameter.set_temp + 10) >= MAX_SET_TEMP_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
                sFWG2_t.Direct_handle_parameter.set_temp = MAX_SET_TEMP_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_temp += 10;
            }

            sFWG2_t.Direct_handle_parameter.set_temp_f_display  =  9 * sFWG2_t.Direct_handle_parameter.set_temp  / 5  + 32;
        }
        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
        {
            if ((sFWG2_t.Direct_handle_parameter.set_temp_f_display + 50) >= MAX_SET_TEMP_F_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = MAX_SET_TEMP_F_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display += 50;
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
        uart5_event = UART5_END_EVENT;
        break;

    case UART5_SET_TEMP_REDUCE_EVENT:
        sFWG2_t.general_parameter.key_setting_flag = true;
        sFWG2_t.general_parameter.setting_time = 0;
        sFWG2_t.general_parameter.setting_temp_flag = true;
        sFWG2_t.general_parameter.setting_temp_time = 0;
        sFWG2_t.Direct_handle_parameter.last_set_temp = 0;

        if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
        {
            if ((sFWG2_t.Direct_handle_parameter.set_temp - 10) <= MIN_SET_TEMP_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
                sFWG2_t.Direct_handle_parameter.set_temp = MIN_SET_TEMP_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_temp -= 10;
            }

            sFWG2_t.Direct_handle_parameter.set_temp_f_display  =  9 * sFWG2_t.Direct_handle_parameter.set_temp  / 5  + 32;
        }
        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
        {
            if ((sFWG2_t.Direct_handle_parameter.set_temp_f_display - 50) <= MIN_SET_TEMP_F_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_temp = 0;
                sFWG2_t.Direct_handle_parameter.set_temp_f_display = MIN_SET_TEMP_F_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_temp_f_display -= 50;
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
        uart5_event = UART5_END_EVENT;
        break;

    case UART5_SET_WIND_ADD_EVENT:
        sFWG2_t.general_parameter.key_setting_flag = true;
        sFWG2_t.general_parameter.setting_time = 0;
        sFWG2_t.general_parameter.setting_wind_flag = true;
        sFWG2_t.general_parameter.setting_wind_time = 0;
        sFWG2_t.Direct_handle_parameter.last_set_wind = 0;

        if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
        {
            if ((sFWG2_t.Direct_handle_parameter.set_wind + 10) >= MAX_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
                sFWG2_t.Direct_handle_parameter.set_wind = MAX_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_wind += 10;
            }
        }
        else if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
        {
            if ((sFWG2_t.Direct_handle_parameter.cold_mode_set_wind + 10) >= MAX_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = MAX_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind += 10;
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
        uart5_event = UART5_END_EVENT;
        break;

    case UART5_SET_WIND_REDUCE_EVENT:
        sFWG2_t.general_parameter.key_setting_flag = true;
        sFWG2_t.general_parameter.setting_time = 0;
        sFWG2_t.general_parameter.setting_wind_flag = true;
        sFWG2_t.general_parameter.setting_wind_time = 0;
        sFWG2_t.Direct_handle_parameter.last_set_wind = 0;

        if (sFWG2_t.Direct_handle_work_mode  ==  NORMAL_MODE)
        {
            if ((sFWG2_t.Direct_handle_parameter.set_wind - 10) <= MIN_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
                sFWG2_t.Direct_handle_parameter.set_wind = MIN_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.set_wind -= 10;
            }
        }
        else if (sFWG2_t.Direct_handle_work_mode  ==  COLD_WIND_MODE)
        {
            if ((sFWG2_t.Direct_handle_parameter.cold_mode_set_wind - 10) <= MIN_SET_WIND_VAL)
            {
                sFWG2_t.Direct_handle_parameter.last_set_wind = 0;
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind = MIN_SET_WIND_VAL;
            }
            else
            {
                sFWG2_t.Direct_handle_parameter.cold_mode_set_wind -= 10;
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
        uart5_event = UART5_END_EVENT;
        break;

    case UART5_GET_CHANNEL_1_EVENT:
        sFWG2_t.general_parameter.key_setting_flag = true;
        sFWG2_t.general_parameter.setting_time = 0;
        sFWG2_t.general_parameter.setting_temp_flag =  true;
        sFWG2_t.general_parameter.setting_temp_time =  0;
        sFWG2_t.general_parameter.setting_wind_flag =  true;
        sFWG2_t.general_parameter.setting_wind_time =  0;

        if (sFWG2_t.general_parameter.fwg2_page == PAGE_MAIN                 || \
                sFWG2_t.general_parameter.fwg2_page == PAGE_DIRECT_CURVE)
        {
            /* show channel value */
            sFWG2_t.general_parameter.countdown_time  = sFWG2_t.general_parameter.ch1_set_time;
            sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch1_set_temp;
            sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch1_set_wind;
            /* show channel state */
            sFWG2_t.general_parameter.ch = 1;

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

        /* show select channel */
        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + CHANNEL_STATE), DWIN_DATA_BITS,
                        sFWG2_t.general_parameter.ch);
        sbeep.status = BEEP_SHORT;
        uart5_event = UART5_END_EVENT;
        break;

    case UART5_GET_CHANNEL_2_EVENT:
        sFWG2_t.general_parameter.key_setting_flag = true;
        sFWG2_t.general_parameter.setting_time = 0;
        sFWG2_t.general_parameter.setting_temp_flag =  true;
        sFWG2_t.general_parameter.setting_temp_time =  0;
        sFWG2_t.general_parameter.setting_wind_flag =  true;
        sFWG2_t.general_parameter.setting_wind_time =  0;

        if (sFWG2_t.general_parameter.fwg2_page == PAGE_MAIN                 || \
                sFWG2_t.general_parameter.fwg2_page == PAGE_DIRECT_CURVE)
        {
            /* show channel value */
            sFWG2_t.general_parameter.countdown_time  = sFWG2_t.general_parameter.ch2_set_time;
            sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch2_set_temp;
            sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch2_set_wind;
            /* show channel state */
            sFWG2_t.general_parameter.ch = 2;

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

        /* show select channel */
        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + CHANNEL_STATE), DWIN_DATA_BITS,
                        sFWG2_t.general_parameter.ch);
        sbeep.status = BEEP_SHORT;
        uart5_event = UART5_END_EVENT;
        break;

    case UART5_GET_CHANNEL_3_EVENT:
        sFWG2_t.general_parameter.key_setting_flag = true;
        sFWG2_t.general_parameter.setting_time = 0;
        sFWG2_t.general_parameter.setting_temp_flag =  true;
        sFWG2_t.general_parameter.setting_temp_time =  0;
        sFWG2_t.general_parameter.setting_wind_flag =  true;
        sFWG2_t.general_parameter.setting_wind_time =  0;

        if (sFWG2_t.general_parameter.fwg2_page == PAGE_MAIN                 || \
                sFWG2_t.general_parameter.fwg2_page == PAGE_DIRECT_CURVE)
        {
            /* show channel value */
            sFWG2_t.general_parameter.countdown_time  = sFWG2_t.general_parameter.ch3_set_time;
            sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch3_set_temp;
            sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch3_set_wind;
            /* show channel state */
            sFWG2_t.general_parameter.ch = 3;

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

        /* show select channel */
        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + CHANNEL_STATE), DWIN_DATA_BITS,
                        sFWG2_t.general_parameter.ch);
        sbeep.status = BEEP_SHORT;
        uart5_event = UART5_END_EVENT;
        break;

    case UART5_GET_CHANNEL_4_EVENT:
        sFWG2_t.general_parameter.key_setting_flag = true;
        sFWG2_t.general_parameter.setting_time = 0;
        sFWG2_t.general_parameter.setting_temp_flag =  true;
        sFWG2_t.general_parameter.setting_temp_time =  0;
        sFWG2_t.general_parameter.setting_wind_flag =  true;
        sFWG2_t.general_parameter.setting_wind_time =  0;

        if (sFWG2_t.general_parameter.fwg2_page == PAGE_MAIN                 || \
                sFWG2_t.general_parameter.fwg2_page == PAGE_DIRECT_CURVE)
        {
            /* show channel value */
            sFWG2_t.general_parameter.countdown_time  = sFWG2_t.general_parameter.ch4_set_time;
            sFWG2_t.Direct_handle_parameter.set_temp = sFWG2_t.general_parameter.ch4_set_temp;
            sFWG2_t.Direct_handle_parameter.set_wind = sFWG2_t.general_parameter.ch4_set_wind;
            /* show channel state */
            sFWG2_t.general_parameter.ch = 4;

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

        /* show select channel */
        sdwin.send_data(&sdwin, (DWIN_BASE_ADDRESS + CHANNEL_STATE), DWIN_DATA_BITS,
                        sFWG2_t.general_parameter.ch);
        sbeep.status = BEEP_SHORT;
        uart5_event = UART5_END_EVENT;
        break;

    case UART5_ENTER_NORMAL_MODE_EVENT:
        /* enter normal mode */
        sFWG2_t.Direct_handle_work_mode = NORMAL_MODE;
        sbeep.status = BEEP_LONG;
        uart5_event = UART5_END_EVENT;
        break;

    case UART5_ENTER_COLD_MODE_EVENT:
        /* enter cold mode */
        sFWG2_t.Direct_handle_work_mode = COLD_WIND_MODE;
        sbeep.status = BEEP_LONG;
        uart5_event = UART5_END_EVENT;
        break;

    case UART5_SHOW_CURVE_PAGE_EVENT:
        page_switch[9] = PAGE_DIRECT_CURVE;
        usart_sendData(DWIN_USART, page_switch, 10);
        sFWG2_t.general_parameter.fwg2_page = PAGE_DIRECT_CURVE;
        sbeep.status = BEEP_SHORT;
        uart5_event = UART5_END_EVENT;
        break;

    case UART5_SHOW_NORMAL_PAGE_EVENT:
        page_switch[9] = PAGE_MAIN;
        usart_sendData(DWIN_USART, page_switch, 10);
        sFWG2_t.general_parameter.fwg2_page = PAGE_MAIN;
        sbeep.status = BEEP_SHORT;
        uart5_event = UART5_END_EVENT;
        break;

    case UART5_SET_TEMP_UNIT_C:
        sFWG2_t.general_parameter.temp_uint = CELSIUS;
        sbeep.status = BEEP_SHORT;
        uart5_event = UART5_END_EVENT;
        break;

    case UART5_SET_TEMP_UNIT_F:
        sFWG2_t.general_parameter.temp_uint = FAHRENHEIT;
        sbeep.status = BEEP_SHORT;
        uart5_event = UART5_END_EVENT;
        break;

    case UART5_SELECT_HANDLE_EVENT:
        sbeep.status = BEEP_SHORT;
        uart5_event = UART5_END_EVENT;
        break;

    case UART5_FACTORY_EVENT:
        WriteDataToUART5(&UART5_data, 0x01, 0x01, 0x01, 0x0A, 258, 256, 0x00, 0x00, 0x00);
        uart5_event = UART5_END_EVENT;
        break;

    case UART5_END_EVENT:
        break;
    }
}








