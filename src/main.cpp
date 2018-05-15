/**
 * Main entry point for LED bike pole project.
 *
 * Author: Danny Ricciotti
 */

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

  network.Setup();

  displaySetup();
  
  lights_setup();
}

void loop() {

  network.Update();
  //
  // static long last_display_update = 0;
  // static long last_known_node_count = -1;
  // if ( millis() - last_display_update > 1000 ) {
  //   int node_count = mesh.getNodeList().size();
  //   if ( node_count != last_known_node_count) {
  //     Serial.print("Node count is now "); Serial.println(node_count);
  //     last_known_node_count = node_count;
  //   }
  //   displayPrintStats(node_count);
  //   last_display_update = millis();
  // }
  //
  // lights_loop();
}
