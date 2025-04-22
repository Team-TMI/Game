// Fill out your copyright notice in the Description page of Project Settings.


#include "SlipperyProp.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "JumpGame/Characters/Frog.h"


// Sets default values
ASlipperyProp::ASlipperyProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASlipperyProp::BeginPlay()
{
	Super::BeginPlay();

	if (AFrog* Character = Cast<AFrog>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		Frog = Character;
	}
}

void ASlipperyProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Frog")))
	{
		Frog->GetCharacterMovement()->GroundFriction = 0.f;
		Frog->GetCharacterMovement()->BrakingDecelerationWalking = 100.f;
	}
}

void ASlipperyProp::OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TEXT("Frog")))
	{
		Frog->GetCharacterMovement()->GroundFriction = 5.f;
		Frog->GetCharacterMovement()->BrakingDecelerationWalking = 1500.f;
	}
}