// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Frog.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSuperJumpRatioChanged, float, NewRatio);

UENUM()
enum class ECharacterStateEnum : uint8
{
	None,
	Deep,
	Shallow,
	Surface
};

UCLASS()
class JUMPGAME_API AFrog : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFrog();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void NotifyControllerChanged() override;
	virtual bool CanJumpInternal_Implementation() const override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Input
	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);
	void StartJump();
	void StopJump();
	void StartSprint();
	void StopSprint();
	void StartCrouch();
	void StopCrouch();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_StartJump();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_StartCrouch();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_StopCrouch();
	UFUNCTION(Server, Reliable)
	void ServerRPC_StartSprint();
	UFUNCTION(Server, Reliable)
	void ServerRPC_StopSprint();
public:
	UFUNCTION(BlueprintCallable)
	void InitFrogState();
	UFUNCTION(BlueprintCallable)
	void SetJumpAvailableBlock(int32 Block);
	void ResetSuperJumpRatio();
	void StopMovementAndResetRotation();
	void ResumeMovement();
	bool GetCanMove() const;
	void CameraMissionMode();
	void CameraMovementMode();
	void SetJumpGaugeVisibility(bool bVisibility);
	void SetCrouchEnabled(bool bEnabled);
	UFUNCTION()
	void OnRep_SuperJumpRatio();
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetJumpAvailableBlock(int32 Block);
	// UFUNCTION(Server, Reliable)
	// void ServerRPC_ResetSuperJumpRatio();
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* SprintAction;

	// 일반 변수
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float Pitch;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float Yaw;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool bIsCrouching;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool bIsSwimming;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	float CrouchTime{};
	FTimerHandle CrouchTimer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_SuperJumpRatio)
	float SuperJumpRatio;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float SuperJumpValue{1.6f};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool bIsSuperJump{false};
	bool bCanMove{true};
	bool bCanCrouch{true};
	float PrevVelocityZLength{};
	
	// 델리게이트
public:
	UPROPERTY(BlueprintAssignable)
	FOnSuperJumpRatioChanged OnSuperJumpRatioChanged;

	// 컴포넌트 
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* CameraCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCharacterTrajectoryComponent* TrajectoryComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSubclassOf<class UJumpGaugeUI> JumpGaugeUIClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWidgetComponent* JumpGaugeUIComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMaterial* WaterPostProcessMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPostProcessComponent* WaterPostProcessComponent;
	
	// Enum
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	ECharacterStateEnum CharacterState;

	// 컨베이어 벨트
	UPROPERTY()
	bool IsOverlap = false;

};
