#include <functional>

#include "debug.h"
#include "network.h"
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
Task taskReport( TASK_SECOND * 120 , TASK_FOREVER, &report );


void report() {
  std::list<uint32_t> m_nodes = mesh.getNodeList();
  Serial.printf("[NET] Connection list (num=%d):", m_nodes.size());
  for (auto node : m_nodes) {
      Serial.printf(" 0x%x", node);
  }
  Serial.print("\n");
}

void sendMessage() {
  
  // Serial.println("TODO: Send Message");
  // LightMode new_color = (LightMode)random(5);
  LightMode new_color = (LightMode)0; // TODO: this can be deleted. No point jsut to send a message
  setLightMode(new_color);
  String msg = "Purple Light!!!!";
  Serial.print("[NET] Sending: "); Serial.println(msg);
  mesh.sendBroadcast( msg );
  
  // Reset the interval for sending out info
  taskSendMessage.setInterval( random( TASK_SECOND * 10, TASK_SECOND * 20 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  const char * from_name = nameForNetworkId(from);
//  Serial.printf("[NET] Message from %s (%u) | %s\n", from_name, from, msg.c_str());
  Serial.printf("[NET] Message from %s (%u)\n", (const char *)from_name, from);
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("[NET] New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
    // Serial.printf("Changed connections %s\n",mesh.subConnectionJson().c_str());
    report();
}

void nodeTimeAdjustedCallback(int32_t offset) {
    // Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}


Network::Network()
{
}

Network::~Network()
{
}

void Network::Setup()
{

    randomSeed(analogRead(A0));
  
//  mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
    mesh.setDebugMsgTypes( ERROR | STARTUP | MESH_STATUS | COMMUNICATION);  // set before init() so that you can see startup messages

    mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
    mesh.onReceive(&receivedCallback);
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

    userScheduler.addTask( taskSendMessage );
    taskSendMessage.enable();

    userScheduler.addTask( taskReport );
    taskReport.enable();



		// INFO(" [NET] Initalizing Network...\n");
    // m_mesh.init(MESH_SSID, MESH_PASSWORD, MESH_PORT);
}

void Network::Update()
{
  userScheduler.execute(); // it will run mesh scheduler as well
  mesh.update();
}

uint32_t Network::GetNodeCount()
{
    return mesh.getNodeList().size();
}

uint32_t Network::GetNodeID()
{
   return mesh.getNodeId();
}

const char * nameForNetworkId(uint32_t node_id) {
  switch (node_id) {
    case 572362449:
      return "W1";
      break;
    case 571995149:
      return "W2";
      break;
    case 571513175:
      return "W3";
      break;
    default:
      Serial.print("Unrecognized Node Id "); Serial.println(node_id);
      return "W?";
      break;
  }
}

