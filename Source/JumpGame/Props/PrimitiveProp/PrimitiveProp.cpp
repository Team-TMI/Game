// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimitiveProp.h"

#include "Components/BoxComponent.h"
#include "JumpGame/MapEditor/Components/GizmoComponent.h"
#include "JumpGame/MapEditor/Components/GizmoPrimaryComponent.h"
#include "JumpGame/MapEditor/Components/GridComponent.h"
#include "JumpGame/MapEditor/DataTable/PropStruct.h"
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

	GizmoPrimary = CreateDefaultSubobject<UGizmoPrimaryComponent>(TEXT("GizmoPrimary"));
	GizmoPrimary->SetupAttachment(GridComp);
	GizmoPrimary->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	GizmoPrimary->SetVisibility(false);
	
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

	ConstructorHelpers::FObjectFinder<UDataTable> DT_PROPDATA
	(TEXT("/Game/MapEditor/DataTable/DT_PropData.DT_PropData"));
	PropID.DataTable = DT_PROPDATA.Object;
	
	SetSize(DefaultSize);
}

void APrimitiveProp::SetPropID(const FName& InPropID)
{
	PropID.RowName = InPropID;
	// FPropStruct* Data = PropID.DataTable->FindRow<FPropStruct>(PropID.RowName, TEXT(""), true);
}

void APrimitiveProp::SetSize(const FVector& InSize)
{
	GridComp->SetSize(InSize);
	float SnapSize = GridComp->GetSnapSize();

	// const FVector BoxExtent = InSize * SnapSize * 0.5f;
	
	// 박스의 크기는 SnapSize의 2의 배수이다.
	const FVector BoxExtent = InSize * SnapSize;

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

void APrimitiveProp::SetNewSizeByRotation(const FVector& InSize)
{
	float SnapSize = GridComp->GetSnapSize();
	
	const FVector BoxExtent = InSize * SnapSize;

	GridOuterCollision->SetBoxExtent(BoxExtent);
	GridInnerCollision->SetBoxExtent(BoxExtent - DefaultCollisionExtentOffset);

	RotateAllGizmos();
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

void APrimitiveProp::SetGizmoLocation(class UGizmoComponent* Gizmo, const FVector& Direction,
	const FVector& InBoxSize)
{
	const float Offset = 10.f;

	const FVector Location = Direction * (InBoxSize + FVector(Offset));
	Gizmo->SetRelativeLocation(Location);
}

void APrimitiveProp::SetGizmoRotation(class UGizmoComponent* Gizmo, const FVector& Direction)
{
	const FRotator Rotation = UKismetMathLibrary::MakeRotFromZ(Direction);
	Gizmo->SetWorldRotation(Rotation);
}

void APrimitiveProp::SetSelected()
{
	bSelected = true;

	// Outer Collision을 꺼줌
	GridOuterCollision->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	GizmoPrimary->SetVisibility(true);
	GizmoPrimary->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
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

	GizmoPrimary->SetVisibility(false);
	GizmoPrimary->SetUnSelected();
	GizmoPrimary->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	for (auto& Gizmo : GizmoArray)
	{
		Gizmo->SetVisibility(false);
		Gizmo->SetUnSelected();
		Gizmo->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	}
}

void APrimitiveProp::SetPrimitivePropCollision(bool bCond)
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


void APrimitiveProp::SetGizmosCollision(bool bCond)
{
	if (bCond)
	{
		GizmoPrimary->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
		for (auto& Gizmo : GizmoArray)
		{
			Gizmo->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
		}
	}
	else
	{
		GizmoPrimary->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		for (auto& Gizmo : GizmoArray)
		{
			Gizmo->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		}
	}
}

void APrimitiveProp::RotateAllGizmos()
{
	for (auto& Gizmo : GizmoArray)
	{
		FVector Direction = Gizmo->GetDirection();
		SetGizmoRotation(Gizmo, Direction);
	}
}
