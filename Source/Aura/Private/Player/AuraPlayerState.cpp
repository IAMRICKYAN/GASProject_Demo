// Copyright RickYan


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

// AAuraPlayerState的构造函数，用于初始化AAuraPlayerState对象
AAuraPlayerState::AAuraPlayerState()
{
	// 创建并设置能力系统组件
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true); // 设置组件为可复制，以便在多玩家游戏中同步
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); // 设置复制模式为混合模式，允许效果和标签的复制

	// 创建并设置属性集组件
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	// 设置网络更新频率
	NetUpdateFrequency = 100.f; // 控制网络同步的频率，单位为Hz
}


void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, XP);
	DOREPLIFETIME(AAuraPlayerState, AttributePoints);
	DOREPLIFETIME(AAuraPlayerState, SpellPoints);
}

// 获取能力系统组件的指针
UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// 增加经验值
void AAuraPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

// 增加等级
void AAuraPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level, true);
}

// 设置经验值
void AAuraPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

// 设置等级
void AAuraPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level, false);
}

// 设置属性点
void AAuraPlayerState::SetAttributePoints(int32 InPoints)
{
	AttributePoints = InPoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

// 设置法术点
void AAuraPlayerState::SetSpellPoints(int32 InPoints)
{
	SpellPoints = InPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

// 当等级被复制时调用的函数
void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level, true);
}

// 当经验值被复制时调用的函数
void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

// 当属性点被复制时调用的函数
void AAuraPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

// 当法术点被复制时调用的函数
void AAuraPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

// 增加属性点
void AAuraPlayerState::AddToAttributePoints(int32 InPoints)
{
	AttributePoints += InPoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

// 增加法术点
void AAuraPlayerState::AddToSpellPoints(int32 InPoints)
{
	SpellPoints += InPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}
