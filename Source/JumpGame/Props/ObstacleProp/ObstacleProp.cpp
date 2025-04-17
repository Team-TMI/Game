// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleProp.h"

#include <filesystem>
#include <tiff.h>

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Utils/FastLogger.h"


// Sets default values
AObstacleProp::AObstacleProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(
		TEXT("/Script/Engine.StaticMesh'/Game/Props/ObstacleBaseCube.ObstacleBaseCube'"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
	}
	CollisionComp->SetBoxExtent(FVector(49, 49, 0));
	CollisionComp->SetRelativeLocation(FVector(0, 0, 100));
	MeshComp->SetRelativeLocation(FVector(0, 0, -50));
	Tags.Add("Obstacle");
}

void AObstacleProp::OnMyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                            const FHitResult& Hit)
{
	AFrog* Character = Cast<AFrog>(OtherActor);
	if (Character)
	{
		if (bDebug)
		{
			FLog::Log("AObstacleProp::OnMyHit");
		}
		
		CalculateForce(Character);
	}
}

// Called when the game starts or when spawned
void AObstacleProp::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicateMovement(true);
	CollisionComp->OnComponentHit.AddDynamic(this, &AObstacleProp::OnMyHit);
}

// Called every frame
void AObstacleProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ObstacleRotate();
}

void AObstacleProp::LaunchCharacter(AFrog* Character, FVector Direction, float Force,
                                    bool bXYOverride, bool bZOverride)
{
	if (bDebug)
	{
		FLog::Log("AObstacleProp::LaunchCharacter", Direction.Z, Force);
	}
	// 가상 함수: 기본 로직
	LaunchVelocity = Direction.GetSafeNormal() * Force;
	Character->LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);
}

void AObstacleProp::CalculateForce(AFrog* Character)
{
	if (bDebug)
	{
		FLog::Log("AObstacleProp::CalculateForce");
	}
}

void AObstacleProp::ObstacleRotate()
{
	float RotSpeed = RotAngle * GetWorld()->DeltaTimeSeconds;
	FRotator DeltaRot = RotAxis * RotSpeed;
	// MeshComp->SetRelativeRotation(DeltaRot); // 혹시모름
	PivotScene->AddLocalRotation(DeltaRot);
}
