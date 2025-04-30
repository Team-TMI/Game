#include "LoadMapComponent.h"

#include "JsonObjectConverter.h"
#include "JumpGame/MapEditor/CategorySystem/PropStruct.h"
#include "JumpGame/MapEditor/Components/GridComponent.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"

ULoadMapComponent::ULoadMapComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void ULoadMapComponent::BeginPlay()
{
	Super::BeginPlay();

}

void ULoadMapComponent::LoadMap(const FString& FilePath)
{
	FString JsonString;

	if (!LoadFileToJsonString(FilePath, JsonString))
	{
		return ;
	}
	if (!ParseJsonStringToMap(JsonString))
	{
		return ;
	}

	BuildMapFromSaveData();
}

bool ULoadMapComponent::LoadFileToJsonString(const FString& FilePath, FString& JsonString)
{
	// 파일에서 문자열 읽기
	if (FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		UE_LOG(LogTemp, Log, TEXT("파일에서 JSON을 성공적으로 읽었습니다!"));
		UE_LOG(LogTemp, Log, TEXT("JSON 문자열: %s"), *JsonString);

		// 이제 여기서 JsonString → FSaveDataArray 파싱도 가능함
		return true;
	}
	UE_LOG(LogTemp, Error, TEXT("파일을 읽는 데 실패했습니다: %s"), *FilePath);
	return false;
}

bool ULoadMapComponent::ParseJsonStringToMap(const FString& JsonString)
{
	if (FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &SaveDataArray, 0, 0))
	{
		UE_LOG(LogTemp, Log, TEXT("JSON → 구조체 변환 성공"));
		return true;
	}
	UE_LOG(LogTemp, Error, TEXT("JSON → 구조체 변환 실패"));
	return false;
}

void ULoadMapComponent::BuildMapFromSaveData()
{
	for (const FSaveData& SaveData : SaveDataArray.SaveDataArray)
	{
		FPropStruct* PropInfo = PropTable->FindRow<FPropStruct>(SaveData.Id, TEXT("LoadMap"), true);
		if (!PropInfo)
		{
			continue ;
		}
		TSubclassOf<APrimitiveProp> PropClass = PropInfo->PropClass;

		SpawnProp(PropClass, SaveData);	
	}
	OnMapLoaded.Broadcast();
}

void ULoadMapComponent::SpawnProp(TSubclassOf<APrimitiveProp> PropClass, const FSaveData& SaveData)
{
	FVector Location = SaveData.Position;
	FRotator Rotation = SaveData.Rotation;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(Location);
	SpawnTransform.SetRotation(FQuat(Rotation));
	SpawnTransform.SetScale3D({1.f, 1.f, 1.f});
	
	FVector Size = SaveData.Size;

	APrimitiveProp* NewProp = GetWorld()->SpawnActorDeferred<APrimitiveProp>(PropClass,	SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (NewProp)
	{
		NewProp->SetSize(Size);
		NewProp->FinishSpawning(SpawnTransform);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Prop Spawn 실패"));
	}
}
