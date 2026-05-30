#ifndef __FAN_H
#define __FAN_H

#include "at32f415.h"
#include "wk_system.h"

#define ADC_CHANNEL 10

u16 get_adcval(adc_channel_select_type adc_channel);
#endif 
