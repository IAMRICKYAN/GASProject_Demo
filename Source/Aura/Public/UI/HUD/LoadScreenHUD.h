// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoadScreenHUD.generated.h"

class UMVVM_LoadScreen;
class ULoadScreenWidget;
/**
 * 
 */
UCLASS()
class AURA_API ALoadScreenHUD : public AHUD
{
	GENERATED_BODY()
public:

	// 加载屏幕小部件类，可在默认编辑器中指定
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadScreenWidgetClass;

	// 加载屏幕小部件实例，蓝图只读，用于显示加载界面
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULoadScreenWidget> LoadScreenWidget;

	// 加载屏幕视图模型类，可在默认编辑器中指定，用于数据绑定和逻辑处理
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadScreen> LoadScreenViewModelClass;

	// 加载屏幕视图模型实例，蓝图只读，用于与加载屏幕小部件交互
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_LoadScreen> LoadScreenViewModel;

protected:
	virtual void BeginPlay() override;
};
