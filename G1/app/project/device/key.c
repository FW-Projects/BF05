#include "key.h"
#include <stddef.h>
static uint16_t g_scan_interval_ms = 25;

void key_set_scan_interval(uint16_t scan_interval_ms)
{
    if (scan_interval_ms > 0)
    {
        g_scan_interval_ms = scan_interval_ms;
    }
}

static uint16_t ms_to_scan_count(uint16_t ms)
{
    if (ms == 0 || g_scan_interval_ms == 0)
        return 0;
    return (ms + g_scan_interval_ms - 1) / g_scan_interval_ms;
}

 
static uint8_t read_key_level(key_obj *key)
{
    uint8_t gpio_level = gpio_input_data_bit_read(key->port, key->pin) ? 1 : 0;
    return (gpio_level == key->active_level) ? 1 : 0;
}

 
void key_set_timing(key_obj *key, key_timing_config_t *timing)
{
    if (key == NULL || timing == NULL)
        return;

    key->debounce_count = ms_to_scan_count(timing->debounce_ms);
    key->long_press_count = ms_to_scan_count(timing->long_press_ms);
    key->double_click_count = ms_to_scan_count(timing->double_click_interval_ms);
    key->long_press_continue_interval = ms_to_scan_count(timing->long_press_continue_interval_ms);

    
    if (key->debounce_count == 0 && timing->debounce_ms > 0)
    {
        key->debounce_count = 1;
    }
}

 
void key_set_timing_simple(key_obj *key,
                           uint16_t debounce_ms,
                           uint16_t long_press_ms,
                           uint16_t double_click_interval_ms,
                           uint16_t long_press_continue_interval_ms)
{
    key_timing_config_t timing = {
        .debounce_ms = debounce_ms,
        .long_press_ms = long_press_ms,
        .double_click_interval_ms = double_click_interval_ms,
        .long_press_continue_interval_ms = long_press_continue_interval_ms};

    key_set_timing(key, &timing);
}

key_event_t key_event_check(key_obj *key)
{
    uint8_t current_level = read_key_level(key);
    key_event_t event = KEY_NONE;

    switch (key->state)
    {
    case STATE_RELEASED:
        if (current_level)
        {
            key->state = STATE_DEBOUNCE;
            key->press_count = 0;
            key->long_press_triggered = 0;
            key->long_press_continue_count = 0;
        }
        key->release_count = 0;
        break;

    case STATE_DEBOUNCE:
        if (current_level)
        {
            key->press_count++;
            if (key->press_count >= key->debounce_count)
            {
                key->state = STATE_PRESSED;
                event = KEY_PRESS;
                key->click_count++;
                key->press_count = 0;
            }
        }
        else
        {
            key->state = STATE_RELEASED;
        }
        break;

    case STATE_PRESSED:
        if (!current_level)
        {
            key->state = STATE_DEBOUNCE;
            key->release_count = 0;

            if (key->press_count < key->long_press_count)
            {
                 
                if (key->click_count == 1)
                {
                     
                    key->state = STATE_WAIT_DOUBLE;
                    
                }
                else if (key->click_count >= 2)
                {
                     
                    event = KEY_DOUBLE_CLICK;
                    key->click_count = 0;
                    key->state = STATE_RELEASED;  
                }
            }
            else
            {
                
                event = KEY_SHORT_PRESS;
                key->click_count = 0;
            }
            key->press_count = 0;
            key->long_press_triggered = 0;
        }
        else
        {
            key->press_count++;

             
            if (key->press_count >= key->long_press_count)
            {
                if (!key->long_press_triggered)
                {
                     
                    event = KEY_LONG_PRESS;
                    key->long_press_triggered = 1;
                    key->state = STATE_LONG_PRESSED;
                    key->long_press_continue_count = 0;
                    key->click_count = 0;  
                }
            }
        }
        break;

    case STATE_LONG_PRESSED:
        if (!current_level)
        {
            key->state = STATE_DEBOUNCE;
            key->long_press_triggered = 0;
            key->press_count = 0;
        }
        else
        {
            
            key->long_press_continue_count++;

            
            if (key->long_press_continue_interval > 0 &&
                key->long_press_continue_count >= key->long_press_continue_interval)
            {
                event = KEY_LONG_PRESS_CONTINUE;
                key->long_press_continue_count = 0;
            }
        }
        break;

    case STATE_WAIT_DOUBLE:
        if (current_level)
        {
             
            key->state = STATE_DEBOUNCE;
            key->press_count = 0;
            key->release_count = 0;
        }
        else
        {
            key->release_count++;
            if (key->release_count >= key->double_click_count)
            {
                
                if (key->click_count == 1)
                {
                    event = KEY_SHORT_PRESS;
                }
                key->state = STATE_RELEASED;
                key->click_count = 0;
            }
        }
        break;
    }

    return event;
}

uint8_t key_is_pressed(key_obj *key)
{
    return (key->state == STATE_PRESSED || key->state == STATE_LONG_PRESSED);
}

uint8_t key_is_long_pressed(key_obj *key)
{
    return (key->state == STATE_LONG_PRESSED);
}