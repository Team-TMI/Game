#include "PropDataComponent.h"

UPropDataComponent::UPropDataComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	// static ConstructorHelpers::FObjectFinder<UDataTable> DT_PROPDATA
	// (TEXT("/Game/MapEditor/DataTable/DT_PropData.DT_PropData"));
	// PropID.DataTable = DT_PROPDATA.Object;
}

void UPropDataComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UPropDataComponent::SetPropID(const FName& InPropID)
{
	// PropID.RowName = InPropID;
	
	// FPropStruct* Data = PropID.DataTable->FindRow<FPropStruct>(PropID.RowName, TEXT(""), true);
}
