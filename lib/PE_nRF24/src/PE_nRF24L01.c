#include "PE_nRF24L01.h"

static const uint8_t PE_nRF24_REG_RX_PW[6] = {
        PE_nRF24_REG_RX_PW_P0,
        PE_nRF24_REG_RX_PW_P1,
        PE_nRF24_REG_RX_PW_P2,
        PE_nRF24_REG_RX_PW_P3,
        PE_nRF24_REG_RX_PW_P4,
        PE_nRF24_REG_RX_PW_P5,
};

static const uint8_t PE_nRF24_REG_RX_ADDR[7] = {
        PE_nRF24_REG_RX_ADDR_P0,
        PE_nRF24_REG_RX_ADDR_P1,
        PE_nRF24_REG_RX_ADDR_P2,
        PE_nRF24_REG_RX_ADDR_P3,
        PE_nRF24_REG_RX_ADDR_P4,
        PE_nRF24_REG_RX_ADDR_P5,
};

/* SPI ****************************************************************************************************************/

static PE_nRF24_RESULT_t PE_nRF24_sendByte(PE_nRF24_t *handle, uint8_t byte) {
    return PE_nRF24_sendData(handle, &byte, 1);
}

static PE_nRF24_RESULT_t PE_nRF24_readMem(PE_nRF24_t *handle, uint8_t addr, uint8_t *data, uint8_t size) {
    if (PE_nRF24_sendByte(handle, addr) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    if (PE_nRF24_readData(handle, data, size) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

static PE_nRF24_RESULT_t PE_nRF24_sendMem(PE_nRF24_t *handle, uint8_t addr, uint8_t *data, uint8_t size) {
    if (PE_nRF24_sendByte(handle, addr) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    if (PE_nRF24_sendData(handle, data, size) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
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

/* DATA ***************************************************************************************************************/

PE_nRF24_RESULT_t PE_nRF24_getRegister(PE_nRF24_t *handle, uint8_t addr, uint8_t *byte) {
    PE_nRF24_RESULT_t result;

    PE_nRF24_setSS0(handle);

    result = PE_nRF24_readMem(handle, PE_nRF24_CMD_R_REGISTER | addr, byte, 1);

    PE_nRF24_setSS1(handle);

    return result;
}

PE_nRF24_RESULT_t PE_nRF24_setRegister(PE_nRF24_t *handle, uint8_t addr, uint8_t *byte) {
    PE_nRF24_RESULT_t result;

    PE_nRF24_setSS0(handle);

    result = PE_nRF24_sendMem(handle, PE_nRF24_CMD_W_REGISTER | addr, byte, 1);

    PE_nRF24_setSS1(handle);

    return result;
}

PE_nRF24_RESULT_t PE_nRF24_getPayload(PE_nRF24_t *handle, uint8_t *data, uint8_t size) {
    PE_nRF24_RESULT_t result;

    PE_nRF24_setSS0(handle);

    result = PE_nRF24_readMem(handle, PE_nRF24_CMD_R_RX_PAYLOAD, data, size);

    PE_nRF24_setSS1(handle);

    return result;
}

PE_nRF24_RESULT_t PE_nRF24_setPayload(PE_nRF24_t *handle, uint8_t *data, uint8_t size) {
    PE_nRF24_RESULT_t result;

    PE_nRF24_setSS0(handle);

    result = PE_nRF24_sendMem(handle, PE_nRF24_CMD_W_TX_PAYLOAD, data, size);

    PE_nRF24_setSS1(handle);

    return result;
}

/* API ****************************************************************************************************************/

PE_nRF24_RESULT_t PE_nRF24_setDirection(PE_nRF24_t *handle, PE_nRF24_DIRECTION_t direction) {
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~PE_nRF24_CONFIG_PRIM_RX;
    reg |= (direction << PE_nRF24_CONFIG_PRIM_RX_Pos);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_setAutoACK(PE_nRF24_t *handle, PE_nRF24_AUTO_ACK_t ack, PE_nRF24_PIPE_t pipe) {
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_EN_AA, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~(1U << pipe);
    reg |= (ack << pipe);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_EN_AA, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_setDataRate(PE_nRF24_t *handle, PE_nRF24_DATA_RATE_t rate) {
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_RF_SETUP, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~(PE_nRF24_RF_SETUP_RF_DR_HIGH|PE_nRF24_RF_SETUP_RF_DR_LOW);
    reg |= rate;

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_RF_SETUP, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_setCRCScheme(PE_nRF24_t *handle, PE_nRF24_CRC_SCHEME_t scheme) {
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~(PE_nRF24_CONFIG_EN_CRC|PE_nRF24_CONFIG_CRCO);
    reg |= (scheme << PE_nRF24_CONFIG_CRCO_Pos);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_setTXPower(PE_nRF24_t *handle, PE_nRF24_TX_POWER_t power) {
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_RF_SETUP, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~PE_nRF24_RF_SETUP_RF_PWR;
    reg |= (power << PE_nRF24_RF_SETUP_RF_PWR_Pos);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_RF_SETUP, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_setRetransmit(PE_nRF24_t *handle, PE_nRF24_RETRY_COUNT_t count, PE_nRF24_RETRY_DELAY_t delay) {
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_SETUP_RETR, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~(PE_nRF24_SETUP_RETR_ARD|PE_nRF24_SETUP_RETR_ARC);
    reg |= (count << PE_nRF24_SETUP_RETR_ARC_Pos);
    reg |= (delay << PE_nRF24_SETUP_RETR_ARD_Pos);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_SETUP_RETR, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_setPowerMode(PE_nRF24_t *handle, PE_nRF24_POWER_t value) {
    uint8_t reg;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~PE_nRF24_CONFIG_PWR_UP;
    reg |= (value << PE_nRF24_CONFIG_PWR_UP_Pos);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_setRFChannel(PE_nRF24_t *handle, uint8_t value) {
    uint8_t reg = (value & PE_nRF24_RF_CH);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_RF_CH, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_setAddressWidth(PE_nRF24_t *handle, PE_nRF24_ADDR_WIDTH_t width) {
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_SETUP_AW, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~PE_nRF24_SETUP_AW;
    reg |= width;

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_SETUP_AW, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_getAddressWidth(PE_nRF24_t *handle, PE_nRF24_ADDR_WIDTH_t *width) {
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_SETUP_AW, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    *width = (PE_nRF24_ADDR_WIDTH_t) (reg & PE_nRF24_SETUP_AW);

    if (*width == PE_nRF24_ADDR_WIDTH_ILLEGAL) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_setTXAddress(PE_nRF24_t *handle, uint8_t *address) {
    PE_nRF24_RESULT_t result;
    uint8_t width;

    PE_nRF24_getAddressWidth(handle, (PE_nRF24_ADDR_WIDTH_t *) &width);

    PE_nRF24_setSS0(handle);

    result = PE_nRF24_sendMem(handle, PE_nRF24_CMD_W_REGISTER + PE_nRF24_REG_TX_ADDR, address, width + 2);

    PE_nRF24_setSS1(handle);

    return result;
}

PE_nRF24_RESULT_t PE_nRF24_setRXAddress(PE_nRF24_t *handle, uint8_t *address, PE_nRF24_PIPE_t pipe) {
    PE_nRF24_RESULT_t result;
    uint8_t width;

    PE_nRF24_getAddressWidth(handle, (PE_nRF24_ADDR_WIDTH_t *) &width);

    if (pipe > PE_nRF24_PIPE_RX1) {
        width = 1;
    } else {
        width += 2;
    }

    PE_nRF24_setSS0(handle);

    result = PE_nRF24_sendMem(handle, PE_nRF24_CMD_W_REGISTER + PE_nRF24_REG_RX_ADDR[pipe], address, width);

    PE_nRF24_setSS1(handle);

    return result;
}

PE_nRF24_RESULT_t PE_nRF24_flushTX(PE_nRF24_t *handle) {
    PE_nRF24_RESULT_t result;

    PE_nRF24_setSS0(handle);

    result = PE_nRF24_sendByte(handle, PE_nRF24_CMD_FLUSH_TX);

    PE_nRF24_setSS1(handle);

    return result;
}

PE_nRF24_RESULT_t PE_nRF24_flushRX(PE_nRF24_t *handle) {
    PE_nRF24_RESULT_t result;

    PE_nRF24_setSS0(handle);

    result = PE_nRF24_sendByte(handle, PE_nRF24_CMD_FLUSH_RX);

    PE_nRF24_setSS1(handle);

    return result;
}

PE_nRF24_RESULT_t PE_nRF24_attachRXPipe(PE_nRF24_t *handle, PE_nRF24_PIPE_t pipe) {
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_EN_RXADDR, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg |= (1U << pipe);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_EN_RXADDR, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_detachRXPipe(PE_nRF24_t *handle, PE_nRF24_PIPE_t pipe) {
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_EN_RXADDR, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg &= ~(1U << pipe);

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_EN_RXADDR, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_attachIRQ(PE_nRF24_t *handle, PE_nRF24_IRQ_t mask) {
    uint8_t reg;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    // Mask must be only one of PE_nRF24_IRQ_*
    reg &= ~mask;

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_detachIRQ(PE_nRF24_t *handle, PE_nRF24_IRQ_t mask) {
    uint8_t reg;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    // Mask must be only one of PE_nRF24_IRQ_*
    reg |= mask;

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_clearIRQ(PE_nRF24_t *handle) {
    uint8_t reg;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_STATUS, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    reg |= PE_nRF24_IRQ_MASK_ALL;

    if (PE_nRF24_setRegister(handle, PE_nRF24_REG_STATUS, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_checkIRQ(PE_nRF24_t *handle, PE_nRF24_IRQ_t mask) {
    uint8_t reg;

    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_STATUS, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    if ((reg & mask) != 0U) {
        return PE_nRF24_RESULT_OK;
    }

    return PE_nRF24_RESULT_ERROR;
}


PE_nRF24_RESULT_t PE_nRF24_getLostCount(PE_nRF24_t *handle, PE_nRF24_RETRY_COUNT_t *value) {
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_OBSERVE_TX, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    *value = (PE_nRF24_RETRY_COUNT_t) ((reg & PE_nRF24_OBSERVE_TX_PLOS_CNT) >> PE_nRF24_OBSERVE_TX_PLOS_CNT_Pos);

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_getRetryCount(PE_nRF24_t *handle, PE_nRF24_RETRY_COUNT_t *value) {
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_OBSERVE_TX, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    *value = (PE_nRF24_RETRY_COUNT_t) ((reg & PE_nRF24_OBSERVE_TX_ARC_CNT) >> PE_nRF24_OBSERVE_TX_ARC_CNT_Pos);

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_getCarrierDetect(PE_nRF24_t *handle, PE_nRF24_BIT_t *value) {
    uint8_t reg;
    if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CD, &reg) != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    *value = (PE_nRF24_BIT_t) (reg & PE_nRF24_CD_BIT);

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_sendPacketFg(PE_nRF24_t *handle, uint8_t *addr, uint8_t *data, uint8_t size, uint16_t timeout) {
    if (handle->status != PE_nRF24_STATUS_READY) {
        return PE_nRF24_RESULT_ERROR;
    }

    PE_nRF24_RESULT_t result = PE_nRF24_RESULT_TIMEOUT;
    uint8_t status;

    handle->status = PE_nRF24_STATUS_BUSY_TX;

    PE_nRF24_setCE0(handle);

    PE_nRF24_setTXAddress(handle, addr);
    PE_nRF24_setDirection(handle, PE_nRF24_DIRECTION_TX);
    PE_nRF24_setPayload(handle, data, size);
    PE_nRF24_detachIRQ(handle, PE_nRF24_IRQ_MASK_TX_DS|PE_nRF24_IRQ_MASK_MAX_RT);

    PE_nRF24_setCE1(handle);

    uint32_t start = PE_nRF24_getMillis();
    do {
        PE_nRF24_getRegister(handle, PE_nRF24_REG_STATUS, &status);

        if (status & (PE_nRF24_IRQ_MASK_TX_DS|PE_nRF24_IRQ_MASK_MAX_RT)) {
            result = PE_nRF24_RESULT_OK;
            break;
        }
    } while (timeout > 0 && ((PE_nRF24_getMillis() - start) < timeout));

    //TODO check timeout before
    if (status & PE_nRF24_IRQ_MASK_TX_DS) {
        //TODO process tx
        PE_nRF24_onTXComplete(handle);
    } else {
        //TODO process max rt
        PE_nRF24_onMaxRetransmit(handle);
        result = PE_nRF24_RESULT_MAX_RT;
    }

    status |= PE_nRF24_IRQ_MASK_TX_DS|PE_nRF24_IRQ_MASK_MAX_RT;

    PE_nRF24_setCE0(handle);
    PE_nRF24_flushTX(handle);
    PE_nRF24_setRegister(handle, PE_nRF24_REG_STATUS, &status);
    PE_nRF24_setCE1(handle);

    handle->status = PE_nRF24_STATUS_READY;

    return result;
}

PE_nRF24_RESULT_t PE_nRF24_sendPacketBg(PE_nRF24_t *handle, uint8_t *addr, uint8_t *data, uint8_t size) {
    if (handle->status != PE_nRF24_STATUS_READY) {
        return PE_nRF24_RESULT_ERROR;
    }

    handle->status = PE_nRF24_STATUS_BUSY_TX;

    PE_nRF24_setCE0(handle);

    PE_nRF24_setTXAddress(handle, addr);
    PE_nRF24_setDirection(handle, PE_nRF24_DIRECTION_TX);
    PE_nRF24_setPayload(handle, data, size);
    //TODO enable irq
    PE_nRF24_setCE1(handle);

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_sendPacket(PE_nRF24_t *handle, uint8_t *addr, uint8_t *data, uint8_t size, uint16_t timeout) {
    if (handle->status != PE_nRF24_STATUS_READY) {
        return PE_nRF24_RESULT_ERROR;
    }

    handle->status = PE_nRF24_STATUS_BUSY_TX;

    PE_nRF24_setCE0(handle);

    PE_nRF24_setTXAddress(handle, addr);
    PE_nRF24_setDirection(handle, PE_nRF24_DIRECTION_TX);
    PE_nRF24_setPayload(handle, data, size);

    PE_nRF24_setCE1(handle);

    uint32_t start = PE_nRF24_getMillis() ;

    do {
        if (handle->status == PE_nRF24_STATUS_READY) {
            break;
        }
    } while (timeout > 0 && ((PE_nRF24_getMillis() - start) < timeout));

    if (handle->status != PE_nRF24_STATUS_READY) {
        handle->status = PE_nRF24_STATUS_READY;
        return PE_nRF24_RESULT_TIMEOUT;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_configureRF(PE_nRF24_t *handle) {
    uint8_t reg;
    PE_nRF24_RESULT_t result = PE_nRF24_RESULT_OK;

    PE_nRF24_setCE0(handle);

    // Set device power up
    if (PE_nRF24_setPowerMode(handle, PE_nRF24_POWER_ON) != PE_nRF24_RESULT_OK) {
        PE_nRF24_setCE1(handle);
        return PE_nRF24_RESULT_ERROR;
    }

    // Check if prev operation success
    do {
        if (PE_nRF24_getRegister(handle, PE_nRF24_REG_CONFIG, &reg) != PE_nRF24_RESULT_OK) {
            PE_nRF24_setCE1(handle);
            return PE_nRF24_RESULT_ERROR;
        }
    } while ((reg & PE_nRF24_CONFIG_PWR_UP) == 0x00);

    result |= PE_nRF24_setAddressWidth(handle, handle->config.addressWidth);
    result |= PE_nRF24_setDataRate(handle, handle->config.dataRate);
    result |= PE_nRF24_setRFChannel(handle, handle->config.rfChannel);
    result |= PE_nRF24_setCRCScheme(handle, handle->config.crcScheme);
    result |= PE_nRF24_setTXPower(handle, handle->config.txPower);
    result |= PE_nRF24_setRetransmit(handle, handle->config.retryCount, handle->config.retryDelay);
    result |= PE_nRF24_setDirection(handle, PE_nRF24_DIRECTION_RX);

    result |= PE_nRF24_setAutoACK(handle, PE_nRF24_AUTO_ACK_OFF, PE_nRF24_PIPE_RX0);
    result |= PE_nRF24_setAutoACK(handle, PE_nRF24_AUTO_ACK_OFF, PE_nRF24_PIPE_RX1);
    result |= PE_nRF24_setAutoACK(handle, PE_nRF24_AUTO_ACK_OFF, PE_nRF24_PIPE_RX2);
    result |= PE_nRF24_setAutoACK(handle, PE_nRF24_AUTO_ACK_OFF, PE_nRF24_PIPE_RX3);
    result |= PE_nRF24_setAutoACK(handle, PE_nRF24_AUTO_ACK_OFF, PE_nRF24_PIPE_RX4);
    result |= PE_nRF24_setAutoACK(handle, PE_nRF24_AUTO_ACK_OFF, PE_nRF24_PIPE_RX5);

    result |= PE_nRF24_attachIRQ(handle, PE_nRF24_IRQ_TX_DS);
    result |= PE_nRF24_attachIRQ(handle, PE_nRF24_IRQ_RX_DR);
    result |= PE_nRF24_attachIRQ(handle, PE_nRF24_IRQ_MAX_RT);

    result |= PE_nRF24_flushTX(handle);
    result |= PE_nRF24_flushRX(handle);

    result |= PE_nRF24_clearIRQ(handle);

    PE_nRF24_setCE1(handle);

    if (result != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

PE_nRF24_RESULT_t PE_nRF24_configureRX(PE_nRF24_t *handle, PE_nRF24_configRX_t *config, PE_nRF24_PIPE_t pipe) {
    PE_nRF24_RESULT_t result = PE_nRF24_RESULT_OK;

    PE_nRF24_setCE0(handle);

    result |= PE_nRF24_setRXAddress(handle, config->address, pipe);
    result |= PE_nRF24_setAutoACK(handle, config->autoACK, pipe);
    result |= PE_nRF24_setRegister(handle, PE_nRF24_REG_RX_PW[pipe], &(config->payloadSize));
    result |= PE_nRF24_attachRXPipe(handle, pipe);

    PE_nRF24_setCE1(handle);

    if (result != PE_nRF24_RESULT_OK) {
        return PE_nRF24_RESULT_ERROR;
    }

    return PE_nRF24_RESULT_OK;
}

__attribute__((weak))
uint32_t PE_nRF24_getMillis(void) {
    return 0;
}

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
        //TODO disable IRQ?
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
        //TODO disable IRQ?
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
