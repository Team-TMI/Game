// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FrogAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UFrogAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class AFrog* Frog;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsFalling;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsCrouching;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Pitch;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Yaw;
};
