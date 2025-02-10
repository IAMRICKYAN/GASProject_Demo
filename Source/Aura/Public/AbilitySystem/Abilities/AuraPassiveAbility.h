// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraPassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraPassiveAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// ActivateAbility函数是Gameplay Ability System中用于激活能力的虚函数。
	// 参数说明：
	// - Handle：能力的规格句柄，用于唯一标识这个能力实例。
	// - ActorInfo：包含激活能力的演员信息，如拥有者、角色等。
	// - ActivationInfo：包含激活能力的详细信息，如激活方式、是否是重复激活等。
	// - TriggerEventData：触发事件的数据，包含事件相关的信息。
	// 这个函数被标记为override，表示它重写了基类中的同名函数。

	void ReceiveDeactivate(const FGameplayTag& AbilityTag);

	// ReceiveDeactivate函数是一个事件接收函数，当能力被停用时会被调用。
	// 参数说明：
	// - AbilityTag：能力的标签，用于标识这个能力。
	// 这个函数不是虚函数，但它可以被 Blueprint 调用，用于处理能力停用时的逻辑。

};
