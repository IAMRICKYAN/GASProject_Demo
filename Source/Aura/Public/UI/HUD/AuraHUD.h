// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;
class UAuraUserWidget;
struct FWidgetControllerParams;
class USpellMenuWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:

	// 函数声明，用于获取不同类型的UI控制器实例，并初始化Overlay UI

	// 获取OverlayWidgetController的实例，需要传入FWidgetControllerParams参数
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	// 获取AttributeMenuWidgetController的实例，需要传入FWidgetControllerParams参数
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);

	// 获取SpellMenuWidgetController的实例，需要传入FWidgetControllerParams参数
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams);

	// 初始化Overlay UI，需要传入玩家控制器、玩家状态、能力系统组件和属性集组件
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);


protected:


private:

	// UObjectPtr和TSubclassOf属性声明，用于管理不同类型的UI组件和控制器

	// OverlayWidget指针，用于持有Overlay UI组件的实例
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;	

	// OverlayWidgetClass，用于指定Overlay UI组件的类，可在编辑器中设置
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	// OverlayWidgetController指针，用于持有Overlay UI控制器的实例
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	// OverlayWidgetControllerClass，用于指定Overlay UI控制器的类，可在编辑器中设置
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	// AttributeMenuWidgetController指针，用于持有属性菜单UI控制器的实例
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	// AttributeMenuWidgetControllerClass，用于指定属性菜单UI控制器的类，可在编辑器中设置
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	// SpellMenuWidgetController指针，用于持有法术菜单UI控制器的实例
	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;

	// SpellMenuWidgetControllerClass，用于指定法术菜单UI控制器的类，可在编辑器中设置
	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;

};
