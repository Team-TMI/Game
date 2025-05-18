#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "FileBrowserUI.generated.h"

DECLARE_DELEGATE_TwoParams(FOnFileSelected, const FString& /*Path*/, bool /*bSuccess*/);

UCLASS()
class JUMPGAME_API UFileBrowserUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void LoadDirectoryContents(const FString& DirectoryPath);
	UFUNCTION()
	void CreateDirectoryButton(const FString& DirName, const FString& FullPath);
	UFUNCTION()
	void CreateFileButton(const FString& FileName, const FString& FullPath);

	UFUNCTION()
	void OnDirectorySelected(const FString& FullPath);
	UFUNCTION()
	void OnFileSelected(const FString& FullPath);
	UFUNCTION()
	void OnItemClicked(const FString& FileName);
	UFUNCTION()
	void OnSelectButtonClicked();

	UFUNCTION()
	void CreateBackButton();
	UFUNCTION()
	void OnCloseButtonClicked();

	FOnFileSelected OnFileSelectedDelegate;

	SETTER(FString, Suffix);
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UI")
	TSubclassOf<class UFileItemUI> FileItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UI")
	TSubclassOf<class UDirectoryItemUI> DirectoryItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UI")
	TSubclassOf<class UBackItemUI> BackButtonClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UScrollBox* FileListScrollBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UEditableText* CurrentFileText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UButton* SelectButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UButton* CloseButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UTextBlock* CurrentDirectoryText;

	FString CurrentDirectory;
	FString CurrentFilePath;
	FString DefaultString = TEXT(R"(파일을 선택하세요)");

	FString Suffix = TEXT("");
};
