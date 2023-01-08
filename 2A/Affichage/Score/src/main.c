#include "../inc/main.h"
#include "stm32f7xx.h"
#include "lvgl/lvgl.h"

#include "hal_stm_lvgl/tft/tft.h"
#include "hal_stm_lvgl/touchpad/touchpad.h"

static lv_obj_t * chiffre0;
static lv_obj_t * chiffre1;
static lv_obj_t * chiffre2;
static lv_obj_t * indice;

LV_IMG_DECLARE(n0);
LV_IMG_DECLARE(n1);
LV_IMG_DECLARE(n2);
LV_IMG_DECLARE(n3);
LV_IMG_DECLARE(n4);
LV_IMG_DECLARE(n5);
LV_IMG_DECLARE(n6);
LV_IMG_DECLARE(n7);
LV_IMG_DECLARE(n8);
LV_IMG_DECLARE(n9);

UART_HandleTypeDef huart6;
UART_HandleTypeDef huart1;
uint8_t pData[5];
uint8_t Tab[3] = {'0','0','0'};

void HAL_UART_MspInit(UART_HandleTypeDef* huart);
static void MX_GPIO_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_USART1_UART_Init(void);
void Error_Handler(void);

static void SystemClock_Config(void);

int main(void)
{
    HAL_Init();

    /* Configure the system clock to 216 MHz */
    SystemClock_Config();

    /* Enable I-Cache */
    SCB_EnableICache();

    /* Enable D-Cache */
    SCB_EnableDCache();

    MX_GPIO_Init();
    MX_USART6_UART_Init();
    MX_USART1_UART_Init();

    lv_init();

    tft_init();
    touchpad_init();

    lv_obj_t * panel1 = lv_obj_create(lv_scr_act());
    lv_obj_set_height(panel1, LV_SIZE_CONTENT);
    lv_obj_align(panel1, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(panel1, 480, 272);

    lv_obj_clear_flag(panel1, LV_OBJ_FLAG_SCROLLABLE); // Permet de rendre l'objet non défilable
    lv_obj_set_scrollbar_mode(panel1, LV_SCROLLBAR_MODE_OFF); // Permet de retirer les scrollbar

    chiffre0 = lv_img_create(panel1);
    chiffre1 = lv_img_create(panel1);
    chiffre2 = lv_img_create(panel1);

    lv_img_set_src(chiffre0, &n0);
    lv_img_set_src(chiffre1, &n0);
    lv_img_set_src(chiffre2, &n0);

    static lv_coord_t grid_col_dsc[] = {150, 150, 150, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t grid_row_dsc[] = {260, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(panel1, grid_col_dsc, grid_row_dsc);

    lv_obj_set_grid_cell(chiffre0, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_set_grid_cell(chiffre1, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_set_grid_cell(chiffre2, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_CENTER, 0, 1);

    HAL_UART_MspInit(&huart1);
    HAL_UART_MspInit(&huart6);
    //HAL_UART_Receive_IT(&huart1, pData, 4);
    HAL_UART_Receive_IT(&huart6, pData, 4);

    while (1)
    {
        HAL_Delay(1);
        lv_task_handler();
    }
}

static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /*##-1- System Clock Configuration #########################################*/
    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 400;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 8;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /* Activate the Over-Drive mode */
    HAL_PWREx_EnableOverDrive();

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
}

static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  huart6.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart6.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

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
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(huart->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PB7     ------> USART1_RX
    PA9     ------> USART1_TX
    */
    GPIO_InitStruct.Pin = VCP_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(VCP_RX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = VCP_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(VCP_TX_GPIO_Port, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(huart->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspInit 0 */

  /* USER CODE END USART6_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART6;
    PeriphClkInitStruct.Usart6ClockSelection = RCC_USART6CLKSOURCE_PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_USART6_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART6 GPIO Configuration
    PC7     ------> USART6_RX
    PC6     ------> USART6_TX
    */
    GPIO_InitStruct.Pin = ARDUINO_RX_D0_Pin|ARDUINO_TX_D1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USART6 interrupt Init */
    HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
  /* USER CODE BEGIN USART6_MspInit 1 */

  /* USER CODE END USART6_MspInit 1 */
  }

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	int mes = 0;
	int i;
	HAL_UART_Transmit(&huart1, pData, sizeof(pData), HAL_MAX_DELAY);
	if(pData[0] == '0')
	{
		for(i=2; i>0;i--)
		{
			mes = Tab[i] - pData[i+1];
			if(mes < 0)
			{
				Tab[i] = mes + 58;
				Tab[i-1] = Tab[i-1] - 1;
			}
			else
			{
				Tab[i] = mes + 48;
			}
		}
	}
	else
	{
		for(i=2; i>=0;i--)
		{
			mes = Tab[i] + pData[i+1];
			if(mes >= 106)
			{
				Tab[i] = mes - 58;
				Tab[i-1] = Tab[i-1] + 1;
			}
			else
			{
				Tab[i] = mes - 48;
			}
		}
	}

	for(i=0; i<3;i++)
	{
		if(Tab[i] < '0')
		{
			Tab[i] = '0';
		}
		else if(Tab[i] > '9')
		{
			Tab[i] = '9';
		}
	}

	for(int i=0;i<3; i++)
	{
		if(i == 0)
		{
			indice = chiffre0;
		}
		else if(i == 1)
		{
			indice = chiffre1;
		}
		else if(i == 2)
		{
			indice = chiffre2;
		}

	    	    	switch(Tab[i])
	    	    	{
	    	    	case '0':
	    	    		lv_img_set_src(indice, &n0);
	    	    		break;
	    	    	case '1':
	    	    		lv_img_set_src(indice, &n1);
	    	    		break;
	    	    	case '2':
	    	    		lv_img_set_src(indice, &n2);
	    	    		break;
	    	    	case '3':
	    	    		lv_img_set_src(indice, &n3);
	    	    		break;
	    	    	case '4':
	    	    		lv_img_set_src(indice, &n4);
	    	    		break;
	    	    	case '5':
	    	    	   	lv_img_set_src(indice, &n5);
	    	    	   	break;
	    	    	case '6':
	    	    	    lv_img_set_src(indice, &n6);
	    	    	    break;
	    	    	case '7':
	    	    	    lv_img_set_src(indice, &n7);
	    	    	    break;
	    	    	case '8':
	    	    	    lv_img_set_src(indice, &n8);
	    	    	    break;
	    	    	case '9':
	    	    	    lv_img_set_src(indice, &n9);
	    	    	    break;
	    	    	default:
	    	    		lv_img_set_src(indice, &n0);
	    	    		break;
	    	    	}
	}
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pins : PC7 PC6 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

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
