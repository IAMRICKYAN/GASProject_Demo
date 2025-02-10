// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class AAuraAIController;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface, public IHighlightInterface
{
	GENERATED_BODY()
public:
	// AAuraEnemy类的构造函数
	AAuraEnemy();

	// 重写PossessedBy函数，当敌人被控制器占据时调用
	virtual void PossessedBy(AController* NewController) override;

	/** Highlight Interface实现 */
	// 实现HighlightActor函数，用于高亮显示敌人
	virtual void HighlightActor_Implementation() override;
	// 实现UnHighlightActor函数，用于取消高亮显示敌人
	virtual void UnHighlightActor_Implementation() override;
	// 实现SetMoveToLocation函数，用于设置敌人移动到的位置
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	/** Highlight Interface实现结束 */

	/** Combat Interface实现 */
	// 实现GetPlayerLevel函数，获取敌人的等级
	virtual int32 GetPlayerLevel_Implementation() override;
	// 实现Die函数，处理敌人死亡逻辑
	virtual void Die(const FVector& DeathImpulse) override;
	// 实现SetCombatTarget函数，设置战斗目标
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	// 实现GetCombatTarget函数，获取当前战斗目标
	virtual AActor* GetCombatTarget_Implementation() const override;
	/** Combat Interface实现结束 */

	// BlueprintReadWrite属性，用于在蓝图中读写战斗目标
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	// BlueprintAssignable属性，用于在蓝图中分配健康值变化的事件
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	// BlueprintAssignable属性，用于在蓝图中分配最大健康值变化的事件
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	// HitReactTagChanged函数，当击中反应标签变化时调用
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	// BlueprintReadOnly属性，表示敌人是否正在执行击中反应
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	// EditAnywhere和BlueprintReadOnly属性，用于在编辑器中设置和从蓝图中读取生命期限
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	// SetLevel函数，用于设置敌人的等级
	void SetLevel(int32 InLevel) { Level = InLevel; }

protected:
	// 重写BeginPlay函数，在游戏开始时调用，用于初始化角色
	virtual void BeginPlay() override;

	// 重写InitAbilityActorInfo函数，初始化能力系统中的角色信息
	virtual void InitAbilityActorInfo() override;

	// 重写InitializeDefaultAttributes函数，初始化角色的默认属性
	virtual void InitializeDefaultAttributes() const override;

	// 重写StunTagChanged函数，当眩晕标签变化时调用，处理角色的眩晕状态
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

	// EditAnywhere和BlueprintReadOnly属性，用于在编辑器中设置和从蓝图中读取角色的等级
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	// VisibleAnywhere和BlueprintReadOnly属性，表示角色的健康条组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	// EditAnywhere属性，用于在编辑器中设置AI行为树
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	// 属性，表示角色的Aura AI控制器
	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

	// BlueprintImplementableEvent函数，在蓝图中实现掉落物品的生成
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLoot();

};
