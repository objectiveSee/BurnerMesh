// Arduino Libraries
#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <CmdMessenger.h>


// Required by MIRF
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

// NOTE for driving LEDs:
// Teensy LC: A 74LV1T125 buffer is connected to pin 17, with
// the increased output voltage available on another pin.

/**
   Project Files
*/
#include "build.h"  // Project settings such as pins or build flags
#include "Settings.h"
#include "network.h"
#include "lights.h"
#include "Button.h"

Network         network;

/**
   Static Members
*/
static Button button0(15);
static Button button1(16);


// indicates a time (in ms) when we should broadcast a mode change. 
static unsigned long last_mode_send_change = 0;
#define kLIGHT_MODE_SEND_DELAY 2000


/*
 * Functions
 */
void process_message(byte * message);
void buttonWasPressed(byte button_id);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
   Main Arduino Functions
*/
void setup() {

  Serial.begin(9600);     // Begin serial
  LOGN(F("Adventure Time 2019!!"));

  // configure the SlaveControllerSerial
  //  if ( serial_input_enabled ) {
  //    SlaveSerial.begin(4800);
  //  }


  blinkLEDOnBootup();     // Blink the LED to give some time to reset Teensy if needed

  loadConfig();           // load important values about angles and other app settings that need to persist

  //  LOGN(F("Begin Accelerometer Setup"));
  //  delay(100);
  //
  //  // Initalize the Accelerometer module
  //  accelerometer = new Accelerometer();
  //  accelerometer->setup();

  // Wireless configure
  network.Setup();

  lights_setup(&network);

  delay(2000);

  LOGN(F("Setup complete"));
}

void loop() {
    
  network.Update();

  // check for time to send a mode change
  unsigned long t = millis();
  if ( t > last_mode_send_change && last_mode_send_change != 0 ) {
    Serial.println("Time to send mode change!");
    network.SendModeChange((byte)lightMode());
    last_mode_send_change = 0;
  }

  button0.loop();
  if(button0.stateChanged() ) {
    Serial.println("Button 0 changed state!");
    if ( button0.isOn() ) {
      buttonWasPressed(0);
    }
  }
  button1.loop();
  if(button1.stateChanged() ) {
    Serial.println("Button 1 changed state!");
    if ( button1.isOn() ) {
      buttonWasPressed(1);
    }
  }

  lights_loop();

  // prevent us from spending all the time inside loop() so interrupt based things can happen
  delay(10);
}

void process_message(byte * msg) {

  Serial.print("Incoming Message: ");
  for ( byte i=0; i<MESSAGE_SIZE; i++) {
    Serial.print(msg[i], HEX); Serial.print(" ");
  } 
  Serial.println("END");

//  byte senderId = msg[0];
  byte commandId = msg[1];

  switch(commandId) {
    case COMMAND_MODE_CHANGE:
    {
      byte newMode = msg[2];
      setLightMode((LightMode)newMode);
    }
    break;
    default:{
      
    }
    break;
  }

}

/**
   Blinks the on-board LED when setup() is called. Give some time to reset Teensy if needed.
   Dont use LED after setup() as pin 13 is also used for the nRF24L01 chip wiring (?? still true?)
*/
void blinkLEDOnBootup() {
#define BLINKY_TIME 100
  pinMode(LED_BUILTIN, OUTPUT);
  for ( int i = 0; i < 10; i++ ) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(BLINKY_TIME);
    digitalWrite(LED_BUILTIN, LOW);
    delay(BLINKY_TIME);
  }
}

/**
 * Application Logic
 */
void buttonWasPressed(byte button_id) {
  if ( button_id == 0 ) {
    advanceLightModeFun();
  } else {
    advanceLightModeNotice();
  }

  last_mode_send_change = millis() + kLIGHT_MODE_SEND_DELAY;
}
