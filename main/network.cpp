#include <functional>

#define MAD_NETWORK_LOGGING 1

#include "build.h"
#include "network.h"
#include "Settings.h"

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>



byte currentOutgoingMessage[PAYLOAD_SIZE];
bool outgoingMessage = false;
int sendCount = 0;
unsigned long lastSend = 0;
#define RESEND_COUNT 5
#define RESEND_INTERVAL 50



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
  Mirf.payload = PAYLOAD_SIZE;

  /*
     Write channel and payload config then power up reciver.
     To change channel:
     Mirf.channel = 10;
     NB: Make sure channel is legal in your area.
  */


  // According to documentation, the `RF_PWR` mask of register 0x06 is by default `11` which means highest power setting

  // set to a non-default channel (under 80 for the USA) to avoid the busier channel
  Mirf.channel = 53;
  Mirf.config();
#if MAD_NETWORK_LOGGING
  Serial.print(F("Wireless config finished, took ")); Serial.print(millis() - function_start);
  Serial.println(F("ms"));    
#endif
}





bool shouldSend() {
  if ( outgoingMessage && sendCount < RESEND_COUNT ) {
//    Serial.print("check resend || "); Serial.println(lastSend);
    if ( lastSend + RESEND_INTERVAL < millis() ) {
//      Serial.println("pass");
      return true;
      
    }
  }
  return false;
}

void Network::Update()
{
  
  _message_received = false; // will be set to true inside HandleMessage if message was received

  // Skip if message is still sending or data isn't ready (nothing new)
  if( ! Mirf.isSending() && Mirf.dataReady() ) {
  
    delay(10);
    byte payload[PAYLOAD_SIZE];
    Mirf.getData((byte *) &payload);
  
    HandleMessage(payload);    
  }

  if ( shouldSend() ) {
    lastSend = millis();
    Serial.print("Sending message attempt #"); Serial.print(sendCount); Serial.print(" --"); Serial.println(lastSend);
    sendCount++;
    SendMessageFullPayload(currentOutgoingMessage);
    if ( sendCount >= RESEND_COUNT ) {
      // reset
      lastSend = 0;
      sendCount = 0;
      outgoingMessage = false;
    }
  }

}

bool Network::MessageReceived() {
  return _message_received;
}
byte * Network::GetMessage() {
  return _message;
}

/**
 * Packet Structure:
 * Byte #
 * 0 | Header
 * 1..3 | Message
 * 5 | Check Sum
 */

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

    // copy into the class' message buffer
    for (byte i=0; i<MESSAGE_SIZE; i++) {
//      Serial.print(i); Serial.print(" = "); Serial.println(message[i+1], HEX);  
      _message[i] = message[i+1];
    }
  }
}

void Network::SendModeChange(byte modeAsByte) {
  byte modeChange[3] = {COMMAND_MODE_CHANGE, modeAsByte, 0};

  // cancel any previous messages outoing and send your own
  byte newmsg[PAYLOAD_SIZE];
  newmsg[0] = 0xDA;  // header
  newmsg[1] = getNodeId();
  newmsg[2] = COMMAND_MODE_CHANGE; 
  newmsg[3] = modeAsByte;
  newmsg[4] = 0x00;
  newmsg[5] = stringChecksum(newmsg);
  
  
  // copy message into the send buffer. 
  memcpy(&currentOutgoingMessage, newmsg, PAYLOAD_SIZE);

  outgoingMessage = true;
  sendCount = 0;
  lastSend = 0;

  // Next loop will now send the message!
}

void Network::SendMessageFullPayload(byte* payload) {

#if MAD_NETWORK_LOGGING
  Serial.print("NEW: Sending paylod: ");
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


/**
 * Formats and sends a payload via wireless.
 * 
 * @param message - pointer to a MESSAGE_SIZE-1 byte array
 * 
 * The subraction is needed as 1 byte of message is the nodeId.
 */
//void Network::SendMessage(byte* message)
//{
//  
//  Mirf.setTADDR((byte *)"bonjr");
//
//  uint8_t n = getNodeId();
//
//  byte payload[PAYLOAD_SIZE];
//  payload[0] = 0xDA;  // header
//  payload[1] = n;
//  
//  // copy message into the send buffer. Subtract one because one byte of message is the node Id
//  // which gets set by this function.
//  memcpy(&payload[2], message, MESSAGE_SIZE - 1); 
//  
//  byte checksum = stringChecksum(payload);
//  payload[PAYLOAD_SIZE - 1] = checksum;
//
//#if MAD_NETWORK_LOGGING
//  Serial.print("Sending paylod: ");
//  for ( byte i=0; i<PAYLOAD_SIZE; i++) {
//    Serial.print(payload[i], HEX); Serial.print(" ");
//  }
//  Serial.println("");
//#endif
//  
//  Mirf.send((byte *)payload);
//  while(Mirf.isSending()){  // TODO: add timeout?
//  }
//#if MAD_NETWORK_LOGGING
//  Serial.println("Finished sending");
//#endif
//  delay(10);
//}

