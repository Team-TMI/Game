#include "MapMenuUI.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/SizeBox.h"
#include "JumpGame/Core/GameState/MapEditorState.h"
#include "JumpGame/Props/SaveLoad/LoadMapComponent.h"
#include "JumpGame/Props/SaveLoad/SaveMapComponent.h"
#include "Kismet/GameplayStatics.h"

void UMapMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SaveButton->OnClicked.AddDynamic(this, &UMapMenuUI::OnSaveButtonClicked);
	LoadButton->OnClicked.AddDynamic(this, &UMapMenuUI::OnLoadButtonClicked);
	SavePopup->SetVisibility(ESlateVisibility::Collapsed);
	SaveConfirm->OnClicked.AddDynamic(this, &UMapMenuUI::OnSaveConfirmClicked);

	BackButton->OnClicked.AddDynamic(this, &UMapMenuUI::OnBackButtonClicked);
	SaveCloseButton->OnClicked.AddDynamic(this, &UMapMenuUI::OnSaveCloseButtonClicked);
}

void UMapMenuUI::OnSaveButtonClicked()
{
	SavePopup->SetVisibility(ESlateVisibility::Visible);
}

void UMapMenuUI::OnLoadButtonClicked()
{
	AMapEditorState* MapEditorState = Cast<AMapEditorState>(GetWorld()->GetGameState());
	if (MapEditorState)
	{
		MapEditorState->GetLoadMapComponent()->LoadMap();
	}
}

void UMapMenuUI::OnSaveConfirmClicked()
{
	AMapEditorState* MapEditorState = Cast<AMapEditorState>(GetWorld()->GetGameState());
	if (MapEditorState)
	{
		FString FileName = FileNameTextBox->GetText().ToString();
		MapEditorState->GetSaveMapComponent()->SaveMap(FileName);
	}
	SavePopup->SetVisibility(ESlateVisibility::Collapsed);
}

void UMapMenuUI::OnBackButtonClicked()
{
	// 로비로 이동하자
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/ClientRoomLevel"));
}

void UMapMenuUI::OnSaveCloseButtonClicked()
{
	SavePopup->SetVisibility(ESlateVisibility::Collapsed);
}
