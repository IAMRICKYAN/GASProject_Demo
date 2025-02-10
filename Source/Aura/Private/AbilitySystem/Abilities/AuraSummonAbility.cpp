// Copyright RickYan


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

// 获取生成位置的数组，根据召唤能力计算每个仆从的生成位置
TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	// 获取角色前方向量
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	// 获取角色位置
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	// 计算每个仆从之间的角度差
	const float DeltaSpread = SpawnSpread / NumMinions;

	// 计算生成范围的最左边的向量
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	// 初始化生成位置数组
	TArray<FVector> SpawnLocations;
	// 循环计算每个仆从的生成位置
	for (int32 i = 0; i < NumMinions; i++)
	{
		// 计算当前仆从的生成方向
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		// 计算随机生成位置
		FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		// 进行视线追踪，检查生成位置是否被阻挡
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 400.f), ChosenSpawnLocation - FVector(0.f, 0.f, 400.f), ECC_Visibility);
		// 如果生成位置被阻挡，则使用阻挡点的位置
		if (Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint;
		}
		// 添加生成位置到数组
		SpawnLocations.Add(ChosenSpawnLocation);
	}
	
	// 返回生成位置数组
	return SpawnLocations;
}

// 随机获取一个仆从类
TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	// 随机选择一个索引
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	// 返回随机选择的仆从类
	return MinionClasses[Selection];
}

