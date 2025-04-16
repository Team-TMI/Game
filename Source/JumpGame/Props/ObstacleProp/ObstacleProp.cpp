// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleProp.h"

#include <filesystem>
#include <tiff.h>

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AObstacleProp::AObstacleProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh>TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
	}
}

void AObstacleProp::OnMyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character)
	{
		LaunchCharacter(Character);
	}
}

// Called when the game starts or when spawned
void AObstacleProp::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->SetBoxExtent(FVector(49,49,0));
	CollisionComp->SetRelativeLocation(FVector(0,0,50));
	CollisionComp->OnComponentHit.AddDynamic(this, &AObstacleProp::OnMyHit);
}

// Called every frame
void AObstacleProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ObstacleRoatate();
}

void AObstacleProp::LaunchCharacter(ACharacter* Character)
{
	// 가상 함수: 기본 로직
	Character->LaunchCharacter(LaunchVelocity,bXYOverrid,bZOverride);
}

void AObstacleProp::ObstacleRoatate()
{
	// Yaw 축으로 회전시
	AddActorLocalRotation(FRotator(0,RotateSpeed*GetWorld()->DeltaTimeSeconds,0));
	RotYaw = GetActorRotation().Yaw;
}

