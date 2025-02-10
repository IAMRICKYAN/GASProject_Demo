// Copyright RickYan


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	// 广播初始健康值
	OnHealthChanged.Broadcast(GetAuraAS()->GetHealth());
	// 广播初始最大健康值
	OnMaxHealthChanged.Broadcast(GetAuraAS()->GetMaxHealth());
	// 广播初始法力值
	OnManaChanged.Broadcast(GetAuraAS()->GetMana());
	// 广播初始最大法力值
	OnMaxManaChanged.Broadcast(GetAuraAS()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// 绑定经验值变化回调
	GetAuraPS()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	// 绑定等级变化回调
	GetAuraPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel, bool bLevelUp)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel, bLevelUp);
		}
	);

	// 绑定健康值变化回调
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

	// 绑定最大健康值变化回调
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	// 绑定法力值变化回调
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);

	// 绑定最大法力值变化回调
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);

	// 如果获取到了Aura的能力系统组件
	if (GetAuraASC())
	{
		// 绑定能力装备回调
		GetAuraASC()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		// 如果初始能力已经赋予，则广播能力信息
		if (GetAuraASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			// 否则，绑定能力赋予回调
			GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}

		// 绑定效果资产标签回调
		GetAuraASC()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					// 检查标签是否匹配消息标签
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						// 获取数据表行并通过标签匹配消息小部件行
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						// 广播消息小部件行
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}
}


void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	// 获取玩家状态中的等级提升信息
	const ULevelUpInfo* LevelUpInfo = GetAuraPS()->LevelUpInfo;
	// 确保等级提升信息存在，如果不存在则报错
	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo. Please fill out AuraPlayerState Blueprint"));

	// 根据新的经验值计算当前等级
	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	// 获取最高等级
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	// 检查当前等级是否在有效范围内
	if (Level <= MaxLevel && Level > 0)
	{
		// 获取当前等级升级所需的经验值
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		// 获取上一等级升级所需的经验值
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		// 计算两等级间所需的经验值差
		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		// 计算当前等级已获得的经验值
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		// 计算经验条百分比
		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		// 广播经验条百分比变化
		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}


void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	// 获取游戏玩法标签的实例，用于访问预定义的标签
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	
	// 创建一个表示上一个插槽的信息结构体，初始化为未解锁状态和无能力
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked; // 设置状态标签为未解锁
	LastSlotInfo.InputTag = PreviousSlot; // 设置输入标签为上一个插槽的标签
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None; // 设置能力标签为无能力
	// 如果PreviousSlot是一个有效的插槽，并且正在装备的技能已经装备在另一个插槽上，则广播空信息
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	// 根据提供的能力标签查找对应的能力信息
	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status; // 更新能力信息的状态标签
	Info.InputTag = Slot; // 更新能力信息的输入标签
	// 广播更新后的能力信息
	AbilityInfoDelegate.Broadcast(Info);
}
