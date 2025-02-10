// Copyright RickYan


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

// 初始化加载槽，创建并配置每个加载槽的视图模型
void UMVVM_LoadScreen::InitializeLoadSlots()
{
	// 创建第一个加载槽视图模型实例，设置名称和索引，然后添加到映射中
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);

	// 创建第二个加载槽视图模型实例，设置名称和索引，然后添加到映射中
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SlotIndex = 1;
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlots.Add(1, LoadSlot_1);

	// 创建第三个加载槽视图模型实例，设置名称和索引，然后添加到映射中
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);

	// 设置加载槽的数量
	SetNumLoadSlots(LoadSlots.Num());
}

// 根据索引获取加载槽视图模型
UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	// 从映射中查找并返回指定索引的加载槽视图模型
	// 注意：FindChecked会在找不到元素时引发异常，确保调用此函数时索引有效
	return LoadSlots.FindChecked(Index);
}


// 处理新建槽按钮按下事件，创建新的游戏槽
void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
	// 获取当前游戏模式并转换为AAuraGameModeBase类型
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	// 如果游戏模式无效，显示调试信息并返回
	if (!IsValid(AuraGameMode))
	{
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Magenta, FString("Please switch to Single Player"));
		return;
	}

	// 设置新游戏槽的默认地图名称、玩家名称、玩家等级、槽状态和玩家起始标签
	LoadSlots[Slot]->SetMapName(AuraGameMode->DefaultMapName);
	LoadSlots[Slot]->SetPlayerName(EnteredName);
	LoadSlots[Slot]->SetPlayerLevel(1);
	LoadSlots[Slot]->SlotStatus = Taken;
	LoadSlots[Slot]->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;
	// 设置地图资产名称
	LoadSlots[Slot]->MapAssetName = AuraGameMode->DefaultMap.ToSoftObjectPath().GetAssetName();

	// 保存游戏槽数据并初始化游戏槽
	AuraGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	LoadSlots[Slot]->InitializeSlot();

	// 获取游戏实例并更新加载槽名称、索引和玩家起始标签
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(AuraGameMode->GetGameInstance());
	AuraGameInstance->LoadSlotName = LoadSlots[Slot]->GetLoadSlotName();
	AuraGameInstance->LoadSlotIndex = LoadSlots[Slot]->SlotIndex;
	AuraGameInstance->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;
}

// 处理新建游戏按钮按下事件，切换到新建游戏界面
void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	// 广播切换小部件开关索引到新建游戏界面
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

// 处理选择槽按钮按下事件，选择一个游戏槽
void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
	// 广播槽选中事件
	SlotSelected.Broadcast();
	// 遍历所有加载槽，启用或禁用选择按钮
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		if (LoadSlot.Key == Slot)
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(false);
		}
		else
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(true);
		}
	}
	// 更新选中的游戏槽
	SelectedSlot = LoadSlots[Slot];
}

// 处理删除按钮按下事件，删除选中的游戏槽
void UMVVM_LoadScreen::DeleteButtonPressed()
{
	// 如果选中的游戏槽有效，则删除游戏槽数据并更新槽状态
	if (IsValid(SelectedSlot))
	{
		AAuraGameModeBase::DeleteSlot(SelectedSlot->GetLoadSlotName(), SelectedSlot->SlotIndex);
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitializeSlot();
		// 启用选择按钮
		SelectedSlot->EnableSelectSlotButton.Broadcast(true);
	}
}


// 处理播放按钮按下事件，开始游戏
void UMVVM_LoadScreen::PlayButtonPressed()
{
	// 获取当前游戏模式并转换为AAuraGameModeBase类型
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	// 获取游戏实例并转换为UAuraGameInstance类型
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(AuraGameMode->GetGameInstance());
	// 设置游戏实例的玩家起始标签、加载槽名称和索引
	AuraGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
	AuraGameInstance->LoadSlotName = SelectedSlot->GetLoadSlotName();
	AuraGameInstance->LoadSlotIndex = SelectedSlot->SlotIndex;
	
	// 如果选中的加载槽有效，则触发地图跳转
	if (IsValid(SelectedSlot))
	{
		AuraGameMode->TravelToMap(SelectedSlot);
	}
}

// 加载加载槽数据
void UMVVM_LoadScreen::LoadData()
{
	// 获取当前游戏模式并转换为AAuraGameModeBase类型
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	// 如果游戏模式无效，则直接返回
	if (!IsValid(AuraGameMode)) return;
	// 遍历所有加载槽
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		// 获取保存游戏对象
		ULoadScreenSaveGame* SaveObject = AuraGameMode->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);

		// 从保存游戏对象中获取玩家名称、保存槽状态等信息
		const FString PlayerName = SaveObject->PlayerName;
		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;

		// 更新加载槽视图模型的属性
		LoadSlot.Value->SlotStatus = SaveSlotStatus;
		LoadSlot.Value->SetPlayerName(PlayerName);
		LoadSlot.Value->InitializeSlot();
		
		// 设置地图名称、玩家起始标签和玩家等级
		LoadSlot.Value->SetMapName(SaveObject->MapName);
		LoadSlot.Value->PlayerStartTag = SaveObject->PlayerStartTag;
		LoadSlot.Value->SetPlayerLevel(SaveObject->PlayerLevel);
	}
}

// 设置加载槽的数量
void UMVVM_LoadScreen::SetNumLoadSlots(int32 InNumLoadSlots)
{
	// 使用MVVM框架的宏来设置属性值，这将触发属性更改通知
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots, InNumLoadSlots);
}

