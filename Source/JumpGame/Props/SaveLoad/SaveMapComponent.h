// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveData.h"
#include "Components/ActorComponent.h"
#include "Net/Core/NetBitArray.h"
#include "SaveMapComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API USaveMapComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USaveMapComponent();

	void SaveMap(const FString& FileName);

protected:
	virtual void BeginPlay() override;

private:
	void GetAllPropsInfo(TArray<FSaveData>& OutSaveDataArray);
	void SaveDataToFile(const FSaveDataArray& InSaveDataArray, const FString& FileName);

	UPROPERTY()
	FSaveDataArray SaveDataArray;
};
