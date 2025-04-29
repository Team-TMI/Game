#include "VerticalPropSlotUI.h"

#include "PropSlot.h"

void UVerticalPropSlotUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UVerticalPropSlotUI::InitWidget(UClickHandlerManager* ClickHandlerManager,
	class UWidgetMapEditDragDropOperation* CachedDragDropOperation)
{
	PropTop->InitWidget(CachedDragDropOperation);
	PropTop->OnPropSlotClicked.AddDynamic(ClickHandlerManager, &UClickHandlerManager::OnPropSlotClicked);
	
	PropBelow->InitWidget(CachedDragDropOperation);
	PropBelow->OnPropSlotClicked.AddDynamic(ClickHandlerManager, &UClickHandlerManager::OnPropSlotClicked);
}

void UVerticalPropSlotUI::SetPropSlots(FPropStruct* Top, FPropStruct* Below)
{
	PropTop->SetPropInfo(Top);
	PropBelow->SetPropInfo(Below);
}

void UVerticalPropSlotUI::ClearInfo()
{
	PropTop->ClearInfo();
	PropBelow->ClearInfo();
}
