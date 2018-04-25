#include "display.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

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
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("# ");
  display.println(connected_node_count);
  display.display();
}
