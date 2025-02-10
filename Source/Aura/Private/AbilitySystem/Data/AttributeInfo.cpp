// Copyright RickYan


#include "AbilitySystem/Data/AttributeInfo.h"

#include "Aura/AuraLogChannels.h"

// 在UAttributeInfo类中查找与给定属性标签精确匹配的属性信息
// @param AttributeTag 要查找的属性标签
// @param bLogNotFound 如果为true，当没有找到对应的属性信息时，会在日志中记录错误
// @return 返回找到的FAuraAttributeInfo对象，如果未找到则返回一个默认构造的FAuraAttributeInfo对象
FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	// 遍历属性信息数组，寻找与给定标签精确匹配的属性信息
	for (const FAuraAttributeInfo& Info : AttributeInformation)
	{
		// 如果找到精确匹配的属性标签，则返回该属性信息
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	// 如果没有找到匹配的属性信息，并且bLogNotFound为true，则记录错误日志
	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	// 如果没有找到匹配的属性信息，返回一个默认构造的FAuraAttributeInfo对象
	return FAuraAttributeInfo();
}
