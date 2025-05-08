// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Core/GameState/LobbyGameState.h"
#include "JumpGame/UI/WaitRoomUI.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"
#include "HAL/PlatformProcess.h"
#include "Async/Async.h"
#include "Misc/Paths.h"
#include "HAL/FileManagerGeneric.h"

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
		// 세션 파괴 성공시 호출되는 함수 등록
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UJumpGameInstance::OnDestroySessionComplete);
		// 세션 비정상 종료 감지시 호출되는 함수 등록
		SessionInterface->OnSessionFailureDelegates.AddUObject(this, &UJumpGameInstance::OnFailureSessionDetected);
		// 세션 끝냈을 시 호출되는 함수 등록
		SessionInterface->OnEndSessionCompleteDelegates.AddUObject(this, &UJumpGameInstance::OnEndSessionComplete);
	}

	if (bIsRunEyeScript)
	{
		RunEyeTrackingScript();
	}
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
	// 세션 실행중 들어오기 허용 여부
	SessionSettings.bAllowJoinInProgress = true;
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

	// 수업시간엔 안나왔지만 추가로 설정
	results[SessionIdx].Session.SessionSettings.bAllowJoinInProgress = true;
	results[SessionIdx].Session.SessionSettings.bShouldAdvertise = true;

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
		UE_LOG(LogTemp, Error, TEXT("Join 성공! : %d"), (int32)Result);
		FString url;
		if (SessionInterface->GetResolvedConnectString(SessionName, url))
		{
			UE_LOG(LogTemp, Warning, TEXT("Resolved URL: %s"), *url);
			APlayerController* pc = GetWorld()->GetFirstPlayerController();
			pc->ClientTravel(url, TRAVEL_Absolute);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GetResolvedConnectString 실패!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Join 실패! 이유: %d"), (int32)Result);
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

void UJumpGameInstance::OnDestroySessionComplete(FName Name, bool bArg)
{
	// 만약에 파괴 성공했다면
	if (bArg == true)
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("UJumpGameInstance::OnDestroySessionComplete"));

		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			// 서버라면, 로비로 이동하자
			if (PC->HasAuthority())
			{
				FFastLogger::LogScreen(FColor::Red, TEXT("서버 이동@@@@@"));
				GetWorld()->ServerTravel(TEXT("/Game/Maps/ClientRoomLevel?closed"));
			}
		}
	}
}

void UJumpGameInstance::OnFailureSessionDetected(const FUniqueNetId& UniqueNetId,
	ESessionFailure::Type Arg)
{
	FFastLogger::LogScreen(FColor::Orange, TEXT("OnFailure"));
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::OpenLevel(World, FName(TEXT("/Game/Maps/ClientRoomLevel")));
	}
}

void UJumpGameInstance::OnEndSessionComplete(FName Name, bool bArg)
{
	FFastLogger::LogScreen(FColor::Red, TEXT("OnEndSessionComplete@@@@@@"));
	UWorld* World = GetWorld();
	if (World)
	{
		// 서버 클라 구분없이 호출한 측에서 독립적으로 레벨 변경
		UGameplayStatics::OpenLevel(World, FName(TEXT("/Game/Maps/ClientRoomLevel")));
	}
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
	FLog::Log("RunEyeTrackingScript");
	
	FString PythonPath = TEXT("C:\\Users\\user\\miniconda3\\envs\\myenv_311\\python.exe");
	FString ScriptPath = TEXT("C:\\FinalProject\\Game\\AI_Service\\eye_tracking\\main.py");
	//FString ScriptPath = TEXT("C:\\FinalProject\\Game\\AI_Service\\eye_tracking\\infinite_counter.py");
	FString WorkingDirectory = FPaths::GetPath(ScriptPath);

	// Python 실행 명령 구성
	FString Command = FString::Printf(TEXT("\"%s\" \"%s\""), *PythonPath, *ScriptPath);

	// 비동기 작업 실행
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [Command, WorkingDirectory, PythonPath, ScriptPath]() {
		FProcHandle ProcHandle = FPlatformProcess::CreateProc(
		   *PythonPath,        // 실행 파일 경로
		   *FString::Printf(TEXT("\"%s\""), *ScriptPath), // 인수 (스크립트 경로)
		   false,              // bLaunchHidden
		   false,              // bLaunchReallyHidden
		   false,              // bLaunchInBackground (AsyncTask가 이미 백그라운드에서 실행)
		   nullptr,            // OutProcessID
		   0,                  // PriorityModifier
		   *WorkingDirectory,      // OptionalWorkingDirectory (FString 내용을 const TCHAR*로 전달)
		   nullptr,            // PipeWriteChild
		   nullptr             // PipeReadChild
		);
	
		if (!ProcHandle.IsValid())
		{
		   UE_LOG(LogTemp, Error, TEXT("비동기 Python 프로세스 생성 실패"));
		}
		else
		{
		   UE_LOG(LogTemp, Log, TEXT("비동기 Python 프로세스 시작"));
		   // FPlatformProcess::WaitForProc(ProcHandle); // 제거: 비동기 유지를 위해 대기하지 않음
		   FPlatformProcess::CloseProc(ProcHandle);
		}
	});
}
