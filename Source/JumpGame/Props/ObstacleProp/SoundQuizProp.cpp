// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundQuizProp.h"

#include "Commandlets/AssetRegistryGenerator.h"
#include "JumpGame/AIServices/Shared/IOManagerComponent.h"
#include "JumpGame/AIServices/Shared/Message.h"


// Sets default values
ASoundQuizProp::ASoundQuizProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASoundQuizProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASoundQuizProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Nine))
	{
		// 9번 누르면 퀴즈 음성 파일 메세지 전송
		SendSoundQuizMessage();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Eight))
	{
		// 8번 누르면 퀴즈 끝 메세지 전송
		SendEndSoundQuizNotify();
	}
}

void ASoundQuizProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	Super::OnMyBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                        SweepResult);

	// GameState 캐스팅
	NetGS = Cast<ANetworkGameState>(GetWorld()->GetGameState());
	// 이때 퀴즈 시작!
	SendStartSoundQuizNotify();
}


void ASoundQuizProp::SendStartSoundQuizNotify()
{
	// 더미데이터 만들어서 보내기

	// AI쪽에서 첫번째 문장을 준다면, 5초동안 녹음 후 전송
	// 퀴즈 시작 메시지를 보내주자
	FQuizNotifyMessage NotifyMessage;
	NotifyMessage.Header.Type = EMessageType::QuizNotify;
	NotifyMessage.QuizID = 1;
	NotifyMessage.Start = 1; //시작
	NotifyMessage.End = 0;

	// 패킷 포장?
	// 전송할 데이터를 네트워크 전송에 적합한 형식으로 정리하는 것
	// 즉, 전송 가능한 구조로 만들자
	FMessageUnion Msg;
	FMemory::Memcpy(&Msg, &NotifyMessage, sizeof(NotifyMessage));
	Msg.Header = NotifyMessage.Header;
	Msg.Header.PayloadSize = sizeof(NotifyMessage);

	// 메세지 전송
	NetGS->IOManagerComponent->SendGameMessage(Msg);
}

void ASoundQuizProp::SendSoundQuizMessage()
{
	// 더미 데이터 만들어서 보내기
	// 음성 녹음 파일을 보내주자
	FWavRequestMessage ReqMessage;
	ReqMessage.Header.Type = EMessageType::WaveRequest;
	ReqMessage.QuizID = 1;
	ReqMessage.Start = 1;
	ReqMessage.Index = 0;
	ReqMessage.Fin = 1;
	ReqMessage.Size = 100;
	ReqMessage.ChunkSize = 100;
	FMemory::Memset(ReqMessage.RawData, 0xAB, 100);
	/* GPT답변
	실제 RawData를 음성 파일에서 채운다면:
	FMemory::Memcpy(ReqMessage.RawData, SoundBuffer, SoundSize);
	테스트용으로 쓸 때는:
	FMemory::Memset(ReqMessage.RawData, 0xAB, 100); // 그냥 가짜 데이터 넣기
	*/
	
	// 패킷 포장
	FMessageUnion Msg;
	FMemory::Memcpy(&Msg, &ReqMessage, sizeof(ReqMessage));
	Msg.Header = ReqMessage.Header;
	Msg.Header.PayloadSize = sizeof(ReqMessage);

	// 메세지 전송
	NetGS->IOManagerComponent->SendGameMessage(Msg);
}

void ASoundQuizProp::ReceiveSoundQuizMessage()
{
	FMessageUnion RespMessage;
	
	NetGS->IOManagerComponent->PopMessage(EMessageType::QuizNotify, RespMessage);
	// AI가 보내준 메세지를 받자
	QuizID = RespMessage.WavResponseMessage.QuizID;
	Similarity = RespMessage.WavResponseMessage.Similarity;
	MessageSize = RespMessage.WavResponseMessage.MessageSize;

	// 받은 메세지를 출력한다
	UE_LOG(LogTemp, Warning, TEXT("Receive QuizID: %d"), QuizID);
	UE_LOG(LogTemp, Warning, TEXT("Receive Similarity: %f"), Similarity);
	UE_LOG(LogTemp, Warning, TEXT("Receive MessageSize: %d"), MessageSize);
}

void ASoundQuizProp::SendEndSoundQuizNotify()
{
	// 더미데이터 만들어서 보내기

	// 정답을 맞췄거나, 횟수 20번이 끝났을때
	// 퀴즈 끝 메시지를 보내주자
	FQuizNotifyMessage EndMessage;
	EndMessage.Header.Type = EMessageType::QuizNotify;
	EndMessage.QuizID = 1;
	EndMessage.Start = 0;
	EndMessage.End = 1; //끝

	// 패킷 포장
	FMessageUnion Msg;
	FMemory::Memcpy(&Msg, &EndMessage, sizeof(EndMessage));
	Msg.Header = EndMessage.Header;
	Msg.Header.PayloadSize = sizeof(EndMessage);

	// 메세지 전송
	NetGS->IOManagerComponent->SendGameMessage(Msg);
}

