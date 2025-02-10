// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "Aura/Aura.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/HighlightInterface.h"
#include "Interaction/SaveInterface.h"
#include "Checkpoint.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class AURA_API ACheckpoint : public APlayerStart, public ISaveInterface, public IHighlightInterface
{
	GENERATED_BODY()
public:
	ACheckpoint(const FObjectInitializer& ObjectInitializer);

	/* Save Interface */
	// 虚函数，用于确定是否应该加载变换（位置、旋转、缩放）
	// 重写基类的方法，默认返回false，表示不加载变换
	virtual bool ShouldLoadTransform_Implementation() override { return false; };

	// 虚函数，用于加载角色
	// 重写基类的方法，具体实现将在派生类中提供
	virtual void LoadActor_Implementation() override;
	/* end Save Interface */

	// BlueprintReadWrite属性，表示这个变量可以在蓝图中读写
	// SaveGame属性，表示这个变量应该被保存到存档中
	// bReached变量，用于表示是否达到了某个状态或条件
	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bReached = false;

	// EditAnywhere属性，表示这个变量可以在编辑器的任何地方被修改
	// bBindOverlapCallback变量，用于表示是否应该绑定重叠事件回调
	UPROPERTY(EditAnywhere)
	bool bBindOverlapCallback = true;

protected:

	// 声明一个UFUNCTION，表示这是一个可以在引擎中调用的函数
	// 虚函数，当球体组件与另一个组件重叠时调用
	// 参数包括重叠的组件、其他演员、其他组件、其他身体索引、是否来自扫描、扫描结果
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 虚函数，重写基类的BeginPlay方法，在游戏开始时调用
	virtual void BeginPlay() override;

	/* Highlight Interface */
	// 虚函数，实现高亮接口中的SetMoveToLocation方法，用于设置移动到位置的逻辑
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	// 虚函数，实现高亮接口中的HighlightActor方法，用于高亮显示演员
	virtual void HighlightActor_Implementation() override;
	// 虚函数，实现高亮接口中的UnHighlightActor方法，用于取消高亮显示演员
	virtual void UnHighlightActor_Implementation() override;
	/* Highlight Interface */

	// UPROPERTY属性，VisibleAnywhere表示这个属性在编辑器中始终可见，但不能被修改
	// TObjectPtr<USceneComponent>表示这是一个指向USceneComponent的弱指针
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MoveToComponent;
	
	// UPROPERTY属性，EditDefaultsOnly表示这个属性只能在编辑器的默认设置中修改
	// int32表示这是一个32位的整数，用于自定义深度模板覆盖
	UPROPERTY(EditDefaultsOnly)
	int32 CustomDepthStencilOverride = CUSTOM_DEPTH_TAN;

	// UFUNCTION属性，BlueprintImplementableEvent表示这是一个蓝图实现的事件
	// 函数用于处理到达检查点的事件，参数是一个动态材质实例
	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);

	// UFUNCTION属性，BlueprintCallable表示这个函数可以在蓝图中被调用
	// 函数用于处理发光效果
	UFUNCTION(BlueprintCallable)
	void HandleGlowEffects();

	// UPROPERTY属性，VisibleAnywhere和BlueprintReadOnly表示这个属性在编辑器中可见，但在蓝图中只读
	// TObjectPtr<UStaticMeshComponent>表示这是一个指向UStaticMeshComponent的弱指针
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;
	
	// UPROPERTY属性，VisibleAnywhere表示这个属性在编辑器中始终可见，但不能被修改
	// TObjectPtr<USphereComponent>表示这是一个指向USphereComponent的弱指针
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

};
