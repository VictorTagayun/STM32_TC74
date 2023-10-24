/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define TC74_Standby 0x80
#define TC74_Normal 0x00
#define TC74_Address 0x4d
#define TC74_I2C_delay 200

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c3;
DMA_HandleTypeDef hdma_i2c3_rx;
DMA_HandleTypeDef hdma_i2c3_tx;

UART_HandleTypeDef hlpuart1;

/* USER CODE BEGIN PV */

enum TC74_Command{
	TempCommand = 0,
	ConfigCommand,
};

enum TC74_ReadWriteFunc{
	TC74_ReadFunc = 0,
	TC74_WriteFunc,
	TC74_ReadTemperatureFunc,
	TC74_ReadConfigFunc,
	TC74_WriteConfigFunc,
};


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C3_Init(void);
/* USER CODE BEGIN PFP */

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

void TC74_Byte_ReadFunc_Temp (void);
void TC74_Byte_ReadFunc_Config (void);
void TC74_ReadFunc_LastCommand (void);
void TC74_Byte_ReadWriteFunc_ReadTemp (void);
void TC74_Byte_ReadWriteFunc_ReadConfig (void);
void TC74_Byte_ReadWriteFunc_WriteConfig_Normal (void);
void TC74_Byte_ReadWriteFunc_WriteConfig_Standby (void);
HAL_StatusTypeDef TC74_Byte_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t Command, uint8_t *pData, uint32_t Timeout);
HAL_StatusTypeDef TC74_Byte_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t Command, uint8_t *pData, uint32_t Timeout);
HAL_StatusTypeDef TC74_Receive_LastReadWriteAddress(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint32_t Timeout);
HAL_StatusTypeDef TC74_Byte_ReadWrite(I2C_HandleTypeDef *hi2c, uint16_t TC74_ReadWriteFunc, uint16_t DevAddress, uint8_t *pData, uint32_t Timeout);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_LPUART1_UART_Init();
  MX_I2C1_Init();
  MX_I2C3_Init();
  /* USER CODE BEGIN 2 */

	printf("\n\n=======================================\n");
	printf("Start >> NUCLEO-G474RE_I2C_TC74-V3 \n");

	TC74_Byte_ReadFunc_Temp(); //TC74_Byte_ReadFunc_Temp();

    while (HAL_I2C_GetState(&hi2c3) != HAL_I2C_STATE_READY)
    {
    }

	TC74_Byte_ReadFunc_Config (); //TC74_Byte_ReadFunc_Config ();

    while (HAL_I2C_GetState(&hi2c3) != HAL_I2C_STATE_READY)
    {
    }

	TC74_ReadFunc_LastCommand();
/*

	TC74_Byte_ReadWriteFunc_ReadTemp();

	TC74_Byte_ReadWriteFunc_ReadConfig();

	TC74_Byte_ReadWriteFunc_WriteConfig_Standby();

	TC74_Byte_ReadWriteFunc_ReadConfig();

	TC74_Byte_ReadWriteFunc_ReadConfig();

	TC74_Byte_ReadWriteFunc_WriteConfig_Normal();

	TC74_Byte_ReadWriteFunc_ReadConfig();

	TC74_Byte_ReadWriteFunc_ReadConfig();
*/

	printf("----\n");
	printf("End   >> NUCLEO-G474RE_I2C_TC74-V3 \n");
	printf("=======================================\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV6;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x30A0A7FB;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.Timing = 0x30A0A7FB;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /** I2C Fast mode Plus enable
  */
  __HAL_SYSCFG_FASTMODEPLUS_ENABLE(I2C_FASTMODEPLUS_I2C1);

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the LPUART1 and Loop until the end of transmission */
	HAL_UART_Transmit(&hlpuart1, (uint8_t *)&ch, 1, 0xFFFF);

	return ch;
}

void TC74_Byte_ReadFunc_Temp (void)
{
	uint8_t TC74_temperature;
	printf("----\n");
	printf("TC74_Byte_Read address = TempCommand = %X \n", TempCommand);
	TC74_Byte_Read(&hi2c3, TC74_Address, TempCommand, &TC74_temperature, 10);
	printf("TC74_temperature read data = %d \n", TC74_temperature);
}

void TC74_Byte_ReadFunc_Config (void)
{
	uint8_t TC74_config;
	printf("----\n");
	printf("TC74_Byte_Read address = ConfigCommand = %X \n", ConfigCommand);
	TC74_Byte_Read(&hi2c3, TC74_Address, ConfigCommand, &TC74_config, 10);
	printf("TC74_config read data = 0x%X \n", TC74_config);
}

void TC74_ReadFunc_LastCommand (void)
{
	uint8_t TC74_LastCommand;
	printf("----\n");
	printf("TC74_Receive_LastCommand \n");
	TC74_Receive_LastReadWriteAddress(&hi2c3, TC74_Address, &TC74_LastCommand, 10);
	printf("TC74_LastCommand = 0x%X = %d\n", TC74_LastCommand, TC74_LastCommand);
}

void TC74_Byte_ReadWriteFunc_ReadTemp (void)
{
	uint8_t TC74_temperature;
	printf("----\n");
	printf("TC74_Byte_ReadWrite  = TC74_ReadTemperatureFunc \n");
	TC74_Byte_ReadWrite(&hi2c3, TC74_ReadTemperatureFunc, TC74_Address, &TC74_temperature, 10);
	printf("TC74_ReadTemperatureFunc read data = %d \n", TC74_temperature);
}

void TC74_Byte_ReadWriteFunc_ReadConfig (void)
{
	uint8_t TC74_config;
	printf("----\n");
	printf("TC74_Byte_ReadWrite  = TC74_ReadConfigFunc \n");
	TC74_Byte_ReadWrite(&hi2c3, TC74_ReadConfigFunc, TC74_Address, &TC74_config, 10);
	printf("TC74_ReadConfigFuncc read data = 0x%X \n", TC74_config);
}

void TC74_Byte_ReadWriteFunc_WriteConfig_Normal (void)
{
	uint8_t TC74_config = TC74_Normal;
	printf("----\n");
	printf("TC74_Byte_ReadWrite  = TC74_WriteConfigFunc = TC74_Normal \n");
	TC74_Byte_ReadWrite(&hi2c3, TC74_WriteConfigFunc, TC74_Address, &TC74_config, 10);
	printf("TC74_ReadConfigFuncc read data = 0x%X \n", TC74_config);
}

void TC74_Byte_ReadWriteFunc_WriteConfig_Standby (void)
{
	uint8_t TC74_config = TC74_Standby;
	printf("----\n");
	printf("TC74_Byte_ReadWrite  = TC74_WriteConfigFunc = TC74_Standby \n");
	TC74_Byte_ReadWrite(&hi2c3, TC74_WriteConfigFunc, TC74_Address, &TC74_config, 10);
	printf("TC74_ReadConfigFuncc read data = 0x%X \n", TC74_config);
}

HAL_StatusTypeDef TC74_Byte_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t Command, uint8_t *pData, uint32_t Timeout)
{
	HAL_StatusTypeDef status;
	status = HAL_I2C_Mem_Read_DMA(hi2c, DevAddress << 1,  Command, 1, pData, 1);
	printf("TC74_Byte_Read status = 0x%X \n", status);
	HAL_Delay(TC74_I2C_delay);
	return status;
}

HAL_StatusTypeDef TC74_Byte_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t Command, uint8_t *pData, uint32_t Timeout)
{
	HAL_StatusTypeDef status;
	status = HAL_I2C_Mem_Write_DMA(hi2c, DevAddress << 1,  Command, 1, pData, 1);
	printf("TC74_Byte_Write status = 0x%X \n", status);
	HAL_Delay(TC74_I2C_delay);
	return status;
}

HAL_StatusTypeDef TC74_Receive_LastReadWriteAddress(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint32_t Timeout)
{
	HAL_StatusTypeDef status;
	status = HAL_I2C_Master_Receive_DMA(hi2c, DevAddress << 1, pData,  1);
	printf("TC74_Receive_LastReadWriteAddress status = 0x%X \n", status);
	HAL_Delay(TC74_I2C_delay);
	return status;
}

HAL_StatusTypeDef TC74_Byte_ReadWrite(I2C_HandleTypeDef *hi2c, uint16_t TC74_ReadWriteFunc, uint16_t DevAddress, uint8_t *pData, uint32_t Timeout)
{
	HAL_StatusTypeDef status;
	if(TC74_ReadWriteFunc == TC74_ReadTemperatureFunc)
		status = HAL_I2C_Mem_Read_DMA(hi2c, DevAddress << 1,  TempCommand, 1, pData, 1);
	else if (TC74_ReadWriteFunc == TC74_ReadConfigFunc)
		status = HAL_I2C_Mem_Read_DMA(hi2c, DevAddress << 1,  ConfigCommand, 1, pData, 1);
	else if (TC74_ReadWriteFunc == TC74_WriteConfigFunc)
		status = HAL_I2C_Mem_Write_DMA(hi2c, DevAddress << 1,  ConfigCommand, 1, pData, 1);
	else
		status = 0xff;
	printf("TC74_Byte_ReadWrite status = 0x%X \n", status);
	HAL_Delay(TC74_I2C_delay);
	return status;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	printf("Error_Handler \n");
	__disable_irq();
	while (1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
