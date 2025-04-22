// Fill out your copyright notice in the Description page of Project Settings.


#include "RisingWaterProp.h"

#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Utils/FastLogger.h"

ARisingWaterProp::ARisingWaterProp()
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> WaterMesh(
		TEXT("/Game/Props/Logic/SM_Water.SM_Water"));
	if (WaterMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(WaterMesh.Object);
		MeshComp->SetRelativeScale3D(FVector(12.f, 12.f, 27.f));
	}

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionComp->SetBoxExtent(FVector(600.f, 600.f, 1300.f));
	CollisionComp->SetCollisionProfileName(TEXT("Water"));
	
	DeepCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DeepCollision"));
	DeepCollision->SetupAttachment(RootComponent);
	DeepCollision->SetBoxExtent(FVector(500.f, 500.f, 700.f));
	DeepCollision->SetRelativeLocation(FVector(0, 0, -20.f));
	DeepCollision->SetCollisionProfileName(TEXT("Water"));

	ShallowCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShallowCollision"));
	ShallowCollision->SetupAttachment(RootComponent);
	ShallowCollision->SetBoxExtent(FVector(500.f, 500.f, 300.f));
	ShallowCollision->SetRelativeLocation(FVector(0, 0, 980.f));
	ShallowCollision->SetCollisionProfileName(TEXT("Water"));

	SurfaceCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SurfaceCollision"));
	SurfaceCollision->SetupAttachment(RootComponent);
	SurfaceCollision->SetBoxExtent(FVector(500.f, 500.f, 10.f));
	SurfaceCollision->SetRelativeLocation(FVector(0, 0, 1290.f));
	SurfaceCollision->SetCollisionProfileName(TEXT("Water"));

	DeadZoneCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DeadZoneCollision"));
	DeadZoneCollision->SetupAttachment(RootComponent);
	DeadZoneCollision->SetBoxExtent(FVector(500.f, 500.f, 100.f));
	DeadZoneCollision->SetRelativeLocation(FVector(0, 0, -820.f));
	DeadZoneCollision->SetCollisionProfileName(TEXT("Water"));

	RespawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RespawnPoint"));
	RespawnPoint->SetupAttachment(RootComponent);
	RespawnPoint->SetRelativeLocation(FVector(0, 0, 480.f));
}

void ARisingWaterProp::BeginPlay()
{
	Super::BeginPlay();

	WaterState = EWaterStateEnum::Rise;

	SetRisingSpeed(50.f);

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ARisingWaterProp::OnBeginOverlap);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ARisingWaterProp::OnEndOverlap);

	DeepCollision->OnComponentBeginOverlap.AddDynamic(this, &ARisingWaterProp::OnBeginDeepOverlap);

	DeadZoneCollision->OnComponentBeginOverlap.AddDynamic(this, &ARisingWaterProp::OnBeginDeadZoneOverlap);

	// Todo: 오버랩 됐을 때 하고, 끝나면 null 초기화
	if (AFrog* Character = Cast<AFrog>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		Frog = Character;
	}
}

void ARisingWaterProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	switch (WaterState)
	{
	case EWaterStateEnum::None:
		break;
	case EWaterStateEnum::Rise:
		RiseWater(DeltaTime);
		break;
	}

	if (!Frog)
	{
		return;
	}

	// 일정 시간이 흘렀는데 상태가 "None"인 경우, 강제로 "Surface"로 수정
	if (!bIsChanged && Frog->bIsSwimming)
	{
		FlowTime += DeltaTime;
		if (FlowTime > JumpTime && Frog->CharacterState == ECharacterStateEnum::None)
		{
			FlowTime = 0.f;
			Frog->CharacterState = ECharacterStateEnum::Surface;
			bIsChanged = true;
		}
	}
	
	UCharacterMovementComponent* MovementComponent{Frog->GetCharacterMovement()};
	if (MovementComponent)
	{
		switch (Frog->CharacterState)
		{
		case ECharacterStateEnum::None:
			// 물 밖 중력 1
			MovementComponent->GravityScale = 1.f;
			break;
		case ECharacterStateEnum::Deep:
			// 물 속 중력 0
			MovementComponent->GravityScale = 0.f;
		// 위쪽으로 강한 힘
			Frog->AddMovementInput(FVector::UpVector, 1000.0f * DeltaTime);
			break;
		case ECharacterStateEnum::Shallow:
			// 물 속 중력 0
			MovementComponent->GravityScale = 0.f;
		// 위쪽으로 약한 힘
			Frog->AddMovementInput(FVector::UpVector, 300.0f * DeltaTime);
			break;
		case ECharacterStateEnum::Surface:
			// 약한 중력 ( 둥둥 떠다니는 느낌 )
			MovementComponent->GravityScale = 0.1f;
		// Z축 속도를 천천히 줄여서 위아래 움직임 댐핑
			MovementComponent->Velocity.Z *= 0.9f;
			break;
		}
	}
}

void ARisingWaterProp::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Frog")))
	{
		Frog->ResetSuperJumpRatio();
		Frog->bIsSwimming = true;
		Frog->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		//FLog::Log("Speed", Frog->GetCharacterMovement()->Velocity.Length());

		// 안빠르면 가라앉지 않게
		if (Frog->GetCharacterMovement()->Velocity.Length() < 1'000.f)
		{
			Frog->CharacterState = ECharacterStateEnum::Surface;
		}

		// 잠시 가라앉고 올라오게
		FTimerDelegate MovementModeDelegate{
			FTimerDelegate::CreateLambda([this]() {
				ShallowCollision->OnComponentBeginOverlap.AddDynamic(this, &ARisingWaterProp::OnBeginShallowOverlap);
				SurfaceCollision->OnComponentBeginOverlap.AddDynamic(this, &ARisingWaterProp::OnBeginSurfaceOverlap);
			})
		};
		GetWorldTimerManager().SetTimer(TimerHandle, MovementModeDelegate, 1.f, false);
	}
}

void ARisingWaterProp::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TEXT("Frog")))
	{
		Frog->ResetSuperJumpRatio();
		GetWorldTimerManager().ClearTimer(TimerHandle);
		Frog->bIsSwimming = false;
		Frog->CharacterState = ECharacterStateEnum::None;
		Frog->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		bIsChanged = false;

		ShallowCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ARisingWaterProp::OnBeginShallowOverlap);
		SurfaceCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ARisingWaterProp::OnBeginSurfaceOverlap);
	}
}

void ARisingWaterProp::OnBeginDeepOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Frog")))
	{
		Frog->CharacterState = ECharacterStateEnum::Deep;
	}
}

void ARisingWaterProp::OnBeginShallowOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                             const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Frog")))
	{
		Frog->CharacterState = ECharacterStateEnum::Shallow;
	}
}

void ARisingWaterProp::OnBeginSurfaceOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                             const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Frog")))
	{
		Frog->CharacterState = ECharacterStateEnum::Surface;
	}
}

void ARisingWaterProp::OnBeginDeadZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                              const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Frog")))
	{
		Frog->CharacterState = ECharacterStateEnum::Deep;
		Frog->SetActorLocation(RespawnPoint->GetComponentLocation());
	}
}

void ARisingWaterProp::SetCollision(bool bEnable)
{
	Super::SetCollision(bEnable);
}

void ARisingWaterProp::RiseWater(float DeltaTime)
{
	float DeltaZ{DeltaTime * RisingSpeed};
	
	if (Frog)
	{
		Frog->SetActorLocation(FVector(Frog->GetActorLocation().X, Frog->GetActorLocation().Y,
	                               Frog->GetActorLocation().Z + DeltaZ));
	}
	
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y,
	                         GetActorLocation().Z + DeltaZ));
}

void ARisingWaterProp::StopRising(float Time)
{
	WaterState = EWaterStateEnum::None;

	if (Time > 0.f)
	{
		FTimerDelegate StopWaterDelegate{
			FTimerDelegate::CreateLambda([this]() {
				WaterState = EWaterStateEnum::Rise;
			})
		};
		GetWorldTimerManager().SetTimer(StopTimerHandle, StopWaterDelegate, Time, false);
	}
}

void ARisingWaterProp::StartRising()
{
	WaterState = EWaterStateEnum::Rise;
}

void ARisingWaterProp::SetRisingSpeed(float Speed)
{
	RisingSpeed = Speed;
}
