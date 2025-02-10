// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UNiagaraSystem;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// AAuraProjectile类的构造函数
	AAuraProjectile();

	// 可在任何地方可见的属性，表示投射物的移动组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	// 可在蓝图读写，并且在生成时暴露的属性，表示伤害效果参数
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

	// 表示寻踪目标的场景组件
	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;

protected:
	// 重写BeginPlay函数，在游戏开始时调用
	virtual void BeginPlay() override;

	// 可在蓝图中调用的函数，表示当投射物击中时调用
	UFUNCTION(BlueprintCallable)
	virtual void OnHit();
	// 重写Destroyed函数，在对象被销毁时调用
	virtual void Destroyed() override;

	// 表示当球体发生重叠时调用的函数
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 可在任何地方可见，但在蓝图中只读的属性，表示球体组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;

	// 函数，用于检查重叠的actor是否有效
	bool IsValidOverlap(AActor* OtherActor);
	// 表示是否已经击中的布尔值
	bool bHit = false;

	// 表示循环声音组件
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;
private:

	// 只在默认情况下可编辑的属性，表示投射物的生命周期
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;

	// 可在任何地方编辑的属性，表示撞击效果
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	// 可在任何地方编辑的属性，表示撞击声音
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	// 可在任何地方编辑的属性，表示循环声音
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;

};
