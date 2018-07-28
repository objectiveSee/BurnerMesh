//////////////////////////////////////////////////
// Pins
//////////////////////////////////////////////////


//#define MAD_USE_UNUSED_LEDS_FOR_STATUS
//#define UNUSED_RELAY_PIN_0 20
//#define UNUSED_RELAY_PIN_1 23

// Buttons
//#define BUTTON_0_PIN 6
//#define BUTTON_1_PIN 5

//////////////////////////////////////////////////
// Logging
//////////////////////////////////////////////////

#define MAD_MAIN_LOGGING                  1
//#define MAD_ACCELEROMETER_LOGGING         1
//#define MAD_ACCELEROMETER_LOGGING_VERBOSE 0
#define MAD_BUTTON_LOGGING                0
//#define MAD_RELAY_LOGGING                 1
#define MAD_SETTINGS_LOGGING              1



// MUST BE BELOW build.h
#ifdef MAD_MAIN_LOGGING
#define LOGN(x)  Serial.println (x)
#define LOG(x)  Serial.print (x)
#else
#pragma message "Logging is disabled"
#define LOGN(x)
#define LOG(x)
#endif


