#pragma once

// UI, Gizmo, Actor, Background 클릭시 수행
#include "ClickResponse.h"

class JUMPGAME_API IClickHandler
{
public:
	IClickHandler() {};
	virtual int32 GetPriority() const = 0;
	virtual bool HandleClick(FClickResponse& ClickResponse, class AMapEditingPlayerController* PlayerController) = 0;
	virtual ~IClickHandler() {};
};
