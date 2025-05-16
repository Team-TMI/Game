#include "BackItemUI.h"

#include "Components/Button.h"

void UBackItemUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ClickButton->OnClicked.AddDynamic(this, &UBackItemUI::OnButtonClicked);
}

FReply UBackItemUI::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

	if (OnDoubleClicked.IsBound())
	{
		OnDoubleClicked.Execute(BackDirectoryPath);
	}
	return FReply::Handled();
}

void UBackItemUI::OnButtonClicked()
{
	if (OnClicked.IsBound())
	{
		OnClicked.Execute(BackDirectoryPath);
	}
}
