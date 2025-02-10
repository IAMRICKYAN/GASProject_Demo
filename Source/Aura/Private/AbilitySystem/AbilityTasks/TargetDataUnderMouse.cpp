// Copyright RickYan


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"
#include "Aura/Aura.h"

// 静态工厂方法，用于创建UTargetDataUnderMouse实例
UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	// 创建一个新的UTargetDataUnderMouse实例，并将其与指定的游戏能力关联
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	// 返回新创建的实例
	return MyObj;
}

// 重写的Activate方法，用于激活UTargetDataUnderMouse实例
void UTargetDataUnderMouse::Activate()
{
	// 检查拥有该能力的角色是否由本地玩家控制
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		// 如果是本地控制，则发送鼠标光标数据
		SendMouseCursorData();
	}
	else
	{
		// 如果不是本地控制，则获取能力规范句柄和激活预测键
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		
		// 将OnTargetDataReplicatedCallback方法注册为 AbilityTargetDataSetDelegate 的回调
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		
		// 尝试调用复制的目标数据委托，如果设置了的话
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		
		// 如果没有调用委托，则设置等待远程玩家数据
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}


// 发送鼠标光标下的目标数据
void UTargetDataUnderMouse::SendMouseCursorData()
{
	// 创建一个预测窗口，用于确保预测的准确性
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	// 获取当前玩家的控制器
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	// 获取鼠标光标下的命中结果
	PC->GetHitResultUnderCursor(ECC_Target, false, CursorHit);

	// 创建一个目标数据句柄
	FGameplayAbilityTargetDataHandle DataHandle;
	// 创建一个单目标命中数据
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	// 设置命中结果
	Data->HitResult = CursorHit;
	// 将数据添加到数据句柄
	DataHandle.Add(Data);
	
	// 将目标数据发送到服务器，以便复制到其他客户端
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	// 如果应该广播能力任务委托
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// 广播有效的数据
		ValidData.Broadcast(DataHandle);
	}
}

// 当目标数据被复制时调用的回调函数
void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	// 消费客户端复制的目标数据，以避免重复处理
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	// 如果应该广播能力任务委托
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// 广播复制的目标数据
		ValidData.Broadcast(DataHandle);
	}
}

