// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:

	// 获取Aura资产管理器的静态实例
	static UAuraAssetManager& Get();

protected:

	// 重写基类的开始初始加载方法
	virtual void StartInitialLoading() override;

};
