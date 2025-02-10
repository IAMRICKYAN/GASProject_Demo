// Copyright RickYan


#include "Checkpoint/MapEntrance.h"

#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

// AMapEntrance的构造函数，用于初始化AMapEntrance对象
// 参数ObjectInitializer用于初始化对象
AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) // 调用基类的构造函数
{
	Sphere->SetupAttachment(MoveToComponent); // 将Sphere组件附加到MoveToComponent组件上
}

// 高亮显示Actor的实现函数
void AMapEntrance::HighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(true); // 设置CheckpointMesh组件以自定义深度渲染，通常用于高亮显示
}

// 加载Actor的实现函数
void AMapEntrance::LoadActor_Implementation()
{
	// 当加载地图入口时，不执行任何操作
}

// 球体重叠事件的处理函数
void AMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 检查重叠的Actor是否实现了UPlayerInterface接口
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true; // 设置标志，表示玩家已到达入口

		// 尝试获取当前游戏模式，并转换为AAuraGameModeBase类型
		if (AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			// 保存世界状态，参数为当前世界和目标地图的资产名称
			AuraGM->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
		}
		
		// 调用玩家接口的SaveProgress函数，保存玩家进度，参数为目的地玩家起始位置的标签
		IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);

		// 通过软对象指针打开目标地图级别
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
}

