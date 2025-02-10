// Copyright RickYan


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"

FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	// 返回能力的描述，包括默认名称和等级
	// 使用富文本格式，<Default>表示默认文本样式，<Level>表示等级文本样式
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum", Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	// 返回下一等级的能力描述，包括等级和提升效果
	// 使用富文本格式，<Default>表示默认文本样式，<Level>表示等级文本样式
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage. </>"), Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	// 返回能力锁定的描述，包括解锁所需的等级
	// 使用富文本格式，<Default>表示默认文本样式
	return FString::Printf(TEXT("<Default>Spell Locked Until Level: %d</>"), Level);
}

float UAuraGameplayAbility::GetManaCost(float InLevel) const
{
	// 根据等级获取法力消耗
	float ManaCost = 0.f;
	// 获取消耗游戏效果
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		// 遍历游戏效果的修改器信息
		for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
		{
			// 如果修改器对应的是法力属性
			if (Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				// 获取静态幅度值作为法力消耗
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(float InLevel) const
{
	// 根据等级获取冷却时间
	float Cooldown = 0.f;
	// 获取冷却游戏效果
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		// 获取静态幅度值作为冷却时间
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}
	return Cooldown;
}

