// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FAuraAbilityInfo;
// 定义一个结构体，用于表示UI小部件的行数据，继承自FTableRowBase，可在蓝图中使用
USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	// Gameplay标签，用于标识消息的类型或分类
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	// 消息文本，显示在UI上的文本内容
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	// 消息小部件的蓝图类，用于实例化显示消息的UI小部件
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UAuraUserWidget> MessageWidget;

	// 图片纹理，用于在UI上显示的图像
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};


class UAuraUserWidget;
class UAbilityInfo;
class UAuraAbilitySystemComponent;

// 声明一个动态多播委托，当属性值变化时触发，携带一个float类型的参数表示新的属性值
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

// 声明一个动态多播委托，当等级变化时触发，携带两个参数：int32类型的NewLevel表示新的等级，bool类型的bLevelUp表示是否升级
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelChangedSignature, int32, NewLevel, bool, bLevelUp);

// 声明一个动态多播委托，用于传递UI小部件行数据，携带一个FUIWidgetRow类型的参数表示行数据
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	// 重写基类函数，用于广播初始值
	virtual void BroadcastInitialValues() override;

	// 重写基类函数，用于将回调函数绑定到依赖项
	virtual void BindCallbacksToDependencies() override;

	// 蓝图可分配的委托，当健康值变化时触发，属于"GAS|Attributes"分类
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	// 蓝图可分配的委托，当最大健康值变化时触发，属于"GAS|Attributes"分类
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	// 蓝图可分配的委托，当法力值变化时触发，属于"GAS|Attributes"分类
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	// 蓝图可分配的委托，当最大法力值变化时触发，属于"GAS|Attributes"分类
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	// 蓝图可分配的委托，用于传递消息小部件行数据，属于"GAS|Messages"分类
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	// 蓝图可分配的委托，当经验值百分比变化时触发，属于"GAS|XP"分类
	UPROPERTY(BlueprintAssignable, Category="GAS|XP")
	FOnAttributeChangedSignature OnXPPercentChangedDelegate;

	// 蓝图可分配的委托，当玩家等级变化时触发，属于"GAS|Level"分类
	UPROPERTY(BlueprintAssignable, Category="GAS|Level")
	FOnLevelChangedSignature OnPlayerLevelChangedDelegate;

protected:
	// 在默认情况下可编辑，蓝图可读，属于"Widget Data"分类的数据表属性，用于存储消息小部件数据
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	// 模板函数，用于通过标签从数据表中获取行数据
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	// 当经验值变化时调用的函数，参数为新的经验值
	void OnXPChanged(int32 NewXP);

	// 当能力装备时调用的函数，参数包括能力标签、状态、槽位和前一个槽位，函数为常量表示不会修改对象状态
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const;
};

// 模板函数实现，用于通过标签从数据表中查找行，返回相应类型的指针
template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}

