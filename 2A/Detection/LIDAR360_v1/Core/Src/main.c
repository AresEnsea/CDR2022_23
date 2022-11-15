/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "platform.h"
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "platform_specific.h"
#include <string.h>
#include "VL53L1X_api.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RadarCircleRadius 						(110.0 / 2.0)
#define Pi 										3.1415
#define VHV_TIMER 								200
#define ROI_CONFIG__USER_ROI_CENTRE_SPAD		0x007F
#define NumOfTOFSensors							9
#define TotalWidthOfSPADS						16
#define WidthOfSPADsPerZone						4
#define NumOfSPADsShiftPerZone					1
#define HorizontalFOVofSensor					19.09
#define SingleSPADFOV							(HorizontalFOVofSensor/TotalWidthOfSPADS)
#define NumOfZonesPerSensor						(((TotalWidthOfSPADS - WidthOfSPADsPerZone) / NumOfSPADsShiftPerZone) + 1)
#define StartingZoneAngle						(WidthOfSPADsPerZone / 2 * SingleSPADFOV)
#define ZoneFOVChangePerStep					(SingleSPADFOV * NumOfSPADsShiftPerZone)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

 /* ----- UART variables, only useful for Serial communication ----- */

 extern char Uart_RXBuffer[80];	/* Buffer for continuous RX */
 #ifdef USER
 extern char UartComm_RXBuffer[80];
 #endif
 extern int UART_Active;		/* Flag to see if UART is active */
 extern int UART_Ready;		/* Flag new command available   */
 extern size_t Uart_RxIndex;

 /* ----- VL53L1X variables ----- */

 uint16_t Dev_init = 0x52;		/* I2C address of device 1 */
 float    LidarAngle[117];
 uint16_t LidarDistance[117];
 uint32_t TimeStamp[117];
 uint16_t Devs[9] = {0x62, 0x64, 0x66, 0x68, 0x6A, 0x6C, 0x6E, 0x70, 0x72};
 uint16_t Distance;
 uint16_t SignalRate;
 uint16_t SpadNb;
 uint16_t AmbientRate;
 uint16_t SignalPerSpad;
 uint16_t AmbientPerSpad;
 uint8_t RangeStatus;
 uint16_t RangeCounter = 0;
 //#define Calibrate
 #ifdef Calibrate
 int16_t  OffsetCal[NumOfTOFSensors*NumOfZonesPerSensor] = {
 		0,0,0,0,0,0,0,0,0,0,0,0,0,
 		0,0,0,0,0,0,0,0,0,0,0,0,0,
 		0,0,0,0,0,0,0,0,0,0,0,0,0,
 		0,0,0,0,0,0,0,0,0,0,0,0,0,
 		0,0,0,0,0,0,0,0,0,0,0,0,0,
 		0,0,0,0,0,0,0,0,0,0,0,0,0,
 		0,0,0,0,0,0,0,0,0,0,0,0,0,
 		0,0,0,0,0,0,0,0,0,0,0,0,0,
 		0,0,0,0,0,0,0,0,0,0,0,0,0
 		};
 #else
 int16_t  OffsetCal[NumOfTOFSensors * NumOfZonesPerSensor] = {
  -40, -44, -29, -24, -21, -18, -14, -15, -14, -13, -17, -22, -32,
  -28, -17, -21, -24, -11, -12, -12,  -9, -11, -26, -25, -36, -38,
  -36, -46, -26, -24, -20, -16, -17, -14, -11, -18, -17, -26, -37,
  -43, -31, -21, -25, -20,  -8, -12, -12, -13, -20, -27, -26, -38,
  -51, -41, -36, -24, -22, -21, -19, -16, -17, -22, -32, -41, -52,
  -44, -38, -26, -26, -29, -17,  -8, -11, -20, -20, -28, -34, -40,
  -50, -41, -37, -39, -23, -11, -21, -18, -20, -18, -23, -34, -48,
  -50, -41, -29, -28, -20, -16, -10, -16, -22, -25, -22, -45, -41,
  -43, -40, -25, -27, -16, -21, -14,  -9, -28, -39, -40, -46, -55
 };


 #endif
 uint16_t   zone_center[]={247,239,231,223,215,207,199,191,183,175,167,159,151, 247, 239, 231, 223, 215};
 // Timing Budget Options:  15, 20, 33, 50, 100, 200, 500
 uint16_t TimingBudget = 15;

 uint16_t current_zone=0;
 char BigBuff[4000];
 char VL53L1X_BUFFER[60];	/* Create a buffer to get data */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void PlotPolarData(uint8_t SensorNum, uint8_t CurrentZone, uint8_t NumOfZones, uint16_t Distance);
void TurnOnSensor(uint8_t SensorNum);
void ResetAllSensors(void);
void ResetAndInitializeAllSensors(void);
void SystemClock_Config(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t dataRead;
void ResetAndInitializeAllSensors(void)
{
	uint8_t i, Sensor, error = 0;
	uint8_t Bootstate = 0;
	int16_t Offset;
	ResetAllSensors();
	HAL_Delay(10);
	for (i = 0; i < NumOfTOFSensors; i++)
	{
		TurnOnSensor(i);
		HAL_Delay(5);
		error += VL53L1X_BootState(Dev_init, &Bootstate);
		while (Bootstate != 0x03)
		{
			HAL_Delay(5);
			error += VL53L1X_BootState(Dev_init, &Bootstate);
		}
		VL53L1X_SensorInit(Dev_init);	/* Initialize sensor  */
		VL53L1X_SetI2CAddress(Dev_init, Devs[i]);	/* Change i2c address Left is now 0x62 and Dev1 */
		dataRead = ReadRegister8(Devs[i], 0x10f);
		dataRead = ReadRegister8(Devs[i], 0x110);
	}
	UART_Print("All Chips booted\n");

	for (Sensor = 0; Sensor < NumOfTOFSensors; Sensor++)
	{
		VL53L1X_SetDistanceMode(Devs[Sensor], 1);
		VL53L1X_SetTimingBudgetInMs(Devs[Sensor], TimingBudget);
		VL53L1X_SetInterMeasurementInMs(Devs[Sensor], TimingBudget);
		VL53L1X_SetROI(Devs[Sensor], WidthOfSPADsPerZone, 6);
		WriteRegister8(Devs[Sensor], ROI_CONFIG__USER_ROI_CENTRE_SPAD, zone_center[0] - 0);
		error = VL53L1X_GetOffset(Devs[Sensor], &Offset);
		VL53L1X_SetOffset(Devs[Sensor], Offset + 40);
	}
	for (Sensor = 0; Sensor < NumOfTOFSensors; Sensor++)
	{
		VL53L1X_StartRanging(Devs[Sensor]);
		HAL_Delay(1);
	}
	if (error !=0)
	{
		UART_Print("Some Errors seen\n");
	}

}

float OldAngle;
double SystemAngle;
void PlotPolarData(uint8_t SensorNum, uint8_t CurrentZone, uint8_t NumOfZones, uint16_t Distance)
{
	double PartZoneAngle;

	float CorrectedDistance = 0;

	if (Distance > 60000)
	{
		Distance = 0;
	}
	PartZoneAngle = (StartingZoneAngle + ZoneFOVChangePerStep*CurrentZone) - (HorizontalFOVofSensor / 2.0);
	SystemAngle = -80 + 20.0*SensorNum + PartZoneAngle;
	CorrectedDistance = pow(pow(RadarCircleRadius,2) + pow(Distance, 2) - (2 * RadarCircleRadius * Distance * cos((180 - PartZoneAngle)/(180) * Pi)), 0.5);
	if (CorrectedDistance < 55)
	{
		CorrectedDistance = 55;
	}
	LidarAngle[SensorNum*13+CurrentZone] = SystemAngle;
	LidarDistance[SensorNum*13+CurrentZone] = (uint16_t)CorrectedDistance;
}

void TurnOnSensor(uint8_t SensorNum)
{
	switch (SensorNum)
	{
		case 0:
			// GPIO PC2
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
			break;
		case 1:
			// GPIO PC3
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
			break;
		case 2:
			// GPIO PC4
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
			break;
		case 3:
			// GPIO PC5
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
			break;
		case 4:
			// GPIO PC6
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
			break;
		case 5:
			// GPIO PC7
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
			break;
		case 6:
			// GPIO PC8
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
			break;
		case 7:
			// GPIO PC9
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
			break;
		case 8:
			// GPIO PC10
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
			break;
		case 9:
			// GPIO PC11
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
			break;
		case 10:
			// GPIO PC12
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
			break;
		case 11:
			// GPIO PB0
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
			break;
		case 12:
			// GPIO PB1
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
			break;
		case 13:
			// GPIO PB2
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
			break;
		case 14:
			// GPIO PB3
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
			break;
		case 15:
			// GPIO PB4
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
			break;
		case 16:
			// GPIO PB5
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
			break;
		case 17:
			// GPIO PB6
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
			break;
		case 18:
			// GPIO PB7
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
			break;
	}
}

void ResetAllSensors(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	VL53L1X_ERROR error = 0;
	uint32_t i=0;
	uint8_t Zone, Sensor, Timeout;

	uint32_t TimeStart, TimeEnd, TotalTime, CurrentTime;
	uint8_t Sensorcheck;
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
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
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
