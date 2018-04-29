#pragma once

typedef enum {LIGHT_MODE_RED = 0, LIGHT_MODE_ORANGE, LIGHT_MODE_GREEN, LIGHT_MODE_BLUE, LIGHT_MODE_RAINBOW, LIGHT_MODE_COUNT, LIGHT_MODE_OFF} LightMode;


void lights_setup();
void lights_loop();

void setLightMode(LightMode mode);
LightMode lightMode();
