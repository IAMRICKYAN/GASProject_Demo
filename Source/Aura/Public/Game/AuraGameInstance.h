// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AuraGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	// UPROPERTY宏用于声明属性，这些属性可以在编辑器中编辑，并且可以由Unreal Engine的序列化系统保存和加载

	// 玩家起始位置的标签，默认为空
	// FName是一个用于标识的字符串，可以用来在地图中查找对应的玩家起始点
	UPROPERTY()
	FName PlayerStartTag = FName();

	// 加载槽的名称，用于保存和加载游戏进度，默认为空字符串
	// FString是Unreal Engine中的字符串类型，用于存储文本信息
	UPROPERTY()
	FString LoadSlotName = FString();

	// 加载槽的索引，用于标识特定的保存游戏进度，默认为0
	// int32是32位整数类型，用于存储整数值
	UPROPERTY()
	int32 LoadSlotIndex = 0;

};
