#pragma once

#include "network.h"

typedef enum {
  LIGHT_MODE_RED = 0, LIGHT_MODE_GREEN, LIGHT_MODE_BLUE, LIGHT_MODE_PURPLE,  // "Notice" modes
  LIGHT_MODE_RAINBOW, LIGHT_MODE_TWINKLE, LIGHT_MODE_FIRE,  LIGHT_MODE_SINELON, LIGHT_MODE_PLASMA, LIGHT_MODE_BLENDWAVE,                    // "Fun" modes
  LIGHT_MODE_COUNT, LIGHT_MODE_OFF} LightMode;

#define COUNT_NOTICE_MODES 4
#define COUNT_FUN_MODES 6


void lights_setup(Network *);
void lights_loop();

void advanceLightModeFun();
void advanceLightModeNotice();
void setLightMode(LightMode mode);
LightMode lightMode();
