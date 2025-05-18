#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "DirectoryItemUI.generated.h"

DECLARE_DELEGATE_OneParam(FOnDirectoryDoubleClicked, const FString& /*Path*/);
DECLARE_DELEGATE_OneParam(FOnDirectoryClicked, const FString& /*Name*/);

UCLASS()
class JUMPGAME_API UDirectoryItemUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	GETTER_SETTER(FString, DirectoryPath);
	GETTER_SETTER(FString, DirectoryName);
	GETTER_SETTER(class UTextBlock*, DirectoryText);

	FOnDirectoryDoubleClicked OnDoubleClicked;
	FOnDirectoryClicked OnClicked;
protected:
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	UFUNCTION()
	void OnButtonClicked();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UTextBlock* DirectoryText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UButton* ClickButton;
	
	UPROPERTY()
	FString DirectoryPath;
	UPROPERTY()
	FString DirectoryName;
};
