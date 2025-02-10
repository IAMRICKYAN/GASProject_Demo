#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	// 默认构造函数
	FDamageEffectParams(){}

	// 世界上下文对象，通常用于获取世界信息和日志
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	// 伤害游戏效果类，用于实例化伤害效果
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	// 源能力系统组件，通常是发出伤害的实体
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	// 目标能力系统组件，通常是接收伤害的实体
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	// 基础伤害值
	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	// 能力等级，可能用于计算伤害加成
	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	// 伤害类型标签
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	// 减益效果触发的概率
	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;

	// 减益效果的额外伤害
	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;

	// 减益效果的持续时间
	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;

	// 减益效果的触发频率
	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;

	// 死亡时冲击力的大小
	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;

	// 死亡时冲击力的方向
	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;

	// 击退力的大小
	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f;

	// 击退效果触发的概率
	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;

	// 击退力的方向
	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;

	// 是否为范围伤害
	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage = false;

	// 范围伤害的内半径
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f;

	// 范围伤害的外半径
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f;

	// 范围伤害的原点
	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;
	
};


USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	// 检查是否为暴击
	bool IsCriticalHit() const { return bIsCriticalHit; }
	// 检查是否为格挡
	bool IsBlockedHit() const { return bIsBlockedHit; }
	// 检查减益效果是否成功
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	// 获取减益伤害值
	float GetDebuffDamage() const { return DebuffDamage; }
	// 获取减益持续时间
	float GetDebuffDuration() const { return DebuffDuration; }
	// 获取减益触发频率
	float GetDebuffFrequency() const { return DebuffFrequency; }
	// 获取伤害类型标签
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	// 获取死亡冲击力
	FVector GetDeathImpulse() const { return DeathImpulse; }
	// 获取击退力
	FVector GetKnockbackForce() const { return KnockbackForce; }
	// 检查是否为范围伤害
	bool IsRadialDamage() const { return bIsRadialDamage; }
	// 获取范围伤害内半径
	float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	// 获取范围伤害外半径
	float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	// 获取范围伤害原点
	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }

	// 设置是否为暴击
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	// 设置是否为格挡
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	// 设置减益效果是否成功
	void SetIsSuccessfulDebuff(bool bInIsDebuff) { bIsSuccessfulDebuff = bInIsDebuff; }
	// 设置减益伤害值
	void SetDebuffDamage(float InDamage) { DebuffDamage = InDamage; }
	// 设置减益持续时间
	void SetDebuffDuration(float InDuration) { DebuffDuration = InDuration; }
	// 设置减益触发频率
	void SetDebuffFrequency(float InFrequency) { DebuffFrequency = InFrequency; }
	// 设置伤害类型标签
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	// 设置死亡冲击力
	void SetDeathImpulse(const FVector& InImpulse) { DeathImpulse = InImpulse; }
	// 设置击退力
	void SetKnockbackForce(const FVector& InForce) { KnockbackForce = InForce; }
	// 设置是否为范围伤害
	void SetIsRadialDamage(bool bInIsRadialDamage) { bIsRadialDamage = bInIsRadialDamage; }
	// 设置范围伤害内半径
	void SetRadialDamageInnerRadius(float InRadialDamageInnerRadius) { RadialDamageInnerRadius = InRadialDamageInnerRadius; }
	// 设置范围伤害外半径
	void SetRadialDamageOuterRadius(float InRadialDamageOuterRadius) { RadialDamageOuterRadius = InRadialDamageOuterRadius; }
	// 设置范围伤害原点
	void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin) { RadialDamageOrigin = InRadialDamageOrigin; }
	
	// 返回用于序列化的实际结构体，子类必须重写此方法！
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FGameplayEffectContext::StaticStruct();
	}

	// 创建此上下文的副本，用于后续修改
	virtual FGameplayEffectContext* Duplicate() const
	{
		FGameplayEffectContext* NewContext = new FGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// 对命中结果进行深拷贝
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	// 自定义序列化，子类必须重写此方法
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
	
protected:

	UPROPERTY()
	bool bIsBlockedHit = false; // 是否格挡了此次攻击

	UPROPERTY()
	bool bIsCriticalHit = false; // 是否为暴击

	UPROPERTY()
	bool bIsSuccessfulDebuff = false; // 是否成功施加了减益效果

	UPROPERTY()
	float DebuffDamage = 0.f; // 减益效果造成的伤害值

	UPROPERTY()
	float DebuffDuration = 0.f; // 减益效果的持续时间

	UPROPERTY()
	float DebuffFrequency = 0.f; // 减益效果的触发频率

	TSharedPtr<FGameplayTag> DamageType; // 伤害类型的游戏玩法标签

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector; // 死亡时产生的冲击力

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector; // 击退效果产生的力

	UPROPERTY()
	bool bIsRadialDamage = false; // 是否为范围伤害

	UPROPERTY()
	float RadialDamageInnerRadius = 0.f; // 范围伤害的内半径

	UPROPERTY()
	float RadialDamageOuterRadius = 0.f; // 范围伤害的外半径

	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector; // 范围伤害的原点

};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true, // 支持网络序列化
		WithCopy = true // 支持复制
	};
};