// Copyright RickYan


#include "AbilitySystem/Data/CharacterClassInfo.h"

// 获取指定角色职业的默认信息
// @param CharacterClass 要查询的角色职业枚举值
// @return 返回对应角色职业的默认信息，如果未找到则触发断言错误
FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}

