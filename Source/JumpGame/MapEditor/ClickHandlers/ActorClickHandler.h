#pragma once

#include "ClickHandlerInterface.h"

class JUMPGAME_API FActorClickHandler : public IClickHandler
{
public:
	FActorClickHandler();
	virtual int32 GetPriority() const override { return 50; }
	virtual bool HandleClick(FClickResponse& ClickResponse, class AMapEditingPlayerController* PlayerController) override;
	virtual ~FActorClickHandler() override;
};
