// Copyright RickYan


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// AAuraProjectile类的构造函数
AAuraProjectile::AAuraProjectile()
{
	// 设置该actor不进行tick更新
	PrimaryActorTick.bCanEverTick = false;
	// 设置该actor可以进行网络复制
	bReplicates = true;

	// 创建默认的球体组件，用于碰撞检测
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	// 设置球体组件为根组件
	SetRootComponent(Sphere);
	// 设置球体组件的碰撞对象类型为投射物
	Sphere->SetCollisionObjectType(ECC_Projectile);
	// 设置球体组件的碰撞启用为仅查询
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// 设置球体组件对所有通道的碰撞响应为忽略
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	// 设置球体组件对世界动态、世界静态和角色通道的碰撞响应为重叠
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// 创建默认的投射物移动组件
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	// 设置投射物移动组件的初始速度和最大速度
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	// 设置投射物移动组件的重力缩放为0，即不受重力影响
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

// BeginPlay函数，在游戏开始时调用
void AAuraProjectile::BeginPlay()
{
	// 调用父类的BeginPlay函数
	Super::BeginPlay();
	// 设置actor的生命周期
	SetLifeSpan(LifeSpan);
	// 设置移动组件的复制状态
	SetReplicateMovement(true);
	// 绑定球体组件的开始重叠事件到OnSphereOverlap函数
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);

	// 在根组件上生成并播放循环声音
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

// OnHit函数，当投射物击中时调用
void AAuraProjectile::OnHit()
{
	// 在击中位置播放撞击声音
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	// 在击中位置生成撞击效果粒子系统
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	// 如果循环声音组件存在，停止并销毁它
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	// 设置击中标志为true
	bHit = true;
}


// AAuraProjectile类的成员函数实现

// Destroyed函数，在对象被销毁时调用
void AAuraProjectile::Destroyed()
{
	// 如果循环声音组件存在
	if (LoopingSoundComponent)
	{
		// 停止声音
		LoopingSoundComponent->Stop();
		// 销毁声音组件
		LoopingSoundComponent->DestroyComponent();
	}
	// 如果没有击中并且没有权限，调用OnHit函数
	if (!bHit && !HasAuthority()) OnHit();
	// 调用父类的Destroyed函数
	Super::Destroyed();
}

// OnSphereOverlap函数，当球体发生重叠时调用
void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 如果重叠的actor无效，直接返回
	if (!IsValidOverlap(OtherActor)) return;
	// 如果没有击中，调用OnHit函数
	if (!bHit) OnHit();
	
	// 如果有权限（通常是服务器）
	if (HasAuthority())
	{
		// 尝试获取目标的能力系统组件
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			// 计算死亡冲量
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			// 计算是否应该有击退效果
			const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
			if (bKnockback)
			{
				// 设置击退方向的旋转
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;
				
				// 计算击退方向和力量
				const FVector KnockbackDirection = Rotation.Vector();
				const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
				DamageEffectParams.KnockbackForce = KnockbackForce;
			}
			
			// 设置目标能力系统组件并应用伤害效果
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		
		// 销毁投射物
		Destroy();
	}
	else
	{
		// 如果没有权限，设置击中标志为true
		bHit = true;
	}
}

// IsValidOverlap函数，用于检查重叠的actor是否有效
bool AAuraProjectile::IsValidOverlap(AActor* OtherActor)
{
	// 如果源能力系统组件为空，返回false
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr) return false;
	// 获取源化身actor
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	// 如果源化身actor与重叠的actor相同，返回false
	if (SourceAvatarActor == OtherActor) return false;
	// 如果源化身actor和重叠的actor是朋友，返回false
	if (!UAuraAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor)) return false;

	// 所有检查都通过，返回true
	return true;
}

