// Copyright RickYan


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	// 初始化VigorDef，设置要捕获的属性为UAuraAttributeSet::GetVigorAttribute()
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	// 设置属性捕获源为目标的属性
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	// 设置不使用快照
	VigorDef.bSnapshot = false;

	// 将VigorDef添加到相关属性捕获列表中
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 从效果规格中获取来源和目标的标签
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 创建评估参数，包含来源和目标的标签
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// 初始化Vigor变量，用于存储捕获的属性值
	float Vigor = 0.f;
	// 获取捕获的Vigor属性值
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	// 确保Vigor值不为负
	Vigor = FMath::Max<float>(Vigor, 0.f);

	// 初始化玩家等级为1
	int32 PlayerLevel = 1;
	// 检查来源对象是否实现了UCombatInterface接口
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		// 如果实现了，获取玩家等级
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}

	// 根据Vigor值和玩家等级计算基础幅度值，并返回
	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}

