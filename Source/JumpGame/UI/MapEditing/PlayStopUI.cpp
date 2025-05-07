#include "PlayStopUI.h"

#include "EnhancedInputSubsystems.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Editor/VREditor/Private/UI/VREditorFloatingCameraUI.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/MapEditor/Pawn/MapEditingPawn.h"

void UPlayStopUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayPawn = GetWorld()->SpawnActor<AFrog>(AFrog::StaticClass(), FVector(-1000, -1000, -1000), FRotator::ZeroRotator);
	PlayPawn->SetActorHiddenInGame(true);

	PlayStopButton->OnClicked.AddDynamic(this, &UPlayStopUI::OnPlayStopButtonClicked);
	
	IMG_PlayStop->SetBrushFromTexture(PlayIcon);
}

void UPlayStopUI::OnPlayStopButtonClicked()
{
	bIsPlayMode = !bIsPlayMode;

	if (bIsPlayMode)
	{
		IMG_PlayStop->SetBrushFromTexture(StopIcon);
		PlayPawn->SetActorHiddenInGame(false);
	}
	else
	{
		IMG_PlayStop->SetBrushFromTexture(PlayIcon);
		PlayPawn->SetActorHiddenInGame(true);
	}
	ChangePlayer();
}

void UPlayStopUI::ChangePlayer()
{
	if (bIsPlayMode)
	{
		ChangeToFrog();
	}
	else
	{
		ChangeToEditor();
	}
}

void UPlayStopUI::ChangeToFrog()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	EditPawn = Cast<AMapEditingPawn>(PC->GetPawn());

	FVector CamLocation = PC->PlayerCameraManager->GetCameraLocation();
	FRotator CamRotation = PC->PlayerCameraManager->GetCameraRotation();
	
	PlayPawn->SetActorLocation(CamLocation);
	PlayPawn->SetActorRotation(CamRotation);

	PC->SetViewTargetWithBlend(PlayPawn, 0.2f);
	PC->Possess(PlayPawn);

	// Hide and deactivate editor pawn
	EditPawn->SetActorHiddenInGame(true);
	EditPawn->SetActive(false);

	// 🎯 Enhanced Input 갱신
	if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->ClearAllMappings();

			// 👉 PlayPawn용 InputContext 등록
			Subsystem->AddMappingContext(PlayPawnInputContext, 0);
		}
	}
}

void UPlayStopUI::ChangeToEditor()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	PlayPawn->SetActorHiddenInGame(true);

	FRotator CamRotation = PC->PlayerCameraManager->GetCameraRotation();
	
	EditPawn->SetActorLocation(PlayPawn->GetActorLocation());
	EditPawn->SetActorRotation(CamRotation);

	PlayPawn->SetActorLocation(FVector(-1000, -1000, -1000));
	PlayPawn->SetActorRotation(FRotator::ZeroRotator);

	PC->SetViewTargetWithBlend(EditPawn, 0.2f);
	PC->Possess(EditPawn);

	EditPawn->SetActorHiddenInGame(false);
	EditPawn->SetActive(true);

	// 🎯 Enhanced Input 갱신
	if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->ClearAllMappings();

			// 👉 EditPawn용 InputContext 등록
			Subsystem->AddMappingContext(EditPawnInputContext, 0);
		}
	}
}
