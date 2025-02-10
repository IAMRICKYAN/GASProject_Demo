// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:

	// 函数，用于对目标演员造成伤害，可以在蓝图中调用
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	// 函数，用于根据类默认值创建伤害效果参数，是一个纯函数，不修改任何状态，可以在蓝图中调用
	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(
		AActor* TargetActor = nullptr,
		FVector InRadialDamageOrigin = FVector::ZeroVector,
		bool bOverrideKnockbackDirection = false,
		FVector KnockbackDirectionOverride = FVector::ZeroVector,
		bool bOverrideDeathImpulse = false,
		FVector DeathImpulseDirectionOverride = FVector::ZeroVector,
		bool bOverridePitch = false,
		float PitchOverride = 0.f) const;

	// 函数，用于获取当前等级的伤害值，是一个纯函数，不修改任何状态，可以在蓝图中调用
	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;

protected:

	// 属性，伤害效果的游戏玩法效果类，只能在默认值编辑器中编辑，并在蓝图中可读
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// 属性，伤害类型标签，只能在默认值编辑器中编辑，分类在"Damage"下
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType;

	// 属性，伤害值，可随等级变化，只能在默认值编辑器中编辑，分类在"Damage"下
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage;

	// 属性，减益效果触发的几率，默认为20%，只能在默认值编辑器中编辑，分类在"Damage"下
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffChance = 20.f;

	// 属性，减益效果的伤害值，默认为5，只能在默认值编辑器中编辑，分类在"Damage"下
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDamage = 5.f;

	// 属性，减益效果的触发频率，默认为1秒一次，只能在默认值编辑器中编辑，分类在"Damage"下
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffFrequency = 1.f;

	// 属性，减益效果的持续时间，默认为5秒，只能在默认值编辑器中编辑，分类在"Damage"下
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDuration = 5.f;

	// 属性，死亡冲量的幅度，默认为1000，只能在默认值编辑器中编辑，分类在"Damage"下
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeathImpulseMagnitude = 1000.f;

	// 属性，击退力的幅度，默认为1000，只能在默认值编辑器中编辑，分类在"Damage"下
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackForceMagnitude = 1000.f;

	// 属性，击退效果触发的几率，默认为0%，只能在默认值编辑器中编辑，分类在"Damage"下
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackChance = 0.f;

	// 属性，是否为范围伤害，默认为否，只能在默认值编辑器中编辑，分类在"Damage"下
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	bool bIsRadialDamage = false;

	// 属性，范围伤害的内半径，默认为0，只能在默认值编辑器中编辑，并在蓝图中可读，分类在"Damage"下
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageInnerRadius = 0.f;

	// 属性，范围伤害的外半径，默认为0，只能在默认值编辑器中编辑，并在蓝图中可读，分类在"Damage"下
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageOuterRadius = 0.f;

	// 函数，用于从标记过的动画数组中随机获取一个标记过的动画，是一个纯函数，不修改任何状态，可以在蓝图中调用
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

};
