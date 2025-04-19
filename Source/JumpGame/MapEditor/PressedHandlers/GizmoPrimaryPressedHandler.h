#pragma once

#include "EPressedHandlingResult.h"
#include "PressedHandlerInterface.h"

class JUMPGAME_API FGizmoPrimaryPressedHandler : public IPressedHandler
{
public:
	FGizmoPrimaryPressedHandler();
	virtual int32 GetPriority() const override { return 100; };
	virtual bool HandlePressed(FClickResponse& PressedResponse, class AMapEditingPlayerController* PlayerController, const FVector& MouseStartPosition, const FVector& InitializedActorPosition) override;
	virtual ~FGizmoPrimaryPressedHandler() override;

private:
	EPressedHandlingResult PressedType = EPressedHandlingResult::GizmoPrimaryPressed;
};
