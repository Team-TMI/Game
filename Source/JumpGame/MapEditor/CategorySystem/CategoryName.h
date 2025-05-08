#pragma once

#include "CoreMinimal.h"
#include "CategoryName.generated.h"

USTRUCT(BlueprintType)
struct FCategoryName : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	FString CategoryName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	FText DisplayName;

	FCategoryName(
		const FString& InCategoryName = TEXT(""),
		const FText& InDisplayName = FText::GetEmpty())
		: CategoryName(InCategoryName)
		, DisplayName(InDisplayName)
	{
	}
};
