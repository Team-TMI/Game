#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CategorySystem.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UCategorySystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UCategorySystem();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class UDataTable* CategoryDataTable = nullptr;
};
