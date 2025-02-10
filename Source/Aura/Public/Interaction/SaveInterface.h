// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ISaveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// 定义一个蓝图可调用和原生事件函数，用于判断是否应该加载变换（例如位置、旋转和缩放）
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ShouldLoadTransform();

	// 定义一个蓝图可调用和原生事件函数，用于加载演员（Actor）的相关数据
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LoadActor();

};
