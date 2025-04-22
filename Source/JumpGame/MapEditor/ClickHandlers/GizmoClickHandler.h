#pragma once

#include "ClickHandlerInterface.h"

class JUMPGAME_API FGizmoClickHandler : public IClickHandler
{
public:
	FGizmoClickHandler();
	virtual int32 GetPriority() const override { return 100; }
	virtual bool HandleClick(FClickResponse& ClickResponse, class AMapEditingPlayerController* PlayerController) override;
	virtual ~FGizmoClickHandler() override;
};
