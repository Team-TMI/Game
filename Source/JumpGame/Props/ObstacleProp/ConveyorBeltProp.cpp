// Fill out your copyright notice in the Description page of Project Settings.


#include "ConveyorBeltProp.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "JumpGame/Props/Components/PropDataComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AConveyorBeltProp::AConveyorBeltProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("ConveyorBelt");
	
	CollisionComp->SetBoxExtent(FVector(100.f, 50.f, 70.f));
	CollisionComp->SetRelativeLocation(FVector(0, 0, 0));
	MeshComp->SetRelativeScale3D(FVector(2, 1, 1));
	
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeLocation(FVector(110, 0, 0));

	// 컨베이어 벨트 콜리전과 overlap
	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));

	Super::SetSize(FVector(2, 1, 1));

	PropDataComponent->SetPropID(TEXT("2005"));
}

void AConveyorBeltProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	AFrog* Frog = Cast<AFrog>(OtherActor);
	if (Frog && !OverlappingFrogs.Contains(Frog))
	{
		
		OverlappingFrogs.Add(Frog);
	}
	
	Super::OnMyBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AConveyorBeltProp::OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AFrog* Frog = Cast<AFrog>(OtherActor);
	if (Frog)
	{
	
		OverlappingFrogs.Remove(Frog);
	}
	
	Super::OnMyEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

// Called when the game starts or when spawned
void AConveyorBeltProp::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AConveyorBeltProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 클라 서버 둘다 각자 실행
	BeltDir = Arrow->GetForwardVector().GetSafeNormal();
	ConveyorMove();
}

void AConveyorBeltProp::ConveyorMove()
{
	CopyOverlapFrogs = OverlappingFrogs;
	int index = 0;
	// 유효한 Frog들만 처리
	for (AFrog* Frog : CopyOverlapFrogs)
	{
		if (!IsValid(Frog)) continue;
		
		FVector DeltaPos = BeltSpeed * GetWorld()->DeltaTimeSeconds * BeltDir;
		
		// TODO: 택1
		Frog->AddActorWorldOffset(DeltaPos, true);
		// Frog->SetActorLocation(Frog->GetActorLocation() + DeltaPos);		
	}
}


