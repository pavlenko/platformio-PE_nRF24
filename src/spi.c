#include "spi.h"

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
void MX_SPI1_Init(SPI_HandleTypeDef* spi) {
    //SPI1 clock from APB2
    spi->Instance               = SPI1;
    spi->Init.Mode              = SPI_MODE_MASTER;
    spi->Init.Direction         = SPI_DIRECTION_2LINES;//TODO check other values possibility
    spi->Init.DataSize          = SPI_DATASIZE_8BIT;
    spi->Init.CLKPolarity       = SPI_POLARITY_LOW;
    spi->Init.CLKPhase          = SPI_PHASE_1EDGE;
    spi->Init.NSS               = SPI_NSS_SOFT;
    spi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    spi->Init.FirstBit          = SPI_FIRSTBIT_MSB;
    spi->Init.TIMode            = SPI_TIMODE_DISABLE;
    spi->Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    spi->Init.CRCPolynomial     = 10;

    if (HAL_SPI_Init(spi) != HAL_OK) {
        Error_Handler(__FILE__, __LINE__);
    }
}

void MX_SPI2_Init(SPI_HandleTypeDef* spi) {
    //SPI2 clock from APB1
    spi->Instance               = SPI2;
    spi->Init.Mode              = SPI_MODE_MASTER;
    spi->Init.Direction         = SPI_DIRECTION_2LINES;//TODO check other values possibility
    spi->Init.DataSize          = SPI_DATASIZE_8BIT;
    spi->Init.CLKPolarity       = SPI_POLARITY_LOW;
    spi->Init.CLKPhase          = SPI_PHASE_1EDGE;
    spi->Init.NSS               = SPI_NSS_SOFT;
    spi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    spi->Init.FirstBit          = SPI_FIRSTBIT_MSB;
    spi->Init.TIMode            = SPI_TIMODE_DISABLE;
    spi->Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    spi->Init.CRCPolynomial     = 10;

    if (HAL_SPI_Init(spi) != HAL_OK) {
        Error_Handler(__FILE__, __LINE__);
    }

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spi) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (spi->Instance == SPI1) {
        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        //PA4 ------> SPI1_NSS
        //PA5 ------> SPI1_SCK
        //PA6 ------> SPI1_MISO
        //PA7 ------> SPI1_MOSI

        GPIO_InitStruct.Pin   = GPIO_PIN_4;
        GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

        GPIO_InitStruct.Pin   = GPIO_PIN_5|GPIO_PIN_7;
        GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin  = GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }

    if (spi->Instance == SPI2) {
        __HAL_RCC_SPI2_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        //PB12 ------> SPI2_NSS
        //PB13 ------> SPI2_SCK
        //PB14 ------> SPI2_MISO
        //PB15 ------> SPI2_MOSI

        GPIO_InitStruct.Pin   = GPIO_PIN_12;
        GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

        GPIO_InitStruct.Pin   = GPIO_PIN_13|GPIO_PIN_15;
        GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin  = GPIO_PIN_14;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }

}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spi) {
    if (spi->Instance==SPI1) {
        __HAL_RCC_SPI1_CLK_DISABLE();

        //PA4 ------> SPI1_NSS
        //PA5 ------> SPI1_SCK
        //PA6 ------> SPI1_MISO
        //PA7 ------> SPI1_MOSI
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    }

    if (spi->Instance == SPI2) {
        __HAL_RCC_SPI2_CLK_DISABLE();

        //PB12 ------> SPI2_NSS
        //PB13 ------> SPI2_SCK
        //PB14 ------> SPI2_MISO
        //PB15 ------> SPI2_MOSI
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
    }

}