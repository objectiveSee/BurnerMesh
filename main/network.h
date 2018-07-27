/**
 * Networking Code
 */

#pragma once

#include <Arduino.h>

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
  byte GetMessage();
  byte GetMessageSender();

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
  byte _message;
  byte _message_from;
};

// const char * nameForNetworkId(uint32_t);

// } // namespace
