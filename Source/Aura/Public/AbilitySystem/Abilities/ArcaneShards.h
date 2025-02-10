// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "ArcaneShards.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UArcaneShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	// 虚拟函数，用于获取当前等级的描述信息，重写基类中的同名函数
	virtual FString GetDescription(int32 Level) override;

	// 虚拟函数，用于获取下一等级的描述信息，重写基类中的同名函数
	virtual FString GetNextLevelDescription(int32 Level) override;

	// 属性，表示最大碎片数量，只能在默认值编辑器中编辑，在蓝图中只读
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxNumShards = 11;

};
