#include <functional>

#include "network.h"
#include "Settings.h"

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

// Payload size used in nRF24L01 communication
#define PAYLOAD_SIZE 4


// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain
void report();

//Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );
//Task taskReport( TASK_SECOND * 120 , TASK_FOREVER, &report );


void report() {
  // std::list<uint32_t> m_nodes = mesh.getNodeList();
  // Serial.printf("[NET] Connection list (num=%d):", m_nodes.size());
  // for (auto node : m_nodes) {
  //     Serial.printf(" 0x%x", node);
  // }
  // Serial.print("\n");
  Serial.println("TODO report()");
}

void sendMessage() {
  
  // Serial.println("TODO: Send Message");
  // LightMode new_color = (LightMode)random(5);
  // LightMode new_color = (LightMode)0; // TODO: this can be deleted. No point jsut to send a message
  // setLightMode(new_color);
  // String msg = "Purple Light!!!!";
  // Serial.print("[NET] Sending: "); Serial.println(msg);
  // mesh.sendBroadcast( msg );
  


  // Reset the interval for sending out info
//  taskSendMessage.setInterval( random( TASK_SECOND * 10, TASK_SECOND * 20 ));
}

Network::Network()
{
}

Network::~Network()
{
}

void Network::Setup()
{
  Serial.println(F("Begin Wireless Setup"));
  unsigned long function_start = millis();

  // Set pins
  Mirf.cePin = 9;
  Mirf.csnPin = 10;

  Serial.println(F("Configuring Wirless..."));
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
  Serial.print(F("Wireless config finished, took ")); Serial.print(millis() - function_start);
  Serial.println(F("ms"));
    
}

void Network::Update()
{
  
  static unsigned long last_send = 0;
  if ( millis() - last_send > 5000 ) {

    Serial.println("Sending");
    last_send = millis();

    Mirf.setTADDR((byte *)"bonjr");

    uint8_t n = getNodeId();

    byte payload[PAYLOAD_SIZE] = {0xDE,0xAD,n,(byte)random(255)};
    
    Mirf.send((byte *)payload);    
    while(Mirf.isSending()){
    }
    Serial.println("Finished sending");
    delay(10);
    
  }

  if( ! Mirf.isSending() && Mirf.dataReady() ) {
    delay(10);
    byte payload[PAYLOAD_SIZE];
    Mirf.getData((byte *) &payload);
  
    Serial.print("Received: ");
    for ( byte i=0; i<PAYLOAD_SIZE; i++) {
      Serial.print(payload[i], HEX); Serial.print(" ");
    }
    Serial.println("");
    
  }

  delay(10);
}

// uint32_t Network::GetNodeCount()
// {
//     return mesh.getNodeList().size();
// }

// uint32_t Network::GetNodeID()
// {
//    return mesh.getNodeId();
// }

// const char * nameForNetworkId(uint32_t node_id) {
//   switch (node_id) {
//     case 572362449:
//       return "W1";
//       break;
//     case 571995149:
//       return "W2";
//       break;
//     case 571513175:
//       return "W3";
//       break;
//     default:
//       Serial.print("Unrecognized Node Id "); Serial.println(node_id);
//       return "W?";
//       break;
//   }
// }

