/**
 * Main entry point for LED bike pole project.
 *
 * Author: Danny Ricciotti
 */

#ifndef MAIN_ROUTINE_BIKE
#define MAIN_ROUTINE_BIKE

#include "painlessMesh.h"

#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include "Adafruit_SSD1306.h"

#include "display.h"
#include "lights.h"
#include "network.h"

Network         network;

void setup() {
  Serial.begin(115200);
  Serial.println("Good Day!");

  network.Setup();

  displaySetup();

  lights_setup(&network);
}

void loop() {
  
  userScheduler.execute();
  
  // Time the loop
  uint32_t loop_start = millis();

  // Update network
  uint32_t t = loop_start;
  network.Update();
  uint32_t network_time = millis() - t;
  
  // Update the display
  t = millis();
  displayPrintStats(network.GetNodeCount());  
  uint32_t display_time = millis() - t;

  // Update the lights
  t = millis();
  lights_loop();
  uint32_t light_time = millis() - t;
  
  // Print timing logs (DEBUG)
  t = millis();
  uint32_t loop_duration = millis() - loop_start;
  if ( loop_duration > 10 ) {
    Serial.print("[DEBUG] Long Loop: "); Serial.print(loop_duration);
    Serial.print(" \tDisplay: "); Serial.print(display_time);
    Serial.print(" \tNetwork: "); Serial.print(network_time);
    Serial.print(" \tLight: "); Serial.print(light_time);
    uint32_t serial_time= millis() - t;
    Serial.print(" \tSerial: "); Serial.println(serial_time);
  }
}

#endif  // MAIN_ROUTINE_BIKE
