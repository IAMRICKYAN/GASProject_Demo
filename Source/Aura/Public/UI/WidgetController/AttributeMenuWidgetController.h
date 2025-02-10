// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAttributeInfo;
struct FAuraAttributeInfo;
struct FGameplayTag;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	// 绑定回调函数到依赖的对象上，重写基类中的虚函数
	virtual void BindCallbacksToDependencies() override;

	// 广播初始值，重写基类中的虚函数
	virtual void BroadcastInitialValues() override;

	// 蓝图可分配的属性信息委托，用于通知属性信息的变化
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	// 蓝图可分配的玩家属性点变化委托，用于通知玩家属性点的变化
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnPlayerStatChangedSignature AttributePointsChangedDelegate;

	// 蓝图可调用的函数，用于升级指定标签的属性
	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

protected:

	// 在默认情况下可编辑的属性信息对象指针
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:

	// 广播属性信息，用于通知特定的属性标签和属性值的变化
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;

};
