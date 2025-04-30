// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundQuizProp.h"

#include "EdGraphSchema_K2_Actions.h"
#include "SoundMommyQuizProp.h"
#include "Components/BoxComponent.h"
#include "Misc/DateTime.h"
#include "JumpGame/AIServices/Shared/IOManagerComponent.h"
#include "JumpGame/AIServices/Shared/Message.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Props/LogicProp/RisingWaterProp.h"
#include "Runtime/Core/Public/Containers/StringConv.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ASoundQuizProp::ASoundQuizProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VoiceRecorderComponent = CreateDefaultSubobject<UVoiceRecorderComponent>(TEXT("VoiceRecorderComponent"));
	if (!VoiceRecorderComponent)
	{
		FFastLogger::LogConsole(TEXT("보이스 레코더 없습니다"));
	}
}

// Called when the game starts or when spawned
void ASoundQuizProp::BeginPlay()
{
	Super::BeginPlay();

	// TODO: 나중에는 오버랩으로 옮김 (더미용)
	NetGS = Cast<ANetworkGameState>(GetWorld()->GetGameState());
	RisingWaterProp = Cast<ARisingWaterProp>(UGameplayStatics::GetActorOfClass(GetWorld(), ARisingWaterProp::StaticClass()));

	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));
}

// Called every frame
void ASoundQuizProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 테스트용 키바인딩
	/*if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Six))
	{
		// 6번 누르면 녹음시작
		StartRecord();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Five))
	{
		// 5번 누르면 녹음끝
		StopRecord();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Seven))
	{
		// 7번 누르면 퀴즈 메세지 받아오기
		ReceiveSoundQuizMessage();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Eight))
	{
		// 8번 누르면 퀴즈 끝 메세지 전송
		SendEndSoundQuizNotify();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Nine))
	{
		// 9번 누르면 퀴즈 음성 파일 메세지 전송
		SendSoundQuizMessage();
	}*/

	if (bIsMessageReceived)
	{
		// 메세지가 왔는지 확인하자
		ReceiveSoundQuizMessage();
	}
}

void ASoundQuizProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	Super::OnMyBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,
	                        SweepResult);

	if (!OtherActor->ActorHasTag("Frog")) return;
		
	// GameState 캐스팅
	NetGS = Cast<ANetworkGameState>(GetWorld()->GetGameState());
	
	// 이때 퀴즈 시작!
	SendStartSoundQuizNotify();
	
	// 캐릭터 퀴즈 카메라로 전환
	if (OtherActor->ActorHasTag(TEXT("Frog")))
	{
		Frog = Cast<AFrog>(OtherActor);
		if (Frog)
		{
			Frog->StopMovementAndResetRotation();
			Frog->CameraMissionMode();
			Frog->SetCrouchEnabled(false);
			Frog->SetJumpGaugeVisibility(false);
		}
	}
	
	// 물 멈추자
	if (RisingWaterProp)
	{
		RisingWaterProp->StopRising();
	}
}

void ASoundQuizProp::OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnMyEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	
}

void ASoundQuizProp::SendStartSoundQuizNotify()
{
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

	// 틱 활성화
	bIsMessageReceived = true;
}

void ASoundQuizProp::SendSoundQuizMessage()
{
	FFastLogger::LogScreen(FColor::Red,TEXT("음성파일 보냅니다"));
	
	FDateTime Now = FDateTime::Now();
	int32 Hour = Now.GetHour();
	int32 Minute = Now.GetMinute();
	int32 Second = Now.GetSecond();
	UE_LOG(LogTemp, Warning, TEXT("SendSoundQuizMessage 현재 시간: %02d:%02d:%02d"), Hour, Minute, Second);

	// 더미 데이터 만들어서 보내기 (음성 녹음 파일)
	// 1. 바이너리 데이터를 로딩하자 (처음 한번만!)
	if (CurrentSendIndex == 0)
	{
		FString FilePath = TEXT("C:/FinalProject/Game/Saved/SoundQuizResponseFile.wav");
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
		// FFastLogger::LogConsole(TEXT("TotalSize: %d"), TotalWavSize);

		// 2. 쪼개서 여러 패킷으로 전송 (1024바이트 단위로)
		// 현재 인덱스 기준으로 청크를 생성
		// WAV 전체에서 1024만큼 데이터를 꺼내고, 마지막 청크는 1024보다 작을 수 있으니까 Min()으로 크기 조절
		int32 CopySize = FMath::Min(ChunkSize, TotalWavSize - (CurrentSendIndex * ChunkSize));
		FWavRequestMessage ReqMessage;
		
		// 첫번째 패킷이면 start=1, 마지막 패킷이면 fin=1
		// 0으로 전체 초기화(패딩값넣기위해)
		FMemory::Memzero(&ReqMessage, sizeof(ReqMessage)); 
		ReqMessage.Header.Type = EMessageType::WaveRequest;
		ReqMessage.QuizID = 1;
		ReqMessage.Start = (CurrentSendIndex == 0) ? 1 : 0;
		ReqMessage.Index = CurrentSendIndex;
		ReqMessage.Fin = ((CurrentSendIndex + 1) * ChunkSize >= TotalWavSize) ? 1 : 0;
		ReqMessage.Size = TotalWavSize;
		// ReqMessage.ChunkSize = CopySize;
		
		// ChunkSize는 항상 1024를 넣는다
		ReqMessage.ChunkSize = ChunkSize;

		// RawData에 현재 인덱스 위치부터 CopySize만큼 WAV 데이터를 복사해 넣기
		FMemory::Memcpy(ReqMessage.RawData, CachedBinaryWav.GetData() + (CurrentSendIndex * ChunkSize), CopySize);

		// 패킷 포장
		FMessageUnion Msg;
		FMemory::Memcpy(&Msg, &ReqMessage, sizeof(ReqMessage));
		Msg.Header = ReqMessage.Header;
		Msg.Header.PayloadSize = sizeof(ReqMessage);

		// 메세지 전송
		NetGS->IOManagerComponent->SendGameMessage(Msg);

		// 다음 인덱스 준비
		CurrentSendIndex++;
		
		// Fin이면 타이머 종료 및 초기화
		if (ReqMessage.Fin)
		{
			FFastLogger::LogScreen(FColor::Red,TEXT("음성파일 전송완료"));
			UE_LOG(LogTemp, Log, TEXT("모든 WAV 패킷 전송 완료"));
			CurrentSendIndex = 0;
			TotalWavSize = 0;
			CachedBinaryWav.Empty();

			// 타이머 전부 초기화
			ClearAllTimer();
			return;
		}
	}
}

void ASoundQuizProp::ReceiveSoundQuizMessage()
{
	FMessageUnion RespMessage;
	// FFastLogger::LogConsole(TEXT("메세지 받을거야!"));
	// AI가 보내준 메세지를 받자
	if (!NetGS->IOManagerComponent->PopMessage(EMessageType::WaveResponse, RespMessage))
	{
		return;
	}

	FFastLogger::LogConsole(TEXT("메세지 받았습니다@@@@@@@@"));
	FDateTime Now = FDateTime::Now();
	int32 Hour = Now.GetHour();
	int32 Minute = Now.GetMinute();
	int32 Second = Now.GetSecond();
	UE_LOG(LogTemp, Warning, TEXT("ReceiveSoundQuizMessage 현재 시간: %02d:%02d:%02d"), Hour, Minute, Second);

	// 내가 만든 변수에 넣자
	QuizID = RespMessage.WavResponseMessage.QuizID;
	Similarity = RespMessage.WavResponseMessage.Similarity;
	// 정답 여부 판단
	bSuccess = RespMessage.WavResponseMessage.bSuccess;
	// 먼저, 보내준 메세지에서 문자열의 길이를 추출한다 (몇글자니)
	FMemory::Memcpy(&MessageSize, RespMessage.WavResponseMessage.Message, sizeof(uint32));

	FFastLogger::LogConsole(TEXT("Payload Size: %d"), RespMessage.Header.PayloadSize);
	FFastLogger::LogConsole(TEXT("Received Message Size : %d"), MessageSize);

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
	UE_LOG(LogTemp, Warning, TEXT("Receive BSuccess: %d"), bSuccess);
	UE_LOG(LogTemp, Warning, TEXT("Receive MessageSize: %d"), MessageSize);
	UE_LOG(LogTemp, Warning, TEXT("Receive MessageStr: %s"), *MessageStr);

	// 총 몇번 응답 했는지를 체크한다
	SendResponseIdx++;

	// 틱 비활성화
	bIsMessageReceived = false;
	
	// 메세지를 받았다! 2초 후에 녹음 시작
	GetWorld()->GetTimerManager().SetTimer(RecordStartTimer, this, &ASoundQuizProp::StartRecord, 2.0f, false);
}

void ASoundQuizProp::SendEndSoundQuizNotify()
{
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

	// 캐릭터 카메라 다시 원래대로
	Frog = Cast<AFrog>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Frog)
	{
		Frog->ResumeMovement();
		Frog->CameraMovementMode();
		Frog->SetCrouchEnabled(true);
		Frog->SetJumpGaugeVisibility(true);
	}
	
	// 물 다시 차오르기
	RisingWaterProp->StartRising();

	// 퀴즈 정보 초기화
	ResetSoundQuiz();

	// 틱 비활성화
	bIsMessageReceived = false;
}

void ASoundQuizProp::ResetSoundQuiz()
{
	QuizID = -1;
	Similarity = 0.f;
	MessageSize = 0;
	MessageStr = "";

	SendResponseIdx = 0;
	
	CachedBinaryWav = { 0 };
	CurrentSendIndex = 0;
	TotalWavSize = 0;
}

void ASoundQuizProp::StartRecord()
{
	// 녹음 시작
	if (VoiceRecorderComponent)
	{
		VoiceRecorderComponent->StartRecording();
		FFastLogger::LogConsole(TEXT("녹음시작@@@@@@@@@@@@@@@@@@@@@@@"));
	}
	
	// 녹음 시작 3초뒤에 녹음 자동 종료
	GetWorld()->GetTimerManager().SetTimer(RecordStopTimer, this, &ASoundQuizProp::StopRecord, 3.0f, false);
}

void ASoundQuizProp::StopRecord()
{
	if (VoiceRecorderComponent)
	{
		VoiceRecorderComponent->StopRecording();
		FFastLogger::LogConsole(TEXT("녹음끝임@@@@@@@@@@@@@@@@@@@@@@@"));
	}

	// NOTE: 바로 보내면 안됨, 1초후에 전송
	// 녹음을 끝냈으니, 파일을 전송해주자 (1초후에)
	GetWorld()->GetTimerManager().SetTimer(SendFileMessage, this, &ASoundQuizProp::SendSoundQuizMessage, 1.0f, false);
	
	// 틱도 다시 활성화
	bIsMessageReceived = true;
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

void ASoundQuizProp::ClearAllTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(RecordStartTimer);
	GetWorld()->GetTimerManager().ClearTimer(RecordStopTimer);
	GetWorld()->GetTimerManager().ClearTimer(SendFileMessage);
}
