#include <Arduino.h>
#include "pins.hpp"
#include <avr/sleep.h>

#include "animations/animation_fade.hpp"
#include "animations/animation_always_on.hpp"
#include "animations/animation_square.hpp"
#include "animations/animation_blink.hpp"
#include "animations/animation_morse.hpp"

#define MILLIS_FAR_OFF 9999999

unsigned long millis_button_pressed = 0;
unsigned long millis_button_released = 0;
unsigned long millis_button_last_state_change = 0;

uint8_t previous_button_state = 0;

bool soft_wakeup = false;

void go_to_sleep();

void try_wakeup() { // called when asleep and button pressed
    millis_button_pressed = millis();
    detachInterrupt(digitalPinToInterrupt(BUTTON));
    // you have to re-init the button as pullup or else everything 
    // breaks and you spent a week banging your head on a wall
    pinMode(BUTTON, INPUT_PULLUP); 
    soft_wakeup = true;
}

void go_to_sleep() {
    noInterrupts();
    
    TURN_OFF_ALL_LEDS();
    while(GET_BUTTON_STATE() == LOW) {} // wait until button released
    
    detachInterrupt(digitalPinToInterrupt(BUTTON));
    attachInterrupt(digitalPinToInterrupt(BUTTON), try_wakeup, FALLING); // from high to low means button pressed
    
    interrupts();
    
    sleep_cpu();
    // try_wakeup should return to right here
}

typedef void (*ANIMATION_FUNC)(void);
typedef void (*ANIMATION_INIT_FUNC)(void);

const ANIMATION_FUNC animation_init_functions[] = {
    animation_always_on::init,
    animation_fade::init,
    animation_square::init,
    animation_blink::init,
    animation_morse::init,
};
#define NUM_ANIMATION_INIT_FUNCTIONS (sizeof(animation_init_functions) / sizeof(animation_init_functions[0]))
const ANIMATION_FUNC animation_functions[] = {
    animation_always_on::animation,
    animation_fade::animation,
    animation_square::animation,
    animation_blink::animation,
};
#define NUM_ANIMATION_FUNCTIONS (sizeof(animation_functions) / sizeof(animation_functions[0]))

uint8_t current_animation_index = 0;

bool easter_egg = false;

void setup() {
    pinMode(LED_TL, OUTPUT);
    pinMode(LED_TR, OUTPUT);
    pinMode(LED_BL, OUTPUT);
    pinMode(LED_BR, OUTPUT);

    pinMode(BUTTON, INPUT_PULLUP);

    TURN_OFF_ALL_LEDS();

    // disable ADC
    // ADC0.CTRLA &= ~ADC_ENABLE_bm;

    for(uint8_t i = 0; i < NUM_ANIMATION_INIT_FUNCTIONS; i++) {
        animation_init_functions[i]();
    }

    previous_button_state = GET_BUTTON_STATE();

    if(GET_BUTTON_STATE() == LOW) { // button held when battery inserted, enable easter egg
        easter_egg = true;
    }

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable(); // does not put the device to sleep, just enables the real sleep_cpu func

    millis_button_pressed = MILLIS_FAR_OFF;
    millis_button_released = MILLIS_FAR_OFF;
}

bool eat_rising_input = false;
uint8_t debounce_last_state = LOW;
unsigned long millis_debounce_timer = 0;
const unsigned int debounce_delay_millis = 50;

void loop() {
    if(easter_egg) {
        animation_morse::animation();
        return;
    }
    // soft_wakeup is set if device has been triggered awake but button hasnt been held for 1s yet
    if(soft_wakeup) {
        if(GET_BUTTON_STATE() == LOW) { // pressed
            if(millis() > millis_button_pressed + 1000) { // button has been held for 1s, wakeup fully
                // reset button pressing vars to make sure it doesnt immediately get put back to sleep
                millis_button_pressed = MILLIS_FAR_OFF;
                millis_button_released = MILLIS_FAR_OFF;
                soft_wakeup = false;
                eat_rising_input = true;
                // TURN_ON_ALL_LEDS();
                digitalWrite(LED_TR, HIGH);
                // while(GET_BUTTON_STATE() == LOW) {}
                previous_button_state = GET_BUTTON_STATE();
            }
        } else { // released
            // button released before 1s passed, go back to sleep
            soft_wakeup = false;
            go_to_sleep();
        }
        return;
    }

    uint8_t state = GET_BUTTON_STATE();

    if(state != debounce_last_state) {
        millis_debounce_timer = millis();
    }

    if(millis() > millis_debounce_timer + debounce_delay_millis) {
        if(previous_button_state == LOW && state == HIGH) { // rising, released
            millis_button_pressed = MILLIS_FAR_OFF;
            millis_button_released = millis();
            // increment animation index
            if(eat_rising_input) {
                eat_rising_input = false;
            } else {
                current_animation_index = (current_animation_index + 1) % NUM_ANIMATION_FUNCTIONS;
            }
            // current_animation_index = (current_animation_index + 1) % NUM_ANIMATION_FUNCTIONS;
        } else if(previous_button_state == HIGH && state == LOW) { // falling, pressed
            millis_button_pressed = millis();
            millis_button_released = MILLIS_FAR_OFF;
        }

        previous_button_state = state;
    }

    debounce_last_state = state;

    // if button has been held down for 1s, put device to sleep
    if(millis() > millis_button_pressed + 1000) {
        millis_button_pressed = MILLIS_FAR_OFF;
        millis_button_released = MILLIS_FAR_OFF;
        go_to_sleep();
        return;
    }

    animation_functions[current_animation_index]();
}