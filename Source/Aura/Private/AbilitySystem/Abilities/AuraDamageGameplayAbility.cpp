// Copyright RickYan


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// 函数，用于对目标演员造成伤害
void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	// 创建伤害效果规范句柄，指定伤害效果类和等级缩放
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	// 获取当前等级的缩放伤害值
	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	// 通过调用者分配标签集和幅度
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);
	// 应用游戏玩法效果规范到目标
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

// 函数，用于根据类默认值和提供的参数创建伤害效果参数
FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor,
	FVector InRadialDamageOrigin, bool bOverrideKnockbackDirection, FVector KnockbackDirectionOverride,
	bool bOverrideDeathImpulse, FVector DeathImpulseDirectionOverride, bool bOverridePitch, float PitchOverride) const
{
	FDamageEffectParams Params;
	// 设置世界上下文对象为能力拥有者的演员
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	// 设置伤害游戏效果类
	Params.DamageGameplayEffectClass = DamageEffectClass;
	// 获取并设置源能力系统组件
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	// 获取并设置目标能力系统组件
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	// 获取并设置基础伤害值
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	// 设置能力等级
	Params.AbilityLevel = GetAbilityLevel();
	// 设置伤害类型
	Params.DamageType = DamageType;
	// 设置减益概率
	Params.DebuffChance = DebuffChance;
	// 设置减益伤害
	Params.DebuffDamage = DebuffDamage;
	// 设置减益持续时间
	Params.DebuffDuration = DebuffDuration;
	// 设置减益频率
	Params.DebuffFrequency = DebuffFrequency;
	// 设置死亡冲力大小
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	// 设置击退力大小
	Params.KnockbackForceMagnitude = KnockbackForceMagnitude;
	// 设置击退概率
	Params.KnockbackChance = KnockbackChance;

	// 如果目标演员有效
	if (IsValid(TargetActor))
	{
		// 计算从能力拥有者到目标的方向
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		// 如果覆盖俯仰角
		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}
		// 获取方向向量
		const FVector ToTarget = Rotation.Vector();
		// 如果不覆盖击退方向
		if (!bOverrideKnockbackDirection)
		{
			Params.KnockbackForce = ToTarget * KnockbackForceMagnitude;
		}
		// 如果不覆盖死亡冲力方向
		if (!bOverrideDeathImpulse)
		{
			Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		}
	}
	
	// 如果覆盖击退方向
	if (bOverrideKnockbackDirection)
	{
		// Normalize the override direction
		KnockbackDirectionOverride.Normalize();
		Params.KnockbackForce = KnockbackDirectionOverride * KnockbackForceMagnitude;
		// 如果覆盖俯仰角
		if (bOverridePitch)
		{
			FRotator KnockbackRotation = KnockbackDirectionOverride.Rotation();
			KnockbackRotation.Pitch = PitchOverride;
			Params.KnockbackForce = KnockbackRotation.Vector() * KnockbackForceMagnitude;
		}
	}

	// 如果覆盖死亡冲力方向
	if (bOverrideDeathImpulse)
	{
		// Normalize the override direction
		DeathImpulseDirectionOverride.Normalize();
		Params.DeathImpulse = DeathImpulseDirectionOverride * DeathImpulseMagnitude;
		// 如果覆盖俯仰角
		if (bOverridePitch)
		{
			FRotator DeathImpulseRotation = DeathImpulseDirectionOverride.Rotation();
			DeathImpulseRotation.Pitch = PitchOverride;
			Params.DeathImpulse = DeathImpulseRotation.Vector() * DeathImpulseMagnitude;
		}
	}
	
	// 如果是范围伤害
	if (bIsRadialDamage)
	{
		Params.bIsRadialDamage = bIsRadialDamage;
		Params.RadialDamageOrigin = InRadialDamageOrigin;
		Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
		Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
	}
	// 返回配置好的伤害效果参数
	return Params;
}


// 函数，用于获取当前等级的伤害值
float UAuraDamageGameplayAbility::GetDamageAtLevel() const
{
	// 获取并返回当前等级的缩放伤害值
	return Damage.GetValueAtLevel(GetAbilityLevel());
}

// 函数，用于从标记的蒙太奇数组中随机选择一个蒙太奇
FTaggedMontage UAuraDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
	// 检查数组是否有元素
	if (TaggedMontages.Num() > 0)
	{
		// 生成一个随机索引
		const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
		// 返回随机选择的蒙太奇
		return TaggedMontages[Selection];
	}

	// 如果数组为空，返回一个默认的FTaggedMontage对象
	return FTaggedMontage();
}

