#pragma once

#pragma pack(push, 1)

enum class EMessageType : uint8
{
	Ping = 1,
	Pong = 2,
	QuizNotify = 3,
	WaveRequest = 4,
	WaveResponse = 5,
	EyeTrackingNotify = 6,
	EyeTrackingRequest = 7,
	EyeTrackingResponse = 8,
};

struct FMessageHeader
{
	EMessageType Type;
	uint16 PayloadSize;
	uint8 SessionID[100];
	uint8 PlayerID;
};

struct FPingMessage
{
	FMessageHeader Header;
	double TimeStamp;
};

struct FPongMessage
{
	FMessageHeader Header;
	double TimeStamp;
};

#pragma pack(pop)

union FMessageUnion
{
	FMessageHeader Header;
	FPingMessage PingMessage;
	FPongMessage PongMessage;
	uint8 RawData[128];
};
