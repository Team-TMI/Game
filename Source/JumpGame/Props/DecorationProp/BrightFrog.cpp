// Fill out your copyright notice in the Description page of Project Settings.


#include "BrightFrog.h"

#include "Components/BoxComponent.h"
#include "JumpGame/Props/Components/PropDataComponent.h"


// Sets default values
ABrightFrog::ABrightFrog()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("Decoration");

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(
		TEXT("/Game/Props/Decoration/BrightFrog/SM_BrightFrog.SM_BrightFrog"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
	}

	PropDataComponent->SetPropID(TEXT("4001"));
}

// Called when the game starts or when spawned
void ABrightFrog::BeginPlay()
{
	Super::BeginPlay();

	UnSelectedObjectMaterial2 = MeshComp->CreateAndSetMaterialInstanceDynamic(1);
	UnSelectedObjectMaterial3 = MeshComp->CreateAndSetMaterialInstanceDynamic(2);
}

void ABrightFrog::SetCollision(bool bEnable)
{
	Super::SetCollision(bEnable);

	// 장애물에 추가 메쉬가 있는 경우에 override
	if (bEnable)
	{
		// 추가 메쉬 설정
		MeshComp->SetMaterial(0, UnSelectedObjectMaterial);
		MeshComp->SetMaterial(1, UnSelectedObjectMaterial2);
		MeshComp->SetMaterial(2, UnSelectedObjectMaterial3);
		MeshComp->SetRenderCustomDepth(false);
	}
	else
	{
		// 추가 메쉬 설정
		// TODO: Material 불투명하게 바꿔주기
		MeshComp->SetMaterial(0, SelectedObjectMaterial);
		MeshComp->SetMaterial(1, SelectedObjectMaterial);
		MeshComp->SetMaterial(2, SelectedObjectMaterial);
		MeshComp->SetRenderCustomDepth(true);
	}
}

// Called every frame
void ABrightFrog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MaterialChangeOnTick();
}

void ABrightFrog::MaterialChangeOnTick()
{
	Super::MaterialChangeOnTick();

	if (bSelected && bIsOnCollision)
	{
		MeshComp->SetMaterial(0, OnCollisionObjectMaterial);
		MeshComp->SetMaterial(1, OnCollisionObjectMaterial);
		MeshComp->SetMaterial(2, OnCollisionObjectMaterial);
		return ;
	}
	if (bSelected)
	{
		MeshComp->SetMaterial(0, SelectedObjectMaterial);
		MeshComp->SetMaterial(1, SelectedObjectMaterial);
		MeshComp->SetMaterial(2, SelectedObjectMaterial);
		return ;
	}
	if (bIsOnCollision)
	{
		MeshComp->SetMaterial(0, OnCollisionObjectMaterial);
		MeshComp->SetMaterial(1, OnCollisionObjectMaterial);
		MeshComp->SetMaterial(2, OnCollisionObjectMaterial);
		return ;
	}
	MeshComp->SetMaterial(0, UnSelectedObjectMaterial);
	MeshComp->SetMaterial(1, UnSelectedObjectMaterial2);
	MeshComp->SetMaterial(2, UnSelectedObjectMaterial3);
}
