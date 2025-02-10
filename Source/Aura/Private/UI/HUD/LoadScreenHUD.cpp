// Copyright RickYan


#include "UI/HUD/LoadScreenHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "UI/Widget/LoadScreenWidget.h"

// ALoadScreenHUD类的BeginPlay函数，用于初始化加载屏幕
void ALoadScreenHUD::BeginPlay()
{
	// 调用父类的BeginPlay函数
	Super::BeginPlay();

	// 创建加载屏幕视图模型实例，并初始化加载槽
	LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this, LoadScreenViewModelClass);
	LoadScreenViewModel->InitializeLoadSlots();

	// 创建加载屏幕小部件实例，并将其添加到视口
	LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
	LoadScreenWidget->AddToViewport();

	// 蓝图初始化小部件，可能包括绑定视图模型等操作
	LoadScreenWidget->BlueprintInitializeWidget();

	// 加载视图模型的数据，可能包括从外部源加载数据等
	LoadScreenViewModel->LoadData();
}

