// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "CombatInterface.generated.h"

class UAbilitySystemComponent;
class UNiagaraSystem;
class UAnimMontage;

// 声明一个多播委托，该委托携带一个参数，用于通知能力系统组件已注册
DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*);

// 声明一个动态多播委托，该委托携带一个参数，用于通知某个角色已死亡
// 参数 DeadActor：指向已死亡角色的指针
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor);

// 声明一个多播委托，该委托携带一个参数，用于通知受到伤害的事件
// 参数 DamageAmount：伤害量（注释中用/*DamageAmount*/表示该参数的说明）
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamageSignature, float /*DamageAmount*/);


// 声明一个结构体，用于存储与动画蒙太奇相关的标签和声音信息，可在蓝图中使用
USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	// 动画蒙太奇资产，默认为空，可在编辑器中设置
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	// 与动画蒙太奇关联的 Gameplay 标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;

	// 与动画蒙太奇关联的插槽标签，用于确定动画的播放位置
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	// 碰撞时播放的声音资产，默认为空，可在编辑器中设置
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};


/**
 * 
 */
// 定义一个战斗接口类，用于实现战斗相关的功能，标记为AURA_API以允许在模块间共享
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// 在此类中添加接口函数。这个类将被继承以实现该接口。
public:
	// 蓝图原生事件，用于获取玩家等级
	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerLevel();

	// 蓝图原生事件和可调用，用于根据动画标签获取战斗插槽位置
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);

	// 蓝图可实现事件和可调用，用于更新面向目标的位置
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);

	// 蓝图原生事件和可调用，用于获取受击反应动画蒙太奇
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	// 纯虚函数，用于实现死亡逻辑，必须被继承类实现
	virtual void Die(const FVector& DeathImpulse) = 0;
	// 纯虚函数，用于获取死亡委托，必须被继承类实现
	virtual FOnDeathSignature& GetOnDeathDelegate() = 0;
	// 纯虚函数，用于获取伤害委托，必须被继承类实现
	virtual FOnDamageSignature& GetOnDamageSignature() = 0; 

	// 蓝图原生事件和可调用，用于检查是否已死亡
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	// 蓝图原生事件和可调用，用于获取角色化身
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	// 蓝图原生事件和可调用，用于获取攻击动画蒙太奇数组
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();

	// 蓝图原生事件和可调用，用于获取血液效果Niagara系统
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetBloodEffect();

	// 蓝图原生事件和可调用，用于根据动画标签获取标记的动画蒙太奇
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);

	// 蓝图原生事件和可调用，用于获取小兵数量
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMinionCount();

	// 蓝图原生事件和可调用，用于增加小兵数量
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IncremenetMinionCount(int32 Amount);

	// 蓝图原生事件和可调用，用于获取角色类别
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterClass GetCharacterClass();

	// 纯虚函数，用于获取能力系统组件注册委托，必须被继承类实现
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() = 0;

	// 蓝图可实现事件和可调用，用于设置是否处于电击循环中
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetInShockLoop(bool bInLoop);

	// 蓝图原生事件和可调用，用于获取武器骨骼网格组件
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetWeapon();

	// 蓝图原生事件和可调用，用于检查是否正在被电击
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsBeingShocked() const;

	// 蓝图原生事件和可调用，用于设置是否正在被电击
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsBeingShocked(bool bInShock);
};

