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

UENUM()
enum class EEmotionState
{
	None,
	WaitingForInput,
	PlayingEmotion,
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

	void FrogSkinFinder();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void NotifyControllerChanged() override;
	virtual bool CanJumpInternal_Implementation() const override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// 카메라 물 오버랩 감지 후 포스트프로세스
	UFUNCTION()
	void OnCameraBeginOverlapWater(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                               const FHitResult& SweepResult);
	UFUNCTION()
	void OnCameraEndOverlapWater(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnTongueBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								   const FHitResult& SweepResult);
	
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
	void TongueAttack();
	void TongueAttackEnd();
	void DebugMode();
	void PropActive();
	void PropCheat();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ExecuteWaterSurfaceJump(const FVector& LaunchVelocity);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Launch(const FVector& LaunchVelocity);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_StartCrouch();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_StopCrouch();
	UFUNCTION(Server, Reliable)
	void ServerRPC_StartSprint();
	UFUNCTION(Server, Reliable)
	void ServerRPC_StopSprint();
	UFUNCTION(Server, Reliable)
	void ServerRPC_StartTongueAttack();


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
	UFUNCTION(BlueprintCallable)
	void SetJumpGaugeVisibility(bool bVisibility);
	void SetCrouchEnabled(bool bEnabled);
	UFUNCTION()
	void OnRep_SuperJumpRatio();
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetJumpAvailableBlock(int32 Block);
	void CalculateWaterCameraOverlapRatio(float dt);
	
public:
	// 물에 들어갔는지, 나왔는지 업데이트
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPC_UpdateOverallWaterState(bool bNowInWater, class ARisingWaterProp* WaterVolume);
	// 물 속 특정 존 상태 (Deep, Shallow, Surface) 변경
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPC_SetSpecificWaterState(ECharacterStateEnum NewState);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPC_TeleportToLocation(FVector TargetLocation);
	UFUNCTION()
	void OnRep_CharacterWaterState();
	UFUNCTION()
	void OnRep_bIsSwimming();

	// 물 속에서의 물리 및 상태
	void HandleInWaterLogic(float DeltaTime);
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_CallLaunchCharacter(const FVector& Dir, float Force, bool bXY, bool bZ);
	UFUNCTION(Server, Reliable)
	void ServerRPC_PrepareMission(FVector Loc);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetMissionCamera();
	UFUNCTION(Server, Reliable)
	void ServerRPC_FinishMission();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetMovementCamera();

	UFUNCTION()
	void OnRep_TongueLengthRatio();
	UFUNCTION()
	void OnRep_IsTongueGrow();
	UFUNCTION()
	void OnRep_CanTongAttack();
	
	// 혓바닥
	void SetTongueLength(float Value);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetSkin(int32 NewIndex);
	UFUNCTION()
	void OnRep_SkinIndex();

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* TongueAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* PropActiveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* PropCheatAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* DebugModeAction;
	
	// 일반 변수
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float Pitch;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float Yaw;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool bIsCrouching;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_bIsSwimming)
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool bCanMove{true};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool bCanCrouch{true};
	float PrevVelocityZLength{};
	// 물 관련
	UPROPERTY(Replicated)
	float TimeSpentInWater;
	UPROPERTY(EditDefaultsOnly)
	float SurfaceStateForceTime;
	UPROPERTY(Replicated)
	bool bWaterStateForcedByTime;
	// 현재 오버랩 중인 물 볼륨
	UPROPERTY()
	TWeakObjectPtr<class ARisingWaterProp> CurrentWaterVolume;
	// 로비 캐릭터인지
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsLobbyFrog{false};
	// 혓바닥 공격
	FTimerHandle TongueTimer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_TongueLengthRatio)
	float TongueLengthRatio{0.f};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_IsTongueGrow)
	bool bIsTongueGrow{false};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_CanTongAttack)
	bool bCanTongAttack{true};
	// 스킨
	UPROPERTY(EditDefaultsOnly)
	TArray<UTexture2D*> SkinTextures;
	UPROPERTY(ReplicatedUsing=OnRep_SkinIndex)
	int32 SkinIndex{};

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
	UPROPERTY()
	UMaterialInstanceDynamic* WaterPostProcessDynamicMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPostProcessComponent* WaterPostProcessComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* FrogTongueMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* TongueCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* TongueTipComponent;

	
	// Enum
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_CharacterWaterState)
	ECharacterStateEnum CharacterWaterState;

	// 사운드
public:
	UPROPERTY(EditAnywhere)
	class USoundBase* JumpSound;

	UPROPERTY()
	TWeakObjectPtr<UPrimitiveComponent> OverlapWaterComponent;

public:
	EEmotionState EmotionState = EEmotionState::None;

	// 감정표현 관련
	UPROPERTY()
	bool bIsBind = false;
	UPROPERTY()
	bool bCanPlayEmotion = true;
	
	UFUNCTION()
	void OnPressCKey();
	UFUNCTION()
	void OnReleasedCKey();
	UFUNCTION()
	void OnSelectionEmotionIndex(int32 EmotionIndex);
	UFUNCTION()
	void CancelEmotion();
	UFUNCTION()
	void PlayEmotion(int32 EmotionIndex);
	UFUNCTION()
	void OnEmotionMontageEnd(UAnimMontage* Montage, bool bInterrupted);

	// 동기화 (클라이언트는 서버로 요청->처리)
	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayEmotion(AFrog* Character, int32 EmotionIndex);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_PlayEmotion(int32 EmotionIndex);

	UFUNCTION(Server, Reliable)
	void ServerRPC_CancelEmotion(AFrog* Character);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_CancelEmotion();

	// 재생할 몽타주
	UPROPERTY()
	UAnimMontage* CurrentEmotionMontage = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Emotion")
	UAnimMontage* GreetingMontage;
	UPROPERTY(EditAnywhere, Category = "Emotion")
	UAnimMontage* AngryMontage;
	UPROPERTY(EditAnywhere, Category = "Emotion")
	UAnimMontage* SadMontage;
	UPROPERTY(EditAnywhere, Category = "Emotion")
	UAnimMontage* MerongMontage;

	// 감정표현 UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UEmotionUI> EmotionUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEmotionUI* EmotionUI;
	
	UFUNCTION()
	void ShowEmotionUI(bool bIsShow);

	// 눈 바꾸기
	UPROPERTY(EditDefaultsOnly)
	TArray<UTexture2D*> EyeTextures;
	UPROPERTY(EditDefaultsOnly)
	int32 EyeIndex{};

	UFUNCTION()
	void ChangeEyeMaterial(int32 MatIndex);

	// 저장
	UPROPERTY()
	UAnimMontage* SaveCurrentMontage = nullptr;
	UPROPERTY()
	int32 SaveCurrentIndex = -1;
};


