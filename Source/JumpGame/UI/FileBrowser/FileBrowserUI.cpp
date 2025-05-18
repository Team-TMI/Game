#include "FileBrowserUI.h"
#include "DirectoryItemUI.h"
#include "FileItemUI.h"
#include "BackItemUI.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UFileBrowserUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CurrentFileText->SetHintText(FText::FromString(DefaultString));
	SetVisibility(ESlateVisibility::Hidden);

	SelectButton->OnClicked.AddDynamic(this, &UFileBrowserUI::OnSelectButtonClicked);
	CloseButton->OnClicked.AddDynamic(this, &UFileBrowserUI::OnCloseButtonClicked);
}

void UFileBrowserUI::LoadDirectoryContents(const FString& DirectoryPath)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	FString NewDirectoryPath;
	if (DirectoryPath.Equals(TEXT("C:")))
	{
		NewDirectoryPath = TEXT("C:\\");
	}
	else
	{
		NewDirectoryPath = DirectoryPath;
	}

	CurrentDirectoryText->SetText(FText::FromString(NewDirectoryPath));
	// FFastLogger::LogScreen(FColor::Red, TEXT("LoadDirectoryContents : %s"), *NewDirectoryPath);
	
	if (!PlatformFile.DirectoryExists(*NewDirectoryPath))
	{
		FFastLogger::LogScreen(FColor::Red, TEXT("Directory does not exist"));
		return;
	}

	CurrentDirectory = NewDirectoryPath;
	FileListScrollBox->ClearChildren(); // 기존 목록 초기화

	CreateBackButton();

	// 디렉토리 우선 검출
	PlatformFile.IterateDirectory(*NewDirectoryPath, [this](const TCHAR* Path, bool bIsDirectory) -> bool
	{
		const FString ItemPath = FString(Path);
		const FString ItemName = FPaths::GetCleanFilename(ItemPath);

		if (bIsDirectory)
		{
			CreateDirectoryButton(ItemName, ItemPath);
		}
		return true;
	});

	// 파일은 후순위로 검출
	PlatformFile.IterateDirectory(*NewDirectoryPath, [this](const TCHAR* Path, bool bIsDirectory) -> bool
	{
		const FString ItemPath = FString(Path);
		const FString ItemName = FPaths::GetCleanFilename(ItemPath);
	
		// json 파일만 검출
		if (!bIsDirectory && ItemName.EndsWith(Suffix))
		{
			CreateFileButton(ItemName, ItemPath);
		}
		return true;
	});

	CurrentFileText->SetText(FText::FromString(TEXT("")));
}

void UFileBrowserUI::CreateDirectoryButton(const FString& DirName, const FString& FullPath)
{
	UDirectoryItemUI* DirectoryItem = CreateWidget<UDirectoryItemUI>(GetWorld(), DirectoryItemClass);
	if (!DirectoryItem)
	{
		return ;
	}
	DirectoryItem->SetDirectoryPath(FullPath);
	DirectoryItem->SetDirectoryName(DirName);

	FText DirectoryText = FText::FromString(DirectoryItem->GetDirectoryName());
	DirectoryItem->GetDirectoryText()->SetText(DirectoryText);
	
	DirectoryItem->OnDoubleClicked.BindUObject(this, &UFileBrowserUI::OnDirectorySelected);
	// DirectoryItem->OnClicked.BindUObject(this, &UFileBrowserUI::OnItemClicked);

	FileListScrollBox->AddChild(DirectoryItem);
}

void UFileBrowserUI::CreateFileButton(const FString& FileName, const FString& FullPath)
{
	UFileItemUI* FileItem = CreateWidget<UFileItemUI>(GetWorld(), FileItemClass);
	if (!FileItem)
	{
		return ;
	}
	FileItem->SetFilePath(FullPath);
	FileItem->SetFileName(FileName);
	FText FileText = FText::FromString(FileItem->GetFileName());
	FileItem->GetFileText()->SetText(FileText);
	
	FileItem->OnDoubleClicked.BindUObject(this, &UFileBrowserUI::OnFileSelected);
	FileItem->OnClicked.BindUObject(this, &UFileBrowserUI::OnItemClicked);

	FileListScrollBox->AddChild(FileItem);
}

void UFileBrowserUI::OnDirectorySelected(const FString& FullPath)
{
	// 디렉토리 선택 시 해당 디렉토리의 내용을 로드
	LoadDirectoryContents(FullPath);
}

// 외부에 최종적으로 선택된 파일 경로를 전달
void UFileBrowserUI::OnFileSelected(const FString& FullPath)
{
	OnFileSelectedDelegate.ExecuteIfBound(CurrentFilePath, true);

	FileListScrollBox->ClearChildren(); // 기존 목록 초기화
	
	CurrentDirectory = TEXT("");
	CurrentFilePath = TEXT("");
	
	SetVisibility(ESlateVisibility::Hidden);
}

void UFileBrowserUI::OnItemClicked(const FString& FilePath)
{
	CurrentFilePath = FilePath;
	FString FileName = FPaths::GetCleanFilename(FilePath);
	CurrentFileText->SetText(FText::FromString(FileName));
}

void UFileBrowserUI::OnSelectButtonClicked()
{
	if (CurrentFilePath.IsEmpty())
	{
		OnFileSelectedDelegate.Execute(CurrentFilePath, false);
		return;
	}
	if (CurrentFilePath.EndsWith(Suffix))
	{
		OnFileSelectedDelegate.Execute(CurrentFilePath, true);
	}
	else
	{
		OnFileSelectedDelegate.Execute(CurrentFilePath, false);
	}
	SetVisibility(ESlateVisibility::Hidden);
}

void UFileBrowserUI::CreateBackButton()
{
	FString ParentDir = FPaths::GetPath(CurrentDirectory);

	if (ParentDir.IsEmpty() || ParentDir == CurrentDirectory || CurrentDirectory.Equals(TEXT("C:\\")))
	{
		return; // 루트면 무시
	}
	
	UBackItemUI* BackButton = CreateWidget<UBackItemUI>(GetWorld(), BackButtonClass);
	if (!BackButton)
	{
		return ;
	}
	
	BackButton->SetBackDirectoryPath(ParentDir);
	BackButton->SetBackDirectoryName(FPaths::GetCleanFilename(ParentDir));
	FText BackButtonText = FText::FromString(BackButton->GetBackDirectoryPath());
	BackButton->GetBackDirectoryText()->SetText(BackButtonText);

	BackButton->OnDoubleClicked.BindUObject(this, &UFileBrowserUI::LoadDirectoryContents);

	FileListScrollBox->AddChild(BackButton);
}

void UFileBrowserUI::OnCloseButtonClicked()
{
	// Close the file browser UI
	SetVisibility(ESlateVisibility::Hidden);

	CurrentDirectory = TEXT("");
	CurrentFilePath = TEXT("");
	CurrentFileText->SetText(FText::FromString(TEXT("")));
	CurrentFileText->SetHintText(FText::FromString(DefaultString));

	OnFileSelectedDelegate.Execute(CurrentFilePath, false);
}
