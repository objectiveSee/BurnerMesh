#include "painlessMesh.h"

#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include "Adafruit_SSD1306.h"

#include "display.h"
#include "lights.h"


#define   MESH_PREFIX     "FakeFrenchWifi"
#define   MESH_PASSWORD   "ThisIsNotSecure"
#define   MESH_PORT       5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain
void report();

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );
Task taskReport( TASK_SECOND * 10 , TASK_FOREVER, &report );

void report() {
  std::list<uint32_t> m_nodes = mesh.getNodeList();
  Serial.printf(" [NET] Connection list (num=%d):", m_nodes.size());
  for (auto node : m_nodes) {
      Serial.printf(" %x ", node);
  }
  Serial.print("\n");
}

void sendMessage() {
  String msg = "Hello from node ";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
    Serial.printf("Changed connections %s\n",mesh.subConnectionJson().c_str());
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();

  userScheduler.addTask( taskReport );
  taskReport.enable();

  displaySetup();

  lights_setup();
}

void loop() {

  userScheduler.execute(); // it will run mesh scheduler as well
  mesh.update();

  static long last_display_update = 0;
  static long last_known_node_count = -1;
  if ( millis() - last_display_update > 1000 ) {
    int node_count = mesh.getNodeList().size();
    if ( node_count != last_known_node_count) {
      Serial.print("Node count is now "); Serial.println(node_count);
      last_known_node_count = node_count;
    }
    displayPrintStats(node_count);
    last_display_update = millis();
  }
  lights_loop();
}
