// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectRoomUI.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/GameState/LobbyGameState.h"
#include "JumpGame/Props/SaveLoad/LoadMapComponent.h"
#include "JumpGame/Utils/FastLogger.h"

void USelectRoomUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Btn_GoBackWait->OnClicked.AddDynamic(this, &USelectRoomUI::OnClickGoBackWait);
	Btn_SelectComplete->OnClicked.AddDynamic(this, &USelectRoomUI::OnClickSelectComplete);
	
	Btn_AllMap->OnClicked.AddDynamic(this, &USelectRoomUI::OnClickAllMap);
	Btn_OriginMap->OnClicked.AddDynamic(this, &USelectRoomUI::OnClickOriginMap);
	Btn_CustomMap->OnClicked.AddDynamic(this, &USelectRoomUI::OnClickCustomMap);
	Btn_PickFile->OnClicked.AddDynamic(this, &USelectRoomUI::OnPickCustomMap);

	
	OriginMapList = GetMapList(TEXT(".json"), TEXT("Content/Maps/OriginMap"));
	CustomMapList = GetMapList(TEXT(".json"), TEXT("Content/Maps/CustomMap"));
	CombinedMapList.Append(OriginMapList);
	CombinedMapList.Append(CustomMapList);
}

void USelectRoomUI::OnClickGoBackWait()
{
	// 맵 선택 취소
	// RemoveFromParent();
	
}

void USelectRoomUI::OnClickSelectComplete()
{
	// TODO: 맵 정보 넘겨주기
	// 선택완료 됐으니 팝업창 지우자
	RemoveFromParent();
}

void USelectRoomUI::OnClickAllMap()
{
	WidgetSwitcher_SR->SetActiveWidgetIndex(0);

}

void USelectRoomUI::OnClickOriginMap()
{
	WidgetSwitcher_SR->SetActiveWidgetIndex(1);

}

void USelectRoomUI::OnClickCustomMap()
{
	WidgetSwitcher_SR->SetActiveWidgetIndex(2);

}

void USelectRoomUI::OnPickCustomMap()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC->HasAuthority())
	{
		return ;
	}
	ALobbyGameState* GameState = Cast<ALobbyGameState>(GetWorld()->GetGameState());
	if (!GameState)
	{
		return ;
	}
	FString Suffix = TEXT(".json");
	GameState->LoadMapComponent->PickFile(Suffix);
}

TArray<FString> USelectRoomUI::GetMapList(const FString& MapType, const FString& MapDir)
{
	TArray<FString> MapList;

	FString ExecutablePath = FPlatformProcess::ExecutablePath();
	FString ExecutableDir = FPaths::GetPath(ExecutablePath);
	FString MapPath = FPaths::Combine(ExecutableDir, MapDir);

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*MapPath))
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("Directory does not exist"));
		return MapList;
	}
	
	PlatformFile.IterateDirectory(*MapPath, [this, &MapList, &MapType](const TCHAR* Path, bool bIsDirectory) -> bool
	{
		const FString ItemPath = FString(Path);
		const FString ItemName = FPaths::GetCleanFilename(ItemPath);

		if (!bIsDirectory && ItemName.EndsWith(MapType))
		{
			MapList.Add(ItemName);
		}
		return true;
	});
	
	return MapList;
}
