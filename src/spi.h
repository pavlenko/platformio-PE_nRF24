#ifndef SPI_H
#define SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void MX_SPI1_Init(SPI_HandleTypeDef* spi);
void MX_SPI2_Init(SPI_HandleTypeDef* spi);

#ifdef __cplusplus
}
#endif

#endif //SPI_H
