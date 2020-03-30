#ifndef PE_NRF24_IRQ_H
#define PE_NRF24_IRQ_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PE_nRF24_def.h"

/**
* @param handle
* @return
*/
PE_nRF24_RESULT_t PE_nRF24_handleIRQ(PE_nRF24_t *handle);

/**
 * @param handle
 */
void PE_nRF24_onTXComplete(PE_nRF24_t *handle);

/**
 * @param handle
 */
void PE_nRF24_onRXComplete(PE_nRF24_t *handle);

/**
 * @param handle
 */
void PE_nRF24_onMaxRetransmit(PE_nRF24_t *handle);

#ifdef __cplusplus
}
#endif

#endif //PE_NRF24_IRQ_H
