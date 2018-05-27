#include "lights.h"
#include "network.h"
#include <Adafruit_NeoPixel.h>

#define LEDS_PER_STRIP 90
#define NEOPIXEL_PIN D8
#define DEFAULT_BRIGHTNESS 20

// Debug flag
#define DONT_CHANGE_LIGHT_MODE_EVER


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

  static int last_count = -1;

  int num_friends = myNetwork->GetNodeCount();
  if ( last_count != num_friends ) {
    last_count = num_friends;
    Serial.print("Node count is "); Serial.println(num_friends);
  }

  for (int i=0; i < LEDS_PER_STRIP; i++) {

    if (  i < num_friends * 10 ) {
      strip.setPixelColor(i, 0x00000000);

    } else {
      strip.setPixelColor(i, c);

    }
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

void lights_loop() {
  
  static uint32_t last_loop = 0;
  static int countSkipped = 0;
  uint32_t time_since = millis() - last_loop;
  if ( time_since < 16 ) {
    // skip light update for performance reasons
    countSkipped++;
    return;
  }
  
  Serial.print("[LIGHT] Skipped "); Serial.print(countSkipped);
  Serial.print("/  \tTime:"); Serial.println(time_since);
  countSkipped = 0;  
  last_loop = millis();
  
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
 
 void showNodeCount() {
   Serial.println("Showing Node Count");
   
   int node_count = (int)network.GetNodeCount();
   Serial.print("Showing Node Count"); Serial.println(node_count);
   int offset = 10;
   
   for (int i=0; i < LEDS_PER_STRIP; i++) {

     if (  i < (node_count * 10)+offset  ) {
       strip.setPixelColor(i, 0xFF0000);
     } else {
       strip.setPixelColor(i, 0x000000);
     }
   }
   strip.show();
 }
