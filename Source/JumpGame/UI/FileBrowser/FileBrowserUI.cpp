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
	SetVisibility(ESlateVisibility::Collapsed);

	SelectButton->OnClicked.AddDynamic(this, &UFileBrowserUI::OnSelectButtonClicked);
	CloseButton->OnClicked.AddDynamic(this, &UFileBrowserUI::OnCloseButtonClicked);
}

void UFileBrowserUI::NativeDestruct()
{
	OnFileSelectedDelegate.Unbind();

	FileListScrollBox->ClearChildren(); // 기존 목록 초기화

	Super::NativeDestruct();
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

	FString ShownString = EllipsisLastFolders(NewDirectoryPath);
	
	CurrentDirectoryText->SetText(FText::FromString(ShownString));
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

		if (bIsDirectory && !ItemName.StartsWith(TEXT(".")))
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
	FString AbsolutePath = FPaths::ConvertRelativePathToFull(FullPath);
	FPaths::MakePlatformFilename(AbsolutePath);
	LoadDirectoryContents(AbsolutePath);
}

// 외부에 최종적으로 선택된 파일 경로를 전달
void UFileBrowserUI::OnFileSelected(const FString& FullPath)
{
	OnFileSelectedDelegate.ExecuteIfBound(CurrentFilePath, true);

	FileListScrollBox->ClearChildren(); // 기존 목록 초기화
	
	CurrentDirectory = TEXT("");
	CurrentFilePath = TEXT("");
	
	SetVisibility(ESlateVisibility::Collapsed);
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
		SetVisibility(ESlateVisibility::Collapsed);
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
	SetVisibility(ESlateVisibility::Collapsed);
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
	SetVisibility(ESlateVisibility::Collapsed);

	CurrentDirectory = TEXT("");
	CurrentFilePath = TEXT("");
	CurrentFileText->SetText(FText::FromString(TEXT("")));
	CurrentFileText->SetHintText(FText::FromString(DefaultString));

	OnFileSelectedDelegate.Execute(CurrentFilePath, false);
}

void UFileBrowserUI::SetHintText(const FString& HintText)
{
	CurrentFileText->SetHintText(FText::FromString(HintText));
}

void UFileBrowserUI::SetInfoText(const FString& InfoText)
{
	InfomationText->SetText(FText::FromString(InfoText));
}

FString UFileBrowserUI::EllipsisLastFolders(const FString& InPath, int32 NumFoldersToKeep)
{
	// 1) OS 구분자를 통일해 두면 처리하기가 쉽다
	FString Normalized = InPath;
	FPaths::NormalizeFilename(Normalized);      // '\' → '/'

	// 뒤에 슬래시가 붙어 있으면 제거
	if (Normalized.EndsWith(TEXT("/")))
	{
		Normalized.LeftChopInline(1);
	}

	// 2) 디렉터리 조각을 분리
	TArray<FString> Segments;
	Normalized.ParseIntoArray(Segments, TEXT("/"), /* CullEmpty = */ true);

	if (Segments.Num() <= NumFoldersToKeep)
	{
		// 폴더가 2개 이하 → 전부 표시 (굳이 줄이지 않음)
		return Normalized;
	}

	// 3) 마지막 N개만 다시 조립
	FString EllipsisPath;
	EllipsisPath.Append(TEXT("…"));
	for (int32 i = Segments.Num() - NumFoldersToKeep; i < Segments.Num(); ++i)
	{
		EllipsisPath.Append(TEXT("/"));
		// 4) 만약에 Segment의 길이가 4자 이상이면 뒤에 ..
		// 그리고 마지막이면 ..을 붙이지 않음
		const bool bIsLast = (i == Segments.Num() - 1);
		if (Segments[i].Len() > 4 && !bIsLast)
		{
			EllipsisPath.Append(Segments[i].Left(4));
			EllipsisPath.Append(TEXT(".."));
		}
		else
		{
			EllipsisPath.Append(Segments[i]);
		}
	}
	return FString(EllipsisPath);
}
