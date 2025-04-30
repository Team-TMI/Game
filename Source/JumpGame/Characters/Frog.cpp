// Fill out your copyright notice in the Description page of Project Settings.


#include "Frog.h"

#include "CharacterTrajectoryComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "JumpGame/UI/Character/JumpGaugeUI.h"
#include "JumpGame/Utils/FastLogger.h"
#include "Net/UnrealNetwork.h"


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

	ConstructorHelpers::FClassFinder<UJumpGaugeUI> JumpGaugeUIWidget
		(TEXT("/Game/UI/Character/WBP_JumpGauge.WBP_JumpGauge_C"));
	if (JumpGaugeUIWidget.Succeeded())
	{
		JumpGaugeUIClass = JumpGaugeUIWidget.Class;
		JumpGaugeUIComponent = CreateDefaultSubobject<UWidgetComponent>(
			TEXT("JumpGaugeUIComponent"));
		JumpGaugeUIComponent->SetupAttachment(GetRootComponent());
		JumpGaugeUIComponent->SetRelativeLocation(FVector(0, 0.f, 0));
		JumpGaugeUIComponent->SetWidgetClass(JumpGaugeUIClass);
		JumpGaugeUIComponent->SetWidgetSpace(EWidgetSpace::Screen);
		JumpGaugeUIComponent->SetDrawSize(FVector2D(240, 200));
		JumpGaugeUIComponent->SetPivot(FVector2D(1.0, 0.5));
	}
	ConstructorHelpers::FObjectFinder<UMaterial> WaterPostProcessFinder
		(TEXT("/Game/PostProcess/MPP_InWater.MPP_InWater"));
	if (WaterPostProcessFinder.Succeeded())
	{
		WaterPostProcessMaterial = WaterPostProcessFinder.Object;
		WaterPostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(
			TEXT("WaterPostProcessComponent"));
		WaterPostProcessComponent->Settings.AddBlendable(WaterPostProcessMaterial, 0.5);
		WaterPostProcessComponent->SetupAttachment(GetRootComponent());
		WaterPostProcessComponent->bEnabled = false;
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
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 150.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1500.0f;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
	GetCharacterMovement()->AirControl = 1.f;
	GetCharacterMovement()->PerchRadiusThreshold = 20.0f;
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 800.0f, 0.0f);
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 150.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	CameraCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CameraCollision"));
	CameraCollision->SetupAttachment(FollowCamera);
	CameraCollision->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	CameraCollision->SetRelativeLocation(FVector(0, 0, 0.f));
	CameraCollision->SetCollisionProfileName(TEXT("CameraCollision"));
	CameraCollision->ComponentTags.Add(TEXT("CameraCollision"));

	// MotionMatching
	TrajectoryComponent = CreateDefaultSubobject<UCharacterTrajectoryComponent>(
		TEXT("TrajectoryComponent"));

	// 초기값 설정
	bIsSwimming = false;
	CharacterState = ECharacterStateEnum::None;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("FrogCollision"));
	GetCapsuleComponent()->CanCharacterStepUpOn = ECB_Yes;

	GetCapsuleComponent()->ComponentTags.Add(TEXT("FrogCapsule"));
	Tags.Add(TEXT("Frog"));

	// 동기화 좀 더 빨라지게
	SetNetUpdateFrequency(200.f);
}

void AFrog::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PlayerController{Cast<APlayerController>(Controller)})
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

	InitFrogState();
}

// Called every frame
void AFrog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//FLog::Log("Speed", GetCharacterMovement()->MaxWalkSpeed);

	// 공중에 있을 때는 회전 잘 안되게
	if (GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	}
	else
	{
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 800.0f, 0.0f);
	}

	PrevVelocityZLength = GetVelocity().Z * -1;

	if (HasAuthority())
	{
		// Pitch, Yaw 계산
		// GetBaseAimRotation()는 서버 카메라의 회전값 사용하므로 클라와 서버의 카메라 방향이 다르면 문제가 생김
		// --> 상대값으로 해야함 ( meshRotation - baseRotation )
		const FRotator MeshRotation = GetMesh()->GetComponentRotation();
		const FRotator AimRotation = GetBaseAimRotation();
		// Pitch: 메시 기준 상대값 계산
		Pitch = (AimRotation - MeshRotation).GetNormalized().Pitch;

		// 상대 Yaw
		const float CharacterYaw{static_cast<float>(GetActorRotation().Yaw)};
		// 절대 Yaw
		const float AimYaw{static_cast<float>(AimRotation.Yaw)};
		// AimYaw - CharacterYaw
		const float RelativeYaw{FMath::FindDeltaAngleDegrees(CharacterYaw, AimYaw)};

		//카메라로 캐릭터 정면 볼 때 고려
		if (RelativeYaw >= 90.f)
		{
			Yaw = 90.f - (RelativeYaw - 90.f);
		}
		else if (RelativeYaw <= -90.f)
		{
			Yaw = -90 - (RelativeYaw + 90.f);
		}
		else
		{
			Yaw = RelativeYaw;
		}
	}
}

// Called to bind functionality to input
void AFrog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(
		PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this,
		                                   &AFrog::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this,
		                                   &AFrog::StopJumping);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this,
		                                   &AFrog::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this,
		                                   &AFrog::StopCrouch);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this,
		                                   &AFrog::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this,
		                                   &AFrog::StopSprint);

		EnhancedInputComponent->
			BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFrog::Move);
		EnhancedInputComponent->
			BindAction(LookAction, ETriggerEvent::Triggered, this, &AFrog::Look);
	}
}


void AFrog::Move(const struct FInputActionValue& Value)
{
	FVector2D MovementVector{Value.Get<FVector2D>()};

	if (Controller && GetCanMove())
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

	if (Controller && GetCanMove())
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// 웅크리면서 점프 가능하게
bool AFrog::CanJumpInternal_Implementation() const
{
	// 원래 점프 로직
	bool bCanJump{Super::CanJumpInternal_Implementation()};

	// 앉은 상태라면 ?
	if (!bCanJump && GetCharacterMovement()->IsCrouching())
	{
		// 현재 지면에 서 있는지
		// 더 점프할 수 있는지
		// 공중이 아닌지
		bCanJump = GetCharacterMovement()->IsMovingOnGround()
			&& JumpCurrentCount < JumpMaxCount
			&& !GetCharacterMovement()->IsFalling();
	}

	return bCanJump;
}

void AFrog::StartJump()
{
	MulticastRPC_StartJump();
}

void AFrog::StopJump()
{
	StopJumping();
}

void AFrog::StartSprint()
{
	ServerRPC_StartSprint();
}

void AFrog::StopSprint()
{
	ServerRPC_StopSprint();
}

void AFrog::SetCrouchEnabled(bool bEnabled)
{
	bCanCrouch = bEnabled;

	if (bCanCrouch)
	{
		SetJumpGaugeVisibility(true);
	}
	else
	{
		SetJumpGaugeVisibility(false);
	}
}

void AFrog::StartCrouch()
{
	MulticastRPC_StartCrouch();
}

void AFrog::StopCrouch()
{
	MulticastRPC_StopCrouch();
}

void AFrog::MulticastRPC_StartJump_Implementation()
{
	// 수면 점프
	if (CharacterState == ECharacterStateEnum::Surface)
	{
		// 1초 동안 물에 충돌 없앰
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Jumping"));
		FVector LaunchVelocity{GetActorForwardVector() * 100.f + FVector::UpVector * 1000.f};
		ACharacter::LaunchCharacter(LaunchVelocity, true, true);

		FTimerHandle TimerHandle;
		FTimerDelegate MovementModeDelegate{
			FTimerDelegate::CreateLambda([this]() {
				GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
			})
		};
		GetWorldTimerManager().SetTimer(TimerHandle, MovementModeDelegate, 1.f, false);

		return;
	}

	// 슈퍼 점프
	if (GetCharacterMovement()->IsCrouching())
	{
		FLog::Log("Time", CrouchTime);
		if (bIsSuperJump)
		{
			FLog::Log("SuperJump");
			SetJumpAvailableBlock(3);
		}
		else if (SuperJumpRatio >= 0.5f)
		{
			FLog::Log("LittleJump");
			SetJumpAvailableBlock(2);
		}

		StopCrouch();

		if (CanJump())
		{
			Jump();
		}

		return;
	}

	if (CanJump())
	{
		Jump();
	}
}

void AFrog::MulticastRPC_StartCrouch_Implementation()
{
	if (!bCanCrouch)
	{
		return;
	}

	// 공중에 있거나 수영 중이면 리턴
	if (GetCharacterMovement()->IsFalling() || bIsSwimming)
	{
		return;
	}

	bIsCrouching = true;
	Crouch();

	// 슈퍼 점프 게이지 충전
	FTimerDelegate CrouchDelegate{
		FTimerDelegate::CreateLambda([this]() {
			CrouchTime += GetWorld()->GetDeltaSeconds();

			if (CrouchTime >= SuperJumpValue)
			{
				bIsSuperJump = true;
			}

			SuperJumpRatio = FMath::Clamp(CrouchTime / SuperJumpValue, 0.f, 1.f);
			OnSuperJumpRatioChanged.Broadcast(SuperJumpRatio);

			//FLog::Log("Ratio", SuperJumpRatio);
		})
	};
	GetWorldTimerManager().SetTimer(CrouchTimer, CrouchDelegate, GetWorld()->GetDeltaSeconds(),
	                                true);
}

void AFrog::MulticastRPC_StopCrouch_Implementation()
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	bIsCrouching = false;
	UnCrouch();

	GetWorldTimerManager().ClearTimer(CrouchTimer);
	CrouchTime = 0.f;

	ResetSuperJumpRatio();
}

void AFrog::ServerRPC_StartSprint_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AFrog::ServerRPC_StopSprint_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void AFrog::InitFrogState()
{
	SetJumpAvailableBlock(1);
	ResetSuperJumpRatio();

	// 로컬 클라만 점프 게이지 보이게
	if (IsLocallyControlled())
	{
		SetJumpGaugeVisibility(true);
	}
	else
	{
		// 다른 클라에서 삭제
		JumpGaugeUIComponent->DestroyComponent();
	}
}

void AFrog::SetJumpAvailableBlock(int32 Block)
{
	float Height{Block * 100.f + 10.f};
	float Value{FMath::Sqrt(Height * FMath::Abs(GetCharacterMovement()->GetGravityZ()) * 2)};
	GetCharacterMovement()->JumpZVelocity = Value;

	// 점프력 높게 설정하면 다시 원래대로 점프력 돌아오게
	if (Block != 1)
	{
		FTimerHandle TimerHandle;
		FTimerDelegate JumpDelegate{
			FTimerDelegate::CreateLambda([this]() {
				SetJumpAvailableBlock(1);
			})
		};
		GetWorldTimerManager().SetTimer(TimerHandle, JumpDelegate, 0.2f, false);
	}
	ServerRPC_SetJumpAvailableBlock(Block);
}

void AFrog::ResetSuperJumpRatio()
{
	SuperJumpRatio = 0.f;
	OnSuperJumpRatioChanged.Broadcast(0.f);

	bIsSuperJump = false;
}

void AFrog::StopMovementAndResetRotation()
{
	bCanMove = false;
	SetActorRotation(FRotator::ZeroRotator);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
}

void AFrog::ResumeMovement()
{
	bCanMove = true;
}

bool AFrog::GetCanMove() const
{
	return bCanMove;
}

void AFrog::CameraMissionMode()
{
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->SetRelativeLocationAndRotation(FVector(0, 0, 150.f), FRotator(-20.f, 0, 0));
	CameraBoom->TargetArmLength = 1'000.f;
}

void AFrog::CameraMovementMode()
{
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	CameraBoom->TargetArmLength = 400.f;
}

void AFrog::SetJumpGaugeVisibility(bool bVisibility)
{
	JumpGaugeUIComponent->SetVisibility(bVisibility);
}

void AFrog::OnRep_SuperJumpRatio()
{
	if (IsLocallyControlled())
	{
		SuperJumpRatio = FMath::Clamp(CrouchTime / SuperJumpValue, 0.f, 1.f);
		OnSuperJumpRatioChanged.Broadcast(SuperJumpRatio);
	}
}

void AFrog::ServerRPC_SetJumpAvailableBlock_Implementation(int32 Block)
{
	float Height{Block * 100.f + 10.f};
	float Value{FMath::Sqrt(Height * FMath::Abs(GetCharacterMovement()->GetGravityZ()) * 2)};
	GetCharacterMovement()->JumpZVelocity = Value;

	// 점프력 높게 설정하면 다시 원래대로 점프력 돌아오게
	if (Block != 1)
	{
		FTimerHandle TimerHandle;
		FTimerDelegate JumpDelegate{
			FTimerDelegate::CreateLambda([this]() {
				SetJumpAvailableBlock(1);
			})
		};
		GetWorldTimerManager().SetTimer(TimerHandle, JumpDelegate, 0.2f, false);
	}
}

void AFrog::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFrog, Pitch);
	DOREPLIFETIME(AFrog, Yaw);
	DOREPLIFETIME(AFrog, bIsCrouching);
	DOREPLIFETIME(AFrog, bIsSwimming);
	DOREPLIFETIME(AFrog, bIsSuperJump);
	DOREPLIFETIME(AFrog, CrouchTime);
	DOREPLIFETIME(AFrog, CharacterState);
	// COND_SkipOwner : 소유자 클라이언트는 업데이트 안해서 중복 실행 방지
	DOREPLIFETIME_CONDITION(AFrog, SuperJumpRatio, COND_SkipOwner);
}
