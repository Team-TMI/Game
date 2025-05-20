#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "BackItemUI.generated.h"

DECLARE_DELEGATE_OneParam(FOnBackButtonDoubleClicked, const FString& /*Path*/);
DECLARE_DELEGATE_OneParam(FOnBackButtonClicked, const FString& );

UCLASS()
class JUMPGAME_API UBackItemUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	GETTER_SETTER(FString, BackDirectoryPath);
	GETTER_SETTER(FString, BackDirectoryName);
	GETTER_SETTER(class UTextBlock*, BackDirectoryText);

	FOnBackButtonDoubleClicked OnDoubleClicked;
	FOnBackButtonClicked OnClicked;
	
protected:
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	UFUNCTION()
	void OnButtonClicked();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UTextBlock* BackDirectoryText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UButton* ClickButton;
	
	UPROPERTY()
	FString BackDirectoryPath;
	UPROPERTY()
	FString BackDirectoryName;
};
