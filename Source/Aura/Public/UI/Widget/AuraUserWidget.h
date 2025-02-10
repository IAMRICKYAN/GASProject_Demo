// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 蓝图可调用的函数，用于设置小部件控制器
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	// 蓝图只读属性，存储小部件控制器的指针
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
protected:
	// 蓝图可实现的事件，在小部件控制器被设置后触发
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();

};
