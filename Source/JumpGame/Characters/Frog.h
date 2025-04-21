// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Frog.generated.h"

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

public:
	UFUNCTION(BlueprintCallable)
	void InitFrogState();
	UFUNCTION(BlueprintCallable)
	void SetJumpAvailableBlock(int32 Block);
	void ResetSuperJumpRatio();

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsCrouching;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsSwimming;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CrouchTime{};
	FTimerHandle CrouchTimer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float SuperJumpRatio;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float SuperJumpValue{3.f};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsSuperJump{false};
	
	// 컴포넌트 
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCharacterTrajectoryComponent* TrajectoryComponent;

	// Enum
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterStateEnum CharacterState;

};
