// Fill out your copyright notice in the Description page of Project Settings.


#include "MapEditingPawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "TestActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "JumpGame/Core/PlayerController/MapEditingPlayerController.h"
#include "JumpGame/MapEditor/ClickHandlers/ClickHandlerManager.h"
#include "JumpGame/MapEditor/Components/GizmoComponent.h"
#include "JumpGame/MapEditor/Components/GridComponent.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"
#include "JumpGame/Utils/FastLogger.h"

// Sets default values
AMapEditingPawn::AMapEditingPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_MAPEDITING
	(TEXT("/Game/MapEditor/Input/IMC_MapEditInputContext.IMC_MapEditInputContext"));
	if (IMC_MAPEDITING.Succeeded())
	{
		IMC_MapEditing = IMC_MAPEDITING.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_CLICK
	(TEXT("/Game/MapEditor/Input/Actions/IA_Click.IA_Click"));
	if (IA_CLICK.Succeeded())
	{
		IA_Click = IA_CLICK.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_MOVEABLE
	(TEXT("/Game/MapEditor/Input/Actions/IA_Moveable.IA_Moveable"));
	if (IA_MOVEABLE.Succeeded())
	{
		IA_Moveable = IA_MOVEABLE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_MOVE
	(TEXT("/Game/MapEditor/Input/Actions/IA_Move.IA_Move.IA_Move"));
	if (IA_MOVE.Succeeded())
	{
		IA_Move = IA_MOVE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_TURN
	(TEXT("/Game/MapEditor/Input/Actions/IA_Turn.IA_Turn.IA_Turn"));
	if (IA_TURN.Succeeded())
	{
		IA_Turn = IA_TURN.Object;
	}

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponentMapEditing"));
	CollisionComponent->InitSphereRadius(35.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("EditorPawn"));
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->SetShouldUpdatePhysicsVolume(true);
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->bDynamicObstacle = true;

	RootComponent = CollisionComponent;
	
	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("MovementComponentMapEditing"));
	MovementComponent->UpdatedComponent = RootComponent;

	ClickHandlerManager = CreateDefaultSubobject<UClickHandlerManager>(TEXT("ClickHandlerManager"));
}

// Called when the game starts or when spawned
void AMapEditingPawn::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (SubSystem)
		{
			SubSystem->AddMappingContext(IMC_MapEditing, 0);
		}
	}
}

// Called every frame
void AMapEditingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMapEditingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerInput)
	{
		// TODO : ClickHeldHandler 체인 구조 만들기
		PlayerInput->BindAction(IA_Click, ETriggerEvent::Started, this, &AMapEditingPawn::HandleLeftClick);
		// PlayerInput->BindAction(IA_Click, ETriggerEvent::Triggered, this, &AMapEditingPawn::OnClick);
		PlayerInput->BindAction(IA_Moveable, ETriggerEvent::Started, this, &AMapEditingPawn::HandleRightClickStarted);
		PlayerInput->BindAction(IA_Moveable, ETriggerEvent::Completed, this, &AMapEditingPawn::HandleRightClickStarted);
		PlayerInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMapEditingPawn::HandleMove);
		PlayerInput->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AMapEditingPawn::HandleMouseMove);
	}
}

void AMapEditingPawn::HandleLeftClick(const FInputActionValue& InputActionValue)
{
	AMapEditingPlayerController* PC = Cast<AMapEditingPlayerController>(GetController());
	if (!PC) return ;
	
	ClickHandlerManager->HandleClick(PC);
}

void AMapEditingPawn::HandleRightClickStarted(const FInputActionValue& InputActionValue)
{
	// 마우스 오른쪽 클릭을 했을 때만 움직일 수 있음
	bCanMove = !bCanMove;

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;
	
	if (bCanMove)
	{
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}
	else
	{
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeGameAndUI());
	}
}

void AMapEditingPawn::HandleMove(const FInputActionValue& InputActionValue)
{
	if (!bCanMove) return;

	FVector MoveInput = InputActionValue.Get<FVector>();
	
	MoveForward(MoveInput.Y);
	MoveRight(MoveInput.X);
	MoveUp(MoveInput.Z);
}

void AMapEditingPawn::HandleMouseMove(const FInputActionValue& InputActionValue)
{
	if (!bCanMove) return;

	FVector2d TurnInput = InputActionValue.Get<FVector2d>();
	AddControllerYawInput(-TurnInput.X);
	AddControllerPitchInput(TurnInput.Y);
}

void AMapEditingPawn::MoveForward(float Val)
{
	if (FMath::IsNearlyZero(Val)) return;
	if (Controller)
	{
		FRotator const ControlSpaceRot = Controller->GetControlRotation();

		// transform to world space and add it
		AddMovementInput( FRotationMatrix(ControlSpaceRot).GetScaledAxis( EAxis::X ), Val );
	}
}

void AMapEditingPawn::MoveRight(float Val)
{
	if (FMath::IsNearlyZero(Val)) return;
	if (Controller)
	{
		FRotator const ControlSpaceRot = Controller->GetControlRotation();

		// transform to world space and add it
		AddMovementInput( FRotationMatrix(ControlSpaceRot).GetScaledAxis( EAxis::Y ), Val );
	}
}

void AMapEditingPawn::MoveUp(float Val)
{
	if (FMath::IsNearlyZero(Val)) return;
	AddMovementInput(FVector::UpVector, Val);
}
