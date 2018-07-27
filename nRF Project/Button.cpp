#include <Arduino.h>

#include "Button.h"
#include "build.h"

#define DEBOUNCE_DURATION 20

Button::Button( int pin_num )
{
  my_pin = pin_num;
  debounced_state = true;    // active low
  last_state = true;         // active low
  last_state_change = 0;

  // enable pull-up resistor to prevent floating input when button is not being pressed.
  pinMode(my_pin, INPUT_PULLUP);
}

bool Button::isOn() {
  // button is active low
  return !debounced_state;
}

void Button::loop() {

	// debounce the button

	unsigned long current_time = millis();
	bool state_now = digitalRead(my_pin);

//        Serial.print("Input = "); Serial.println(state_now?"HIGH":"LOW");

	if ( last_state != state_now ) {
		last_state_change = current_time;
	} else {
        	if ( (debounced_state != state_now)  && (current_time - last_state_change > DEBOUNCE_DURATION) ) {
		  debounced_state = state_now;
#if MAD_BUTTON_LOGGING
		  Serial.print("Button (pin "); Serial.print(my_pin); Serial.print(") is now "); Serial.println(debounced_state?"Released":"Pressed");
#endif
	       }
        }

	last_state = state_now;
}

