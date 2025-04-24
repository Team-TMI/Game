// Fill out your copyright notice in the Description page of Project Settings.


#include "RollingBallProp.h"

#include "ObjectPoolComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "JumpGame/Props/LogicProp/RisingWaterProp.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/KismetMathLibrary.h"


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

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSphere(
		TEXT("/Script/Engine.StaticMesh'/Game/Props/SM_ObstacleSphere.SM_ObstacleSphere'"));
	if (TempSphere.Succeeded())
	{
		MeshComp->SetStaticMesh(TempSphere.Object);
	}
	MeshComp->SetRelativeScale3D(FVector(3.f));
	MeshComp->SetCollisionProfileName(TEXT("RollingBall"));

	// 속도 설정
	ProjectileComp->InitialSpeed = 1000.f;
	ProjectileComp->MaxSpeed = 1000.f;
	ProjectileComp->SetIsReplicated(true);
}

void ARollingBallProp::OnMyRollingBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                                          const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("Frog")) return;
	
	FFastLogger::LogConsole(TEXT("Hit!!!: %s"), *OtherActor->GetName());
	HitNormal = Hit.ImpactNormal;
	// NOTE: 구체가 Hit되었을 때, Hit.Normal 과 Hit.ImpactNormal에 대한 차이 
	/*
	Normal:
	구체 트레이스 테스트의 경우, 법선 벡터는 "충돌 지점에서 구체의 중심을 향해 안쪽으로 향하는" 벡터입니다. 즉, 충돌 지점에서 구체의 중심으로 들어가는 방향입니다.
	ImpactNormal:
	구체가 평면에 부딪힐 때, 법선 벡터는 "평면으로부터 바깥쪽으로 향하는" 벡터입니다. 즉, 충돌 표면에서 구체 방향으로 나가는 방향입니다.
	*/
	
	// TODO: 어떤 물체랑 부딪혔는지 분기처리 (장애물이면 bounce, 땅이면 구르기 등)
	if (OtherActor->ActorHasTag("Ground"))
	{
		bIsHitGround = true;
		ProjectileComp->StopMovementImmediately();
		ProjectileComp->Deactivate();
		// FLog::Log(TEXT("바닥이랑 닿았다!"));
	}
	
	ARisingWaterProp* Water = Cast<ARisingWaterProp>(OtherActor);
	if (Water)
	{
		// 물에 부딪히면 타이머 초기화
		GetWorld()->GetTimerManager().ClearTimer(PoolTimerHandle);
		ReturnSelf();
		// FLog::Log(TEXT("물에 부딪힘, Clear Timer"));
	}

	/*
	if (OtherActor->ActorHasTag("Water"))
	{
		// 물에 부딪히면 타이머 초기화
		GetWorld()->GetTimerManager().ClearTimer(PoolTimerHandle);
		ReturnSelf();
		FLog::Log(TEXT("물에 부딪힘, Clear Timer"));
	}
	*/
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

	RollingBall();
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
		MeshComp->SetCollisionProfileName(TEXT("RollingBall"));
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
		MeshComp->IgnoreActorWhenMoving(this, true);
	}

	FFastLogger::LogConsole(TEXT("SetActive: %d"), bIsActive);
}

void ARollingBallProp::LaunchProjectile()
{
	LaunchDir = Arrow->GetForwardVector();
	ProjectileComp->Velocity = LaunchDir * ProjectileComp->InitialSpeed;

	// 발사 후 다시 복귀하는 타이밍
	// 바닥에 닿지않으면, 4초후에 복귀하자
	GetWorld()->GetTimerManager().SetTimer(PoolTimerHandle, this, &ARollingBallProp::ReturnSelf,
	                                       4.0f, false);
}

void ARollingBallProp::RollingBall()
{
	if (bIsHitGround)
	{
		FVector MeshPos = GetActorLocation();
		FVector NewPos;

		// TODO: 탱크방식으로 바꾸는게 나을 듯함 (근데 Linetrace 고정해놔야함 (mesh만 구르게)
		// NOTE: 평지에서도, 경사면에서도 작동하는 법
		// HitNormal을 Actor의 새로운 UpVector로 바꾼다 (회전값 생성)
		FRotator NewRot = UKismetMathLibrary::MakeRotFromZY(HitNormal, GetActorRightVector());
		// 새 회전값 적용
		SetActorRotation(NewRot);

		// 이걸 기준으로 액터의 앞 방향을 구해서 적용
		GroundDir = GetActorForwardVector();
		NewPos = MeshPos + GroundDir * (RollingSpeed * GetWorld()->GetDeltaSeconds());
		// 해결: true를 붙여야만 Hit다시감지
		SetActorLocation(NewPos, true);
		
		/* 기존 방식은: 경사면에서만 사용가능
		GroundDir = FVector::CrossProduct(HitNormal, FVector::CrossProduct(GravityDir, HitNormal));
		NewPos = MeshPos + GroundDir * (RollingSpeed * GetWorld()->GetDeltaSeconds());
		MeshComp->SetRelativeLocation(NewPos);*/
	}
}
