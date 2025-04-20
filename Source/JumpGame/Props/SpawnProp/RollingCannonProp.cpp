// Fill out your copyright notice in the Description page of Project Settings.


#include "RollingCannonProp.h"

#include "RollingBallProp.h"
#include "JumpGame/Utils/FastLogger.h"


// Sets default values
ARollingCannonProp::ARollingCannonProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("Cannon");
	bReplicates = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	ObjectPool = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("ObjectPool"));
}

void ARollingCannonProp::OnProjectileReturn()
{
	// 알림 받으면 다시 발사!
	FFastLogger::LogConsole(TEXT("알림받았다!! 다시 발사"));
	FireRollingBall();
}

// Called when the game starts or when spawned
void ARollingCannonProp::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	FireRollingBall();

	if (ObjectPool)
	{
		ObjectPool->OnObjectRetrurn.AddDynamic(this, &ARollingCannonProp::OnProjectileReturn);
	}
}

// Called every frame
void ARollingCannonProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARollingCannonProp::FireRollingBall()
{
	if (!ObjectPool) return;

	ARollingBallProp* Projectile = ObjectPool->GetRollingBallProp();
	if (Projectile)
	{
		FFastLogger::LogConsole(TEXT("Projectile!!!!!!"));
		Projectile->SetActorLocation(GetActorLocation());
		Projectile->SetActorRotation(GetActorRotation());
		Projectile->SetActive(true);
		Projectile->LaunchProjectile();
	}
}


