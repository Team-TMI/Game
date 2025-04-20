#include "RotateHandlerManager.h"

#include "JumpGame/MapEditor/ClickHandlers/ClickHandlerManager.h"
#include "JumpGame/MapEditor/Components/GridComponent.h"
#include "JumpGame/MapEditor/Pawn/MapEditingPawn.h"
#include "JumpGame/Props/PrimitiveProp/PrimitiveProp.h"

URotateHandlerManager::URotateHandlerManager()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

}

void URotateHandlerManager::BeginPlay()
{
	Super::BeginPlay();


}

void URotateHandlerManager::InitializeComponent()
{
	Super::InitializeComponent();

	AMapEditingPawn* Pawn = Cast<AMapEditingPawn>(GetOwner());
	if (!Pawn) return;

	Pawn->GetClickHandlerManager()->OnControlledPropChanged.AddDynamic(this, &URotateHandlerManager::ResetAxis);
}

void URotateHandlerManager::HandleRotate(FClickResponse& ClickResponse)
{
	APrimitiveProp* PrimitiveProp = ClickResponse.TargetProp;
	if (!PrimitiveProp) return;

	UGridComponent* GridComponent = PrimitiveProp->GetGridComp();
	if (!GridComponent) return;

	GridComponent->Rotate(Axis);

	PrimitiveProp->SetNewSizeByRotation(GridComponent->GetSize());
	
	FFastLogger::LogScreen(FColor::Red, TEXT("Rotate : %s"), *Axis.ToString());
}

void URotateHandlerManager::HandleAxis(FVector InAxis, FClickResponse& ClickResponse)
{
	APrimitiveProp* PrimitiveProp = ClickResponse.TargetProp;
	if (!PrimitiveProp) return;

	Axis = InAxis.GetSafeNormal().GetAbs();
	FFastLogger::LogScreen(FColor::Red, TEXT("Axis : %s"), *Axis.ToString());
}

void URotateHandlerManager::ResetAxis()
{
	Axis = FVector(0, 0, 1);
	FFastLogger::LogScreen(FColor::Red, TEXT("Reset Axis : %s"), *Axis.ToString());
}

