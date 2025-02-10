// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraBeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	// 函数，用于存储鼠标数据信息，可以在蓝图中调用
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);

	// 函数，用于存储所有者变量，可以在蓝图中调用
	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();

	// 函数，用于追踪第一个目标，可以在蓝图中调用
	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);

	// 函数，用于存储额外的目标，可以在蓝图中调用
	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);

	// 蓝图实现事件，当主要目标死亡时触发
	UFUNCTION(BlueprintImplementableEvent)
	void PrimaryTargetDied(AActor* DeadActor);

	// 蓝图实现事件，当额外目标死亡时触发
	UFUNCTION(BlueprintImplementableEvent)
	void AdditionalTargetDied(AActor* DeadActor);

protected:
	// 属性，鼠标命中位置，可以在蓝图中读写
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	FVector MouseHitLocation;

	// 属性，鼠标命中的演员对象，可以在蓝图中读写
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<AActor> MouseHitActor;

	// 属性，所有者玩家控制器，可以在蓝图中读写
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;

	// 属性，所有者角色，可以在蓝图中读写
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<ACharacter> OwnerCharacter;

	// 属性，最大电击目标数量，只能在默认值编辑器中编辑，归类在"Beam"类别下
	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	int32 MaxNumShockTargets = 5;

};
