// Fill out your copyright notice in the Description page of Project Settings.


#include "GameProgressBarUI.h"

#include "PlayerMarkerWidget.h"
#include "Components/BoxComponent.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "GameFramework/PlayerState.h"
#include "JumpGame/Core/GameInstance/JumpGameInstance.h"
#include "JumpGame/Core/GameState/MapGameState.h"
#include "JumpGame/Props/LogicProp/GameFinishProp.h"
#include "JumpGame/Props/LogicProp/GameStartProp.h"
#include "JumpGame/Props/LogicProp/RisingWaterProp.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

void UGameProgressBarUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// 프로그레스 바에서 표시할 마커 위치값
	AGameStartProp* StartProp = Cast<AGameStartProp>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameStartProp::StaticClass()));
	ARisingWaterProp* WaterProp = Cast<ARisingWaterProp>(UGameplayStatics::GetActorOfClass(GetWorld(), ARisingWaterProp::StaticClass()));

	StartPropZ = StartProp->GetActorLocation().Z;
	WaterPropZ = WaterProp->SurfaceCollision->GetComponentLocation().Z;

	GI = Cast<UJumpGameInstance>(GetWorld()->GetGameInstance());
	
	// 모든 플레이어들의 값이 업데이트 되었다면 이 함수를 실행해야함
	AMapGameState* GS = Cast<AMapGameState>(GetWorld()->GetGameState());
	GS->OnAllClientAddedDelegate.AddDynamic(this, &UGameProgressBarUI::InitUISetting);
	
}

void UGameProgressBarUI::NativeTick(const FGeometry& MyGeometry, float DeltaSeconds)
{
	Super::NativeTick(MyGeometry, DeltaSeconds);

	AGameFinishProp* FinishProp = Cast<AGameFinishProp>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameFinishProp::StaticClass()));
	if (!FinishProp) return;
	FinPropZ = FinishProp->GetActorLocation().Z;
	if (bIsInit)
	{
		// 지속적으로 업데이트 하자
		UpdateProgressBar();
	}
}

void UGameProgressBarUI::InitUISetting()
{
	FFastLogger::LogScreen(FColor::Red, TEXT("InitUISetting!!!!!!"));
	
	CreatePlayerMarkers();
	bIsInit = true;
}

void UGameProgressBarUI::UpdatePlayerPos()
{
	AMapGameState* GS = Cast<AMapGameState>(GetWorld()->GetGameState());
	if (!GS) return;
	
	PlayerPos.SetNum(GS->PlayerArray.Num());
	
	// 그 중 1등 플레이어의 위치
	WinnerPos = 0.f;

	int32 PlayerIndex = 0;
	for (APlayerState* PlayerState : GS->PlayerArray)
	{
		// AController* Controller = PlayerState->GetOwner<AController>();
		AFrog* Frog = Cast<AFrog>(PlayerState->GetPawn());
		if (Frog)
		{
			if (PlayerPos.IsValidIndex(PlayerIndex))
			{
				// FFastLogger::LogConsole(TEXT("UpdatePlayerPos: %d"),PlayerIndex);
				// 플레이어 위치값(Z좌표임) 저장
				float CurrentPosition = Frog->GetActorLocation().Z - 30.f;
				
				PlayerPos[PlayerIndex] = CurrentPosition; // PlayerID에 해당하는 인덱스에 저장
				// 1등 플레이어 위치 갱신
				WinnerPos = FMath::Max(WinnerPos, CurrentPosition);
			}
		}
		PlayerIndex++;
	}
}

void UGameProgressBarUI::UpdateProgressBar()
{
	// 플레이어 정보 업데이트받고 (1등 포함)
	UpdatePlayerPos();
	
	// 1등 플레이어 기준으로 프로그레스 바 업데이트 시키자
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

	AMapGameState* GS = Cast<AMapGameState>(GetWorld()->GetGameState());
	if (!GS) return;
	
	PlayerMarkers.SetNum(GS->PlayerArray.Num());
	FFastLogger::LogConsole(TEXT("GS->PlayerArray.Num(): %d"), GS->PlayerArray.Num());
	
	int32 PlayerIndex = 0;
	for (APlayerState* PlayerState : GS->PlayerArray)
	{
		// AController* Controller = PlayerState->GetOwner<AController>();
		AFrog* Frog = Cast<AFrog>(PlayerState->GetPawn());
		
		if (Frog)
		{
			// 마커 위젯 생성
			UPlayerMarkerWidget* NewMarker = CreateWidget<UPlayerMarkerWidget>(GetWorld(), PlayerMarkerFactory);
			if (NewMarker)
			{
				FFastLogger::LogConsole(TEXT("CreatePlayerMarkers: %d"),PlayerIndex);
				// 위젯을 프로그레스 바 컨테이너에 추가
				Overlay_Player->AddChild(NewMarker);
				// 플레이어 ID와 정보 설정
				NewMarker->SetPlayerID(PlayerIndex);
        
				// 위젯 초기 위치 설정
				FVector2D TopLeft = Overlay_Player->GetCachedGeometry().GetAbsolutePosition(); // 좌측 상단
				FVector2D Size = Overlay_Player->GetCachedGeometry().GetLocalSize(); // 위젯 크기
				FVector2D BottomRight = TopLeft + FVector2D(Size.X, Size.Y); // 우측 하단
		
				NewMarker->SetRenderTranslation(BottomRight);
        
				// 로컬 플레이어 확인하여 색상 설정
				if (Frog->IsLocallyControlled())
				{
					NewMarker->SetColorAndOpacity(FColor::Green);
				}
				else
				{
					NewMarker->SetColorAndOpacity(FColor::Yellow);
				}
        
				// PlayerID 인덱스에 마커 저장
				PlayerMarkers[PlayerIndex] = NewMarker;
				FFastLogger::LogConsole(TEXT("PlayerIndex: %d"), PlayerIndex);

				PlayerIndex++;
			}
		}
	}
}

void UGameProgressBarUI::UpdatePlayerMarkers()
{
	// ProgressBar 크기 기준으로 위치 결정
	float BarHeight = GameProgressBar->GetCachedGeometry().GetLocalSize().Y;
	
	// 모든 플레이어 마커 위치 업데이트
	for (int32 i = 0; i < PlayerMarkers.Num(); i++)
	{
		UPlayerMarkerWidget* Marker = PlayerMarkers[i];
		if (!Marker || !PlayerPos.IsValidIndex(i)) continue;
        
		// 진행도 계산
		float PlayerProgress = (PlayerPos[i] - StartPropZ) / TotalGamePos;
		PlayerProgress = FMath::Clamp(PlayerProgress, 0.0f, 1.0f);
		
		float MarkerY = BarHeight * (1.0f - PlayerProgress) - 30.f; // 이미지 크기 (30)
		FVector2D MarkerPos(0.f, MarkerY);
		
		// 마커 위치 설정
		Marker->SetRenderTranslation(MarkerPos);

		// 🟢 후처리 연출 추가
		AMapGameState* GS = Cast<AMapGameState>(GetWorld()->GetGameState());
		if (!GS || !GS->PlayerArray.IsValidIndex(i)) continue;

		AFrog* Frog = Cast<AFrog>(GS->PlayerArray[i]->GetPawn());
		if (Frog && Frog->IsLocallyControlled())
		{
			float FogValue = FMath::Lerp(0.f, 1.f, PlayerProgress * 1.5f); // 필요시 커브 적용

			Frog->SetFrogVignetteIntensity_PP(1.f - FogValue);
			Frog->SetLightIntensity(FogValue);
			UpdateFogDensity(FogValue);
			UpdateSunBrightness(FogValue);

			UE_LOG(LogTemp, Warning, TEXT("FogValue: %f"), FogValue);
		}
	}
}
