// Fill out your copyright notice in the Description page of Project Settings.


#include "Frog.h"

#include "CharacterTrajectoryComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AFrog::AFrog()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> FrogMesh
		(TEXT("/Game/Characters/Fat_Frog/SM_Frog17.SM_Frog17"));
	if (FrogMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(FrogMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -75), FRotator(0, -90, 0));
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> FrogABP
		(TEXT("/Game/Characters/ABP_Frog.ABP_Frog_C"));
	if (FrogABP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(FrogABP.Class);
	}

	ConstructorHelpers::FObjectFinder<UInputMappingContext> FrogIMC
		(TEXT("/Game/Characters/Input/IMC_Frog.IMC_Frog"));
	if (FrogIMC.Succeeded())
	{
		DefaultMappingContext = FrogIMC.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> Frog_Move
		(TEXT("/Game/Characters/Input/IA_FrogMove.IA_FrogMove"));
	if (Frog_Move.Succeeded())
	{
		MoveAction = Frog_Move.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> Frog_Jump
		(TEXT("/Game/Characters/Input/IA_FrogJump.IA_FrogJump"));
	if (Frog_Jump.Succeeded())
	{
		JumpAction = Frog_Jump.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> Frog_Look
		(TEXT("/Game/Characters/Input/IA_FrogLook.IA_FrogLook"));
	if (Frog_Look.Succeeded())
	{
		LookAction = Frog_Look.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> Frog_Crouch
		(TEXT("/Game/Characters/Input/IA_FrogCrouch.IA_FrogCrouch"));
	if (Frog_Crouch.Succeeded())
	{
		CrouchAction = Frog_Crouch.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UInputAction> Frog_Sprint
		(TEXT("/Game/Characters/Input/IA_FrogSprint.IA_FrogSprint"));
	if (Frog_Sprint.Succeeded())
	{
		SprintAction = Frog_Sprint.Object;
	}

	// CapsuleComponent Settings
	GetCapsuleComponent()->InitCapsuleSize(43.f, 70.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// CharacterMovement Settings
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxAcceleration = 800.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	GetCharacterMovement()->SetCrouchedHalfHeight(60.f);
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->GroundFriction = 5.0f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 150.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1500.0f;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.25f;
	GetCharacterMovement()->PerchRadiusThreshold = 20.0f;
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 800.0f, 0.0f);
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// MotionMatching
	TrajectoryComponent = CreateDefaultSubobject<UCharacterTrajectoryComponent>(TEXT("TrajectoryComponent"));

	// 초기값 설정
	bIsSwimming = false;
	CharacterState = ECharacterStateEnum::None;
}

void AFrog::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}


// Called when the game starts or when spawned
void AFrog::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(TEXT("Frog"));
}

// Called every frame
void AFrog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	}
	else
	{
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 800.0f, 0.0f);
	}
}

// Called to bind functionality to input
void AFrog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AFrog::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFrog::StopJumping);
		
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AFrog::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AFrog::StopCrouch);
		
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AFrog::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AFrog::StopSprint);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFrog::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFrog::Look);
	}
}


void AFrog::Move(const struct FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AFrog::Look(const struct FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFrog::StartJump()
{
	// 수면 점프
	if (CharacterState == ECharacterStateEnum::Surface)
	{
		ACharacter::LaunchCharacter(FVector(0, 0, 500.0f), true, true);
		return;
	}
	
	Jump();
}

void AFrog::StopJump()
{
	StopJumping();
}

void AFrog::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AFrog::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void AFrog::StartCrouch()
{
	bIsCrouching = true;
	GetCharacterMovement()->MaxWalkSpeed = 150.f;
	Crouch();
}

void AFrog::StopCrouch()
{
	bIsCrouching = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	UnCrouch();
}

void AFrog::StartSwim()
{
	FLog::Log("StartSwim");
	bIsSwimming = true;
}

void AFrog::StopSwim()
{
	FLog::Log("StopSwim");
	bIsSwimming = false;
}
