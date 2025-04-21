#pragma once

#include "CoreMinimal.h"
#include "JumpGame/AIServices/Shared/Message.h"
#include "MessagePackage.generated.h"

// 가공할 데이터들의 구조체
USTRUCT(BlueprintType)
struct JUMPGAME_API FMessagePackage
{
	GENERATED_BODY()

public:
	UPROPERTY(Blueprintable)
	EMessageType MessageType = EMessageType::None;
	UPROPERTY(Blueprintable)
	class USoundWave* SoundWave = nullptr;
	UPROPERTY(Blueprintable)
	FMessageUnion MessageUnion;
	
	FMessagePackage(const EMessageType& InMessageType = EMessageType::None)
		: MessageType(InMessageType)
	{
	}
};