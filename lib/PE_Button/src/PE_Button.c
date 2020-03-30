#include "PE_Button.h"

void PE_Button_dispatchKey(PE_Button_Key_t *key, uint8_t state, uint32_t millis) {
    // Save previous state
    key->prevState = key->currState;

    // Read current state
    key->currState = state;

    // Reset execution flags if state changed
    if (key->currState != key->prevState) {
        key->millis      = millis;
        key->trigPress   = 0;
        key->trigRelease = 0;
        key->trigHold    = 0;
        return;
    }

    // Process pressed/released handler, must be first because must called always
    if (millis - key->millis > PE_BUTTON_TIMEOUT_DEBRIEF) {
        if (key->currState) {
            if (key->trigPress == 0) {
                key->trigPress = 1;
                PE_Button_onPress(key);
            }
        } else {
            if (key->trigRelease == 0) {
                key->trigRelease = 1;
                PE_Button_onRelease(key);
            }
        }
    }

#if defined(PE_BUTTON_USE_HOLD_SINGULAR)
    // Process singular handler
    if (key->trigPress != 0 && key->trigHold == 0 && millis - key->millis > PE_BUTTON_TIMEOUT_HOLD_SINGULAR) {
        key->trigHold = 1;
        PE_Button_onHoldSingular(key);
    }
#endif

#if defined(PE_BUTTON_USE_HOLD_REPEATED)
    // Process repeated handler
    if (key->trigPress != 0  && millis - key->millis > PE_BUTTON_TIMEOUT_HOLD_REPEATED) {
        key->millis = millis - (PE_BUTTON_TIMEOUT_HOLD_REPEATED - PE_BUTTON_TIMEOUT_REPEAT);
        PE_Button_onHoldRepeated(key);
    }
#endif
}

__attribute__((weak))
void PE_Button_onPress(PE_Button_Key_t *key) {
    (void) key;
}

__attribute__((weak))
void PE_Button_onRelease(PE_Button_Key_t *key) {
    (void) key;
}

__attribute__((weak))
void PE_Button_onHoldSingular(PE_Button_Key_t *key) {
    (void) key;
}

__attribute__((weak))
void PE_Button_onHoldRepeated(PE_Button_Key_t *key) {
    (void) key;
}
