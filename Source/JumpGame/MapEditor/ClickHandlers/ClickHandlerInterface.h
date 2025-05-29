#pragma once

// UI, Gizmo, Actor, Background 클릭시 수행
#include "ClickResponse.h"

class JUMPGAME_API IClickHandler
{
public:
	IClickHandler() {};
	virtual int32 GetPriority() const = 0;
	virtual bool HandleClick(FClickResponse& ClickResponse, class AMapEditingPlayerController* PlayerController, bool bRotateGizmoMode) = 0;
	virtual ~IClickHandler() {};
	virtual void Init(class AMapEditingPawn* InEditingPawn) { ;
		// 초기화 함수, 필요시 오버라이드
		EditingPawn = InEditingPawn; 
	}
	
protected:
	class AMapEditingPawn* EditingPawn = nullptr; // 편집 중인 Pawn
};
