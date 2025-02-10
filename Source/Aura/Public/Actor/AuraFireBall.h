// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "AuraFireBall.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraFireBall : public AAuraProjectile
{
	GENERATED_BODY()
public:
	// BlueprintImplementableEvent允许蓝图实现具体逻辑
	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeline();

	// BlueprintReadOnly属性，只能在蓝图中读取
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> ReturnToActor;

	// BlueprintReadWrite属性，可以在蓝图中读取和写入
	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams ExplosionDamageParams;
	
protected:
	// 重写BeginPlay函数，在游戏开始时调用
	virtual void BeginPlay() override;

	// 重写OnSphereOverlap函数，当球体组件与其他组件重叠时调用
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// 重写OnHit函数，当演员被击中时调用
	virtual void OnHit() override;

};
