// Copyright RickYan


#include "Actor/PointCollection.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;

	Pt_0 = CreateDefaultSubobject<USceneComponent>("Pt_0");
	ImmutablePts.Add(Pt_0);
	SetRootComponent(Pt_0);

	Pt_1 = CreateDefaultSubobject<USceneComponent>("Pt_1");
	ImmutablePts.Add(Pt_1);
	Pt_1->SetupAttachment(GetRootComponent());

	Pt_2 = CreateDefaultSubobject<USceneComponent>("Pt_2");
	ImmutablePts.Add(Pt_2);
	Pt_2->SetupAttachment(GetRootComponent());

	Pt_3 = CreateDefaultSubobject<USceneComponent>("Pt_3");
	ImmutablePts.Add(Pt_3);
	Pt_3->SetupAttachment(GetRootComponent());
	
	Pt_4 = CreateDefaultSubobject<USceneComponent>("Pt_4");
	ImmutablePts.Add(Pt_4);
	Pt_4->SetupAttachment(GetRootComponent());

	Pt_5 = CreateDefaultSubobject<USceneComponent>("Pt_5");
	ImmutablePts.Add(Pt_5);
	Pt_5->SetupAttachment(GetRootComponent());
	
	Pt_6 = CreateDefaultSubobject<USceneComponent>("Pt_6");
	ImmutablePts.Add(Pt_6);
	Pt_6->SetupAttachment(GetRootComponent());

	Pt_7 = CreateDefaultSubobject<USceneComponent>("Pt_7");
	ImmutablePts.Add(Pt_7);
	Pt_7->SetupAttachment(GetRootComponent());

	Pt_8 = CreateDefaultSubobject<USceneComponent>("Pt_8");
	ImmutablePts.Add(Pt_8);
	Pt_8->SetupAttachment(GetRootComponent());

	Pt_9 = CreateDefaultSubobject<USceneComponent>("Pt_9");
	ImmutablePts.Add(Pt_9);
	Pt_9->SetupAttachment(GetRootComponent());

	Pt_10 = CreateDefaultSubobject<USceneComponent>("Pt_10");
	ImmutablePts.Add(Pt_10);
	Pt_10->SetupAttachment(GetRootComponent());

	
}

// APointCollection类的成员函数，用于获取地面点数组
TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride)
{
	// 检查ImmutablePts数组中的元素数量是否足够，否则输出错误信息
	checkf(ImmutablePts.Num() >= NumPoints, TEXT("Attempted to access ImmutablePts out of bounds."));

	// 创建一个数组副本，用于存储最终的结果
	TArray<USceneComponent*> ArrayCopy;

	// 遍历ImmutablePts数组中的每个点
	for (USceneComponent* Pt : ImmutablePts)
	{
		// 如果已经收集了足够的点，返回数组副本
		if (ArrayCopy.Num() >= NumPoints) return ArrayCopy;

		// 如果当前点不是Pt_0
		if (Pt != Pt_0)
		{
			// 计算从Pt_0到当前点的向量
			FVector ToPoint = Pt->GetComponentLocation() - Pt_0->GetComponentLocation();
			// 根据YawOverride旋转向量
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			// 设置当前点的世界位置
			Pt->SetWorldLocation(Pt_0->GetComponentLocation() + ToPoint);
		}

		// 计算提升后的位置（在Z轴上增加500单位）
		const FVector RaisedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f);
		// 计算降低后的位置（在Z轴上减少500单位）
		const FVector LoweredLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f);

		// 用于存储射线追踪结果的变量
		FHitResult HitResult;
		// 创建一个数组，用于存储需要忽略的actor
		TArray<AActor*> IgnoreActors;
		// 获取半径1500单位内的所有活着的玩家，并将它们添加到忽略列表中
		UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(this, IgnoreActors, TArray<AActor*>(), 1500.f, GetActorLocation());

		// 设置碰撞查询参数，添加忽略的actor
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		// 执行射线追踪，从提升后的位置到降低后的位置
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName("BlockAll"), QueryParams);

		// 根据射线追踪的结果调整位置，使点的Z坐标与撞击点对齐
		const FVector AdjustedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		Pt->SetWorldLocation(AdjustedLocation);
		// 设置点的世界旋转，使其朝向撞击面的法线
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

		// 将当前点添加到数组副本中
		ArrayCopy.Add(Pt);
	}
	// 返回包含所有调整后点的数组副本
	return ArrayCopy;
}


void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}

