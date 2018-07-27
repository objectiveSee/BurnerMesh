#include "display.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "network.h"

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

extern Network network;

void displaySetup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // init done

  // uncomment for adafruit image on bootup
  // display.display();
  // delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  // text display tests
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println("Hello");
  display.println("World");

  display.display();
  // delay(10000);
  // display.clearDisplay();

}

void displayPrintStats(int connected_node_count) {
  
  static long last_display_update = 0;
  static long last_known_node_count = -1;
  bool shouldUpdateDisplay = false;
  
  uint32_t now = millis();
  
  // Show "Hello World" for first 5 seconds
  if ( millis() < now ) {
    return;
  }
  
  if ( now - last_display_update > 100 ) {
    // int node_count = network.GetNodeCount();

    if ( connected_node_count != last_known_node_count ) {
      
      last_known_node_count = connected_node_count;
      Serial.print("[DISPLAY] Nodes # "); Serial.println(connected_node_count);
      shouldUpdateDisplay = true;
      
    } else if ( now - last_display_update > 5000 ) {
      // update every 5 seconds regardless in case message to OLED was missed.
      shouldUpdateDisplay = true;
    }
    last_display_update = now;
  }
  
  
  if ( shouldUpdateDisplay ) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("# ");
    display.println(connected_node_count);

    const char * myNodeName = nameForNetworkId(network.GetNodeID());
    display.print(myNodeName);
    display.display();
  }
}
