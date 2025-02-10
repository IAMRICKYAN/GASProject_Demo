// Copyright RickYan


#include "Input/AuraInputConfig.h"

/**
 * 在 AbilityInputActions 数组中查找与给定 InputTag 匹配的输入动作。
 *
 * @param InputTag 要查找的输入标签。
 * @param bLogNotFound 如果为 true，则在找不到匹配项时记录错误日志。
 * @return 返回匹配的输入动作指针，如果未找到则返回 nullptr。
 */
const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	// 遍历 AbilityInputActions 数组
	for (const FAuraInputAction& Action : AbilityInputActions)
	{
		// 检查输入动作是否有效且输入标签是否匹配
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			// 如果找到匹配的输入动作，返回该动作的指针
			return Action.InputAction;
		}
	}

	// 如果设置了 bLogNotFound 且未找到匹配的输入动作，则记录错误日志
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}

	// 如果未找到匹配的输入动作，返回 nullptr
	return nullptr;
}
