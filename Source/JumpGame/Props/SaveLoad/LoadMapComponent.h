#pragma once

#include "CoreMinimal.h"
#include "SaveData.h"
#include "Components/ActorComponent.h"
#include "LoadMapComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMapLoaded);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API ULoadMapComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULoadMapComponent();

	UFUNCTION()
	void LoadMap();
	UFUNCTION()
	void LoadMapWithString(const FString& FileName);

	UFUNCTION()
	void OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, TArray<FString>& OutFileNames);
	UFUNCTION()
	void PrintData(const FString& File);

	UPROPERTY(BlueprintAssignable)
	FOnMapLoaded OnMapLoaded;
	
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	bool LoadFileToJsonString(const FString& FilePath, FString& OutString);
	UFUNCTION()
	bool ParseJsonStringToMap(const FString& JsonString);
	UFUNCTION()
	void BuildMapFromSaveData();
	UFUNCTION()
	void SpawnProp(TSubclassOf<class APrimitiveProp> PropClass, const FSaveData& SaveData);

	UPROPERTY()
	FSaveDataArray SaveDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapLoad", meta = (AllowPrivateAccess = "true"))
	class UDataTable* PropTable = nullptr;
};
