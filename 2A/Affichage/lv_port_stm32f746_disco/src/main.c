
#include "stm32f7xx.h"
#include "lvgl/lvgl.h"

#include "hal_stm_lvgl/tft/tft.h"
#include "hal_stm_lvgl/touchpad/touchpad.h"

#include "lvgl/demos/lv_demos.h"
#include "lvgl/examples/lv_examples.h"

static lv_style_t style;
static const lv_font_t * font_large;

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

    lv_init();

    tft_init();
    touchpad_init();

    // lv_demo_benchmark();
    // lv_demo_music();
    // lv_demo_stress();
    // lv_demo_widgets();
    // lv_example_style_8();


    font_large = &lv_font_montserrat_14;
    lv_style_init(&style);
    lv_style_set_text_font(&style, font_large);

    lv_obj_t * panel1 = lv_obj_create(lv_scr_act());
    lv_obj_set_height(panel1, LV_SIZE_CONTENT);
    lv_obj_align(panel1, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(panel1, 250, 150);

    LV_IMG_DECLARE(n0);
    lv_obj_t * chiffre0 = lv_img_create(panel1);
    lv_img_set_src(chiffre0, &n0);

    lv_obj_t * text = lv_label_create(panel1);
    lv_obj_add_style(text, &style, 0);
    lv_label_set_text(text, "Score : ");


    static lv_coord_t grid_col_dsc[] = {100, 100, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t grid_row_dsc[] = {100,LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(panel1, grid_col_dsc, grid_row_dsc);

    lv_obj_set_grid_cell(chiffre0, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_set_grid_cell(text, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);

    /*LV_IMG_DECLARE(n0);
    lv_obj_t *img1 = lv_img_create(lv_scr_act());
    lv_img_set_src(img1, &n0);
    lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);*/
    //lv_obj_set_size(img1, 480, 272);

    /*lv_obj_t * img2 = lv_img_create(lv_scr_act());
    lv_img_set_src(img2, LV_SYMBOL_OK "Accept");
    lv_obj_align_to(img2, img1, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);*/

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

