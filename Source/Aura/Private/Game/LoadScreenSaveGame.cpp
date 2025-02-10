// Copyright RickYan


#include "Game/LoadScreenSaveGame.h"

// 从保存的地图列表中获取与指定地图名称匹配的保存地图信息
// 参数InMapName是要查找的地图名称
// 返回与InMapName匹配的FSavedMap对象，如果未找到则返回一个默认构造的FSavedMap对象
FSavedMap ULoadScreenSaveGame::GetSavedMapWithMapName(const FString& InMapName)
{
	for (const FSavedMap& Map : SavedMaps)
	{
		if (Map.MapAssetName == InMapName)
		{
			return Map;
		}
	}
	return FSavedMap();
}

// 检查保存的地图列表中是否存在指定地图名称的地图
// 参数InMapName是要检查的地图名称
// 如果找到匹配的地图名称，则返回true，否则返回false
bool ULoadScreenSaveGame::HasMap(const FString& InMapName)
{
	for (const FSavedMap& Map : SavedMaps)
	{
		if (Map.MapAssetName == InMapName)
		{
			return true;
		}
	}
	return false;
}

