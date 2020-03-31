#include "PE_nRF24_spi.h"

PE_nRF24_RESULT_t PE_nRF24_readMem(PE_nRF24_t *handle, uint8_t addr, uint8_t *data, uint8_t size) {
    if (PE_nRF24_sendByte(handle, addr) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    if (PE_nRF24_readData(handle, data, size) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_sendMem(PE_nRF24_t *handle, uint8_t addr, uint8_t *data, uint8_t size) {
    if (PE_nRF24_sendByte(handle, addr) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    if (PE_nRF24_sendData(handle, data, size) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_readByte(PE_nRF24_t *handle, uint8_t *byte) {
    return PE_nRF24_readData(handle, byte, 1);
}

PE_nRF24_RESULT_t PE_nRF24_sendByte(PE_nRF24_t *handle, uint8_t byte) {
    return PE_nRF24_sendData(handle, &byte, 1);
}

__attribute__((weak))
PE_nRF24_RESULT_t PE_nRF24_readData(PE_nRF24_t *handle, uint8_t *data, uint8_t size) {
    (void) handle;
    (void) data;
    (void) size;
    return PE_nRF24_RESULT_ERROR;
}

__attribute__((weak))
PE_nRF24_RESULT_t PE_nRF24_sendData(PE_nRF24_t *handle, uint8_t *data, uint8_t size) {
    (void) handle;
    (void) data;
    (void) size;
    return PE_nRF24_RESULT_ERROR;
}

__attribute__((weak))
void PE_nRF24_setCE0(PE_nRF24_t *handle) {
    (void) handle;
}

__attribute__((weak))
void PE_nRF24_setCE1(PE_nRF24_t *handle) {
    (void) handle;
}

__attribute__((weak))
void PE_nRF24_setSS0(PE_nRF24_t *handle) {
    (void) handle;
}

__attribute__((weak))
void PE_nRF24_setSS1(PE_nRF24_t *handle) {
    (void) handle;
}