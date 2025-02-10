// Copyright RickYan


#include "AbilitySystem/Abilities/AuraPassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
										  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
										  const FGameplayEventData* TriggerEventData)
{
	// 调用基类的ActivateAbility函数，确保执行基类的激活逻辑
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 尝试获取Avatar演员的Aura能力系统组件
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		// 将当前对象的ReceiveDeactivate函数注册到Aura能力系统组件的DeactivatePassiveAbility事件中
		// 这样当需要停用被动能力时，ReceiveDeactivate会被调用
		AuraASC->DeactivatePassiveAbility.AddUObject(this, &UAuraPassiveAbility::ReceiveDeactivate);
	}
}

void UAuraPassiveAbility::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
	// 检查传入的AbilityTag是否与当前能力的标签完全匹配
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		// 如果标签匹配，结束当前能力
		// 参数说明：
		// - CurrentSpecHandle：当前能力的规格句柄
		// - CurrentActorInfo：当前能力的演员信息
		// - CurrentActivationInfo：当前能力的激活信息
		// - true：表示能力被正常结束
		// - true：表示应该向客户端发送能力结束的通知
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

