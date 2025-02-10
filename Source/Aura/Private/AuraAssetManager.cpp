// Copyright RickYan


#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "AuraGameplayTags.h"

// 获取UAuraAssetManager的单例实例
UAuraAssetManager& UAuraAssetManager::Get()
{
	// 确保引擎已经初始化
	check(GEngine);
	
	// 尝试将引擎的资产管理器转换为UAuraAssetManager类型
	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	// 返回UAuraAssetManager的引用
	return *AuraAssetManager;
}

// 开始初始加载过程
void UAuraAssetManager::StartInitialLoading()
{
	// 调用基类的StartInitialLoading方法以执行任何基本的初始加载
	Super::StartInitialLoading();
	// 初始化本地的游戏玩法标签
	FAuraGameplayTags::InitializeNativeGameplayTags();

	// 初始化全局能力系统数据，这是使用目标数据所必需的
	UAbilitySystemGlobals::Get().InitGlobalData();
}

