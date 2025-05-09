// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleProp.h"

#include <filesystem>
#include <tiff.h>

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AObstacleProp::AObstacleProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("Obstacle");

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(
		TEXT("/Script/Engine.StaticMesh'/Game/Props/SM_ObstacleBaseCube.SM_ObstacleBaseCube'"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
	}
	CollisionComp->SetBoxExtent(FVector(55.f, 55.f, 55.f));
	
	Super::SetSize(FVector(1,1,1));

	bReplicates = true;
}

void AObstacleProp::OnMyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                            const FHitResult& Hit)
{
	if (!OtherActor->ActorHasTag("Frog")) return;
	if (bIsActive == false) return;
	
	/*AFrog* Character = Cast<AFrog>(OtherActor);
	if (Character)
	{
		if (bDebug)
		{
			FLog::Log("AObstacleProp::OnMyHit");
		}
		
		CalculateForce(Character);
	}*/
}

void AObstacleProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Frog")) return;
	if (bIsActive == false) return;
	
	AFrog* Character = Cast<AFrog>(OtherActor);
	if (Character)
	{
		if (bDebug)
		{
			FLog::Log("AObstacleProp::OnMyBeginOverlap");
		}
		
		CalculateForce(Character);
	}
	
	if (bDebug)
	{
		FLog::Log("AObstacleProp::OnMyBeginOverlap");
	}
}

void AObstacleProp::OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bDebug)
	{
		FLog::Log("AObstacleProp::OnMyEndOverlap");
	}
}

// Called when the game starts or when spawned
void AObstacleProp::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicateMovement(true);
	CollisionComp->OnComponentHit.AddDynamic(this, &AObstacleProp::OnMyHit);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AObstacleProp::OnMyBeginOverlap);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AObstacleProp::OnMyEndOverlap);
}

void AObstacleProp::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AObstacleProp, DeltaRot);
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
	if (bIsActive == false) return;
	
	// 가상 함수: 기본 로직
	// 서버라면
	if (HasAuthority())
	{
		LaunchVelocity = Direction.GetSafeNormal() * Force;
		Character->LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);
	}
	// else
	// {
	// 	ServerRPC_LaunchCharacter(Character, Direction, Force, bXYOverride, bZOverride);
	// }
}

// void AObstacleProp::ServerRPC_LaunchCharacter_Implementation(AFrog* Character, FVector Direction,
// 	float Force, bool bXYOverride, bool bZOverride)
// {
// 	FLog::Log("AObstacleProp::ServerRPC_LaunchCharacter");
// 	LaunchCharacter(Character, Direction, Force, bXYOverride, bZOverride);
// }

void AObstacleProp::CalculateForce(AFrog* Character)
{
	if (bDebug)
	{
		FLog::Log("AObstacleProp::CalculateForce");
	}
}

void AObstacleProp::ObstacleRotate()
{
	if (bIsActive == false) return;
	
	//서버라면
	if (HasAuthority())
	{
		float RotSpeed = RotAngle * GetWorld()->DeltaTimeSeconds;
		DeltaRot = RotAxis * RotSpeed;
		// MeshComp->SetRelativeRotation(DeltaRot); // 혹시모름
		PivotScene->AddLocalRotation(DeltaRot, true);
	}
}

void AObstacleProp::OnRep_ObstacleRotate()
{
	if (bIsActive == false) return;
	
	// 클라이언트라면 이 함수가 실행
	PivotScene->AddLocalRotation(DeltaRot);
}
