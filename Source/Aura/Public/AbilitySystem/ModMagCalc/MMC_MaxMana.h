// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MaxMana();

	// 声明一个虚函数CalculateBaseMagnitude_Implementation，它接受一个常量引用参数Spec，类型为FGameplayEffectSpec，
	// 并且函数本身是const的，表示它不会修改类的成员变量。这个函数返回一个float类型的值。
	// 使用virtual关键字表示这是一个虚函数，可以被派生类重写。
	// 使用override关键字表示这个函数重写了基类中的同名函数。
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;


private:

	// 声明一个FGameplayEffectAttributeCaptureDefinition类型的变量，命名为IntDef。
	// FGameplayEffectAttributeCaptureDefinition是一个结构体或类，用于定义游戏效果属性捕获的参数，
	// 包括要捕获的属性、属性来源以及是否使用快照等信息。
	// 这个变量将用于配置和存储与游戏效果属性捕获相关的定义。
	FGameplayEffectAttributeCaptureDefinition IntDef;

};
