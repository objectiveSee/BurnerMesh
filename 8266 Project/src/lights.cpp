#include "lights.h"
#include "network.h"
#include <Adafruit_NeoPixel.h>

#define LEDS_PER_STRIP 16
#define NEOPIXEL_PIN D8
#define DEFAULT_BRIGHTNESS 20

// Debug flag
#define DONT_CHANGE_LIGHT_MODE_EVER
// #define SKIP_LIGHT_LOOP



 void showNodeCount();

/**
 * Light modes as enum.
 * NOTE: LIGHT_MODE_OFF is after the LIGHT_MODE_COUNT so it isn't cycled through.
 */
#define DEFAULT_LIGHT_MODE LIGHT_MODE_NODE_COUNT
static LightMode _lightMode = DEFAULT_LIGHT_MODE;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS_PER_STRIP, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

Network * myNetwork;

void allColor(unsigned int c) {

  for (int i=0; i < LEDS_PER_STRIP; i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

void lights_setup(Network * theirNetwork) {
  myNetwork = theirNetwork;
  
  strip.setBrightness(DEFAULT_BRIGHTNESS);
  strip.begin();
  // allColor(0);
  strip.show();
}

// TODO: fancier lights!

#define LIGHT_UPDATE_MIN_INTERVAL_IN_MS 16

void lights_loop() {
  
#ifdef SKIP_LIGHT_LOOP
  return;
#endif
  
  static uint32_t last_loop = 0;
  static int countSkipped = 0;

  uint32_t now = millis();
  uint32_t time_since = now - last_loop;

  
  // skip light update for performance reasons
  if ( time_since < LIGHT_UPDATE_MIN_INTERVAL_IN_MS ) {
    countSkipped++;
    return;
  }
  
  // Debug how many times function was called before lights updated
#if 0
  Serial.print("[LIGHT] Skipped "); Serial.print(countSkipped);
  Serial.print("  \tTime:"); Serial.println(time_since);
#endif
  countSkipped = 0;  

  last_loop = now;
  
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
    case LIGHT_MODE_NODE_COUNT : {
      showNodeCount();
    }
    break;
    default:{
      allColor(0x222222);
    }
    break;
  }
  
#if 0
  uint32_t loop_time = millis() - now;
  Serial.print("[LIGHT] Light loop took "); Serial.print(loop_time); Serial.println(" ms");
#endif

  // int brightness = (millis()/10)%100;
  // strip.setBrightness(brightness);
}

void setLightMode(LightMode mode) {
#ifndef DONT_CHANGE_LIGHT_MODE_EVER
  _lightMode = mode;
#endif
}
LightMode lightMode() {
  return _lightMode;
};


/**
 * Node Count Animation
 */
 extern Network network;

 #define LEDS_PER_NODE 3
 
 void showNodeCount() {
   
   int node_count = (int)network.GetNodeCount();
//   Serial.print("[LIGHTS] Showing Node Count"); Serial.println(node_count);
   int offset = LEDS_PER_NODE;
   
   for (int i=0; i < LEDS_PER_STRIP; i++) {

     if (  i < (node_count * LEDS_PER_NODE)+offset  ) {
       strip.setPixelColor(i, 0xFF0000);
     } else {
       strip.setPixelColor(i, 0x00FF22);
     }
   }
   strip.show();
 }
