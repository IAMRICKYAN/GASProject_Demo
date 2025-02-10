// Copyright RickYan


#include "Game/AuraGameModeBase.h"

#include "EngineUtils.h"
#include "Aura/AuraLogChannels.h"
#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "GameFramework/Character.h"

// 保存槽数据到指定槽位
// 参数LoadSlot是指向UMVVM_LoadSlot的指针，包含了加载槽的信息
// 参数SlotIndex是保存槽的索引
void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	// 检查指定槽位是否已存在保存数据
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex))
	{
		// 如果存在，则删除原有的保存数据
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	}
	// 创建一个新的保存游戏对象
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	// 将保存游戏对象转换为ULoadScreenSaveGame类型
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	// 设置保存游戏对象的属性
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadScreenSaveGame->SaveSlotStatus = Taken; // 假设Taken是一个枚举值，表示槽位已被占用
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
	LoadScreenSaveGame->MapAssetName = LoadSlot->MapAssetName;
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
	// 将保存游戏对象保存到指定槽位
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}

// 获取指定槽位的保存数据
// 参数SlotName是保存槽的名称
// 参数SlotIndex是保存槽的索引
// 返回一个指向ULoadScreenSaveGame的指针，包含了加载屏幕的保存数据
ULoadScreenSaveGame* AAuraGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject = nullptr;
	// 检查指定槽位是否已存在保存数据
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		// 如果存在，则加载保存数据
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		// 如果不存在，则创建一个新的保存游戏对象
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	// 将保存游戏对象转换为ULoadScreenSaveGame类型
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	// 返回加载屏幕的保存数据
	return LoadScreenSaveGame;
}

// 删除指定槽位的保存数据
// 参数SlotName是保存槽的名称
// 参数SlotIndex是保存槽的索引
void AAuraGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	// 检查指定槽位是否已存在保存数据
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		// 如果存在，则删除保存数据
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}


// 检索游戏内的保存数据
// 返回一个指向ULoadScreenSaveGame的指针，包含了游戏内的保存数据
ULoadScreenSaveGame* AAuraGameModeBase::RetrieveInGameSaveData()
{
	// 获取游戏实例并转换为UAuraGameInstance类型
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());

	// 从游戏实例中获取加载槽的名称和索引
	const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;

	// 使用加载槽的名称和索引获取保存数据
	return GetSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);
}

// 保存游戏内的进度数据
// 参数SaveObject是指向ULoadScreenSaveGame的指针，包含了要保存的游戏数据
void AAuraGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveObject)
{
	// 获取游戏实例并转换为UAuraGameInstance类型
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());

	// 从游戏实例中获取加载槽的名称和索引
	const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;

	// 更新游戏实例中的玩家起始标签
	AuraGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;

	// 将保存对象保存到指定的槽位
	UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
}


// 保存当前世界状态到指定的保存游戏中
// 参数World是指向当前世界的指针
// 参数DestinationMapAssetName是目标地图资产的名称，用于指示玩家将前往的地图
void AAuraGameModeBase::SaveWorldState(UWorld* World, const FString& DestinationMapAssetName) const
{
	// 获取当前世界的地图名称，并移除流式关卡前缀
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	// 获取游戏实例并转换为UAuraGameInstance类型，确保转换成功
	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(GetGameInstance());
	check(AuraGI);

	// 获取指定槽位的保存游戏数据
	if (ULoadScreenSaveGame* SaveGame = GetSaveSlotData(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex))
	{
		// 如果提供了目标地图资产名称，则更新保存游戏中的地图资产名称和地图名称
		if (DestinationMapAssetName != FString(""))
		{
			SaveGame->MapAssetName = DestinationMapAssetName;
			SaveGame->MapName = GetMapNameFromMapAssetName(DestinationMapAssetName);
		}
		
		// 如果保存游戏中没有当前世界的地图数据，则添加新的保存地图数据
		if (!SaveGame->HasMap(WorldName))
		{
			FSavedMap NewSavedMap;
			NewSavedMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add(NewSavedMap);
		}

		// 获取当前世界的保存地图数据
		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
		SavedMap.SavedActors.Empty(); // 清空已保存的演员列表，我们将重新填充

		// 遍历世界中的所有演员，并保存符合条件的演员数据
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			// 如果演员无效或未实现USaveInterface接口，则跳过
			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;

			// 创建保存演员数据结构并填充数据
			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.Transform = Actor->GetTransform();

			// 使用内存写入器和档案来序列化演员数据
			FMemoryWriter MemoryWriter(SavedActor.Bytes);
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;
			Actor->Serialize(Archive);

			// 将保存的演员数据添加到保存地图数据中
			SavedMap.SavedActors.AddUnique(SavedActor);
		}

		// 更新保存游戏中的保存地图数据
		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapAssetName == WorldName)
			{
				MapToReplace = SavedMap;
			}
		}

		// 将更新后的保存游戏数据保存到槽位
		UGameplayStatics::SaveGameToSlot(SaveGame, AuraGI->LoadSlotName, AuraGI->LoadSlotIndex);
	}
}


// 加载当前世界状态从指定的保存游戏中
// 参数World是指向当前世界的指针
void AAuraGameModeBase::LoadWorldState(UWorld* World) const
{
	// 获取当前世界的地图名称，并移除流式关卡前缀
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	// 获取游戏实例并转换为UAuraGameInstance类型，确保转换成功
	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(GetGameInstance());
	check(AuraGI);

	// 检查指定槽位是否存在保存游戏
	if (UGameplayStatics::DoesSaveGameExist(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex))
	{
		// 加载保存游戏数据
		ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex));
		if (SaveGame == nullptr)
		{
			// 如果加载失败，记录错误并返回
			UE_LOG(LogAura, Error, TEXT("Failed to load slot"));
			return;
		}
		
		// 遍历世界中的所有演员
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			// 如果演员未实现USaveInterface接口，则跳过
			if (!Actor->Implements<USaveInterface>()) continue;

			// 遍历保存游戏中的已保存演员数据
			for (FSavedActor SavedActor : SaveGame->GetSavedMapWithMapName(WorldName).SavedActors)
			{
				// 如果找到匹配的演员名称
				if (SavedActor.ActorName == Actor->GetFName())
				{
					// 如果应该加载变换，则设置演员的变换
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SavedActor.Transform);
					}

					// 使用内存读取器和档案来反序列化演员数据
					FMemoryReader MemoryReader(SavedActor.Bytes);
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive); // 将二进制数据转换回变量

					// 执行演员的加载操作
					ISaveInterface::Execute_LoadActor(Actor);
				}
			}
		}
	}
}

// 根据加载槽信息传送到指定地图
// 参数Slot是指向UMVVM_LoadSlot的指针，包含了加载槽的信息
void AAuraGameModeBase::TravelToMap(UMVVM_LoadSlot* Slot)
{
	// 获取加载槽的名称和索引
	const FString SlotName = Slot->GetLoadSlotName();
	const int32 SlotIndex = Slot->SlotIndex;

	// 根据加载槽中的地图名称打开对应的地图级别
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}


// 从地图资产名称获取地图名称
// 参数MapAssetName是地图资产的名称
// 返回与地图资产名称对应的地图名称，如果未找到则返回空字符串
FString AAuraGameModeBase::GetMapNameFromMapAssetName(const FString& MapAssetName) const
{
	for (auto& Map : Maps)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName() == MapAssetName)
		{
			return Map.Key;
		}
	}
	return FString();
}

// 实现选择玩家起始点的函数
// 参数Player是指向玩家控制器的指针
// 返回一个指向选定的玩家起始点的指针，如果未找到则返回nullptr
AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	if (Actors.Num() > 0)
	{
		AActor* SelectedActor = Actors[0];
		for (AActor* Actor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == AuraGameInstance->PlayerStartTag)
				{
					SelectedActor = PlayerStart;
					break;
				}
			}
		}
		return SelectedActor;
	}
	return nullptr;
}

// 玩家死亡时调用的函数
// 参数DeadCharacter是指向已死亡角色的指针
void AAuraGameModeBase::PlayerDied(ACharacter* DeadCharacter)
{
	ULoadScreenSaveGame* SaveGame = RetrieveInGameSaveData();
	if (!IsValid(SaveGame)) return;

	// 使用保存游戏中的地图资产名称打开对应的地图级别
	UGameplayStatics::OpenLevel(DeadCharacter, FName(SaveGame->MapAssetName));
}

// 开始游戏时调用的函数
void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	// 添加默认地图名称和地图到Maps映射中
	Maps.Add(DefaultMapName, DefaultMap);
}

