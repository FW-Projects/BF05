
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
    SHOW_DIRECT_ERROR_CODE = 0x00,     //直风警报弹窗显示 0（不显示） 1（风扇异常）2（低温警报）3（高温报警）
    SHOW_DIRECT_HANDLE_MODE,        //弹窗显示 0（不显示） 1 显示旋风加强模式弹框  2显示冷风模式弹框

    SHOW_DIRECT_WORKING_STATE,      //直风工作状态

    SHOW_DIRECT_TEMP,               //直风温度显示  0：显示实时温度   1：显示设置温度
    SHOW_DIRECT_WIND,               //直风风量显示  0：显示实时风量   1：显示设置风量


    SHOW_DIRECT_FAN_GIF_H,          //直风风扇动画 高位 （0：关闭动画 1：开启动画）
    SHOW_DIRECT_FAN_GIF_L,          //直风风扇动画 低位 （0：关闭动画 1：开启动画）

    SHOW_DIRECT_CURRENT_TEMP,      //直风当前温度颜色
    
	SHOW_COUNTDOWN,                   //倒计时显示
	
	SHOW_DIRECT_TPME_OUTPUT_POWER,    //直风加热输出功率百分比
    SHOW_DIRECT_TPME_OUTPUT_POWER_BAR,//直风加热输出功率百分比进度条
	
	SHOW_DIRECT_CURRENT_TEMP_BAR,     //直风当前温度进度条
    SHOW_DIRECT_CURRENT_WIND_BAR,     //直风当前风量进度条
	
	
    SHOW_DIRECT_SET_TEMP,           //直风设置温度
    SHOW_DIRECT_SET_WIND,           //直风设置风量



    SET_CHANNEL = 0x0F,                    //选择通道  1~4
    /* end the page 1 address of  */

    /* start the page 2 address of  */
    SHOW_CURVE_TEMP = 0x10,
    SHOW_CURVE_WIND,
	SHOW_CURVE_TIME,
    /* end the page 2 address of  */
    /* start the page 3 and page 4 address of  */
    PAGE_ID = 0x13,
	//1（主页）                  \
	2（曲线）                    \
	3（菜单1）                   \
	4（菜单2）                   \
	5（工作模式设置）            \
	6通常模式设置                \
	7编程模式设置                \
	8温度单位设置                \
	9静音设置                    \
	10温度锁定设置               \
	11加强模式/冷风模式选择      \
	12加强模式参数设置           \
	13亮度调节                   \
	14温度校准设置               \
	15默认倒计时设置             \
	16触摸握把设置               \
	17串口设置                   \
	18语言设置                   \
	19版本号设置                 \
	20恢复出厂设置               \
	21OTA升级设置                \
	24第三页主菜单               \
	25加强功能设置页             \
	26编程曲线页                 
    /* end the page 3 and page 4 address of  */

    /* start the page 5 address of  */
    FWG2_WORK_MODE = 0x14,                  //通常、编程模式  0：正常模式 1：编程模式
    /* end the page 5 address of  */

    /* start the page 6 address of  */
    SHOW_CH1_SET_TEMP = 0x15,                    //通道1温度设置（显示）
    SHOW_CH1_SET_WIND,                    //通道1风量设置（显示）
    SHOW_CH1_SET_TIME,                    //通道1计时设置（显示）

    SHOW_CH2_SET_TEMP,                    //通道2温度设置（显示）
    SHOW_CH2_SET_WIND,                    //通道2风量设置（显示）
    SHOW_CH2_SET_TIME,                    //通道2计时设置（显示）

    SHOW_CH3_SET_TEMP,                    //通道3温度设置（显示）
    SHOW_CH3_SET_WIND,                    //通道3风量设置（显示）
    SHOW_CH3_SET_TIME,                    //通道3计时设置（显示）

    SHOW_CH4_SET_TEMP,                    //通道4温度设置（显示）
    SHOW_CH4_SET_WIND,                    //通道4风量设置（显示）
    SHOW_CH4_SET_TIME = 0x20,             //通道4计时设置（显示）
    /* end the page 6 address of  */

    /* start the page 7 address of  */
    CODE_CHANNEL = 0x21,                    //编程通道              1~4
    SET_PRE_TEMP,                    //预热温度设置
    SET_PRE_WIND,                    //预热风速设置
    SET_PRE_TIME,                    //预热拆焊时间
    SET_WORK_TEMP1,                  //拆焊温度1设置
    SET_WORK_WIND1,                  //拆焊风速1设置
    SET_WORK_TIME1,                  //拆焊时间1设置
    SET_WORK_TEMP2,                  //拆焊温度2设置
    SET_WORK_WIND2,                  //拆焊风速2设置
    SET_WORK_TIME2,                  //拆焊时间2设置
    SET_WORK_TEMP3,                  //拆焊温度3设置
    SET_WORK_WIND3,                  //拆焊风速3设置
    SET_WORK_TIME3 = 0x2D,                  //拆焊时间3设置
    /* end the page 7 address of  */

    /* start the page 8 address of  */
    SET_TEMP_UNITS = 0x2E,                  //设置显示温度单位     0：华氏度 1:摄氏度
    /* end the page 8 address of  */

    /* start the page 9 address of  */
    SET_SPEAKER_FUNCTION = 0x2F,            //设置蜂鸣器开启或关闭 0：关闭   1：开启
    /* end the page 9 address of  */

    /* start the page 10 address of  */
    SET_DISPLAY_LOCK_MODE = 0x30,           //设置温度显示锁定模式 0：不锁定 1：锁定
    /* end the page 10 address of  */

    /* start the page 11 address of  */
    SET_FN_KEY_MODE = 0x31,                 //设置加强、冷风模式   0：加强   1：冷风  2：倒计时
	SET_ADJUST_KEY,
    /* end the page 11 address of  */

    /* start the page 12 address of  */
    SET_ENHANCE_TEMP = 0x33,                 //设置增强模式温度
    SET_ENHANCE_COUNTDOWN_TIME,               //设置增强模式时间
    /* end the page 12 address of  */

    /* start the page 13 address of  */
    /* end the page 13 address of  */

    /* start the page 14 address of  */
    SET_DIRECT_CALIBRATE_TEMP = 0x35,                 //当前温度值
    /* end the page 14 address of  */

    /* start the page 15 address of  */
    SET_TIME = 0x36,                          //设置默认时间
    /* end the page 15 address of  */

    /* start the page 16 address of  */
    SET_TOUCH_FUNCTION = 0x37,                //设置手柄触摸功能
    /* end the page 16 address of  */

    /* start the page 17 address of  */
    SET_UART_FUNCTION = 0x38,                //设置串口功能
    /* end the page 17 address of  */

    /* start the page 18 address of  */
    /* end the page 18 address of  */

    /* start the page 19 address of  */
    SHOW_SOFTWARE_VER = 0x39 ,                 //设置软件版本号
    /* end the page 19 address of  */

    /* start the page 20 address of  */
    RESET_FWG2 = 0x3B,                        //恢复出厂设置
	RESET_FWG2_MES,      //恢复出厂设置完成弹窗  0：弹出  1：隐藏
    /* end the page 20 address of  */

    /* start the page 21 address of  */
    SET_OTA_FUNCTION = 0x3D,                  //设置OTA功能
    /* end the page 21 address of  */


    /* start the page 30 address of  */
    EN_HANCE_FUNCTION = 0x3E,
    /* end the page 30 address of  */

     /* start the page 28 address of  */
	 SHOW_CODE_MODE_TEMP = 0x3F,
	 SHOW_CODE_MODE_WIND ,
	 SHOW_CODE_MODE_COUNTDOWN_TIME,
	 START_STOP_CODE_MODE,            //停止运行编程模式按键
	 SHOW_CODE_STEP,                  //运行步骤指示          0~5


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
