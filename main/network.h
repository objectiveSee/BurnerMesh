/**
 * Networking Code
 */

#pragma once

#include <Arduino.h>

// Payload size used in nRF24L01 communication
#define PAYLOAD_SIZE 6

// Paylaod - 2 (checksum, header removed)
#define MESSAGE_SIZE 4

class Network final
{
public:
	Network();
	~Network();

	void Setup();
	void Update();
  void SendMessage(byte message);
  void HandleMessage(byte* message);
  bool MessageReceived();
  byte * GetMessage();

	// uint32_t GetTime();
	//  uint32_t GetNodeID();
	// int32_t  GetNodeOffset();
	// uint32_t GetNodeCount();
  // uint32_t GetStability();
	//
	// using ReceivedCallbackT = std::function<void(uint32_t from, const String& msg)>;
	// void Broadcast(const String& msg);
	// void SetReceived(ReceivedCallbackT callback);

private:
	// // Callbacks for mesh control
	// void ReceivedCallback(uint32_t from, String& msg);
	// void NewConnectionCallback(uint32_t nodeId);
	// void ChangedConnectionCallback();
	// void NodeTimeAdjustedCallback(int32_t offset);
	// void DelayReceivedCallback(uint32_t from, int32_t delay);

  bool _message_received;
  byte _message[MESSAGE_SIZE];
};

// const char * nameForNetworkId(uint32_t);

// } // namespace
