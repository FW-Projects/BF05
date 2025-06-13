
#ifndef __DWIN_HANDLE_H
#define __DWIN_HANDLE_H

#include "stdint.h"
#include "at32_Usart.h"

#define ORANGE_COLOR           0xFC60
#define BLUE_COLOR             0x3C7F
#define WHITE_COLOR            0xFFFF
#define RED_COLOR              0xE000


#define SHOW_DIRECT_HANDLE_SET_TEMP_TIME         100
#define SHOW_DIRECT_HANDLE_SET_WIND_TIME         100



#define DIRECT_TEMP_REFIRSH_TIME                 5
#define DIRECT_WIND_REFIRSH_TIME                 5


#define CURVE_REFIRSH_TIME                       1

#define DWIN_HANDLE_TIME          1


#define LOCK_RANGE                20



#define DWIN_USART                2

#define MAX_RECV_LEN              9
#define MAX_SEND_LEN              8
#define CHECK_HEAD(A,B)         ((A==0x5A)&&(B==0xA5)?TRUE:FALSE)

#define FRAME_HEAD1             0x00
#define FRAME_HEAD2             0x01
#define FRAME_DATA_LED          0x02
#define FRAME_CMD               0x03
#define FRAME_Address_H         0x04
#define FRAME_Address_L         0x05
#define FRAME_VAL_LEN           0x06
#define FRAME_VAL_H             0x07
#define FRAME_VAL_L             0x08



#define DWIN_BASE_ADDRESS       0x5000   //temp
#define DWIN_BASE_ADDRESS_1     0x300   //wind
#define DWIN_DATA_BITS          2
#define DWIN_DATA_BITS_1        0B

#define rxbuff_length 1024

#define txbuff_length 1024

typedef enum
{
    /* start the page 1 address of  */
    SHOW_DIRECT_ERROR_CODE = 0x00,     //ֱ�羯��������ʾ 0������ʾ�� 1�������쳣��2�����¾�����3�����±�����
    SHOW_DIRECT_HANDLE_MODE,        //������ʾ 0������ʾ�� 1 ��ʾ�����ǿģʽ����  2��ʾ���ģʽ����

    SHOW_DIRECT_WORKING_STATE,      //ֱ�繤��״̬

    SHOW_DIRECT_TEMP,               //ֱ���¶���ʾ  0����ʾʵʱ�¶�   1����ʾ�����¶�
    SHOW_DIRECT_WIND,               //ֱ�������ʾ  0����ʾʵʱ����   1����ʾ���÷���


    SHOW_DIRECT_FAN_GIF_H,          //ֱ����ȶ��� ��λ ��0���رն��� 1������������
    SHOW_DIRECT_FAN_GIF_L,          //ֱ����ȶ��� ��λ ��0���رն��� 1������������

    SHOW_DIRECT_CURRENT_TEMP,      //ֱ�統ǰ�¶���ɫ
    
	SHOW_COUNTDOWN,                   //����ʱ��ʾ
	
	SHOW_DIRECT_TPME_OUTPUT_POWER,    //ֱ�����������ʰٷֱ�
    SHOW_DIRECT_TPME_OUTPUT_POWER_BAR,//ֱ�����������ʰٷֱȽ�����
	
	SHOW_DIRECT_CURRENT_TEMP_BAR,     //ֱ�統ǰ�¶Ƚ�����
    SHOW_DIRECT_CURRENT_WIND_BAR,     //ֱ�統ǰ����������
	
	
    SHOW_DIRECT_SET_TEMP,           //ֱ�������¶�
    SHOW_DIRECT_SET_WIND,           //ֱ�����÷���



    SET_CHANNEL = 0x0F,                    //ѡ��ͨ��  1~4
    /* end the page 1 address of  */

    /* start the page 2 address of  */
    SHOW_CURVE_TEMP = 0x10,
    SHOW_CURVE_WIND,
	SHOW_CURVE_TIME,
    /* end the page 2 address of  */
    /* start the page 3 and page 4 address of  */
    PAGE_ID = 0x13,
	//1����ҳ��                  \
	2�����ߣ�                    \
	3���˵�1��                   \
	4���˵�2��                   \
	5������ģʽ���ã�            \
	6ͨ��ģʽ����                \
	7���ģʽ����                \
	8�¶ȵ�λ����                \
	9��������                    \
	10�¶���������               \
	11��ǿģʽ/���ģʽѡ��      \
	12��ǿģʽ��������           \
	13���ȵ���                   \
	14�¶�У׼����               \
	15Ĭ�ϵ���ʱ����             \
	16�����հ�����               \
	17��������                   \
	18��������                   \
	19�汾������                 \
	20�ָ���������               \
	21OTA��������                \
	24����ҳ���˵�               \
	25��ǿ��������ҳ             \
	26�������ҳ                 
    /* end the page 3 and page 4 address of  */

    /* start the page 5 address of  */
    FWG2_WORK_MODE = 0x14,                  //ͨ�������ģʽ  0������ģʽ 1�����ģʽ
    /* end the page 5 address of  */

    /* start the page 6 address of  */
    SHOW_CH1_SET_TEMP = 0x15,                    //ͨ��1�¶����ã���ʾ��
    SHOW_CH1_SET_WIND,                    //ͨ��1�������ã���ʾ��
    SHOW_CH1_SET_TIME,                    //ͨ��1��ʱ���ã���ʾ��

    SHOW_CH2_SET_TEMP,                    //ͨ��2�¶����ã���ʾ��
    SHOW_CH2_SET_WIND,                    //ͨ��2�������ã���ʾ��
    SHOW_CH2_SET_TIME,                    //ͨ��2��ʱ���ã���ʾ��

    SHOW_CH3_SET_TEMP,                    //ͨ��3�¶����ã���ʾ��
    SHOW_CH3_SET_WIND,                    //ͨ��3�������ã���ʾ��
    SHOW_CH3_SET_TIME,                    //ͨ��3��ʱ���ã���ʾ��

    SHOW_CH4_SET_TEMP,                    //ͨ��4�¶����ã���ʾ��
    SHOW_CH4_SET_WIND,                    //ͨ��4�������ã���ʾ��
    SHOW_CH4_SET_TIME = 0x20,             //ͨ��4��ʱ���ã���ʾ��
    /* end the page 6 address of  */

    /* start the page 7 address of  */
    CODE_CHANNEL = 0x21,                    //���ͨ��              1~4
    SET_PRE_TEMP,                    //Ԥ���¶�����
    SET_PRE_WIND,                    //Ԥ�ȷ�������
    SET_PRE_TIME,                    //Ԥ�Ȳ�ʱ��
    SET_WORK_TEMP1,                  //���¶�1����
    SET_WORK_WIND1,                  //�𺸷���1����
    SET_WORK_TIME1,                  //��ʱ��1����
    SET_WORK_TEMP2,                  //���¶�2����
    SET_WORK_WIND2,                  //�𺸷���2����
    SET_WORK_TIME2,                  //��ʱ��2����
    SET_WORK_TEMP3,                  //���¶�3����
    SET_WORK_WIND3,                  //�𺸷���3����
    SET_WORK_TIME3 = 0x2D,                  //��ʱ��3����
    /* end the page 7 address of  */

    /* start the page 8 address of  */
    SET_TEMP_UNITS = 0x2E,                  //������ʾ�¶ȵ�λ     0�����϶� 1:���϶�
    /* end the page 8 address of  */

    /* start the page 9 address of  */
    SET_SPEAKER_FUNCTION = 0x2F,            //���÷�����������ر� 0���ر�   1������
    /* end the page 9 address of  */

    /* start the page 10 address of  */
    SET_DISPLAY_LOCK_MODE = 0x30,           //�����¶���ʾ����ģʽ 0�������� 1������
    /* end the page 10 address of  */

    /* start the page 11 address of  */
    SET_FN_KEY_MODE = 0x31,                 //���ü�ǿ�����ģʽ   0����ǿ   1�����  2������ʱ
	SET_ADJUST_KEY,
    /* end the page 11 address of  */

    /* start the page 12 address of  */
    SET_ENHANCE_TEMP = 0x33,                 //������ǿģʽ�¶�
    SET_ENHANCE_COUNTDOWN_TIME,               //������ǿģʽʱ��
    /* end the page 12 address of  */

    /* start the page 13 address of  */
    /* end the page 13 address of  */

    /* start the page 14 address of  */
    SET_DIRECT_CALIBRATE_TEMP = 0x35,                 //��ǰ�¶�ֵ
    /* end the page 14 address of  */

    /* start the page 15 address of  */
    SET_TIME = 0x36,                          //����Ĭ��ʱ��
    /* end the page 15 address of  */

    /* start the page 16 address of  */
    SET_TOUCH_FUNCTION = 0x37,                //�����ֱ���������
    /* end the page 16 address of  */

    /* start the page 17 address of  */
    SET_UART_FUNCTION = 0x38,                //���ô��ڹ���
    /* end the page 17 address of  */

    /* start the page 18 address of  */
    /* end the page 18 address of  */

    /* start the page 19 address of  */
    SHOW_SOFTWARE_VER = 0x39 ,                 //��������汾��
    /* end the page 19 address of  */

    /* start the page 20 address of  */
    RESET_FWG2 = 0x3B,                        //�ָ���������
	RESET_FWG2_MES,      //�ָ�����������ɵ���  0������  1������
    /* end the page 20 address of  */

    /* start the page 21 address of  */
    SET_OTA_FUNCTION = 0x3D,                  //����OTA����
    /* end the page 21 address of  */


    /* start the page 30 address of  */
    EN_HANCE_FUNCTION = 0x3E,
    /* end the page 30 address of  */

     /* start the page 28 address of  */
	 SHOW_CODE_MODE_TEMP = 0x3F,
	 SHOW_CODE_MODE_WIND ,
	 SHOW_CODE_MODE_COUNTDOWN_TIME,
	 START_STOP_CODE_MODE,            //ֹͣ���б��ģʽ����
	 SHOW_CODE_STEP,                  //���в���ָʾ          0~5


	 /* end the page 28 address of  */
	 
	 
     /* out of page value */
     SHOW_DIRECT_TEMP_COLOUR = 0x6003,
     SHOW_DIRECT_WIND_COLOUR = 0x6063,
     SHOW_DIRECT_TIME_COLOUR = 0x6033,
	 
	 SHOW_CURVE_TEMP_VALUE_COLOUR  = 0x60C3,
	 SHOW_CURVE_WIND_VALUE_COLOUR  = 0x60F3,
	 SHOW_CURVE_TIME_VALUE_COLOUR  = 0x6103,
	 
	 SHOW_CODE_TIME_COLOUR   = 0x6093,
} Dwin_Val_Address_e;

typedef enum
{
    DwinNone,
    DwinCRC16
} DwinCheckCodeType;


typedef enum
{
    FC_VAR_Write = ((uint8_t)0x82),
    FC_VAR_Read = ((uint8_t)0x83),
} DwinFunctionCode;


typedef struct DwinObject
{
    DwinCheckCodeType checkMode;
    uint8_t rx_buff[rxbuff_length];
    uint8_t read_size;
    uint8_t tx_buff[txbuff_length];
    void (*send_data)(struct DwinObject *dwin, uint16_t startAddress, uint16_t length, uint16_t data);
    void (*recv_data)(struct DwinObject *dwin);
} DwinObjectType;

extern DwinObjectType sdwin;
extern uint16_t show_direct_set_temp_time ;
extern uint16_t show_direct_set_wind_time ;


void DwinInitialization(DwinObjectType *dwin);
void Page_Main_Heartbeat_Packet(void);
void Page_General_Heartbeat_Packet(void);
void Page_Set_Heartbeat_Packet(void);
void Page_Direct_Work_Heartbeat_Packet(void);
void Page_Cyclone_Work_Heartbeat_Packet(void);

void Page_Direct_Curve_Heartbeat_Packet(void);
void Page_Cyclone_Curve_Heartbeat_Packet(void);
void Page_Code_Heartbeat_Packet(void);
void Page_ALL_Curve_Heartbeat_Packet(void);
void Page_Switch(void);
#endif
/*********** (C) COPYRIGHT 1999-2019 Moonan Technology *********END OF FILE****/
