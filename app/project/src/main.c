/* add user code begin Header */
/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
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

/* Includes ------------------------------------------------------------------*/
#include "at32f415_wk_config.h"
#include "wk_adc.h"
#include "wk_debug.h"
#include "wk_exint.h"
#include "wk_spi.h"
#include "wk_tmr.h"
#include "wk_usart.h"
#include "wk_dma.h"
#include "wk_gpio.h"
#include "wk_system.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "tmt.h"
#include "EventRecorder.h"
#include "iap.h"
#include "FWG2_handle.h"
#include "output_handle.h"
#include "beep_handle.h"
#include "flash_handle.h"
#include "key_handle.h"
#include "dwin_handle.h"
#include "work_handle.h"
#include "output_handle.h"
#include "at32_spiflash.h"
#include "PID_operation.h"
/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */

/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */
uint8_t beep_flag = 0;


/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */
void iap_task(void);
void key_task(void);
void adc_task(void);
void beep_task(void);
void dwin_task(void);
void work_task(void);
void flash_task(void);
void output_task(void);
/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

/* add user code end 0 */

/**
  * @brief main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  /* add user code begin 1 */
  /* config vector table offset */
  nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x4000);
  /* add user code end 1 */

  /* system clock config. */
  wk_system_clock_config();

  /* config periph clock. */
  wk_periph_clock_config();

  /* init debug function. */
  wk_debug_config();

  /* nvic config. */
  wk_nvic_config();

  /* timebase config. */
  wk_timebase_init();

  /* init gpio function. */
  wk_gpio_config();

  /* init dma1 channel1 */
  wk_dma1_channel1_init();
  /* config dma channel transfer parameter */
  /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
  wk_dma_channel_config(DMA1_CHANNEL1, 
                        DMA1_CHANNEL1_PERIPHERAL_BASE_ADDR, 
                        DMA1_CHANNEL1_MEMORY_BASE_ADDR, 
                        DMA1_CHANNEL1_BUFFER_SIZE);
  dma_channel_enable(DMA1_CHANNEL1, TRUE);

  /* init usart1 function. */
  wk_usart1_init();

  /* init usart2 function. */
  wk_usart2_init();

  /* init usart3 function. */
  wk_usart3_init();

  /* init uart4 function. */
  wk_uart4_init();

  /* init uart5 function. */
  wk_uart5_init();

  /* init spi1 function. */
  wk_spi1_init();

  /* init adc1 function. */
  wk_adc1_init();

  /* init exint function. */
  wk_exint_config();

  /* init tmr2 function. */
  wk_tmr2_init();

  /* init tmr3 function. */
  wk_tmr3_init();

  /* init tmr9 function. */
  wk_tmr9_init();

  /* add user code begin 2 */
    tmt_init();
    tmt.create(iap_task, IAP_HANDLE_TIME);
    tmt.create(key_task, KEY_HANDLE_TIME);
    tmt.create(adc_task, KEY_HANDLE_TIME);
    tmt.create(beep_task, BEEP_HANDLE_TIME);
    tmt.create(work_task, WORK_HANDLE_TIME);
    tmt.create(flash_task, FLASH_HANDLE_TIME);
    tmt.create(output_task, OUTPUT_HANDLE_TIME);
	tmt.create(dwin_task, DWIN_HANDLE_TIME);
    EventRecorderInitialize(0, 1);
    FWG2_Init(&sFWG2_t);
    DwinInitialization(&sdwin);
    PID_Init(&direct_pid, 200, 0.5, 0.5, 59999);
    iap_init();
    spiflash_init();
    __IO uint32_t flash_id_index  = spiflash_read_id();
    BSP_UsartInit();
    printf("system init ok\r\n");
    /* wait for system reday */
    wk_delay_ms(3000);
    /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */
    tmt.run();
    /* add user code end 3 */
  }
}

  /* add user code begin 4 */
void iap_task(void)
{
    iap_command_handle();
}

void key_task(void)
{
    key_handle();
}

void beep_task()
{
    if (sFWG2_t.general_parameter.speak_state == SPEAKER_OPEN)
    {
        BeepProc(&sbeep);
    }
    else
    {
        sbeep.off();
        sbeep.status = BEEP_OFF;
    }
}

void adc_task(void)
{
    static bool first_in = false;
#if 0
    sFWG2_t.Direct_handle_parameter.actual_temp  = (get_adcval(ADC_CHANNEL_10) >> 2) + sFWG2_t.general_parameter.mcu_temp;
#endif

    if (first_in == false)
    {
		sFWG2_t.Direct_handle_parameter.actual_temp  = (get_adcval(ADC_CHANNEL_10) >> 2);
        sFWG2_t.general_parameter.mcu_temp =  get_adcval(ADC_CHANNEL_16);
        sFWG2_t.general_parameter.mcu_temp = (1.26 - ((double)sFWG2_t.general_parameter.mcu_temp * 3.3 / 4096)) / (-0.00423);
        if (sFWG2_t.general_parameter.mcu_temp <= 50 && sFWG2_t.general_parameter.mcu_temp > 0 && sFWG2_t.Direct_handle_parameter.actual_temp<500)
        {
            first_in = true;
        }
    }

}

void dwin_task(void)
{
    static uint16_t time_count;
	
    static bool first_in = false;
    static uint8_t state = 0;

    if (sFWG2_t.FWG2_STATE == FWG2_WORKING)
    {
        if (first_in == false)
        {
            sFWG2_t.Direct_handle_parameter.set_temp_f_display  =  9 * sFWG2_t.Direct_handle_parameter.set_temp  / 5  + 32;
            first_in = true;
        }

        if (sFWG2_t.general_parameter.work_mode == CODE)
        {
            sFWG2_t.Direct_handle_parameter.actual_temp_f_display = 9 * (sFWG2_t.Direct_handle_parameter.actual_temp)  / 5  + 32;
        }
        else if (sFWG2_t.general_parameter.work_mode == NORMAL)
        {
            if (sFWG2_t.general_parameter.enhance_state == ENHANCE_OPEN)
            {
                sFWG2_t.Direct_handle_parameter.actual_temp_f_display = 9 * (sFWG2_t.Direct_handle_parameter.actual_temp -
                    sFWG2_t.Direct_handle_parameter.set_calibration_temp - ENHANCE_TEMP)  / 5  + 32;
            }
            else if (sFWG2_t.general_parameter.enhance_state == ENHANCE_CLOSE)
            {
                sFWG2_t.Direct_handle_parameter.actual_temp_f_display = 9 * (sFWG2_t.Direct_handle_parameter.actual_temp -
                    sFWG2_t.Direct_handle_parameter.set_calibration_temp)  / 5  + 32;
            }
        }

        time_count++;

        if (time_count % 5 == 0)
        {
            //Page_Main_Heartbeat_Packet();
			//Page_ALL_Curve_Heartbeat_Packet();

            Page_General_Heartbeat_Packet();
            Page_Set_Heartbeat_Packet();
            Page_Switch();
        }
        else if (time_count % 3 == 0)
        {
            Page_Direct_Work_Heartbeat_Packet();
            Page_Direct_Curve_Heartbeat_Packet();
			Page_Code_Heartbeat_Packet();
        }

        sdwin.recv_data(&sdwin);
    }
}

void work_task(void)
{
    Direct_handle_switch();
}

void flash_task(void)
{
    FlashProc();
}

void output_task(void)
{
    fan_control();
    hot_control();
}

/* add user code end 4 */
