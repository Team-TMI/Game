// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildProp.h"

#include "Components/BoxComponent.h"
#include "JumpGame/MapEditor/Components/GizmoComponent.h"


// Sets default values
ABuildProp::ABuildProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));
	MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Block);
	CollisionComp->bReplicatePhysicsToAutonomousProxy = false;
	GridInnerCollision->bReplicatePhysicsToAutonomousProxy = false;
	GridOuterCollision->bReplicatePhysicsToAutonomousProxy = false;
	MeshComp->bReplicatePhysicsToAutonomousProxy = false;
	RotateGizmo->bReplicatePhysicsToAutonomousProxy = false;
	GizmoPrimary->bReplicatePhysicsToAutonomousProxy = false;
	GizmoOne->bReplicatePhysicsToAutonomousProxy = false;
	GizmoTwo->bReplicatePhysicsToAutonomousProxy = false;
	GizmoThree->bReplicatePhysicsToAutonomousProxy = false;
	GizmoFour->bReplicatePhysicsToAutonomousProxy = false;
	GizmoFive->bReplicatePhysicsToAutonomousProxy = false;
	GizmoSix->bReplicatePhysicsToAutonomousProxy = false;
}

// Called when the game starts or when spawned
void ABuildProp::BeginPlay()
{
	Super::BeginPlay();

	bReplicates = false;
	SetReplicateMovement(false);

}

// Called every frame
void ABuildProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//FLog::Log("", bReplicates);
}

