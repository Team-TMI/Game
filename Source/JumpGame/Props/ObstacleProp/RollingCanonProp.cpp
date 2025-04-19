// Fill out your copyright notice in the Description page of Project Settings.


#include "RollingCanonProp.h"

#include "RollingBallProp.h"


// Sets default values
ARollingCanonProp::ARollingCanonProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObjectPool = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("ObjectPool"));
}

// Called when the game starts or when spawned
void ARollingCanonProp::BeginPlay()
{
	Super::BeginPlay();
	
	FireRollingBall();
}

// Called every frame
void ARollingCanonProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARollingCanonProp::FireRollingBall()
{
	if (!ObjectPool) return;

	ARollingBallProp* Projectile = ObjectPool->GetRollingBallProp();
	if (Projectile)
	{
		Projectile->SetActive(true);
		Projectile->SetActorLocation(GetActorLocation());
		Projectile->SetActorRotation(GetActorRotation());
	}

	UE_LOG(LogTemp, Warning, TEXT("ARollingCanonProp::FireRollingBall"));
}


