#include "MapMenuUI.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/SizeBox.h"
#include "JumpGame/Core/GameState/MapEditorState.h"
#include "JumpGame/Props/SaveLoad/LoadMapComponent.h"
#include "JumpGame/Props/SaveLoad/SaveMapComponent.h"

void UMapMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SaveButton->OnClicked.AddDynamic(this, &UMapMenuUI::OnSaveButtonClicked);
	LoadButton->OnClicked.AddDynamic(this, &UMapMenuUI::OnLoadButtonClicked);
	SavePopup->SetVisibility(ESlateVisibility::Hidden);
	SaveConfirm->OnClicked.AddDynamic(this, &UMapMenuUI::OnSaveConfirmClicked);
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
	SavePopup->SetVisibility(ESlateVisibility::Hidden);
}

