// Copyright RickYan


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"

// AAuraCharacter的构造函数，用于初始化角色对象
AAuraCharacter::AAuraCharacter()
{
	// 创建并初始化摄像头吊臂组件，命名为"CameraBoom"
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	// 将摄像头吊臂组件附加到角色的根组件上
	CameraBoom->SetupAttachment(GetRootComponent());
	// 设置摄像头吊臂使用绝对旋转
	CameraBoom->SetUsingAbsoluteRotation(true);
	// 禁用摄像头吊臂的碰撞检测
	CameraBoom->bDoCollisionTest = false;

	// 创建并初始化顶视摄像头组件，命名为"TopDownCameraComponent"
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	// 将顶视摄像头组件附加到摄像头吊臂的指定插槽上
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// 禁用顶视摄像头的pawn控制旋转
	TopDownCameraComponent->bUsePawnControlRotation = false;
	
	// 创建并初始化升级特效的Niagara组件，命名为"LevelUpNiagaraComponent"
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	// 将升级特效组件附加到角色的根组件上
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	// 设置升级特效组件不自动激活
	LevelUpNiagaraComponent->bAutoActivate = false;
	
	// 获取角色移动组件并设置其属性
	GetCharacterMovement()->bOrientRotationToMovement = true; // 设置角色移动时朝向旋转
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f); // 设置角色的旋转速率
	GetCharacterMovement()->bConstrainToPlane = true; // 设置角色移动限制在平面上
	GetCharacterMovement()->bSnapToPlaneAtStart = true; // 设置角色在开始时对齐到平面

	// 禁用控制器旋转的影响
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// 设置角色的职业为元素使
	CharacterClass = ECharacterClass::Elementalist;
}


// 覆写PossessedBy函数，当角色被新控制器（NewController）占据时执行自定义逻辑
void AAuraCharacter::PossessedBy(AController* NewController)
{
	// 调用基类的PossessedBy函数，确保执行默认的占据逻辑
	Super::PossessedBy(NewController);

	// 在服务器上初始化能力系统的角色信息
	InitAbilityActorInfo();
	// 加载角色的保存进度，包括技能、属性等
	LoadProgress();

	// 检查当前游戏模式是否为AAuraGameModeBase类型
	if (AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		// 如果是，则调用LoadWorldState函数加载游戏世界的状态
		AuraGameMode->LoadWorldState(GetWorld());
	}
}


// LoadProgress函数，用于加载角色的保存进度
void AAuraCharacter::LoadProgress()
{
	// 尝试获取当前游戏模式为AAuraGameModeBase类型的实例
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (AuraGameMode)
	{
		// 从游戏模式中检索保存的游戏数据
		ULoadScreenSaveGame* SaveData = AuraGameMode->RetrieveInGameSaveData();
		// 如果没有保存数据，则直接返回
		if (SaveData == nullptr) return;

		// 检查是否是第一次加载
		if (SaveData->bFirstTimeLoadIn)
		{
			// 如果是第一次加载，初始化默认属性并添加角色能力
			InitializeDefaultAttributes();
			AddCharacterAbilities();
		}
		else
		{
			// 如果不是第一次加载，从保存数据中恢复角色状态
			if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
			{
				// 添加从保存数据中恢复的角色能力
				AuraASC->AddCharacterAbilitiesFromSaveData(SaveData);
			}
			
			// 尝试获取AAuraPlayerState类型的玩家状态
			if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
			{
				// 从保存数据中设置玩家的等级、经验值、属性点和技能点
				AuraPlayerState->SetLevel(SaveData->PlayerLevel);
				AuraPlayerState->SetXP(SaveData->XP);
				AuraPlayerState->SetAttributePoints(SaveData->AttributePoints);
				AuraPlayerState->SetSpellPoints(SaveData->SpellPoints);
			}
			
			// 使用保存数据初始化默认属性
			UAuraAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(this, AbilitySystemComponent, SaveData);
		}
	}
}


// OnRep_PlayerState函数，当玩家的PlayerState在多玩家环境中被复制时调用
void AAuraCharacter::OnRep_PlayerState()
{
	// 调用基类的OnRep_PlayerState函数以保持默认行为
	Super::OnRep_PlayerState();

	// 在客户端初始化能力系统的角色信息
	// 这确保了客户端和服务器之间的能力系统状态同步
	InitAbilityActorInfo();
}


// AddToXP_Implementation函数，实现添加经验值到玩家状态的具体逻辑
void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	// 获取AAuraPlayerState的实例，这是玩家状态的具体子类，用于管理经验值等数据
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	// 确保获取的玩家状态不为空，如果为空则触发断言错误
	check(AuraPlayerState);
	// 调用玩家状态的AddToXP函数，将输入的经验值（InXP）添加到玩家的总经验值中
	AuraPlayerState->AddToXP(InXP);
}


// 等级提升的具体实现函数
void AAuraCharacter::LevelUp_Implementation()
{
	// 在所有客户端上播放等级提升的粒子效果
	MulticastLevelUpParticles();
}


// 多播等级提升粒子效果的实现函数，用于在所有客户端上播放粒子效果
void AAuraCharacter::MulticastLevelUpParticles_Implementation() const
{
	// 检查等级提升的Niagara粒子组件是否有效
	if (IsValid(LevelUpNiagaraComponent))
	{
		// 获取顶部下视摄像机的位置
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		// 获取Niagara粒子系统的位置
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		// 计算从粒子系统位置指向摄像机位置的旋转
		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
		// 设置Niagara粒子系统的世界旋转，使其面向摄像机
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		// 激活Niagara粒子系统，播放等级提升的粒子效果
		LevelUpNiagaraComponent->Activate(true);
	}
}


// 获取玩家当前经验值的实现函数
int32 AAuraCharacter::GetXP_Implementation() const
{
	// 获取AAuraPlayerState的实例，这是管理玩家经验的玩家状态子类
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	// 确保获取的玩家状态不为空
	check(AuraPlayerState);
	// 返回玩家状态中存储的经验值
	return AuraPlayerState->GetXP();
}

// 根据输入的经验值查找对应等级的实现函数
int32 AAuraCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	// 获取AAuraPlayerState的实例
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	// 确保获取的玩家状态不为空
	check(AuraPlayerState);
	// 使用玩家状态中的LevelUpInfo来查找对应经验值的等级
	return AuraPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

// 获取指定等级提升时奖励的属性点数的实现函数
int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	// 获取AAuraPlayerState的实例
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	// 确保获取的玩家状态不为空
	check(AuraPlayerState);
	// 返回指定等级提升时奖励的属性点数
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

// 获取指定等级提升时奖励的技能点数的实现函数
int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	// 获取AAuraPlayerState的实例
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	// 确保获取的玩家状态不为空
	check(AuraPlayerState);
	// 返回指定等级提升时奖励的技能点数
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
}


// 添加玩家等级的实现函数
void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	// 获取AAuraPlayerState的实例，用于管理玩家等级
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	// 确保获取的玩家状态不为空
	check(AuraPlayerState);
	// 向玩家状态添加等级
	AuraPlayerState->AddToLevel(InPlayerLevel);

	// 如果获取的AbilitySystemComponent是UAuraAbilitySystemComponent的实例
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		// 更新能力状态，基于新的玩家等级
		AuraASC->UpdateAbilityStatuses(AuraPlayerState->GetPlayerLevel());
	}
}

// 添加属性点数的实现函数
void AAuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	// 获取AAuraPlayerState的实例
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	// 确保获取的玩家状态不为空
	check(AuraPlayerState);
	// 向玩家状态添加属性点数
	AuraPlayerState->AddToAttributePoints(InAttributePoints);
}

// 添加技能点数的实现函数
void AAuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	// 获取AAuraPlayerState的实例
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	// 确保获取的玩家状态不为空
	check(AuraPlayerState);
	// 向玩家状态添加技能点数
	AuraPlayerState->AddToSpellPoints(InSpellPoints);
}

// 获取当前属性点数的实现函数
int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	// 获取AAuraPlayerState的实例
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	// 确保获取的玩家状态不为空
	check(AuraPlayerState);
	// 返回当前属性点数
	return AuraPlayerState->GetAttributePoints();
}

// 获取当前技能点数的实现函数
int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	// 获取AAuraPlayerState的实例
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	// 确保获取的玩家状态不为空
	check(AuraPlayerState);
	// 返回当前技能点数
	return AuraPlayerState->GetSpellPoints();
}


// 显示魔法圈的实现函数，用于在玩家角色下方显示一个指定的贴花材质
void AAuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	// 检查当前控制器是否为AAuraPlayerController的实例
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		// 调用控制器的方法显示魔法圈，并传递贴花材质
		AuraPlayerController->ShowMagicCircle(DecalMaterial);
		// 隐藏鼠标指针，因为魔法圈可能用于指示交互，不需要鼠标指针
		AuraPlayerController->bShowMouseCursor = false;
	}
}

// 隐藏魔法圈的实现函数，用于移除玩家角色下方的贴花材质
void AAuraCharacter::HideMagicCircle_Implementation()
{
	// 检查当前控制器是否为AAuraPlayerController的实例
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		// 调用控制器的方法隐藏魔法圈
		AuraPlayerController->HideMagicCircle();
		// 显示鼠标指针，因为魔法圈不再需要，可能需要鼠标进行其他操作
		AuraPlayerController->bShowMouseCursor = true;
	}
}


// 保存玩家进度的实现函数，根据提供的检查点标签保存当前游戏状态
void AAuraCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	// 尝试获取当前游戏模式为AAuraGameModeBase类型的实例
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	// 如果获取成功，则继续保存进度
	if (AuraGameMode)
	{
		// 从游戏模式中检索当前游戏的保存数据
		ULoadScreenSaveGame* SaveData = AuraGameMode->RetrieveInGameSaveData();
		// 如果保存数据为空，则直接返回，不进行保存
		if (SaveData == nullptr) return;

		// 设置玩家起始位置的标签为提供的检查点标签
		SaveData->PlayerStartTag = CheckpointTag;

		// 尝试获取当前玩家状态为AAuraPlayerState类型的实例
		if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
		{
			// 保存玩家等级、经验值、属性点和技能点
			SaveData->PlayerLevel = AuraPlayerState->GetPlayerLevel();
			SaveData->XP = AuraPlayerState->GetXP();
			SaveData->AttributePoints = AuraPlayerState->GetAttributePoints();
			SaveData->SpellPoints = AuraPlayerState->GetSpellPoints();
		}

		// 保存玩家的基础属性值
		SaveData->Strength = UAuraAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Intelligence = UAuraAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Resilience = UAuraAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Vigor = UAuraAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());

		// 标记这不是第一次加载游戏
		SaveData->bFirstTimeLoadIn = false;

		// 如果当前角色没有权限（不是服务器端），则不保存能力数据
		if (!HasAuthority()) return;

		// 获取能力系统组件为UAuraAbilitySystemComponent的实例
		UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
		// 创建一个委托，用于遍历并保存每个能力
		FForEachAbility SaveAbilityDelegate;
		// 清空之前保存的能力数据
		SaveData->SavedAbilities.Empty();
		// 绑定委托到一个lambda表达式，用于处理每个能力的保存
		SaveAbilityDelegate.BindLambda([this, AuraASC, SaveData](const FGameplayAbilitySpec& AbilitySpec)
		{
			// 获取能力的标签
			const FGameplayTag AbilityTag = AuraASC->GetAbilityTagFromSpec(AbilitySpec);
			// 获取能力信息
			UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(this);
			// 根据标签找到具体的能力信息
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);

			// 创建一个保存能力的数据结构
			FSavedAbility SavedAbility;
			// 设置保存能力的属性
			SavedAbility.GameplayAbility = Info.Ability;
			SavedAbility.AbilityLevel = AbilitySpec.Level;
			SavedAbility.AbilitySlot = AuraASC->GetSlotFromAbilityTag(AbilityTag);
			SavedAbility.AbilityStatus = AuraASC->GetStatusFromAbilityTag(AbilityTag);
			SavedAbility.AbilityTag = AbilityTag;
			SavedAbility.AbilityType = Info.AbilityType;

			// 将保存的能力添加到保存数据中，确保不重复
			SaveData->SavedAbilities.AddUnique(SavedAbility);
		});
		// 遍历所有能力，并执行保存委托
		AuraASC->ForEachAbility(SaveAbilityDelegate);
		
		// 最终，调用游戏模式的方法来保存游戏进度数据
		AuraGameMode->SaveInGameProgressData(SaveData);
	}
}


// 获取玩家等级的实现函数
int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	// 获取当前玩家的状态为AAuraPlayerState类型的实例
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	// 确保获取的玩家状态不为空，以避免空指针异常
	check(AuraPlayerState);
	// 返回玩家状态的当前等级
	return AuraPlayerState->GetPlayerLevel();
}

// 角色死亡函数，处理角色死亡时的逻辑
void AAuraCharacter::Die(const FVector& DeathImpulse)
{
	// 调用基类的Die函数，处理通用的死亡逻辑
	Super::Die(DeathImpulse);

	// 创建一个定时器委托，用于延迟执行角色死亡后的特定逻辑
	FTimerDelegate DeathTimerDelegate;
	DeathTimerDelegate.BindLambda([this]() // 使用Lambda表达式绑定要执行的操作
	{
		// 尝试获取当前游戏模式为AAuraGameModeBase类型的实例
		AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
		if (AuraGM)
		{
			// 如果获取成功，通知游戏模式玩家已死亡
			AuraGM->PlayerDied(this);
		}
	});
	// 设置定时器，延迟执行DeathTimerDelegate中的逻辑
	GetWorldTimerManager().SetTimer(DeathTimer, DeathTimerDelegate, DeathTime, false);

	// 将顶视摄像头组件从角色上分离，保持世界坐标变换
	TopDownCameraComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}


// 当角色被击晕的状态在服务器和客户端之间同步时调用的函数
void AAuraCharacter::OnRep_Stunned()
{
	// 尝试获取UAuraAbilitySystemComponent的实例
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		// 获取游戏玩法标签的实例
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		// 创建一个标签容器，用于存储被阻塞的标签
		FGameplayTagContainer BlockedTags;
		// 添加需要阻塞的标签
		BlockedTags.AddTag(GameplayTags.Player_Block_CursorTrace);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputHeld);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputPressed);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputReleased);
		// 根据角色是否被击晕来添加或移除阻塞标签，并激活或停用击晕减益组件
		if (bIsStunned)
		{
			AuraASC->AddLooseGameplayTags(BlockedTags); // 添加阻塞标签
			StunDebuffComponent->Activate(); // 激活击晕减益组件
		}
		else
		{
			AuraASC->RemoveLooseGameplayTags(BlockedTags); // 移除阻塞标签
			StunDebuffComponent->Deactivate(); // 停用击晕减益组件
		}
	}
}

// 当角色被燃烧的状态在服务器和客户端之间同步时调用的函数
void AAuraCharacter::OnRep_Burned()
{
	// 根据角色是否被燃烧来激活或停用燃烧减益组件
	if (bIsBurned)
	{
		BurnDebuffComponent->Activate(); // 激活燃烧减益组件
	}
	else
	{
		BurnDebuffComponent->Deactivate(); // 停用燃烧减益组件
	}
}


// 初始化能力系统组件的演员信息，并设置相关的能力和属性
void AAuraCharacter::InitAbilityActorInfo()
{
	// 获取AAuraPlayerState的实例
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	// 确保获取的玩家状态不为空
	check(AuraPlayerState);
	// 初始化能力系统组件的演员信息，将玩家状态和角色本身作为参数
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	// 通知能力系统组件演员信息已设置
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	// 设置角色的能力系统组件为玩家状态的能力系统组件
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	// 设置角色的属性集为玩家状态的属性集
	AttributeSet = AuraPlayerState->GetAttributeSet();
	// 广播能力系统组件已注册的事件
	OnAscRegistered.Broadcast(AbilitySystemComponent);
	// 注册游戏玩法标签事件，当击晕减益标签新增或移除时调用StunTagChanged函数
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraCharacter::StunTagChanged);

	// 尝试获取AAuraPlayerController的实例
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		// 尝试获取AAuraHUD的实例
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			// 初始化HUD覆盖层，传递玩家控制器、玩家状态、能力系统组件和属性集
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}
