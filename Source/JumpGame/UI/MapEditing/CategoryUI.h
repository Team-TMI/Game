#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JumpGame/MapEditor/CategorySystem/ECategoryType.h"
#include "CategoryUI.generated.h"

UCLASS()
class JUMPGAME_API UCategoryUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	void InitWidget(class UClickHandlerManager* ClickHandlerManager, class UWidgetMapEditDragDropOperation* DragDropOperation);

private:
	void ClearSubCategoryButtons();
	void ClearMajorCategoryButtons();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UPropGridUI* GridUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UScrollBox* SubCategories;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UScrollBox* MajorCategories;

	UPROPERTY()
	class UMajorCategoryButtonUI* SelectedMajorCategory = nullptr;
	UPROPERTY()
	class UCategorySystem* CategorySystem = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UI")
	TSubclassOf<class UMajorCategoryButtonUI> MajorCategoryButtonUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "UI")
	TSubclassOf<class USubCategoryButtonUI> SubCategoryButtonUIClass;

	UPROPERTY()
	TMap<EMajorCategoryType, class UMajorCategoryButtonUI*> MajorCategoryButtons;
	UPROPERTY()
	TMap<ESubCategoryType, class USubCategoryButtonUI*> SubCategoryButtons;
	
	UFUNCTION()
	void OnMajorCategoryButtonClicked(const EMajorCategoryType& MajorCategory, bool bAbsolute = false);
	UFUNCTION()
	void OnSubCategoryButtonClicked(const EMajorCategoryType& MajorCategory, const ESubCategoryType& SubCategory);
};
