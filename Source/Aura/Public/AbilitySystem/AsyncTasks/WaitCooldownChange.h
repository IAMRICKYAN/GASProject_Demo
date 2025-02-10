// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "WaitCooldownChange.generated.h"

class UAbilitySystemComponent;
struct FGameplayEffectSpec;

// 声明一个动态多播委托，该委托具有一个参数，用于通知冷却时间变化
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);


/**
 * 
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class AURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	// 声明一个可在蓝图中分配的多播委托，用于通知冷却开始时的事件
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;

	// 声明一个可在蓝图中分配的多播委托，用于通知冷却结束时的事件
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;

	// 声明一个蓝图可调用的静态函数，用于等待冷却时间变化
	// 该函数创建并返回一个UWaitCooldownChange任务对象，该对象监视指定的冷却标签
	// AbilitySystemComponent: 能力系统组件指针，用于管理能力相关的冷却
	// InCooldownTag: 要监视的冷却标签
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);

	// 声明一个蓝图可调用的函数，用于结束当前任务
	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:

	// 声明一个属性，用于存储指向能力系统组件的弱指针
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	// 声明一个游戏玩法标签，用于标识特定的冷却
	FGameplayTag CooldownTag;

	// 声明一个函数，当冷却标签的数量发生变化时调用
	// InCooldownTag: 发生变化的冷却标签
	// NewCount: 新的标签数量
	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);

	// 声明一个函数，当新的活跃游戏效果被添加到目标能力系统组件时调用
	// TargetASC: 目标能力系统组件指针
	// SpecApplied: 应用到的游戏效果规格
	// ActiveEffectHandle: 活跃游戏效果的句柄
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle);

};
