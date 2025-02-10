// Copyright RickYan


#include "AI/BTService_FindNearestPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(
	UBehaviorTreeComponent& OwnerComp, // 行为树组件的引用
	uint8* NodeMemory,                 // 指向节点内存的指针
	float DeltaSeconds                 // 自上一帧以来的时间差
)
{
	// 调用基类的TickNode函数，执行默认的节点更新逻辑
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 获取拥有此行为树的服务的行为者（通常是AI控制器控制的Pawn）
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	// 确定目标标签，如果拥有者是玩家，则寻找敌人，否则寻找玩家
	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	// 获取所有带有目标标签的演员
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag);

	// 初始化最近距离和最近演员变量
	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;

	// 遍历所有带有目标标签的演员，找到最近的演员
	for (AActor* Actor : ActorsWithTag)
	{
		if (IsValid(Actor) && IsValid(OwningPawn))
		{
			const float Distance = OwningPawn->GetDistanceTo(Actor);
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = Actor;
			}
		}
	}

	// 将找到的最近演员和距离设置到黑板中，以便其他行为树节点可以使用
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);
}
