#include "PE_nRF24L01.h"

/* IRQ ****************************************************************************************************************/

PE_nRF24_RESULT_t PE_nRF24_handleIRQ(PE_nRF24_t *handle) {
    uint8_t status;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_STATUS, &status) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    if ((status & PE_nRF24_IRQ_MASK_RX_DR) != 0U) {
        uint8_t statusFIFO;

        PE_nRF24_setCE0(handle);

        do {
            PE_nRF24_getPayload(handle, handle->bufferData, handle->bufferSize);
            PE_nRF24_getRegister(handle, PE_nRF24_REG_FIFO_STATUS, &statusFIFO);
        } while ((statusFIFO & PE_nRF24_FIFO_STATUS_RX_EMPTY) == 0x00);

        status |= PE_nRF24_IRQ_MASK_RX_DR;

        PE_nRF24_setRegister(handle, PE_nRF24_REG_STATUS, &status);

        PE_nRF24_setCE1(handle);

        PE_nRF24_onRXComplete(handle);
    }

    if ((status & PE_nRF24_IRQ_MASK_TX_DS) != 0U) {
        PE_nRF24_setCE0(handle);

        status |= PE_nRF24_IRQ_MASK_TX_DS;

        PE_nRF24_setDirection(handle, PE_nRF24_DIRECTION_RX);
        PE_nRF24_setRegister(handle, PE_nRF24_REG_STATUS, &status);

        PE_nRF24_setCE1(handle);

        handle->status = PE_nRF24_STATUS_READY;

        PE_nRF24_onTXComplete(handle);
    }

    if ((status & PE_nRF24_IRQ_MASK_MAX_RT) != 0U) {
        PE_nRF24_flushTX(handle);

        PE_nRF24_setPowerMode(handle, PE_nRF24_POWER_OFF);
        PE_nRF24_setPowerMode(handle, PE_nRF24_POWER_ON);

        PE_nRF24_setCE0(handle);

        status |= PE_nRF24_IRQ_MASK_MAX_RT;

        PE_nRF24_setDirection(handle, PE_nRF24_DIRECTION_RX);
        PE_nRF24_setRegister(handle, PE_nRF24_REG_STATUS, &status);

        PE_nRF24_setCE1(handle);

        handle->status = PE_nRF24_STATUS_READY;

        PE_nRF24_onMaxRetransmit(handle);
    }

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
