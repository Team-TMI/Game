#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapMenuUI.generated.h"

UCLASS()
class JUMPGAME_API UMapMenuUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
private:
	UFUNCTION()
	void OnSaveButtonClicked();
	UFUNCTION()
	void OnLoadButtonClicked();
	UFUNCTION()
	void OnSaveConfirmClicked();
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "MapMenu", meta = (AllowPrivateAccess = "true"))
	class UButton* SaveButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "MapMenu", meta = (AllowPrivateAccess = "true"))
	class UButton* LoadButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "MapMenu", meta = (AllowPrivateAccess = "true"))
	class USizeBox* SavePopup;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "MapMenu", meta = (AllowPrivateAccess = "true"))
	class UEditableTextBox* FileNameTextBox;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "MapMenu", meta = (AllowPrivateAccess = "true"))
	class UButton* SaveConfirm;

private:
	// TODO: 수정할거면 수정하기 (뒤로가기 관련)
	UFUNCTION()
	void OnBackButtonClicked();
	UFUNCTION()
	void OnSaveCloseButtonClicked();
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "MapMenu", meta = (AllowPrivateAccess = "true"))
	class UButton* BackButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "MapMenu", meta = (AllowPrivateAccess = "true"))
	class UButton* SaveCloseButton;
};
