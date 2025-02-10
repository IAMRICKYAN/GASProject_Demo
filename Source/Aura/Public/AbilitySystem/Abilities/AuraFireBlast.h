// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraFireBlast.generated.h"

class AAuraFireBall;
/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	// 虚函数，用于获取当前等级的能力描述，重写基类中的同名函数
	virtual FString GetDescription(int32 Level) override;
	// 虚函数，用于获取下一等级的能力描述，重写基类中的同名函数
	virtual FString GetNextLevelDescription(int32 Level) override;

	// UFUNCTION标签表示该函数可以在蓝图中被调用
	UFUNCTION(BlueprintCallable)
	// 函数，用于生成火球数组
	TArray<AAuraFireBall*> SpawnFireBalls();
protected:
	// UPROPERTY标签表示该属性可以在编辑器的默认设置中编辑，分类为"FireBlast"
	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	// 保护成员变量，表示生成的火球数量，默认值为12
	int32 NumFireBalls = 12;

private:
	// UPROPERTY标签表示该属性可以在编辑器的默认设置中编辑
	UPROPERTY(EditDefaultsOnly)
	// 私有成员变量，表示火球的类对象，用于实例化火球
	TSubclassOf<AAuraFireBall> FireBallClass;

};
