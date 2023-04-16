#include "../inc/main.h"
#include "stm32f7xx.h"
#include "lvgl/lvgl.h"

#include "hal_stm_lvgl/tft/tft.h"
#include "hal_stm_lvgl/touchpad/touchpad.h"

#include <string.h>
#include <stdio.h>

static void event_btn(lv_event_t * e);
static lv_style_t style_btn_green;
static lv_style_t style_btn_blue;

void Style_btn_confirm();
static lv_style_t style;
static lv_style_t style_pr;
static lv_style_transition_dsc_t trans;
static lv_style_prop_t props[] = {LV_STYLE_OUTLINE_WIDTH, LV_STYLE_OUTLINE_OPA, 0};
int confirm = 0;

void HAL_UART_MspInit(UART_HandleTypeDef* huart);
static void MX_GPIO_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_USART1_UART_Init(void);
void Error_Handler(void);
UART_HandleTypeDef huart6;
UART_HandleTypeDef huart1;

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

    Style_btn_confirm();
    lv_style_init(&style_btn_green);
    lv_style_set_bg_color(&style_btn_green, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_radius(&style_btn_green, 80);
    lv_style_set_width(&style_btn_green, 64);
    lv_style_set_height(&style_btn_green, 55);

    lv_style_init(&style_btn_blue);
    lv_style_set_bg_color(&style_btn_blue, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_radius(&style_btn_blue, 80);
    lv_style_set_width(&style_btn_blue, 64);
    lv_style_set_height(&style_btn_blue, 55);

    LV_IMG_DECLARE(TableRedi);
    lv_obj_t * table = lv_img_create(lv_scr_act());
    lv_obj_set_size(table, 480, 272);
    lv_obj_center(table);
    lv_img_set_src(table, &TableRedi);

    uint32_t i;
    for(i = 0; i < 10; i++) {
    	lv_obj_t * btn = lv_btn_create(table);
    	lv_obj_add_flag(btn, LV_OBJ_FLAG_EVENT_BUBBLE);

    	switch(i)
    	{
    	case 0:
    		lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 36-32, 30-27);
    		lv_obj_add_style(btn, &style_btn_green, 0);
    		break;
    	case 1:
    		lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 180-32, 30-27);
    		lv_obj_add_style(btn, &style_btn_blue, 0);
            break;
        case 2:
        	lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 300-32, 30-27);
        	lv_obj_add_style(btn, &style_btn_green, 0);
        	break;
        case 3:
        	lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 444-32, 30-27);
        	lv_obj_add_style(btn, &style_btn_blue, 0);
        	break;
        case 4:
        	lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 444-32, 98-27);
        	lv_obj_add_style(btn, &style_btn_green, 0);
        	break;
        case 5:
        	lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 444-32, 173-27);
        	lv_obj_add_style(btn, &style_btn_blue, 0);
            break;
        case 6:
        	lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 444-32, 241-27);
        	lv_obj_add_style(btn, &style_btn_green, 0);
            break;
        case 7:
        	lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 300-32, 241-27);
        	lv_obj_add_style(btn, &style_btn_blue, 0);
            break;
        case 8:
        	lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 180-32, 241-27);
        	lv_obj_add_style(btn, &style_btn_green, 0);
        	break;
        case 9:
        	lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 36-32, 241-27);
        	lv_obj_add_style(btn, &style_btn_blue, 0);
        	break;
        default:
            break;
    	}

    	lv_obj_t * label = lv_label_create(btn);
    	lv_label_set_text_fmt(label, "%"LV_PRIu32, i);
    	lv_obj_center(label);
    }

    lv_obj_t * btn1 = lv_btn_create(table);
    lv_obj_remove_style_all(btn1);
    lv_obj_add_style(btn1, &style, 0);
    lv_obj_add_style(btn1, &style_pr, LV_STATE_PRESSED);
    lv_obj_set_size(btn1, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_center(btn1);
    lv_obj_add_flag(btn1, LV_OBJ_FLAG_EVENT_BUBBLE);

    lv_obj_t * lab = lv_label_create(btn1);
    lv_label_set_text(lab, "Confirm");
    lv_obj_center(lab);

    lv_obj_add_event_cb(table, event_btn, LV_EVENT_CLICKED, NULL);

    HAL_UART_MspInit(&huart1);
    //HAL_UART_MspInit(&huart6);
    //HAL_UART_Receive_IT(&huart1, pData, 4);
    //HAL_UART_Receive_IT(&huart6, pData, 3);

    while (1)
    {
        HAL_Delay(5);
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

void Style_btn_confirm()
{
	lv_style_init(&style);

	lv_style_set_radius(&style, 3);

	lv_style_set_bg_opa(&style, LV_OPA_100);
	lv_style_set_bg_color(&style, lv_color_black());
	lv_style_set_bg_grad_color(&style, lv_palette_darken(LV_PALETTE_GREY, 2));
	lv_style_set_bg_grad_dir(&style, LV_GRAD_DIR_VER);

	lv_style_set_border_opa(&style, LV_OPA_40);
	lv_style_set_border_width(&style, 2);
	lv_style_set_border_color(&style, lv_palette_main(LV_PALETTE_GREY));

	lv_style_set_shadow_width(&style, 8);
	lv_style_set_shadow_color(&style, lv_palette_main(LV_PALETTE_GREY));
	lv_style_set_shadow_ofs_y(&style, 8);

	lv_style_set_outline_opa(&style, LV_OPA_COVER);
	lv_style_set_outline_color(&style, lv_color_black());

	lv_style_set_text_color(&style, lv_color_white());
	lv_style_set_pad_all(&style, 10);


	lv_style_init(&style_pr);

	lv_style_set_outline_width(&style_pr, 30);
	lv_style_set_outline_opa(&style_pr, LV_OPA_TRANSP);

	lv_style_set_translate_y(&style_pr, 5);
	lv_style_set_shadow_ofs_y(&style_pr, 3);
	lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_GREY, 2));
	lv_style_set_bg_grad_color(&style_pr, lv_palette_darken(LV_PALETTE_GREY, 4));


	lv_style_transition_dsc_init(&trans, props, lv_anim_path_linear, 300, 0, NULL);

	lv_style_set_transition(&style_pr, &trans);
}

static void event_btn(lv_event_t * e)
{
	int fullr;
	int Index;
	int fullcolor;
	char gbuf[5];

    lv_obj_t * target = lv_event_get_target(e);
    lv_obj_t * cont = lv_event_get_current_target(e);
    if(target == cont) return;

    Index = lv_obj_get_index(target);
    if(confirm == 0 && Index == 10)
    {
    	confirm++;
    	lv_color_t r = lv_palette_main(LV_PALETTE_RED);
    	fullr = r.full;
    	for(int i = 0;i<10;i++)
    	{
    		lv_obj_t * obj = lv_obj_get_child(cont, i);
    		lv_color_t color = lv_obj_get_style_bg_color(obj, LV_PART_MAIN);
    		fullcolor = color.full;
    		if(fullcolor == fullr)
    		{
    			lv_obj_t * label = lv_obj_get_child(target, 0);
    		    lv_label_set_text_fmt(label, "Confirm: %"LV_PRIu32, i);

    			int taille=0;
    			taille=sprintf(gbuf,"%d \r\n",i);
    			if(taille>0)
    				HAL_UART_Transmit(&huart6, (unsigned char *)gbuf, taille, 1);
    		}

    		if(i%2 == 0)
    		{
    			lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_GREEN), 0);
    		}
    		else
    		{
    			lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE), 0);
    		}
    	}

    }
    else if(confirm == 0 && Index != 10)
    {
    	for(int i = 0;i<10;i++)
    	{
    		lv_obj_t * obj = lv_obj_get_child(cont, i);
    	    if(obj != target)
    	    {
    	    	if(i%2 == 0)
    	    	{
    	    		lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_GREEN), 0);
    	    	}
    	    	else
    	    	{
    	    		lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE), 0);
    	    	}
    	    }
    	    else
    	    {
    	    	lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_RED), 0);
    	    }
    	}
    }
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
