// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

// 枚举类型，用于定义效果应用的政策，可在蓝图中使用
UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap,     // 在重叠时应用效果
	ApplyOnEndOverlap,  // 在结束重叠时应用效果
	DoNotApply          // 不应用效果
};

// 枚举类型，用于定义效果移除的政策，可在蓝图中使用
UENUM(BlueprintType)
enum class EEffectRemovalPolicy
{
	RemoveOnEndOverlap, // 在结束重叠时移除效果
	DoNotRemove         // 不移除效果
};


UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();
	virtual void Tick(float DeltaTime) override;
protected:
    // 重写BeginPlay函数，在游戏开始时进行初始化
    virtual void BeginPlay() override;

    // 可在蓝图中读写的位置向量，用于存储计算后的位置
    UPROPERTY(BlueprintReadWrite)
    FVector CalculatedLocation;

    // 可在蓝图中读写的旋转器，用于存储计算后的旋转
    UPROPERTY(BlueprintReadWrite)
    FRotator CalculatedRotation;

    // 可在任何地方编辑的布尔值，用于控制是否旋转，默认为false
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
    bool bRotates = false;

    // 可在任何地方编辑的浮点值，用于设置旋转速率，默认为45度每秒
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
    float RotationRate = 45.f;

    // 可在任何地方编辑的布尔值，用于控制是否进行正弦运动，默认为false
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
    bool bSinusoidalMovement = false;

    // 可在蓝图中调用的函数，用于开始正弦运动
    UFUNCTION(BlueprintCallable)
    void StartSinusoidalMovement();

    // 可在蓝图中调用的函数，用于开始旋转
    UFUNCTION(BlueprintCallable)
    void StartRotation();
    
    // 可在任何地方编辑的浮点值，用于设置正弦运动的幅度，默认为1
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
    float SineAmplitude = 1.f;

    // 可在任何地方编辑的浮点值，用于设置正弦运动的周期常数，默认为1
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
    float SinePeriodConstant = 1.f; 

    // 可在任何地方编辑的向量，用于存储初始位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
    FVector InitialLocation;

    // 可在蓝图中调用的函数，用于对目标应用游戏玩法效果
    UFUNCTION(BlueprintCallable)
    void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

    // 可在蓝图中调用的函数，用于处理重叠事件
    UFUNCTION(BlueprintCallable)
    void OnOverlap(AActor* TargetActor);

    // 可在蓝图中调用的函数，用于处理结束重叠事件
    UFUNCTION(BlueprintCallable)
    void OnEndOverlap(AActor* TargetActor);

    // 可在任何地方编辑的布尔值，用于控制应用效果后是否销毁，默认为false
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    bool bDestroyOnEffectApplication = false;

    // 可在任何地方编辑的布尔值，用于控制是否对敌人应用效果，默认为false
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    bool bApplyEffectsToEnemies = false;
    
    // 可在任何地方编辑的游戏玩法效果类，用于即时效果
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

    // 可在任何地方编辑的效果应用策略，用于即时效果，默认为不应用
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

    // 可在任何地方编辑的游戏玩法效果类，用于持续时间效果
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

    // 可在任何地方编辑的效果应用策略，用于持续时间效果，默认为不应用
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

    // 可在任何地方编辑的游戏玩法效果类，用于无限持续时间效果
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

    // 可在任何地方编辑的效果应用策略，用于无限持续时间效果，默认为不应用
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

    // 可在任何地方编辑的效果移除策略，用于无限持续时间效果，默认为在结束重叠时移除
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
    EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

    // 活跃效果句柄与能力系统组件的映射，用于跟踪应用的效果
    TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;
	
	// 可在任何地方编辑的浮点值，用于设置角色的等级，默认为1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	float ActorLevel = 1.f;

private:
	// 私有浮点值，用于跟踪运行时间
	float RunningTime = 0.f;
    
	// 私有成员函数，用于处理物品的运动逻辑，DeltaTime为上一帧到当前帧的时间间隔
	void ItemMovement(float DeltaTime);

};