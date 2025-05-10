// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryMenuUI.h"

#include "ClientRoomUI.h"
#include "Components/Button.h"
#include "JumpGame/Characters/LobbyCharacter/LobbyFrog.h"
#include "Kismet/GameplayStatics.h"
#include "UICam/LobbyCameraComp.h"

void UStoryMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Btn_Back->OnClicked.AddDynamic(this, &UStoryMenuUI::OnClickBtn_Back);
	
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
{
}

void UStoryMenuUI::OnClickEp1()
{
}

void UStoryMenuUI::OnClickEp2()
{
}
