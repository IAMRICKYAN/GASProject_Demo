// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameplayTagContainer.h"
#include "PassiveNiagaraComponent.generated.h"

class UAuraAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class AURA_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	UPassiveNiagaraComponent();

	// 声明一个可编辑的默认属性，用于存储被动技能的Tag
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveSpellTag;

protected:
	// 重写BeginPlay函数，用于在游戏开始时执行逻辑
	virtual void BeginPlay() override;

	// 当被动技能激活或取消激活时调用的函数
	void OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate);

	// 检查并激活已装备的被动技能
	void ActivateIfEquipped(UAuraAbilitySystemComponent* AuraASC);
};
