/* Includes ------------------------------------------------------------------*/

#include "led.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static volatile uint32_t LED_counter;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void MX_LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO clock enable */
    LED_GPIO_CLK_ENABLE();

    /* GPIO Configuration */
    GPIO_InitStruct.Pin   = LED_GPIO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;

    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}

void MX_LED_ON(uint8_t counter)
{
    LED_counter = HAL_GetTick() + counter;

    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, LED_STATE_ON);
}

void MX_LED_OFF(uint8_t force)
{
    if (force == 0 && LED_counter > HAL_GetTick()) {
        return;
    }

    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, LED_STATE_OFF);
}

void MX_LED_PLAY(const uint16_t *data, uint8_t size)
{
    uint8_t index;

    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, LED_STATE_OFF);

    for (index = 0; index < size; index++) {
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
        HAL_Delay(*(data + index));
    }
}