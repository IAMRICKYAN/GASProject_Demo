// Copyright RickYan


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	// 广播初始能力信息
	BroadcastAbilityInfo();
	// 广播当前技能点数
	SpellPointsChanged.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	// 绑定能力状态改变时的回调
	GetAuraASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
	{
		// 如果改变的能力是当前选中的能力，则更新选中能力的状态和按钮启用状态
		if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.Status = StatusTag;
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
			FString Description;
			FString NextLevelDescription;
			GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
		}
		
		// 如果有能力信息，则更新并广播能力信息
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	// 绑定能力装备时的回调
	GetAuraASC()->AbilityEquipped.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);

	// 绑定技能点数改变时的回调
	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
	{
		// 广播技能点数改变
		SpellPointsChanged.Broadcast(SpellPoints);
		// 更新当前技能点数
		CurrentSpellPoints = SpellPoints;

		// 根据新的技能点数和选中能力的状态，更新按钮启用状态
		bool bEnableSpendPoints = false;
		bool bEnableEquip = false;
		ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
		FString Description;
		FString NextLevelDescription;
		GetAuraASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
		// 广播法术球选中委托，以反映可能的按钮启用状态变化
		SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
	});
}


void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	// 如果正在等待装备选择，则广播停止等待的委托并更新等待状态
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	// 获取游戏玩法标签的实例和当前技能点
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();	
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;	
	
	// 验证能力标签的有效性
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	// 如果标签无效、为空或能力规范无效，则设置状态为锁定
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		// 否则，从能力规范获取状态
		AbilityStatus = GetAuraASC()->GetStatusFromSpec(*AbilitySpec);
	}

	// 更新选中能力的标签和状态
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;
	// 决定是否启用花费技能点按钮和装备按钮
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquip);
	// 获取能力描述和下一级描述
	FString Description;
	FString NextLevelDescription;
	GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
	// 广播法术球选中委托
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	// 如果有Aura能力系统组件，则尝试在服务器上花费技能点
	if (GetAuraASC())
	{
		GetAuraASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::GlobeDeselect()
{
	// 如果正在等待装备选择，则广播停止等待的委托并更新等待状态
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	// 重置选中能力的信息
	SelectedAbility.Ability = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FAuraGameplayTags::Get().Abilities_Status_Locked;

	// 广播法术球取消选中委托，不启用按钮，不提供描述
	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}


// 装备按钮被按下时的处理函数
void USpellMenuWidgetController::EquipButtonPressed()
{
	// 获取选中的能力的类型
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;

	// 广播等待装备选择的事件
	WaitForEquipDelegate.Broadcast(AbilityType);
	// 设置正在等待装备选择的状态
	bWaitingForEquipSelection = true;

	// 获取选中的能力的状态
	const FGameplayTag SelectedStatus = GetAuraASC()->GetStatusFromAbilityTag(SelectedAbility.Ability);
	// 如果选中的能力状态与已装备的状态匹配
	if (SelectedStatus.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		// 获取选中的槽位
		SelectedSlot = GetAuraASC()->GetSlotFromAbilityTag(SelectedAbility.Ability);
	}
}

// 法术行中的球被按下时的处理函数
void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	// 如果不是正在等待装备选择，则直接返回
	if (!bWaitingForEquipSelection) return;
	// 检查选中的能力是否与槽位的能力类型匹配
	// (不要在被动槽位装备攻击法术，反之亦然)
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

	// 服务器上装备能力
	GetAuraASC()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	// 标记等待装备选择的标志为false，表示装备过程已经开始
	bWaitingForEquipSelection = false;

	// 获取游戏玩法标签的实例
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	
	// 创建一个表示上一个插槽的信息对象，并初始化
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked; // 设置状态标签为“已解锁”
	LastSlotInfo.InputTag = PreviousSlot; // 设置输入标签为上一个插槽的标签
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None; // 设置能力标签为“无能力”
	
	// 如果PreviousSlot是有效的插槽，并且正在装备的技能已经装备在另一个插槽上，则广播空信息
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	// 根据AbilityTag找到对应的能力信息
	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	// 更新能力信息的状态标签和输入标签
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	// 广播更新后的能力信息
	AbilityInfoDelegate.Broadcast(Info);

	// 广播停止等待装备的委托，并传递新装备的能力类型
	StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);
	// 广播法术球重新分配的委托，并传递新装备的能力标签
	SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
	// 取消选择法术球
	GlobeDeselect();
}


void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	// 获取游戏玩法标签的实例
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	// 默认情况下，不启用技能点按钮和装备按钮
	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;

	// 检查能力状态是否为“已装备”
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		// 如果已装备，则启用装备按钮
		bShouldEnableEquipButton = true;
		// 如果有剩余的技能点，则也启用技能点按钮
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	// 检查能力状态是否为“有资格”
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		// 如果有资格并且有剩余的技能点，则启用技能点按钮
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	// 检查能力状态是否为“已解锁”
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		// 如果已解锁，则启用装备按钮
		bShouldEnableEquipButton = true;
		// 如果有剩余的技能点，则也启用技能点按钮
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
}
