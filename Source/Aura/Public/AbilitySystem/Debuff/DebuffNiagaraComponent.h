// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameplayTagContainer.h"
#include "DebuffNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	UDebuffNiagaraComponent();

	// 声明一个属性，该属性在编辑器中始终可见，但不能被修改
	// 用于存储减益效果的标签
	UPROPERTY(VisibleAnywhere)
	FGameplayTag DebuffTag;

protected:
	virtual void BeginPlay() override;
	
	// 函数声明：当减益标签发生变化时调用
	// 参数 CallbackTag：变化的标签
	// 参数 NewCount：标签的新计数
	void DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

};
