// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ScalableFloat.h"
#include "CharacterClassInfo.generated.h"


class UGameplayEffect;
class UGameplayAbility;

// 枚举类型，用于定义角色职业，可以在蓝图中使用
UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist, // 元素使
	Warrior,      // 战士
	Ranger        // 游侠
};

// 结构体，用于存储每个角色职业的默认信息，可以在蓝图中使用
USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	// 主要属性的游戏效果类，只能在默认值编辑器中编辑，归类在"Class Defaults"类别下
	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	// 初始能力的游戏能力类数组，只能在默认值编辑器中编辑，归类在"Class Defaults"类别下
	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	// 经验奖励的可缩放浮点数，默认值为FScalableFloat的默认构造值，只能在默认值编辑器中编辑，归类在"Class Defaults"类别下
	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	FScalableFloat XPReward = FScalableFloat();
};


/**
 * 
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	// 属性，用于存储不同角色职业的默认信息映射，只能在默认值编辑器中编辑，归类在"Character Class Defaults"类别下
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;

	// 属性，用于存储由调用者设置的主要属性游戏效果类，只能在默认值编辑器中编辑，归类在"Common Class Defaults"类别下
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes_SetByCaller;

	// 属性，用于存储次要属性游戏效果类，只能在默认值编辑器中编辑，归类在"Common Class Defaults"类别下
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	// 属性，用于存储无限次要属性游戏效果类，只能在默认值编辑器中编辑，归类在"Common Class Defaults"类别下
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes_Infinite;

	// 属性，用于存储生命属性游戏效果类，只能在默认值编辑器中编辑，归类在"Common Class Defaults"类别下
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	// 属性，用于存储通用游戏能力类数组，只能在默认值编辑器中编辑，归类在"Common Class Defaults"类别下
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	// 属性，用于存储伤害计算系数的曲线表，只能在默认值编辑器中编辑，归类在"Common Class Defaults|Damage"类别下
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;

	// 函数，用于获取指定角色职业的默认信息
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);

};
