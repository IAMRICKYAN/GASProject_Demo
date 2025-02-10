// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointCollection.generated.h"

UCLASS()
class AURA_API APointCollection : public AActor
{
	GENERATED_BODY()
	
public:	
	APointCollection();

	// UFUNCTION宏，标记该函数为蓝图可调用的纯函数，不修改对象状态，仅返回值
	UFUNCTION(BlueprintPure)
	// GetGroundPoints函数，根据地面位置、点数和偏航覆盖获取地面点数组
	TArray<USceneComponent*> GetGroundPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride = 0.f);
	
protected:
	virtual void BeginPlay() override;

	// UPROPERTY宏，标记该属性为蓝图只读，在任何地方可见
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	// ImmutablePts数组，存储不可变的场景组件点
	TArray<USceneComponent*> ImmutablePts;

	// 以下为单个场景组件点的属性，均标记为蓝图只读，在任何地方可见
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_0; // 第0个点

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_1; // 第1个点

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_2; // 第2个点

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_3; // 第3个点

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_4; // 第4个点

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_5; // 第5个点

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_6; // 第6个点

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_7; // 第7个点

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_8; // 第8个点

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_9; // 第9个点

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_10; // 第10个点

	
};
