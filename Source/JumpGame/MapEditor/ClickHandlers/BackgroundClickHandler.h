#pragma once

#include "ClickHandlerInterface.h"

class JUMPGAME_API FBackgroundClickHandler : public IClickHandler
{
public:
	FBackgroundClickHandler();
	virtual int32 GetPriority() const override { return 0; }
	virtual bool HandleClick(FClickResponse& ClickResponse, class AMapEditingPlayerController* PlayerController) override;
	virtual ~FBackgroundClickHandler() override;
};
