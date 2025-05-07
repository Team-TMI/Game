// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimitiveProp.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "JumpGame/MapEditor/Components/GizmoComponent.h"
#include "JumpGame/MapEditor/Components/GizmoPrimaryComponent.h"
#include "JumpGame/MapEditor/Components/GridComponent.h"
#include "JumpGame/MapEditor/CategorySystem/PropStruct.h"
#include "JumpGame/Props/Components/PropDataComponent.h"
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

	PropDataComponent = CreateDefaultSubobject<UPropDataComponent>(TEXT("PropDataComponent"));
	PropDataComponent->SetPropID(TEXT("1002"));
	
	SetSize(DefaultSize);


	// RotateWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("RotateWidgetComponent"));
	// RotateWidgetComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> M_SELECTED_OBJECT_MATERIAL
	(TEXT("/Game/MapEditor/Material/MI_SelectedObject.MI_SelectedObject"));
	if (M_SELECTED_OBJECT_MATERIAL.Succeeded())
	{
		SelectedObjectMaterial = M_SELECTED_OBJECT_MATERIAL.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> M_COLLISION_MATERIAL
	(TEXT("/Game/MapEditor/Material/MI_CollisionObject.MI_CollisionObject"));
	if (M_COLLISION_MATERIAL.Succeeded())
	{
		OnCollisionObjectMaterial = M_COLLISION_MATERIAL.Object;
	}
}

void APrimitiveProp::OnGridPropBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CollisionCount++;
	bIsOnCollision = true;
}

void APrimitiveProp::OnGridPropEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CollisionCount--;
	if (CollisionCount <= 0)
	{
		CollisionCount = 0;
		bIsOnCollision = false;
	}
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
		// TODO: 기즈모 색상 어떻게 하지 어떤 축으로 도는지 모르겠음
		// GizmoArray[i]->ChangeColorByNewAxis(GizmoDirectionArray[i]);
	}

	// 항상 정면 앞에 RotateWidgetComponent이 위치해야 함
	// RotateWidgetComponent->SetWorldLocation()
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

	GridInnerCollision->OnComponentBeginOverlap.AddDynamic(this, &APrimitiveProp::OnGridPropBeginOverlap);
	GridInnerCollision->OnComponentEndOverlap.AddDynamic(this, &APrimitiveProp::OnGridPropEndOverlap);

	if (GetWorld()->GetMapName().Contains(TEXT("MapEditorLevel")))
	{
		GridOuterCollision->SetHiddenInGame(false);
		GridOuterCollision->SetVisibility(true);
	}
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
	GizmoPrimary->SetRenderCustomDepth(true);
	for (auto& Gizmo : GizmoArray)
	{
		Gizmo->SetVisibility(true);
		Gizmo->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
		Gizmo->SetRenderCustomDepth(true);
	}

	this->SetCollision(false);
}

void APrimitiveProp::SetUnSelected()
{
	bSelected = false;

	// Outer Collision을 켜줌
	GridOuterCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);

	GizmoPrimary->SetVisibility(false);
	GizmoPrimary->SetUnSelected();
	GizmoPrimary->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	GizmoPrimary->SetRenderCustomDepth(false);
	for (auto& Gizmo : GizmoArray)
	{
		Gizmo->SetVisibility(false);
		Gizmo->SetUnSelected();
		Gizmo->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		Gizmo->SetRenderCustomDepth(false);
	}

	this->SetCollision(true);
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
	// TODO: 기즈모 색상 어떻게 하지 어떤 축으로 도는지 모르겠음
	// for (auto& Gizmo : GizmoArray)
	// {
	// 	FVector Direction = Gizmo->GetDirection();
	// 	Gizmo->ChangeColorByNewAxis(Direction);
	// }
}
