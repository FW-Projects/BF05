#ifndef _BEEP_H
#define _BEEP_H

#include <stdio.h>
#include "at32f415.h"

/* output */
#define BEEP_PORT             GPIOB
#define BEEP_PIN              GPIO_PINS_12

typedef enum
{
    BEEP_OPEN = 0,
    BEEP_CLOSE,
} BEEP_WORK;


typedef enum
{
    BEEP_OFF = 0,
    BEEP_SHORT,
	BEEP_LONG,
} BEEP_STATUS;

typedef struct
{
	BEEP_WORK beep_work;
    char *name;
    BEEP_STATUS status;      //״̬
    void (*on)(void);     //��
    void (*off)(void);    //�ر�
	
} beep_t;

static void beep_on(void);
static void beep_off(void);
#endif
