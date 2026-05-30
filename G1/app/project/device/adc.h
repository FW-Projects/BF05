#ifndef __ADC_H
#define __ADC_H

#include "at32f415.h"
#include "wk_system.h"

#define ADC_CHANNEL 10

void adc_init(void);uint16_t Search(uint16_t a[],uint16_t len);
u16 get_adcval(adc_channel_select_type adc_channel);
u16 get_adcval_average(adc_channel_select_type adc_channel,u8 times);
u16 middleValueFilter(void);
u16 moveAverageFilter(adc_channel_select_type adc_channel);
uint16_t modeFilter(void);
uint16_t max_count_filter(void);
uint16_t temp_get_filter_max_count(void);
uint16_t average(uint8_t n);
#endif 
