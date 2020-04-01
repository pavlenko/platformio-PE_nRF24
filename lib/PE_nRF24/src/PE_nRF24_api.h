#ifndef PE_NRF24_API_H
#define PE_NRF24_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PE_nRF24_def.h"

/**
 * @param handle
 * @param addr
 * @param byte
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_getRegister(PE_nRF24_t *handle, uint8_t addr, uint8_t *byte);

/**
 * @param handle
 * @param addr
 * @param byte
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_setRegister(PE_nRF24_t *handle, uint8_t addr, uint8_t *byte);

/**
 * @param handle
 * @param data
 * @param size
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_getPayload(PE_nRF24_t *handle, uint8_t *data, uint8_t size);

/**
 * @param handle
 * @param data
 * @param size
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_setPayload(PE_nRF24_t *handle, uint8_t *data, uint8_t size);

/**
 * Read packet in blocking mode
 *
 * @param handle
 * @param data
 * @param size
 * @param timeout
 *
 * @return Operation status
 */
PE_nRF24_RESULT_t PE_nRF24_readPacket(PE_nRF24_t *handle, uint8_t *data, uint8_t size, uint16_t timeout);

/**
 * Send packet in blocking mode
 *
 * @param handle
 * @param data
 * @param size
 * @param timeout
 *
 * @return Operation status
 */
PE_nRF24_RESULT_t PE_nRF24_sendPacket(PE_nRF24_t *handle, uint8_t *addr, uint8_t *data, uint8_t size, uint16_t timeout);

/**
 * @param handle
 * @param width
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_setAddressWidth(PE_nRF24_t *handle, PE_nRF24_ADDR_WIDTH_t width);

/**
 * @param handle
 * @param width
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_getAddressWidth(PE_nRF24_t *handle, PE_nRF24_ADDR_WIDTH_t *width);

/**
 * @param handle
 * @param address
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_setTXAddress(PE_nRF24_t *handle, uint8_t *address);

/**
 * @param handle
 * @param address
 * @param pipe
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_setRXAddress(PE_nRF24_t *handle, uint8_t *address, PE_nRF24_PIPE_t pipe);

/**
 * @param handle
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_flushTX(PE_nRF24_t *handle);

/**
 * @param handle
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_flushRX(PE_nRF24_t *handle);

/**
 * @param handle
 * @param mask
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_attachIRQ(PE_nRF24_t *handle, PE_nRF24_IRQ_t mask);

/**
 * @param handle
 * @param mask
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_detachIRQ(PE_nRF24_t *handle, PE_nRF24_IRQ_t mask);

/**
 * @param handle
 * @param mask
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_clearIRQ(PE_nRF24_t *handle);

/**
 * @param handle
 * @param mask
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_checkIRQ(PE_nRF24_t *handle, PE_nRF24_IRQ_t mask);

/**
 * @param handle
 * @param direction
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_setDirection(PE_nRF24_t *handle, PE_nRF24_DIRECTION_t direction);

/**
 * @param handle
 * @param ack
 * @param pipe
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_setAutoACK(PE_nRF24_t *handle, PE_nRF24_AUTO_ACK_t ack, PE_nRF24_PIPE_t pipe);

/**
 * @param handle
 * @param rate
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_setDataRate(PE_nRF24_t *handle, PE_nRF24_DATA_RATE_t rate);

/**
 * @param handle
 * @param scheme
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_setCRCScheme(PE_nRF24_t *handle, PE_nRF24_CRC_SCHEME_t scheme);

/**
 * @param handle
 * @param power
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_setTXPower(PE_nRF24_t *handle, PE_nRF24_TX_POWER_t power);

/**
 * @param handle
 * @param count
 * @param delay
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_setRetransmit(PE_nRF24_t *handle, PE_nRF24_RETRY_COUNT_t count, PE_nRF24_RETRY_DELAY_t delay);

/**
 * @param handle
 * @param value
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_setPowerMode(PE_nRF24_t *handle, PE_nRF24_POWER_t value);

/**
 * @param handle
 * @param value
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_setRFChannel(PE_nRF24_t *handle, uint8_t value);

/**
 * @param handle
 * @param pipe
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_attachRXPipe(PE_nRF24_t *handle, PE_nRF24_PIPE_t pipe);

/**
 * @param handle
 * @param pipe
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_detachRXPipe(PE_nRF24_t *handle, PE_nRF24_PIPE_t pipe);

/**
 * @param handle
 * @param value
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_getLostCount(PE_nRF24_t *handle, PE_nRF24_RETRY_COUNT_t *value);

/**
 * @param handle
 * @param value
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_getRetryCount(PE_nRF24_t *handle, PE_nRF24_RETRY_COUNT_t *value);

/**
 * @param handle
 * @param value
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_getCarrierDetect(PE_nRF24_t *handle, PE_nRF24_BIT_t *value);

/**
 * @param handle
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_configureRF(PE_nRF24_t *handle);

/**
 * @param handle
 * @param config
 * @param pipe
 * @return
 */
PE_nRF24_RESULT_t PE_nRF24_configureRX(PE_nRF24_t *handle, PE_nRF24_configRX_t *config, PE_nRF24_PIPE_t pipe);

/**
 * @return
 */
uint32_t PE_nRF24_getMillis(void);

extern const uint8_t PE_nRF24_REG_RX_PW[6];
extern const uint8_t PE_nRF24_REG_RX_ADDR[7];

#ifdef __cplusplus
}
#endif

#endif //PE_NRF24_API_H
