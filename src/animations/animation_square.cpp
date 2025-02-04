#include "animation_square.hpp"

namespace animation_square {

unsigned long millis_last_update = 0;
uint8_t lit_led = 0;

void init() {

}
void animation() {
    if(millis() < millis_last_update + 400) return;

    millis_last_update = millis();

    TURN_OFF_ALL_LEDS();
    digitalWrite(LEDS[lit_led], HIGH);

    lit_led = (lit_led + 1) % NUM_LEDS;
}

}