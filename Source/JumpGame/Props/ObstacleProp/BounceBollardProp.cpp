// Fill out your copyright notice in the Description page of Project Settings.


#include "BounceBollardProp.h"

#include "Components/BoxComponent.h"
#include "JumpGame/Characters/Frog.h"


// Sets default values
ABounceBollardProp::ABounceBollardProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("BounceBollard");
	
	CollisionComp->SetRelativeScale3D(FVector(0,0,0));
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(
		TEXT("/Script/Engine.StaticMesh'/Game/Props/SM_ObstacleCylinder.SM_ObstacleCylinder'"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
	}
	MeshComp->SetRelativeLocation(FVector(0, 0, 0));
}

void ABounceBollardProp::OnBollardHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AFrog* Frog = Cast<AFrog>(OtherActor);
	
	CalculateForce(Frog);
}

// Called when the game starts or when spawned
void ABounceBollardProp::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	// 콜리전 설정 (Block)
	MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	MeshComp->OnComponentHit.AddDynamic(this, &ABounceBollardProp::OnBollardHit);
}

// Called every frame
void ABounceBollardProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABounceBollardProp::CalculateForce(AFrog* Character)
{
	FVector CharacterDir = Character->GetVelocity().GetSafeNormal();
	FVector Direction = CharacterDir*(-1);
	DrawDebugLine(GetWorld(),GetActorLocation(),GetActorLocation()+Direction*200,FColor::Red, false, 3.0f, 0, 2.f);
	float Force = 1000;
	
	Super::LaunchCharacter(Character, Direction, Force);
}

