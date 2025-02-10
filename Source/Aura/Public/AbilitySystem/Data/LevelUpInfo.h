// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

// 结构体，用于定义角色升级时的信息，可以在蓝图中使用
USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	// 升级所需的经验值或条件，默认值为0，表示没有特殊要求
	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	// 升级时获得的属性点数，默认值为1
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 1;

	// 升级时获得的技能点数，默认值为1
	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointAward = 1;
};


/**
 * 
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
public:

	// 属性：升级信息数组，包含每个等级的升级要求和相关奖励，只能在默认值编辑器中编辑
	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraLevelUpInfo> LevelUpInformation;

	// 函数：根据给定的经验值查找对应的等级
	// @param XP 经验值
	// @return 返回达到给定经验值时的等级，如果经验值不足以达到任何等级，则返回0或相应的逻辑值
	int32 FindLevelForXP(int32 XP) const;

};
