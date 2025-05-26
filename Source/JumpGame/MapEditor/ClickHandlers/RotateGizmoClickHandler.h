#pragma once

#include "ClickHandlerInterface.h"

class JUMPGAME_API FRotateGizmoClickHandler : public IClickHandler
{
public:
	FRotateGizmoClickHandler();
	virtual int32 GetPriority() const override { return 200; }
	virtual bool HandleClick(FClickResponse& ClickResponse, class AMapEditingPlayerController* PlayerController, bool bRotateGizmoMode) override;
	virtual ~FRotateGizmoClickHandler() override;
};
