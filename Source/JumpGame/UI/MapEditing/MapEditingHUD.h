#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapEditingHUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDragEnterWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDragLeaveWidget);

UCLASS()
class JUMPGAME_API UMapEditingHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	void InitWidget(class UClickHandlerManager* ClickHandlerManager, class UWidgetMapEditDragDropOperation* DragDropOperation);
	void AddClickFunctionsToDelegate(class UClickHandlerManager* ClickHandlerManager);
	void AddDragDropFunctionsToDelegate(class UWidgetMapEditDragDropOperation* DragDropOperation);

	UPROPERTY(BlueprintAssignable)
	FOnDragEnterWidget OnDragEnterWidget;
	UPROPERTY(BlueprintAssignable)
	FOnDragLeaveWidget OnDragLeaveWidget;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapEditing", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> PropSlotClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapEditing", meta = (AllowPrivateAccess = "true"))
	class UPropSlot* PropSlot = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UBorder* TestBorder = nullptr;
};
