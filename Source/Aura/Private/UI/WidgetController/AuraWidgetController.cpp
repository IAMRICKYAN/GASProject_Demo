// Copyright RickYan


#include "UI/WidgetController/AuraWidgetController.h"

#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

// 设置小部件控制器的参数，通过传入的FWidgetControllerParams结构体初始化相关成员变量
void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	// 初始化玩家控制器指针
	PlayerController = WCParams.PlayerController;
	// 初始化玩家状态指针
	PlayerState = WCParams.PlayerState;
	// 初始化能力系统组件指针
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	// 初始化属性集指针
	AttributeSet = WCParams.AttributeSet;
}


void UAuraWidgetController::BroadcastInitialValues()
{
	
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
	
}

// 广播能力信息，将当前所有能力的信息通过委托广播出去
void UAuraWidgetController::BroadcastAbilityInfo()
{
	// 如果启动能力尚未赋予，则不执行广播
	if (!GetAuraASC()->bStartupAbilitiesGiven) return;

	// 创建一个广播委托，用于遍历每个能力
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		// 根据能力标签查找能力信息
		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		// 设置输入标签
		Info.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		// 设置状态标签
		Info.StatusTag = AuraAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
		// 广播能力信息
		AbilityInfoDelegate.Broadcast(Info);
	});
	// 遍历所有能力，执行广播委托
	GetAuraASC()->ForEachAbility(BroadcastDelegate);
}

// 获取Aura玩家控制器指针，如果未初始化则进行类型转换并存储
AAuraPlayerController* UAuraWidgetController::GetAuraPC()
{
	if (AuraPlayerController == nullptr)
	{
		AuraPlayerController = Cast<AAuraPlayerController>(PlayerController);
	}
	return AuraPlayerController;
}

// 获取Aura玩家状态指针，如果未初始化则进行类型转换并存储
AAuraPlayerState* UAuraWidgetController::GetAuraPS()
{
	if (AuraPlayerState == nullptr)
	{
		AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	}
	return AuraPlayerState;
}

// 获取Aura能力系统组件指针，如果未初始化则进行类型转换并存储
UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	}
	return AuraAbilitySystemComponent;
}

// 获取Aura属性集指针，如果未初始化则进行类型转换并存储
UAuraAttributeSet* UAuraWidgetController::GetAuraAS()
{
	if (AuraAttributeSet == nullptr)
	{
		AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	}
	return AuraAttributeSet;
}
