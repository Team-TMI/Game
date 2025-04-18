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
	void OnClick(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void OnMoveable(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void OnMove(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void OnTurn(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void MoveForward(float Val);
	UFUNCTION()
	void MoveRight(float Val);
	UFUNCTION()
	void MoveUp(float Val);

	UFUNCTION()
	void ClearSelection();
	UFUNCTION()
	void ClearGizmo();
	UFUNCTION()
	void SetControlledActor(class APrimitiveProp* NewActor);
	UFUNCTION()
	void UpdateGizmo(class UGizmoComponent* NewGizmo);

	// IMC_MAPEDITING
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_MapEditing = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Click = nullptr;

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
};
