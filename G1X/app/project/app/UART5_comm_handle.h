#ifndef __UART5_COMM_HANDLE_H
#define __UART5_COMM_HANDLE_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "at32f415.h"
#include "at32_Usart.h"
#define UART5_HANDLE_TIME   5
#define UART5_USART         4
#define UART5_RX_BUFF_SIZE  256
#define UART5_TX_BUFF_SIZE  256

#define UART5_CRC_SIZE      4
#define UART5_MAX_SEND_SIZE 22
#define UART5_MAX_RECV_SIZE 22

#define UART5_CHECK_HEAD(A,B)         ((A==0xD1)&&(B==0xF0)?TRUE:FALSE)

#define UART5_HEAD1                0x00
#define UART5_CMD1                 0x01
#define UART5_CMD2                 0x02
#define UART5_ID_H                 0x03
#define UART5_ID_L                 0x04
#define UART5_DATA_LEN_H           0x05
#define UART5_DATA_LEN_L           0x06
#define UART5_DATA1_LEN_H          0x07
#define UART5_DATA1_LEN_L          0x08
#define UART5_DATA2_LEN_H          0x09
#define UART5_DATA2_LEN_L          0x0A
#define UART5_DATA3_LEN_H          0x0B
#define UART5_DATA3_LEN_L          0x0C
#define UART5_DATA4_LEN_H          0x0D
#define UART5_DATA4_LEN_L          0x0E
#define UART5_DATA5_LEN_H          0x0F
#define UART5_DATA5_LEN_L          0x10
#define UART5_CRC32_1              0x11
#define UART5_CRC32_2              0x12
#define UART5_CRC32_3              0x13
#define UART5_CRC32_4              0x14
#define UART5_HEAD2                0x15

typedef enum
{
    UART5_HEAD_1 = ((uint8_t)0xD1),
    UART5_HEAD_2 = ((uint8_t)0xF0),

} uart5_head_e;

typedef enum
{
	UART5_GENERAL = 0x01,
	UART5_FIRMWARE_UPDATE = 0x02,
    UART5_UI_UPDATE = 0x03,
}uart5_cmd1_e;

typedef enum
{
	UART5_GENERAL_ACK = 0x02,
	UART5_GENERAL_READ = 0x05,
	UART5_GENERAL_WRITE = 0x06,
	
	
}uart5_cmd2_e;

typedef enum
{        
    UART5_SET_TEMP_ADD_EVENT,
    UART5_SET_TEMP_REDUCE_EVENT,
    UART5_SET_TEMP_EVENT,

    UART5_SET_WIND_ADD_EVENT,
    UART5_SET_WIND_REDUCE_EVENT,
    UART5_SET_WIND_EVENT,

    UART5_GET_CHANNEL_1_EVENT,
    UART5_GET_CHANNEL_2_EVENT,
    UART5_GET_CHANNEL_3_EVENT,
    UART5_GET_CHANNEL_4_EVENT,

    UART5_ENTER_ENHANCE_MODE_EVENT,
    UART5_ENTER_NORMAL_MODE_EVENT,
    UART5_ENTER_COLD_MODE_EVENT,

    UART5_SHOW_CURVE_PAGE_EVENT,
    UART5_SHOW_NORMAL_PAGE_EVENT,

    UART5_SET_TEMP_UNIT_C,
    UART5_SET_TEMP_UNIT_F,

    UART5_SELECT_HANDLE_EVENT,
    UART5_FACTORY_EVENT,
    UART5_END_EVENT,
}uart5_event_e;





typedef struct UART5Object
{
    bool connect_flag;
    uint8_t rx_buff[UART5_RX_BUFF_SIZE];
    uint8_t read_size;
    uint8_t tx_buff[UART5_TX_BUFF_SIZE];
    uint32_t check_crc_buff[UART5_CRC_SIZE];

} UART5_DATA_t;
void uart5_comm_handle(void);

#endif

