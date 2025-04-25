#pragma once

#include "CoreMinimal.h"
#include "ECategoryType.h"
#include "MajorTableInfo.h"
#include "PropStruct.h"
#include "Components/ActorComponent.h"
#include "CategorySystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPropAdded, const FPropStruct&, Prop);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPropRemoved, const FPropStruct&, Prop);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UCategorySystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UCategorySystem();

protected:
	virtual void BeginPlay() override;

	// CRUD 함수
	UFUNCTION(BlueprintCallable)
	bool AddProp(const FPropStruct& NewProp);
	UFUNCTION(BlueprintCallable)
	bool RemovePropByID(FName ID);

	// 조회 함수
	const TArray<FPropStruct*>& GetPropsByMajor(EMajorCategoryType Major);
	const TArray<FPropStruct*>& GetPropsBySub(EMajorCategoryType Major, ESubCategoryType Sub);
	const FPropStruct* GetPropsByID(FName ID);

	UPROPERTY(BlueprintAssignable)
	FOnPropAdded OnPropAdded;
	UPROPERTY(BlueprintAssignable)
	FOnPropRemoved OnPropRemoved;

private:
	UFUNCTION()
	bool SubBelongsToMajorCategory(const EMajorCategoryType InMajorCategoryType, const ESubCategoryType InSubCategoryType);
	UFUNCTION()
	bool ValidateProp(const FPropStruct& InPropStruct);
	UFUNCTION()
	void ReIndex();
	UFUNCTION()
	void IndexProp(FPropStruct& InPropStruct);
	
	const FMajorTableInfo* FindMajorTableInfoRow(const EMajorCategoryType InMajorCategoryType);
	
	UPROPERTY()
	class UDataTable* CategoryDataTable = nullptr;
	UPROPERTY()
	class UDataTable* MajorTableInfoTable = nullptr;

	// 카테고리별로 Prop을 관리하기 위한 Map
	TArray<FPropStruct> PropList;
	TMap<EMajorCategoryType, TMap<ESubCategoryType, TArray<FPropStruct*>>> Index;
};
