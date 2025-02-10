// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;

// 声明一个多播委托，该委托有一个int32类型的参数，用于通知玩家状态值发生变化
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*StatValue*/);

// 声明一个多播委托，该委托有两个参数，第一个是int32类型的状态值，第二个是bool类型表示是否升级，用于通知玩家等级发生变化
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32 /*StatValue*/, bool /*bLevelUp*/);


/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// 虚函数，用于获取能力系统组件，重写基类的方法
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// 获取属性集组件
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// 编辑器中可编辑的默认属性，指向等级提升信息的对象
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	// 玩家经验值变化的多播委托
	FOnPlayerStatChanged OnXPChangedDelegate;

	// 玩家等级变化的多播委托
	FOnLevelChanged OnLevelChangedDelegate;

	// 玩家属性点变化的多播委托
	FOnPlayerStatChanged OnAttributePointsChangedDelegate;

	// 玩家法术点变化的多播委托
	FOnPlayerStatChanged OnSpellPointsChangedDelegate;

	// 内联函数，获取玩家等级
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }

	// 内联函数，获取玩家经验值
	FORCEINLINE int32 GetXP() const { return XP; }

	// 内联函数，获取玩家属性点
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }

	// 内联函数，获取玩家法术点
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }

	// 增加玩家经验值
	void AddToXP(int32 InXP);

	// 增加玩家等级
	void AddToLevel(int32 InLevel);

	// 增加玩家属性点
	void AddToAttributePoints(int32 InPoints);

	// 增加玩家法术点
	void AddToSpellPoints(int32 InPoints);

	// 设置玩家经验值
	void SetXP(int32 InXP);

	// 设置玩家等级
	void SetLevel(int32 InLevel);

	// 设置玩家属性点
	void SetAttributePoints(int32 InPoints);

	// 设置玩家法术点
	void SetSpellPoints(int32 InPoints);

	
protected:
	
	// 可在任何地方可见的属性，指向能力系统组件的指针
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// 私有属性，指向属性集组件的指针
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;


private:

	// 可在任何地方可见的属性，表示角色的等级，默认值为1，并且会在多玩家游戏中复制，使用OnRep_Level函数来响应复制
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	// 可在任何地方可见的属性，表示角色的经验值，默认值为0，并且会在多玩家游戏中复制，使用OnRep_XP函数来响应复制
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 0;

	// 可在任何地方可见的属性，表示角色的属性点，默认值为0，并且会在多玩家游戏中复制，使用OnRep_AttributePoints函数来响应复制
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;

	// 可在任何地方可见的属性，表示角色的法术点，默认值为0，并且会在多玩家游戏中复制，使用OnRep_SpellPoints函数来响应复制
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SpellPoints)
	int32 SpellPoints = 0;

	// 当角色的等级在多玩家游戏中被复制时调用的函数，参数为旧的等级值
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	// 当角色的经验值在多玩家游戏中被复制时调用的函数，参数为旧的经验值
	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	// 当角色的属性点在多玩家游戏中被复制时调用的函数，参数为旧的属性点值
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);

	// 当角色的法术点在多玩家游戏中被复制时调用的函数，参数为旧的法术点值
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);

};
