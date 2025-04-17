// Fill out your copyright notice in the Description page of Project Settings.


#include "RisingWaterProp.h"

#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Utils/FastLogger.h"

ARisingWaterProp::ARisingWaterProp()
{
	CollisionComp->SetBoxExtent(FVector(600.f, 600.f, 1300.f));
	CollisionComp->SetCollisionProfileName(TEXT("Water"));

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DeepCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DeepCollision"));
	DeepCollision->SetupAttachment(RootComponent);
	DeepCollision->SetBoxExtent(FVector(500.f, 500.f, 500.f));
	DeepCollision->SetRelativeLocation(FVector(0, 0, 170.f));
	DeepCollision->SetCollisionProfileName(TEXT("Water"));

	ShallowCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShallowCollision"));
	ShallowCollision->SetupAttachment(RootComponent);
	ShallowCollision->SetBoxExtent(FVector(500.f, 500.f, 300.f));
	ShallowCollision->SetRelativeLocation(FVector(0, 0, 970.f));
	ShallowCollision->SetCollisionProfileName(TEXT("Water"));

	SurfaceCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SurfaceCollision"));
	SurfaceCollision->SetupAttachment(RootComponent);
	SurfaceCollision->SetBoxExtent(FVector(500.f, 500.f, 10.f));
	SurfaceCollision->SetRelativeLocation(FVector(0, 0, 1280.f));
	SurfaceCollision->SetCollisionProfileName(TEXT("Water"));

	DeadZoneCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DeadZoneCollision"));
	DeadZoneCollision->SetupAttachment(RootComponent);
	DeadZoneCollision->SetBoxExtent(FVector(500.f, 500.f, 100.f));
	DeadZoneCollision->SetRelativeLocation(FVector(0, 0, -430.f));
	DeadZoneCollision->SetCollisionProfileName(TEXT("Water"));

	RespawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RespawnPoint"));
	RespawnPoint->SetupAttachment(RootComponent);
	RespawnPoint->SetRelativeLocation(FVector(0, 0, 970.f));
}

void ARisingWaterProp::BeginPlay()
{
	Super::BeginPlay();

	WaterState = EWaterStateEnum::Rise;
	
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ARisingWaterProp::OnBeginOverlap);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ARisingWaterProp::OnEndOverlap);

	DeepCollision->OnComponentBeginOverlap.AddDynamic(this, &ARisingWaterProp::OnBeginDeepOverlap);

	DeadZoneCollision->OnComponentBeginOverlap.AddDynamic(this, &ARisingWaterProp::OnBeginDeadZoneOverlap);

	Frog = Cast<AFrog>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void ARisingWaterProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FLog::Log("Character State", static_cast<float>(Frog->CharacterState));

	switch (WaterState)
	{
	case EWaterStateEnum::None:
		break;
	case EWaterStateEnum::Rise:
		RiseWater(DeltaTime);
		break;
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
		Frog->bIsSwimming = true;
		Frog->GetCharacterMovement()->SetMovementMode(MOVE_Flying);

		// 잠시 가라앉고 올라오게
		FTimerHandle TimerHandle;
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
		Frog->bIsSwimming = false;
		Frog->CharacterState = ECharacterStateEnum::None;
		Frog->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

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
		Frog->CharacterState = ECharacterStateEnum::Shallow;
		Frog->SetActorLocation(RespawnPoint->GetComponentLocation());
	}
}

void ARisingWaterProp::RiseWater(float DeltaTime)
{
	float DeltaZ{DeltaTime * 50.f};

	Frog->SetActorLocation(FVector(Frog->GetActorLocation().X, Frog->GetActorLocation().Y,
	                               Frog->GetActorLocation().Z + DeltaZ));
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y,
							   GetActorLocation().Z + DeltaZ));
}
