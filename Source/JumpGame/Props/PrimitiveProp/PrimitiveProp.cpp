// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimitiveProp.h"

#include "Components/BoxComponent.h"
#include "JumpGame/MapEditor/Components/GizmoComponent.h"
#include "JumpGame/MapEditor/Components/GridComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
APrimitiveProp::APrimitiveProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GridComp = CreateDefaultSubobject<UGridComponent>(TEXT("GridComp"));
	SetRootComponent(GridComp);
	GridComp->SetSize(FVector(1, 1, 1));

	// Collision을 세팅해야 함
	GridOuterCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("GridOuterCollision"));
	GridOuterCollision->SetCollisionProfileName(TEXT("GridOuterPrefet"));
	GridOuterCollision->SetupAttachment(GridComp);

	GridInnerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("GridInnerCollision"));
	GridInnerCollision->SetCollisionProfileName(TEXT("GridInnerPrefet"));
	GridInnerCollision->SetupAttachment(GridComp);


	// Gizmo들고 옴
	GizmoOne = CreateDefaultSubobject<UGizmoComponent>(TEXT("GizmoOne"));
	GizmoArray.Add(GizmoOne);

	GizmoTwo = CreateDefaultSubobject<UGizmoComponent>(TEXT("GizmoTwo"));
	GizmoArray.Add(GizmoTwo);

	GizmoThree = CreateDefaultSubobject<UGizmoComponent>(TEXT("GizmoThree"));
	GizmoArray.Add(GizmoThree);

	GizmoFour = CreateDefaultSubobject<UGizmoComponent>(TEXT("GizmoFour"));
	GizmoArray.Add(GizmoFour);

	GizmoFive = CreateDefaultSubobject<UGizmoComponent>(TEXT("GizmoFive"));
	GizmoArray.Add(GizmoFive);

	GizmoSix = CreateDefaultSubobject<UGizmoComponent>(TEXT("GizmoSix"));
	GizmoArray.Add(GizmoSix);

	for (auto& Gizmo : GizmoArray)
	{
		Gizmo->SetupAttachment(GridComp);
		Gizmo->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		Gizmo->SetVisibility(false);
	}
	
	SetSize(DefaultSize);
}

void APrimitiveProp::SetSize(const FVector& InSize)
{
	GridComp->SetSize(InSize);
	float SnapSize = GridComp->GetSnapSize();

	const FVector BoxExtent = InSize * SnapSize * 0.5f;

	GridOuterCollision->SetBoxExtent(BoxExtent);
	GridInnerCollision->SetBoxExtent(BoxExtent - DefaultCollisionExtentOffset);

	// Gizmo들 위치 설정
	// 총 6개의 Gizmo와 6개의 방향이 존재함
	for (int32 i = 0; i < GizmoArray.Num(); i++)
	{
		SetGizmoLocation(GizmoArray[i], GizmoDirectionArray[i], BoxExtent);
		SetGizmoRotation(GizmoArray[i], GizmoDirectionArray[i]);
	}
}

// Called when the game starts or when spawned
void APrimitiveProp::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APrimitiveProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APrimitiveProp::SetGizmoLocation(class UGizmoComponent* Grid, const FVector& Direction,
	const FVector& InBoxSize)
{
	const float Offset = 10.f;

	const FVector Location = Direction * (InBoxSize + FVector(Offset));
	Grid->SetRelativeLocation(Location);
}

void APrimitiveProp::SetGizmoRotation(class UGizmoComponent* Grid, const FVector& Direction)
{
	const FRotator Rotation = UKismetMathLibrary::MakeRotFromZ(Direction);
	Grid->SetRelativeRotation(Rotation);
}

void APrimitiveProp::SetSelected()
{
	bSelected = true;

	// Outer Collision을 꺼줌
	GridOuterCollision->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	for (auto& Gizmo : GizmoArray)
	{
		Gizmo->SetVisibility(true);
		Gizmo->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	}
}

void APrimitiveProp::SetUnSelected()
{
	bSelected = false;

	// Outer Collision을 켜줌
	GridOuterCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	
	for (auto& Gizmo : GizmoArray)
	{
		Gizmo->SetVisibility(false);
		Gizmo->SetUnSelected();
		Gizmo->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	}
}

void APrimitiveProp::SetCollision(bool bCond)
{
	if (bCond)
	{
		GridOuterCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	}
	else
	{
		GridOuterCollision->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	}
}
