// Copyright RickYan


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"
#include "AbilitySystemComponent.h"

// 静态工厂方法，用于创建并初始化UWaitCooldownChange实例，以等待冷却时间变化
UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag)
{
	// 创建一个新的UWaitCooldownChange对象
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	// 设置能力系统组件和冷却标签
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;
	
	// 检查能力系统组件和冷却标签是否有效
	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		// 如果无效，结束任务并返回nullptr
		WaitCooldownChange->EndTask();
		return nullptr;
	}

	// 注册游戏玩法标签事件，以知道何时冷却结束（冷却标签被移除）
	AbilitySystemComponent->RegisterGameplayTagEvent(
		InCooldownTag,
		EGameplayTagEventType::NewOrRemoved).AddUObject(
			WaitCooldownChange,
			&UWaitCooldownChange::CooldownTagChanged);

	// 注册活跃游戏效果添加事件，以知道何时应用了冷却效果
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &UWaitCooldownChange::OnActiveEffectAdded);

	// 返回创建的UWaitCooldownChange实例
	return WaitCooldownChange;
}

// 结束任务，清理资源
void UWaitCooldownChange::EndTask()
{
	// 检查能力系统组件是否有效
	if (!IsValid(ASC)) return;
	// 移除所有与冷却标签相关的事件绑定
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	// 标记对象为准备销毁并标记为垃圾
	SetReadyToDestroy();
	MarkAsGarbage();
}

// 冷却标签变化时的回调函数
void UWaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	// 如果新的标签数量为0，表示冷却结束
	if (NewCount == 0)
	{
		// 广播冷却结束事件，传递剩余时间为0
		CooldownEnd.Broadcast(0.f);
	}
}


// 当一个新的活跃游戏效果被添加到目标能力系统组件时调用的回调函数
void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	// 获取应用的游戏效果规范中的所有资产标签
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	// 获取应用的游戏效果规范中授予的所有标签
	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	// 检查资产标签或授予的标签中是否包含特定的冷却标签
	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		// 创建一个游戏效果查询，用于匹配拥有特定冷却标签的活跃游戏效果
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		// 获取与查询匹配的活跃游戏效果的剩余时间数组
		TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		// 如果有匹配的游戏效果
		if (TimesRemaining.Num() > 0)
		{
			// 初始化剩余时间变量为第一个元素
			float TimeRemaining = TimesRemaining[0];
			// 遍历剩余时间数组，找到最长的剩余时间
			for (int32 i = 0; i < TimesRemaining.Num(); i++)
			{
				if (TimesRemaining[i] > TimeRemaining)
				{
					TimeRemaining = TimesRemaining[i];
				}
			}
			
			// 广播冷却开始事件，传递最长的剩余时间
			CooldownStart.Broadcast(TimeRemaining);
		}
	}
}
