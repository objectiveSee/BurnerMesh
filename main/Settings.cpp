// FROM: http://playground.arduino.cc/Code/EEPROMLoadAndSaveSettings

/* LoadAndSaveSettings
 * footswitch 2012-03-05, original code by Joghurt (2010)
 * Demonstrates how to load and save settings to the EEPROM
 * Tested on Arduino Uno R2 with Arduino 0023
 */
// Contains EEPROM.read() and EEPROM.write()
#include <EEPROM.h>
#include <Arduino.h>
#include "build.h"
#include "Settings.h"

// Tell it where to store your config data in EEPROM
#define CONFIG_START 32

// ID of the settings block
#define CONFIG_VERSION "ss04" // must also change `my_settings`

StoreStruct my_settings = {
  's','s','0','4',    // must also change CONFIG_VERSION
  255,
  false
};


bool EEPRomVersionCheck() {
   if (EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
      EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2] &&
      EEPROM.read(CONFIG_START + 3) == CONFIG_VERSION[3]) {
        return true;
   }
   return false;
}

void loadConfig() {
  
  // To make sure there are settings, and they are YOURS! If nothing is found it will use the default settings.
 if (EEPRomVersionCheck() ) {
    // read settings from EEPROM
    for (unsigned int t=0; t<sizeof(my_settings); t++) {
      *((char*)&my_settings + t) = EEPROM.read(CONFIG_START + t);
    }

    #if MAD_SETTINGS_LOGGING
    Serial.print("[SETTINGS] Loaded from EEPRom! NodeId ="); Serial.print(my_settings.nodeId); 
    Serial.print(", isSpecial = "); Serial.print(my_settings.isSpecial); 
    Serial.println(".");
    #endif

  } else {
    #if MAD_SETTINGS_LOGGING
    Serial.print("[SETTINGS] Using Default EEPROM Settings. NodeId ="); Serial.print(my_settings.nodeId); 
    Serial.print(", isSpecial = "); Serial.print(my_settings.isSpecial); 
    Serial.println(".");
    #endif
    // settings aren't valid! will overwrite with default settings
    saveConfig();
  }
}

void saveConfig() {
  for (unsigned int t=0; t<sizeof(my_settings); t++) {
    EEPROM.write(CONFIG_START + t, *((char*)&my_settings + t));
  }
  #if MAD_SETTINGS_LOGGING
  Serial.print("[SETTINGS] Saved new EEPROM settings. NodeId ="); Serial.print(my_settings.nodeId); 
  Serial.print(", isSpecial"); Serial.print(my_settings.isSpecial); 
  Serial.println(".");
  #endif
}

uint8_t getNodeId() {
  return my_settings.nodeId;
}

