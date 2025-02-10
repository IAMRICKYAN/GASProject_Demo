// Copyright RickYan


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"

// AAuraEffectActor的构造函数
AAuraEffectActor::AAuraEffectActor()
{
	// 禁用Tick函数，除非显式启用
	PrimaryActorTick.bCanEverTick = false;

	// 创建并设置根组件为USceneComponent
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

// 每帧调用的Tick函数
void AAuraEffectActor::Tick(float DeltaTime)
{
	// 调用基类的Tick函数
	Super::Tick(DeltaTime);

	// 累加运行时间
	RunningTime += DeltaTime;

	// 计算正弦周期
	const float SinePeriod = 2 * PI / SinePeriodConstant;

	// 如果运行时间超过正弦周期，则重置
	if (RunningTime > SinePeriod)
	{
		RunningTime = 0.f;
	}

	// 更新物品的运动
	ItemMovement(DeltaTime);
}

// 物品运动的逻辑
void AAuraEffectActor::ItemMovement(float DeltaTime)
{
	// 如果启用旋转
	if (bRotates)
	{
		// 计算旋转量
		const FRotator DeltaRotation(0.f, DeltaTime * RotationRate, 0.f);
		// 应用旋转
		CalculatedRotation = UKismetMathLibrary::ComposeRotators(CalculatedRotation, DeltaRotation);
	}

	// 如果启用正弦运动
	if (bSinusoidalMovement)
	{
		// 计算正弦值
		const float Sine = SineAmplitude * FMath::Sin(RunningTime * SinePeriodConstant);
		// 更新位置
		CalculatedLocation = InitialLocation + FVector(0.f, 0.f, Sine);
	}
}

// 游戏开始时调用的函数
void AAuraEffectActor::BeginPlay()
{
	// 调用基类的BeginPlay函数
	Super::BeginPlay();

	// 初始化位置和旋转
	InitialLocation = GetActorLocation();
	CalculatedLocation = InitialLocation;
	CalculatedRotation = GetActorRotation();
}

// 开始正弦运动的函数
void AAuraEffectActor::StartSinusoidalMovement()
{
	// 启用正弦运动
	bSinusoidalMovement = true;

	// 初始化位置
	InitialLocation = GetActorLocation();
	CalculatedLocation = InitialLocation;
}

// 开始旋转的函数
void AAuraEffectActor::StartRotation()
{
	// 启用旋转
	bRotates = true;

	// 初始化旋转
	CalculatedRotation = GetActorRotation();
}

// 将游戏效果应用到目标演员的函数
void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// 如果目标演员是敌人且不应用效果到敌人，则直接返回
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;
	
	// 获取目标演员的能力系统组件
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	// 确保游戏效果类有效
	check(GameplayEffectClass);

	// 创建效果上下文句柄
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	// 添加源对象
	EffectContextHandle.AddSourceObject(this);

	// 创建效果规格句柄
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);

	// 应用游戏效果到目标
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	// 检查是否为无限持续时间效果
	const bool bIsInfinite =  EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

	// 如果是无限持续时间效果且移除政策是在结束重叠时移除，则添加到活跃效果句柄映射
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}

	// 如果不是无限持续时间效果，则销毁自身
	if (!bIsInfinite)
	{
		Destroy();
	}
}


// 当Aura效果演员与另一个演员重叠时调用的函数
void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	// 如果目标演员是敌人且不应用效果到敌人，则直接返回
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;
	
	// 根据不同的效果应用政策，决定是否应用即时效果
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	// 根据不同的效果应用政策，决定是否应用持续时间效果
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	// 根据不同的效果应用政策，决定是否应用无限持续时间效果
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

// 当Aura效果演员与另一个演员结束重叠时调用的函数
void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	// 如果目标演员是敌人且不应用效果到敌人，则直接返回
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectsToEnemies) return;
	
	// 根据不同的效果应用政策，决定是否在结束重叠时应用即时效果
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	// 根据不同的效果应用政策，决定是否在结束重叠时应用持续时间效果
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	// 根据不同的效果应用政策，决定是否在结束重叠时应用无限持续时间效果
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	// 根据效果移除政策，决定是否在结束重叠时移除无限持续时间效果
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		// 获取目标演员的能力系统组件
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		// 准备一个数组来存储需要移除的效果句柄
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		// 遍历活跃效果句柄映射，找到属于目标能力系统组件的效果句柄
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				// 移除活跃的游戏效果
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				// 将句柄添加到待移除数组
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		// 从活跃效果句柄映射中移除所有待移除的句柄
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}



