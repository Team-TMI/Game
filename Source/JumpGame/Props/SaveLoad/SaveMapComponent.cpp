#include "SaveMapComponent.h"

#include "IContentBrowserSingleton.h"
#include "JsonObjectConverter.h"
#include "JumpGame/MapEditor/Components/GridComponent.h"
#include "JumpGame/Props/Components/PropDataComponent.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"
#include "Kismet/GameplayStatics.h"

USaveMapComponent::USaveMapComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USaveMapComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USaveMapComponent::SaveMap(const FString& FileName)
{
	GetAllPropsInfo(SaveDataArray.SaveDataArray);
	SaveDataToFile(SaveDataArray, FileName);
}

void USaveMapComponent::GetAllPropsInfo(TArray<FSaveData>& OutSaveDataArray)
{
	// 현재 월드에 있는 APrmitiveActor를 모두 가져옴
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APrimitiveProp::StaticClass(), AllActors);
	
	for (AActor* Actor : AllActors)
	{
		if (APrimitiveProp* PrimitiveProp = Cast<APrimitiveProp>(Actor))
		{
			FSaveData SaveData;

			SaveData.Id = PrimitiveProp->GetPropDataComponent()->GetPropID();
			SaveData.Position = PrimitiveProp->GetActorLocation();
			SaveData.Rotation = PrimitiveProp->GetActorRotation();
			SaveData.Size = PrimitiveProp->GetGridComp()->GetSize();
			
			OutSaveDataArray.Add(SaveData);
		}
	}
}

void USaveMapComponent::SaveDataToFile(const FSaveDataArray& InSaveDataArray, const FString& FileName)
{
	// InSaveDataArray를 JSON으로 변환
	FString JsonString;

	FJsonObjectConverter::UStructToJsonObjectString(InSaveDataArray, JsonString);

	// 경로에서 디렉토리만 추출
	FString Directory = DefaultDirectory + FPaths::GetPath(FileName);
    
	// 디렉토리 없으면 생성
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*Directory))
	{
		PlatformFile.CreateDirectoryTree(*Directory);
	}

	// 저장
	bool bSuccess = FFileHelper::SaveStringToFile(JsonString, *FileName);

	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("성공적으로 저장했습니다! 경로: %s"), *FileName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("저장 실패했습니다. 경로: %s"), *FileName);
	}
}
