/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "itf_io.h"
#include "itf_wdgt.h"
#include "itf_uart.h"
#include "itf_debug.h"
#include "itf_spi.h"
#include "itf_i2c.h"
#include "itf_rtc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void button_isr(void);
void rtc_cb(void);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  // Test UART
//  itf_uart_read_enable(H_ITF_UART_DEBUG);

  // Test RTC
//  itf_rtc_set_callback(rtc_cb);

  /* Infinite loop */
  for(;;)
  {
    // Test I/O input
//    if (itf_io_get_value(H_ITF_IO_BUTTON_1) == ITF_IO_LOW)
//    {
//        itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_HIGH);
//    }
//    else
//    {
//        itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_LOW);
//    }
//    osDelay(10);

    // Test I/O output
//    itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_HIGH);
//    osDelay(1000);
//    itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_LOW);
//    osDelay(1000);
//    itf_io_toggle_value(H_ITF_IO_LED_GREEN);
//    osDelay(1000);
//    itf_io_toggle_value(H_ITF_IO_LED_GREEN);
//    osDelay(1000);

    // Test I/O interrupt
//    osDelay(5000);
//    itf_io_set_int_cb(H_ITF_IO_BUTTON_1, button_isr);
//    osDelay(5000);
//    itf_io_set_int_cb(H_ITF_IO_BUTTON_1, NULL);

    // Test UART
//    char data[32];
//    bool ret;
//    size_t len;
//
//    ret = itf_uart_write(H_ITF_UART_DEBUG, "Hola\r\n");
//
//    if (!ret)
//    {
//      for(;;);
//    }
//
//    len = itf_uart_read(H_ITF_UART_DEBUG, data, 32);
//    ret = itf_uart_write_bin(H_ITF_UART_DEBUG, data, len);
//
//    if (!ret)
//    {
//      for(;;);
//    }

    // Test debug UART
//    if (itf_debug_uart_check())
//    {
//      itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_HIGH);
//    }
//    else
//    {
//      itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_LOW);
//    }
//
//    itf_debug_write("Hola\r\n", 6);
//
//    int d_char = itf_debug_get_char();
//
//    if (-2 == d_char)
//    {
//      itf_debug_write("-2\r\n", 4);
//    }
//    else if (-1 == d_char)
//    {
//      itf_debug_write("-1\r\n", 4);
//    }
//    else
//    {
//        itf_debug_put_char(d_char);
//    }
//
//    osDelay(5000);

    // Test SPI
//    uint8_t data_tx[100];
//    uint8_t data_rx[100] = {0};
//
//    for (size_t i = 0; i < 100; i++)
//    {
//        data_tx[i] = i;
//    }
//
//    bool ret = itf_spi_transaction(H_ITF_SPI_0, data_tx, data_rx, 100);
//
//    osDelay(5000);

    // Test watchdog
//    osDelay(1000);
//    itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_HIGH);
//
//    // Change timeout
////    bool ret = itf_wdgt_set_timeout(10000);
//
//    // Infinite feed loop
//    for (;;)
//    {
//      itf_wdgt_feed();
//      osDelay(4000);
//    }

    // Test I2C (PCF85063A)
//    uint8_t tx_data[7] = {0x04, 30, 30, 12, 15, 6, 25};
//    uint8_t rx_data[6] = {0};
//    bool ret_tx = itf_i2c_transaction(H_ITF_I2C_0, 0x51, tx_data, 7, NULL, 0);
//    bool ret_rx = itf_i2c_transaction(H_ITF_I2C_0, 0x51, tx_data, 1, rx_data, 6);
//    osDelay(5);

    // Test power consumption
    itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_HIGH);
    HAL_Delay(500);
    itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_LOW);
    osDelay(2000);

    // Default action
//    osDelay(4000);
    itf_wdgt_feed();
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void
button_isr (void)
{
    itf_io_toggle_value(H_ITF_IO_LED_GREEN);
}

void
rtc_cb (void)
{
    itf_io_toggle_value(H_ITF_IO_LED_GREEN);
}
/* USER CODE END Application */

