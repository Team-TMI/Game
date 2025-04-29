// Fill out your copyright notice in the Description page of Project Settings.


#include "FrogAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

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
		//UE_LOG(LogTemp, Warning, TEXT("%s Pitch: %f"), Frog->GetLocalRole() == ROLE_Authority ? TEXT("Server") : TEXT("Client"), Pitch);
		// 카메라로 캐릭터 정면 볼 때 고려
		if (RelativeYaw >= 90.f)
		{
			Yaw = 90.f - (RelativeYaw - 90.f);
		}
		else if (RelativeYaw <= -90.f)
		{
			Yaw = -90 - (RelativeYaw + 90.f);
		}
		else
		{
			Yaw = RelativeYaw;
		}
	}
}

void UFrogAnimInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UFrogAnimInstance, Pitch);
	DOREPLIFETIME(UFrogAnimInstance, Yaw);
	DOREPLIFETIME(UFrogAnimInstance, Speed);
	DOREPLIFETIME(UFrogAnimInstance, bIsFalling);
	DOREPLIFETIME(UFrogAnimInstance, bIsCrouching);
	DOREPLIFETIME(UFrogAnimInstance, bIsSwimming);
}
