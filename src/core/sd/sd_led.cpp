#include "main.hpp"

static bool s_lastState = false;

void sd_led_update(bool sdAvailable) { 

    if (sdAvailable) {
        uint32_t phase = (esp_timer_get_time() / 1000) % 500;
        bool newState = phase < 50;

        if (newState == s_lastState)
            return;

        digitalWrite(SD_STATUS_LED, newState ? HIGH : LOW);
        s_lastState = newState;
        return;
    }

    if (!s_lastState)
        return;

    digitalWrite(SD_STATUS_LED, LOW);
    s_lastState = false;
}