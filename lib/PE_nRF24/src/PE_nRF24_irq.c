#include "PE_nRF24_irq.h"

#include <stddef.h>
#include "PE_nRF24_api.h"
#include "PE_nRF24_spi.h"

static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_RX_DR(PE_nRF24_t *handle);
static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_TX_DS(PE_nRF24_t *handle);
static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_MAX_RT(PE_nRF24_t *handle);

PE_nRF24_RESULT_t PE_nRF24_handleIRQ(PE_nRF24_t *handle) {
    uint8_t status;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_STATUS, &status) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    // Process RX data ready (RX_DR bit)
    if ((status & (uint8_t) PE_nRF24_IRQ_RX_DR) != 0U) {
        PE_nRF24_handleIRQ_RX_DR(handle);
    }

    // Process TX sent (TX_DS bit)
    if ((status & (uint8_t) PE_nRF24_IRQ_TX_DS) != 0U) {
        PE_nRF24_handleIRQ_TX_DS(handle);
    }

    // Process reach retransmission count (MAX_RT bit)
    if ((status & (uint8_t) PE_nRF24_IRQ_MAX_RT) != 0U) {
        PE_nRF24_handleIRQ_MAX_RT(handle);
    }

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_RX_DR(PE_nRF24_t *handle)
{
    uint8_t statusFIFO;

    PE_nRF24_setCE0(handle);

    do {
        // Read payload to internal buffer, it must exists for proper work
        PE_nRF24_getPayload(handle, handle->bufferData, handle->bufferSize);

        // Clear pending IRQ
        PE_nRF24_clearIRQ(handle);

        PE_nRF24_getRegister(handle, PE_nRF24_REG_FIFO_STATUS, &statusFIFO);

        PE_nRF24_onRXComplete(handle);
    } while ((statusFIFO & PE_nRF24_FIFO_STATUS_RX_EMPTY) == 0x00);

    PE_nRF24_setCE1(handle);

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_TX_DS(PE_nRF24_t *handle)
{
    PE_nRF24_setCE0(handle);

    // Set direction to RX
    PE_nRF24_setDirection(handle, PE_nRF24_DIRECTION_RX);

    // Clear pending IRQ
    PE_nRF24_clearIRQ(handle);

    PE_nRF24_setCE1(handle);

    handle->status = PE_nRF24_STATUS_READY;

    PE_nRF24_onTXComplete(handle);

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_handleIRQ_MAX_RT(PE_nRF24_t *handle)
{
    PE_nRF24_flushTX(handle);

    // Toggle RF power up bit
    PE_nRF24_setPowerMode(handle, PE_nRF24_POWER_OFF);
    PE_nRF24_setPowerMode(handle, PE_nRF24_POWER_ON);

    PE_nRF24_setCE0(handle);

    // Set direction to RX
    PE_nRF24_setDirection(handle, PE_nRF24_DIRECTION_RX);

    // Clear pending IRQ
    PE_nRF24_clearIRQ(handle);

    PE_nRF24_setCE1(handle);

    handle->status = PE_nRF24_STATUS_READY;

    PE_nRF24_onMaxRetransmit(handle);

    return PE_nRF24_RESULT_OK;
}

__attribute__((weak))
void PE_nRF24_onTXComplete(PE_nRF24_t *handle) {
    (void) handle;
}

__attribute__((weak))
void PE_nRF24_onRXComplete(PE_nRF24_t *handle) {
    (void) handle;
}

__attribute__((weak))
void PE_nRF24_onMaxRetransmit(PE_nRF24_t *handle) {
    (void) handle;
}
