// Copyright RickYan


#include "AI/BTTask_Attack.h"

// UBTTask_Attack类中重写的ExecuteTask函数，用于执行攻击任务
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, // 行为树组件的引用，表示拥有此攻击任务的行为树
	uint8* NodeMemory                  // 指向节点内存的指针，用于存储攻击任务特定的数据
)
{
	// 调用基类的ExecuteTask函数，执行默认的任务逻辑
	// 这通常用于保留基类行为，同时允许在派生类中添加或修改特定逻辑
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

