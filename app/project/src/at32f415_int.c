/* add user code begin Header */
/**
  **************************************************************************
  * @file     at32f415_int.c
  * @brief    main interrupt service routines.
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
/* add user code end Header */

/* includes ------------------------------------------------------------------*/
#include "at32f415_int.h"
#include "wk_system.h"
/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include <math.h>
#include "iap_usart.h"
#include "tmt.h"
#include "FWG2_handle.h"
#include "output_handle.h"
#include "beep_handle.h"
#include "at32_Usart.h"
#include "PID_operation.h"
/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */
#define    TIME_1S        1000
#define    TIME_500MS     500
#define    TIME_1MS       1

#define ADC_VREF                         (3.29)
#define ADC_TEMP_BASE                    (1.26)
#define ADC_TEMP_SLOPE                   (-0.00423)


__IO uint16_t adc1_ordinary_value = 0;
/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */
__IO uint32_t tmr3_ch1_freq = 0;
__IO uint32_t tmr3_ch1_readvalue = 0;


/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */
static void Direct_Handle_PWM_Out(void);
static void code_mode_time(void);
static void enhance_mode_countdown(void);
static void count_down(void);

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

/* add user code end 0 */

/* external variables ---------------------------------------------------------*/
/* add user code begin external variables */

/* add user code end external variables */

/**
  * @brief  this function handles nmi exception.
  * @param  none
  * @retval none
  */
void NMI_Handler(void)
{
  /* add user code begin NonMaskableInt_IRQ 0 */
    /* add user code end NonMaskableInt_IRQ 0 */

  /* add user code begin NonMaskableInt_IRQ 1 */
    /* add user code end NonMaskableInt_IRQ 1 */
}

/**
  * @brief  this function handles hard fault exception.
  * @param  none
  * @retval none
  */
void HardFault_Handler(void)
{
  /* add user code begin HardFault_IRQ 0 */

    /* add user code end HardFault_IRQ 0 */
  /* go to infinite loop when hard fault exception occurs */
  while (1)
  {
    /* add user code begin W1_HardFault_IRQ 0 */
        /* add user code end W1_HardFault_IRQ 0 */
  }
}

/**
  * @brief  this function handles memory manage exception.
  * @param  none
  * @retval none
  */
void MemManage_Handler(void)
{
  /* add user code begin MemoryManagement_IRQ 0 */

    /* add user code end MemoryManagement_IRQ 0 */
  /* go to infinite loop when memory manage exception occurs */
  while (1)
  {
    /* add user code begin W1_MemoryManagement_IRQ 0 */
        /* add user code end W1_MemoryManagement_IRQ 0 */
  }
}

/**
  * @brief  this function handles bus fault exception.
  * @param  none
  * @retval none
  */
void BusFault_Handler(void)
{
  /* add user code begin BusFault_IRQ 0 */

    /* add user code end BusFault_IRQ 0 */
  /* go to infinite loop when bus fault exception occurs */
  while (1)
  {
    /* add user code begin W1_BusFault_IRQ 0 */
        /* add user code end W1_BusFault_IRQ 0 */
  }
}

/**
  * @brief  this function handles usage fault exception.
  * @param  none
  * @retval none
  */
void UsageFault_Handler(void)
{
  /* add user code begin UsageFault_IRQ 0 */

    /* add user code end UsageFault_IRQ 0 */
  /* go to infinite loop when usage fault exception occurs */
  while (1)
  {
    /* add user code begin W1_UsageFault_IRQ 0 */
        /* add user code end W1_UsageFault_IRQ 0 */
  }
}

/**
  * @brief  this function handles svcall exception.
  * @param  none
  * @retval none
  */
void SVC_Handler(void)
{
  /* add user code begin SVCall_IRQ 0 */
    /* add user code end SVCall_IRQ 0 */
  /* add user code begin SVCall_IRQ 1 */
    /* add user code end SVCall_IRQ 1 */
}

/**
  * @brief  this function handles debug monitor exception.
  * @param  none
  * @retval none
  */
void DebugMon_Handler(void)
{
  /* add user code begin DebugMonitor_IRQ 0 */
    /* add user code end DebugMonitor_IRQ 0 */
  /* add user code begin DebugMonitor_IRQ 1 */
    /* add user code end DebugMonitor_IRQ 1 */
}

/**
  * @brief  this function handles pendsv_handler exception.
  * @param  none
  * @retval none
  */
void PendSV_Handler(void)
{
  /* add user code begin PendSV_IRQ 0 */
    /* add user code end PendSV_IRQ 0 */
  /* add user code begin PendSV_IRQ 1 */
    /* add user code end PendSV_IRQ 1 */
}

/**
  * @brief  this function handles systick handler.
  * @param  none
  * @retval none
  */
void SysTick_Handler(void)
{
  /* add user code begin SysTick_IRQ 0 */
    static bool first_in = false;
    static uint16_t time_out = TIME_1S;
    time_out--;

    /*start the get fan speed of*/
    if (!time_out)
    {
        sFWG2_t.Direct_handle_parameter.actual_wind = tmr3_ch1_readvalue;
        tmr3_ch1_readvalue = 0;
        time_out = TIME_1S;
    }

    /* get mcu temp */
    if (first_in == false && sFWG2_t.FWG2_STATE == FWG2_WORKING)
    {
        sFWG2_t.Direct_handle_parameter.actual_temp  = (get_adcval(ADC_CHANNEL_10) >> 2);
        adc1_ordinary_value = get_adcval(ADC_CHANNEL_16);
        sFWG2_t.general_parameter.mcu_temp = (ADC_TEMP_BASE - (double)adc1_ordinary_value * ADC_VREF / 4096) / ADC_TEMP_SLOPE +
                                             15;

        if (sFWG2_t.general_parameter.mcu_temp <= 50 && sFWG2_t.general_parameter.mcu_temp > 0
                && sFWG2_t.Direct_handle_parameter.actual_temp < 500)
        {
            first_in = true;
        }
    }

    /*end the get fan speed of*/
    enhance_mode_countdown();
    code_mode_time();
    count_down();
    tmt.tick();
    USART1_TimeOutCounter();
    USART2_TimeOutCounter();
    USART3_TimeOutCounter();
    UART4_TimeOutCounter();
    UART5_TimeOutCounter();
    /* add user code end SysTick_IRQ 0 */

  wk_timebase_handler();
  /* add user code begin SysTick_IRQ 1 */
    /* add user code end SysTick_IRQ 1 */
}

/**
  * @brief  this function handles TMR2 handler.
  * @param  none
  * @retval none
  */
void TMR2_GLOBAL_IRQHandler(void)
{
  /* add user code begin TMR2_GLOBAL_IRQ 0 */
    if (tmr_flag_get(TMR2, TMR_OVF_FLAG) != RESET)
    {
        if (sFWG2_t.FWG2_STATE == FWG2_WORKING)
        {
            /* PWMÊä³ö */
            Direct_Handle_PWM_Out();
			//sFWG2_t.general_parameter.pid_out = 0;
			//sFWG2_t.Direct_handle_parameter.error_time = 0;
        }

        if (sFWG2_t.general_parameter.pid_out <= 0)
        {
            sFWG2_t.general_parameter.pid_out = 0;
        }

        tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_1, (uint32_t)sFWG2_t.general_parameter.pid_out);
        tmr_counter_enable(TMR2, TRUE);
        tmr_flag_clear(TMR2, TMR_OVF_FLAG);
    }

    /* add user code end TMR2_GLOBAL_IRQ 0 */

  /* add user code begin TMR2_GLOBAL_IRQ 1 */
    /* add user code end TMR2_GLOBAL_IRQ 1 */
}

/**
  * @brief  this function handles TMR3 handler.
  * @param  none
  * @retval none
  */
void TMR3_GLOBAL_IRQHandler(void)
{
  /* add user code begin TMR3_GLOBAL_IRQ 0 */
    if (tmr_interrupt_flag_get(TMR3, TMR_C1_FLAG) != RESET)
    {
        tmr3_ch1_readvalue++;
        tmr_flag_clear(TMR3, TMR_C1_FLAG);
    }

    /* add user code end TMR3_GLOBAL_IRQ 0 */

  /* add user code begin TMR3_GLOBAL_IRQ 1 */
    /* add user code end TMR3_GLOBAL_IRQ 1 */
}
 

/* add user code begin 1 */
static void Direct_Handle_PWM_Out(void)
{
    static uint16_t Direct_handle_last_set_temp = 0;
    static fwg2_code_mode_step_e last_code_mode_step = CODE_WAIT;
    static uint16_t direct_hot_run_time_ms = 0;
    static float hight_kd = 52000;
    static uint32_t low_kd = 10000;
	
	static float hight_ki = 4;
    static float low_ki = 2;
	 
    static int16_t set_temp;
    static int16_t actual_temp;
    static uint16_t delay_time = 0;
    static bool change_flag = false;

    if (change_flag)
    {
        if (direct_pid.Kd <= low_kd)
        {
            direct_pid.Kd = low_kd;
        }
		
		if (direct_pid.Ki <= low_ki)
        {
            direct_pid.Ki = low_ki;
        }
		
    }
    else
    {
        direct_pid.Kd = hight_kd;
		direct_pid.Ki = hight_ki;
    }

    if (direct_hot_run_time_ms >= 12000)
    {
        direct_hot_run_time_ms = 0;
        sFWG2_t.general_parameter.direct_hot_work_time_m ++;
    }

    /* get set temp */
    if (sFWG2_t.general_parameter.work_mode == NORMAL)
    {
        if (sFWG2_t.general_parameter.enhance_state == ENHANCE_OPEN)
        {
            if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
            {
                set_temp = sFWG2_t.Direct_handle_parameter.set_temp + ENHANCE_TEMP;
            }
            else if (sFWG2_t.Direct_handle_work_mode == QUICK_MODE)
            {
                set_temp = sFWG2_t.Direct_handle_parameter.set_temp +
                           ENHANCE_TEMP +
                           sFWG2_t.Direct_handle_parameter.quick_work_temp;
            }
        }
        else if (sFWG2_t.general_parameter.enhance_state == ENHANCE_CLOSE)
        {
            if (sFWG2_t.Direct_handle_work_mode == NORMAL_MODE)
            {
                set_temp = sFWG2_t.Direct_handle_parameter.set_temp;
            }
            else if (sFWG2_t.Direct_handle_work_mode == QUICK_MODE)
            {
                set_temp = sFWG2_t.Direct_handle_parameter.set_temp +
                           sFWG2_t.Direct_handle_parameter.quick_work_temp;
            }
        }
    }
    else if (sFWG2_t.general_parameter.work_mode == CODE)
    {
        set_temp = sFWG2_t.Direct_handle_parameter.set_temp;
    }

    /* normal mode pid control */
    if (sFWG2_t.general_parameter.work_mode == NORMAL)
    {
        if (sFWG2_t.Direct_handle_position == NOT_IN_POSSITION || sFWG2_t.general_parameter.fwg2_sleep_state == SLEEP_CLOSE)
        {
            if (sFWG2_t.general_parameter.relay_open_flag == true)
            {
                /* get actual temp */
                sFWG2_t.Direct_handle_parameter.actual_temp = temp_get_filter_move_average(ADC_CHANNEL);
				 actual_temp = sFWG2_t.Direct_handle_parameter.actual_temp +
                              sFWG2_t.Direct_handle_parameter.linear_calibration_temp -
                              sFWG2_t.Direct_handle_parameter.set_calibration_temp;

                if (Direct_handle_last_set_temp != sFWG2_t.Direct_handle_parameter.set_temp)
                {
                    sFWG2_t.Direct_handle_parameter.linear_calibration_temp = direct_linear_correction(set_temp);
                    PID_Clear_I(&direct_pid);
					//direct_pid.Ki = direct_pid.Ki / 2;
                    Direct_handle_last_set_temp = sFWG2_t.Direct_handle_parameter.set_temp;
                }

                /*  when the actual temp was betwen the set temp ¡À100 than will run pid funtion*/
                if (actual_temp <= set_temp + PID_RANGE &&
                        actual_temp >= set_temp - PID_RANGE)
                {
					
					
                    if (actual_temp <= (set_temp + 5) && actual_temp >= (set_temp - 5))
                    {
                        delay_time++;

                        if (delay_time >= 60)
                        {
                            delay_time = 0;
                            change_flag = true;

                            if (direct_pid.Kd - 2000 <= low_kd)
                            {
                                direct_pid.Kd = low_kd;
                            }
                            else
                            {
                                direct_pid.Kd -= 2000;
                            }
							
							if (direct_pid.Ki - 0.1 <= low_ki)
                            {
                                direct_pid.Ki = low_ki;
                            }
                            else
                            {
                                direct_pid.Ki -= 0.1;
                            }
							
							
                        }
                    }
                    else
                    {
                        change_flag = false;
                        delay_time = 0;
                    }

                    /* run pid funtion */
                    sFWG2_t.general_parameter.pid_out = PID_Position_Calc(&direct_pid, set_temp,
                                                        actual_temp);
                    direct_hot_run_time_ms++;
                }
                /*  when the actual temp over than set temp 100 ,stop pid funtion*/
                else if (actual_temp > set_temp + PID_RANGE)
                {
                    sFWG2_t.general_parameter.pid_out = 0;
                    direct_hot_run_time_ms++;
                }
                else if (actual_temp < set_temp - PID_RANGE)
                {
                    sFWG2_t.general_parameter.pid_out = MAX_PWM_OUTPUT;
                    direct_hot_run_time_ms++;
                }

                if (sFWG2_t.general_parameter.pid_out <= 0)
                {
                    sFWG2_t.general_parameter.pid_out = 0;
                }
            }
        }
        else if (sFWG2_t.Direct_handle_position == IN_POSSITION && sFWG2_t.general_parameter.fwg2_sleep_state == SLEEP_OPEN)
        {
            /* get actual temp */
            sFWG2_t.Direct_handle_parameter.actual_temp = temp_get_filter_move_average(ADC_CHANNEL);
            actual_temp = sFWG2_t.Direct_handle_parameter.actual_temp +
                          sFWG2_t.Direct_handle_parameter.linear_calibration_temp -
                          sFWG2_t.Direct_handle_parameter.set_calibration_temp;
            sFWG2_t.general_parameter.pid_out = 0;
            PID_Clear(&direct_pid);
        }

#if 1

        if (sFWG2_t.Direct_handle_work_mode == COLD_WIND_MODE)
        {
            /* get actual temp */
            sFWG2_t.Direct_handle_parameter.actual_temp = temp_get_filter_move_average(ADC_CHANNEL);
            actual_temp = sFWG2_t.Direct_handle_parameter.actual_temp +
                          sFWG2_t.Direct_handle_parameter.linear_calibration_temp -
                          sFWG2_t.Direct_handle_parameter.set_calibration_temp;
            sFWG2_t.general_parameter.pid_out = 0;
            PID_Clear(&direct_pid);
        }

#endif
    }

#if 1
    /* code mode control pid */
    else if (sFWG2_t.general_parameter.work_mode == CODE)
    {
        /* direct handle control begin */
        if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
        {
            if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_START)
            {
                if (last_code_mode_step != sFWG2_t.general_parameter.code_mode_step)
                {
                    last_code_mode_step = sFWG2_t.general_parameter.code_mode_step;
                    PID_Clear(&direct_pid);
                }

                if (sFWG2_t.general_parameter.relay_open_flag == true)
                {
                    direct_pid.Kp = 1000;
                    direct_pid.Ki = 0.6;
                    direct_pid.Kd = 22000;
                    /* get actual temp */
                    sFWG2_t.Direct_handle_parameter.actual_temp = temp_get_filter_move_average(ADC_CHANNEL);
                    actual_temp = sFWG2_t.Direct_handle_parameter.actual_temp +
                                  sFWG2_t.Direct_handle_parameter.linear_calibration_temp -
                                  sFWG2_t.Direct_handle_parameter.set_calibration_temp;

                    /* run pid funtion */
                    if (sFWG2_t.general_parameter.code_ch == 0)
                    {
                        if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
                        {
                            sFWG2_t.Direct_handle_parameter.linear_calibration_temp = direct_linear_correction(
                                    sFWG2_t.general_parameter.code0_pre_temp);
                            sFWG2_t.general_parameter.pid_out = PID_Position_Calc(&direct_pid, sFWG2_t.general_parameter.code0_pre_temp,
                                                                actual_temp);
                            direct_hot_run_time_ms++;
                        }
                        else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
                        {
                            sFWG2_t.Direct_handle_parameter.linear_calibration_temp = direct_linear_correction(
                                    sFWG2_t.general_parameter.code0_temp_1);
                            sFWG2_t.general_parameter.pid_out = PID_Position_Calc(&direct_pid, sFWG2_t.general_parameter.code0_temp_1,
                                                                actual_temp);
                            direct_hot_run_time_ms++;
                        }
                        else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
                        {
                            sFWG2_t.Direct_handle_parameter.linear_calibration_temp = direct_linear_correction(
                                    sFWG2_t.general_parameter.code0_temp_2);
                            sFWG2_t.general_parameter.pid_out = PID_Position_Calc(&direct_pid, sFWG2_t.general_parameter.code0_temp_2,
                                                                actual_temp);
                            direct_hot_run_time_ms++;
                        }
                        else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
                        {
                            sFWG2_t.Direct_handle_parameter.linear_calibration_temp = direct_linear_correction(
                                    sFWG2_t.general_parameter.code0_temp_3);
                            sFWG2_t.general_parameter.pid_out = PID_Position_Calc(&direct_pid, sFWG2_t.general_parameter.code0_temp_3,
                                                                actual_temp);
                            direct_hot_run_time_ms++;
                        }
                        else if (sFWG2_t.general_parameter.code_mode_step == CODE_END)
                        {
                            sFWG2_t.general_parameter.code_mode_state = CODE_MODE_STOP;
                            sFWG2_t.general_parameter.code_mode_step = CODE_WAIT;
                        }
                    }
                    else if (sFWG2_t.general_parameter.code_ch == 1)
                    {
                        if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
                        {
                            sFWG2_t.Direct_handle_parameter.linear_calibration_temp = direct_linear_correction(
                                    sFWG2_t.general_parameter.code1_pre_temp);
                            sFWG2_t.general_parameter.pid_out = PID_Position_Calc(&direct_pid, sFWG2_t.general_parameter.code1_pre_temp,
                                                                actual_temp);
                            direct_hot_run_time_ms++;
                        }
                        else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
                        {
                            sFWG2_t.Direct_handle_parameter.linear_calibration_temp = direct_linear_correction(
                                    sFWG2_t.general_parameter.code1_temp_1);
                            sFWG2_t.general_parameter.pid_out = PID_Position_Calc(&direct_pid, sFWG2_t.general_parameter.code1_temp_1,
                                                                actual_temp);
                            direct_hot_run_time_ms++;
                        }
                        else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
                        {
                            sFWG2_t.Direct_handle_parameter.linear_calibration_temp = direct_linear_correction(
                                    sFWG2_t.general_parameter.code1_temp_2);
                            sFWG2_t.general_parameter.pid_out = PID_Position_Calc(&direct_pid, sFWG2_t.general_parameter.code1_temp_2,
                                                                actual_temp);
                            direct_hot_run_time_ms++;
                        }
                        else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
                        {
                            sFWG2_t.Direct_handle_parameter.linear_calibration_temp = direct_linear_correction(
                                    sFWG2_t.general_parameter.code1_temp_3);
                            sFWG2_t.general_parameter.pid_out = PID_Position_Calc(&direct_pid, sFWG2_t.general_parameter.code1_temp_3,
                                                                actual_temp);
                            direct_hot_run_time_ms++;
                        }
                        else if (sFWG2_t.general_parameter.code_mode_step == CODE_END)
                        {
                            sFWG2_t.general_parameter.code_mode_state = CODE_MODE_STOP;
                            sFWG2_t.general_parameter.code_mode_step = CODE_WAIT;
                        }
                    }
                    else if (sFWG2_t.general_parameter.code_ch == 2)
                    {
                        if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
                        {
                            sFWG2_t.Direct_handle_parameter.linear_calibration_temp = direct_linear_correction(
                                    sFWG2_t.general_parameter.code2_pre_temp);
                            sFWG2_t.general_parameter.pid_out = PID_Position_Calc(&direct_pid, sFWG2_t.general_parameter.code2_pre_temp,
                                                                actual_temp);
                            direct_hot_run_time_ms++;
                        }
                        else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
                        {
                            sFWG2_t.Direct_handle_parameter.linear_calibration_temp = direct_linear_correction(
                                    sFWG2_t.general_parameter.code2_temp_1);
                            sFWG2_t.general_parameter.pid_out = PID_Position_Calc(&direct_pid, sFWG2_t.general_parameter.code2_temp_1,
                                                                actual_temp);
                            direct_hot_run_time_ms++;
                        }
                        else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
                        {
                            sFWG2_t.Direct_handle_parameter.linear_calibration_temp = direct_linear_correction(
                                    sFWG2_t.general_parameter.code2_temp_2);
                            sFWG2_t.general_parameter.pid_out = PID_Position_Calc(&direct_pid, sFWG2_t.general_parameter.code2_temp_2,
                                                                actual_temp);
                            direct_hot_run_time_ms++;
                        }
                        else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
                        {
                            sFWG2_t.Direct_handle_parameter.linear_calibration_temp = direct_linear_correction(
                                    sFWG2_t.general_parameter.code2_temp_3);
                            sFWG2_t.general_parameter.pid_out = PID_Position_Calc(&direct_pid, sFWG2_t.general_parameter.code2_temp_3,
                                                                actual_temp);
                            direct_hot_run_time_ms++;
                        }
                        else if (sFWG2_t.general_parameter.code_mode_step == CODE_END)
                        {
                            sFWG2_t.general_parameter.code_mode_state = CODE_MODE_STOP;
                            sFWG2_t.general_parameter.code_mode_step = CODE_WAIT;
                        }
                    }
                    else if (sFWG2_t.general_parameter.code_ch == 3)
                    {
                        if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
                        {
                            sFWG2_t.Direct_handle_parameter.linear_calibration_temp = direct_linear_correction(
                                    sFWG2_t.general_parameter.code3_pre_temp);
                            sFWG2_t.general_parameter.pid_out = PID_Position_Calc(&direct_pid, sFWG2_t.general_parameter.code3_pre_temp,
                                                                actual_temp);
                            direct_hot_run_time_ms++;
                        }
                        else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
                        {
                            sFWG2_t.Direct_handle_parameter.linear_calibration_temp = direct_linear_correction(
                                    sFWG2_t.general_parameter.code3_temp_1);
                            sFWG2_t.general_parameter.pid_out = PID_Position_Calc(&direct_pid, sFWG2_t.general_parameter.code3_temp_1,
                                                                actual_temp);
                            direct_hot_run_time_ms++;
                        }
                        else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
                        {
                            sFWG2_t.Direct_handle_parameter.linear_calibration_temp = direct_linear_correction(
                                    sFWG2_t.general_parameter.code3_temp_2);
                            sFWG2_t.general_parameter.pid_out = PID_Position_Calc(&direct_pid, sFWG2_t.general_parameter.code3_temp_2,
                                                                actual_temp);
                            direct_hot_run_time_ms++;
                        }
                        else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
                        {
                            sFWG2_t.Direct_handle_parameter.linear_calibration_temp = direct_linear_correction(
                                    sFWG2_t.general_parameter.code3_temp_3);
                            sFWG2_t.general_parameter.pid_out = PID_Position_Calc(&direct_pid, sFWG2_t.general_parameter.code3_temp_3,
                                                                actual_temp);
                            direct_hot_run_time_ms++;
                        }
                        else if (sFWG2_t.general_parameter.code_mode_step == CODE_END)
                        {
                            sFWG2_t.general_parameter.code_mode_state = CODE_MODE_STOP;
                            sFWG2_t.general_parameter.code_mode_step = CODE_WAIT;
                        }
                    }

                    if (sFWG2_t.general_parameter.pid_out <= 0)
                    {
                        sFWG2_t.general_parameter.pid_out = 0;
                    }

                    direct_pid.Kd = hight_kd;
                }
            }
            else if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_STOP)
            {
                /* get actual temp */
                sFWG2_t.Direct_handle_parameter.actual_temp = temp_get_filter_move_average(ADC_CHANNEL);
                actual_temp = sFWG2_t.Direct_handle_parameter.actual_temp +
                              sFWG2_t.Direct_handle_parameter.linear_calibration_temp -
                              sFWG2_t.Direct_handle_parameter.set_calibration_temp;
                 
                PID_Clear(&direct_pid);
                sFWG2_t.general_parameter.pid_out = 0;
            }
        }

        /* direct handle control end */
    }

#endif

    if (sFWG2_t.Direct_handle_error_state != HANDLE_OK)
    {
        sFWG2_t.general_parameter.pid_out = 0;
    }
}



static void code_mode_time(void)
{
    static bool first_in = false;
    static uint16_t code_mode_countdown_time = TIME_1S;

    /* start of code mode countdown */
    if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_START)
    {
        if (first_in == false)
        {
            first_in = true;

            if (sFWG2_t.general_parameter.code_ch == 0)
            {
                if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
                {
                    sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code0_pre_time;
                }
                else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
                {
                    sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code0_time_1;
                }
                else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
                {
                    sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code0_time_2;
                }
                else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
                {
                    sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code0_time_3;
                }
            }
            else if (sFWG2_t.general_parameter.code_ch == 1)
            {
                if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
                {
                    sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code1_pre_time;
                }
                else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
                {
                    sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code1_time_1;
                }
                else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
                {
                    sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code1_time_2;
                }
                else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
                {
                    sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code1_time_3;
                }
            }
            else if (sFWG2_t.general_parameter.code_ch == 2)
            {
                if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
                {
                    sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code2_pre_time;
                }
                else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
                {
                    sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code2_time_1;
                }
                else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
                {
                    sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code2_time_2;
                }
                else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
                {
                    sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code2_time_3;
                }
            }
            else if (sFWG2_t.general_parameter.code_ch == 3)
            {
                if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
                {
                    sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code3_pre_time;
                }
                else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
                {
                    sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code3_time_1;
                }
                else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
                {
                    sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code3_time_2;
                }
                else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
                {
                    sFWG2_t.general_parameter.code_mode_countdown_time_display = sFWG2_t.general_parameter.code3_time_3;
                }
            }
        }

        /* start counting time */
        if (sFWG2_t.general_parameter.code_ch == 0)
        {
            if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                {
                    if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                    {
                        if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                        {
                            if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code0_pre_temp - 10) && \
                                    sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code0_pre_temp + 10))
                            {
                                code_mode_countdown_time--;
                            }
                            else
                            {
                                code_mode_countdown_time = TIME_1S;
                            }
                        }
                        else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                        {
                            if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code0_pre_temp_f_display - 18) && \
                                    sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code0_pre_temp_f_display + 18))
                            {
                                code_mode_countdown_time--;
                            }
                            else
                            {
                                code_mode_countdown_time = TIME_1S;
                            }
                        }
                    }
                }
            }
            else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                {
                    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code0_temp_1 - 10) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code0_temp_1 + 10))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code0_temp_1_f_display - 18) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code0_temp_1_f_display + 18))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                }
            }
            else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                {
                    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code0_temp_2 - 10) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code0_temp_2 + 10))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code0_temp_2_f_display - 18) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code0_temp_2_f_display + 18))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                }
            }
            else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                {
                    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code0_temp_3 - 10) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code0_temp_3 + 10))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code0_temp_3_f_display - 18) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code0_temp_3_f_display + 18))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                }
            }
        }
        else if (sFWG2_t.general_parameter.code_ch == 1)
        {
            if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                {
                    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code1_pre_temp - 10) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code1_pre_temp + 10))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code1_pre_temp_f_display - 18) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code1_pre_temp_f_display + 18))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                }
            }
            else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                {
                    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code1_temp_1 - 10) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code1_temp_1 + 10))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code1_temp_1_f_display - 18) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code1_temp_1_f_display + 18))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                }
            }
            else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                {
                    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code1_temp_2 - 10) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code1_temp_2 + 10))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code1_temp_2_f_display - 18) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code1_temp_2_f_display + 18))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                }
            }
            else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                {
                    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code1_temp_3 - 10) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code1_temp_3 + 10))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code1_temp_3_f_display - 18) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code1_temp_3_f_display + 18))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                }
            }
        }
        else if (sFWG2_t.general_parameter.code_ch == 2)
        {
            if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                {
                    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code2_pre_temp - 10) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code2_pre_temp + 10))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code2_pre_temp_f_display - 18) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code2_pre_temp_f_display + 18))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                }
            }
            else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                {
                    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code2_temp_1 - 10) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code2_temp_1 + 10))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code2_temp_1_f_display - 18) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code2_temp_1_f_display + 18))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                }
            }
            else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                {
                    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code2_temp_2 - 10) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code2_temp_2 + 10))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code2_temp_2_f_display - 18) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code2_temp_2_f_display + 18))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                }
            }
            else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                {
                    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code2_temp_3 - 10) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code2_temp_3 + 10))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code2_temp_3_f_display - 18) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code2_temp_3_f_display + 18))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                }
            }
        }
        else if (sFWG2_t.general_parameter.code_ch == 3)
        {
            if (sFWG2_t.general_parameter.code_mode_step == CODE_PRE_HEAT)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                {
                    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code3_pre_temp - 10) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code3_pre_temp + 10))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code3_pre_temp_f_display - 18) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code3_pre_temp_f_display + 18))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                }
            }
            else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_1)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                {
                    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code3_temp_1 - 10) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code3_temp_1 + 10))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code3_temp_1_f_display - 18) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code3_temp_1_f_display + 18))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                }
            }
            else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_2)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                {
                    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code3_temp_2 - 10) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code3_temp_2 + 10))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code3_temp_2_f_display - 18) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code3_temp_2_f_display + 18))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                }
            }
            else if (sFWG2_t.general_parameter.code_mode_step == CODE_STEUP_3)
            {
                if (sFWG2_t.general_parameter.code_mode_handle_select == SELECT_DIRECT_HANDLE)
                {
                    if (sFWG2_t.general_parameter.temp_uint == CELSIUS)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code3_temp_3 - 10) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code3_temp_3 + 10))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                    else if (sFWG2_t.general_parameter.temp_uint == FAHRENHEIT)
                    {
                        if (sFWG2_t.Direct_handle_parameter.show_temp > (sFWG2_t.general_parameter.code3_temp_3_f_display - 18) && \
                                sFWG2_t.Direct_handle_parameter.show_temp < (sFWG2_t.general_parameter.code3_temp_3_f_display + 18))
                        {
                            code_mode_countdown_time--;
                        }
                        else
                        {
                            code_mode_countdown_time = TIME_1S;
                        }
                    }
                }
            }
        }

        if (!code_mode_countdown_time)
        {
            code_mode_countdown_time = TIME_1S;
            sFWG2_t.general_parameter.code_mode_countdown_time_display--;

            if (sFWG2_t.general_parameter.code_mode_countdown_time_display <= -1)
            {
                first_in = false;
                sFWG2_t.general_parameter.code_mode_step ++;

                if (sFWG2_t.general_parameter.code_mode_step >= CODE_END)
                {
                    sFWG2_t.general_parameter.code_mode_countdown_time_display = 0;
                    sFWG2_t.general_parameter.code_mode_step = CODE_WAIT;
                    sFWG2_t.general_parameter.code_mode_state = CODE_MODE_STOP;
                }
            }
        }
    }
    else if (sFWG2_t.general_parameter.code_mode_state == CODE_MODE_STOP)
    {
        first_in = false;
        code_mode_countdown_time = TIME_1S;
        sFWG2_t.general_parameter.code_mode_step = CODE_WAIT;
        sFWG2_t.general_parameter.code_mode_countdown_time_display = 0;
    }

    /* end of code mode countdown */
}

void count_down(void)
{
#if 1
    static uint16_t countdown_time = TIME_1S;

    /* start of countdown */
    if (sFWG2_t.general_parameter.countdown_flag)
    {
        if (sFWG2_t.Direct_handle_position == IN_POSSITION)
        {
            sFWG2_t.general_parameter.countdown_flag = false;
            sbeep.status = BEEP_LONG;
        }

        countdown_time--;

        if (!countdown_time)
        {
            sFWG2_t.general_parameter.countdown_time_display--;

            if (sFWG2_t.general_parameter.countdown_time_display <= -1)
            {
                sFWG2_t.general_parameter.countdown_time_display = sFWG2_t.general_parameter.countdown_time;
                sFWG2_t.general_parameter.countdown_flag = false;
                sbeep.status = BEEP_LONG;
            }

            countdown_time = TIME_1S;
        }
    }
    else
    {
        countdown_time = TIME_1S;
        sFWG2_t.general_parameter.countdown_time_display =  sFWG2_t.general_parameter.countdown_time;
    }/* end of countdown */

#endif
}

void enhance_mode_countdown(void)
{
#if 1
    static uint16_t direct_handle_enhance_countdown_time = TIME_1S;

    /* start of direct handle's enhance mode countdown */
    if (sFWG2_t.Direct_handle_work_mode == QUICK_MODE)
    {
        direct_handle_enhance_countdown_time--;

        if (!direct_handle_enhance_countdown_time)
        {
            direct_handle_enhance_countdown_time = TIME_1S;
            sFWG2_t.Direct_handle_parameter.quick_work_time_display --;

            if (sFWG2_t.Direct_handle_parameter.quick_work_time_display <= -1)
            {
                sFWG2_t.Direct_handle_work_mode = NORMAL_MODE;
                sbeep.status = BEEP_LONG;
            }
        }
    }
    else
    {
        direct_handle_enhance_countdown_time = TIME_1S;
        sFWG2_t.Direct_handle_parameter.quick_work_time_display =  sFWG2_t.Direct_handle_parameter.quick_work_time;
    }

#endif
}

/* add user code end 1 */
