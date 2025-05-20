// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectRoomUI.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/GameState/LobbyGameState.h"
#include "JumpGame/Props/SaveLoad/LoadMapComponent.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "MapSlotUI.h"
#include "Components/HorizontalBox.h"
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

	ScrollBox_AllMap->ClearChildren();
	ScrollBox_OriginMap->ClearChildren();
	ScrollBox_CustomMap->ClearChildren();
	
	OriginMapList = GetMapList(TEXT(".json"), TEXT("Content\\Maps\\OriginMap"));
	CustomMapList = GetMapList(TEXT(".json"), TEXT("Content\\Maps\\CustomMap"));
	CombinedMapList.Append(OriginMapList);
	CombinedMapList.Append(CustomMapList);

	InitAllMap();
	InitOriginMap();
	InitCustomMap();

	for (int32 i = 0; i < HorizontalBoxCount; i++)
	{
		UHorizontalBox* HorizontalBox = NewObject<UHorizontalBox>(this, UHorizontalBox::StaticClass());
		MapSlotBoxes.Add(HorizontalBox);
	}
}

void USelectRoomUI::OnClickGoBackWait()
{
	// 맵 선택 취소
	// RemoveFromParent();
	PreviewSelectedMapSlotUI = nullptr;
}

void USelectRoomUI::OnClickSelectComplete()
{
	// TODO: 맵 정보 넘겨주기
	// 선택완료 됐으니 팝업창 지우자
	RemoveFromParent();
	CurrentSelectedMapSlotUI = PreviewSelectedMapSlotUI;
	PreviewSelectedMapSlotUI = nullptr;
	UJumpGameInstance* GameInstance = Cast<UJumpGameInstance>(GetGameInstance());
	if (!GameInstance)
	{
		return ;
	}

	GameInstance->SetMapFilePath(CurrentSelectedMapSlotUI->GetMapFullPath());
}

void USelectRoomUI::OnClickAllMap()
{
	WidgetSwitcher_SR->SetActiveWidgetIndex(0);
	FillScrollBox(ScrollBox_AllMap, CombinedMapListUI);
}

void USelectRoomUI::OnClickOriginMap()
{
	WidgetSwitcher_SR->SetActiveWidgetIndex(1);
	FillScrollBox(ScrollBox_OriginMap, OriginMapListUI);
}

void USelectRoomUI::OnClickCustomMap()
{
	WidgetSwitcher_SR->SetActiveWidgetIndex(2);
	FillScrollBox(ScrollBox_CustomMap, CustomMapListUI);
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
		// const FString ItemName = FPaths::GetCleanFilename(ItemPath);

		if (!bIsDirectory && ItemPath.EndsWith(MapType))
		{
			MapList.Add(ItemPath);
		}
		return true;
	});
	
	return MapList;
}

void USelectRoomUI::InitAllMap()
{
	for (FString& MapFullPath : CombinedMapList)
	{
		FString MapName = FPaths::GetCleanFilename(MapFullPath);
		UMapSlotUI* MapSlot = CreateWidget<UMapSlotUI>(GetWorld(), MapSlotClass);
		MapSlot->Init(MapFullPath, MapName);
		MapSlot->OnClicked.BindUObject(this, &USelectRoomUI::OnMapClicked);
		MapSlot->SetPadding(FMargin(0, 0, 10.0, 0));
		CombinedMapListUI.Add(MapSlot);
	}
}

void USelectRoomUI::InitOriginMap()
{
	for (FString& MapFullPath : OriginMapList)
	{
		FString MapName = FPaths::GetCleanFilename(MapFullPath);
		UMapSlotUI* MapSlot = CreateWidget<UMapSlotUI>(GetWorld(), MapSlotClass);
		MapSlot->Init(MapFullPath, MapName);
		MapSlot->OnClicked.BindUObject(this, &USelectRoomUI::OnMapClicked);
		MapSlot->SetPadding(FMargin(0, 0, 10.0, 0));
		OriginMapListUI.Add(MapSlot);
	}
}

void USelectRoomUI::InitCustomMap()
{
	for (FString& MapFullPath : CustomMapList)
	{
		FString MapName = FPaths::GetCleanFilename(MapFullPath);
		UMapSlotUI* MapSlot = CreateWidget<UMapSlotUI>(GetWorld(), MapSlotClass);
		MapSlot->Init(MapFullPath, MapName);
		MapSlot->OnClicked.BindUObject(this, &USelectRoomUI::OnMapClicked);
		MapSlot->SetPadding(FMargin(0, 0, 10.0, 0));
		CustomMapListUI.Add(MapSlot);
	}
}

void USelectRoomUI::OnMapClicked(UMapSlotUI* MapSlot)
{
	UJumpGameInstance* GameInstance = Cast<UJumpGameInstance>(GetGameInstance());
	if (!GameInstance)
	{
		return ;
	}

	PreviewSelectedMapSlotUI = MapSlot;	
}

void USelectRoomUI::FillScrollBox(UScrollBox* ScrollBox, const TArray<UMapSlotUI*>& MapList)
{
	// 초기화
	ScrollBox->ClearChildren();
	for (int32 i = 0; i < MapSlotBoxes.Num(); i++)
	{
		MapSlotBoxes[i]->ClearChildren();
	}

	// 각각의 HorizontalBox에 3개씩 넣어준다
	const int32 NumPerRow = 3;
	int32 RowCount = 0;
	for (int32 i = 0; i < MapList.Num(); i += NumPerRow)
	{
		UHorizontalBox* HorizontalBox = MapSlotBoxes[RowCount++]; 
		for (int32 j = 0; j < NumPerRow; j++)
		{
			int32 Index = i + j;
			if (Index >= MapList.Num())
				break;

			UMapSlotUI* InSlot = MapList[Index];
			HorizontalBox->AddChild(InSlot);
		}

		ScrollBox->AddChild(HorizontalBox);
	}
}