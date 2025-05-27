#include "MapMenuUI.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/SizeBox.h"
#include "JumpGame/Core/GameState/MapEditorState.h"
#include "JumpGame/Props/SaveLoad/LoadMapComponent.h"
#include "JumpGame/Props/SaveLoad/SaveMapComponent.h"
#include "JumpGame/UI/FileBrowser/FileBrowserUI.h"
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

// SaveConfirm 버튼을 클릭했을 때 이미지를 넣을 지 여부를 묻는게 좋다고 생각함.
// 만약 이미지를 넣는 경우에는 이미지 파일을 선택할 수 있는 UI를 띄우고,
// 선택한 이미지 파일을 Base64로 인코딩하여 저장하는 방식으로 구현할 수 있음.
// 만약 이미지를 넣지 않는 경우에는 그냥 저장만 하면 될 것 같음.
void UMapMenuUI::OnSaveConfirmClicked()
{
	AMapEditorState* MapEditorState = Cast<AMapEditorState>(GetWorld()->GetGameState());
	if (MapEditorState)
	{
		SaveFileName = FileNameTextBox->GetText().ToString();

		MapEditorState->GetLoadMapComponent()->GetFileBrowserUI()->OnFileSelectedDelegate.BindUObject(this, &UMapMenuUI::OnImageSelected);
		MapEditorState->GetLoadMapComponent()->GetFileBrowserUI()->SetSuffix(TEXT(".png"));
		MapEditorState->GetLoadMapComponent()->GetFileBrowserUI()->SetHintText(TEXT("썸네일 이미지를 선택해주세요."));

		FString RelativeDir = FPaths::ProjectDir();

		FString AbsoluteDir = FPaths::ConvertRelativePathToFull(RelativeDir);
		FPaths::MakePlatformFilename(AbsoluteDir);

		FFastLogger::LogScreen(FColor::Red, TEXT("AbsoluteDir : %s"), *AbsoluteDir);

		MapEditorState->GetLoadMapComponent()->GetFileBrowserUI()->SetVisibility(ESlateVisibility::Visible);
		MapEditorState->GetLoadMapComponent()->GetFileBrowserUI()->LoadDirectoryContents(AbsoluteDir);
		// MapEditorState->GetSaveMapComponent()->SaveMap(FileName);
	}
	SavePopup->SetVisibility(ESlateVisibility::Collapsed);
}

void UMapMenuUI::OnImageSelected(const FString& FileName, bool bSuccess)
{
	AMapEditorState* MapEditorState = Cast<AMapEditorState>(GetWorld()->GetGameState());
	if (!MapEditorState)
	{
		SaveFileName = TEXT("");
		return;
	}

	MapEditorState->GetLoadMapComponent()->GetFileBrowserUI()->OnFileSelectedDelegate.Unbind();

	FString ImageBase64 = TEXT("");
	FileToBase64(FileName, ImageBase64);
	// TODO: 피드백 UI 제공
	if (MapEditorState->GetSaveMapComponent()->SaveMap(SaveFileName, ImageBase64))
	{
		UE_LOG(LogTemp, Log, TEXT("맵 저장 성공: %s"), *SaveFileName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("맵 저장 실패: %s"), *SaveFileName);
	}
	SaveFileName = TEXT("");
}

void UMapMenuUI::OnBackButtonClicked()
{
	// 로비로 이동하자
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/ClientRoomLevel"));
}

void UMapMenuUI::OnSaveCloseButtonClicked()
{
	SavePopup->SetVisibility(ESlateVisibility::Collapsed);
}

bool UMapMenuUI::FileToBase64(const FString& FileName, FString& OutBase64)
{
	const FString FilePath = FPaths::ConvertRelativePathToFull(FileName);

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("[FileToBase64] File not found : %s"), *FilePath);
		return false;
	}

	TArray<uint8> FileData;
	if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("[FileToBase64] Load failed : %s"), *FilePath);
		return false;
	}

	OutBase64 = FBase64::Encode(FileData);
	return true;
}
