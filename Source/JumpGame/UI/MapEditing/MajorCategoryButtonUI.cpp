// Fill out your copyright notice in the Description page of Project Settings.


#include "MajorCategoryButtonUI.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UMajorCategoryButtonUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	MajorCategoryButton->OnClicked.AddDynamic(this, &UMajorCategoryButtonUI::OnMajorCategoryButtonClicked);
}

void UMajorCategoryButtonUI::SetMajorCategory(EMajorCategoryType MajorCategory)
{
	MajorCategoryType = MajorCategory;
	// 4 글자만 보여주기
	MajorCategoryText->SetText(FText::FromString(FCommonUtil::GetEnumDisplayName(MajorCategory).ToString().Left(4)));
	// MajorCategoryImage->SetBrushFromTexture(FCommonUtil::GetMajorCategoryIcon(MajorCategory));
}

void UMajorCategoryButtonUI::OnMajorCategoryButtonClicked()
{
	OnMajorButtonClicked.Broadcast(MajorCategoryType, false);
}
