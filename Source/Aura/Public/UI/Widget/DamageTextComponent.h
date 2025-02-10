// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	// 该函数为一个蓝图可执行事件，同时也可以被蓝图调用
	// 用于设置伤害文本的显示，包括伤害值、是否为格挡命中以及是否为暴击
	// 参数:
	// - Damage: float 类型，表示造成的伤害值
	// - bBlockedHit: bool 类型，表示此次攻击是否被格挡
	// - bCriticalHit: bool 类型，表示此次攻击是否为暴击
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage, bool bBlockedHit, bool bCriticalHit);

};
