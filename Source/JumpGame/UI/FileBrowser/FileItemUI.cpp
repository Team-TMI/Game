// Fill out your copyright notice in the Description page of Project Settings.


#include "FileItemUI.h"

#include "Components/Button.h"

void UFileItemUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ClickButton->OnClicked.AddDynamic(this, &UFileItemUI::OnButtonClicked);
}

FReply UFileItemUI::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

	if (OnDoubleClicked.IsBound())
	{
		OnDoubleClicked.Execute(FilePath);
	}
	return FReply::Handled();
}

void UFileItemUI::OnButtonClicked()
{
	if (OnClicked.IsBound())
	{
		OnClicked.Execute(FilePath);
	}
}


