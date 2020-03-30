#ifndef PE_BUTTON_H
#define PE_BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifndef PE_BUTTON_USE_HOLD_SINGULAR
#define PE_BUTTON_USE_HOLD_SINGULAR
#endif

#ifndef PE_BUTTON_USE_HOLD_REPEATED
#define PE_BUTTON_USE_HOLD_REPEATED
#endif

#ifndef PE_BUTTON_TIMEOUT_DEBRIEF
#define PE_BUTTON_TIMEOUT_DEBRIEF 20U
#endif

#ifndef PE_BUTTON_TIMEOUT_HOLD_SINGULAR
#define PE_BUTTON_TIMEOUT_HOLD_SINGULAR 800U
#endif

#ifndef PE_BUTTON_TIMEOUT_HOLD_REPEATED
#define PE_BUTTON_TIMEOUT_HOLD_REPEATED 1200U
#endif

#ifndef PE_BUTTON_TIMEOUT_REPEAT
#define PE_BUTTON_TIMEOUT_REPEAT 200U
#endif

typedef struct PE_Button_Key_s {
    uint8_t prevState: 1;
    uint8_t currState: 1;
    uint8_t trigPress: 1;
    uint8_t trigRelease: 1;
    uint8_t trigHold: 1;
    uint32_t millis;
} PE_Button_Key_t;

void PE_Button_dispatchKey(PE_Button_Key_t *key, uint8_t state, uint32_t millis);

void PE_Button_onPress(PE_Button_Key_t *key);

void PE_Button_onRelease(PE_Button_Key_t *key);

void PE_Button_onHoldSingular(PE_Button_Key_t *key);

void PE_Button_onHoldRepeated(PE_Button_Key_t *key);

#ifdef __cplusplus
}
#endif

#endif //STM_32_HEXAPOD_API_PE_BUTTON_H
