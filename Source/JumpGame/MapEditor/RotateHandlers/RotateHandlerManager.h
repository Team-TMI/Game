#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JumpGame/MapEditor/ClickHandlers/ClickResponse.h"
#include "RotateHandlerManager.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API URotateHandlerManager : public UActorComponent
{
	GENERATED_BODY()

public:
	URotateHandlerManager();

	void HandleRotate(FClickResponse& ClickResponse);
	void HandleAxis(FVector InAxis, FClickResponse& ClickResponse);
	
protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	
private:
	UFUNCTION()
	void ResetAxis();
	
	UPROPERTY()
	FVector Axis = FVector(0, 0, 1);
};
