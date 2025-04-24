// Fill out your copyright notice in the Description page of Project Settings.


#include "TrampolineProp.h"

#include <filesystem>

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Utils/FastLogger.h"


// Sets default values
ATrampolineProp::ATrampolineProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("Trampoline");
}

// Called when the game starts or when spawned
void ATrampolineProp::BeginPlay()
{
	Super::BeginPlay();

	// 콜리전 설정 (Block)
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

// Called every frame
void ATrampolineProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ATrampolineProp::CalculateForce(AFrog* Character)
{

	//FVector CharacterDir = Character->GetVelocity().GetSafeNormal();
	//FVector Direction = CharacterDir + FVector::UpVector;
	
	FVector Direction{GetActorUpVector()};
	
	float Force;
	if (temp)
	{
		Force = FMath::Clamp(static_cast<float>(Character->PrevVelocityZLength), 800.f, 1'500.f);
	}
	else
	{
		Force = FMath::Clamp(static_cast<float>(Character->PrevVelocityZLength), 700.f, 700.f);
	}
	
	Force *= 1.2;
	
	if (bDebug)
	{
		FLog::Log("Z Speed", Character->PrevVelocityZLength);
	}
	
	Super::LaunchCharacter(Character, Direction, Force);
}