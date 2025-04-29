#include "PropGridUI.h"

#include "VerticalPropSlotUI.h"
#include "Components/ScrollBox.h"

void UPropGridUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PropSlots.Empty();

	ClearPropGrid();
	
	for (int32 i = 0; i < PropStack; i++)
	{
		UVerticalPropSlotUI* PropSlot = CreateWidget<UVerticalPropSlotUI>(GetWorld(), VerticalPropSlotUIClass);
		PropSlots.Add(PropSlot);
	}
}

void UPropGridUI::InitWidget(class UClickHandlerManager* ClickHandlerManager,
	class UWidgetMapEditDragDropOperation* DragDropOperation)
{
	CachedClickHandlerManager = ClickHandlerManager;
	CachedDragDropOperation = DragDropOperation;

	for (auto& PropSlot : PropSlots)
	{
		PropSlot->InitWidget(CachedClickHandlerManager, CachedDragDropOperation);
	}
}

void UPropGridUI::ClearPropGrid()
{
	GridScroll->ClearChildren();
	
	for (auto& PropSlot : PropSlots)
	{
		PropSlot->ClearInfo();
	}
}

void UPropGridUI::UpdatePropGrid(const EMajorCategoryType& MajorCategory, UCategorySystem* CategorySystem)
{
	ClearPropGrid();

	const TArray<FPropStruct*>& PropList = CategorySystem->GetPropsByMajor(MajorCategory);
	
	int32 SlotIndex = 0;
	for (int32 i = 0; i < PropList.Num() && PropList.Num() >= 2; i += 2)
	{
		PropSlots[SlotIndex]->SetPropSlots(PropList[i], PropList[i + 1]);
		GridScroll->AddChild(PropSlots[SlotIndex]);
		SlotIndex++;
	}
	if (PropList.Num() % 2 == 1)
	{
		PropSlots[SlotIndex]->SetPropSlots(PropList.Last(), nullptr);
		GridScroll->AddChild(PropSlots[SlotIndex]);
		SlotIndex++;
	}
}

void UPropGridUI::UpdatePropGridBySub(const EMajorCategoryType& MajorCategory, ESubCategoryType SubCategory,
	UCategorySystem* CategorySystem)
{
	ClearPropGrid();

	const TArray<FPropStruct*>& PropList = CategorySystem->GetPropsBySub(MajorCategory, SubCategory);

	int32 SlotIndex = 0;
	for (int32 i = 0; i < PropList.Num() && PropList.Num() >= 2 ; i += 2)
	{
		PropSlots[SlotIndex]->SetPropSlots(PropList[i], PropList[i + 1]);
		GridScroll->AddChild(PropSlots[SlotIndex]);
		SlotIndex++;
	}
	if (PropList.Num() % 2 == 1)
	{
		PropSlots[SlotIndex]->SetPropSlots(PropList.Last(), nullptr);
		GridScroll->AddChild(PropSlots[SlotIndex]);
		SlotIndex++;
	}
}
