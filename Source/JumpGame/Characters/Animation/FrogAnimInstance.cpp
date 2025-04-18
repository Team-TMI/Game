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
		bIsSwimming = Frog->bIsSwimming;
		
		Pitch = Frog->GetBaseAimRotation().Pitch;

		// 상대 Yaw
		const float CharacterYaw{static_cast<float>(Frog->GetActorRotation().Yaw)};
		// 절대 Yaw
		const float AimYaw{static_cast<float>(Frog->GetBaseAimRotation().Yaw)};
		// AimYaw - CharacterYaw
		const float RelativeYaw{FMath::FindDeltaAngleDegrees(CharacterYaw, AimYaw)};
		
		// 카메라로 캐릭터 정면 볼 때 고려
		if (RelativeYaw >= 90.f)
		{
			Yaw = 90.f - (RelativeYaw - 90.f);
		}
		else if (RelativeYaw <= -90.f)
		{
			Yaw = -90 - (RelativeYaw+ 90.f);
		}
		else
		{
			Yaw = RelativeYaw;
		}
	}
}
