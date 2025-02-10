// Copyright RickYan


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

// 在UAbilityInfo类中查找与给定能力标签匹配的能力信息
// @param AbilityTag 要查找的能力标签
// @param bLogNotFound 如果为true，当没有找到对应的能力信息时，会在日志中记录错误
// @return 返回找到的FAuraAbilityInfo对象，如果未找到则返回一个默认构造的FAuraAbilityInfo对象
FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	// 遍历AbilityInformation数组，寻找匹配的能力标签
	for (const FAuraAbilityInfo& Info : AbilityInformation)
	{
		// 如果找到匹配的能力标签，返回对应的能力信息
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	// 如果没有找到匹配的能力标签，并且bLogNotFound为true，记录错误日志
	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}

	// 如果没有找到匹配的能力标签，返回一个默认构造的FAuraAbilityInfo对象
	return FAuraAbilityInfo();
}

