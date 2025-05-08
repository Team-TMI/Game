#include "SubCategoryButtonUI.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "JumpGame/MapEditor/CategorySystem/CategoryName.h"
#include "JumpGame/Utils/CommonUtils.h"

void USubCategoryButtonUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// TODO : 여기서 부터 진행
	SubCategoryButton->OnClicked.AddDynamic(this, &USubCategoryButtonUI::SubCategoryButtonClicked);
}

void USubCategoryButtonUI::SetSubCategoryInfo(EMajorCategoryType InMajorCategory, ESubCategoryType InSubCategory)
{
	MajorCategory = InMajorCategory;
	SubCategory = InSubCategory;

	// SubCategoryText->SetText(FText::FromString(FCommonUtil::GetEnumDisplayName(SubCategory).ToString()));
	FText InCategoryName = CategoryNameTable->FindRow<FCategoryName>(FName(*FCommonUtil::GetEnumDisplayName(SubCategory).ToString()), TEXT(""), true)->DisplayName;
	SubCategoryText->SetText(InCategoryName);
}

void USubCategoryButtonUI::SubCategoryButtonClicked()
{
	OnSubCategoryButtonClicked.Broadcast(MajorCategory, SubCategory);
}
