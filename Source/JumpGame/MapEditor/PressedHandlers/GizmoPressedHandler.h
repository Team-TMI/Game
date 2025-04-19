#pragma once

#include "EPressedHandlingResult.h"
#include "PressedHandlerInterface.h"

class JUMPGAME_API FGizmoPressedHandler : public IPressedHandler
{
public:
	FGizmoPressedHandler();
	virtual int32 GetPriority() const override { return 50; };
	virtual bool HandlePressed(FClickResponse& PressedResponse, class AMapEditingPlayerController* PlayerController, const FVector& MouseStartPosition, const FVector& InitializedActorPosition) override;
	virtual ~FGizmoPressedHandler() override;

private:
	EPressedHandlingResult PressedType = EPressedHandlingResult::GizmoPressed;
};
