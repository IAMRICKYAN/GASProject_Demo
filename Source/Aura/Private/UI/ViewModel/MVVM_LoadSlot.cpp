// Copyright RickYan


#include "UI/ViewModel/MVVM_LoadSlot.h"

// 初始化槽，根据槽状态设置小部件切换器的索引，并广播该索引
void UMVVM_LoadSlot::InitializeSlot()
{
	// 获取槽状态的值，该值将用作小部件切换器的索引
	const int32 WidgetSwitcherIndex = SlotStatus.GetValue();
	// 广播小部件切换器的索引，以便在蓝图中可以响应并更新UI
	SetWidgetSwitcherIndex.Broadcast(WidgetSwitcherIndex);
}

// 设置玩家名称，并使用MVVM框架的宏来更新属性值
void UMVVM_LoadSlot::SetPlayerName(FString InPlayerName)
{
	// 使用MVVM框架的宏来设置玩家名称属性，这将自动处理属性更改通知
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}

// 设置地图名称，并使用MVVM框架的宏来更新属性值
void UMVVM_LoadSlot::SetMapName(FString InMapName)
{
	// 使用MVVM框架的宏来设置地图名称属性，这将自动处理属性更改通知
	UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName);
}

// 设置玩家等级，并使用MVVM框架的宏来更新属性值
void UMVVM_LoadSlot::SetPlayerLevel(int32 InLevel)
{
	// 使用MVVM框架的宏来设置玩家等级属性，这将自动处理属性更改通知
	UE_MVVM_SET_PROPERTY_VALUE(PlayerLevel, InLevel);
}

// 设置加载槽名称，并使用MVVM框架的宏来更新属性值
void UMVVM_LoadSlot::SetLoadSlotName(FString InLoadSlotName)
{
	// 使用MVVM框架的宏来设置加载槽名称属性，这将自动处理属性更改通知
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}
