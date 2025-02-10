// Copyright RickYan


#include "AbilitySystem/Abilities/AuraBeamSpell.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

// 函数，用于存储鼠标数据信息
void UAuraBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	// 如果发生阻塞命中
	if (HitResult.bBlockingHit)
	{
		// 存储命中位置和命中演员
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		// 如果没有发生阻塞命中，则取消当前技能
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

// 函数，用于存储拥有者变量
void UAuraBeamSpell::StoreOwnerVariables()
{
	// 如果当前演员信息有效
	if (CurrentActorInfo)
	{
		// 获取并存储拥有者的玩家控制器
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		// 获取并存储拥有者的角色
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}


// 函数，用于追踪光束的第一个目标
void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	// 确保OwnerCharacter不为空
	check(OwnerCharacter);
	// 检查OwnerCharacter是否实现了UCombatInterface接口
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		// 获取武器骨骼网格组件
		if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
		{
			// 定义要忽略的演员数组，包括OwnerCharacter本身
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(OwnerCharacter);
			// 存储命中结果
			FHitResult HitResult;
			// 获取武器TipSocket的位置
			const FVector SocketLocation = Weapon->GetSocketLocation(FName("TipSocket"));
			// 执行球形单次追踪
			UKismetSystemLibrary::SphereTraceSingle(
				OwnerCharacter,
				SocketLocation,
				BeamTargetLocation,
				10.f, // 追踪半径
				TraceTypeQuery1, // 追踪类型
				false, // 是否追踪复杂碰撞
				ActorsToIgnore,
				EDrawDebugTrace::None, // 不绘制调试追踪
				HitResult,
				true); // 是否返回物理材质

			// 如果发生阻塞命中
			if (HitResult.bBlockingHit)
			{
				// 存储命中位置和命中演员
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
			}
		}
	}
	// 如果MouseHitActor实现了UCombatInterface接口
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor))
	{
		// 如果PrimaryTargetDied函数尚未绑定到死亡委托
		if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAuraBeamSpell::PrimaryTargetDied))
		{
			// 绑定PrimaryTargetDied函数到死亡委托
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAuraBeamSpell::PrimaryTargetDied);
		}
	}
}

// 函数，用于存储额外的目标
void UAuraBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	// 定义要忽略的演员数组，包括技能拥有者和主要命中演员
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(MouseHitActor);
	
	// 获取指定半径内的所有存活玩家
	TArray<AActor*> OverlappingActors;
	UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		ActorsToIgnore,
		850.f, // 搜索半径
		MouseHitActor->GetActorLocation());
	
	// 计算额外目标的最大数量
	int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);
	
	// 获取最接近的额外目标
	UAuraAbilitySystemLibrary::GetClosestTargets(
		NumAdditionalTargets,
		OverlappingActors,
		OutAdditionalTargets,
		MouseHitActor->GetActorLocation());

	// 遍历所有额外目标
	for (AActor* Target : OutAdditionalTargets)
	{
		// 如果目标实现了UCombatInterface接口
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
		{
			// 如果AdditionalTargetDied函数尚未绑定到死亡委托
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAuraBeamSpell::AdditionalTargetDied))
			{
				// 绑定AdditionalTargetDied函数到死亡委托
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAuraBeamSpell::AdditionalTargetDied);
			}
		}
	}
}

