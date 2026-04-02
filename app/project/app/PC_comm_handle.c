#include "PC_comm_handle.h"
#include "iap.h"
#include "iap_usart.h"
#include "dwin_handle.h"
#include "FWG2_handle.h"
PC_DATA_t pc_data;

pc_event_e pc_event = PC_END_EVENT;

static void RecvDataFromPC(PC_DATA_t *pc);
static void WriteDataToPC(PC_DATA_t * pc,
                          uint16_t cmd_1,  uint16_t cmd_2,
                          uint16_t id,     uint16_t data_len,
                          uint16_t data_1, uint16_t data_2,
                          uint16_t data_3, uint16_t data_4,
                          uint16_t data_5);
void pc_event_handle(void);
void send_heartbeat_data(PC_DATA_t * pc, FWG2_Handle * FWG2);

void pc_comm_handle(void)
{
    static uint16_t time_1 = 0;
    static uint16_t time_2 = 0;
    time_1++;
    time_2++;

    if (time_1 >= 5)
    {
        pc_event_handle();
        time_1 = 0;
    }

    if (time_2 >= 100)
    {
        send_heartbeat_data(&pc_data, &sFWG2_t);
        time_2 = 0;
    }

    RecvDataFromPC(&pc_data);
}

static void RecvDataFromPC(PC_DATA_t *pc)
{
    uint32_t crc_value;
    pc->read_size = usart_receiveData(PC_USART, pc->rx_buff);

    if (pc->read_size == PC_MAX_RECV_SIZE)
    {
        if (PC_CHECK_HEAD(pc->rx_buff[PC_HEAD1], pc->rx_buff[PC_HEAD2]))
        {
            /* crc check */
            memset(pc->check_crc_buff, 0, PC_CRC_BUFF_SIZE);
            convert_data(pc->rx_buff, pc->check_crc_buff, PC_CMD1, PC_DATA5_LEN_L);
            crc_value = crc_block_calculate(pc->check_crc_buff, 4);
            crc_data_reset();

            if (pc->rx_buff[PC_CRC32_1] == ((crc_value >> 24) & 0xff)  &&
                    pc->rx_buff[PC_CRC32_2] == ((crc_value >> 16) & 0xff)  &&
                    pc->rx_buff[PC_CRC32_3] == ((crc_value >> 8)  & 0xff)   &&
                    pc->rx_buff[PC_CRC32_4] == (crc_value & 0xff))
            {
                /* connect PC */
                if (pc->rx_buff[PC_CMD1] == 0x01 && pc->rx_buff[PC_CMD2] == 0x01 && pc->rx_buff[PC_ID_L] == 0x00)
                {
                    pc_event = CONNECT_PC_EVENT;
                }
                /* jump to bootloader */
                else if (pc->rx_buff[PC_CMD1] == PC_FIRMWARE_UPDATE && pc->rx_buff[PC_CMD2] == PC_FIRMWARE_UPDATE_CONNECT
                         && pc->rx_buff[PC_ID_L] == 0x00)
                {
                    pc_event = IAP_EVENT;
                }
                /* other cmd */
                else
                {
                    __NOP();
                }
            }
        }

        pc->read_size = 0;
    }
}


static void WriteDataToPC(PC_DATA_t * pc,
                          uint16_t cmd_1,  uint16_t cmd_2,
                          uint16_t id,     uint16_t data_len,
                          uint16_t data_1, uint16_t data_2,
                          uint16_t data_3, uint16_t data_4,
                          uint16_t data_5)
{
    static uint32_t crc_value;
    pc->tx_buff[PC_HEAD1] = PC_HEAD_1;
    pc->tx_buff[PC_CMD1] = cmd_1;
    pc->tx_buff[PC_CMD2] = cmd_2;
    pc->tx_buff[PC_ID_H] = (uint8_t)((id >> 8) & 0xff);
    pc->tx_buff[PC_ID_L] = (uint8_t)((id & 0XFF));
    pc->tx_buff[PC_DATA_LEN_H] = (uint8_t)((data_len >> 8) & 0xff)	;
    pc->tx_buff[PC_DATA_LEN_L] = (uint8_t)((data_len & 0XFF));
    pc->tx_buff[PC_DATA1_LEN_H] = (uint8_t)((data_1 >> 8) & 0xff);
    pc->tx_buff[PC_DATA1_LEN_L] = (uint8_t)((data_1 & 0XFF));
    pc->tx_buff[PC_DATA2_LEN_H] = (uint8_t)((data_2 >> 8) & 0xff);
    pc->tx_buff[PC_DATA2_LEN_L] = (uint8_t)((data_2 & 0XFF));
    pc->tx_buff[PC_DATA3_LEN_H] = (uint8_t)((data_3 >> 8) & 0xff);
    pc->tx_buff[PC_DATA3_LEN_L] = (uint8_t)((data_3 & 0XFF));
    pc->tx_buff[PC_DATA4_LEN_H] = (uint8_t)((data_4 >> 8) & 0xff);
    pc->tx_buff[PC_DATA4_LEN_L] = (uint8_t)((data_4 & 0XFF));
    pc->tx_buff[PC_DATA5_LEN_H] = (uint8_t)((data_5 >> 8) & 0xff);
    pc->tx_buff[PC_DATA5_LEN_L] = (uint8_t)((data_5 & 0XFF));
    memset(pc->check_crc_buff, 0, PC_CRC_BUFF_SIZE);
    convert_data(pc->tx_buff, pc->check_crc_buff, PC_CMD1, PC_DATA5_LEN_L);
    crc_value = crc_block_calculate(pc->check_crc_buff, 4);
    crc_data_reset();
    pc->tx_buff[PC_CRC32_1] = ((crc_value >> 24) & 0xff);
    pc->tx_buff[PC_CRC32_2] = ((crc_value >> 16) & 0xff);
    pc->tx_buff[PC_CRC32_3] = ((crc_value >> 8) & 0xff);
    pc->tx_buff[PC_CRC32_4] = (crc_value & 0xff);
    pc->tx_buff[PC_HEAD2] = PC_HEAD_2;
    /* send data */
    usart_sendData(PC_USART, pc->tx_buff, PC_MAX_SEND_SIZE);
}

void pc_event_handle(void)
{
    static uint32_t crc_value;

    switch (pc_event)
    {
    case CONNECT_PC_EVENT:
        pc_data.tx_buff[PC_HEAD1] = PC_HEAD_1;
	
        pc_data.tx_buff[PC_CMD1] = 0x01;
        pc_data.tx_buff[PC_CMD2] = 0x01;
	
        pc_data.tx_buff[PC_ID_H] = LOCAL_DEVECE_ID_2;
        pc_data.tx_buff[PC_ID_L] = LOCAL_DEVECE_ID_1;
	
        pc_data.tx_buff[PC_DATA_LEN_H] =  0x00;
        pc_data.tx_buff[PC_DATA_LEN_L] =  0x0A;
	
        pc_data.tx_buff[PC_DATA1_LEN_H] = 0x01;
        pc_data.tx_buff[PC_DATA1_LEN_L] = 0x04;
        pc_data.tx_buff[PC_DATA2_LEN_H] = 0x03;
	
        pc_data.tx_buff[PC_DATA2_LEN_L] = 0x01;
        pc_data.tx_buff[PC_DATA3_LEN_H] = 0x00;
        pc_data.tx_buff[PC_DATA3_LEN_L] = 0x00;
	
        pc_data.tx_buff[PC_DATA4_LEN_H] = 0x00;
        pc_data.tx_buff[PC_DATA4_LEN_L] = 0x00;
		
        pc_data.tx_buff[PC_DATA5_LEN_H] = 0x00;
        pc_data.tx_buff[PC_DATA5_LEN_L] = 0x00;
		
        memset(pc_data.check_crc_buff, 0, PC_CRC_BUFF_SIZE);
        convert_data(pc_data.tx_buff, pc_data.check_crc_buff, PC_CMD1, PC_DATA5_LEN_L);
        crc_value = crc_block_calculate(pc_data.check_crc_buff, 4);
        crc_data_reset();
		
        pc_data.tx_buff[PC_CRC32_1] = ((crc_value >> 24) & 0xff);
        pc_data.tx_buff[PC_CRC32_2] = ((crc_value >> 16) & 0xff);
        pc_data.tx_buff[PC_CRC32_3] = ((crc_value >> 8) & 0xff);
        pc_data.tx_buff[PC_CRC32_4] = (crc_value & 0xff);
        pc_data.tx_buff[PC_HEAD2] = PC_HEAD_2;
		/* send data */
        usart_sendData(PC_USART, pc_data.tx_buff, PC_MAX_SEND_SIZE);
 
        pc_event = PC_END_EVENT;
        break;

    case IAP_EVENT:
        pc_event = PC_END_EVENT;
        iap_flag = IAP_REV_FLAG_DONE;
        break;

    case PC_END_EVENT:
        break;
    }
}

void send_heartbeat_data(PC_DATA_t * pc, FWG2_Handle * FWG2)
{
#if 0
    static uint32_t crc_value;
    static uint32_t convert_result;
    static uint8_t last_channel = 0;
    static uint16_t show_set_temp = 0;

    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
    {
        show_set_temp = sFWG2_t.Direct_handle_parameter.set_temp;
    }
    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
    {
        show_set_temp = sFWG2_t.Direct_handle_parameter.set_temp_f_display;
    }

    if (sFWG2_t.general_parameter.setting_temp_flag == true)
    {
        pc->tx_buff[10]       = (uint8_t)((show_set_temp >> 8) & 0xff);
        pc->tx_buff[11]       = (uint8_t)((show_set_temp & 0XFF));
    }
    else if (sFWG2_t.general_parameter.setting_temp_flag == false)
    {
        pc->tx_buff[10]       = (uint8_t)(((sFWG2_t.Direct_handle_parameter.show_temp) >> 8) & 0xff);
        pc->tx_buff[11]       = (uint8_t)(((sFWG2_t.Direct_handle_parameter.show_temp) & 0XFF));
    }

    pc->tx_buff[0]        = 0xD1;
    pc->tx_buff[1]        = 0x01;
    pc->tx_buff[2]        = 0x02;
    pc->tx_buff[3]        = 0x00;
    pc->tx_buff[4]        = 0x01;
    pc->tx_buff[5]        = 0x00;
    pc->tx_buff[6]        = 0x22;
    pc->tx_buff[7]        = 1;
    pc->tx_buff[8]        = 2;
    pc->tx_buff[9]        = 3;
    pc->tx_buff[12]       = (uint8_t)sFWG2_t.Direct_handle_parameter.show_wind;

    if (FWG2->general_parameter.countdown_flag == true)
    {
        pc->tx_buff[13]       = (uint8_t)((FWG2->general_parameter.countdown_time_display >> 8) & 0xff);
        pc->tx_buff[14]       = (uint8_t)((FWG2->general_parameter.countdown_time_display & 0XFF));
    }
    else if (FWG2->general_parameter.countdown_flag == false)
    {
        pc->tx_buff[13]       = (uint8_t)((FWG2->general_parameter.countdown_time >> 8) & 0xff);
        pc->tx_buff[14]       = (uint8_t)((FWG2->general_parameter.countdown_time & 0XFF));
    }

    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
    {
        pc->tx_buff[15]       = (uint8_t)((FWG2->general_parameter.ch1_set_temp >> 8) & 0xff);
        pc->tx_buff[16]       = (uint8_t)((FWG2->general_parameter.ch1_set_temp & 0XFF));
    }
    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
    {
        pc->tx_buff[15]       = (uint8_t)(((9 * FWG2->general_parameter.ch1_set_temp / 5 + 32) >> 8) & 0xff);
        pc->tx_buff[16]       = (uint8_t)(((9 * FWG2->general_parameter.ch1_set_temp / 5 + 32) & 0XFF));
    }

    pc->tx_buff[17]       = (uint8_t)FWG2->general_parameter.ch1_set_wind;

    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
    {
        pc->tx_buff[18]       = (uint8_t)((FWG2->general_parameter.ch2_set_temp >> 8) & 0xff);
        pc->tx_buff[19]       = (uint8_t)((FWG2->general_parameter.ch2_set_temp & 0XFF));
    }
    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
    {
        pc->tx_buff[18]       = (uint8_t)(((9 * FWG2->general_parameter.ch2_set_temp / 5 + 32) >> 8) & 0xff);
        pc->tx_buff[19]       = (uint8_t)(((9 * FWG2->general_parameter.ch2_set_temp / 5 + 32) & 0XFF));
    }

    pc->tx_buff[20]       = (uint8_t)FWG2->general_parameter.ch2_set_wind;

    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
    {
        pc->tx_buff[21]       = (uint8_t)((FWG2->general_parameter.ch3_set_temp >> 8) & 0xff);
        pc->tx_buff[22]       = (uint8_t)((FWG2->general_parameter.ch3_set_temp & 0XFF));
    }
    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
    {
        pc->tx_buff[21]       = (uint8_t)(((9 * FWG2->general_parameter.ch3_set_temp / 5 + 32) >> 8) & 0xff);
        pc->tx_buff[22]       = (uint8_t)(((9 * FWG2->general_parameter.ch3_set_temp / 5 + 32) & 0XFF));
    }

    pc->tx_buff[23]       = (uint8_t)FWG2->general_parameter.ch3_set_wind;

    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
    {
        pc->tx_buff[24]       = (uint8_t)((FWG2->general_parameter.ch4_set_temp >> 8) & 0xff);
        pc->tx_buff[25]       = (uint8_t)((FWG2->general_parameter.ch4_set_temp & 0XFF));
    }
    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
    {
        pc->tx_buff[24]       = (uint8_t)(((9 * FWG2->general_parameter.ch4_set_temp / 5 + 32) >> 8) & 0xff);
        pc->tx_buff[25]       = (uint8_t)(((9 * FWG2->general_parameter.ch4_set_temp / 5 + 32) & 0XFF));
    }

    pc->tx_buff[26]       = (uint8_t)FWG2->general_parameter.ch4_set_wind;

    if (last_channel != sFWG2_t.general_parameter.ch)
    {
        last_channel = sFWG2_t.general_parameter.ch;
        /* show channel select state */
        pc->tx_buff[27]       = (uint8_t)FWG2->general_parameter.ch;
    }

    /* show channel select state */
    if (sFWG2_t.general_parameter.ch == 1)
    {
        if (sFWG2_t.general_parameter.ch1_set_temp != sFWG2_t.Direct_handle_parameter.set_temp ||
                sFWG2_t.general_parameter.ch1_set_wind != sFWG2_t.Direct_handle_parameter.set_wind ||
                sFWG2_t.general_parameter.ch1_set_time != sFWG2_t.general_parameter.countdown_time)
        {
            /* show not select channel */
            pc->tx_buff[27]       = 0;
        }
    }
    else if (sFWG2_t.general_parameter.ch == 2)
    {
        if (sFWG2_t.general_parameter.ch2_set_temp != sFWG2_t.Direct_handle_parameter.set_temp ||
                sFWG2_t.general_parameter.ch2_set_wind != sFWG2_t.Direct_handle_parameter.set_wind ||
                sFWG2_t.general_parameter.ch2_set_time != sFWG2_t.general_parameter.countdown_time)
        {
            /* show not select channel */
            pc->tx_buff[27]       = 0;
        }
    }
    else if (sFWG2_t.general_parameter.ch == 3)
    {
        if (sFWG2_t.general_parameter.ch3_set_temp != sFWG2_t.Direct_handle_parameter.set_temp ||
                sFWG2_t.general_parameter.ch3_set_wind != sFWG2_t.Direct_handle_parameter.set_wind ||
                sFWG2_t.general_parameter.ch3_set_time != sFWG2_t.general_parameter.countdown_time)
        {
            /* show not select channel */
            pc->tx_buff[27]       = 0;
        }
    }
    else if (sFWG2_t.general_parameter.ch == 4)
    {
        if (sFWG2_t.general_parameter.ch4_set_temp != sFWG2_t.Direct_handle_parameter.set_temp ||
                sFWG2_t.general_parameter.ch4_set_wind != sFWG2_t.Direct_handle_parameter.set_wind ||
                sFWG2_t.general_parameter.ch4_set_time != sFWG2_t.general_parameter.countdown_time)
        {
            /* show not select channel */
            pc->tx_buff[27]       = 0;
        }
    }

    pc->tx_buff[28]       = (uint8_t)!FWG2->Direct_handle_state;
    pc->tx_buff[29]       = (uint8_t)FWG2->Direct_handle_work_mode;
    pc->tx_buff[30]       = (uint8_t)FWG2->Direct_handle_error_state;
    pc->tx_buff[31]       = (uint8_t)FWG2->general_parameter.temp_uint;
    pc->tx_buff[32]       = (uint8_t)FWG2->general_parameter.fwg2_page;
    pc->tx_buff[33]       = (uint8_t)(sFWG2_t.general_parameter.pid_out / 599);

    if (sFWG2_t.general_parameter.setting_temp_flag == true)
    {
        pc->tx_buff[34]   = 1;
    }
    else if (sFWG2_t.general_parameter.setting_temp_flag == false)
    {
        pc->tx_buff[34]   = 0;
    }

    if (sFWG2_t.general_parameter.setting_wind_flag || sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
    {
        pc->tx_buff[35]       = 1;
    }
    else
    {
        pc->tx_buff[35]       = 0;
    }

    if (FWG2->general_parameter.countdown_flag == true)
    {
        pc->tx_buff[36]       = 0;
    }
    else if (FWG2->general_parameter.countdown_flag == false)
    {
        pc->tx_buff[36]       = 1;
    }

    if (FWG2->general_parameter.speak_state == SPEAKER_CLOSE)
    {
        pc->tx_buff[37]       = 0;
    }
    else if (FWG2->general_parameter.speak_state == SPEAKER_OPEN)
    {
        pc->tx_buff[37]       = 1;
    }

    if (FWG2->general_parameter.enhance_state == ENHANCE_CLOSE)
    {
        pc->tx_buff[38]       = 0;
    }
    else if (FWG2->general_parameter.speak_state == ENHANCE_OPEN)
    {
        pc->tx_buff[38]       = 1;
    }

    pc->tx_buff[39] = 0;
    pc->tx_buff[40] = 0;
    memset(pc->check_crc_buff, 0, PC_CRC_BUFF_SIZE);
    convert_data(pc->tx_buff, pc->check_crc_buff, 1, 40);
    crc_value = crc_block_calculate(pc->check_crc_buff, 10);
    crc_data_reset();
    pc->tx_buff[41] = ((crc_value >> 24) & 0xff);
    pc->tx_buff[42] = ((crc_value >> 16) & 0xff);
    pc->tx_buff[43] = ((crc_value >> 8) & 0xff);
    pc->tx_buff[44] = (crc_value & 0xff);
    pc->tx_buff[45] = PC_HEAD_2;
    /* send data */
    usart_sendData(PC_USART, pc->tx_buff, 46);
#endif
#if 1
    static uint32_t crc_value;
    static uint32_t convert_result;
	
    pc->tx_buff[0]        = PC_HEAD_1;
    pc->tx_buff[1]        = 0x01;
    pc->tx_buff[2]        = 0x02;
    pc->tx_buff[3]        = LOCAL_DEVECE_ID_2;
    pc->tx_buff[4]        = LOCAL_DEVECE_ID_1;
    pc->tx_buff[5]        = 0x00;
    pc->tx_buff[6]        = 0x06;
    pc->tx_buff[7]        = (uint8_t)(((sFWG2_t.Direct_handle_parameter.show_temp) >> 8) & 0xff);
    pc->tx_buff[8]        = (uint8_t)(((sFWG2_t.Direct_handle_parameter.show_temp) & 0XFF));
    pc->tx_buff[9]        = (uint8_t)sFWG2_t.Direct_handle_parameter.show_wind;
    pc->tx_buff[10]       = (uint8_t)FWG2->general_parameter.temp_uint;
    pc->tx_buff[11]       = 0x00;
    pc->tx_buff[12]       = 0x00;
    memset(pc->check_crc_buff, 0, PC_CRC_BUFF_SIZE);
    convert_data(pc->tx_buff, pc->check_crc_buff, 1, 12);
    crc_value = crc_block_calculate(pc->check_crc_buff,3);
    crc_data_reset();
    pc->tx_buff[13] = ((crc_value >> 24) & 0xff);
    pc->tx_buff[14] = ((crc_value >> 16) & 0xff);
    pc->tx_buff[15] = ((crc_value >> 8) & 0xff);
    pc->tx_buff[16] = (crc_value & 0xff);
    pc->tx_buff[17] = PC_HEAD_2;
	 /* send data */
    usart_sendData(PC_USART, pc->tx_buff, 18);
#endif
}

























