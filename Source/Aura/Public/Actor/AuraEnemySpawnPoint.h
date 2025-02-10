// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Engine/TargetPoint.h"
#include "AuraEnemySpawnPoint.generated.h"

class AAuraEnemy;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
public:

	// 允许蓝图调用的函数，用于生成敌人
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

	// 可在任何地方编辑的属性，用于指定敌人的类，可以在蓝图中读取
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	TSubclassOf<AAuraEnemy> EnemyClass;

	// 可在任何地方编辑的属性，用于指定敌人的等级，默认为1，可以在蓝图中读取
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	int32 EnemyLevel = 1;

	// 可在任何地方编辑的属性，用于指定敌人的角色类，默认为战士，可以在蓝图中读取
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

};
