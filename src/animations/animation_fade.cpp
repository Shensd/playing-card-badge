#include "animation_fade.hpp"

namespace animation_fade {

unsigned long millis_last_update = 0;
uint8_t leds_brightness[] = {0, 128, 64, 255};
int8_t leds_delta[] = {1, 1, -1, -1};

void init() {

}

void animation() {
    if (millis() < millis_last_update + 1) return;
    millis_last_update = millis();

    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        if (leds_brightness[i] == 255) {
            leds_delta[i] = -1;
            leds_brightness[i] = 254;
        } else if (leds_brightness[i] == 0) {
            leds_delta[i] = 1;
            leds_brightness[i] = 1;
        } else {
            leds_brightness[i] += leds_delta[i];
        }

        analogWrite(LEDS[i], leds_brightness[i]);
    }
}

}