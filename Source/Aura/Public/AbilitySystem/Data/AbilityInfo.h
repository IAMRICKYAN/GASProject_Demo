// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilityInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
	GENERATED_BODY()

	// 能力标签，用于标识这个能力，只能在默认值编辑器中编辑，蓝图可读
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	// 输入标签，用于标识触发这个能力的输入，蓝图只读
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	// 状态标签，用于标识这个能力当前的状态，蓝图只读
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag();

	// 冷却标签，用于标识这个能力的冷却状态，只能在默认值编辑器中编辑，蓝图可读
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag();

	// 能力类型标签，用于标识这个能力的类型，只能在默认值编辑器中编辑，蓝图可读
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityType = FGameplayTag();

	// 能力的图标，用于在界面中显示，只能在默认值编辑器中编辑，蓝图可读
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;

	// 背景材质，用于在界面中作为能力的背景显示，只能在默认值编辑器中编辑，蓝图可读
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;

	// 等级要求，表示使用这个能力所需的最低等级，只能在默认值编辑器中编辑，蓝图可读
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequirement = 1;

	// 能力类，表示这个能力的具体实现，只能在默认值编辑器中编辑，蓝图可读
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;
};


/**
 * 
 */
UCLASS()
class AURA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
public:

	// 能力信息数组，包含所有能力的相关数据，只能在默认值编辑器中编辑，蓝图可读，归类在"AbilityInformation"类别下
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInformation")
	TArray<FAuraAbilityInfo> AbilityInformation;

	// 根据能力标签查找对应的能力信息
	// @param AbilityTag 要查找的能力标签
	// @param bLogNotFound 如果为true，当没有找到对应的能力信息时，会在日志中记录
	// @return 返回找到的FAuraAbilityInfo对象，如果未找到则返回一个默认构造的FAuraAbilityInfo对象
	FAuraAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;

};
