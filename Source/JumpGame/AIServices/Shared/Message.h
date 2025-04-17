#pragma once

#pragma pack(push, 1)

enum class EMessageType : uint8
{
	Ping = 1,
	Pong = 2,
};

struct FMessageHeader
{
	EMessageType Type;
	uint16 PayloadSize;
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
