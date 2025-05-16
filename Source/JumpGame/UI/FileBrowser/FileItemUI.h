#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "FileItemUI.generated.h"

DECLARE_DELEGATE_OneParam(FOnFileItemDoubleClicked, const FString& /*Path*/);
DECLARE_DELEGATE_OneParam(FOnFileItemClicked, const FString& /*Path*/);

UCLASS()
class JUMPGAME_API UFileItemUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	GETTER_SETTER(FString, FilePath);
	GETTER_SETTER(FString, FileName);
	GETTER_SETTER(class UTextBlock*, FileText);

	FOnFileItemDoubleClicked OnDoubleClicked;
	FOnFileItemClicked OnClicked;
protected:
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	UFUNCTION()
	void OnButtonClicked();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UTextBlock* FileText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UButton* ClickButton;
	
	UPROPERTY()
	FString FilePath;
	UPROPERTY()
	FString FileName;
};
