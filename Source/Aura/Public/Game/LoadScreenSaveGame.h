// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

class UGameplayAbility;
UENUM(BlueprintType)
// 枚举类型，表示保存槽的状态
enum ESaveSlotStatus
{
	Vacant,    // 空闲状态，表示保存槽当前没有被使用
	EnterName, // 输入名称状态，表示用户正在为保存槽输入名称
	Taken      // 已占用状态，表示保存槽已经被使用
};


USTRUCT()
struct FSavedActor
{
	GENERATED_BODY()

	UPROPERTY()
	FName ActorName = FName();  // 存储演员的名字

	UPROPERTY()
	FTransform Transform = FTransform();  // 存储演员的变换信息

	// 从演员序列化的变量 - 仅标记有SaveGame说明符的那些
	UPROPERTY()
	TArray<uint8> Bytes;  // 存储演员的序列化字节数组
};


// 内联函数，用于比较两个FSavedActor对象是否相等
// 参数Left是左边的FSavedActor对象
// 参数Right是右边的FSavedActor对象
// 返回值为bool，如果两个对象的ActorName相等则返回true，否则返回false
inline bool operator==(const FSavedActor& Left, const FSavedActor& Right)
{
	return Left.ActorName == Right.ActorName;
}


USTRUCT()
struct FSavedMap
{
	GENERATED_BODY()

	UPROPERTY()
	FString MapAssetName = FString();  // 存储地图资产的名称

	UPROPERTY()
	TArray<FSavedActor> SavedActors;  // 存储已保存的演员数组
};


USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()

	// GameplayAbility类的一个子类，用于指定游戏玩法能力
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ClassDefaults")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	// 用于标识能力的标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityTag = FGameplayTag();

	// 用于标识能力状态的标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityStatus = FGameplayTag();

	// 用于标识能力槽位的标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilitySlot = FGameplayTag();

	// 用于标识能力类型的标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityType = FGameplayTag();

	// 能力的等级，默认为1
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 AbilityLevel = 1;
};


// 内联函数，用于比较两个FSavedAbility对象是否相等
// 参数Left是左边的FSavedAbility对象
// 参数Right是右边的FSavedAbility对象
// 函数通过精确匹配两个对象的AbilityTag来判断它们是否相等
// 返回值为bool，如果AbilityTag完全匹配则返回true，否则返回false
inline bool operator==(const FSavedAbility& Left, const FSavedAbility& Right)
{
	return Left.AbilityTag.MatchesTagExact(Right.AbilityTag);
}


/**
 * 
 */
UCLASS()
class AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	FString SlotName = FString(); // 存储槽位的名称

	UPROPERTY()
	int32 SlotIndex = 0; // 存储槽位的索引

	UPROPERTY()
	FString PlayerName = FString("Default Name"); // 存储玩家的名称，默认为"Default Name"

	UPROPERTY()
	FString MapName = FString("Default Map Name"); // 存储地图的名称，默认为"Default Map Name"

	UPROPERTY()
	FString MapAssetName = FString("Default Map Asset Name"); // 存储地图资产的名称，默认为"Default Map Asset Name"

	UPROPERTY()
	FName PlayerStartTag; // 存储玩家起始位置的标签

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant; // 存储槽位的状态，默认为"Vacant"（空置）

	UPROPERTY()
	bool bFirstTimeLoadIn = true; // 标记是否是第一次加载，默认为true

	/* Player */

	UPROPERTY()
	int32 PlayerLevel = 1; // 存储玩家的等级，默认为1

	UPROPERTY()
	int32 XP = 0; // 存储玩家的经验值，默认为0

	UPROPERTY()
	int32 SpellPoints = 0; // 存储玩家的法术点数，默认为0

	UPROPERTY()
	int32 AttributePoints = 0; // 存储玩家的属性点数，默认为0

	UPROPERTY()
	float Strength = 0; // 存储玩家的力量值，默认为0

	UPROPERTY()
	float Intelligence = 0; // 存储玩家的智力值，默认为0

	UPROPERTY()
	float Resilience = 0; // 存储玩家的韧性值，默认为0

	UPROPERTY()
	float Vigor = 0; // 存储玩家的活力值，默认为0

	/* Abilities */

	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities; // 存储已保存的能力数组

	UPROPERTY()
	TArray<FSavedMap> SavedMaps; // 存储已保存的地图数组

	FSavedMap GetSavedMapWithMapName(const FString& InMapName); // 根据地图名称获取已保存的地图信息
	bool HasMap(const FString& InMapName); // 检查是否已保存指定名称的地图

};
