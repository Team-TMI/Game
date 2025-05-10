// Fill out your copyright notice in the Description page of Project Settings.


#include "SignCautionProp.h"


// Sets default values
ASignCautionProp::ASignCautionProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset
	(TEXT("/Game/Fab/LowPolySeparate/sign_caution.sign_caution"));
	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void ASignCautionProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASignCautionProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

