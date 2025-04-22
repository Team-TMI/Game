// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundQuizProp.h"

#include "HttpFwd.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Interfaces/IHttpResponse.h"
#include "JumpGame/AIServices/Shared/IOManagerComponent.h"
#include "JumpGame/AIServices/Shared/Message.h"
#include "JumpGame/UI/Obstacle/SoundQuizUI.h"


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

	NetGS = Cast<ANetworkGameState>(GetWorld()->GetGameState());

	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));
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
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Seven))
	{
		// 7번 누르면 퀴즈 끝 메세지 전송
		ReceiveSoundQuizMessage();
	}
}

void ASoundQuizProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	Super::OnMyBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                        SweepResult);

	// GameState 캐스팅
	// NetGS = Cast<ANetworkGameState>(GetWorld()->GetGameState());
	// 이때 퀴즈 시작!
	SendStartSoundQuizNotify();

	// 시작하면 UI 띄우자
	SoundQuizUI = CreateWidget<USoundQuizUI>(GetWorld(), SoundQuizUIClass);
	if (SoundQuizUI)
	{
		SoundQuizUI->AddToViewport();
	}
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
	// NetGS->IOManagerComponent->SendGameMessage(Msg);
}

void ASoundQuizProp::SendSoundQuizMessage()
{
	// 더미 데이터 만들어서 보내기 (음성 녹음 파일)
	// 1. 바이너리 데이터를 로딩하자 (처음 한번만!)
	if (CurrentSendIndex == 0)
	{
		FString FilePath = TEXT("C:/WavTest/btn_slide.wav");
		LoadWavFileBinary(FilePath, CachedBinaryWav);
		// 없으면 로그 출력
		if (CachedBinaryWav.Num() == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("WAV 로드 실패"));
			return;
		}
		// 총 WAV 데이터 크기를 저장해두고, 이후 인덱싱에 사용
		TotalWavSize = CachedBinaryWav.Num();
	}
	
	while (true)
	{
		FFastLogger::LogConsole(TEXT("TotalSize: %d"), TotalWavSize);

		// 2. 쪼개서 여러 패킷으로 전송 (1024바이트 단위로)
		// 현재 인덱스 기준으로 청크를 생성
		// WAV 전체에서 1024만큼 데이터를 꺼내고, 마지막 청크는 1024보다 작을 수 있으니까 Min()으로 크기 조절
		int32 CopySize = FMath::Min(ChunkSize, TotalWavSize - (CurrentSendIndex * ChunkSize));
		FWavRequestMessage ReqMessage;

		// 첫번째 패킷이면 start=1, 마지막 패킷이면 fin=1
		FMemory::Memzero(&ReqMessage, sizeof(ReqMessage));
		ReqMessage.Header.Type = EMessageType::WaveRequest;
		ReqMessage.QuizID = 1;
		ReqMessage.Start = (CurrentSendIndex == 0) ? 1 : 0;
		ReqMessage.Index = CurrentSendIndex;
		ReqMessage.Fin = ((CurrentSendIndex + 1) * ChunkSize >= TotalWavSize) ? 1 : 0;
		ReqMessage.Size = TotalWavSize;
		ReqMessage.ChunkSize = CopySize;

		// RawData에 현재 인덱스 위치부터 CopySize만큼 WAV 데이터를 복사해 넣기
		FMemory::Memcpy(ReqMessage.RawData, CachedBinaryWav.GetData() + (CurrentSendIndex * ChunkSize), CopySize);

		// 패킷 포장
		FMessageUnion Msg;
		FMemory::Memcpy(&Msg, &ReqMessage, sizeof(ReqMessage));
		Msg.Header = ReqMessage.Header;
		Msg.Header.PayloadSize = sizeof(ReqMessage);

		// 메세지 전송
		// NetGS->IOManagerComponent->SendGameMessage(Msg);

		// 다음 인덱스 준비
		CurrentSendIndex++;
		
		// Fin이면 타이머 종료 및 초기화
		if (ReqMessage.Fin)
		{
			UE_LOG(LogTemp, Log, TEXT("모든 WAV 패킷 전송 완료"));
			CurrentSendIndex = 0;
			TotalWavSize = 0;
			CachedBinaryWav.Empty();
			return;
		}
	}
}

void ASoundQuizProp::ReceiveSoundQuizMessage()
{
	FMessageUnion RespMessage;
	// AI가 보내준 메세지를 받자
	if (!NetGS->IOManagerComponent->PopMessage(EMessageType::WaveResponse, RespMessage))
	{
		return;
	}

	// 내가 만든 변수에 넣자
	QuizID = RespMessage.WavResponseMessage.QuizID;
	Similarity = RespMessage.WavResponseMessage.Similarity;
	// 먼저, 보내준 메세지에서 문자열의 길이를 추출한다 (몇글자니)
	FMemory::Memcpy(&MessageSize, RespMessage.WavResponseMessage.Message, sizeof(uint32));
	// 그 다음, 문자열 길이가 0보다 크면 길이만큼 문자열을 복사해서 생성
	if (MessageSize > 0)
	{
		unsigned char* StrSize = RespMessage.WavResponseMessage.Message + sizeof(uint32);
		FUTF8ToTCHAR Converter(reinterpret_cast<const ANSICHAR*>(StrSize), MessageSize);
		MessageStr = FString(Converter.Length(), Converter.Get());
	}
	
	// 받은 메세지를 출력한다
	UE_LOG(LogTemp, Warning, TEXT("Receive QuizID: %d"), QuizID);
	UE_LOG(LogTemp, Warning, TEXT("Receive Similarity: %f"), Similarity);
	UE_LOG(LogTemp, Warning, TEXT("Receive MessageSize: %d"), MessageSize);
	UE_LOG(LogTemp, Warning, TEXT("Receive MessageStr: %s"), *MessageStr);

	// UI업데이트
	SoundQuizUI->UpdateFromResponse(Similarity, MessageStr);
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

// WAV 파일을 로드하고 바이너리 데이터로 전환
void ASoundQuizProp::LoadWavFileBinary(const FString& FilePath, TArray<uint8>& BinaryData)
{
    // WAV 파일을 로드
    if (!FFileHelper::LoadFileToArray(BinaryData, *FilePath))
    {
       // 로드 실패 시
       UE_LOG(LogTemp, Error, TEXT("Failed to load WAV file from path: %s"), *FilePath);
       return;
    }
    // 로드 성공 시
    UE_LOG(LogTemp, Log, TEXT("Successfully loaded WAV file: %s"), *FilePath);
}
