// Fill out your copyright notice in the Description page of Project Settings.


#include "RollingBallProp.h"

#include "ObjectPoolComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "JumpGame/Props/LogicProp/RisingWaterProp.h"
#include "JumpGame/Utils/FastLogger.h"


// Sets default values
ARollingBallProp::ARollingBallProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("RollingBall");
	bReplicates = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	PivotScene = CreateDefaultSubobject<USceneComponent>(TEXT("PivotScene"));
	PivotScene->SetupAttachment(RootComponent);

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileComp->SetUpdatedComponent(RootComponent);
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Dir"));
	Arrow->SetupAttachment(RootComponent);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSphere(TEXT("/Script/Engine.StaticMesh'/Game/Props/SM_ObstacleSphere.SM_ObstacleSphere'"));
	if (TempSphere.Succeeded())
	{
		MeshComp->SetStaticMesh(TempSphere.Object);
	}
	MeshComp->SetRelativeScale3D(FVector(3.f));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 속도 설정
	ProjectileComp->InitialSpeed = 1000.f;
	ProjectileComp->MaxSpeed = 1000.f;
	ProjectileComp->SetIsReplicated(true);
}

void ARollingBallProp::OnMyRollingBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FFastLogger::LogConsole(TEXT("Hit!!!: %s"), *OtherActor->GetName());
	
	ARisingWaterProp* water = Cast<ARisingWaterProp>(OtherActor);
	if (water)
	{
		// 물에 부딪히면 타이머 초기화
		GetWorld()->GetTimerManager().ClearTimer(PoolTimerHandle);
		ReturnSelf();
		FLog::Log("물에 부딪힘, Clear Timer");
	}
}

// Called when the game starts or when spawned
void ARollingBallProp::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComp->OnComponentHit.AddDynamic(this, &ARollingBallProp::OnMyRollingBallHit);
}

// Called every frame
void ARollingBallProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARollingBallProp::ReturnSelf()
{
	// 소속 풀 없으면 함수 나가자
	if (ObjectPool == nullptr) return;
	// 오브젝트 풀에 스스로를 반환하고 비활성화
	SetActive(false);
	ObjectPool->ReturnObject(this);
}

void ARollingBallProp::SetActive(bool bIsActive)
{
	// 활성, 비활성화 여부
	bActive = bIsActive;
	SetActorHiddenInGame(!bIsActive);
	SetActorTickEnabled(bIsActive);
	if (bIsActive)
	{
		MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	}
	else
	{
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (bIsActive)
	{
		ProjectileComp->StopMovementImmediately();
		ProjectileComp->bSweepCollision = true;
		ProjectileComp->bRotationFollowsVelocity = true;
		ProjectileComp->bShouldBounce = false;
		ProjectileComp->bIsHomingProjectile = false;
		ProjectileComp->ProjectileGravityScale = 1;
		ProjectileComp->InitialSpeed = 1000.f;
		ProjectileComp->Velocity = FVector(1, 0, 0) * ProjectileComp->InitialSpeed;
		// NOTE: SetUpdatedComponent를 하지 않으면 재소환 이후 projectile movement가 실행되지않음
		ProjectileComp->SetUpdatedComponent(RootComponent);
		ProjectileComp->Activate(true);
		MeshComp->IgnoreActorWhenMoving(this,true);
	}
	
	FFastLogger::LogConsole(TEXT("SetActive: %d"), bIsActive);
}

void ARollingBallProp::LaunchProjectile()
{
	FVector LaunchDir = Arrow->GetForwardVector();
	ProjectileComp->Velocity = LaunchDir * ProjectileComp->InitialSpeed;

	// 발사 후 다시 복귀하는 타이밍
	// 바닥에 닿지않으면, 6초후에 복귀하자
	GetWorld()->GetTimerManager().SetTimer(PoolTimerHandle, this, &ARollingBallProp::ReturnSelf, 6.0f, false);
}