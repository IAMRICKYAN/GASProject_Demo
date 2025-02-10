// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UPassiveNiagaraComponent;
class UDebuffNiagaraComponent;
class UNiagaraSystem;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UAnimMontage;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	// AAuraCharacterBase的构造函数
	AAuraCharacterBase();

	// 每帧调用的Tick函数，用于更新逻辑，DeltaTime为上一帧到当前帧的时间间隔
	virtual void Tick(float DeltaTime) override;

	// 获取需要网络复制的属性列表
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	// 处理伤害的函数，返回实际造成的伤害值
	// DamageAmount为伤害量，DamageEvent为伤害事件，EventInstigator为事件发起者，DamageCauser为伤害来源
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// 获取能力系统组件，用于管理角色能力
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// 获取属性集，用于存储和管理角色属性，如生命值、力量等
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }


	/** Combat Interface */
	
	// 获取受击反应动画蒙太奇
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;	
	
	// 角色死亡，传入死亡冲击力
	virtual void Die(const FVector& DeathImpulse) override;
	
	// 获取死亡事件委托
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	
	// 根据动画标签获取战斗插槽位置
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	
	// 判断角色是否已死亡
	virtual bool IsDead_Implementation() const override;
	
	// 获取角色化身（通常是角色的Actor）
	virtual AActor* GetAvatar_Implementation() override;
	
	// 获取攻击动画蒙太奇数组
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	
	// 获取血液效果 Niagara系统
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	
	// 根据动画标签获取标记的动画蒙太奇
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	
	// 获取小兵数量
	virtual int32 GetMinionCount_Implementation() override;
	
	// 增加小兵数量
	virtual void IncremenetMinionCount_Implementation(int32 Amount) override;
	
	// 获取角色类别
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	
	// 获取ASC注册事件委托
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	
	// 获取武器骨骼网格组件
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
	
	// 设置角色是否正在受到电击
	virtual void SetIsBeingShocked_Implementation(bool bInShock) override;
	
	// 判断角色是否正在受到电击
	virtual bool IsBeingShocked_Implementation() const override;
	
	// 获取伤害事件委托
	virtual FOnDamageSignature& GetOnDamageSignature() override;
	
	/** Combat Interface 结束 */
	
	// ASC注册事件委托
	FOnASCRegistered OnAscRegistered;
	
	// 死亡事件委托
	FOnDeathSignature OnDeathDelegate;
	
	// 伤害事件委托
	FOnDamageSignature OnDamageDelegate;
	
	// 多播处理死亡事件，传入死亡冲击力
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);
	
	// 攻击动画蒙太奇数组
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;
	
	// 是否被眩晕，用于网络复制
	UPROPERTY(ReplicatedUsing=OnRep_Stunned, BlueprintReadOnly)
	bool bIsStunned = false;
	
	// 是否被燃烧，用于网络复制
	UPROPERTY(ReplicatedUsing=OnRep_Burned, BlueprintReadOnly)
	bool bIsBurned = false;
	
	// 是否正在受到电击，用于网络复制
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsBeingShocked = false;
	
	// 网络复制回调：角色被眩晕
	UFUNCTION()
	virtual void OnRep_Stunned();
	
	// 网络复制回调：角色被燃烧
	UFUNCTION()
	virtual void OnRep_Burned();
	
	// 设置角色类别
	void SetCharacterClass(ECharacterClass InClass) { CharacterClass = InClass; }

protected:
	// 重写BeginPlay函数，用于在游戏开始时执行初始化逻辑。
	virtual void BeginPlay() override;
	
	// 该属性用于定义角色的武器骨骼网格组件。可以在编辑器中编辑，同时在蓝图中只读，归类在"Combat"类别下。
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	// 该属性用于定义武器尖端插槽的名称。可以在编辑器中编辑，归类在"Combat"类别下。
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;
	
	// 该属性用于定义左手插槽的名称。可以在编辑器中编辑，归类在"Combat"类别下。
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandSocketName;
	
	// 该属性用于定义右手插槽的名称。可以在编辑器中编辑，归类在"Combat"类别下。
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandSocketName;
	
	// 该属性用于定义尾部插槽的名称。可以在编辑器中编辑，归类在"Combat"类别下。
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName TailSocketName;
	
	// 该属性用于标识角色是否死亡。在蓝图中只读。
	UPROPERTY(BlueprintReadOnly)
	bool bDead = false;
	
	// 当角色的眩晕标签发生变化时调用的虚函数。
	// CallbackTag: 触发变化的标签。
	// NewCount: 标签的新计数。
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	// 该属性用于定义角色的基础行走速度。可以在编辑器中编辑，同时在蓝图中只读，归类在"Combat"类别下。
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 600.f;
	
	// 该属性用于存储角色的能力系统组件（Ability System Component）。
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	// 该属性用于存储角色的属性集（Attribute Set）。
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	// 初始化角色能力系统信息的虚函数。
	virtual void InitAbilityActorInfo();
	
	// 该属性用于定义角色的默认主要属性效果类。可以在编辑器中编辑，同时在蓝图中只读，归类在"Attributes"类别下。
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	// 该属性用于定义角色的默认次要属性效果类。可以在编辑器中编辑，同时在蓝图中只读，归类在"Attributes"类别下。
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	
	// 该属性用于定义角色的默认生命属性效果类。可以在编辑器中编辑，同时在蓝图中只读，归类在"Attributes"类别下。
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	
	// 将指定的GameplayEffect应用到自身。
	// GameplayEffectClass: 要应用的GameplayEffect类。
	// Level: 效果等级。
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	
	// 初始化角色默认属性的虚函数。
	virtual void InitializeDefaultAttributes() const;
	
	// 添加角色的初始能力。
	void AddCharacterAbilities();

	/* Dissolve Effects */

	// 触发溶解效果的主逻辑函数。
	void Dissolve();

	// 蓝图可实现的函数，用于启动角色溶解的时间轴效果。
	// DynamicMaterialInstance: 动态材质实例，用于控制溶解效果。
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	// 蓝图可实现的函数，用于启动武器溶解的时间轴效果。
	// DynamicMaterialInstance: 动态材质实例，用于控制武器溶解效果。
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	// 该属性用于定义角色溶解效果的材质实例。可以在编辑器中编辑，同时在蓝图中只读。
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	// 该属性用于定义武器溶解效果的材质实例。可以在编辑器中编辑，同时在蓝图中只读。
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	// 该属性用于定义角色死亡时的血液特效。可以在编辑器中编辑，同时在蓝图中只读，归类在"Combat"类别下。
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UNiagaraSystem* BloodEffect;

	// 该属性用于定义角色死亡时的音效。可以在编辑器中编辑，同时在蓝图中只读，归类在"Combat"类别下。
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	USoundBase* DeathSound;

	/* Minions */

	// 用于记录当前召唤的仆从数量。
	int32 MinionCount = 0;

	// 该属性用于定义角色的职业类型。可以在编辑器中任意编辑，同时在蓝图中只读，归类在"Character Class Defaults"类别下。
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	// 该属性用于定义角色燃烧Debuff效果的Niagara组件。在编辑器中可见，但不可编辑。
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;

	// 该属性用于定义角色眩晕Debuff效果的Niagara组件。在编辑器中可见，但不可编辑。
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent;
	
private:
	// 该属性用于定义角色初始拥有的主动技能列表。可以在编辑器中任意编辑，归类在"Abilities"类别下。
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	// 该属性用于定义角色初始拥有的被动技能列表。可以在编辑器中任意编辑，归类在"Abilities"类别下。
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	// 该属性用于定义角色受到攻击时的反应动画蒙太奇。可以在编辑器中任意编辑，归类在"Combat"类别下。
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	// 该属性用于定义角色光环保护效果的Niagara组件。在编辑器中可见，但不可编辑。
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> HaloOfProtectionNiagaraComponent;

	// 该属性用于定义角色生命吸取效果的Niagara组件。在编辑器中可见，但不可编辑。
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> LifeSiphonNiagaraComponent;

	// 该属性用于定义角色法力吸取效果的Niagara组件。在编辑器中可见，但不可编辑。
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> ManaSiphonNiagaraComponent;

	// 该属性用于定义特效附加的Scene组件。在编辑器中可见，但不可编辑。
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;
};
