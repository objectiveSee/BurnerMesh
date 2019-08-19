/**
 * Networking Code
 */

#pragma once

#include <Arduino.h>

// Payload size used in nRF24L01 communication
#define PAYLOAD_SIZE 6

// Paylaod - 2 (checksum, header removed)
#define MESSAGE_SIZE 4

/*
 * Commands for wireless comm
 */
#define COMMAND_MODE_CHANGE 0x0D
#define COMMAND_HEARTBEAT 0x0A


class Network final
{
public:
	Network();
	~Network();

	void Setup();
	void Update();
//	void SendMessage(byte* message);
  void SendMessageFullPayload(byte* payload);
	void SendModeChange(byte modeAsByte);
	void HandleMessage(byte* message);
	bool MessageReceived();
	byte * GetMessage();

private:
	bool _message_received;
	byte _message[MESSAGE_SIZE];
};
