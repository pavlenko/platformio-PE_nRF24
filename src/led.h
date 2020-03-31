/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C" {
#endif

//TODO check for pull up diode

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

#define LED_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED_GPIO_PORT         GPIOC
#define LED_GPIO_PIN          GPIO_PIN_13
#define LED_STATE_ON          GPIO_PIN_RESET
#define LED_STATE_OFF         GPIO_PIN_SET

/* Exported function prototypes --------------------------------------------- */
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void MX_LED_Init(void);

void MX_LED_ON(uint8_t counter);

void MX_LED_OFF(uint8_t force);

void MX_LED_PLAY(const uint16_t *data, uint8_t size);

#ifdef __cplusplus
}
#endif

#endif //__LED_H
