// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

// 声明一个动态多播委托，当玩家统计数据变化时触发，携带一个int32类型的参数表示新的值
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewValue);

// 声明一个动态多播委托，用于传递能力信息，携带一个常量引用参数，类型为FAuraAbilityInfo，表示能力的信息
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo&, Info);


class UAttributeSet;
class UAbilitySystemComponent;
class AAuraPlayerController;
class AAuraPlayerState;
class UAuraAbilitySystemComponent;
class UAuraAttributeSet;
class UAbilityInfo;

// 定义一个结构体，用于在蓝图中传递小部件控制器的参数
USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	// 默认构造函数
	FWidgetControllerParams() {}

	// 参数化构造函数，用于初始化结构体的成员变量
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	// 玩家控制器指针，可以在蓝图中编辑和读取
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	// 玩家状态指针，可以在蓝图中编辑和读取
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	// 能力系统组件指针，可以在蓝图中编辑和读取
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	// 属性集指针，可以在蓝图中编辑和读取
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};


/**
 * 
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
public:
	// 蓝图可调用的函数，用于设置小部件控制器的参数
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	// 蓝图可调用的函数，用于广播初始值
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();

	// 虚函数，用于绑定回调到依赖项
	virtual void BindCallbacksToDependencies();

	// 蓝图可分配的委托，用于通知能力信息的变化，分类在"GAS|Messages"下
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FAbilityInfoSignature AbilityInfoDelegate;

	// 函数，用于广播能力信息
	void BroadcastAbilityInfo();
protected:

	// 编辑默认值时-only属性，蓝图可读，用于存储能力信息，分类在"Widget Data"下
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	// 蓝图只读属性，表示玩家控制器，分类在"WidgetController"下
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	// 蓝图只读属性，表示玩家状态，分类在"WidgetController"下
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	// 蓝图只读属性，表示能力系统组件，分类在"WidgetController"下
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// 蓝图只读属性，表示属性集，分类在"WidgetController"下
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	// 蓝图只读属性，表示Aura玩家控制器，分类在"WidgetController"下
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<AAuraPlayerController> AuraPlayerController;

	// 蓝图只读属性，表示Aura玩家状态，分类在"WidgetController"下
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<AAuraPlayerState> AuraPlayerState;

	// 蓝图只读属性，表示Aura能力系统组件，分类在"WidgetController"下
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	// 蓝图只读属性，表示Aura属性集，分类在"WidgetController"下
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;

	// 函数，用于获取Aura玩家控制器指针
	AAuraPlayerController* GetAuraPC();

	// 函数，用于获取Aura玩家状态指针
	AAuraPlayerState* GetAuraPS();

	// 函数，用于获取Aura能力系统组件指针
	UAuraAbilitySystemComponent* GetAuraASC();

	// 函数，用于获取Aura属性集指针
	UAuraAttributeSet* GetAuraAS();
};
