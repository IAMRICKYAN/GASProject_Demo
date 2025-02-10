// Copyright RickYan


#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"


UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;
}

void UDebuffNiagaraComponent::BeginPlay()
{
	// 调用基类的BeginPlay函数
	Super::BeginPlay();

	// 尝试将拥有者转换为战斗接口
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	// 如果获取到能力系统组件
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		// 注册游戏玩法标签事件，当DebuffTag被添加或移除时，调用DebuffTagChanged函数
		ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);
	}
	// 如果没有获取到能力系统组件，但是拥有者实现了战斗接口
	else if (CombatInterface)
	{
		// 当能力系统组件注册时，添加一个弱绑定lambda表达式，注册游戏玩法标签事件
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC)
		{
			InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);
		});
	}
}


// 当减益标签发生变化时调用此函数
void UDebuffNiagaraComponent::DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	// 检查拥有者是否有效
	const bool bOwnerValid = IsValid(GetOwner());
	// 检查拥有者是否实现了战斗接口，并且不是死亡状态
	const bool bOwnerAlive = GetOwner()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(GetOwner());
	
	// 如果新计数大于0，拥有者有效且拥有者未死亡
	if (NewCount > 0 && bOwnerValid && bOwnerAlive)
	{
		// 激活减益Niagara组件
		Activate();
	}
	else
	{
		// 否则，关闭减益Niagara组件
		Deactivate();
	}
}
