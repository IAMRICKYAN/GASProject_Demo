// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "SpellMenuWidgetController.generated.h"

// 声明一个动态多播委托，用于处理法术球被选中的事件，包含四个参数：是否选中、点数消耗按钮是否启用、装备按钮是否启用、描述字符串、下一个等级描述字符串
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled, FString, DescriptionString, FString, NextLevelDescriptionString);

// 声明一个动态多播委托，用于处理等待装备选择的事件，包含一个参数：游戏玩法标签和能力类型
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);

// 声明一个动态多播委托，用于处理法术球重新分配的事件，包含一个参数：游戏玩法标签和能力标签
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeReassignedSignature, const FGameplayTag&, AbilityTag);

// 定义一个结构体，用于存储选中的能力信息，包含两个游戏玩法标签：能力和状态
struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag(); // 能力标签
	FGameplayTag Status = FGameplayTag(); // 状态标签
};


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	// 虚函数，用于广播初始值，覆盖基类中的实现
	virtual void BroadcastInitialValues() override;
	// 虚函数，用于将回调绑定到依赖项，覆盖基类中的实现
	virtual void BindCallbacksToDependencies() override;

	// 蓝图可分配属性，用于玩家属性变化时触发的事件
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SpellPointsChanged;

	// 蓝图可分配属性，用于法术球被选中时触发的事件
	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;

	// 蓝图可分配属性，用于等待装备选择时触发的事件
	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature WaitForEquipDelegate;

	// 蓝图可分配属性，用于停止等待装备选择时触发的事件
	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature StopWaitingForEquipDelegate;

	// 蓝图可分配属性，用于法术球重新分配时触发的事件
	UPROPERTY(BlueprintAssignable)
	FSpellGlobeReassignedSignature SpellGlobeReassignedDelegate;

	// 蓝图可调用函数，用于处理法术球被选中的情况
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	// 蓝图可调用函数，用于处理点数消耗按钮被按下的事件
	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();

	// 蓝图可调用函数，用于处理法术球取消选中的事件
	UFUNCTION(BlueprintCallable)
	void GlobeDeselect();

	// 蓝图可调用函数，用于处理装备按钮被按下的事件
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();

	// 蓝图可调用函数，用于处理法术行中的球被按下的事件
	UFUNCTION(BlueprintCallable)
	void SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);

	// 函数，用于处理能力被装备的事件
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

private:
	
	// 静态函数，用于确定是否启用按钮
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton);
	// 选中的能力信息，初始化为无能力和锁定状态
	FSelectedAbility SelectedAbility = { FAuraGameplayTags::Get().Abilities_None,  FAuraGameplayTags::Get().Abilities_Status_Locked };
	// 当前法术点数
	int32 CurrentSpellPoints = 0;
	// 是否正在等待装备选择
	bool bWaitingForEquipSelection = false;
	// 选中的槽位标签
	FGameplayTag SelectedSlot;

};

