// Fill out your copyright notice in the Description page of Project Settings.


#include "ConveyorBeltProp.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AConveyorBeltProp::AConveyorBeltProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("ConveyorBelt");
	
	CollisionComp->SetBoxExtent(FVector(102.f, 51.f, 60.f));
	CollisionComp->SetRelativeLocation(FVector(0, 0, 0));
	MeshComp->SetRelativeLocation(FVector(0, 0, -10));
	MeshComp->SetRelativeScale3D(FVector(2, 1, 1));
	
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeLocation(FVector(110, 0, 0));

	// 컨베이어 벨트 콜리전과 overlap
	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));
}

void AConveyorBeltProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	Character = Cast<AFrog>(OtherActor);
	if (Character && !OverlappingFrogs.Contains(Character))
	{
		OverlappingFrogs.Add(Character); // 중복 체크 후 추가
	}
	
	Super::OnMyBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AConveyorBeltProp::OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Character)
	{
		OverlappingFrogs.Remove(Character); // 정확히 그 개체만 제거
	}
	
	Super::OnMyEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

// Called when the game starts or when spawned
void AConveyorBeltProp::BeginPlay()
{
	Super::BeginPlay();

	BeltDir = Arrow->GetForwardVector().GetSafeNormal();
}

// Called every frame
void AConveyorBeltProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 서버에서만 실행
	if (!HasAuthority()) return;

	ConveyorMove();
}

void AConveyorBeltProp::ConveyorMove()
{
	// 유효한 Frog들만 처리
	for (AFrog* Frog : OverlappingFrogs)
	{
		if (IsValid(Frog))
		{
			FVector DeltaPos = BeltSpeed * GetWorld()->DeltaTimeSeconds * BeltDir;
			Frog->AddActorWorldOffset(DeltaPos);
		}
	}
}


