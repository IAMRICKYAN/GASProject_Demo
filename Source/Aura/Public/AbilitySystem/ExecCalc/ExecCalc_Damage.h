// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecCalc_Damage();

	/**
	 * 确定 debuff 效果。
	 * 
	 * 本函数用于根据提供的参数确定 debuff 效果的具体执行细节和结果。它通过分析游戏效果的执行参数、效果的规格说明、
	 * 评估参数以及标签到属性的捕获定义映射来计算和应用 debuff 效果。
	 * 
	 * @param ExecutionParams 游戏效果的自定义执行参数，包含执行过程中的上下文信息。
	 * @param Spec 游戏效果的规格说明，详细描述了效果的特性。
	 * @param EvaluationParameters 用于评估属性的参数，帮助确定效果如何应用。
	 * @param InTagsToDefs 标签到属性捕获定义的映射，用于理解哪些属性与特定标签相关联。
	 */
	void DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                     const FGameplayEffectSpec& Spec,
	                     FAggregatorEvaluateParameters EvaluationParameters,
	                     const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const;

	// 虚函数：执行具体的实现逻辑
	// 参数：
	// - ExecutionParams：游戏效果自定义执行参数，包含执行所需的各种参数信息
	// - OutExecutionOutput：游戏效果自定义执行输出，用于存储执行后的结果信息
	// 函数功能：根据提供的执行参数，执行特定的游戏效果逻辑，并将执行结果输出
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
