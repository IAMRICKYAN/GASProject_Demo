// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;

	// 获取指定等级的能力描述
	virtual FString GetDescription(int32 Level);
	// 获取下一等级的能力描述
	virtual FString GetNextLevelDescription(int32 Level);
	// 获取锁定状态的等级描述
	static FString GetLockedDescription(int32 Level);

protected:

	// 根据等级获取法力消耗
	float GetManaCost(float InLevel = 1.f) const;
	// 根据等级获取冷却时间
	float GetCooldown(float InLevel = 1.f) const;

};
