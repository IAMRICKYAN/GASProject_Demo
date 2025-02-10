// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);
/**
 * 
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
public:

	// 定义一个可以在蓝图中调用的函数，用于创建一个目标数据对象，该对象表示鼠标下的目标数据
	// Category指定了在蓝图中的分类
	// DisplayName是蓝图中的显示名称
	// HidePin隐藏了OwningAbility参数的引脚，因为它默认为自己
	// DefaultToSelf指定OwningAbility参数默认为调用该函数的对象自身
	// BlueprintInternalUseOnly表示这个函数是为蓝图内部使用而设计的，不建议直接从蓝图调用
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	// 定义一个可以在蓝图中分配的属性，用于存储鼠标目标数据的签名
	// BlueprintAssignable表示这个属性可以在蓝图中被分配（即可以绑定事件）
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;


private:

	// 虚函数，用于激活组件或行为，重写基类的Activate方法
	virtual void Activate() override;

	// 函数，用于发送鼠标光标数据
	void SendMouseCursorData();

	// 回调函数，当目标数据被复制时调用，处理复制的数据和激活标签
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);

};
