// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "Checkpoint/Checkpoint.h"
#include "MapEntrance.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AMapEntrance : public ACheckpoint
{
	GENERATED_BODY()
public:
	// 构造函数，用于初始化AMapEntrance对象
	AMapEntrance(const FObjectInitializer& ObjectInitializer);

	/* 高亮接口 */
	// 高亮显示Actor的虚函数实现，用于在游戏中突出显示这个入口
	virtual void HighlightActor_Implementation() override;
	/* 高亮接口结束 */

	/* 保存接口 */
	// 加载Actor的虚函数实现，用于在游戏加载时恢复这个入口的状态
	virtual void LoadActor_Implementation() override;
	/* 保存接口结束 */

	// 可编辑的属性，用于指定目标地图的软对象指针
	// TSoftObjectPtr允许地图被延迟加载，避免在编辑器中直接加载大型地图
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;

	// 可编辑的属性，用于指定目标地图中玩家起始位置的标签
	// FName是一个用于标识的字符串，可以用来在目标地图中查找对应的玩家起始点
	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;

protected:
	
	// 虚函数，用于处理球体重叠事件
	// 当另一个Actor的组件与这个Actor的球体组件重叠时，此函数被调用
	// 参数说明：
	// - OverlappedComponent：触发重叠的组件（属于这个Actor）
	// - OtherActor：与球体重叠的另一个Actor
	// - OtherComp：与球体重叠的另一个组件（属于OtherActor）
	// - OtherBodyIndex：重叠发生的另一个物体的身体索引
	// - bFromSweep：如果重叠是由于扫描测试引起的，则为true
	// - SweepResult：包含重叠测试结果的FHitResult结构体
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								 const FHitResult& SweepResult) override;


};
