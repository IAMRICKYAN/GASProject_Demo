// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class ULoadScreenSaveGame;
// 声明一个多播委托，带有一个参数，用于通知效果资产标签的变化
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);

// 声明一个多播委托，无参数，用于通知能力给予的事件
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);

// 声明一个单播委托，带有一个参数，用于遍历每个游戏能力
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);

// 声明一个多播委托，带有三个参数，用于通知能力状态的变化
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, int32 /*AbilityLevel*/);

// 声明一个多播委托，带有四个参数，用于通知能力装备的事件
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*Status*/, const FGameplayTag& /*Slot*/, const FGameplayTag& /*PrevSlot*/);

// 声明一个多播委托，带有一个参数，用于通知被动能力去激活的事件
DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbility, const FGameplayTag& /*AbilityTag*/);

// 声明一个多播委托，带有两个参数，用于通知被动效果激活或去激活的事件
DECLARE_MULTICAST_DELEGATE_TwoParams(FActivatePassiveEffect, const FGameplayTag& /*AbilityTag*/, bool /*bActivate*/);


/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	// 设置能力角色信息
	void AbilityActorInfoSet();
	
	// 效果资产标签的多播委托
	FEffectAssetTags EffectAssetTags;
	
	// 能力给予的多播委托
	FAbilitiesGiven AbilitiesGivenDelegate;
	
	// 能力状态变化的多播委托
	FAbilityStatusChanged AbilityStatusChanged;
	
	// 能力装备的多播委托
	FAbilityEquipped AbilityEquipped;
	
	// 被动能力去激活的多播委托
	FDeactivatePassiveAbility DeactivatePassiveAbility;
	
	// 被动效果激活或去激活的多播委托
	FActivatePassiveEffect ActivatePassiveEffect;
	
	// 从保存数据中添加角色能力
	void AddCharacterAbilitiesFromSaveData(ULoadScreenSaveGame* SaveData);
	
	// 添加角色启动能力
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	
	// 添加角色启动被动能力
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	
	// 标记启动能力是否已给予
	bool bStartupAbilitiesGiven = false;
	
	// 能力输入标签按下时的处理函数
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	
	// 能力输入标签持续按住时的处理函数
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	
	// 能力输入标签释放时的处理函数
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
	// 遍历每个能力的委托调用函数
	void ForEachAbility(const FForEachAbility& Delegate);
	
	// 从能力规格中获取能力标签的静态函数
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	// 从能力规格中获取输入标签的静态函数
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	// 从能力规格中获取状态的静态函数
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	// 根据能力标签获取状态的函数
	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);
	
	// 根据能力标签获取槽位的函数
	FGameplayTag GetSlotFromAbilityTag(const FGameplayTag& AbilityTag);
	
	// 检查槽位是否为空的函数
	bool SlotIsEmpty(const FGameplayTag& Slot);
	
	// 检查能力是否拥有特定槽位的静态函数
	static bool AbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);
	
	// 检查能力是否拥有任何槽位的静态函数
	static bool AbilityHasAnySlot(const FGameplayAbilitySpec& Spec);
	
	// 根据槽位获取能力规格的函数
	FGameplayAbilitySpec* GetSpecWithSlot(const FGameplayTag& Slot);
	
	// 检查是否为被动能力的函数
	bool IsPassiveAbility(const FGameplayAbilitySpec& Spec) const;
	
	// 为能力分配槽位的静态函数
	static void AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);
	
	// 多播激活被动效果的网络函数
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastActivatePassiveEffect(const FGameplayTag& AbilityTag, bool bActivate);
	
	// 根据能力标签获取能力规格的函数
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	
	// 升级属性的函数
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	
	// 升级属性的服务器端函数
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);
	
	// 更新能力状态的函数
	void UpdateAbilityStatuses(int32 Level);
	
	// 在服务器上花费技能点的函数
	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);
	
	// 在服务器上装备能力的函数
	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Slot);
	
	// 在客户端装备能力的函数
	UFUNCTION(Client, Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);
	
	// 根据能力标签获取描述和下一级描述的函数
	bool GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);
	
	// 清除槽位的静态函数
	static void ClearSlot(FGameplayAbilitySpec* Spec);
	
	// 清除特定槽位的能力的函数
	void ClearAbilitiesOfSlot(const FGameplayTag& Slot);
	// 检查指定的能力规格是否拥有特定槽位的静态函数
	static bool AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot);

protected:

	// 虚函数，用于在能力激活时进行复制通知的覆盖实现
	virtual void OnRep_ActivateAbilities() override;

	// 客户端可靠函数，用于通知客户端应用了游戏效果
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

	// 客户端可靠函数，用于更新能力的状态
	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);

};
