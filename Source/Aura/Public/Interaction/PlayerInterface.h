// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// 定义一个蓝图原生事件函数，用于根据输入的经验值（InXP）查找对应的等级
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 InXP) const;

	// 定义一个蓝图原生事件函数，用于获取当前经验值
	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;

	// 定义一个蓝图原生事件函数，用于根据等级（Level）获取奖励的属性点数
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 Level) const;

	// 定义一个蓝图原生事件函数，用于根据等级（Level）获取奖励的技能点数
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(int32 Level) const;

	// 定义一个蓝图原生事件函数，用于给玩家添加经验值（InXP）
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);

	// 定义一个蓝图原生事件函数，用于给玩家添加等级（InPlayerLevel）
	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 InPlayerLevel);

	// 定义一个蓝图原生事件函数，用于给玩家添加属性点数（InAttributePoints）
	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(int32 InAttributePoints);

	// 定义一个蓝图原生事件函数，用于获取玩家的属性点数
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints() const;

	// 定义一个蓝图原生事件函数，用于给玩家添加技能点数（InSpellPoints）
	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoints(int32 InSpellPoints);

	// 定义一个蓝图原生事件函数，用于获取玩家的技能点数
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints() const;

	// 定义一个蓝图原生事件函数，用于执行玩家升级操作
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	// 定义一个蓝图原生事件和可调用函数，用于显示魔法圈，可以传入一个贴图材质（DecalMaterial）
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	// 定义一个蓝图原生事件和可调用函数，用于隐藏魔法圈
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideMagicCircle();

	// 定义一个蓝图原生事件和可调用函数，用于保存游戏进度，传入一个检查点标签（CheckpointTag）
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SaveProgress(const FName& CheckpointTag);

};
