#include <Arduino.h>
#include <painlessMesh.h>
#include "network.h"
#include "debug.h"

// #define   MESH_PREFIX     "whateverYouLike"
// #define   MESH_PASSWORD   "somethingSneaky"
// #define   MESH_PORT       5555

using namespace LightSwarm;
Network         network;

// Scheduler userScheduler; // to control your personal task
// painlessMesh  mesh;

// // User stub
// void sendMessage() ; // Prototype so PlatformIO doesn't complain
//
// Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );
//
// void sendMessage() {
//   Serial.println("Sending Message");
//
//   String msg = "Hello from node ";
//   msg += mesh.getNodeId();
//   mesh.sendBroadcast( msg );
//   taskSendMessage.setInterval( random( TASK_SECOND * 5, TASK_SECOND * 10 ));
// }

// // Needed for painless library
// void receivedCallback( uint32_t from, String &msg ) {
//   Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
// }
//
// void newConnectionCallback(uint32_t nodeId) {
//     Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
// }
//
// void changedConnectionCallback() {
//     Serial.printf("Changed connections %s\n",mesh.subConnectionJson().c_str());
// }
//
// void nodeTimeAdjustedCallback(int32_t offset) {
//     Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
// }

void setup() {
    Serial.begin(115200);
    Serial.println("Hello, World!\n");

    INFO("SETUP 1: NodeId=%08X\n", network.GetNodeID());
    network.Init();
    INFO("SETUP 2: NodeId=%08X\n", network.GetNodeID());


// mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  // mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  //
  // mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  // mesh.onReceive(&receivedCallback);
  // mesh.onNewConnection(&newConnectionCallback);
  // mesh.onChangedConnections(&changedConnectionCallback);
  // mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  //
  // userScheduler.addTask( taskSendMessage );
  // taskSendMessage.enable();
}

void loop() {
  // userScheduler.execute(); // it will run mesh scheduler as well
  // mesh.update();

  network.Update();
  delay(100);
}
