// Fill out your copyright notice in the Description page of Project Settings.


#include "RollingBallProp.h"

#include "ObjectPoolComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ARollingBallProp::ARollingBallProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("RollingBall");

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

	// 속도 설정
	ProjectileComp->InitialSpeed = 1000.f;
	ProjectileComp->MaxSpeed = 1000.f;
	ProjectileComp->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void ARollingBallProp::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

// Called every frame
void ARollingBallProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARollingBallProp::RetrunSelf()
{
	// 소속 풀 없으면 함수 나가자
	if (ObjectPool == nullptr) return;
	
	// 오브젝트 풀에 스스로를 반환하고 비활성화
	ObjectPool->ReturnObject(this);
	SetActive(false);
}

void ARollingBallProp::SetActive(bool bIsActive)
{
	// 활성, 비활성화 여부
	bActive = bIsActive;
	SetActorHiddenInGame(!bIsActive);
	// SetActorEnableCollision(bIsActive);
	// SetActorTickEnabled(bIsActive);
}
