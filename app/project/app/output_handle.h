#ifndef _OUTPUT_HANDLE_H
#define _OUTPUT_HANDLE_H
#include <stdbool.h>
#include "adc.h"

#define OUTPUT_HANDLE_TIME          10
#define RELAY_OPEN_TIME             60
typedef enum
{
    FAN_STOP = 0,
    FAN_WORKING,
} fan_state_t;

typedef enum
{
    HOT_STOP = 0,
    HOT_WORKING,
    HOT_WORKING_OVERLOAD
} hot_state_t;

typedef struct
{
    bool open;
    fan_state_t fan_state;
    hot_state_t hot_state;
    uint8_t channel;
} output_t;




void fan_control(void);
void hot_control(void);
extern uint8_t linear_correction(uint16_t user_set_temp);
extern uint16_t temp_get(void);
uint16_t temp_get_filter_move_average(adc_channel_select_type adc_channel);
uint16_t temp_get_filter_middleValue(void);
uint16_t temp_get_filter_average(void);
uint16_t temp_get_filter_mode(void);
uint16_t temp_get_filter_max_count(void);
uint16_t temp_get_average(void);
#endif
