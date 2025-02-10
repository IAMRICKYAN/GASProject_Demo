// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_FindNearestPlayer.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UBTService_FindNearestPlayer : public UBTService_BlueprintBase
{
	GENERATED_BODY()
protected:

	// 虚函数，用于在每个游戏帧更新行为树节点，重写基类的TickNode函数以自定义节点更新逻辑
	virtual void TickNode(
		UBehaviorTreeComponent& OwnerComp, // 行为树组件的引用
		uint8* NodeMemory,                 // 指向节点内存的指针
		float DeltaSeconds                 // 自上一帧以来的时间差，用于计算帧间隔
	) override;

	// 黑板键选择器，用于在蓝图中读取和编辑，指定要跟随的目标
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector TargetToFollowSelector;

	// 黑板键选择器，用于在蓝图中读取和编辑，指定与目标的距离
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector DistanceToTargetSelector;

};
