// Copyright RickYan


#include "AbilitySystem/Data/LootTiers.h"

// ULootTiers类的方法，用于根据预设的战利品项生成实际的战利品列表
TArray<FLootItem> ULootTiers::GetLootItems()
{
	TArray<FLootItem> ReturnItems; // 用于存储最终生成的战利品项列表

	// 遍历每个预设的战利品项
	for (FLootItem& Item : LootItems)
	{
		// 根据最大生成数量进行循环
		for (int32 i = 0; i < Item.MaxNumberToSpawn; ++i)
		{
			// 生成一个1到100之间的随机数，如果这个数小于战利品项的生成概率，则添加到返回列表中
			if (FMath::FRandRange(1.f, 100.f) < Item.ChanceToSpawn)
			{
				FLootItem NewItem; // 创建一个新的战利品项
				NewItem.LootClass = Item.LootClass; // 设置新战利品项的类
				NewItem.bLootLevelOverride = Item.bLootLevelOverride; // 设置新战利品项的等级覆盖标志
				ReturnItems.Add(NewItem); // 将新战利品项添加到返回列表中
			}
		}
	}

	return ReturnItems; // 返回生成的战利品项列表
}

