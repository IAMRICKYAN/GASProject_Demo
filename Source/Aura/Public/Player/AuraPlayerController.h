// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"


class IHighlightInterface;
class UNiagaraSystem;
class UDamageTextComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;
class AMagicCircle;

// 枚举类，用于表示目标状态，基于uint8类型以节省内存
enum class ETargetingStatus : uint8
{
	TargetingEnemy,     // 正在瞄准敌人
	TargetingNonEnemy,  // 正在瞄准非敌人（例如友军或中立对象）
	NotTargeting        // 当前没有瞄准任何目标
};


/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	/**
 * 构造函数，用于初始化AAuraPlayerController对象。
 */
	AAuraPlayerController();

	/**
	 * 重写PlayerTick函数，用于每帧更新玩家控制器。
	 * @param DeltaTime 自上一帧以来的时间间隔。
	 */
	virtual void PlayerTick(float DeltaTime) override;

	/**
	 * 客户端可靠调用的函数，用于显示伤害数字。
	 * @param DamageAmount 伤害数值。
	 * @param TargetCharacter 受到伤害的角色。
	 * @param bBlockedHit 是否为被阻挡的攻击。
	 * @param bCriticalHit 是否为暴击。
	 */
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

	/**
	 * 蓝图可调用的函数，用于显示魔法圈。
	 * @param DecalMaterial 用于显示魔法圈的贴花材质，默认为nullptr。
	 */
	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	/**
	 * 蓝图可调用的函数，用于隐藏魔法圈。
	 */
	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();


	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext; // 输入映射上下文

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction; // 移动动作

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShiftAction; // Shift键动作

	void ShiftPressed() { bShiftKeyDown = true; }; // Shift键按下
	void ShiftReleased() { bShiftKeyDown = false; }; // Shift键释放
	bool bShiftKeyDown = false; // Shift键按下状态

	void Move(const FInputActionValue& InputActionValue); // 处理移动输入

	void CursorTrace(); // 光标追踪
	TObjectPtr<AActor> LastActor; // 上一个演员
	TObjectPtr<AActor> ThisActor; // 当前演员
	FHitResult CursorHit; // 光标命中结果
	static void HighlightActor(AActor* InActor); // 高亮显示演员
	static void UnHighlightActor(AActor* InActor); // 取消高亮显示演员

	void AbilityInputTagPressed(FGameplayTag InputTag); // 技能输入标签按下
	void AbilityInputTagReleased(FGameplayTag InputTag); // 技能输入标签释放
	void AbilityInputTagHeld(FGameplayTag InputTag); // 技能输入标签保持

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig; // 输入配置

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent; // 技能系统组件

	UAuraAbilitySystemComponent* GetASC(); // 获取技能系统组件

	FVector CachedDestination = FVector::ZeroVector; // 缓存的目的地
	float FollowTime = 0.f; // 跟随时间
	float ShortPressThreshold = 0.5f; // 短按阈值
	bool bAutoRunning = false; // 自动奔跑状态
	ETargetingStatus TargetingStatus = ETargetingStatus::NotTargeting; // 目标状态

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f; // 自动奔跑接受半径

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline; // 样条组件

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem; // 点击粒子系统

	void AutoRun(); // 自动奔跑

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass; // 伤害文本组件类

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass; // 魔法圈类

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle; // 魔法圈

	void UpdateMagicCircleLocation(); // 更新魔法圈位置

};
