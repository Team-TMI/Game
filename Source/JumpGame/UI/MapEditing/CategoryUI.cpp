// Fill out your copyright notice in the Description page of Project Settings.


#include "CategoryUI.h"

#include "MajorCategoryButtonUI.h"
#include "PropGridUI.h"
#include "SubCategoryButtonUI.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "JumpGame/Core/GameState/MapEditorState.h"
#include "JumpGame/MapEditor/CategorySystem/CategorySystem.h"
#include "JumpGame/MapEditor/CategorySystem/ECategoryType.h"

class ANetworkGameState;

void UCategoryUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	ClearMajorCategoryButtons();
	ClearSubCategoryButtons();
	
	// GameState를 가져와서 모든 MajorCategory 리스트를 가져온다.
	AMapEditorState* GameState = GetWorld()->GetGameState<AMapEditorState>();
	if (GameState)
	{
		// Category 시스템을 가져옴.
		CategorySystem = GameState->GetCategorySystem();
	}

	if (!CategorySystem)
	{
		UE_LOG(LogTemp, Error, TEXT("CategorySystem is null"));
		return;
	}

	// MajorCategory 버튼들을 미리 만들어두기
	const TArray<EMajorCategoryType>& MajorCategoriesList = CategorySystem->GetMajorCategories();
	for (auto& MajorCategory : MajorCategoriesList)
	{
		UMajorCategoryButtonUI* MajorCategoryButtonUI = CreateWidget<UMajorCategoryButtonUI>(GetWorld(), MajorCategoryButtonUIClass);
		if (MajorCategoryButtonUI)
		{
			MajorCategoryButtons.Add(MajorCategory, MajorCategoryButtonUI);
			
			MajorCategoryButtonUI->SetMajorCategory(MajorCategory);
			MajorCategories->AddChild(MajorCategoryButtonUI);

			MajorCategoryButtonUI->OnMajorButtonClicked.AddDynamic(this, &UCategoryUI::OnMajorCategoryButtonClicked);
			
			if (MajorCategory == EMajorCategoryType::Basic)
			{
				SelectedMajorCategory = MajorCategoryButtonUI;
			}
		}
	}

	// SubCategory 버튼들을 미리 만들어두기
	for (auto& MajorCategory : MajorCategoriesList)
	{
		const TArray<ESubCategoryType>& SubCategoryList = CategorySystem->GetSubCategoriesByMajor(MajorCategory);
		for (auto& SubCategory : SubCategoryList)
		{
			USubCategoryButtonUI* SubCategoryButtonUI = CreateWidget<USubCategoryButtonUI>(GetWorld(), SubCategoryButtonUIClass);
			
			SubCategoryButtonUI->SetSubCategoryInfo(MajorCategory, SubCategory);

			SubCategoryButtons.Add(SubCategory, SubCategoryButtonUI);

			SubCategoryButtonUI->OnSubCategoryButtonClicked.AddDynamic(this, &UCategoryUI::OnSubCategoryButtonClicked);
		}
	}

	OnMajorCategoryButtonClicked(SelectedMajorCategory->GetMajorCategoryType(), true);
	
	FText Text = FText::FromString(DefaultSearchText);
	SearchText->SetHintText(Text);

	SearchText->OnTextCommitted.AddDynamic(this, &UCategoryUI::OnSearchTextCommitted);
}

void UCategoryUI::InitWidget(class UClickHandlerManager* ClickHandlerManager,
	class UWidgetMapEditDragDropOperation* DragDropOperation)
{
	GridUI->InitWidget(ClickHandlerManager, DragDropOperation);
}

void UCategoryUI::OnMajorCategoryButtonClicked(const EMajorCategoryType& MajorCategory, bool bAbsolute)
{
	ClearSubCategoryButtons();

	const TArray<ESubCategoryType>& SubCategoryList = CategorySystem->GetSubCategoriesByMajor(MajorCategory);
	for (auto& SubCategory : SubCategoryList)
    {
        USubCategoryButtonUI* SubCategoryButtonUI = SubCategoryButtons[SubCategory];
        if (SubCategoryButtonUI)
        {
            SubCategories->AddChild(SubCategoryButtonUI);
            SubCategoryButtonUI->SetVisibility(ESlateVisibility::Visible);
        }
    }

	SelectedMajorCategory = MajorCategoryButtons[MajorCategory];

	// 모든 PropData를 업데이트한다.
	GridUI->UpdatePropGrid(MajorCategory, CategorySystem);
}

void UCategoryUI::ClearSubCategoryButtons()
{
	// MajorCategory 버튼을 클릭했을 때, SubCategory 버튼들을 초기화한다.
	TArray<UWidget*> Children = SubCategories->GetAllChildren();
	for (int32 i = Children.Num() - 1; i >= 0; --i)
	{
		UWidget* Child = Children[i];
		
		if (Child && Child->IsA(USubCategoryButtonUI::StaticClass()))
		{
			SubCategories->RemoveChild(Child);
		}
	}
}

void UCategoryUI::ClearMajorCategoryButtons()
{
	TArray<UWidget*> Children = MajorCategories->GetAllChildren();

	// 역순 루프로 제거해야 제대로 제거됨
	for (int32 i = Children.Num() - 1; i >= 0; --i)
	{
		UWidget* Child = Children[i];
        
		if (Child && Child->IsA(UMajorCategoryButtonUI::StaticClass()))
		{
			MajorCategories->RemoveChild(Child);
		}
	}
}

void UCategoryUI::OnSubCategoryButtonClicked(const EMajorCategoryType& MajorCategory, const ESubCategoryType& SubCategory)
{
    // SubCategory 버튼을 클릭했을 때, PropGridUI를 업데이트한다.
    GridUI->UpdatePropGridBySub(MajorCategory, SubCategory, CategorySystem);
}

void UCategoryUI::OnSearchTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		FString InputText = Text.ToString();
		GridUI->UpdatePropGridBySearch(InputText, CategorySystem);
		return ;
	}
	FString InputText = Text.ToString();
	if (InputText == TEXT(""))
	{
		GridUI->UpdatePropGrid(SelectedMajorCategory->GetMajorCategoryType(), CategorySystem);
		FText DefaultText = FText::FromString(DefaultSearchText);
		SearchText->SetHintText(DefaultText);
	}
}

void UCategoryUI::OnPropSlotClicked(const FName& PropID)
{
}
