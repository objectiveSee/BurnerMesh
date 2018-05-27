#include <functional>

#include "debug.h"
#include "network.h"
#include "lights.h"
// #include "version.h"

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
  
  // Serial.println("TODO: Send Message");
  // LightMode new_color = (LightMode)random(5);
  LightMode new_color = (LightMode)0; // TODO: this can be deleted. No point jsut to send a message
  setLightMode(new_color);
  String msg = "I choose "+new_color;
  Serial.print("[ADVENTURE] I am choosing "); Serial.println(new_color);
  // msg += mesh.getNodeId();
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

// namespace BurnerMesh {


Network::Network()
{
	// INFO(" [NET] Network Alloc\n");
	// using namespace std::placeholders;
	//
	// // CAN THIS BE DONE IN THE CONSTRUCTOR???
	// // m_mesh.setDebugMsgTypes(ERROR | MESH_STATUS);// | COMMUNICATION);		// <-- Good value for production LightSwarm code
	// m_mesh.setDebugMsgTypes(ERROR | DEBUG | STARTUP | CONNECTION);  // set before init() so that you can see startup messages
	// // m_mesh.setDebugMsgTypes(ERROR | DEBUG | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE );
	//
	// m_mesh.onReceive(             std::bind(&Network::ReceivedCallback, this, _1, _2));
	// m_mesh.onNewConnection(       std::bind(&Network::NewConnectionCallback, this, _1));
	// m_mesh.onChangedConnections(  std::bind(&Network::ChangedConnectionCallback, this));
	// m_mesh.onNodeTimeAdjusted(    std::bind(&Network::NodeTimeAdjustedCallback, this, _1));
	// m_mesh.onNodeDelayReceived(   std::bind(&Network::DelayReceivedCallback, this, _1, _2));
	//
	// randomSeed(analogRead(A0));
}

Network::~Network()
{
}

void Network::Setup()
{
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


		// INFO(" [NET] Initalizing Network...\n");
    // m_mesh.init(MESH_SSID, MESH_PASSWORD, MESH_PORT);
}

void Network::Update()
{
  userScheduler.execute(); // it will run mesh scheduler as well
  mesh.update();

// 		// INFO(" [NET] Update");
//     m_mesh.update();
//
//
//     // get next random time for send message
//     if (m_sendMessageTime == 0) {
//         m_sendMessageTime = m_mesh.getNodeTime() + random(5000000, 10000000); // 5-10 sec
// 		}
//
//     // if the time is ripe, send everyone a message!
//     if (m_sendMessageTime != 0 && (int)m_sendMessageTime - (int)m_mesh.getNodeTime() < 0)
//     {
//
//         m_helloCounter++;
// #if 1
//         INFO(" [NET] Broadcasting hello! count=%u smt=%d time=%d diff=%d\n",
//             m_helloCounter,
//             (int)m_sendMessageTime,
//             (int)m_mesh.getNodeTime(),
//             (int)m_sendMessageTime - (int)m_mesh.getNodeTime());
//
//         String msg = "Hello #" +
//             String(m_helloCounter, DEC) +
//             " from-id=" +
//             String(m_mesh.getNodeId(), HEX);
//
//         bool error = m_mesh.sendBroadcast(msg);
// #endif
//         m_sendMessageTime = 0;
//
//         if (m_calcDelay)
//         {
// 						INFO("Doing delay calc\n");
//             for (auto node : m_nodes) {
//                 m_mesh.startDelayMeas(node);
// 						}
//             m_calcDelay = false;
//         }
//     }
}

// uint32_t Network::GetTime()
// {
//     return m_mesh.getNodeTime();
// }
//
// uint32_t Network::GetNodeID()
// {
//     return m_mesh.getNodeId();
// }
//
// int32_t Network::GetNodeOffset()
// {
//     const uint32_t  NODE_Jelly_0 = 0x000000;
//     const uint32_t  NODE_Jelly_1 = 0x000001;
//     const uint32_t  NODE_Jelly_2 = 0x000002;
//     const uint32_t  NODE_Jelly_3 = 0x000003;
//     const uint32_t  NODE_Jelly_4 = 0x000004;
//     const uint32_t  NODE_Jelly_5 = 0x000005;
//     const uint32_t  NODE_Dong_0  = 0x000006;
//     const uint32_t  NODE_Dong_1  = 0x000007;
//     const uint32_t  NODE_Dong_2  = 0x000008;
//     const uint32_t  NODE_Dong_3  = 0x000009;
//     const uint32_t  NODE_Dong_4  = 0x000010;
//
//     switch (GetNodeID())
//     {
//         case NODE_Jelly_0: return 0;
//         case NODE_Jelly_1: return 4;
//         case NODE_Jelly_2: return 8;
//         case NODE_Jelly_3: return 12;
//         case NODE_Jelly_4: return 16;
//         case NODE_Jelly_5: return 20;
//
//         case NODE_Dong_0: return 0;
//         case NODE_Dong_1: return 4;
//         case NODE_Dong_2: return 8;
//         case NODE_Dong_3: return 12;
//         case NODE_Dong_4: return 16;
//
//         default: return 0;
//     }
// }
//

// TODO: cache value. See other commented out code
uint32_t Network::GetNodeCount()
{
    return mesh.getNodeList().size();
}

//
// uint32_t Network::GetStability()
// {
//     return m_mesh.stability;
// }
//
// void Network::Broadcast(const String& message)
// {
//     m_mesh.sendBroadcast(const_cast<String&>(message));
// }
//
// void Network::SetReceived(ReceivedCallbackT callback)
// {
// 		INFO(" [NET] Received Connection\n");
//     m_mesh.onReceive(callback);
// }
//
// void Network::ReceivedCallback(uint32_t from, String& msg)
// {
//     INFO(" [NET] <%x>:  Received msg=%s\n", m_mesh.getNodeId(), msg.c_str());
// }
//
// void Network::NewConnectionCallback(uint32_t nodeId)
// {
//     INFO(" [NET] New Connection, id=%x\n", nodeId);
// }
//
// // void Network::printNodeList() {
// //
// // }
//
// void Network::ChangedConnectionCallback()
// {
//     INFO(" [NET] Changed connections %s\n", m_mesh.subConnectionJson().c_str());
//
//     m_nodes = m_mesh.getNodeList();
//     INFO(" [NET] Connection list (num=%d):", m_nodes.size());
//     for (auto node : m_nodes)
//         INFO(" %x ", node);
//     INFO("\n");
//
//     m_calcDelay = true;
// }
//
// void Network::NodeTimeAdjustedCallback(int32_t offset)
// {
//     INFO(" [NET] Adjusted time %u. Offset = %d\n", m_mesh.getNodeTime(), offset);
// }
//
// void Network::DelayReceivedCallback(uint32_t from, int32_t delay)
// {
//     INFO(" [NET] Delay to node %x is %d us\n", from, delay);
// }

// } // namespace
