#pragma once

#include "CoreMinimal.h"
#include "PropStruct.generated.h"

// DataTable Struct
USTRUCT(BlueprintType)
struct FPropStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	FText PropName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	FText PropDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	class UTexture2D* PropIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prop")
	class UClass* PropClass;
};
