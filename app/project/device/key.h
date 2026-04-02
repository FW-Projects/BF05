#ifndef __KEY_H
#define __KEY_H

#include "at32f415.h"

// 按键事件类型
typedef enum {
    KEY_NONE = 0,           // 无事件
    KEY_PRESS,              // 按下（瞬时事件）
    KEY_SHORT_PRESS,        // 短按（释放后触发）
    KEY_LONG_PRESS,         // 长按（达到阈值时触发）
    KEY_LONG_PRESS_CONTINUE,// 长按持续（持续触发）
    KEY_DOUBLE_CLICK        // 双击
} key_event_t;

// 按键状态（内部使用）
typedef enum {
    STATE_RELEASED = 0,     // 释放状态
    STATE_PRESSED,          // 按下状态
    STATE_DEBOUNCE,         // 消抖状态
    STATE_WAIT_DOUBLE,      // 等待双击状态
    STATE_LONG_PRESSED      // 长按状态
} key_state_t;

// 时间参数结构体
typedef struct {
    uint16_t debounce_ms;           // 消抖时间（毫秒）
    uint16_t long_press_ms;         // 长按阈值时间（毫秒）
    uint16_t double_click_interval_ms; // 双击间隔时间（毫秒）
    uint16_t long_press_continue_interval_ms; // 长按持续触发间隔（毫秒）
} key_timing_config_t;

// 按键结构体
typedef struct {
    // 硬件配置（用户设置）
    gpio_type *port;        // GPIO端口指针，如GPIOC
    uint16_t pin;           // GPIO引脚，如GPIO_PINS_13
    uint8_t active_level;   // 激活电平：0=低电平有效，1=高电平有效
    
    // 状态机变量（内部使用）
    key_state_t state;
    uint16_t press_count;   // 按下计数（单位：扫描周期）
    uint16_t release_count; // 释放计数
    uint16_t long_press_continue_count; // 长按持续计数
    uint8_t click_count;    // 点击计数
    uint8_t long_press_triggered; // 长按已触发标志
    
    // 时间参数（内部计算，基于扫描周期）
    uint8_t debounce_count;          // 消抖所需扫描次数
    uint8_t long_press_count;        // 长按所需扫描次数  
    uint8_t double_click_count;      // 双击间隔所需扫描次数
    uint8_t long_press_continue_interval; // 长按持续触发间隔
} key_obj;

/**
 * @brief 定义按键对象的宏
 * @param name 按键变量名
 * @param port GPIO端口，如GPIOC
 * @param pin GPIO引脚，如GPIO_PINS_13  
 * @param active_lvl 激活电平：0=低电平有效，1=高电平有效
 */
#define KEY_DEFINE(name, port, pin, active_lvl) \
    key_obj name = { \
        port, pin, active_lvl,        /* 硬件配置 */ \
        STATE_RELEASED, 0, 0, 0, 0, 0, /* 状态变量 */ \
        1, 50, 15, 10                 /* 默认时间参数 */ \
    }

/**
 * @brief 设置按键扫描周期（必须在设置时间参数前调用）
 * @param scan_interval_ms 扫描间隔时间（毫秒）
 */
void key_set_scan_interval(uint16_t scan_interval_ms);

/**
 * @brief 设置按键所有时间参数（毫秒单位）
 * @param key 按键对象指针
 * @param timing 时间参数结构体
 */
void key_set_timing(key_obj *key, key_timing_config_t *timing);

/**
 * @brief 快速设置按键时间参数（简化版）
 * @param key 按键对象指针
 * @param debounce_ms 消抖时间（毫秒）
 * @param long_press_ms 长按阈值时间（毫秒）
 * @param double_click_interval_ms 双击间隔时间（毫秒）
 * @param long_press_continue_interval_ms 长按持续触发间隔（毫秒，0表示禁用）
 */
void key_set_timing_simple(key_obj *key, 
                          uint16_t debounce_ms, 
                          uint16_t long_press_ms, 
                          uint16_t double_click_interval_ms,
                          uint16_t long_press_continue_interval_ms);

/**
 * @brief 按键事件检测函数（需周期性调用）
 * @param key 按键对象指针
 * @return 检测到的事件类型
 */
key_event_t key_event_check(key_obj *key);

/**
 * @brief 检查按键是否处于按下状态
 * @param key 按键对象
 * @return 1=按下，0=释放
 */
uint8_t key_is_pressed(key_obj *key);

/**
 * @brief 检查按键是否处于长按状态
 * @param key 按键对象
 * @return 1=长按，0=非长按
 */
uint8_t key_is_long_pressed(key_obj *key);

#endif