// Fill out your copyright notice in the Description page of Project Settings.


#include "GameProgressBarUI.h"

#include "PlayerMarkerWidget.h"
#include "Components/BoxComponent.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "GameFramework/PlayerState.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Props/LogicProp/GameFinishProp.h"
#include "JumpGame/Props/LogicProp/GameStartProp.h"
#include "JumpGame/Props/LogicProp/RisingWaterProp.h"
#include "Kismet/GameplayStatics.h"

void UGameProgressBarUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	PlayerInfo = GI->GetPlayerInfo();

	// 프로그레스 바에서 표시할 마커 위치값
	AGameStartProp* StartProp = Cast<AGameStartProp>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameStartProp::StaticClass()));
	AGameFinishProp* FinishProp = Cast<AGameFinishProp>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameFinishProp::StaticClass()));
	ARisingWaterProp* WaterProp = Cast<ARisingWaterProp>(UGameplayStatics::GetActorOfClass(GetWorld(), ARisingWaterProp::StaticClass()));

	StartPropZ = StartProp->GetActorLocation().Z;
	FinPropZ = FinishProp->GetActorLocation().Z;
	WaterPropZ = WaterProp->SurfaceCollision->GetComponentLocation().Z;
	
	Character = Cast<AFrog>(GetWorld()->GetFirstPlayerController()->GetPawn());

	// MyPlayerID 는 PlayerInfo의 Value의 PlayerID로 저장해놔야함
	
	CreatePlayerMarkers();
}

void UGameProgressBarUI::NativeTick(const FGeometry& MyGeometry, float DeltaSeconds)
{
	Super::NativeTick(MyGeometry, DeltaSeconds);

	UpdateProgressBar();
}

void UGameProgressBarUI::UpdatePlayerPos()
{
	// Max Player ID 찾기
	int32 MaxPlayerID = -1;
	for (auto& It : PlayerInfo)
	{
		MaxPlayerID = FMath::Max(MaxPlayerID, It.Value.PlayerID);
	}

	PlayerPos.SetNum(MaxPlayerID + 1);
	PlayerPos.Init(0.f, MaxPlayerID + 1);
	WinnerPos = 0.f;
	
	// 각 플레이어 정보를 맵에서 가져와 PlayerID 순서대로 저장하고 싶음
	for (auto& it : PlayerInfo)
	{
		// PlayerID를 배열 인덱스로 사용하자
		FPlayerInfo& Player = it.Value;
		int32 PlayerIndex = Player.PlayerID; 
        
		// PlayerID가 유효한 범위인지 확인하고
		if (PlayerPos.IsValidIndex(PlayerIndex))
		{
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex);
			if (PC && PC->GetPawn())
			{
				APawn* Pawn = PC->GetPawn();
                
				// 플레이어 위치값(Z좌표임) 저장
				float Position = Pawn->GetActorLocation().Z;
				PlayerPos[PlayerIndex] = Position; // PlayerID에 해당하는 인덱스에 저장
                
				// 1등 플레이어 위치 갱신
				WinnerPos = FMath::Max(WinnerPos, Position);
			}
		}
	}
}

void UGameProgressBarUI::UpdateProgressBar()
{
	// 플레이어 정보 업데이트받고
	UpdatePlayerPos();
	
	// 1등 플레이어 기준으로 업데이트 됨
	float CurrentWinnerPos = WinnerPos - StartPropZ;
	TotalGamePos = FinPropZ - StartPropZ;
	GameProgressBar->SetPercent(CurrentWinnerPos / TotalGamePos);

	// 마커 위치도 업데이트하자
	UpdatePlayerMarkers();
}

void UGameProgressBarUI::CreatePlayerMarkers()
{
	// 기존 마커 제거
	for (UPlayerMarkerWidget* ExistingMarker : PlayerMarkers)
	{
		if (ExistingMarker)
		{
			ExistingMarker->RemoveFromParent();
		}
	}
	PlayerMarkers.Empty();
    
	// PlayerID 최대값 찾기 (배열 크기 결정용)
	int32 MaxPlayerID = -1;
	for (auto& it : PlayerInfo)
	{
		MaxPlayerID = FMath::Max(MaxPlayerID, it.Value.PlayerID);
	}
    
	// 플레이어 마커 배열 크기 설정
	PlayerMarkers.SetNum(MaxPlayerID + 1);
    
	// 각 플레이어에 대한 마커 생성
	for (auto& it : PlayerInfo)
	{
		FPlayerInfo& Player = it.Value;
		int32 PlayerIndex = Player.PlayerID;
        
		// 마커 위젯 생성
		PlayerMarker = CreateWidget<UPlayerMarkerWidget>(GetWorld(), PlayerMarkerFactory);
		if (PlayerMarker)
		{
			// 위젯을 프로그레스 바 컨테이너에 추가
			Overlay_Player->AddChild(PlayerMarker);
            
			// 위젯 초기 위치 설정
			FVector2D StartPos = Overlay_Player->GetCachedGeometry().GetAbsolutePosition();
			PlayerMarker->SetRenderTranslation(StartPos);
            
			// 플레이어 ID와 정보 설정
			PlayerMarker->SetPlayerID(PlayerIndex);
			FFastLogger::LogConsole(TEXT("PlayerIndex : %d"), PlayerIndex);
            
			// 로컬 플레이어 확인하여 색상 설정
			if (PlayerIndex == MyPlayerID)
			{
				PlayerMarker->SetColorAndOpacity(FColor::Green);
			}
			else
			{
				PlayerMarker->SetColorAndOpacity(FColor::Yellow);
			}
            
			// PlayerID 인덱스에 마커 저장
			PlayerMarkers[PlayerIndex] = PlayerMarker;
		}
	}
}

void UGameProgressBarUI::UpdatePlayerMarkers()
{
	// ProgressBar 크기 기준으로 위치 결정
	float BarWidth = GameProgressBar->GetCachedGeometry().GetLocalSize().X;
	
	// 모든 플레이어 마커 위치 업데이트
	for (int32 i = 0; i < PlayerMarkers.Num(); i++)
	{
		UPlayerMarkerWidget* Marker = PlayerMarkers[i];
		if (!Marker || !PlayerPos.IsValidIndex(i)) continue;
        
		// 진행도 계산
		float PlayerProgress = (PlayerPos[i] - StartPropZ) / TotalGamePos;
		PlayerProgress = FMath::Clamp(PlayerProgress, 0.0f, 1.0f);
        
		float MarkerX = BarWidth * PlayerProgress;
		FVector2D MarkerPos(MarkerX, 0.f);
		
		// 마커 위치 설정
		Marker->SetRenderTranslation(MarkerPos);
	}
}
