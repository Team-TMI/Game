// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

void UJumpGameInstance::Init()
{
	Super::Init();

	// 현재 사용하는 서브시스템을 가져오자
	IOnlineSubsystem* Subsys = IOnlineSubsystem::Get();
	if (Subsys)
	{
		SessionInterface = Subsys->GetSessionInterface();

		// 세션 생성 성공시 호출되는 함수 등록
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UJumpGameInstance::OnCreateSessionComplete);
		// 세션 검색 성공시 호출되는 함수 등록
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UJumpGameInstance::OnFindSessionComplete);
		// 세션 참여 성공시 호출되는 함수 등록
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UJumpGameInstance::OnJoinSessionComplete);
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
	// 세션 검색을 허용할 지 여부
	SessionSettings.bShouldAdvertise = true;
	// 세션 최대 인원 설정
	SessionSettings.NumPublicConnections = PlayerCount;
	// 커스텀 정보 (세션이름)
	// base64로 인코딩
	DisplayName = StringBase64Encode(DisplayName);
	SessionSettings.Set(FName(TEXT("DP_NAME")), DisplayName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 세션 생성
	SessionInterface->CreateSession(0,FName(DisplayName),SessionSettings);
}

void UJumpGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] 세션 생성 성공"), *SessionName.ToString());

		// 서버가 멀티플레이 하는 맵으로 이동
		// TODO: 이동할 맵 경로 설정하기
		GetWorld()->ServerTravel(TEXT("/Game/Maps/InGameLevel?listen"));
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
		UE_LOG(LogTemp,Warning,TEXT("세션 검색 성공!"));
		// 검색된 세션 결과들
		auto results = SessionSearch->SearchResults;
		for (int32 i=0; i<results.Num(); i++)
		{
			FString displayName;
			results[i].Session.SessionSettings.Get(FName(TEXT("DP_NAME")), displayName);

			// base64 Decode
			displayName = StringBase64Decode(displayName);
			
			UE_LOG(LogTemp,Warning,TEXT("세션 - %d, 이름: %s"), i, *displayName);

			OnFindComplete.ExecuteIfBound(i, displayName);
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("세션 검색 실패"));
	}

	// 검색 끝났다는 걸 알리자
	OnFindComplete.ExecuteIfBound(-1, FString());
}

void UJumpGameInstance::JoinOtherSession(int32 SessionIdx)
{
	// 검색된 세션 결과들
	auto results = SessionSearch->SearchResults;
	if (results.Num() == 0) return;

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
