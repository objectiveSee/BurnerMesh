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

Network         network;

// MUST BE BELOW build.h
#ifdef MAD_MAIN_LOGGING
#define LOGN(x)  Serial.println (x)
#define LOG(x)  Serial.print (x)
#else
#pragma message "Logging is disabled"
#define LOGN(x)
#define LOG(x)
#endif


/**
   Static Members
*/
static bool wireless_input_enabled = true;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
   Main Arduino Functions
*/
void setup() {

  Serial.begin(9600);     // Begin serial
  LOGN(F("Adventure Time!!"));

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

  if ( wireless_input_enabled ) {
    network.Update();
  }

  lights_loop();

  // prevent us from spending all the time inside loop() so interrupt based things can happen
  delay(10);
}


/*
   Passes a command from Wireless "API" into the command handlers
    Byte 0: Header (0x55)
    Byte 1: Header 2 (0xAA) or Remote ID
    Byte 2: Address/Command byte
    Byte 3: Tail (0xFF)
*/
void cmd_handle_from_wireless(byte * commandRcvd ) {

  // we respect the mode here if in a mode where the Lighting contorller sends poofs only.
  // This could be changed. Remote beats all ;)

  if ( commandRcvd[0] != 0x55 ) {
    LOGN("Wireless error: Header is not 0x55");
    return;
  } else if ( commandRcvd[3] != 0xFF ) {
    LOGN("Wireless error: Tail is not 0xFF");
    return;
  }
#if MAD_MAIN_LOGGING
  Serial.print("Wireless received 0x"); Serial.print(commandRcvd[2], HEX);
  Serial.print(" from remote ID# 0x"); Serial.println(commandRcvd[1], HEX);
#endif

  switch (commandRcvd[2]) {

    default:
      LOGN("Unsupported command from wireless.");
      break;
  }
}

/**
   Configure Wireless
*/
void wireless_setup() {

//   LOGN(F("Begin Wireless Setup"));
//   unsigned long function_start = millis();


//   // Set pins
//   Mirf.cePin = 9;
//   Mirf.csnPin = 10;

//   LOGN(F("Configuring Wirless..."));
//   Mirf.spi = &MirfHardwareSpi;
//   Mirf.init();

//   // Configure reciving address.
//   Mirf.setRADDR((byte *)"bonjr");

//   /*
//      Set the payload length to sizeof(unsigned long) the
//      return type of millis().

//      NB: payload on client and server must be the same.
//   */
//   Mirf.payload = sizeof(unsigned long);

//   if ( sizeof(unsigned long) != PAYLOAD_SIZE ) {  // ok to delete after a while
//     Serial.println("BAD PAYLOAD SIZE!");
//     delay(1000);
//     while(1) {}
//   }

//   /*
//      Write channel and payload config then power up reciver.
//      To change channel:
//      Mirf.channel = 10;
//      NB: Make sure channel is legal in your area.
//   */

//   //  Mirf.channel = 90;
//   Mirf.config();
// //  LOG(F("Wireless config finished, took ")); LOG(millis() - function_start);
// //  LOGN(F("ms"));
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
