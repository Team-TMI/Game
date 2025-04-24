#pragma once

#include "CoreMinimal.h"
#include "ClickHandlerInterface.h"

class JUMPGAME_API FPropSlotClickHandler : public IClickHandler
{
public:
	FPropSlotClickHandler();
	virtual int32 GetPriority() const override { return 200; }
	virtual bool HandleClick(FClickResponse& ClickResponse, class AMapEditingPlayerController* PlayerController) override;
	virtual ~FPropSlotClickHandler() override;
};
