// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateSpikeProp.h"

#include "ToolContextInterfaces.h"
#include "Components/BoxComponent.h"


// Sets default values
ARotateSpikeProp::ARotateSpikeProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("RotateSpike");

	MeshComp->SetupAttachment(PivotScene);
	
	Hammer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hammer"));
	Hammer->SetupAttachment(MeshComp);
	Hammer->SetRelativeLocation(FVector(0, 160, 0));
	
	CollisionComp->SetBoxExtent(FVector(60.f));
	CollisionComp->SetRelativeLocation(FVector::ZeroVector);
	CollisionComp->SetupAttachment(Hammer);
	
	// TODO: 가시 추가
	Spike = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spike"));
	Spike->SetupAttachment(PivotScene);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempCylinder(TEXT("/Script/Engine.StaticMesh'/Game/Props/SM_ObstacleCylinder.SM_ObstacleCylinder'"));
	if (TempCylinder.Succeeded())
	{
		MeshComp->SetStaticMesh(TempCylinder.Object);
	}

	// 메쉬랑 부딪힘 (CollisionComp랑은 충돌하지 않는다)
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	Hammer->SetCollisionProfileName(TEXT("Prop"));
	Spike->SetCollisionProfileName(TEXT("Prop"));
}

// Called when the game starts or when spawned
void ARotateSpikeProp::BeginPlay()
{
	Super::BeginPlay();

	SwingHammer();
}

// Called every frame
void ARotateSpikeProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARotateSpikeProp::CalculateForce(AFrog* Character)
{
	Super::CalculateForce(Character);
}

void ARotateSpikeProp::SwingHammer_Implementation()
{
}

void ARotateSpikeProp::ReverseSwingHammer_Implementation()
{
}

inline void ARotateSpikeProp::SetCollision(bool bEnable)
{
	Super::SetCollision(bEnable);
	
	if (bEnable)
	{
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Hammer->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Spike->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Hammer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Spike->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// TODO: Material 불투명하게 바꿔주기
	}
}
