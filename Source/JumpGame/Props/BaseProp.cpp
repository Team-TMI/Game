// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProp.h"

#include "Components/BoxComponent.h"


// Sets default values
ABaseProp::ABaseProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	Tags.Add("Prop");

	PivotScene = CreateDefaultSubobject<USceneComponent>(TEXT("PivotScene"));
	PivotScene->SetupAttachment(RootComponent);
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->SetBoxExtent(FVector(50.f));
	CollisionComp->SetupAttachment(PivotScene);
	CollisionComp->SetCollisionProfileName(TEXT("Prop"));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionComp);
	MeshComp->SetCollisionProfileName(TEXT("Prop"));
}

// Called when the game starts or when spawned
void ABaseProp::BeginPlay()
{
	Super::BeginPlay();
	
	// 동작 동기화
	SetReplicateMovement(true);

	UnSelectedObjectMaterial = MeshComp->CreateAndSetMaterialInstanceDynamic(0);
}

void ABaseProp::SetCollision(bool bEnable)
{
	Super::SetCollision(bEnable);
	
	// 장애물에 추가 메쉬가 있는 경우에 override
	if (bEnable)
	{
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		// 추가 메쉬 설정
		MeshComp->SetMaterial(0, UnSelectedObjectMaterial);
		MeshComp->SetRenderCustomDepth(false);
	}
	else
	{
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 추가 메쉬 설정
		// TODO: Material 불투명하게 바꿔주기
		MeshComp->SetMaterial(0, SelectedObjectMaterial);
		MeshComp->SetRenderCustomDepth(true);
	}
}


// Called every frame
void ABaseProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MaterialChangeOnTick();
}

void ABaseProp::SetPropActive(bool Value)
{
	bIsActive = Value;
}

void ABaseProp::SetPropCheatMode(bool Value)
{
	bCheatMode = Value;
}

void ABaseProp::MaterialChangeOnTick()
{
	Super::MaterialChangeOnTick();
	
	if (bSelected && bIsOnCollision)
	{
		MeshComp->SetMaterial(0, OnCollisionObjectMaterial);
		return ;
	}
	if (bSelected)
	{
		MeshComp->SetMaterial(0, SelectedObjectMaterial);
		return ;
	}
	if (bIsOnCollision)
	{
		MeshComp->SetMaterial(0, OnCollisionObjectMaterial);
		return ;
	}
	MeshComp->SetMaterial(0, UnSelectedObjectMaterial);
}
