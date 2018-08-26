#include "build.h"
#include "lights.h"
#include "network.h"

#include "WS2812Serial.h"
#define USE_WS2812SERIAL
#include <FastLED.h>
#define NUM_LEDS 90 
CRGB leds[NUM_LEDS];
#define PIN 24

#define DEFAULT_BRIGHTNESS 255

// Debug flag
//#define DONT_CHANGE_LIGHT_MODE_EVER

CRGB wheel(int WheelPos, int dim);
Network *myNetwork;

uint8_t gHue = 0; // rotating "base color" used by many of the patterns


void setPixelHeatColor (int Pixel, byte temperature);
void Fire(int Cooling, int Sparking);
void TwinkleRandom();
void Rainbow(byte, int, int);
void plasma();
void blendwave();
void sinelon();

/**
 * Light modes as enum.
 * NOTE: LIGHT_MODE_OFF is after the LIGHT_MODE_COUNT so it isn't cycled through.
 */
LightMode _lightMode = LIGHT_MODE_RED;


void advanceLightModeFun() {
  
  if ( (int)_lightMode > COUNT_NOTICE_MODES ) {
    
    // switch from fun mode to fun mode (advance by 1)
    int nextMode = (int)_lightMode - COUNT_NOTICE_MODES;      // find offset of current mode into just the "fun" ones
    
    nextMode = (nextMode+1) % COUNT_FUN_MODES;                    // advance by 1 mode
    nextMode = nextMode + COUNT_NOTICE_MODES;                 // add back the offset from the non-fun ones
    setLightMode((LightMode)nextMode);

    } else {

    // set to 1st fun mode
    int nextMode = COUNT_NOTICE_MODES + 1;
    setLightMode((LightMode)nextMode);
  }
}

// TODO: when switching from fun to notice, or vice versus, it's off by 1

void advanceLightModeNotice() {

  int nowMode = (int)_lightMode;
  
  if ( nowMode < COUNT_NOTICE_MODES) {
    nowMode = (nowMode + 1) % COUNT_NOTICE_MODES;
  } else {
    // set to fire notice mode
    nowMode = LIGHT_MODE_RED;
  }
  setLightMode((LightMode)nowMode);
}

void lights_setup(Network * theirNetwork) {
  
  myNetwork = theirNetwork;
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);

  // NOTE: since you're using double strips, you need to halve the mA here.
  // EG: setting 500mA will limit the bike pole to 1000mA
  FastLED.addLeds<WS2812SERIAL, PIN, BRG>(leds, NUM_LEDS);
  set_max_power_in_volts_and_milliamps(5, 500);

}

void allColor(CRGB color) {
  for(int led = 0; led < NUM_LEDS; led++) { 
    leds[led] = color; 
  }
}

void lights_loop() {

  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow

  switch(_lightMode) {
    case LIGHT_MODE_RED: {
        allColor(CRGB(255,0,0));
    }
    break;
    case LIGHT_MODE_PURPLE: {
      allColor(0x5D2275);
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
    case LIGHT_MODE_FIRE: {
      Fire(40,255);
    }
    case LIGHT_MODE_TWINKLE: {
      TwinkleRandom();
    }
    break;
    case LIGHT_MODE_RAINBOW: {
      Rainbow(10,30,100);
    }
    break;
    
    case LIGHT_MODE_PLASMA: {
      plasma();
    }
    break;

    case LIGHT_MODE_BLENDWAVE: {
      blendwave();
    }
    break;
 
    case LIGHT_MODE_SINELON: {
      sinelon();
    }
    break;
    
    default:{
      
    }
    break;
  }

  FastLED.show();
  
  
#if 0
  uint32_t loop_time = millis() - now;
  Serial.print("[LIGHT] Light loop took "); Serial.print(loop_time); Serial.println(" ms");
#endif

}

void setLightMode(LightMode mode) {

  LOG("Light mode changing from "); LOG((int)_lightMode);
  LOG(" to "); LOGN((int)mode);
  
#ifndef DONT_CHANGE_LIGHT_MODE_EVER
  _lightMode = mode;
#endif
}
LightMode lightMode() {
  return _lightMode;
};

CRGB wheel(int WheelPos, int dim) {
  CRGB color;
  if (85 > WheelPos) {
   color.r=0;
   color.g=WheelPos * 3/dim;
   color.b=(255 - WheelPos * 3)/dim;;
  } 
  else if (170 > WheelPos) {
   color.r=WheelPos * 3/dim;
   color.g=(255 - WheelPos * 3)/dim;
   color.b=0;
  }
  else {
   color.r=(255 - WheelPos * 3)/dim;
   color.g=0;
   color.b=WheelPos * 3/dim;
  }
  return color;
}

/**
 *  Fire Animation!
 *  I think it came from here: 
 *  https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDEffects
 */
void Fire(int Cooling, int Sparking) {
  static byte heat[NUM_LEDS];
  int cooldown;

  bool shouldRenderFrame = false;
  EVERY_N_MILLISECONDS( 20 ) {
    shouldRenderFrame = true;
  }
  if ( !shouldRenderFrame ) {
//    LOGN("SKipping Frame");
    return;
  } 
//  else {
//    LOGN("Not skilling");
//  }
  
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
    
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
  
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
    
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < NUM_LEDS; j++) {
    setPixelHeatColor(j, heat[j] );
  }

}

void setPixel(int Pixel, byte red, byte green, byte blue);
void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

/**
 * Twinkle Animation
 */
void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void TwinkleRandom() {
  
   fadeall();
   if ( random(4) == 0 ) {
    setPixel(random(NUM_LEDS),random(0,255),random(0,255),random(0,255));
   }
}

/**
 * Rainbow
 */
void Rainbow(byte nthLedOff, int rainbowSpeed, int pixelSpeed) {
  // void  fill_rainbow (struct CRGB *pFirstLED, int numToFill, uint8_t initialhue, uint8_t deltahue=5)
//  fill_solid( leds, NUM_LEDS, CRGB(50,0,200)); 

  byte pos = (millis()/rainbowSpeed);
  byte offPos = (millis()/pixelSpeed)%nthLedOff;

  fill_rainbow( leds, NUM_LEDS, pos, 1);

  for (int i = 0; i < NUM_LEDS; i++ ) {
      if ( i%nthLedOff == offPos ) {
      leds[i] = CRGB::Black;
    }
  }
}


// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?b:0)                              // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0

CRGBPalette16 currentPalette = PartyColors_p;
TBlendType    currentBlending = LINEARBLEND;                  // NOBLEND or LINEARBLEND

void plasma() {                                                 // This is the heart of this program. Sure is short. . . and fast.

  int thisPhase = beatsin8(6,-64,64);                           // Setting phase change for a couple of waves.
  int thatPhase = beatsin8(7,-64,64);

  for (int k=0; k<NUM_LEDS; k++) {                              // For each of the LED's in the strand, set a brightness based on a wave as follows:

    int colorIndex = cubicwave8((k*23)+thisPhase)/2 + cos8((k*15)+thatPhase)/2;           // Create a wave and add a phase change and add another wave with its own phase change.. Hey, you can even change the frequencies if you wish.
    int thisBright = qsuba(colorIndex, beatsin8(7,0,96));                                 // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..

    leds[k] = ColorFromPalette(currentPalette, colorIndex, thisBright, currentBlending);  // Let's now add the foreground colour.
  }
}

void blendwave() {

  CRGB clr1;
  CRGB clr2;
  uint8_t speed;
  uint8_t loc1;
  uint8_t loc2;
  uint8_t ran1;
  uint8_t ran2;

  speed = beatsin8(6,0,255);

  #define BLENDYBRIGHT 128

  clr1 = blend(CHSV(beatsin8(3,0,255),255,BLENDYBRIGHT), CHSV(beatsin8(4,0,255),255,BLENDYBRIGHT), speed);
  clr2 = blend(CHSV(beatsin8(4,0,255),255,BLENDYBRIGHT), CHSV(beatsin8(3,0,255),255,BLENDYBRIGHT), speed);

  loc1 = beatsin8(10,0,NUM_LEDS-1);
  
  fill_gradient_RGB(leds, 0, clr2, loc1, clr1);
  fill_gradient_RGB(leds, loc1, clr2, NUM_LEDS-1, clr1);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}
  
