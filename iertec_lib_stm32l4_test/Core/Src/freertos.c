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
#include "sys_util.h"
#include "debug_util.h"
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
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void button_isr(void);
void rtc_cb(void);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 4 */
__weak void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
__weak void vApplicationMallocFailedHook(void)
{
   /* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created. It is also called by various parts of the
   demo application. If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
}
/* USER CODE END 5 */

/* USER CODE BEGIN VPORT_SUPPORT_TICKS_AND_SLEEP */
__weak void vPortSuppressTicksAndSleep( TickType_t xExpectedIdleTime )
{
  // Generated when configUSE_TICKLESS_IDLE == 2.
  // Function called in tasks.c (in portTASK_FUNCTION).
  // TO BE COMPLETED or TO BE REPLACED by a user one, overriding that weak one.
}
/* USER CODE END VPORT_SUPPORT_TICKS_AND_SLEEP */

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
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 256);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  // Test UART
//  itf_uart_read_enable(H_ITF_UART_DEBUG);

  // Test reset
//  uint32_t     reset_code;
//  const char * reset_str;
//
//  sys_get_reset_source(&reset_code, &reset_str);

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
//    itf_io_set_int_cb(H_ITF_IO_BUTTON_1, button_isr);
//    osDelay(2000);
//    itf_io_set_int_cb(H_ITF_IO_BUTTON_1, NULL);
//    osDelay(2000);

    // Test UART
//    char data[32];
//    bool ret;
//    size_t len;
//
//    ret = itf_uart_write(H_ITF_UART_DEBUG, "Hola\r\n");
//
//    if (!ret)
//    {
//      itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_HIGH);
//      for(;;);
//    }
//
//    len = itf_uart_read(H_ITF_UART_DEBUG, data, 32);
//    ret = itf_uart_write_bin(H_ITF_UART_DEBUG, data, len);
//
//    if (!ret)
//    {
//      itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_HIGH);
//      for(;;);
//    }

    // Test debug UART
//    DEBUG_ASSERT(false);
//    static int i = 0;
//
//    if (itf_debug_is_connected())
//    {
//      itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_HIGH);
//    }
//    else
//    {
//      itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_LOW);
//    }
//
//    debug_printf("Hola %d\r\n", i++);
//
//    int d_char = debug_get_char();
//
//    if (d_char < 0)
//    {
//      debug_printf("%d\r\n", d_char);
//    }
//    else
//    {
//        debug_printf("%c\r\n", d_char);
//    }
//
//    osDelay(4000);

    // Test SPI
//    uint8_t data_tx[100];
//    uint8_t data_rx[100] = {0};
//
//    for (size_t i = 0; i < 100; i++)
//    {
//      data_tx[i] = i;
//      data_rx[i] = 0;
//    }
//
//    bool ret = itf_spi_transaction(H_ITF_SPI_0, data_tx, data_rx, 100);
//
//    for (size_t i = 0; i < 100; i++)
//    {
//      if (data_tx[i] != data_rx[i])
//      {
//        ret = false;
//        break;
//      }
//    }
//
//    if (!ret)
//    {
//      itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_HIGH);
//      for(;;);
//    }
//
//    osDelay(4000);

    // Test watchdog
//    osDelay(1000);
//    itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_HIGH);
//
//    // Change timeout
//    bool ret = itf_wdgt_set_timeout(10000);
//
//    if (!ret)
//    {
//      for(;;);
//    }
//
//    // Infinite feed loop
//    for (;;)
//    {
//      osDelay(4000);
//      itf_wdgt_feed();
//      osDelay(6000);
//      itf_wdgt_feed();
//    }

    // Test I2C (PCF85063A)
//    uint8_t data_tx[7] = {0x04, 30, 30, 12, 15, 6, 25};
//    uint8_t data_rx[6] = {0};
//    bool ret = itf_i2c_transaction(H_ITF_I2C_0, 0x51, data_tx, 7, NULL, 0);
//
//    if (!ret)
//    {
//      itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_HIGH);
//      for(;;);
//    }
//
//    ret = itf_i2c_transaction(H_ITF_I2C_0, 0x51, data_tx, 1, data_rx, 6);
//
//    for (size_t i = 1; i < 7; i++)
//    {
//      if (data_tx[i] != data_rx[i - 1])
//      {
//        ret = false;
//        break;
//      }
//    }
//
//    if (!ret)
//    {
//      itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_HIGH);
//      for(;;);
//    }
//
//    osDelay(4000);

    // Test power consumption
//    itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_HIGH);
//    HAL_Delay(500);
//    itf_io_set_value(H_ITF_IO_LED_GREEN, ITF_IO_LOW);
//    osDelay(2000);

    // Test time
    uint32_t time_1 = sys_get_timestamp();
    sys_sleep_msec(1000);
    uint32_t time_2 = sys_get_timestamp();
    uint32_t time_diff = time_2 - time_1;

    debug_printf("TSL %u\r\n", time_diff);

    time_1 = sys_get_timestamp();
    HAL_Delay(1000);
    time_2 = sys_get_timestamp();
    time_diff = time_2 - time_1;

    debug_printf("TDH %u\r\n", time_diff);

    time_1 = sys_get_timestamp();
    sys_delay_usec(1000000);
    time_2 = sys_get_timestamp();
    time_diff = time_2 - time_1;

    debug_printf("TDR %u\r\n", time_diff);

    // Default action
//    osDelay(4000);
    itf_wdgt_feed();

    vApplicationPrintRtosInfo();
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
