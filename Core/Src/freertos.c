/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f4xx_hal.h"  // this include must be compiled ASAP to defined the struct HAL_StatusTypeDef
#include "stm32f4xx_hal_gpio.h"
#include "usart.h"
#include "stdbool.h"
#include "tim.h"
#include <stdio.h>
#include <string.h>
#include "adc.h"
#include "rtc.h"
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
/* Definitions for LedBlinkTask */
osThreadId_t LedBlinkTaskHandle;
const osThreadAttr_t LedBlinkTask_attributes = {
  .name = "LedBlinkTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ButtonTask */
osThreadId_t ButtonTaskHandle;
const osThreadAttr_t ButtonTask_attributes = {
  .name = "ButtonTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for StepResponse */
osThreadId_t StepResponseHandle;
const osThreadAttr_t StepResponse_attributes = {
  .name = "StepResponse",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for PWMResponse */
osThreadId_t PWMResponseHandle;
const osThreadAttr_t PWMResponse_attributes = {
  .name = "PWMResponse",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for Scheduler */
osThreadId_t SchedulerHandle;
const osThreadAttr_t Scheduler_attributes = {
  .name = "Scheduler",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ComQueue */
osMessageQueueId_t ComQueueHandle;
const osMessageQueueAttr_t ComQueue_attributes = {
  .name = "ComQueue"
};
/* Definitions for UARTMutex */
osMutexId_t UARTMutexHandle;
const osMutexAttr_t UARTMutex_attributes = {
  .name = "UARTMutex"
};
/* Definitions for LED2Mutex */
osMutexId_t LED2MutexHandle;
const osMutexAttr_t LED2Mutex_attributes = {
  .name = "LED2Mutex"
};
/* Definitions for ButtonMutex */
osMutexId_t ButtonMutexHandle;
const osMutexAttr_t ButtonMutex_attributes = {
  .name = "ButtonMutex"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void LED2_Blink(uint16_t rate);
void LED2_SOS(void);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartLedBlinkTask(void *argument);
void StartButtonTask(void *argument);
void StartStepResponse(void *argument);
void StartPWMResponse(void *argument);
void StartScheduler(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of UARTMutex */
  UARTMutexHandle = osMutexNew(&UARTMutex_attributes);

  /* creation of LED2Mutex */
  LED2MutexHandle = osMutexNew(&LED2Mutex_attributes);

  /* creation of ButtonMutex */
  ButtonMutexHandle = osMutexNew(&ButtonMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of ComQueue */
  ComQueueHandle = osMessageQueueNew (16, sizeof(uint8_t), &ComQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of LedBlinkTask */
  LedBlinkTaskHandle = osThreadNew(StartLedBlinkTask, NULL, &LedBlinkTask_attributes);

  /* creation of ButtonTask */
  ButtonTaskHandle = osThreadNew(StartButtonTask, NULL, &ButtonTask_attributes);

  /* creation of StepResponse */
  StepResponseHandle = osThreadNew(StartStepResponse, NULL, &StepResponse_attributes);

  /* creation of PWMResponse */
  PWMResponseHandle = osThreadNew(StartPWMResponse, NULL, &PWMResponse_attributes);

  /* creation of Scheduler */
  SchedulerHandle = osThreadNew(StartScheduler, NULL, &Scheduler_attributes);

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
  /* Infinite loop */
  for(;;)
  {
    osDelay(200);
  }
    //HAL_UART_Transmit(&huart2, (uint8_t *)"Default Task\r\n", sizeof("Default Task\r\n")-1, HAL_MAX_DELAY);
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartLedBlinkTask */
/**
* @brief Function implementing the LedBlinkTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLedBlinkTask */
void StartLedBlinkTask(void *argument)
{
  /* USER CODE BEGIN StartLedBlinkTask */
  /* Infinite loop */
  bool blinkRate = true; // Default blink rate
  uint8_t queuemsg;
  for(;;)
  {   
    osMessageQueueGet(ComQueueHandle, &queuemsg, 0, 100);
    if (queuemsg == 0xFF) 
    {
      while(osMutexAcquire(UARTMutexHandle, osWaitForever) != osOK);
      HAL_UART_Transmit(&huart2, (uint8_t *)"MSG received\r\n", sizeof("MSG received\r\n")-1, HAL_MAX_DELAY);
      osMutexRelease(UARTMutexHandle);
      blinkRate = !blinkRate; // Change blink rate to fast when SOS is received
      queuemsg = 0; // Reset the message after processing
    }

    if (blinkRate) {
      LED2_Blink(1000); // Slow blink
    } else {
      LED2_Blink(200); // Fast blink
    }
    
    //LED2_SOS();
  }
  /* USER CODE END StartLedBlinkTask */
}

/* USER CODE BEGIN Header_StartButtonTask */
/**
* @brief Function implementing the ButtonTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartButtonTask */
void StartButtonTask(void *argument)
{
  /* USER CODE BEGIN StartButtonTask */
  /* Infinite loop */
  GPIO_PinState buttonState;
  for(;;)
  {
    
    uint8_t DebounceCounter = 0;
    
    while(DebounceCounter < 5) {
      while(osMutexAcquire(ButtonMutexHandle, osWaitForever) != osOK);
      buttonState = HAL_GPIO_ReadPin(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN);
      osMutexRelease(ButtonMutexHandle);
      
      if(buttonState == GPIO_PIN_RESET) {
        DebounceCounter++;
      } else {
        DebounceCounter = 0;
      }
      osDelay(10); // Debounce delay
    }

    // Button is pressed
    // osMutexAcquire(UARTMutexHandle, osWaitForever);
    // HAL_UART_Transmit(&huart2, (uint8_t *)"SOS\r\n", sizeof("SOS\r\n")-1, HAL_MAX_DELAY);
    // osMutexRelease(UARTMutexHandle);
    uint8_t message = 0xFF;
    osMessageQueuePut(ComQueueHandle, &message, 0, osWaitForever);
    osDelay(100);

    while(buttonState == GPIO_PIN_RESET) {
      while(osMutexAcquire(ButtonMutexHandle, osWaitForever) != osOK);
      buttonState = HAL_GPIO_ReadPin(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN);
      osMutexRelease(ButtonMutexHandle);
      osDelay(10); // Debounce delay
    }
  }
  /* USER CODE END StartButtonTask */
}

/* USER CODE BEGIN Header_StartStepResponse */
/**
* @brief Function implementing the StepResponse thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartStepResponse */
void StartStepResponse(void *argument)
{
  /* USER CODE BEGIN StartStepResponse */
  /* Infinite loop */

  RTC_TimeTypeDef internal_time;
  uint32_t  raw_responce = 0;
  uint32_t response_array[1000];
  uint32_t time_array[1000];
  HAL_ADC_Start_DMA(&hadc1,(uint32_t*)&raw_responce,1); 
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  char cmd_msg[] = "step responce 0-20 duty-cycle\r\n";
  char res_msg[] = "raw response = 00000\r\n";
  char timestamp_msg[] = "000000";
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
  HAL_UART_Transmit(&huart2, (uint8_t *)cmd_msg, sizeof(cmd_msg)-1, HAL_MAX_DELAY);
  sprintf(res_msg, "raw response = %05d\r\n", (int)raw_responce);
  HAL_UART_Transmit(&huart2, (uint8_t *)res_msg, sizeof(res_msg)-1, HAL_MAX_DELAY);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 200);
  for (size_t i = 0; i < 1000; i++)
  {
    HAL_RTC_GetTime(&hrtc, &internal_time, RTC_FORMAT_BCD);
    response_array[i] = raw_responce;
    time_array[i] = internal_time.SecondFraction;
  }
  for (size_t i = 0; i < 1000; i++)
  {
    sprintf(timestamp_msg, "%06d", (int) time_array[i]);
    HAL_UART_Transmit(&huart2, (uint8_t *)timestamp_msg, sizeof(timestamp_msg)-1, HAL_MAX_DELAY);
    sprintf(res_msg, "raw response = %05d\r\n", (int)response_array[i]);
    HAL_UART_Transmit(&huart2, (uint8_t *)res_msg, sizeof(res_msg)-1, HAL_MAX_DELAY);
  }
  
  
  
  /* USER CODE END StartStepResponse */
}

/* USER CODE BEGIN Header_StartPWMResponse */
/**
* @brief Function implementing the PWMResponse thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPWMResponse */
void StartPWMResponse(void *argument)
{
  /* USER CODE BEGIN StartPWMResponse */
  /* Infinite loop */
  uint16_t   pwm_cmd = 0;
  uint32_t  raw_responce = 0;
  HAL_ADC_Start_DMA(&hadc1,(uint32_t*)&raw_responce,1); 
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  char cmd_msg[] = "duty cycle = 000 -- ";
  char res_msg[] = "raw response = 00000\r\n";
  for (size_t i = 0; i < 80; i++)
  {
    pwm_cmd = i*10;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pwm_cmd);
    osDelay(1000);
    sprintf(cmd_msg, "duty cycle = %03d -- ", i);
    sprintf(res_msg, "raw response = %05d\r\n", (int)raw_responce);
    HAL_UART_Transmit(&huart2, (uint8_t *)cmd_msg, sizeof(cmd_msg)-1, HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2, (uint8_t *)res_msg, sizeof(res_msg)-1, HAL_MAX_DELAY);
  }
  
  /* USER CODE END StartPWMResponse */
}

/* USER CODE BEGIN Header_StartScheduler */
/**
* @brief Function implementing the Scheduler thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartScheduler */
void StartScheduler(void *argument)
{
  /* USER CODE BEGIN StartScheduler */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartScheduler */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void LED2_Blink(uint16_t rate)
{

  osDelay(rate);
  while(osMutexAcquire(LED2MutexHandle, osWaitForever) != osOK);
  HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
  osMutexRelease(LED2MutexHandle);
}

void LED2_SOS(void)
{
  while(osMutexAcquire(LED2MutexHandle, osWaitForever) != osOK);
  HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET);
  osDelay(500); // Initial delay before starting SOS pattern
  // SOS in Morse code: ... --- ...
  for (int i = 0; i < 6; i++) {
    HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
    osDelay(200); // Short blink
  }
  osDelay(600); // Pause between letters
  for (int i = 0; i < 3; i++) {
    HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
    osDelay(600); // Long blink
    HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET);
    osDelay(200); // Short pause
  }
  osDelay(600); // Pause between letters
  for (int i = 0; i < 6; i++) {
    HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
    osDelay(200); // Short blink
  }
  osDelay(1000); // Pause before repeating
  osMutexRelease(LED2MutexHandle);
}

/* USER CODE END Application */

