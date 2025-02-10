// Copyright RickYan


#include "Actor/AuraEnemySpawnVolume.h"

#include "Actor/AuraEnemySpawnPoint.h"
#include "Aura/Aura.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"

// AAuraEnemySpawnVolume的构造函数
AAuraEnemySpawnVolume::AAuraEnemySpawnVolume()
{
	// 禁用Tick函数，因为这个体积（Volume）不需要每帧更新
	PrimaryActorTick.bCanEverTick = false;

	// 创建一个盒子组件，并将其设置为根组件
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	SetRootComponent(Box);

	// 设置盒子组件的碰撞属性
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 仅用于查询，不用于物理模拟
	Box->SetCollisionObjectType(ECC_WorldStatic); // 设置为世界静态对象
	Box->SetCollisionResponseToAllChannels(ECR_Ignore); // 默认忽略所有碰撞通道
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // 仅对Pawn对象响应重叠事件
}

// 加载演员的实现，如果已经达到则销毁自身
void AAuraEnemySpawnVolume::LoadActor_Implementation()
{
	if (bReached)
	{
		Destroy();
	}
}

// 游戏开始时调用的函数
void AAuraEnemySpawnVolume::BeginPlay()
{
	// 调用基类的BeginPlay
	Super::BeginPlay();

	// 绑定盒子组件的开始重叠事件到OnBoxOverlap函数
	Box->OnComponentBeginOverlap.AddDynamic(this, &AAuraEnemySpawnVolume::OnBoxOverlap);
}

// 当盒子组件与其他组件开始重叠时调用的函数
void AAuraEnemySpawnVolume::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 检查其他演员是否实现了UPlayerInterface接口
	if (!OtherActor->Implements<UPlayerInterface>()) return;
	
	// 设置已达到状态为true
	bReached = true;

	// 遍历所有生成点并生成敌人
	for (AAuraEnemySpawnPoint* Point : SpawnPoints)
	{
		if (IsValid(Point))
		{
			Point->SpawnEnemy();
		}
	}

	// 禁用盒子组件的碰撞，以防止再次触发重叠事件
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


