#pragma once

#include "ECategoryType.generated.h"

UENUM(BlueprintType)
enum class EMajorCategoryType : uint8
{
	None = 0		UMETA(DisplayName = "None"),
	Basic = 1		UMETA(DisplayName = "Basic"),
	Obstacle = 2	UMETA(DisplayName = "Obstacle"),
	Platform = 3	UMETA(DisplayName = "Platform"),
	Decoration = 4	UMETA(DisplayName = "Decoration"),
	Count			UMETA(DisplayName = "Count"),			// 카테고리 개수
};

UENUM()
enum class ESubCategoryType : uint8
{
	None = 0		UMETA(DisplayName = "None"),

	// 자연 재질
	Wood			UMETA(DisplayName = "Wood"),			// 목재, 대나무 포함
	Stone			UMETA(DisplayName = "Stone"),			// 암석 계열
	Soil			UMETA(DisplayName = "Soil"),			// 흙, 모래, 자갈 포함
	Plant			UMETA(DisplayName = "Plant"),			// 수풀, 잔디, 꽃, 덩쿨 등
	Water			UMETA(DisplayName = "Water"),			// 수풀(물), 수련잎 포함
	Ice				UMETA(DisplayName = "Ice"),				// 얼음, 눈 포함

	// 구조 및 인공물
	Glass			UMETA(DisplayName = "Glass"),			// 유리
	Concrete		UMETA(DisplayName = "Concrete"),		// 콘크리트, 인공 건축물
	Metal			UMETA(DisplayName = "Metal"),			// 필요시 추가

	// 동적/특수 효과
	Fire			UMETA(DisplayName = "Fire"),			// 불
	Etc				UMETA(DisplayName = "Etc"),				// 기타

	Count			UMETA(DisplayName = "Count"),			// 카테고리 개수
};