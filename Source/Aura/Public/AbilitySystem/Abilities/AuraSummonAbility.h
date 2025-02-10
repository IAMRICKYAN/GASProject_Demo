// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:

	// 蓝图可调用函数，用于获取生成位置数组
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

	// 蓝图纯函数，分类为"Summoning"，用于随机获取一个仆从类
	UFUNCTION(BlueprintPure, Category="Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();

	// 编辑默认属性，分类为"Summoning"，用于设置生成仆从的数量，默认值为5
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	int32 NumMinions = 5;

	// 编辑默认属性，分类为"Summoning"，用于存储可生成的仆从类数组
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	// 编辑默认属性，分类为"Summoning"，用于设置生成位置的最小距离，默认值为50
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MinSpawnDistance = 50.f;

	// 编辑默认属性，分类为"Summoning"，用于设置生成位置的最大距离，默认值为250
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MaxSpawnDistance = 250.f;

	// 编辑默认属性，分类为"Summoning"，用于设置生成位置的分散程度，默认值为90
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float SpawnSpread = 90.f;



	
};
