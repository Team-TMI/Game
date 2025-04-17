// Fill out your copyright notice in the Description page of Project Settings.


#include "ConveyorBelt.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "JumpGame/Characters/Frog.h"


// Sets default values
AConveyorBelt::AConveyorBelt()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("ConveyorBelt");
	
	// TODO: 컨베이어 벨트 크기 설정
	CollisionComp->SetBoxExtent(FVector(102, 51, 60));
	CollisionComp->SetRelativeLocation(FVector(0, 0, 0));
	MeshComp->SetRelativeLocation(FVector(0, 0, -10));
	MeshComp->SetRelativeScale3D(FVector(2, 1, 1));
	
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeLocation(FVector(110, 0, 0));
}

void AConveyorBelt::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	Character = Cast<AFrog>(GetWorld()->GetFirstPlayerController()->GetCharacter());
}

void AConveyorBelt::OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Character = nullptr;
}

// Called when the game starts or when spawned
void AConveyorBelt::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AConveyorBelt::OnMyBeginOverlap);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AConveyorBelt::OnMyEndOverlap);
}

// Called every frame
void AConveyorBelt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BeltSpeed = 350.f;
	if (Character)
	{
		FVector DeltaPos = (BeltSpeed * DeltaTime) * Arrow->GetForwardVector();
		Character->AddActorWorldOffset(DeltaPos);
	}
}

