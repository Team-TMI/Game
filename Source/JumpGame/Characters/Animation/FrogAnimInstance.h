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

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class AFrog* Frog;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float Speed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	bool bIsFalling;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	bool bIsCrouching;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float Pitch;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float Yaw;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	bool bIsSwimming;
};
