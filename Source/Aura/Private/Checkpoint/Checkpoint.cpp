// Copyright RickYan


#include "Checkpoint/Checkpoint.h"

#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

// ACheckpoint类的构造函数，使用FObjectInitializer来初始化对象
ACheckpoint::ACheckpoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) // 调用基类的构造函数
{
	PrimaryActorTick.bCanEverTick = false; // 设置该演员不参与每帧的Tick更新

	// 创建默认的静态网格组件，用于表示检查点的视觉效果
	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckpointMesh");
	CheckpointMesh->SetupAttachment(GetRootComponent()); // 将网格组件附加到根组件上
	CheckpointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 设置碰撞为查询和物理碰撞
	CheckpointMesh->SetCollisionResponseToAllChannels(ECR_Block); // 对所有碰撞通道设置响应为阻塞

	// 设置自定义深度模板值，用于渲染效果
	CheckpointMesh->SetCustomDepthStencilValue(CustomDepthStencilOverride);
	CheckpointMesh->MarkRenderStateDirty(); // 标记渲染状态为脏，以更新渲染

	// 创建默认的球体组件，用于检测重叠事件
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(CheckpointMesh); // 将球体组件附加到检查点网格上
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 设置碰撞为仅查询
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore); // 对所有碰撞通道设置响应为忽略
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // 对Pawn通道设置响应为重叠

	// 创建默认的场景组件，用于设置移动到位置的逻辑
	MoveToComponent = CreateDefaultSubobject<USceneComponent>("MoveToComponent");
	MoveToComponent->SetupAttachment(GetRootComponent()); // 将场景组件附加到根组件上
}

// LoadActor_Implementation函数的实现，用于加载演员
void ACheckpoint::LoadActor_Implementation()
{
	if (bReached) // 如果检查点已被到达
	{
		HandleGlowEffects(); // 处理发光效果
	}
}


// 当球体组件与另一个组件开始重叠时调用的回调函数
void ACheckpoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>()) // 检查重叠的演员是否实现了UPlayerInterface接口
	{
		bReached = true; // 设置检查点为已到达状态

		// 尝试获取当前游戏模式，并转换为AAuraGameModeBase类型
		if (AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			const UWorld* World = GetWorld(); // 获取当前世界对象
			FString MapName = World->GetMapName(); // 获取地图名称
			MapName.RemoveFromStart(World->StreamingLevelsPrefix); // 移除地图名称前的前缀

			AuraGM->SaveWorldState(GetWorld(), MapName); // 调用游戏模式的SaveWorldState函数保存世界状态
		}
		
		// 调用重叠演员的SaveProgress函数，传递PlayerStartTag作为参数
		IPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);
		HandleGlowEffects(); // 处理检查点的发光效果
	}
}

// 在游戏开始时调用的函数，用于初始化检查点
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay(); // 调用基类的BeginPlay函数

	if (bBindOverlapCallback) // 如果设置了绑定重叠回调
	{
		// 将OnSphereOverlap函数动态绑定到球体组件的开始重叠事件
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnSphereOverlap);
	}
}


// 设置移动到位置的实现函数，用于获取检查点移动组件的位置
void ACheckpoint::SetMoveToLocation_Implementation(FVector& OutDestination)
{
	OutDestination = MoveToComponent->GetComponentLocation(); // 将移动组件的位置赋值给输出参数
}

// 高亮显示演员的实现函数，用于在检查点未被到达时设置自定义深度渲染
void ACheckpoint::HighlightActor_Implementation()
{
	if (!bReached) // 如果检查点未被到达
	{
		CheckpointMesh->SetRenderCustomDepth(true); // 启用自定义深度渲染，通常用于高亮显示
	}
}

// 取消高亮显示演员的实现函数，用于禁用自定义深度渲染
void ACheckpoint::UnHighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(false); // 禁用自定义深度渲染，取消高亮显示
}

// 处理发光效果的函数，用于在检查点被到达时更新材质和碰撞设置
void ACheckpoint::HandleGlowEffects()
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 禁用球体组件的碰撞，防止进一步的重叠事件
	UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CheckpointMesh->GetMaterial(0), this); // 创建动态材质实例
	CheckpointMesh->SetMaterial(0, DynamicMaterialInstance); // 将动态材质实例应用到检查点网格组件
	CheckpointReached(DynamicMaterialInstance); // 广播检查点到达事件，并传递动态材质实例以供蓝图进一步处理
}

