// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "MapEditingPawn.generated.h"

UCLASS()
class JUMPGAME_API AMapEditingPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMapEditingPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	GETTER(class APrimitiveProp*, ControlledActor);

private:
	UFUNCTION()
	void HandleLeftClick(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void HandleLeftPressedStarted(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void HandleLeftPressed(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void HandleLeftPressedCompleted(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void HandleRightClickStarted(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void HandleMove(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void HandleMouseMove(const FInputActionValue& InputActionValue);

	// UPROPERTY()가 안됨
	FVector2d TurnInput;

	UFUNCTION()
	void MoveForward(float Val);
	UFUNCTION()
	void MoveRight(float Val);
	UFUNCTION()
	void MoveUp(float Val);

	// IMC_MAPEDITING
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_MapEditing = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Click = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Pressed = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Moveable = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Turn = nullptr;

	UPROPERTY()
	bool bCanMove = false;

	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPawnMovementComponent* MovementComponent = nullptr;
	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionComponent = nullptr;

	UPROPERTY(Category = Pawn, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class APrimitiveProp* ControlledActor = nullptr;
	UPROPERTY(Category = Pawn, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UGizmoComponent* CachedGizmo = nullptr;

	UPROPERTY(Category = Pawn, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UClickHandlerManager* ClickHandlerManager = nullptr;
	UPROPERTY(Category = Pawn, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPressedHandlerManager* PressedHandlerManager = nullptr;
};
