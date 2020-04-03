void init_RX_blocking() {
    /* Initialize NRF24L01+ on channel 15 and 32bytes of payload */
    /* By default 2Mbps data rate and 0dBm output power */
    /* NRF24L01 goes to RX mode by default */
    TM_NRF24L01_Init(15, 32);
    TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_2M, TM_NRF24L01_OutputPower_M18dBm);
    TM_NRF24L01_SetMyAddress(MyAddress);
    TM_NRF24L01_SetTxAddress(TxAddress);
}

void loop_RX_blocking() {
    if (TM_NRF24L01_DataReady()) {/* If data is ready on NRF24L01+ */
        TM_NRF24L01_GetData(dataIn);/* Get data from NRF24L01+ */
        TM_NRF24L01_Transmit(dataIn);/* Send it back, automatically goes to TX mode */

        do {
            /* Wait till sending */
            transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
        } while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);

        TM_NRF24L01_PowerUpRx();/* Go back to RX mode */
    }
}

void extI_RX_locking() {}

void init_RX_IRQ() {
    /* Initialize NRF24L01+ on channel 15 and 32bytes of payload */
    /* By default 2Mbps data rate and 0dBm output power */
    /* NRF24L01 goes to RX mode by default */
    TM_NRF24L01_Init(15, 32);
    TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_2M, TM_NRF24L01_OutputPower_M18dBm);
    TM_NRF24L01_SetMyAddress(MyAddress);
    TM_NRF24L01_SetTxAddress(TxAddress);
    TM_EXTI_Attach(IRQ_PORT, IRQ_PIN, TM_EXTI_Trigger_Falling);
}

void loop_RX_IRQ() {}
void extI_RX_IRQ() {
    TM_NRF24L01_Read_Interrupts(&NRF_IRQ);

    if (NRF_IRQ.F.DataReady) {/* If data is ready on NRF24L01+ */
        TM_NRF24L01_GetData(dataIn);/* Get data from NRF24L01+ */
        TM_NRF24L01_Transmit(dataIn);/* Send it back, NRF goes automatically to TX mode */

        do {
            /* Wait till sending */
            transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
        } while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);

        TM_NRF24L01_PowerUpRx();/* Go back to RX mode */
    }

    TM_NRF24L01_Clear_Interrupts();/* Clear interrupts */
}

void init_TX_blocking() {
    /* Initialize NRF24L01+ on channel 15 and 32bytes of payload */
    /* By default 2Mbps data rate and 0dBm output power */
    /* NRF24L01 goes to RX mode by default */
    TM_NRF24L01_Init(15, 32);
    TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_2M, TM_NRF24L01_OutputPower_M18dBm);
    TM_NRF24L01_SetMyAddress(MyAddress);
    TM_NRF24L01_SetTxAddress(TxAddress);
}

void loop_TX_blocking() {
    if (TM_DELAY_Time() > 2000) {/* Every 2 seconds */
        TM_NRF24L01_Transmit(dataOut);/* Transmit data, goes automatically to TX mode */

        do {
            /* Get transmission status */
            transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
        } while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);


        TM_NRF24L01_PowerUpRx();/* Go back to RX mode */

        /* Wait received data, wait max 100ms, if time is larger, then data were probably lost */
        while (!TM_NRF24L01_DataReady() && TM_DELAY_Time() < 100);

        /* Get data from NRF2L01+ */
        TM_NRF24L01_GetData(dataIn);

        if (transmissionStatus == TM_NRF24L01_Transmit_Status_Ok) {
            TM_USART_Puts(USART1, ": OK\n");
        } else if (transmissionStatus == TM_NRF24L01_Transmit_Status_Lost) {
            TM_USART_Puts(USART1, ": LOST\n");
        } else {
            TM_USART_Puts(USART1, ": SENDING\n");
        }
    }
}
void extI_TX_blocking() {}

void init_TX_IRQ() {
    /* Initialize NRF24L01+ on channel 15 and 32bytes of payload */
    /* By default 2Mbps data rate and 0dBm output power */
    /* NRF24L01 goes to RX mode by default */
    TM_NRF24L01_Init(15, 32);
    TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_2M, TM_NRF24L01_OutputPower_M18dBm);
    TM_NRF24L01_SetMyAddress(MyAddress);
    TM_NRF24L01_SetTxAddress(TxAddress);
    TM_EXTI_Attach(IRQ_PORT, IRQ_PIN, TM_EXTI_Trigger_Falling);
}

void loop_TX_IRQ() {
    if (TM_DELAY_Time() > 2000) {/* Every 2 seconds */
        sprintf((char *)dataOut, "abcdefghijklmnoszxABCDEFCBDA");/* Fill data with something */

        TM_NRF24L01_Transmit(dataOut);/* Transmit data, goes automatically to TX mode */

        transmissionStatus = TM_NRF24L01_Transmit_Status_Sending;
        printed = 0;
    }

    if (
            transmissionStatus != TM_NRF24L01_Transmit_Status_Sending && /*!< We are not sending anymore */
            !printed                                                     /*!< We didn't print status to user */
            ) {
        if (transmissionStatus == TM_NRF24L01_Transmit_Status_Ok) {
            TM_USART_Puts(USART1, "OK\n");
        }
        if (transmissionStatus == TM_NRF24L01_Transmit_Status_Lost) {
            TM_USART_Puts(USART1, "LOST\n");
        }

        printed = 1;
    }
}
void extI_TX_IRQ() {
    TM_NRF24L01_Read_Interrupts(&NRF_IRQ);

    if (NRF_IRQ.F.DataSent) {/* Check if transmitted OK */
        transmissionStatus = TM_NRF24L01_Transmit_Status_Ok;/* Save transmission status */

        TM_NRF24L01_PowerUpRx();/* Go back to RX mode */
    }

    if (NRF_IRQ.F.MaxRT) {/* Check if max retransmission reached and last transmission failed */
        transmissionStatus = TM_NRF24L01_Transmit_Status_Lost;/* Save transmission status */

        TM_NRF24L01_PowerUpRx();/* Go back to RX mode */
    }

    if (NRF_IRQ.F.DataReady) {/* If data is ready on NRF24L01+ */
        /* Get data from NRF24L01+ */
        TM_NRF24L01_GetData(dataIn);
    }
}