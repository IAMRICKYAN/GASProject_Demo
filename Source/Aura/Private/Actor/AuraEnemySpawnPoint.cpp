// Copyright RickYan


#include "Actor/AuraEnemySpawnPoint.h"

#include "Character/AuraEnemy.h"

// AAuraEnemySpawnPoint类中的函数，用于生成敌人
void AAuraEnemySpawnPoint::SpawnEnemy()
{
	// 设置生成参数
	FActorSpawnParameters SpawnParams;
	// 指定生成时的碰撞处理方式，如果可能则调整位置，但总是生成
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 延迟生成敌人，允许在生成前设置属性
	AAuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(EnemyClass, GetActorTransform());
	// 设置敌人的等级
	Enemy->SetLevel(EnemyLevel);
	// 设置敌人的角色类
	Enemy->SetCharacterClass(CharacterClass);
	// 完成生成过程
	Enemy->FinishSpawning(GetActorTransform());
	// 生成默认控制器，用于控制敌人
	Enemy->SpawnDefaultController();
}

