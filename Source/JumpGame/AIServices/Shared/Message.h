#pragma once

// #pragma pack(push, 1)

enum class EMessageType : uint8
{
	None = 0,
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

// Ping
struct FPingMessage
{
	FMessageHeader Header;
	double TimeStamp;
};

// Pong
struct FPongMessage
{
	FMessageHeader Header;
	double TimeStamp;
};

// QuizNotify
struct FQuizNotifyMessage
{
	FMessageHeader Header; // QuizNotify
		
	uint8 QuizID; // 퀴즈의 일련번호
	uint8 Start; // 0~1
	uint8 End; // 0~1
};

// WaveRequest
struct FWavRequestMessage {
	FMessageHeader Header; // WaveRequest
		 
	uint8 QuizID;
	uint8 Start; // 0 ~ 1
	uint8 Index; // 패킷의 순서
	uint8 Fin; // 0 ~ 1
	uint8 Size; // 총 음성 사이즈
	uint8 ChunkSize; // 청크 사이즈
	uint8 RawData[1024]; // 음성 바이너리 데이터
};

// WaveResponse
struct FWavResponseMessage {
	FMessageHeader Header; // WaveResponse
		
	uint8 QuizID;
	float Similarity; // 유사도
	uint8 MessageSize; // [len(str), str]
		
	// 가변 길이 만큼 문자열을 뒤에 붙여서 보냄!
	// STRING!!;
};

// EyeTrackingNotify
struct FEyeTrackingNotifyMessage {
	FMessageHeader Header; // EyetrackingNotify
		
	uint8 QuizID;
	uint8 Start; // 0~1
	uint8 End; // 0~1
};

// EyeTrackingRequest
struct FEyeTrackingRequest {
	FMessageHeader Header; // EyetrackingRequest
		
	uint8 QuizID;
	uint8 Start;
	uint8 End;
};

// EyeTrackingResponse
struct FEyeTrackingResponse {
	FMessageHeader Header;

	uint8 QuizID;		
	float Width; // 해상도 너비
	float Height; // 해상도 높이
	float X; // 좌표값 x
	float Y; // 좌표값 y
	uint8 bBlink; // 0~1(감았다: 1) 
	uint8 State; // 100 : 정상성, 200 : 클라 에러, 300 : 서버 에러
};

// #pragma pack(pop)

union FMessageUnion
{
	FMessageHeader Header;
	FPingMessage PingMessage;
	FPongMessage PongMessage;
	FQuizNotifyMessage QuizNotifyMessage;
	FWavRequestMessage WavRequestMessage;
	FWavResponseMessage WavResponseMessage;
	FEyeTrackingNotifyMessage EyeTrackingNotifyMessage;
	FEyeTrackingRequest EyeTrackingRequestMessage;
	FEyeTrackingResponse EyeTrackingResponseMessage;
	uint8 RawData[1460];
};
