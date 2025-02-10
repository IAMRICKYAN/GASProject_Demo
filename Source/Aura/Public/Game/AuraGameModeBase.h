// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class ULootTiers;
class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAbilityInfo;
class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	// UPROPERTY宏用于声明属性，这些属性可以在编辑器中编辑，并且可以由Unreal Engine的序列化系统保存和加载

	// 角色类信息，只能在默认值中编辑，分类为"Character Class Defaults"
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	// 能力信息，只能在默认值中编辑，分类为"Ability Info"
	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	// 掉落等级信息，只能在默认值中编辑，分类为"Loot Tiers"
	UPROPERTY(EditDefaultsOnly, Category = "Loot Tiers")
	TObjectPtr<ULootTiers> LootTiers;

	// 保存槽数据，接收一个UMVVM_LoadSlot指针和槽索引
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);

	// 获取保存槽数据，根据槽名称和索引返回ULoadScreenSaveGame对象
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;

	// 静态函数，用于删除指定名称和索引的保存槽
	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);

	// 检索游戏内的保存数据，返回ULoadScreenSaveGame对象
	ULoadScreenSaveGame* RetrieveInGameSaveData();

	// 保存游戏内进度数据，接收一个ULoadScreenSaveGame指针
	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject);

	// 保存世界状态，接收一个UWorld指针和目标地图资产名称（默认为空字符串）
	void SaveWorldState(UWorld* World, const FString& DestinationMapAssetName = FString("")) const;

	// 加载世界状态，接收一个UWorld指针
	void LoadWorldState(UWorld* World) const;

	// 根据槽信息传送到地图
	void TravelToMap(UMVVM_LoadSlot* Slot);

	// 加载屏幕保存游戏类，只能在默认值中编辑
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	// 默认地图名称，只能在默认值中编辑
	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	// 默认地图，只能在默认值中编辑
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	// 默认玩家起始标签，只能在默认值中编辑
	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;

	// 地图映射，键为地图资产名称，值为地图的软对象指针，只能在默认值中编辑
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	// 根据地图资产名称获取地图名称
	FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;

	// 选择玩家起始位置的虚函数实现，接收一个AController指针
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	// 玩家死亡时调用的函数，接收一个ACharacter指针
	void PlayerDied(ACharacter* DeadCharacter);

protected:
	virtual void BeginPlay() override;
	
};
