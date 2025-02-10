// Copyright RickYan


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Camera/CameraShakeSourceActor.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

// Aura伤害静态数据结构体
struct AuraDamageStatics
{
	// 属性捕获定义
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor); // 护甲
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration); // 护甲穿透
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance); // 格挡几率
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance); // 暴击几率
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance); // 暴击抵抗
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage); // 暴击伤害
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance); // 火焰抗性
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance); // 闪电抗性
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance); // 奥术抗性
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance); // 物理抗性
	
	// 构造函数，用于初始化属性捕获定义
	AuraDamageStatics()
	{
		// 定义属性捕获，指定属性集、属性、捕获对象（目标或来源）以及是否为可选属性
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false); // 目标护甲
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false); // 目标格挡几率
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false); // 来源护甲穿透
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false); // 来源暴击几率
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false); // 目标暴击抵抗
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false); // 来源暴击伤害

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false); // 目标火焰抗性
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false); // 目标闪电抗性
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false); // 目标奥术抗性
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false); // 目标物理抗性
	}
};


// 静态函数：获取Aura伤害静态数据实例
// 返回值：Aura伤害静态数据的常量引用
// 函数功能：提供一个全局访问点来获取Aura伤害相关的静态数据，确保整个程序中使用的都是同一份实例
static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics; // 定义一个静态的Aura伤害静态数据实例
	return DStatics; // 返回该实例的常量引用
}


// UExecCalc_Damage的构造函数
UExecCalc_Damage::UExecCalc_Damage()
{
	// 添加相关属性到捕获列表，这些属性将用于伤害计算
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef); // 护甲
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef); // 格挡几率
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef); // 护甲穿透
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef); // 暴击几率
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef); // 暴击抵抗
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef); // 暴击伤害

	// 添加元素抗性到捕获列表
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef); // 火焰抗性
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef); // 闪电抗性
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef); // 奥术抗性
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef); // 物理抗性
}


// 函数：确定并应用减益效果
// 参数：
// - ExecutionParams：游戏效果自定义执行参数，包含执行所需的各种参数信息
// - Spec：游戏效果规格，描述了游戏效果的详细信息
// - EvaluationParameters：聚合评估参数，用于评估属性值
// - InTagsToDefs：标签到属性捕获定义的映射，用于根据标签获取属性捕获定义
// 函数功能：根据伤害类型和减益几率，确定是否对目标应用减益效果，并设置相关的减益参数
void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters,
						 const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get(); // 获取游戏标签实例

	// 遍历伤害类型到减益类型的映射
	for (TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key; // 伤害类型标签
		const FGameplayTag& DebuffType = Pair.Value; // 减益类型标签
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f); // 获取该伤害类型的伤害值
		if (TypeDamage > -.5f) // 检查伤害值是否有效（考虑浮点数精度）
		{
			// 确定是否成功减益
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance, false, -1.f); // 获取减益几率

			float TargetDebuffResistance = 0.f; // 目标减益抵抗
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType]; // 获取对应的抵抗标签
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ResistanceTag], EvaluationParameters, TargetDebuffResistance); // 计算目标减益抵抗值
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f); // 确保抵抗值不小于0
			const float EffectiveDebuffChance = SourceDebuffChance * ( 100 - TargetDebuffResistance ) / 100.f; // 计算有效减益几率
			const bool bDebuff = FMath::RandRange(1, 100) < EffectiveDebuffChance; // 随机确定是否减益
			if (bDebuff)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext(); // 获取游戏效果上下文句柄

				// 设置减益成功的相关参数
				UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle, true);
				UAuraAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);

				// 获取并设置减益伤害、持续时间及频率
				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Damage, false, -1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Duration, false, -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Frequency, false, -1.f);

				UAuraAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UAuraAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
				UAuraAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
			}
		}
	}
}


void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
		// 创建一个映射，用于存储需要捕获的属性标签和对应的属性捕获定义
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	// 获取游戏玩法标签实例
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
	
	// 将 secondary 属性（如护甲、格挡几率、护甲穿透、暴击几率等）和 resistance 属性（如奥术抗性、火焰抗性、闪电抗性、物理抗性等）的标签及其捕获定义添加到映射中
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, DamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, DamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, DamageStatics().CriticalHitResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, DamageStatics().CriticalHitDamageDef);
	
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, DamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, DamageStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, DamageStatics().PhysicalResistanceDef);
	
	// 获取源和目标的能力系统组件
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	// 获取源和目标的化身演员
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	
	// 获取源和目标的玩家等级，如果化身演员实现了 UCombatInterface 接口
	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}
	
	// 获取游戏效果规格和上下文句柄
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	
	// 获取源和目标的标签容器
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	// 创建聚合器评估参数，用于后续的属性计算
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	// 调用 DetermineDebuff 函数来确定并应用减益效果
	// 参数包括执行参数、游戏效果规格、聚合器评估参数和属性标签到捕获定义的映射
	DetermineDebuff(ExecutionParams, Spec, EvaluationParameters, TagsToCaptureDefs);


	// 获取通过SetByCaller设置的伤害值
	float Damage = 0.f;

	// 遍历所有伤害类型与抗性类型的映射关系
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		// 获取当前伤害类型的Tag
		const FGameplayTag DamageTypeTag = Pair.Key;
    
		// 获取与伤害类型对应的抗性类型的Tag
		const FGameplayTag ResistanceTag = Pair.Value;
    
		// 检查TagsToCaptureDefs是否包含当前抗性类型的Tag，如果不包含则触发断言
		checkf(TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
    
		// 获取抗性类型的捕获定义
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ResistanceTag];

		// 获取当前伤害类型的伤害值，如果未找到则返回0
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false);
    
		// 如果伤害值小于等于0，跳过当前循环
		if (DamageTypeValue <= 0.f)
		{
			continue;
		}
    
		// 初始化抗性值为0
		float Resistance = 0.f;
    
		// 尝试计算捕获的抗性属性值
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
    
		// 将抗性值限制在0到100之间
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		// 根据抗性值调整伤害值
		DamageTypeValue *= (100.f - Resistance) / 100.f;
	

		// 检查是否是范围伤害（Radial Damage）
		if (UAuraAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		{
			// 以下是实现范围伤害的逻辑步骤：
			// 1. 在 AuraCharacterBase 中重写 TakeDamage 函数。 *
			// 2. 创建一个委托 OnDamageDelegate，在 TakeDamage 中广播接收到的伤害值。 *
			// 3. 在这里将 Lambda 函数绑定到受害者的 OnDamageDelegate 上。 *
			// 4. 调用 UGameplayStatics::ApplyRadialDamageWithFalloff 来施加伤害（这将导致受害者的 TakeDamage 被调用，
			//    从而触发 OnDamageDelegate 的广播）。
			// 5. 在 Lambda 函数中，将从广播中接收到的伤害值赋给 DamageTypeValue。 *

			// 检查目标是否实现了 ICombatInterface 接口
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar))
			{
				// 将 Lambda 函数绑定到目标的 OnDamageDelegate 上
				CombatInterface->GetOnDamageSignature().AddLambda([&](float DamageAmount)
				{
					// 当接收到伤害广播时，将伤害值赋给 DamageTypeValue
					DamageTypeValue = DamageAmount;
				});
			}

			// 使用 UGameplayStatics::ApplyRadialDamageWithFalloff 施加范围伤害
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				TargetAvatar, // 伤害施加的目标
				DamageTypeValue, // 基础伤害值
				0.f, // 最小伤害值
				UAuraAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle), // 范围伤害的中心点
				UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle), // 内半径（完全伤害区域）
				UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle), // 外半径（伤害衰减区域）
				1.f, // 伤害衰减曲线指数
				UDamageType::StaticClass(), // 伤害类型
				TArray<AActor*>(), // 忽略的 Actor 列表
				SourceAvatar, // 伤害来源
				nullptr // 额外的伤害计算参数
			);
		}
		
		Damage += DamageTypeValue;
	}

	// 捕获目标的格挡几率，并判断是否成功格挡
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f); // 确保格挡几率不小于0
	
	// 判断是否成功格挡：随机生成一个1到100的数，如果小于格挡几率，则格挡成功
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	
	// 将格挡结果设置到效果上下文中
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	
	// 如果格挡成功，伤害减半
	Damage = bBlocked ? Damage / 2.f : Damage;
	
	// 捕获目标的护甲值
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f); // 确保护甲值不小于0
	
	// 捕获攻击者的护甲穿透值
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f); // 确保护甲穿透值不小于0
	
	// 获取角色职业信息
	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	
	// 获取护甲穿透系数曲线
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel); // 根据攻击者等级计算护甲穿透系数
	
	// 计算有效护甲值：护甲穿透忽略目标护甲的一定百分比
	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
	
	// 获取有效护甲系数曲线
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel); // 根据目标等级计算有效护甲系数
	
	// 根据有效护甲值减少伤害：护甲忽略一定百分比的伤害
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;
	
	// 捕获攻击者的暴击几率
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f); // 确保暴击几率不小于0
	
	// 捕获目标的暴击抗性
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f); // 确保暴击抗性不小于0
	
	// 捕获攻击者的暴击伤害
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f); // 确保暴击伤害不小于0
	
	// 获取暴击抗性系数曲线
	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetPlayerLevel); // 根据目标等级计算暴击抗性系数
	
	// 计算有效暴击几率：暴击抗性减少暴击几率的一定百分比
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	
	// 判断是否触发暴击：随机生成一个1到100的数，如果小于有效暴击几率，则触发暴击
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;
	
	// 将暴击结果设置到效果上下文中
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	
	// 如果触发暴击，伤害翻倍并加上暴击伤害加成
	Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;
	
	// 创建一个伤害修饰器，将最终伤害值添加到输出中
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
