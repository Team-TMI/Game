// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyFrog.h"

#include "JumpGame/Core/PlayerController/LobbyPlayerController.h"


// Sets default values
ALobbyFrog::ALobbyFrog()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<UAnimInstance> FrogABP
	(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/ABP_LobbyFrog.ABP_LobbyFrog_C'"));
	if (FrogABP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(FrogABP.Class);
	}
}

// Called when the game starts or when spawned
void ALobbyFrog::BeginPlay()
{
	Super::BeginPlay();

	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(Controller);
	if (!PC)
	{
		
	}
}

// Called every frame
void ALobbyFrog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(Controller);
	if (!PC)
	{
		return ;
	}
	if (PC->GetPitchYawRatio(PitchRatio, YawRatio) == 0)
	{
		return;
	}

	// pitch min max : -70 ~ 70
	// roll min max : -70 ~ 70
	PitchRatio = FMath::Clamp(PitchRatio, -1, 1);
	YawRatio = FMath::Clamp(YawRatio, -1, 1);

	Yaw = FMath::Lerp(MinMax.X, MinMax.Y, FMath::Abs(YawRatio));
	Yaw = FMath::Sign(YawRatio) * Yaw * -1;

	float PitchSign = FMath::Sign(PitchRatio);
	if (PitchSign < 0)
	{
		Pitch = FMath::Lerp(MinusPitchMinMax.X, MinusPitchMinMax.Y, FMath::Abs(PitchRatio));
	}
	else
	{
		Pitch = FMath::Lerp(PlusPitchMinMax.X, PlusPitchMinMax.Y, FMath::Abs(PitchRatio));
	}
	Pitch = FMath::Sign(PitchRatio) * Pitch;
}

// Called to bind functionality to input
void ALobbyFrog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

