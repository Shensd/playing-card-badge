#include "animation_blink.hpp"

namespace animation_blink {

unsigned long millis_last_tick = 0;
bool led_state = false;

void init() {
    millis_last_tick = millis();
}

void animation() {
    if(millis() < millis_last_tick + 300) return;

    millis_last_tick = millis();
    led_state = !led_state;

    if(led_state) {
        TURN_ON_ALL_LEDS();
    } else {
        TURN_OFF_ALL_LEDS();
    }
}

}