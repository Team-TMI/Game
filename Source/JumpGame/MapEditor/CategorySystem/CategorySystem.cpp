#include "CategorySystem.h"

UCategorySystem::UCategorySystem()
{
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_PROPDATA
	(TEXT("/Game/MapEditor/DataTable/DT_PropData.DT_PropData"));
	CategoryDataTable = DT_PROPDATA.Object;
}

// 카테고리 시스템을 만들어보자!!
void UCategorySystem::BeginPlay()
{
	Super::BeginPlay();

	
}
