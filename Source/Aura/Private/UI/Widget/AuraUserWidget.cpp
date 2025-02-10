// Copyright RickYan


#include "UI/Widget/AuraUserWidget.h"

/**
 * 设置小部件控制器的函数。
 * 此函数将外部提供的小部件控制器赋值给成员变量，并触发控制器设置完成的事件。
 * 
 * @param InWidgetController 指向要设置的小部件控制器的指针，该控制器将负责管理小部件的行为。
 */
void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	// 将传入的小部件控制器赋值给成员变量，以便在类中其他地方使用
	WidgetController = InWidgetController;

	// 触发小部件控制器设置完成的事件，允许在蓝图中执行额外的设置或初始化操作
	WidgetControllerSet();
}

