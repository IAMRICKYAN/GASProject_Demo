// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MaxHealth();

	// 虚函数，用于计算游戏效果的基础幅度，重写基类的CalculateBaseMagnitude_Implementation函数以自定义计算逻辑
	virtual float CalculateBaseMagnitude_Implementation(
		const FGameplayEffectSpec& Spec // 游戏效果规格的常量引用，包含用于计算幅度的所有必要信息
	) const override;


private:

	//用于定义如何捕获游戏效果中的属性。
	FGameplayEffectAttributeCaptureDefinition VigorDef;
};
