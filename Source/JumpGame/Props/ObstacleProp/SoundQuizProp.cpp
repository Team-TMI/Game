// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundQuizProp.h"


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
	
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Zero))
	{
		// 0번 누르면 퀴즈 시작 메세지 전송
		SendStartQuizNotify();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Nine))
	{
		// 9번 누르면 퀴즈 음성 파일 메세지 전송
		SendDummyMessage();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Eight))
	{
		// 8번 누르면 퀴즈 끝 메세지 전송
		SendEndQuizNotify();
	}
}

void ASoundQuizProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	Super::OnMyBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                        SweepResult);

	// 원래는 이때 퀴즈 시작!
}


void ASoundQuizProp::SendStartQuizNotify()
{
	/*// 더미데이터 만들어서 보내기
	// 퀴즈 시작 메시지를 보내준다
	FQuizNotifyMessage NotifyMessage;
	NotifyMessage.Header.Type = EMessageType::QuizNotify;
	NotifyMessage.QuizID = 1;
	NotifyMessage.Start = 1; //시작
	NotifyMessage.End = 0;

	// 패킷 포장???? 이거 내가 하는건가
	// 전송할 데이터를 네트워크 전송에 적합한 형식으로 정리 (전송 가능한 구조로 만들자)
	FMessageUnion Msg;
	FMemory::Memcpy(Msg.RawData, &NotifyMessage, sizeof(NotifyMessage));
	Msg.Header = NotifyMessage.Header;
	Msg.Header.PayloadSize = sizeof(NotifyMessage);

	// 메세지 전송
	SocketHandler->SendGameMessage(Msg);*/
}

void ASoundQuizProp::SendDummyMessage()
{
	/*// 더미 데이터 만들어서 보내기
	// 음성 녹음 파일을 보내준다
	FWavRequestMessage ReqMessage;
	ReqMessage.Header.Type = EMessageType::WaveRequest;
	ReqMessage.QuizID = 1;
	ReqMessage.Start = 1;
	ReqMessage.index = 0;
	ReqMessage.Fin = 1;
	ReqMessage.Size = 100;
	ReqMessage.ChunkSize = 100;
	FMemory::Memset(ReqMessage.RawData, 0xAB, 100);
	/* GPT답변
	실제 RawData를 음성 파일에서 채운다면:
	FMemory::Memcpy(ReqMessage.RawData, SoundBuffer, SoundSize);
	테스트용으로 쓸 때는:
	FMemory::Memset(ReqMessage.RawData, 0xAB, 100); // 그냥 가짜 데이터 넣기
	#1#
	
	// 패킷 포장
	FMessageUnion Msg;
	FMemory::Memcpy(Msg.RawData, &ReqMessage, sizeof(ReqMessage));
	Msg.Header = Req.Header;
	Msg.Header.PayloadSize = sizeof(ReqMessage);

	// 메세지 전송
	SocketHandler->SendGameMessage(Msg);*/
}

void ASoundQuizProp::ReceiveDummyMessage()
{
	/*// 메세지 받기????
	QuizID = FWavResponseMessage.QuizID;
	Similarity = FWavResponseMessage.Similarity;
	MessageSize = FWavResponseMessage.MessageSize;

	// 받은 메세지를 출력한다
	UE_LOG(LogTemp, Warning, TEXT("Receive QuizID: %d"), QuizID);
	UE_LOG(LogTemp, Warning, TEXT("Receive Similarity: %f"), Similarity);
	UE_LOG(LogTemp, Warning, TEXT("Receive MessageSize: %d"), MessageSize);*/
}

void ASoundQuizProp::SendEndQuizNotify()
{
	/*// 더미데이터 만들어서 보내기
	// 퀴즈 끝 메시지를 보내준다
	FSoundQuizNotifyMessage EndMessage;
	EndMessage.Header.Type = EMessageType::QuizNotify;
	NotifyMessage.QuizID = 1;
	NotifyMessage.Start = 0;
	NotifyMessage.End = 1; //끝

	// 패킷 포장
	FMessageUnion Msg;
	FMemory::Memcpy(Msg.RawData, &EndMessage, sizeof(EndMessage));
	Msg.Header = EndMessage.Header;
	Msg.Header.PayloadSize = sizeof(EndMessage);

	// 메세지 전송
	SocketHandler->SendGameMessage(Msg);*/
}

