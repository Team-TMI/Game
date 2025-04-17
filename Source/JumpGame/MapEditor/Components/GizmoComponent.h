// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GizmoComponent.generated.h"

/**
 * 
 */
UCLASS()
class JUMPGAME_API UGizmoComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UGizmoComponent();

	UFUNCTION()
	FVector GetDirection() const;
	UFUNCTION()
	void SetSelected();
	UFUNCTION()
	void SetUnSelected();
	
protected:
	UPROPERTY()
	class UStaticMesh* GizmoMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	class UMaterialInstance* GizmoMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	class UMaterialInstance* GizmoSelectedMaterial;

private:
	UPROPERTY()
	bool bSelected = false;
};
