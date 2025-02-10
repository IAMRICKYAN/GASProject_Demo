// Copyright RickYan


#include "Actor/AuraFireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GameplayCueManager.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"

// AAuraFireBall类的成员函数实现

// BeginPlay函数，在游戏开始时调用
void AAuraFireBall::BeginPlay()
{
	// 调用父类的BeginPlay函数
	Super::BeginPlay();
	// 开始执行出射时间线
	StartOutgoingTimeline();
}

// OnSphereOverlap函数，当球体发生重叠时调用
void AAuraFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 如果重叠的actor无效，则直接返回
	if (!IsValidOverlap(OtherActor)) return;

	// 如果当前对象具有网络权限
	if (HasAuthority())
	{
		// 尝试获取目标actor的能力系统组件
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			// 计算死亡冲击力
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			// 设置死亡冲击力
			DamageEffectParams.DeathImpulse = DeathImpulse;
			
			// 设置目标能力系统组件
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			// 应用伤害效果
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
}

// OnHit函数，当火球击中目标时调用
void AAuraFireBall::OnHit()
{
	// 如果拥有者存在
	if (GetOwner())
	{
		// 创建游戏提示参数
		FGameplayCueParameters CueParams;
		// 设置提示位置为当前actor的位置
		CueParams.Location = GetActorLocation();
		// 执行非复制的游戏提示
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FAuraGameplayTags::Get().GameplayCue_FireBlast, CueParams);
	}
	
	// 如果循环声音组件存在
	if (LoopingSoundComponent)
	{
		// 停止声音
		LoopingSoundComponent->Stop();
		// 销毁声音组件
		LoopingSoundComponent->DestroyComponent();
	}
	// 设置击中标志为true
	bHit = true;
}

