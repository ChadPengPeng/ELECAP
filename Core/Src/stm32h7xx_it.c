/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "main.h"
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "osc.h"
#include "event.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TIM13UpdateFreq 100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern OscData *thisOsc;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;
extern DMA_HandleTypeDef hdma_dac1_ch1;
extern TIM_HandleTypeDef htim13;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */
  static uint32_t tick = 0;
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET)
  {
    if(HAL_GetTick() - tick > 50){
      addEvent(KEY1);
      tick = HAL_GetTick();
    }
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
  }
  /* USER CODE END EXTI0_IRQn 0 */
  /* USER CODE BEGIN EXTI0_IRQn 1 */

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line1 interrupt.
  */
void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */
  static uint32_t tick = 0;
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_1) != RESET)
  {
    if(HAL_GetTick() - tick > 50){
      addEvent(KEY2);
      tick = HAL_GetTick();
    }
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
  }
  /* USER CODE END EXTI1_IRQn 0 */
  /* USER CODE BEGIN EXTI1_IRQn 1 */

  /* USER CODE END EXTI1_IRQn 1 */
}

/**
  * @brief This function handles EXTI line2 interrupt.
  */
void EXTI2_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_IRQn 0 */
  static uint32_t tick = 0;
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_2) != RESET)
  {
    if(HAL_GetTick() - tick > 50){
      addEvent(KEY3);
      tick = HAL_GetTick();
    }
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
  }
  /* USER CODE END EXTI2_IRQn 0 */
  /* USER CODE BEGIN EXTI2_IRQn 1 */

  /* USER CODE END EXTI2_IRQn 1 */
}

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */
  static uint32_t tick = 0;   
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3) != RESET)
  {
    if(HAL_GetTick() - tick > 50){
      addEvent(KEY4);
      tick = HAL_GetTick();
    }
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
  }
  /* USER CODE END EXTI3_IRQn 0 */
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream0 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */
	extern DMA_HandleTypeDef hdma_dac1_ch1;
	if (__HAL_DMA_GET_FLAG(&hdma_dac1_ch1, __HAL_DMA_GET_TC_FLAG_INDEX(&hdma_dac1_ch1)) != RESET)
    {
        __HAL_DMA_CLEAR_FLAG(&hdma_dac1_ch1, __HAL_DMA_GET_TC_FLAG_INDEX(&hdma_dac1_ch1));
        //HAL_DMAEx_TCMpleteCallback(&hdma1_stream0);
    }
    
    if (__HAL_DMA_GET_FLAG(&hdma_dac1_ch1, __HAL_DMA_GET_HT_FLAG_INDEX(&hdma_dac1_ch1)) != RESET)
    {
        __HAL_DMA_CLEAR_FLAG(&hdma_dac1_ch1, __HAL_DMA_GET_HT_FLAG_INDEX(&hdma_dac1_ch1));
        //HAL_DMAEx_HTCMpleteCallback(&hdma1_stream0);
    }
    
    if (__HAL_DMA_GET_FLAG(&hdma_dac1_ch1, __HAL_DMA_GET_TE_FLAG_INDEX(&hdma_dac1_ch1)) != RESET)
		{
        __HAL_DMA_CLEAR_FLAG(&hdma_dac1_ch1, __HAL_DMA_GET_TE_FLAG_INDEX(&hdma_dac1_ch1));
        //HAL_DMAEx_ErrorCallback(&hdma1_stream0);
    }
  /* USER CODE END DMA1_Stream0_IRQn 0 */
  /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */

  /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream1 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */
	extern DMA_HandleTypeDef hdma_adc1;
	if (__HAL_DMA_GET_FLAG(&hdma_adc1, __HAL_DMA_GET_TC_FLAG_INDEX(&hdma_adc1)) != RESET)
    {
        __HAL_DMA_CLEAR_FLAG(&hdma_adc1, __HAL_DMA_GET_TC_FLAG_INDEX(&hdma_adc1));
        //HAL_DMAEx_TCMpleteCallback(&hdma1_stream0);
				busy &= ~1;
			
    }
    
    if (__HAL_DMA_GET_FLAG(&hdma_adc1, __HAL_DMA_GET_HT_FLAG_INDEX(&hdma_adc1)) != RESET)
    {
        __HAL_DMA_CLEAR_FLAG(&hdma_adc1, __HAL_DMA_GET_HT_FLAG_INDEX(&hdma_adc1));
        //HAL_DMAEx_HTCMpleteCallback(&hdma1_stream0);
    }
    
    if (__HAL_DMA_GET_FLAG(&hdma_adc1, __HAL_DMA_GET_TE_FLAG_INDEX(&hdma_adc1)) != RESET)
		{
        __HAL_DMA_CLEAR_FLAG(&hdma_adc1, __HAL_DMA_GET_TE_FLAG_INDEX(&hdma_adc1));
        //HAL_DMAEx_ErrorCallback(&hdma1_stream0);
    }
  /* USER CODE END DMA1_Stream1_IRQn 0 */
  /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */

  /* USER CODE END DMA1_Stream1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream2 global interrupt.
  */
void DMA1_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream2_IRQn 0 */
  extern DMA_HandleTypeDef hdma_adc2;
	if (__HAL_DMA_GET_FLAG(&hdma_adc2, __HAL_DMA_GET_TC_FLAG_INDEX(&hdma_adc2)) != RESET)
    {
        __HAL_DMA_CLEAR_FLAG(&hdma_adc2, __HAL_DMA_GET_TC_FLAG_INDEX(&hdma_adc2));
        //HAL_DMAEx_TCMpleteCallback(&hdma1_stream0);
				busy &= ~(1<<1);
    }
    
    if (__HAL_DMA_GET_FLAG(&hdma_adc2, __HAL_DMA_GET_HT_FLAG_INDEX(&hdma_adc2)) != RESET)
    {
        __HAL_DMA_CLEAR_FLAG(&hdma_adc2, __HAL_DMA_GET_HT_FLAG_INDEX(&hdma_adc2));
        //HAL_DMAEx_HTCMpleteCallback(&hdma1_stream0);
    }
    
    if (__HAL_DMA_GET_FLAG(&hdma_adc2, __HAL_DMA_GET_TE_FLAG_INDEX(&hdma_adc2)) != RESET)
		{
        __HAL_DMA_CLEAR_FLAG(&hdma_adc2, __HAL_DMA_GET_TE_FLAG_INDEX(&hdma_adc2));
        //HAL_DMAEx_ErrorCallback(&hdma1_stream0);
    }
  /* USER CODE END DMA1_Stream2_IRQn 0 */
  /* USER CODE BEGIN DMA1_Stream2_IRQn 1 */

  /* USER CODE END DMA1_Stream2_IRQn 1 */
}

/**
  * @brief This function handles TIM8 update interrupt and TIM13 global interrupt.
  */
void TIM8_UP_TIM13_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 0 */
	if(__HAL_TIM_GET_FLAG(&htim13,TIM_IT_UPDATE) != RESET)
	{
		static int lastWaveNum = 0;
    thisOsc->freq = (TIM15->CNT - lastWaveNum) * TIM13UpdateFreq;
		lastWaveNum = TIM15->CNT;
		__HAL_TIM_CLEAR_IT(&htim13,TIM_IT_UPDATE);
	}	
  /* USER CODE END TIM8_UP_TIM13_IRQn 0 */
  /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 1 */

  /* USER CODE END TIM8_UP_TIM13_IRQn 1 */
}

/**
  * @brief This function handles DMAMUX1 overrun interrupt.
  */
void DMAMUX1_OVR_IRQHandler(void)
{
  /* USER CODE BEGIN DMAMUX1_OVR_IRQn 0 */

  /* USER CODE END DMAMUX1_OVR_IRQn 0 */
  // Handle DMA1_Stream1
  HAL_DMAEx_MUX_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMAMUX1_OVR_IRQn 1 */

  /* USER CODE END DMAMUX1_OVR_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
