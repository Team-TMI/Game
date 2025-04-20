#include "GridComponent.h"

#include "Evaluation/IMovieSceneEvaluationHook.h"
#include "JumpGame/Utils/FastLogger.h"

UGridComponent::UGridComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UGridComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UGridComponent::InitializeComponent()
{
	Super::InitializeComponent();

	UpdatedActor = GetOwner();
}

void UGridComponent::SetSize(FVector NewSize)
{
	Size.X = FMath::RoundToInt(NewSize.X);
	Size.Y = FMath::RoundToInt(NewSize.Y);
	Size.Z = FMath::RoundToInt(NewSize.Z);
	
	// Offset = Size * SnapSize * 0.5f;
}

bool UGridComponent::MoveByGizmoPrimary(FVector MouseLocation, const FHitResult& HitResult)
{
	bool flag = true;

	FVector Sign = FVector(1.f, 1.f, 1.f);

	Sign.X = ((int32)MouseLocation.X == 0 || (MouseLocation.X >= 0.f)) ? 1.f : -1.f;
	Sign.Y = ((int32)MouseLocation.Y == 0 || (MouseLocation.Y >= 0.f)) ? 1.f : -1.f;
	Sign.Z = ((int32)MouseLocation.Z == 0 || (MouseLocation.Z >= 0.f)) ? 1.f : -1.f;

	// GridComponent의 위치 계산
	// 그리드의 스냅 단위는 100으로 유지
	X = (int32)FMath::RoundToInt(MouseLocation.X) / ((int32)SnapSize);
	Y = (int32)FMath::RoundToInt(MouseLocation.Y) / ((int32)SnapSize);
	Z = (int32)FMath::RoundToInt(MouseLocation.Z) / ((int32)SnapSize);

	// 음수일 경우 -1을 더해줌 :
	// 이유 : 음수일 경우 -53도 100으로 나누면 0이 나오기 때문이다. 이때 부터의 기준점은 -100에 Snap이 되어야 한다.
	X = Sign.X < 0.f ? X - 1 : X;
	Y = Sign.Y < 0.f ? Y - 1 : Y;
	Z = Sign.Z < 0.f ? Z - 1 : Z;

	// 충돌이 발생했을 때 접촉 면의 Normal을 받아와서 만약 해당 Normal이 -1이면 해당 방향으로 1칸 이동
	if (HitResult.IsValidBlockingHit())
	{
		FVector ImpactNormal = HitResult.ImpactNormal;
		ImpactNormal.Normalize();
		X = (int32)ImpactNormal.X < 0.f ? X - Size.X : X;
		Y = (int32)ImpactNormal.Y < 0.f ? Y - Size.Y : Y;
		Z = (int32)ImpactNormal.Z < 0.f ? Z - Size.Z : Z;
	}

	// 100 단위 그리드 기준이지만, 블록의 중심을 고려하여 보정
	FVector GridOrigin = FVector(X, Y, Z) * SnapSize;
	// FVector CenterOffset = FVector(Size) * SnapSize * 0.5f;
	
	// 물체의 중점을 그리드의 교차점으로 옮김
	FVector CenterOffset = FVector::ZeroVector;
	FVector GridLocation = GridOrigin + CenterOffset;
	
	UpdatedActor->SetActorLocation(GridLocation);

	// 만약 충돌하는 곳이 있다면 해당 장소는 못 간다는 것을 알려줌
	return flag;
}

void UGridComponent::MoveByGizmo(const FVector& NewLocation)
{
	FVector Sign = FVector(1.f, 1.f, 1.f);

	Sign.X = ((int32)NewLocation.X == 0 || (NewLocation.X >= 0.f)) ? 1.f : -1.f;
	Sign.Y = ((int32)NewLocation.Y == 0 || (NewLocation.Y >= 0.f)) ? 1.f : -1.f;
	Sign.Z = ((int32)NewLocation.Z == 0 || (NewLocation.Z >= 0.f)) ? 1.f : -1.f;

	// GridComponent의 위치 계산
	// 그리드의 스냅 단위는 100으로 유지
	X = (int32)FMath::RoundToInt(NewLocation.X) / ((int32)SnapSize);
	Y = (int32)FMath::RoundToInt(NewLocation.Y) / ((int32)SnapSize);
	Z = (int32)FMath::RoundToInt(NewLocation.Z) / ((int32)SnapSize);

	// NOTE: Gizmo로 이동할 때는 -1을 더해주지 않음, 왜냐하면 딱 거기 있어야 하는데 이것 때문에 한번 더 연산이 들어가기 때문이다.
	// X = Sign.X < 0.f ? X - 1 : X;
	// Y = Sign.Y < 0.f ? Y - 1 : Y;
	// Z = Sign.Z < 0.f ? Z - 1 : Z;

	// 100 단위 그리드 기준이지만, 블록의 중심을 고려하여 보정
	FVector GridOrigin = FVector(X, Y, Z) * SnapSize;
	// FVector CenterOffset = FVector(Size) * SnapSize * 0.5f;
	
	// 물체의 중점을 그리드의 교차점으로 옮김
	FVector CenterOffset = FVector::ZeroVector;
	FVector GridLocation = GridOrigin + CenterOffset;
	
	UpdatedActor->SetActorLocation(GridLocation);
}

void UGridComponent::RotateActorInGrid(FVector Direction)
{
	// Direction을 기준으로 회전
	
}
