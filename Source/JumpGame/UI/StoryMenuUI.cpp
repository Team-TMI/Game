// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryMenuUI.h"

#include "ClientRoomUI.h"
#include "Components/Button.h"
#include "JumpGame/Characters/LobbyCharacter/LobbyFrog.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/LotusLeafRiceUI.h"
#include "Lobby/MoonPondUI.h"
#include "Lobby/StoryUI.h"
#include "UICam/LobbyCameraComp.h"

UStoryMenuUI::UStoryMenuUI(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UMoonPondUI> MoonPondUI_ClassFinder(
	   TEXT("/Game/UI/Story/WBP_MoonPondStory.WBP_MoonPondStory_C")
   );
	if (MoonPondUI_ClassFinder.Succeeded())
	{
		MoonPondUIClass = MoonPondUI_ClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<ULotusLeafRiceUI> LotusLeafRiceUI_ClassFinder(
		TEXT("/Game/UI/Story/WBP_RiceStory.WBP_RiceStory_C")
	);
	if (LotusLeafRiceUI_ClassFinder.Succeeded())
	{
		LotusLeafRiceUIClass = LotusLeafRiceUI_ClassFinder.Class;
	}
}

void UStoryMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Btn_Back->OnClicked.AddDynamic(this, &UStoryMenuUI::OnClickBtn_Back);
	Btn_Ep1->OnClicked.AddDynamic(this, &UStoryMenuUI::OnClickEp1);
	Btn_Ep2->OnClicked.AddDynamic(this, &UStoryMenuUI::OnClickEp2);

	ALobbyFrog* Frog = Cast<ALobbyFrog>(GetWorld()->GetFirstPlayerController()->GetPawn());
	CameraComp = Cast<ULobbyCameraComp>(Frog->CameraComp);
}

void UStoryMenuUI::OnClickBtn_Back()
{
	// 로비로 돌아가자
	CameraComp->SetViewTarget();
	OnClickBackToLobby.Broadcast();
	RemoveFromParent();
}

void UStoryMenuUI::OnClickMainStory()
{}

void UStoryMenuUI::OnClickEp1()
{
	APlayerController* OwningPlayer{GetOwningPlayer()};
	if (OwningPlayer && LotusLeafRiceUIClass)
	{
		UStoryUI::CreateAndShowStory<ULotusLeafRiceUI>(OwningPlayer, LotusLeafRiceUIClass);
	}
}

void UStoryMenuUI::OnClickEp2()
{
	APlayerController* OwningPlayer{GetOwningPlayer()};
	if (OwningPlayer && MoonPondUIClass)
	{
		UStoryUI::CreateAndShowStory<UMoonPondUI>(OwningPlayer, MoonPondUIClass);
	}
}
