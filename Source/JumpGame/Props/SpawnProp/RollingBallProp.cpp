// Fill out your copyright notice in the Description page of Project Settings.


#include "RollingBallProp.h"

#include "ObjectPoolComponent.h"
#include "Components/ArrowComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Utils/FastLogger.h"


// Sets default values
ARollingBallProp::ARollingBallProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Tags.Add("RollingBall");

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	PivotScene = CreateDefaultSubobject<USceneComponent>(TEXT("PivotScene"));
	PivotScene->SetupAttachment(RootComponent);
	
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Dir"));
	Arrow->SetupAttachment(RootComponent);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetRelativeLocation(FVector(-30, 0, 0));
	BoxComp->SetBoxExtent(FVector(35));

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSphere(
		TEXT("/Script/Engine.StaticMesh'/Game/Fab/RollingBall/SM_Acorn.SM_Acorn'"));
	if (TempSphere.Succeeded())
	{
		MeshComp->SetStaticMesh(TempSphere.Object);
	}
	MeshComp->SetRelativeScale3D(FVector(1.f));
	MeshComp->SetCollisionProfileName(TEXT("OverlapProp"));
}

void ARollingBallProp::OnMyRollingBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                                          const FHitResult& Hit)
{
	/*FVector HitLocation = Hit.Location;
	FLog::Log("OnMyRollingBallHit");
	
	AFrog* Frog = Cast<AFrog>(OtherActor);
	if (Frog)
	{
		FVector LaunchVelocity = LaunchDir.GetSafeNormal() * 100;
		Frog->LaunchCharacter(LaunchVelocity, false, false);
		FLog::Log("OnMyRollingBallHit Frog");
	}

	// 타이머 초기화
	GetWorld()->GetTimerManager().ClearTimer(PoolTimerHandle);
	ReturnSelf();
	
	if (HasAuthority())
	{
		MulticastRPC_PlayEffect(HitLocation);
	}*/
}

void ARollingBallProp::OnMyRollingBallOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	FVector HitLocation = SweepResult.Location;
	FLog::Log("OnMyRollingBallOverlap");
	
	AFrog* Frog = Cast<AFrog>(OtherActor);
	if (Frog)
	{
		FVector LaunchVelocity = LaunchDir.GetSafeNormal() * 500;
		Frog->LaunchCharacter(LaunchVelocity, true, true);
		FLog::Log("OnMyRollingBallOverlap Frog");
	}

	// 타이머 초기화
	// GetWorld()->GetTimerManager().ClearTimer(PoolTimerHandle);
	ReturnSelf();
	
	if (HasAuthority())
	{
		MulticastRPC_PlayEffect(HitLocation);
	}
}

// Called when the game starts or when spawned
void ARollingBallProp::BeginPlay()
{
	Super::BeginPlay();

	// 동작 동기화
	SetReplicateMovement(true);

	// MeshComp->OnComponentHit.AddDynamic(this, &ARollingBallProp::OnMyRollingBallHit);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ARollingBallProp::OnMyRollingBallOverlap);
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

	if (HasAuthority())
	{
		// 오브젝트 풀에 스스로를 반환하고 비활성화
		SetActive(false);
		ObjectPool->ReturnObject();
	}
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
}

void ARollingBallProp::LaunchProjectile()
{
	LaunchDir = Arrow->GetForwardVector();

	// 발사 후 다시 복귀하는 타이밍
	// 바닥에 닿지않으면, 4초후에 복귀하자
	GetWorld()->GetTimerManager().SetTimer(PoolTimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		this->ReturnSelf();
	}), 4.0f, false);

	if (HasAuthority())
	{
		// 있던 자리에서 터지기
		MulticastRPC_PlayEffect(GetActorLocation());
	}
}

void ARollingBallProp::RollingBall()
{
	FVector MeshPos = GetActorLocation();
	
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FVector Movement = LaunchDir * (RollingSpeed * DeltaTime);
	
	SetActorLocation(MeshPos + Movement, true);

	// 회전하기
	float Distance = Movement.Size();
	// 반지름이 작아질수록 회전 속도가 빨라짐
	float Radius = 50.0f; 
	float RotationDegrees = FMath::RadiansToDegrees(Distance / Radius);
	
	FVector RotationAxis = FVector::CrossProduct(LaunchDir.GetSafeNormal(), FVector::UpVector);
	FQuat DeltaQuat = FQuat(RotationAxis, FMath::DegreesToRadians(RotationDegrees));
	DrawDebugCoordinateSystem(GetWorld(), GetActorLocation(), GetActorRotation(), 100.f, false, -1.f, 0, 2.f);

	// 회전 적용
	MeshComp->AddLocalRotation(DeltaQuat, false, nullptr, ETeleportType::None);
}

void ARollingBallProp::MulticastRPC_PlayEffect_Implementation(FVector Location)
{
	// 클라이언트 전부에서 호출됨 (서버 포함)
	// UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Location);
	// UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Location);
}

