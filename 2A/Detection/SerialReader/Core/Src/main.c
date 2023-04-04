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
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f4xx_hal_uart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define USE_HAL_UART_REGISTER_CALLBACKS 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define TRAME_BOUND		0xFF
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */
uint8_t chiffres[16] ={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
uint8_t buffer[20000] = {0};
uint8_t DataAcquiered = 0;
uint8_t newDataset = 0;
uint8_t trameStarted = 0;
uint16_t bufferIndex = 0;
uint16_t trameIndex = 0;

uint8_t NROI; uint8_t loopC =0;

uint8_t sideTrameText[32] = "##############################\r\n";
uint8_t middleTrameText[32] = "------------------------------\r\n";
uint8_t startContent[8] = "Ncapt : ";
uint8_t middleContent[10] = " , nROI : ";

uint16_t distanceList[2000] = {0};
uint8_t captorList[2000] = {0};
uint8_t roiList[2000] = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void readTrame(void);
void readHeader(void);
void readValue(void);
void trameStatus(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void readTrame(void){
	trameStatus();

	if(newDataset){
		HAL_UART_Transmit(&huart2, sideTrameText, 32, 100);
		readHeader();
		HAL_UART_Transmit(&huart2, middleTrameText, 32, 100);
		readValue();
		HAL_UART_Transmit(&huart2, sideTrameText, 32, 100);
		newDataset = 0;
		loopC++;

		if(loopC > 4){
			HAL_Delay(5);
		}
	}
}
void readHeader(void){
	uint8_t value[20];
	uint8_t ncaptActifs = buffer[trameIndex+3 ];
	NROI = buffer[trameIndex+4 ];
	uint8_t tpsMesure = buffer[trameIndex+5 ];
	int size = sprintf((char*)value, "%d micro secondes\r\n", (int)tpsMesure*10);
	HAL_UART_Transmit(&huart2, &"Nous avons NcaptActifs = ", 25, 100);
	HAL_UART_Transmit(&huart2, &chiffres[ncaptActifs], 1, 100);
	HAL_UART_Transmit(&huart2, &", qui utlisent ", 15, 100);
	HAL_UART_Transmit(&huart2, &chiffres[NROI], 1, 100);
	HAL_UART_Transmit(&huart2, &" ROIs differents en \n\r", 22, 100);
	HAL_UART_Transmit(&huart2, value, strlen(value), 100);
}
void readValue(void){
	int i = trameIndex + 3+3;

	uint8_t Nmesure = 0;
	uint8_t value[30];
	uint8_t Ncapteur; uint8_t indiceROI; uint16_t distance;
	while(i < (bufferIndex - 3)){
		//Selon le code du steme de detection : "Pour que le premier element de la chaine de caracteres ne soit pas '\0'"
		// Donc il faut enlever l'offset sur buffer[i]
		indiceROI = (buffer[i]-1)%NROI;
		Ncapteur  = (buffer[i]-1 -indiceROI)/NROI;
		distance  = (buffer[i+1]<<8) + buffer[i+2];
		sprintf((char*)value, " , Distance \t\t = %d\r\n", (int)distance);

		HAL_UART_Transmit(&huart2, startContent, 8, 100);			//"Ncapt : "
		HAL_UART_Transmit(&huart2, &chiffres[Ncapteur], 1, 100);
		HAL_UART_Transmit(&huart2, middleContent, 10, 100);			//" , nROI : "
		HAL_UART_Transmit(&huart2, &chiffres[indiceROI], 1, 100);
		HAL_UART_Transmit(&huart2, value, strlen(value), 100);		//" , Distance \t\t = %d\r\n"

		captorList[Nmesure]=Ncapteur; roiList[Nmesure]=indiceROI; distanceList[Nmesure]=distance;

		i += 3;
		Nmesure++;
	}
}
/**
  * @brief Modifies the state of the incoming trame
  */
void trameStatus(void){
	//End of Trame
	if(trameStarted && (bufferIndex >= 3) && DataAcquiered){
		if((buffer[bufferIndex - 3] == TRAME_BOUND) && (buffer[bufferIndex - 2] == TRAME_BOUND) && (buffer[bufferIndex - 1] == TRAME_BOUND)){
			trameStarted = 0;

			newDataset = 1;
		}
	}
	//Start of trame
	else if(!trameStarted && (bufferIndex >= 3) && DataAcquiered){
		if((buffer[bufferIndex - 3] == TRAME_BOUND) && (buffer[bufferIndex - 2] == TRAME_BOUND) && (buffer[bufferIndex - 1] == TRAME_BOUND)){
			trameStarted = 1;
			trameIndex = bufferIndex;
		}
	}
	DataAcquiered = 0;
}
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
  MX_USART2_UART_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  //HAL_UART_Transmit(&huart1, buffer, 1, 1);
  //HAL_UART_Receive_IT(&huart1, buffer, sizeof(buffer));
  HAL_UART_Receive_DMA(&huart1, &buffer[bufferIndex], 3);


  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
  HAL_Delay(100);
  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  if(DataAcquiered)
		  readTrame();
		  //HAL_UART_Transmit(&huart2, &buffer[bufferIndex-3], 3, 100);
	HAL_Delay(500);

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	DataAcquiered = 1;
    bufferIndex += 3;
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    //HAL_UART_Receive_IT(&huart1, buffer, 3);
    HAL_UART_Receive_DMA(&huart1, &buffer[bufferIndex], 3);



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
