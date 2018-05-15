#include "lights.h"
#include <Adafruit_NeoPixel.h>

#define LEDS_PER_STRIP 10
#define NEOPIXEL_PIN D8
#define DEFAULT_BRIGHTNESS 10

/**
 * Light modes as enum.
 * NOTE: LIGHT_MODE_OFF is after the LIGHT_MODE_COUNT so it isn't cycled through.
 */
#define DEFAULT_LIGHT_MODE LIGHT_MODE_RAINBOW
static LightMode _lightMode = DEFAULT_LIGHT_MODE;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS_PER_STRIP, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void allColor(unsigned int c) {
  for (int i=0; i < LEDS_PER_STRIP; i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

void lights_setup() {
  strip.setBrightness(DEFAULT_BRIGHTNESS);  // TODO: Debugging only
  strip.begin();
  // allColor(0);
  strip.show();
}

void lights_loop() {
  switch (_lightMode) {
    case LIGHT_MODE_RED: {
      allColor(0xFF0000);
    }
    break;
    case LIGHT_MODE_ORANGE: {
      allColor(0xFF5555);
    }
    break;
    case LIGHT_MODE_GREEN: {
      allColor(0x00FF00);
    }
    break;
    case LIGHT_MODE_BLUE: {
      allColor(0x0000FF);
    }
    break;
    case LIGHT_MODE_RAINBOW: {
      allColor(0x00FFFF);
    }
    break;
    default:{
      allColor(0x222222);
    }
    break;
  }

  int brightness = (millis()/10)%100;
  strip.setBrightness(brightness);
}

void setLightMode(LightMode mode) {
  _lightMode = mode;
}
LightMode lightMode() {
  return _lightMode;
};
