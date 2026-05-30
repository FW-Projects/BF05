

#ifndef __dwinlcdfunction_h
#define __dwinlcdfunction_h

#include "stdint.h"

#ifndef RxBufferLength
#define RxBufferLength 260
#endif

/*定义迪文屏的开发类型*/
typedef enum DwinSort
{
    DGUS,
    DGUSII
} DwinSortType;

/* 定义通讯校验码使用与否枚举 */
typedef enum DwinCheckCode
{
    DwinNone,
    DwinCRC16
} DwinCheckCodeType;

/*功能码定义*/
typedef enum
{
    FC_REG_Write = ((uint8_t)0x80),       /*写寄存器的功能码*/
    FC_REG_Read = ((uint8_t)0x81),        /*读寄存器的功能码*/
    FC_VAR_Write = ((uint8_t)0x82),       /*写变量的功能码*/
    FC_VAR_Read = ((uint8_t)0x83),        /*读变量的功能码*/
    FC_Curve_Write = ((uint8_t)0x84)      /*写曲线缓冲区的功能码*/
} DwinFunctionCode;

/* 定义迪文串口屏对象类型 */
typedef struct DwinObject
{
    DwinSortType sort;							//屏的类别
    DwinCheckCodeType checkMode;		//校验方式
    struct DwinRxBuffer
    {
        uint8_t queue[RxBufferLength];	//键值存储队列
        uint8_t pRead;									//读队列指针
        uint8_t pWrite;									//写队列指针
        uint16_t (*DeQueue)(struct DwinObject *dwin, uint8_t *rxBuf);	//出队操作
        void (*EnQueue)(struct DwinObject *dwin, uint8_t rData);		//入队操作
    } rxBuffer;					//定义接收缓存队列

    void (*SendData)(uint8_t *txData, uint16_t length);	//发送数据
    void (*GetRegister)(struct DwinObject *dwin, uint8_t regAddress, uint8_t readByteLength);
    void (*SetRegister)(struct DwinObject *dwin, uint8_t regAddress, uint8_t *txData, uint16_t length);
} DwinObjectType;

typedef void (*SendDataForDwinType)(uint8_t *txData, uint16_t length);

/*写数据变量存储器，一次最多允许写47个字，即length<=94*/
void WriteFlashDataToDwinLCD(DwinObjectType *dwin, uint16_t startAddress, uint8_t *txData, uint16_t length);
/*读变量存储器数据*/
void ReadFlashDataFromDwinLCD(DwinObjectType *dwin, uint16_t startAddress, uint8_t readWordLength);
/*写曲线缓冲区，一次最多允许写8个字，即length<=16*/
void WriteCurveToDwinLCD(DwinObjectType *dwin, uint8_t *txData, uint16_t length, uint8_t channelMode);

#endif
/*********** (C) COPYRIGHT 1999-2019 Moonan Technology *********END OF FILE****/
