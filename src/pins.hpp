#ifndef PINS_HPP
#define PINS_HPP

#define LED_TL 3
#define LED_TR 4
#define LED_BL 2
#define LED_BR 1
#define NUM_LEDS 4
#define BUTTON 0

const uint8_t LEDS[] = {LED_TL, LED_TR, LED_BR, LED_BL};

#define MASK_LED_TL   0b00000100
#define MASK_LED_TR   0b00001000
#define MASK_LED_BL   0b00000010
#define MASK_LED_BR   0b10000000
#define MASK_ALL_LEDS 0b10001110
#define MASK_BUTTON   0b01000000

// #define MACRO_GET_BUTTON_STATE ((PORTA.IN & 0b01000000) >> 6)
static const uint8_t GET_BUTTON_STATE() {
    return ((PORTA.IN & 0b01000000) >> 6);
}
// #define MACRO_TURN_ON_ALL_LEDS (PORTA.OUT |= MASK_ALL_LEDS)
// #define MACRO_TURN_OFF_ALL_LEDS (PORTA.OUT &= ~MASK_ALL_LEDS)
static const void _SET_ALL_LEDS(uint8_t state) {
    for(uint8_t i = 0; i < NUM_LEDS; i++) digitalWrite(LEDS[i], state);
}
static const void TURN_ON_ALL_LEDS() {
    _SET_ALL_LEDS(HIGH);
}
static const void TURN_OFF_ALL_LEDS() {
    _SET_ALL_LEDS(LOW);
}

#endif // !PINS_HPP