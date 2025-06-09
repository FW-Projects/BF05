

#ifndef __dwinlcdfunction_h
#define __dwinlcdfunction_h

#include "stdint.h"

#ifndef RxBufferLength
#define RxBufferLength 260
#endif

/*����������Ŀ�������*/
typedef enum DwinSort
{
    DGUS,
    DGUSII
} DwinSortType;

/* ����ͨѶУ����ʹ�����ö�� */
typedef enum DwinCheckCode
{
    DwinNone,
    DwinCRC16
} DwinCheckCodeType;

/*�����붨��*/
typedef enum
{
    FC_REG_Write = ((uint8_t)0x80),       /*д�Ĵ����Ĺ�����*/
    FC_REG_Read = ((uint8_t)0x81),        /*���Ĵ����Ĺ�����*/
    FC_VAR_Write = ((uint8_t)0x82),       /*д�����Ĺ�����*/
    FC_VAR_Read = ((uint8_t)0x83),        /*�������Ĺ�����*/
    FC_Curve_Write = ((uint8_t)0x84)      /*д���߻������Ĺ�����*/
} DwinFunctionCode;

/* ������Ĵ������������� */
typedef struct DwinObject
{
    DwinSortType sort;							//�������
    DwinCheckCodeType checkMode;		//У�鷽ʽ
    struct DwinRxBuffer
    {
        uint8_t queue[RxBufferLength];	//��ֵ�洢����
        uint8_t pRead;									//������ָ��
        uint8_t pWrite;									//д����ָ��
        uint16_t (*DeQueue)(struct DwinObject *dwin, uint8_t *rxBuf);	//���Ӳ���
        void (*EnQueue)(struct DwinObject *dwin, uint8_t rData);		//��Ӳ���
    } rxBuffer;					//������ջ������

    void (*SendData)(uint8_t *txData, uint16_t length);	//��������
    void (*GetRegister)(struct DwinObject *dwin, uint8_t regAddress, uint8_t readByteLength);
    void (*SetRegister)(struct DwinObject *dwin, uint8_t regAddress, uint8_t *txData, uint16_t length);
} DwinObjectType;

typedef void (*SendDataForDwinType)(uint8_t *txData, uint16_t length);

/*д���ݱ����洢����һ���������д47���֣���length<=94*/
void WriteFlashDataToDwinLCD(DwinObjectType *dwin, uint16_t startAddress, uint8_t *txData, uint16_t length);
/*�������洢������*/
void ReadFlashDataFromDwinLCD(DwinObjectType *dwin, uint16_t startAddress, uint8_t readWordLength);
/*д���߻�������һ���������д8���֣���length<=16*/
void WriteCurveToDwinLCD(DwinObjectType *dwin, uint8_t *txData, uint16_t length, uint8_t channelMode);

#endif
/*********** (C) COPYRIGHT 1999-2019 Moonan Technology *********END OF FILE****/
