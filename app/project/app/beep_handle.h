#ifndef _BEEP_HANDLE_H
#define _BEEP_HANDLE_H
#include "beep.h"

#define BEEP_HANDLE_TIME          20
#define BEEP_WAIT_TIME            2

typedef enum
{
    BEEP_IDLE,
    BEEP_START_SHORT,
    BEEP_START_LONG,
    BEEP_WAIT,
    BEEP_FINISH
} beep_handle_t;

void BeepProc(beep_t *beep);
extern  beep_t sbeep;
#endif
