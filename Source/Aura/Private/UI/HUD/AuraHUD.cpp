// Copyright RickYan


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"

// 获取覆盖层小部件控制器的函数，如果尚未初始化则创建并初始化
UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	// 检查覆盖层小部件控制器是否为空，如果是则创建并初始化
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams); // 设置小部件控制器参数
		OverlayWidgetController->BindCallbacksToDependencies(); // 绑定回调函数到依赖项
	}
	return OverlayWidgetController; // 返回覆盖层小部件控制器
}

// 获取属性菜单小部件控制器的函数，如果尚未初始化则创建并初始化
UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	// 检查属性菜单小部件控制器是否为空，如果是则创建并初始化
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams); // 设置小部件控制器参数
		AttributeMenuWidgetController->BindCallbacksToDependencies(); // 绑定回调函数到依赖项
	}
	return AttributeMenuWidgetController; // 返回属性菜单小部件控制器
}

// 获取技能菜单小部件控制器的函数，如果尚未初始化则创建并初始化
USpellMenuWidgetController* AAuraHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	// 检查技能菜单小部件控制器是否为空，如果是则创建并初始化
	if (SpellMenuWidgetController == nullptr)
	{
		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetControllerParams(WCParams); // 设置小部件控制器参数
		SpellMenuWidgetController->BindCallbacksToDependencies(); // 绑定回调函数到依赖项
	}
	return SpellMenuWidgetController; // 返回技能菜单小部件控制器
}

// 初始化覆盖层的函数，设置玩家控制器、玩家状态、能力系统组件和属性集
void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// 检查覆盖层小部件类和控制器类是否已初始化
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD"));
	
	// 创建覆盖层小部件并强制转换为Aura用户小部件
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);
	
	// 创建小部件控制器参数
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	// 获取覆盖层小部件控制器
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	// 设置小部件控制器并广播初始值
	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	// 将小部件添加到视口
	Widget->AddToViewport();
}

