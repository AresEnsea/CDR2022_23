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
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

CRC_HandleTypeDef hcrc;

DMA2D_HandleTypeDef hdma2d;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CRC_Init(void);
static void MX_DMA2D_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
TS_StateTypeDef  ts; // typedef struct

char xTouchStr[10];
/* USER CODE END 0 */

#define CHAR_HAUTEUR 24
#define CHAR_REP 8
#define CHAR_LARGEUR 17

// ROUGE VERT BLEU ALPHA
static const unsigned char couleur_magenta[4] = {0xff, 0x00, 0xff, 0xff};
static const unsigned char couleur_cyan[4] = {0x00, 0xff, 0xff, 0xff};
static const unsigned char couleur_jaune[4] = {0xff, 0xff, 0xff, 0xff};
static const unsigned char couleur_noir[4] = {0xff, 0x00, 0x00, 0x00};
int min(int a, int b)
{
	return a < b ? a : b;
}
void chiffre(char a_dessiner, const unsigned char* bg, const unsigned char* fg, uint32_t x, uint32_t y)
{
	static unsigned char tableau_converti[CHAR_LARGEUR * CHAR_REP * 4];

	DMA2D_HandleTypeDef hdma2d;
	hdma2d.Instance = DMA2D;

	hdma2d.Init.Mode = DMA2D_M2M;
	hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
	hdma2d.Init.OutputOffset = BSP_LCD_GetXSize() - CHAR_LARGEUR * CHAR_REP;

	hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
	hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
	hdma2d.LayerCfg[1].InputOffset = 0;

	unsigned tab_src_idx = (a_dessiner - 0x20) * (CHAR_HAUTEUR * (((CHAR_LARGEUR)/8)+1));
	for(int num_ligne = 0; num_ligne < CHAR_HAUTEUR; ++num_ligne)
	{
		int tab_idx = 0;
		// on converti une ligne de la bitmap en pixel ARGB8888
		for(int i = 0; i < ((CHAR_LARGEUR)/8)+1; ++i)
		{
			int bit_idx = 7;
			for(int j = 1; j < min(CHAR_LARGEUR - (i * 8), 8); ++j)
			{
				const unsigned char* couleur = (Font24.table[tab_src_idx] & (1 << bit_idx)) ? fg : bg;
				for(int k = 0; k < CHAR_REP; ++k)
				{
					tableau_converti[tab_idx + 0] = couleur[0];
					tableau_converti[tab_idx + 1] = couleur[1];
					tableau_converti[tab_idx + 2] = couleur[2];
					tableau_converti[tab_idx + 3] = couleur[3];
					tab_idx += 4;
				}
				--bit_idx;
			}
			++tab_src_idx;
		}
		// On affiche la ligne CHAR_REP fois sur tant de lignes differentes de l'écran
		for(int rep = 0; rep < CHAR_REP; ++rep)
		{
	    	HAL_DMA2D_Init(&hdma2d);
	    	HAL_DMA2D_ConfigLayer(&hdma2d, 1);
	    	HAL_DMA2D_Start(&hdma2d, (uintptr_t)tableau_converti, LCD_FB_START_ADDRESS + (x + (y + num_ligne * CHAR_REP + rep) * BSP_LCD_GetXSize()) * 4, CHAR_LARGEUR * CHAR_REP , 1);
	    	HAL_DMA2D_PollForTransfer(&hdma2d, 10);
		}
	}
}


void DMA2D_FillRect(uint32_t color, uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    	DMA2D_HandleTypeDef hdma2d;
    	hdma2d.Instance = DMA2D;

    	hdma2d.Init.Mode = DMA2D_R2M;
    	hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
    	hdma2d.Init.OutputOffset = BSP_LCD_GetXSize() - width;

    	HAL_DMA2D_Init(&hdma2d);
    	HAL_DMA2D_Start(&hdma2d, color, LCD_FB_START_ADDRESS + (x + y * BSP_LCD_GetXSize()) * 4, width, height);
    	HAL_DMA2D_PollForTransfer(&hdma2d, 10);
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	/* USER CODE BEGIN 1 */
	/* Enable the CPU Cache */
	/* Enable I-Cache */
	SCB_EnableICache();
	/* Enable D-Cache */
	SCB_EnableDCache();
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
  MX_CRC_Init();
  MX_DMA2D_Init();
  /* USER CODE BEGIN 2 */

  BSP_SDRAM_Init(); /* Initializes the SDRAM device */
  __HAL_RCC_CRC_CLK_ENABLE(); /* Enable the CRC Module */

  BSP_TS_Init(480, 272); // Déclaration des pixels

  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
  BSP_LCD_DisplayOn();

  BSP_LCD_SelectLayer(0);
  BSP_LCD_Clear(LCD_COLOR_BLACK);

  BSP_LCD_SetTextColor(0xffffffff);
  BSP_LCD_SetBackColor(0x00000000);
  BSP_LCD_SetFont(&Font24);

  BSP_LCD_DisplayStringAt(0, 20, (uint8_t *)"Bonjour", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 200, (uint8_t *)"Appuyer sur l'ecran",CENTER_MODE);

  //BSP_LCD_SetTextColor(0xff0000f8); // bgra
  //BSP_LCD_FillRect(0, 0, 200, 200);

  /* USER CODE END 2 */

    //DMA2D_FillRect(0, 20, CENTER_MODE);
    //DMA2D_FillRect(0xfff00c3e, 0, 0, BSP_LCD_GetXSize()/2, BSP_LCD_GetYSize());
    //DMA2D_FillRect(0xffffffff, BSP_LCD_GetXSize()/2, 0, BSP_LCD_GetXSize()/2, BSP_LCD_GetYSize());
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
	  BSP_TS_GetState(&ts);
	  if(ts.touchDetected >= 1)
	  {
		  BSP_LCD_Clear(LCD_COLOR_BLACK);
		  int BSP = 0;
		  while(BSP == 0)
		  {
			  BSP_LCD_DisplayStringAt(0, 200, (uint8_t *)"Bienvenue",CENTER_MODE);

			  BSP_TS_GetState(&ts);
			  sprintf(xTouchStr, "X: %3d", ts.touchX[0]);
			  BSP_LCD_DisplayStringAt(20, 20, (uint8_t *)xTouchStr, LEFT_MODE);

			  sprintf(xTouchStr, "Y: %3d", ts.touchY[0]);
			  BSP_LCD_DisplayStringAt(20, 60, (uint8_t *)xTouchStr, LEFT_MODE);
			  HAL_Delay(5);

			  BSP_LCD_FillRect(350, 0, 130, 50);
			  BSP_LCD_DisplayStringAt(350, 10, (uint8_t *)"Retour :", RIGHT_MODE);
			  if((ts.touchX[0] >= 350 && ts.touchX[0] <= 480) && (ts.touchY[0] >= 0 && ts.touchY[0] <= 50))
			  {
				  BSP == 1;
			  }
		  }
	  }

	  //chiffre('1', couleur_noir, couleur_jaune, 20, 20);
	  //chiffre('3', couleur_magenta, couleur_noir, 160, 20);
	  //chiffre('5', couleur_noir, couleur_cyan, 300, 20);

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
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief DMA2D Initialization Function
  * @param None
  * @retval None
  */
static void MX_DMA2D_Init(void)
{

  /* USER CODE BEGIN DMA2D_Init 0 */

  /* USER CODE END DMA2D_Init 0 */

  /* USER CODE BEGIN DMA2D_Init 1 */

  /* USER CODE END DMA2D_Init 1 */
  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_M2M;
  hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
  hdma2d.Init.OutputOffset = 0;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0;
  if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DMA2D_Init 2 */

  /* USER CODE END DMA2D_Init 2 */

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
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

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
