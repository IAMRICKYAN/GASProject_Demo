// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelected);

class UMVVM_LoadSlot;
/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:

	// 初始化加载槽，可能包括创建和配置加载槽视图模型
	void InitializeLoadSlots();

	// 可在蓝图中分配的加载槽选中事件
	UPROPERTY(BlueprintAssignable)
	FSlotSelected SlotSelected;

	// 在默认编辑器中可编辑的加载槽视图模型类
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	// 根据索引获取加载槽视图模型的纯蓝图函数
	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;

	// 新建槽按钮按下时的处理函数，用于创建新槽并设置名称
	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 Slot, const FString& EnteredName);

	// 新游戏按钮按下时的处理函数，用于在指定槽上开始新游戏
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 Slot);

	// 选择槽按钮按下时的处理函数，用于选择指定的加载槽
	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 Slot);

	// 删除按钮按下时的处理函数，用于删除当前选中的加载槽
	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();

	// 开始游戏按钮按下时的处理函数，用于开始游戏
	UFUNCTION(BlueprintCallable)
	void PlayButtonPressed();

	// 加载数据，可能包括从存储加载槽信息
	void LoadData();

	// 设置加载槽的数量
	void SetNumLoadSlots(int32 InNumLoadSlots);

	// 获取加载槽的数量
	int32 GetNumLoadSlots() const { return NumLoadSlots; }

	
private:

	// 存储加载槽索引与对应视图模型实例的映射
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;

	// 第0个加载槽的视图模型实例
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;

	// 第1个加载槽的视图模型实例
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;

	// 第2个加载槽的视图模型实例
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;

	// 当前选中的加载槽视图模型实例
	UPROPERTY()
	UMVVM_LoadSlot* SelectedSlot;

	// 加载槽的数量，可在编辑器中任意位置编辑，支持蓝图读写，字段通知，具有setter和getter，允许私有访问
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	int32 NumLoadSlots;

};
