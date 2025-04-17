// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JumpGame/Utils/CommonUtils.h"
#include "GridComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class JUMPGAME_API UGridComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGridComponent();

	GETTER(float, SnapSize);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void SetSize(FVector NewSize);
	bool MoveActorInGrid(FVector MouseLocation, const FHitResult& HitResult);
	void RotateActorInGrid(FVector Direction);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid", meta=(AllowPrivateAccess="true"))
	class AActor* UpdatedActor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grid", meta=(AllowPrivateAccess="true"))
	float RotateAngle = 90.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Grid", meta=(AllowPrivateAccess="true"))
	float SnapSize = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid", meta=(AllowPrivateAccess="true"))
	FVector Size = FVector(1.f, 1.f, 1.f);

	// 사용하지 않는 변수 : 일단 냅둠
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grid", meta=(AllowPrivateAccess="true"))
	// FVector Offset = Size * SnapSize * 0.5f;
};
