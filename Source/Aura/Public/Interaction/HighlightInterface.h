// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HighlightInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UHighlightInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IHighlightInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// 蓝图原生事件，用于高亮显示角色
	UFUNCTION(BlueprintNativeEvent)
	void HighlightActor();

	// 蓝图原生事件，用于取消高亮显示角色
	UFUNCTION(BlueprintNativeEvent)
	void UnHighlightActor();

	// 蓝图原生事件，用于设置移动到指定位置
	// 参数 OutDestination：引用传递，用于输出计算后的目的地位置
	UFUNCTION(BlueprintNativeEvent)
	void SetMoveToLocation(FVector& OutDestination);

};
