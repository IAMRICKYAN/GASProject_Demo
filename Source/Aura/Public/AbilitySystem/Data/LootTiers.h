// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

// 结构体，用于定义战利品项的详细信息，可以在蓝图中使用
USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()

	// 战利品项的类，用于实例化具体的战利品对象，可以在编辑器中任意位置编辑，在蓝图中只读
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTiers|Spawning")
	TSubclassOf<AActor> LootClass;

	// 战利品项的生成概率，默认值为0，表示不会生成，可以在编辑器中任意位置编辑
	UPROPERTY(EditAnywhere, Category = "LootTiers|Spawning")
	float ChanceToSpawn = 0.f;

	// 战利品项的最大生成数量，默认值为0，表示不会生成，可以在编辑器中任意位置编辑
	UPROPERTY(EditAnywhere, Category = "LootTiers|Spawning")
	int32 MaxNumberToSpawn = 0;

	// 是否覆盖战利品的等级，默认为true，表示会覆盖，可以在编辑器中任意位置编辑，在蓝图中只读
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTiers|Spawning")
	bool bLootLevelOverride = true;
};


/**
 * 
 */
UCLASS()
class AURA_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()
public:

	// 函数，用于获取战利品项数组，可以在蓝图中调用
	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetLootItems();

	// 属性，存储战利品项数组，只能在默认值编辑器中编辑，归类在"LootTiers|Spawning"类别下
	UPROPERTY(EditDefaultsOnly, Category = "LootTiers|Spawning")
	TArray<FLootItem> LootItems;

};
