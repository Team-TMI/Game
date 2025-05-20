#include "DirectoryItemUI.h"

#include "Components/Button.h"

void UDirectoryItemUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ClickButton->OnClicked.AddDynamic(this, &UDirectoryItemUI::OnButtonClicked);
}

FReply UDirectoryItemUI::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

	if (OnDoubleClicked.IsBound())
	{
		OnDoubleClicked.Execute(DirectoryPath);
	}
	return FReply::Handled();
}

FReply UDirectoryItemUI::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	if (OnClicked.IsBound())
	{
		OnClicked.Execute(DirectoryPath);
	}
	return FReply::Handled();
}

void UDirectoryItemUI::OnButtonClicked()
{
	if (OnClicked.IsBound())
	{
		OnClicked.Execute(DirectoryPath);
	}
}
