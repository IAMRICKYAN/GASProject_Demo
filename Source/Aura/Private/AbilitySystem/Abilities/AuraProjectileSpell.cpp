// Copyright RickYan


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
										   const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
										   const FGameplayEventData* TriggerEventData)
{
	// 调用基类的ActivateAbility函数，执行通用的激活逻辑。
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// TODO: 在这里添加UAuraProjectileSpell特有的激活逻辑，例如初始化参数、设置状态等。
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	// 检查当前是否为服务器，因为生成弹幕通常只在服务器上执行。
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	// 获取发射socket的位置。
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);

	// 计算从socket位置到目标位置的旋转。
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	// 如果需要覆盖俯仰角，则设置新的俯仰角。
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}
    
	// 创建弹幕的生成变换，包括位置和旋转。
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
        
	// 延迟生成弹幕，以便在生成前可以设置额外的参数。
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    
	// 设置弹幕的损伤效果参数，从类默认值中创建。
	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
        
	// 完成弹幕的生成。
	Projectile->FinishSpawning(SpawnTransform);
}
