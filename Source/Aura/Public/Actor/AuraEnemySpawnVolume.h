// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "AuraEnemySpawnVolume.generated.h"

class AAuraEnemySpawnPoint;
class UBoxComponent;
UCLASS()
class AURA_API AAuraEnemySpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	AAuraEnemySpawnVolume();

	/* Save Interface */
	// 虚拟函数，用于加载演员的状态，覆盖了基类的实现
	virtual void LoadActor_Implementation() override;
	/* end Save Interface */

	// 蓝图可读，保存游戏时也会保存的属性，表示是否达到某个状态
	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;

protected:
	// 覆盖基类的BeginPlay函数，用于在游戏开始时进行初始化
	virtual void BeginPlay() override;

	// 事件函数，当盒子组件与另一个组件重叠时调用
	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 可在编辑器中任意修改的属性，存储了一系列敌人生成点的指针
	UPROPERTY(EditAnywhere)
	TArray<AAuraEnemySpawnPoint*> SpawnPoints;
private:

	// 在编辑器中始终可见的属性，指向一个盒子组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;

};
