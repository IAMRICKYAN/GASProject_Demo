// Copyright RickYan


#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

// 在游戏开始时调用的函数，用于初始化和设置被动效果的激活
void UPassiveNiagaraComponent::BeginPlay()
{
	// 调用基类的BeginPlay函数
	Super::BeginPlay();

	// 尝试获取拥有者的AuraAbilitySystemComponent组件
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		// 添加被动效果激活的回调函数
		AuraASC->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
		// 检查并激活已装备的被动效果
		ActivateIfEquipped(AuraASC);
	}
	// 如果没有找到AuraAbilitySystemComponent组件，尝试获取CombatInterface接口
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		// 添加AbilitySystemComponent注册时的回调函数
		CombatInterface->GetOnASCRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* ASC)
		{
			// 再次尝试获取AuraAbilitySystemComponent组件
			if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
			{
				// 添加被动效果激活的回调函数
				AuraASC->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
				// 检查并激活已装备的被动效果
				ActivateIfEquipped(AuraASC);
			}
		});
	}
}

// 检查并激活已装备的被动效果的函数
void UPassiveNiagaraComponent::ActivateIfEquipped(UAuraAbilitySystemComponent* AuraASC)
{
	// 获取启动能力是否已给予的标志
	const bool bStartupAbilitiesGiven = AuraASC->bStartupAbilitiesGiven;
	// 如果启动能力已给予
	if (bStartupAbilitiesGiven)
	{
		// 检查被动技能标签的状态是否为已装备
		if (AuraASC->GetStatusFromAbilityTag(PassiveSpellTag) == FAuraGameplayTags::Get().Abilities_Status_Equipped)
		{
			// 激活被动效果
			Activate();
		}
	}
}

// 被动效果激活的回调函数
void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate)
{
	// 检查能力标签是否与被动技能标签完全匹配
	if (AbilityTag.MatchesTagExact(PassiveSpellTag))
	{
		// 如果应该激活且当前未激活
		if (bActivate && !IsActive())
		{
			// 激活被动效果
			Activate();
		}
		else
		{
			// 否则，停用被动效果
			Deactivate();
		}
	}
}
