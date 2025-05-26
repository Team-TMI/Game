#pragma once

#include "CoreMinimal.h"
#include "ClickHandlerInterface.h"

class JUMPGAME_API FPropSlotClickHandler : public IClickHandler
{
public:
	FPropSlotClickHandler();
	virtual int32 GetPriority() const override { return 300; }
	virtual bool HandleClick(FClickResponse& ClickResponse, class AMapEditingPlayerController* PlayerController, bool bRotateGizmoMode) override;
	virtual ~FPropSlotClickHandler() override;
};
