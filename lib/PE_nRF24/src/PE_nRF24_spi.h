#ifndef PE_NRF24_SPI_H
#define PE_NRF24_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PE_nRF24_def.h"

/**
 * @param handle
 * @param addr
 * @param data
 * @param size
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_readMem(PE_nRF24_t *handle, uint8_t addr, uint8_t *data, uint8_t size);

/**
 * @param handle
 * @param addr
 * @param data
 * @param size
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_sendMem(PE_nRF24_t *handle, uint8_t addr, uint8_t *data, uint8_t size);

/**
 * @param handle
 * @param byte
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_readByte(PE_nRF24_t *handle, uint8_t *byte);

/**
 * @param handle
 * @param byte
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_sendByte(PE_nRF24_t *handle, uint8_t byte);

/**
 * @param handle
 * @param data
 * @param size
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_readData(PE_nRF24_t *handle, uint8_t *data, uint8_t size);

/**
 * @param handle
 * @param data
 * @param size
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_sendData(PE_nRF24_t *handle, uint8_t *data, uint8_t size);

/**
 * @param state
 */
void PE_nRF24_setCE0(PE_nRF24_t *handle);

/**
 * @param state
 */
void PE_nRF24_setCE1(PE_nRF24_t *handle);

/**
 * @param state
 */
void PE_nRF24_setSS0(PE_nRF24_t *handle);

/**
 * @param state
 */
void PE_nRF24_setSS1(PE_nRF24_t *handle);

#ifdef __cplusplus
}
#endif

#endif //PE_NRF24_SPI_H
