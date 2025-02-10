// Copyright RickYan


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

// UAttributeMenuWidgetController类成员函数实现

// 绑定回调函数到依赖的对象上
void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	// 确保AttributeInfo已正确初始化
	check(AttributeInfo);
	// 遍历属性标签到属性的映射
	for (auto& Pair : GetAuraAS()->TagsToAttributes)
	{
		// 为每个属性添加值变化时的回调
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				// 当属性值变化时，广播属性信息
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
	
	// 添加属性点变化时的回调
	GetAuraPS()->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 Points)
		{
			// 当属性点变化时，广播属性点变化委托
			AttributePointsChangedDelegate.Broadcast(Points);
		}
	);
}

// 广播初始值
void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	// 获取并检查Aura属性集
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	// 遍历属性标签到属性的映射，广播每个属性的初始信息
	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	
	// 广播初始属性点
	AttributePointsChangedDelegate.Broadcast(GetAuraPS()->GetAttributePoints());
}

// 升级指定标签的属性
void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	// 获取并检查Aura能力系统组件
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	// 调用能力系统组件的升级属性函数
	AuraASC->UpgradeAttribute(AttributeTag);
}

// 广播属性信息
void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	// 根据属性标签查找属性信息
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	// 更新属性值为当前属性集的数值
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	// 广播属性信息委托
	AttributeInfoDelegate.Broadcast(Info);
}
