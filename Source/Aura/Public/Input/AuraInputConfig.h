// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AuraInputConfig.generated.h"

// 定义一个用于Aura游戏输入动作的结构体，可以在蓝图中使用
USTRUCT(BlueprintType)
struct FAuraInputAction
{
	// 标准结构体生成代码
	GENERATED_BODY()

	// 输入动作属性，只能在默认值中编辑，用于存储指向UInputAction的指针，默认为nullptr
	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	// 输入标签属性，只能在默认值中编辑，用于存储与输入动作关联的游戏玩法标签，默认为空标签
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};


/**
 * 
 */
UCLASS()
class AURA_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:

	// 查找与给定输入标签匹配的能力输入操作
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;

	// 能力输入操作的数组
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraInputAction> AbilityInputActions;
};
