#include <functional>

#include "network.h"
#include "Settings.h"

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

// Payload size used in nRF24L01 communication
#define PAYLOAD_SIZE 4

byte stringChecksum(byte *s)
{
  byte c = 0;
  for ( byte i = 0; i < PAYLOAD_SIZE-1; i++ ) {
    c += s[i]; 
  }
  return c;
}

Network::Network()
{
}

Network::~Network()
{
}

void Network::Setup()
{
#if MAD_NETWORK_LOGGING
  Serial.println(F("Begin Wireless Setup"));
#endif
  unsigned long function_start = millis();

  // Set pins
  Mirf.cePin = 9;
  Mirf.csnPin = 10;

  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();

  // Configure reciving address.
  Mirf.setRADDR((byte *)"bonjr");

  /*
     Set the payload length to sizeof(unsigned long) the
     return type of millis().

     NB: payload on client and server must be the same.
  */
  Mirf.payload = sizeof(unsigned long);

  if ( sizeof(unsigned long) != PAYLOAD_SIZE ) {  // ok to delete after a while
    Serial.println("BAD PAYLOAD SIZE!");
    delay(1000);
    while(1) {}
  }

  /*
     Write channel and payload config then power up reciver.
     To change channel:
     Mirf.channel = 10;
     NB: Make sure channel is legal in your area.
  */

//  Mirf.channel = 53;
  Mirf.config();
#if MAD_NETWORK_LOGGING
  Serial.print(F("Wireless config finished, took ")); Serial.print(millis() - function_start);
  Serial.println(F("ms"));    
#endif
}

void Network::Update()
{

  // Heartbeat message
  static unsigned long next_send = 10000;
  if ( millis() > next_send ) {
    next_send = millis() + random(10000,60000); // 10-60 seconds
    SendMessage(0x0A);
  }

  _message_received = false; // will be set to true inside HandleMessage if message was received

  if( ! Mirf.isSending() && Mirf.dataReady() ) {
  
    delay(10);
    byte payload[PAYLOAD_SIZE];
    Mirf.getData((byte *) &payload);
  
    HandleMessage(payload);    
  }
}

bool Network::MessageReceived() {
  return _message_received;
}
byte Network::GetMessage() {
  return _message;
}
byte Network::GetMessageSender() {
  return _message_from;
}

void Network::HandleMessage(byte* message) {

#if MAD_NETWORK_LOGGING
  Serial.print("Received: ");
  for ( byte i=0; i<PAYLOAD_SIZE; i++) {
    Serial.print(message[i], HEX); Serial.print(" ");
  }
  Serial.println("");
#endif
 
  byte checksum = stringChecksum(message);
  if ( message[0] != 0xDA || message[PAYLOAD_SIZE-1] != checksum ) {
    Serial.print("Invalid header or checksum. Expected checksum is ");
    Serial.println(checksum, HEX);
  } else {
    // save the message and set flag for 1 loop iteration
    _message_received = true;
    _message = message[2];
    _message_from = message[1];
  }
}

void Network::SendMessage(byte message)
{
  
  Mirf.setTADDR((byte *)"bonjr");

  uint8_t n = getNodeId();

  byte payload[PAYLOAD_SIZE] = {0xDA, n, message, 0x00};

  byte checksum = stringChecksum(payload);
  payload[PAYLOAD_SIZE - 1] = checksum;

#if MAD_NETWORK_LOGGING
  Serial.print("Sending: ");
  for ( byte i=0; i<PAYLOAD_SIZE; i++) {
    Serial.print(payload[i], HEX); Serial.print(" ");
  }
  Serial.println("");
#endif
  
  Mirf.send((byte *)payload);
  while(Mirf.isSending()){  // TODO: add timeout?
  }
#if MAD_NETWORK_LOGGING
  Serial.println("Finished sending");
#endif
  delay(10);
}

