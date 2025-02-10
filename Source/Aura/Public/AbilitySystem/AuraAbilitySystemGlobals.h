// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	// 虚拟函数，用于分配游戏效果上下文对象的内存
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;

};
