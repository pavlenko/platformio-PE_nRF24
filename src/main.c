#include "main.h"

#include <PE_Button.h>
#include <PE_nRF24_api.h>
#include <PE_nRF24_irq.h>

#include "spi.h"

//TODO constant passed from compiler, remove after write code
#ifndef PE_nRF_SLAVE
#define PE_nRF_SLAVE
#endif

PE_Button_Key_t key1;
PE_nRF24_t nRF24;

SPI_HandleTypeDef SPIn;

void SystemClock_Config(void);
void MX_GPIO_Init();

static uint8_t received = 0;

int main()
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init(&SPIn);

    nRF24.config.addressWidth = PE_nRF24_ADDR_WIDTH_3BIT;
    nRF24.config.dataRate     = PE_nRF24_DATA_RATE_1000KBPS;
    nRF24.config.rfChannel    = 1;
    nRF24.config.crcScheme    = PE_nRF24_CRC_SCHEME_OFF;
    nRF24.config.txPower      = PE_nRF24_TX_POWER__6dBm;
    nRF24.config.retryCount   = 0;
    nRF24.config.retryDelay   = 0;

    if (PE_nRF24_configureRF(&nRF24) != PE_nRF24_RESULT_OK) {
        Error_Handler(__FILE__, __LINE__);
    }

#ifdef PE_nRF_MASTER
    const char addr[] = PE_nRF24_TEST_ADDRESS;
    const char data[] = "Hello";
#endif

#ifdef PE_nRF_SLAVE
    // Initialize RX
    PE_nRF24_configRX_t nRF24_configRX;

    nRF24_configRX.address     = (uint8_t *) PE_nRF24_TEST_ADDRESS;
    nRF24_configRX.autoACK     = PE_nRF24_AUTO_ACK_OFF;
    nRF24_configRX.payloadSize = 32;

    if (PE_nRF24_configureRX(&nRF24_handle, &nRF24_configRX, PE_nRF24_PIPE_RX0) != PE_nRF24_RESULT_OK) {
        Error_Handler(__FILE__, __LINE__);
    }

    PE_nRF24_attachRXPipe(&nRF24, PE_nRF24_PIPE_RX0);
    PE_nRF24_attachIRQ(&nRF24, PE_nRF24_IRQ_RX_DR);
#endif

    while (1) {
#ifdef PE_nRF_MASTER
        PE_Button_dispatchKey(&key1, HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == 0, HAL_GetTick());

        if (PE_nRF24_sendPacket(&nRF24_handle, (uint8_t *) addr, (uint8_t *) data, strlen(data), 10) != PE_nRF24_RESULT_OK) {
            Error_Handler(__FILE__, __LINE__);
        }
#endif
#ifdef PE_nRF_SLAVE
        if (PE_nRF24_readPacket(&nRF24_handle, (uint8_t *) data, size, 0) != PE_nRF24_RESULT_OK) {
            Error_Handler(__FILE__, __LINE__);
        }

        received = 0;

        while (received == 0);

        //TODO check data received
#endif
    }
}

void PE_Button_onPress(PE_Button_Key_t *key) {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

void PE_Button_onRelease(PE_Button_Key_t *key) {
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}

void PE_nRF24_setCE0(PE_nRF24_t *handle) {
    //any pin
    (void) handle;
}

void PE_nRF24_setCE1(PE_nRF24_t *handle) {
    //any pin
    (void) handle;
}

void PE_nRF24_setSS0(PE_nRF24_t *handle) {
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

void PE_nRF24_setSS1(PE_nRF24_t *handle) {
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

#ifdef PE_nRF_SLAVE
void PE_nRF24_onRXComplete(PE_nRF24_t *handle) {
    (void) handle;
    received = 1;
}

void EXTIRQ_Handler(void) {
    if (PE_nRF24_handleIRQ(&nRF24) != PE_nRF24_RESULT_OK) {
        Error_Handler(__FILE__, __LINE__);
    }
}
#endif

void MX_GPIO_Init() {
    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO clock enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* GPIO Configuration */
    GPIO_InitStruct.Pin   = GPIO_PIN_13;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;

    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = GPIO_PIN_15;
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;

    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
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

    while (1) {
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
