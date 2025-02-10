// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ULoadScreenWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	// 该函数为一个蓝图可执行事件，同时也可以被蓝图调用
	// 用于在蓝图中对小部件进行初始化设置
	// 此函数没有参数，通常在蓝图编辑器中实现具体的初始化逻辑
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintInitializeWidget();

};
