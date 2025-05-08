// Fill out your copyright notice in the Description page of Project Settings.


#include "RollingCannonProp.h"

#include "RollingBallProp.h"
#include "Components/BoxComponent.h"
#include "JumpGame/Props/Components/PropDataComponent.h"
#include "JumpGame/Utils/FastLogger.h"


// Sets default values
ARollingCannonProp::ARollingCannonProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Tags.Add("Cannon");
	
	ObjectPool = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("ObjectPool"));

	// 콜리전 없음
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel9, ECollisionResponse::ECR_Ignore);
	MeshComp->SetCollisionResponseToChannel(ECC_GameTraceChannel9, ECollisionResponse::ECR_Ignore);
	PropDataComponent->SetPropID(TEXT("5002"));
}

void ARollingCannonProp::OnProjectileReturn()
{
	// 서버에서만 처리
	// OnObjectReturned은 모든 클라이언트에서 호출되어, FireRollingBall() 중복될 수도 있음
	if (HasAuthority())
	{
		// 알림 받으면 다시 발사!
		// FFastLogger::LogConsole(TEXT("알림받았다!! 다시 발사"));
		FireRollingBall();
	}
}

// Called when the game starts or when spawned
void ARollingCannonProp::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// 서버에서만
	if (HasAuthority() && ObjectPool)
	{
		FireRollingBall();
		ObjectPool->OnObjectReturn.AddDynamic(this, &ARollingCannonProp::OnProjectileReturn);
	}
}

void ARollingCannonProp::SetCollision(bool bEnable)
{
	Super::SetCollision(bEnable);

	// 장애물에 추가 메쉬가 있는 경우에 override
	if (bEnable)
	{
		// 추가 메쉬 설정
		MeshComp->SetMaterial(0, UnSelectedObjectMaterial);
		MeshComp->SetRenderCustomDepth(false);
	}
	else
	{
		// 추가 메쉬 설정
		// TODO: Material 불투명하게 바꿔주기
		MeshComp->SetMaterial(0, SelectedObjectMaterial);
		MeshComp->SetRenderCustomDepth(true);
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
		// FFastLogger::LogConsole(TEXT("Projectile!!!!!!"));
		Projectile->SetActorLocation(GetActorLocation());
		Projectile->SetActorRotation(GetActorRotation());
		Projectile->SetActive(true);
		Projectile->LaunchProjectile();
		Projectile->bIsHitGround = false;
	}
}


