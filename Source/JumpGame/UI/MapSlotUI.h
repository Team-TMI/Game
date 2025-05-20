#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "MapSlotUI.generated.h"

DECLARE_DELEGATE_OneParam(FOnMapSlotClicked, class UMapSlotUI* /*Map Full Path*/);

UCLASS()
class JUMPGAME_API UMapSlotUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintCallable)
	void Init(const FString& InMapFullPath, const FString& InMapName);

	FOnMapSlotClicked OnClicked;

	GETTER(FString, MapFullPath);
	GETTER(FString, MapName);
private:
	UFUNCTION()
	void OnBtnMapSlotClicked();
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "MapSlot", meta = (AllowPrivateAccess = "true"))
	class UButton* Btn_MapSlot;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "MapSlot", meta = (AllowPrivateAccess = "true"))
	class UTextBlock* Text_MapName;
	
	UPROPERTY()
	FString MapFullPath;
	UPROPERTY()
	FString MapName;
};
