// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButton, bool, bEnable);

/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:

	// 可在蓝图中分配的事件，用于设置小部件切换器的索引
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	// 可在蓝图中分配的事件，用于启用或禁用选择槽按钮
	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlotButton EnableSelectSlotButton;

	// 初始化槽，可能包括设置槽的默认状态和属性
	void InitializeSlot();

	// 槽的索引，用于标识特定的槽
	UPROPERTY()
	int32 SlotIndex;

	// 槽的状态，表示保存槽的当前状态（例如：空、已保存、已锁定等）
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	// 玩家起始标签，用于标识玩家在地图上的起始位置
	UPROPERTY()
	FName PlayerStartTag;

	// 地图资产名称，表示与槽关联的地图的名称
	UPROPERTY()
	FString MapAssetName;

	/** Field Notifies */
	// 字段通知区域，可能包含用于通知属性更改的代码或宏

	// 设置玩家名称
	void SetPlayerName(FString InPlayerName);
	// 设置地图名称
	void SetMapName(FString InMapName);
	// 设置玩家等级
	void SetPlayerLevel(int32 InLevel);
	// 设置加载槽名称
	void SetLoadSlotName(FString InLoadSlotName);

	// 获取玩家名称，常量函数，不会修改对象状态
	FString GetPlayerName() const { return PlayerName; }
	// 获取地图名称，常量函数，不会修改对象状态
	FString GetMapName() const { return MapName; }
	// 获取玩家等级，常量函数，不会修改对象状态
	int32 GetPlayerLevel() const { return PlayerLevel; }
	// 获取加载槽名称，常量函数，不会修改对象状态
	FString GetLoadSlotName() const { return LoadSlotName; }


private:

	// 玩家名称，可在编辑器中任意位置编辑，支持蓝图读写，具有字段通知机制，并提供setter和getter函数，允许私有访问
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	FString PlayerName;

	// 地图名称，可在编辑器中任意位置编辑，支持蓝图读写，具有字段通知机制，并提供setter和getter函数，允许私有访问
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	FString MapName;

	// 玩家等级，可在编辑器中任意位置编辑，支持蓝图读写，具有字段通知机制，并提供setter和getter函数，允许私有访问
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	int32 PlayerLevel;

	// 加载槽名称，可在编辑器中任意位置编辑，支持蓝图读写，具有字段通知机制，并提供setter和getter函数，允许私有访问
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	FString LoadSlotName;

};
