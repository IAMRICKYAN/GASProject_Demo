// Copyright RickYan


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Aura/Aura.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AuraGameplayTags.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// AAuraEnemy类的构造函数，用于初始化敌人角色
AAuraEnemy::AAuraEnemy()
{
	// 设置网格体的可见性碰撞响应为阻挡
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	// 创建并设置能力系统组件，并设置为可复制
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// 设置不使用控制器旋转
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	// 设置使用控制器期望旋转
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	// 创建并设置属性集
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	// 创建并设置健康条小部件，并附加到根组件
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	// 设置网格体和武器的自定义深度模板值，并标记渲染状态为脏
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	GetMesh()->MarkRenderStateDirty();
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->MarkRenderStateDirty();
	
	// 设置基础步行速度
	BaseWalkSpeed = 250.f;
}

// 当敌人被控制器占据时调用
void AAuraEnemy::PossessedBy(AController* NewController)
{
	// 调用基类实现
	Super::PossessedBy(NewController);

	// 如果不是权威客户端，则返回
	if (!HasAuthority()) return;
	// 尝试将新控制器转换为AuraAIController
	AuraAIController = Cast<AAuraAIController>(NewController);
	// 初始化黑板并运行行为树
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuraAIController->RunBehaviorTree(BehaviorTree);
	// 设置黑板值
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

// 高亮显示敌人角色
void AAuraEnemy::HighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
}

// 取消高亮显示敌人角色
void AAuraEnemy::UnHighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

// 设置移动到位置（实现）
void AAuraEnemy::SetMoveToLocation_Implementation(FVector& OutDestination)
{
	// 不改变OutDestination
}

// 获取玩家等级（实现）
int32 AAuraEnemy::GetPlayerLevel_Implementation()
{
	return Level;
}

// 敌人角色死亡
void AAuraEnemy::Die(const FVector& DeathImpulse)
{
	// 设置生命期限
	SetLifeSpan(LifeSpan);
	// 如果有AuraAIController，设置黑板上的"Dead"值为true
	if (AuraAIController) AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	// 生成掉落物品
	SpawnLoot();
	// 调用基类实现
	Super::Die(DeathImpulse);
}

// 设置战斗目标（实现）
void AAuraEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

// 获取战斗目标（实现）
AActor* AAuraEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

// BeginPlay函数，在游戏开始时调用，用于初始化敌人角色
void AAuraEnemy::BeginPlay()
{
	// 调用基类的BeginPlay函数
	Super::BeginPlay();
	// 设置角色的最大步行速度
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	// 初始化能力系统组件的演员信息
	InitAbilityActorInfo();
	// 如果当前是权威客户端（服务器），则给予敌人启动能力
	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);	
	}

	// 如果健康条小部件是UAuraUserWidget，则设置小部件的控制器为当前敌人角色
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}
	
	// 如果属性集是UAuraAttributeSet，则设置健康值和最大健康值变化的回调
	if (const UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				// 广播健康值变化
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				// 广播最大健康值变化
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		
		// 注册击中反应游戏标签事件，并在标签添加或移除时调用HitReactTagChanged函数
		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AAuraEnemy::HitReactTagChanged
		);

		// 广播初始健康值和最大健康值
		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	}
	
}

// 当击中反应游戏标签变化时调用
void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	// 设置击中反应状态
	bHitReacting = NewCount > 0;
	// 如果处于击中反应状态，则停止移动，否则恢复基础步行速度
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	// 如果有AuraAIController，则更新黑板上的击中反应状态
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

// 初始化能力系统组件的演员信息
void AAuraEnemy::InitAbilityActorInfo()
{
	// 初始化能力系统组件的演员信息
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	// 调用UAuraAbilitySystemComponent的AbilityActorInfoSet函数
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	// 注册击晕减益游戏标签事件，并在标签添加或移除时调用StunTagChanged函数
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraEnemy::StunTagChanged);

	// 如果当前是权威客户端，则初始化默认属性
	if (HasAuthority())
	{
		InitializeDefaultAttributes();		
	}
	// 广播能力系统组件已注册事件
	OnAscRegistered.Broadcast(AbilitySystemComponent);
}

// 初始化默认属性
void AAuraEnemy::InitializeDefaultAttributes() const
{
	// 调用UAuraAbilitySystemLibrary的InitializeDefaultAttributes函数初始化属性
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

// 当击晕游戏标签变化时调用的函数
void AAuraEnemy::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	// 调用基类的StunTagChanged函数，以处理任何基类特定的逻辑
	Super::StunTagChanged(CallbackTag, NewCount);
	
	// 检查是否有一个AuraAIController实例，并且该控制器有一个黑板组件
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		// 根据新计数判断是否处于击晕状态，并更新黑板上的"Stunned"键的值
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Stunned"), bIsStunned);
	}
}
