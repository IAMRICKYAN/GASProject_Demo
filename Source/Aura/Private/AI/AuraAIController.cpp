// Copyright RickYan


#include "AI/AuraAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AAuraAIController::AAuraAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent"); // 创建并初始化黑板组件，用于存储AI决策数据
	check(Blackboard); // 确保黑板组件已成功创建

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent"); // 创建并初始化行为树组件，用于控制AI行为
	check(BehaviorTreeComponent); // 确保行为树组件已成功创建
}
