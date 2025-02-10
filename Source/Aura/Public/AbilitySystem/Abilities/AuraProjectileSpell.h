// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
class UGameplayEffect;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()


protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// ActivateAbility函数是Gameplay Ability System中用于激活能力的虚函数。
	// 它被重写以提供自定义的激活逻辑。
	// 参数说明：
	// - Handle：能力的规格句柄，用于唯一标识这个能力实例。
	// - ActorInfo：包含激活能力的演员信息的指针。
	// - ActivationInfo：包含能力激活信息的结构体。
	// - TriggerEventData：触发能力的事件数据指针，可能为空。

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);

	// SpawnProjectile函数是一个可在蓝图中调用的函数，用于生成弹幕。
	// 参数说明：
	// - ProjectileTargetLocation：弹幕的目标位置。
	// - SocketTag：用于标识发射弹幕的socket的Gameplay Tag。
	// - bOverridePitch：布尔值，表示是否覆盖默认的俯仰角。
	// - PitchOverride：如果bOverridePitch为true，则这个值用于覆盖默认的俯仰角。

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;

	// ProjectileClass属性是一个 UClass 属性，用于指定要生成的弹幕类。
	// 它可以在编辑器中任意修改，并且可以在蓝图中访问。
	// AAuraProjectile是弹幕的基类，具体的弹幕类型应该是这个类的子类。

	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 5;

	// NumProjectiles属性用于指定生成弹幕的数量。
	// 它只能在编辑器中默认值处修改，不能在实例中修改。
	// 默认值为5，表示如果没有特别指定，将生成5个弹幕。

};
