// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "Kismet/KismetMathLibrary.h"

void UFrogAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Frog = Cast<AFrog>(TryGetPawnOwner());
}

void UFrogAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Frog)
	{
		Speed = UKismetMathLibrary::VSize(Frog->GetCharacterMovement()->Velocity);
		bIsFalling = Frog->GetCharacterMovement()->IsFalling();
		bIsCrouching = Frog->bIsCrouching;
	}
}
