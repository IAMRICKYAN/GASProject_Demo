// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()
public:
	// 虚函数，用于获取当前等级的能力描述，重写基类中的实现
	virtual FString GetDescription(int32 Level) override;
	// 虚函数，用于获取下一等级的能力描述，重写基类中的实现
	virtual FString GetNextLevelDescription(int32 Level) override;

	// 可在蓝图中调用的函数，用于生成投射物
	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget);

protected:

	// 编辑默认值属性，用于定义投射物的分散角度
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float ProjectileSpread = 90.f;

	// 编辑默认值属性，用于定义最大投射物数量
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	int32 MaxNumProjectiles = 5;

	// 编辑默认值属性，用于定义最小自导加速度
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float HomingAccelerationMin = 1600.f;

	// 编辑默认值属性，用于定义最大自导加速度
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float HomingAccelerationMax = 3200.f;

	// 编辑默认值属性，用于定义是否发射自导投射物
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	bool bLaunchHomingProjectiles = true;

};
