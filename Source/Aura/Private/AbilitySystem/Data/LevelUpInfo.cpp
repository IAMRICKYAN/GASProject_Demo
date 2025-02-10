// Copyright RickYan


#include "AbilitySystem/Data/LevelUpInfo.h"

// ULevelUpInfo类中的成员函数，用于根据给定的经验值查找对应的等级
// @param XP 经验值
// @return 返回达到给定经验值时的等级，如果经验值不足以达到任何等级，则返回当前检查的等级
int32 ULevelUpInfo::FindLevelForXP(int32 XP) const
{
	int32 Level = 1; // 初始化等级为1
	bool bSearching = true; // 控制循环的布尔变量，表示是否继续搜索

	while (bSearching)
	{
		// 检查是否已经达到或超过了数组中的最高等级信息
		if (LevelUpInformation.Num() - 1 <= Level) return Level;

		// 如果当前经验值大于或等于当前等级的升级要求，则继续检查下一个等级
		if (XP >= LevelUpInformation[Level].LevelUpRequirement)
		{
			++Level; // 增加等级，检查下一个等级的要求
		}
		else
		{
			bSearching = false; // 如果当前经验值不满足升级要求，停止搜索
		}
	}
	return Level; // 返回当前等级，即经验值不足以达到的下一个等级
}
