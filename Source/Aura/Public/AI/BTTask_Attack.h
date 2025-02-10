// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UBTTask_Attack : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

	// 虚函数，用于执行行为树节点任务，返回任务执行结果
	// 重写基类的ExecuteTask函数以自定义任务执行逻辑
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp, // 行为树组件的引用，表示拥有此节点的行为树
		uint8* NodeMemory                  // 指向节点内存的指针，用于存储节点特定的数据
	) override;

};
