#include "animation_morse.hpp"

namespace animation_morse {

unsigned long millis_last_tick = 0;


void init() {

}

// data is encoded as multiples of [time increment], first element (index 0) always indicates LEDS on for x [time increment]
const uint8_t encoded_morse[] = {
    1,1,3,1,3,1,1,3,1,1,3,3,1,1,1,1,
    1,3,3,3,1,3,3,1,1,1,1,1,1,3,1,1,
    1,3,3,1,1,7,3,1,1,1,3,1,1,3,3,1,
    3,1,3,3,3,1,3,7,1,1,1,1,1,1,1,3,
    1,1,3,1,1,1,1,3,1,1,3,1,3,1,1,3,
    1,1,3,1,3,1,3,3,1,1,3,1,3,1,3,3,
    1,1,1,1,1,3,3,1,3,1,1,1,3,3,1,1,
    3,1,3,1,1,7,1,1,3,1,1,1,1,3,3,1,
    3,1,3,3,1,1,3,1,3,3,1,3,1,1,3,1,
    1,7,1,1,1,1,1,3,3,1,3,1,1,1,3,
    20 // turn off leds for 20 [time increment after playing message]
};
const uint8_t encoded_morse_len = sizeof(encoded_morse) / sizeof(encoded_morse[0]);
const uint8_t time_increment = 200;
uint8_t morse_index = 0;

void animation() {
    if(morse_index % 2 == 0) {
        TURN_ON_ALL_LEDS();
    } else {
        TURN_OFF_ALL_LEDS();
    }

    if(millis() < millis_last_tick + (encoded_morse[morse_index] * time_increment)) return;

    millis_last_tick = millis();

    morse_index = (morse_index + 1) % encoded_morse_len;
}

}