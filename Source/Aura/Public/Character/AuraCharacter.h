// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UNiagaraComponent;
class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
public:
	// 构造函数，用于初始化AAuraCharacter对象
	AAuraCharacter();
	
	// 覆写虚函数，当角色被控制器（NewController）占据时调用
	virtual void PossessedBy(AController* NewController) override;
	
	// 覆写虚函数，当玩家的状态（PlayerState）在多玩家环境中被复制时调用
	virtual void OnRep_PlayerState() override;
	
	/** Player Interface实现部分 */
	// 实现接口函数，用于增加经验值（InXP）
	virtual void AddToXP_Implementation(int32 InXP) override;
	
	// 实现接口函数，用于处理角色升级
	virtual void LevelUp_Implementation() override;
	
	// 实现接口函数，用于获取当前经验值
	virtual int32 GetXP_Implementation() const override;
	
	// 实现接口函数，根据输入的经验值（InXP）查找对应的等级
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	
	// 实现接口函数，根据等级（Level）获取奖励的属性点数
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	
	// 实现接口函数，根据等级（Level）获取奖励的技能点数
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	
	// 实现接口函数，用于增加玩家等级（InPlayerLevel）
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	
	// 实现接口函数，用于增加属性点数（InAttributePoints）
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	
	// 实现接口函数，用于增加技能点数（InSpellPoints）
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	
	// 实现接口函数，用于获取当前属性点数
	virtual int32 GetAttributePoints_Implementation() const override;
	
	// 实现接口函数，用于获取当前技能点数
	virtual int32 GetSpellPoints_Implementation() const override;
	
	// 实现接口函数，用于显示魔法圈（DecalMaterial为材质接口指针）
	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial) override;
	
	// 实现接口函数，用于隐藏魔法圈
	virtual void HideMagicCircle_Implementation() override;
	
	// 实现接口函数，用于保存游戏进度，传入一个检查点标签（CheckpointTag）
	virtual void SaveProgress_Implementation(const FName& CheckpointTag) override;
	/** Player Interface实现部分结束 */
	
	/** Combat Interface实现部分 */
	// 实现接口函数，用于获取玩家等级
	virtual int32 GetPlayerLevel_Implementation() override;
	
	// 实现接口函数，用于处理角色死亡，DeathImpulse为死亡时的冲击力向量
	virtual void Die(const FVector& DeathImpulse) override;
	/** Combat Interface实现部分结束 */
	
	// 编辑默认值属性，用于设置死亡后的等待时间，默认为5秒
	UPROPERTY(EditDefaultsOnly)
	float DeathTime = 5.f;
	
	// 用于处理死亡计时器的定时器句柄
	FTimerHandle DeathTimer;
	
	// 可在任何地方可见且蓝图可读的属性，用于存储升级特效的Niagara组件指针
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
	
	// 覆写虚函数，当角色的眩晕状态在多玩家环境中被复制时调用
	virtual void OnRep_Stunned() override;
	
	// 覆写虚函数，当角色的燃烧状态在多玩家环境中被复制时调用
	virtual void OnRep_Burned() override;
	
	// 函数，用于加载游戏进度
	void LoadProgress();

	
private:
	// 可在任何地方可见的属性，用于存储顶视摄像头组件指针
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;
	
	// 可在任何地方可见的属性，用于存储摄像头吊臂组件指针
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	// 覆写虚函数，用于初始化能力系统中的角色信息
	virtual void InitAbilityActorInfo() override;
	
	// 多玩家网络可靠广播函数，用于在所有客户端上播放升级粒子效果
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;

};
