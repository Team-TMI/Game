// Fill out your copyright notice in the Description page of Project Settings.


#include "ConveyorBeltProp.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JumpGame/Characters/Frog.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AConveyorBeltProp::AConveyorBeltProp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("ConveyorBelt");
	
	CollisionComp->SetBoxExtent(FVector(102.f, 51.f, 60.f));
	CollisionComp->SetRelativeLocation(FVector(0, 0, 0));
	MeshComp->SetRelativeLocation(FVector(0, 0, -10));
	MeshComp->SetRelativeScale3D(FVector(2, 1, 1));
	
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeLocation(FVector(110, 0, 0));

	// 컨베이어 벨트 콜리전과 overlap
	CollisionComp->SetCollisionProfileName(TEXT("OverlapProp"));
}

void AConveyorBeltProp::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	AFrog* Frog = Cast<AFrog>(OtherActor);
	if (Frog && !OverlappingFrogs.Contains(Frog))
	{
		
		OverlappingFrogs.Add(Frog);
	}
	
	Super::OnMyBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AConveyorBeltProp::OnMyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AFrog* Frog = Cast<AFrog>(OtherActor);
	if (Frog)
	{
	
		OverlappingFrogs.Remove(Frog);
	}
	
	Super::OnMyEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

// Called when the game starts or when spawned
void AConveyorBeltProp::BeginPlay()
{
	Super::BeginPlay();

	BeltDir = Arrow->GetForwardVector().GetSafeNormal();
}

// Called every frame
void AConveyorBeltProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 클라 서버 둘다 각자 실행
	ConveyorMove();
}

void AConveyorBeltProp::ConveyorMove()
{
	// 유효한 Frog들만 처리
	for (AFrog* Frog : OverlappingFrogs)
	{
		if (!IsValid(Frog)) continue;
		
		FVector DeltaPos = BeltSpeed * GetWorld()->DeltaTimeSeconds * BeltDir;
		
		// if (Frog->GetVelocity().IsNearlyZero())
		// {
			// Frog->AddActorWorldOffset(DeltaPos, true);
			Frog->SetActorLocation(Frog->GetActorLocation() + DeltaPos);
		// }
		
		// 클라, 서버 동일한 로직 처리
		/*if (Frog->IsLocallyControlled())
		{
			UCharacterMovementComponent* MovementComp = Frog->GetCharacterMovement();
			
			if (!Frog->GetVelocity().IsNearlyZero())
			{
				// 조작 있다면, AddActorWorldOffset말고 velocity에 더한다
				// *벨트 방향과 비교하기
				FVector InputDir = Frog->GetVelocity().GetSafeNormal();
				float Dot = FVector::DotProduct(InputDir, BeltDir);

				// 기본 최대 속도
				float OriginMaxSpeed = MovementComp->GetMaxSpeed();
				// 컨베이어 벨트에서 추가해줄 속도
				float BeltBoostSpeed = 300.f;
				// 최대 허용 속도
				float MaxAllowSpeed = OriginMaxSpeed + BeltBoostSpeed;

				FFastLogger::LogScreen(FColor::Red, TEXT("움직이고 있음"));
				FFastLogger::LogScreen(FColor::Red, TEXT("OriginMaxSpeed : %f"), OriginMaxSpeed);
				

				// 벨트 방향쪽이라면 (+1)
				if (Dot > 0.7f)
				{
					// 속도 추가 후 Clamp 하자
					MovementComp->Velocity += DeltaPos;
					float CurrentSpeed = MovementComp->Velocity.Size();
					FFastLogger::LogScreen(FColor::Red,TEXT("CurrentSpeed : %f"), CurrentSpeed);
                    
					if (CurrentSpeed > MaxAllowSpeed)
					{
						// 속도의 방향은 유지하되 크기를 제한
						MovementComp->Velocity = MovementComp->Velocity.GetSafeNormal() * MaxAllowSpeed;
						FFastLogger::LogScreen(FColor::Red,TEXT("MovementComp->Velocity : %f"), MovementComp->Velocity.Size());
					}
				}
				else if (Dot < -0.7f)
				{
					// 속도 감소 후 최소 속도 보장
					MovementComp->Velocity -= DeltaPos;
					float CurrentSpeed = MovementComp->Velocity.Size();
                    
					// 최소 속도가 0보다 작아지지 않도록
					if (CurrentSpeed < 0.0f)
					{
						MovementComp->Velocity = FVector::ZeroVector;
					}
				}
				else
				{
					// 벨트와 수직 방향일때는 (뛸때, 측면으로 이동할때)
					// 일단 아무 영향이 없다 
				}
			}
		}*/
	}
}


