// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Online/OnlineSessionNames.h"
#include "HAL/PlatformProcess.h"

void UJumpGameInstance::Init()
{
	Super::Init();

	// 현재 사용하는 서브시스템을 가져오자
	IOnlineSubsystem* Subsys = IOnlineSubsystem::Get();
	if (Subsys)
	{
		SessionInterface = Subsys->GetSessionInterface();

		// 세션 생성 성공시 호출되는 함수 등록
		SessionInterface->OnCreateSessionCompleteDelegates.
		                  AddUObject(this, &UJumpGameInstance::OnCreateSessionComplete);
		// 세션 검색 성공시 호출되는 함수 등록
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UJumpGameInstance::OnFindSessionComplete);
		// 세션 참여 성공시 호출되는 함수 등록
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UJumpGameInstance::OnJoinSessionComplete);
	}

	RunEyeTrackingScript();
}

void UJumpGameInstance::CreateMySession(FString DisplayName, int32 PlayerCount)
{
	// 세션을 만들기 위한 옵션을 설정
	FOnlineSessionSettings SessionSettings;
	// Lan 사용 여부
	FName subsystemName = IOnlineSubsystem::Get()->GetSubsystemName();
	UE_LOG(LogTemp, Warning, TEXT("서브시스템 이름: %s"), *subsystemName.ToString())
	SessionSettings.bIsLANMatch = subsystemName.IsEqual(FName(TEXT("NULL")));

	// 로비 사용할지 여부
	SessionSettings.bUseLobbiesIfAvailable = true;
	// 친구 상태를 확인할 수 있는지 (게임중/로그아웃 등등 공개할건지) 여부
	SessionSettings.bUsesPresence = true;
	// 세션 검색을 허용할 지 여부
	SessionSettings.bShouldAdvertise = true;
	// 세션 최대 인원 설정
	SessionSettings.NumPublicConnections = PlayerCount;
	// 커스텀 정보 (세션이름)
	// base64로 인코딩
	DisplayName = StringBase64Encode(DisplayName);
	FName EncodedName = FName(DisplayName);
	SessionSettings.Set(FName(TEXT("DP_NAME")), DisplayName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 세션 이름 저장
	CurrentSessionName = EncodedName;

	// 세션 생성
	SessionInterface->CreateSession(0, FName(DisplayName), SessionSettings);
}

void UJumpGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] 세션 생성 성공"), *SessionName.ToString());

		// 대기방으로 이동
		GetWorld()->ServerTravel(TEXT("/Game/Maps/WaitRoomLevel?listen"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] 세션 생성 실패"), *SessionName.ToString());
	}
}

void UJumpGameInstance::FindOtherSession()
{
	// sessionSearch 만들자
	SessionSearch = MakeShared<FOnlineSessionSearch>();

	// Lan 사용 여부
	FName subsystemName = IOnlineSubsystem::Get()->GetSubsystemName();
	UE_LOG(LogTemp, Warning, TEXT("서브시스템 이름: %s"), *subsystemName.ToString())
	SessionSearch->bIsLanQuery = subsystemName.IsEqual(FName(TEXT("NULL")));

	// 어떤 옵션을 기준으로 검색할건지
	SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
	// 검색 갯수
	SessionSearch->MaxSearchResults = 100;

	// 위 설정들을 가지고 세션 검색해주세요
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UJumpGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("세션 검색 성공!"));
		// 검색된 세션 결과들
		auto results = SessionSearch->SearchResults;
		for (int32 i = 0; i < results.Num(); i++)
		{
			FString displayName;
			results[i].Session.SessionSettings.Get(FName(TEXT("DP_NAME")), displayName);

			// base64 Decode
			displayName = StringBase64Decode(displayName);

			UE_LOG(LogTemp, Warning, TEXT("세션 - %d, 이름: %s"), i, *displayName);

			OnFindComplete.ExecuteIfBound(i, displayName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("세션 검색 실패"));
	}

	// 검색 끝났다는 걸 알리자
	OnFindComplete.ExecuteIfBound(-1, FString());
}

void UJumpGameInstance::JoinOtherSession(int32 SessionIdx)
{
	// 검색된 세션 결과들
	auto results = SessionSearch->SearchResults;
	if (results.Num() == 0)
		return;

	// 세션 이름을 가져오자 (일단 0번째)
	FString displayName;
	// 5.5 이슈 해결 (이 값이 자동으로 false되니까 다시 변환해주기)
	results[SessionIdx].Session.SessionSettings.bUseLobbiesIfAvailable = true;
	results[SessionIdx].Session.SessionSettings.bUsesPresence = true;

	results[SessionIdx].Session.SessionSettings.Get(FName(TEXT("DP_NAME")), displayName);

	// 세션참여
	SessionInterface->JoinSession(0, FName(displayName), results[SessionIdx]);
}

void UJumpGameInstance::OnJoinSessionComplete(FName SessionName,
                                              EOnJoinSessionCompleteResult::Type Result)
{
	// 만약에 참여 성공했다면
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		// 서버가 만들어 놓은 세션 url얻어오자
		FString url;
		SessionInterface->GetResolvedConnectString(SessionName, url);
		// 서버가 있는 맵으로 이동하자
		APlayerController* pc = GetWorld()->GetFirstPlayerController();
		pc->ClientTravel(url, TRAVEL_Absolute);
	}
}

void UJumpGameInstance::LeaveSession(bool bDestroySession)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	IOnlineSubsystem* Subsys = IOnlineSubsystem::Get();
	if (Subsys)
	{
		SessionInterface = Subsys->GetSessionInterface();
		if (SessionInterface.IsValid() && bDestroySession)
		{
			// 생성된 세션의 이름을 가져오자
			FName SessionName = CurrentSessionName;
			// 서버가 방을 떠나면, 세션을 삭제하자
			if (PC->HasAuthority())
			{
				// 먼저 세션을 끝내고
				SessionInterface->EndSession(SessionName);
				// 그 다음 파괴
				SessionInterface->DestroySession(SessionName);
			}
			else
			{
				// 클라이언트가 방을 떠나면, 그 클라이언트만 세션에서 나오자
				SessionInterface->EndSession(SessionName);
			}
		}
	}
	FFastLogger::LogConsole(TEXT("UJumpGameInstance::LeaveSession, Server Leaving Session"));
}

FString UJumpGameInstance::StringBase64Encode(FString Str)
{
	// Set 할 때 : FString -> UTF8 -> TArray<uint8> -> base64 로 Encode
	std::string utf8String = TCHAR_TO_UTF8(*Str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)utf8String.c_str(), utf8String.length());

	return FBase64::Encode(arrayData);
}

FString UJumpGameInstance::StringBase64Decode(FString Str)
{
	// Get 할 때 : base64 를 Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(Str, arrayData);
	std::string utf8String((char*)arrayData.GetData(), arrayData.Num());

	return UTF8_TO_TCHAR(utf8String.c_str());
}

void UJumpGameInstance::SetPlayerWinInfo(const FString PlayerNetID, bool bIsWin)
{
	if (PlayerMap.Contains(PlayerNetID))
	{
		// 승리 상태 업데이트
		FPlayerInfo& PlayerInfo = PlayerMap[PlayerNetID];
		PlayerInfo.bIsWin = bIsWin;
	}
}

void UJumpGameInstance::RunEyeTrackingScript()
{
	// // Conda 가상환경 활성화
	// // Python 스크립트 실행
	// // 실시간으로 출력을 캡처
	//
	// // Conda의 activate.bat 파일 절대 경로 : 가상환경 활성화용 ( 다르면 자기꺼에 맞게 수정 해야함 )
	// FString CondaBatPath = TEXT("C:\\Users\\user\\miniconda3\\Scripts\\activate.bat");
	// // 사용할 Conda 가상환경 이름
	// FString CondaEnv = TEXT("myenv_311");
	// // 실행할 Python 스크립트 절대 경로
	// FString ScriptPath = TEXT("C:\\FinalProject\\Game\\AI_Service\\eye_tracking\\main.py");
	//
	// // IPC를 위한 파이프 생성
	// // 목적 : Python 스크립트의 출력을 언리얼에서 읽기 위해 사용
	// void* ReadPipe = nullptr;
	// void* WritePipe = nullptr;
	// FPlatformProcess::CreatePipe(ReadPipe, WritePipe);
	//
	// // 생성된 명령어
	// // cmd.exe /C "C:\Users\user\miniconda3\Scripts\activate.bat myenv_311
	// //		&& python "C:\FinalProject\Game\AI_Service\eye_tracking\main.py""
	// FString FullCommand = FString::Printf(
	// 	TEXT("/C \"\"%s\" %s && python \"%s\"\""),
	// 	*CondaBatPath,
	// 	*CondaEnv,
	// 	*ScriptPath
	// );
	//
	// // 프로세스 비동기로 실행 (cmd.exe를 메인 프로세스로)
	// // Python 스크립트의 출력을 WritePipe로, 입력을 ReadPipe로
	// FProcHandle ProcHandle = FPlatformProcess::CreateProc(
	// 	// 실행 파일
	// 	TEXT("cmd.exe"),
	// 	*FullCommand, // 인자 ( 위에서 만든 명령어 )
	// 	true, // 비동기 ( 언리얼 안멈추게 )
	// 	false, // 히든 창 ( 창 안보이게 )
	// 	false, // 디버깅용 플래그
	// 	nullptr,
	// 	0,
	// 	nullptr,
	// 	WritePipe, // 출력 리다이렉트 (파이프)
	// 	ReadPipe // 입력 리다이렉트 (파이프)
	// );
	//
	// // 별도 스레드에서 파이프 데이터 읽기
	// Async(EAsyncExecution::Thread, [ReadPipe, ProcHandle, WritePipe]() mutable {
	// 	// Python 프로세스가 실행 중인 동안 반복
	// 	while (FPlatformProcess::IsProcRunning(ProcHandle))
	// 	{
	// 		// 파이프에서 데이터 읽기
	// 		FString Output = FPlatformProcess::ReadPipe(ReadPipe);
	// 		if (!Output.IsEmpty())
	// 		{
	// 			// 파이썬 출력 처리 (게임 스레드에서 실행해야 UI 조작 가능)
	// 			AsyncTask(ENamedThreads::GameThread, [Output]() {
	// 				UE_LOG(LogTemp, Log, TEXT("[Python] %s"), *Output);
	// 			});
	// 		}
	// 		FPlatformProcess::Sleep(0.1f);
	// 	}
	//
	// 	// 리소스 정리 : 프로세스 종료 후 파이프, 핸들 닫기
	// 	FPlatformProcess::ClosePipe(ReadPipe, WritePipe);
	// 	FPlatformProcess::CloseProc(ProcHandle);
	// });


	// 1. Conda 환경으로 Python 스크립트 비동기 실행
	// FString CondaBatPath = TEXT("C:\\Users\\user\\miniconda3\\Scripts\\activate.bat");
	// FString CondaEnv = TEXT("myenv_311");
	// FString ScriptPath = TEXT("C:\\FinalProject\\Game\\AI_Service\\eye_tracking\\main.py");
	//
	// // 명령어 구성 (Hidden 창에서 실행)
	// FString Command = FString::Printf(
	// 	TEXT("/C \"\"%s\" %s && python \"%s\"\""),
	// 	*CondaBatPath,
	// 	*CondaEnv,
	// 	*ScriptPath
	// );
	//
	// // 비동기로 Python 실행 (창 숨김)
	// Async(EAsyncExecution::Thread, [Command]() {
	// 	std::system(TCHAR_TO_ANSI(*Command));
	// });

	FString CondaBatPath = TEXT("C:\\Users\\user\\miniconda3\\Scripts\\activate.bat");
	FString CondaEnv = TEXT("myenv_311");
	FString ScriptPath = TEXT("C:\\FinalProject\\Game\\AI_Service\\eye_tracking\\main.py");


	// 새 창에서 python 실행 후 창 유지
	FString Command = FString::Printf(
		TEXT("cmd /c \"\"%s\" %s && start \"\" cmd /k python \"%s\"\""),
		*CondaBatPath,
		*CondaEnv,
		*ScriptPath
	);
	// 비동기로 실행 (프로세스 완전 분리)
	Async(EAsyncExecution::Thread, [Command]()
	{
		std::system(TCHAR_TO_UTF8(*Command));
	});
}
