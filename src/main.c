#include "main.h"

#include <PE_Button.h>
#include <PE_nRF24L01.h>

#include "led.h"
#include "spi.h"

PE_Button_Key_t key1;
PE_nRF24_t nRF24;

SPI_HandleTypeDef SPIn;

void SystemClock_Config(void);
void MX_GPIO_Init();

volatile uint8_t val1 = 0;

int main()
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_LED_Init();
    MX_LED_OFF(1);
    MX_SPI1_Init(&SPIn);

    PE_nRF24_Config_t config;

    config.rfChannel    = 1;
    config.payloadWidth = 1;
    config.crcScheme    = PE_nRF24_CRC_SCHEME_1BYTE;
    config.txPower      = PE_nRF24_TX_POWER_18dBm;
    config.dataRate     = PE_nRF24_DATA_RATE__250KBPS;
    config.retryCount   = PE_nRF24_RETRY_COUNT__1;
    config.retryDelay   = PE_nRF24_RETRY_DELAY_1000us;
    config.addressWidth = PE_nRF24_ADDR_WIDTH_5BIT;
    config.addressRX    = (uint8_t *) PE_nRF24_TEST_ADDRESS;
    config.addressTX    = (uint8_t *) PE_nRF24_TEST_ADDRESS;

    if (PE_nRF24L01_initialize(&nRF24, &config) != PE_nRF24_RESULT_OK) {
        Error_Handler(__FILE__, __LINE__);
    }

    uint32_t start = HAL_GetTick();

    while (1) {
#ifdef PE_nRF_MASTER
        PE_Button_dispatchKey(&key1, HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0, HAL_GetTick());

        uint8_t data[] = {val1};

        if (HAL_GetTick() - start > 100) {
            start = HAL_GetTick();

            PE_nRF24L01_setCE0(&nRF24);

            PE_nRF24L01_setDirection(&nRF24, PE_nRF24_DIRECTION_TX);
            PE_nRF24L01_flushTX(&nRF24);
            PE_nRF24L01_setTXPayload(&nRF24, data);

            PE_nRF24L01_setCE1(&nRF24);
        }

        MX_LED_OFF(0);
#endif
#ifdef PE_nRF_SLAVE
        MX_LED_OFF(0);
#endif
    }
}

void PE_Button_onPress(PE_Button_Key_t *key) {
    val1 = 1;
    MX_LED_ON(10);
    (void) key;
}

void PE_Button_onHoldRepeated(PE_Button_Key_t *key) {
    val1 = 1;
    MX_LED_ON(10);
    (void) key;
}

void PE_Button_onRelease(PE_Button_Key_t *key) {
    val1 = 0;
    MX_LED_OFF(0);
    (void) key;
}

uint32_t PE_nRF24L01_getMillis(void) {
    return HAL_GetTick();
}

PE_nRF24_RESULT_t PE_nRF24L01_readData(PE_nRF24_t *handle, uint8_t *data, uint8_t size) {
    (void) handle;

    if (HAL_SPI_Receive(&SPIn, data, size, 1000) != HAL_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24L01_sendData(PE_nRF24_t *handle, uint8_t *data, uint8_t size) {
    (void) handle;

    if (HAL_SPI_Transmit(&SPIn, data, size, 1000) != HAL_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

void PE_nRF24L01_setCE0(PE_nRF24_t *handle) {
    //PA2 ------> nRF24_CE
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
    (void) handle;
}

void PE_nRF24L01_setCE1(PE_nRF24_t *handle) {
    //PA2 ------> nRF24_CE
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
    (void) handle;
}

void PE_nRF24L01_setSS0(PE_nRF24_t *handle) {
    if (SPIn.Instance == SPI1) {
        //PA4 ------> SPI1_NSS
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    }
    if (SPIn.Instance == SPI2) {
        //PB12 ------> SPI2_NSS
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    }
    (void) handle;
}

void PE_nRF24L01_setSS1(PE_nRF24_t *handle) {
    if (SPIn.Instance == SPI1) {
        //PA4 ------> SPI1_NSS
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    }
    if (SPIn.Instance == SPI2) {
        //PB12 ------> SPI2_NSS
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    }
    (void) handle;
}

void PE_nRF24L01_onMaxRetransmit(PE_nRF24_t *handle) {
    (void) handle;

}

#ifdef PE_nRF_MASTER
//void PE_nRF24L01_onTXComplete(PE_nRF24_t *handle) {
//    (void) handle;
//    MX_LED_ON(5);
//}
#endif

#ifdef PE_nRF_SLAVE
void PE_nRF24_onRXComplete(PE_nRF24_t *handle) {
    val1 = 1;
    (void) handle;
    MX_LED_ON(5);
}
#endif

void EXTI3_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_3) {
        if (PE_nRF24L01_handleIRQ(&nRF24) != PE_nRF24_RESULT_OK) {
            //MX_LED_ON(2);
        }
    }
}

void MX_GPIO_Init() {
    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO clock enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* GPIO Configuration */
    GPIO_InitStruct.Pin   = GPIO_PIN_15;
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;

    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    //PA2 ------> nRF24_CE
    //PA3 ------> nRF24_IRQ
    GPIO_InitStruct.Pin   = GPIO_PIN_2;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin   = GPIO_PIN_3;
    GPIO_InitStruct.Mode  = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Initializes the CPU clock source
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL8;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler(__FILE__, __LINE__);
    }

    // Initializes the CPU, AHB and APB buses clocks
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK
                                  | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1
                                  | RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler(__FILE__, __LINE__);
    }
}

/**
 * @brief  Initializes the Global MSP.
 * @param  None
 * @retval None
 */
void HAL_MspInit(void)
{
    /* Alternate Function I/O clock enable */
    __HAL_RCC_AFIO_CLK_ENABLE();

    /* Power interface clock enable */
    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_AFIO_REMAP_SWJ_NOJTAG();
}

/**
 * @brief  DeInitializes the Global MSP.
 * @param  None
 * @retval None
 */
void HAL_MspDeInit(void)
{}

/**
 * @brief   This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1) {
        __NOP();
    }
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1) {
        __NOP();
    }
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1) {
        __NOP();
    }
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1) {
        __NOP();
    }
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
    HAL_IncTick();
}

/**
 * @brief This function is executed in case of error occurrence.
 *
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void Error_Handler(const char * file, int line)
{
    UNUSED(file);
    UNUSED(line);

    const uint16_t sequence[] = {
            100,
            200,
            100,
            200,
            100,
            1200,
    };

    while (1) {
        MX_LED_PLAY(sequence, 6);
        __NOP();
    }
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    UNUSED(file);
    UNUSED(line);
}
#endif
