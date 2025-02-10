// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraBeamSpell.h"
#include "Electrocute.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UElectrocute : public UAuraBeamSpell
{
	GENERATED_BODY()
public:
	// 虚函数，用于获取当前等级的描述信息，重写基类中的同名函数
	virtual FString GetDescription(int32 Level) override;

	// 虚函数，用于获取下一等级的描述信息，重写基类中的同名函数
	virtual FString GetNextLevelDescription(int32 Level) override;

};
