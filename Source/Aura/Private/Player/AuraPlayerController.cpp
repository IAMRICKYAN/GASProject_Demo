// Copyright RickYan


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Aura/Aura.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/HighlightInterface.h"
#include "UI/Widget/DamageTextComponent.h"

// AAuraPlayerController的构造函数，用于初始化AAuraPlayerController对象
AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true; // 设置该控制器在多玩家游戏中可复制
	Spline = CreateDefaultSubobject<USplineComponent>("Spline"); // 创建并初始化一个名为"Spline"的USplineComponent组件
}

// 每帧调用的PlayerTick函数，用于处理玩家控制器的每帧逻辑
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime); // 调用父类的PlayerTick函数以保持默认行为
	CursorTrace(); // 执行光标追踪逻辑
	AutoRun(); // 处理自动奔跑逻辑
	UpdateMagicCircleLocation(); // 更新魔法圈的位置
}

// 显示魔法圈的函数，接受一个UMaterialInterface指针作为参数，用于设置魔法圈的材质
void AAuraPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle)) // 检查MagicCircle对象是否有效
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass); // 如果无效，则在世界中生成一个AMagicCircle对象
		if (DecalMaterial) // 如果传入了有效的材质
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial); // 设置魔法圈的材质
		}
	}
}


// 隐藏魔法圈的函数
void AAuraPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle)) // 检查MagicCircle对象是否有效
	{
		MagicCircle->Destroy(); // 如果有效，则销毁MagicCircle对象
	}
}

// 显示伤害数字的函数，用于在目标角色上方显示伤害数值
// 参数DamageAmount是伤害数值
// 参数TargetCharacter是指向受伤害角色的指针
// 参数bBlockedHit表示是否为格挡攻击
// 参数bCriticalHit表示是否为暴击
void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController()) // 检查目标角色、伤害文本组件类和是否为本地控制器
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass); // 创建伤害文本组件
		DamageText->RegisterComponent(); // 注册组件
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform); // 将组件附加到目标角色的根组件
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform); // 从组件分离，保持世界坐标变换
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit); // 设置伤害文本内容
	}
}

// 自动奔跑的函数，用于控制角色沿 spline 路径自动奔跑
void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return; // 如果未启用自动奔跑，则直接返回
	if (APawn* ControlledPawn = GetPawn()) // 获取控制的Pawn对象
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World); // 找到spline上最接近Pawn位置的点
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World); // 获取该点的方向
		ControlledPawn->AddMovementInput(Direction); // 向该方向添加移动输入

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length(); // 计算到目的地的距离
		if (DistanceToDestination <= AutoRunAcceptanceRadius) // 如果距离小于等于自动奔跑接受半径
		{
			bAutoRunning = false; // 停止自动奔跑
		}
	}
}


// 更新魔法圈位置的函数，将魔法圈移动到光标命中点
void AAuraPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle)) // 检查MagicCircle对象是否有效
	{
		MagicCircle->SetActorLocation(CursorHit.ImpactPoint); // 如果有效，则将MagicCircle的位置设置为光标命中点
	}
}

// 高亮显示指定演员的函数
// 参数InActor是指向要高亮的演员的指针
void AAuraPlayerController::HighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>()) // 检查InActor是否有效并且实现了UHighlightInterface接口
	{
		IHighlightInterface::Execute_HighlightActor(InActor); // 调用接口的高亮函数
	}
}

// 取消高亮显示指定演员的函数
// 参数InActor是指向要取消高亮的演员的指针
void AAuraPlayerController::UnHighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>()) // 检查InActor是否有效并且实现了UHighlightInterface接口
	{
		IHighlightInterface::Execute_UnHighlightActor(InActor); // 调用接口的取消高亮函数
	}
}


// 执行光标追踪的函数，用于检测光标下方的对象并相应地高亮或取消高亮
void AAuraPlayerController::CursorTrace()
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		// 如果玩家有阻止光标追踪的游戏标签，则取消高亮当前和最后一个演员，并重置它们
		UnHighlightActor(LastActor);
		UnHighlightActor(ThisActor);
		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}
	// 确定追踪通道，如果魔法圈有效，则排除玩家
	const ECollisionChannel TraceChannel = IsValid(MagicCircle) ? ECC_ExcludePlayers : ECC_Visibility;
	// 获取光标下的命中结果
	GetHitResultUnderCursor(TraceChannel, false, CursorHit);
	if (!CursorHit.bBlockingHit) return; // 如果没有阻塞命中，则退出

	// 更新最后一个和当前演员
	LastActor = ThisActor;
	if (IsValid(CursorHit.GetActor()) && CursorHit.GetActor()->Implements<UHighlightInterface>())
	{
		ThisActor = CursorHit.GetActor(); // 如果命中演员有效且实现高亮接口，则设置为当前演员
	}
	else
	{
		ThisActor = nullptr; // 否则，重置当前演员
	}

	// 如果最后一个和当前演员不同，则更新高亮状态
	if (LastActor != ThisActor)
	{
		UnHighlightActor(LastActor); // 取消高亮最后一个演员
		HighlightActor(ThisActor); // 高亮当前演员
	}
}

// 处理能力输入标签按下的函数
// 参数InputTag是按下的输入标签
void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		// 如果玩家有阻止输入的游戏标签，则直接返回
		return;
	}
	// 检查是否为左键按钮的输入标签
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (IsValid(ThisActor))
		{
			// 如果当前演员有效，则根据是否为敌人接口设置目标状态
			TargetingStatus = ThisActor->Implements<UEnemyInterface>() ? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNonEnemy;
		}
		else
		{
			TargetingStatus = ETargetingStatus::NotTargeting; // 如果当前演员无效，则设置为目标状态为未目标
		}
		bAutoRunning = false; // 取消自动奔跑
	}
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag); // 通知ASC输入标签被按下
}


// 处理能力输入标签释放的函数
// 参数InputTag是释放的输入标签
void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputReleased))
	{
		// 如果玩家有阻止输入释放的游戏标签，则直接返回
		return;
	}
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		// 如果输入标签不是左键，则通知ASC输入标签被释放
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}

	// 通知ASC输入标签被释放
	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);

	// 如果不是瞄准敌人且Shift键未按下
	if (TargetingStatus != ETargetingStatus::TargetingEnemy && !bShiftKeyDown)
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			// 如果点击时间短且控制了Pawn
			if (IsValid(ThisActor) && ThisActor->Implements<UHighlightInterface>())
			{
				// 如果当前演员有效且实现了高亮接口，则设置移动到位置
				IHighlightInterface::Execute_SetMoveToLocation(ThisActor, CachedDestination);
			}
			else if (GetASC() && !GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
			{
				// 否则，如果ASC存在且没有阻止输入按下的游戏标签，则生成点击效果
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
			}
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				// 如果找到了导航路径，则清除并设置spline点
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}
				if (NavPath->PathPoints.Num() > 0)
				{
					// 更新缓存目的地并开始自动奔跑
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					bAutoRunning = true;
				}
			}
		}
		// 重置跟随时间和目标状态
		FollowTime = 0.f;
		TargetingStatus = ETargetingStatus::NotTargeting;
	}
}

// 处理能力输入标签按住的函数
// 参数InputTag是按住的输入标签
void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputHeld))
	{
		// 如果玩家有阻止输入按住的游戏标签，则直接返回
		return;
	}
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		// 如果输入标签不是左键，则通知ASC输入标签被按住
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}

	// 如果正在瞄准敌人或Shift键被按下
	if (TargetingStatus == ETargetingStatus::TargetingEnemy || bShiftKeyDown)
	{
		// 通知ASC输入标签被按住
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		// 增加跟随时间
		FollowTime += GetWorld()->GetDeltaSeconds();
		if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;

		if (APawn* ControlledPawn = GetPawn())
		{
			// 计算世界方向并添加移动输入
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}


// 获取Aura能力系统组件的函数
// 返回指向UAuraAbilitySystemComponent的指针
UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		// 如果AuraAbilitySystemComponent为空，则尝试从控制的Pawn中获取能力系统组件
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

// 游戏开始时调用的函数，用于初始化玩家控制器
void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay(); // 调用基类的BeginPlay函数
	check(AuraContext); // 确保AuraContext已正确设置

	// 获取增强输入子系统并添加映射上下文
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	// 设置鼠标光标显示和默认样式
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// 配置输入模式，不锁定鼠标到视口，且在捕获时显示光标
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

// 设置输入组件的函数，用于绑定输入动作
void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent(); // 调用基类的SetupInputComponent函数

	// 强制转换输入组件为UAuraInputComponent并绑定动作
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move); // 绑定移动动作
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed); // 绑定Shift键按下动作
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased); // 绑定Shift键释放动作
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld); // 绑定能力输入标签的动作
}


void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// 如果技能系统组件存在并且玩家处于输入阻塞状态，则不处理移动
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}

	// 获取输入轴向量
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	// 获取控制器旋转
	const FRotator Rotation = GetControlRotation();
	// 仅保留偏航旋转
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	// 计算前进方向
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// 计算右方方向
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 如果控制了角色
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// 添加前进方向的移动输入
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		// 添加右方方向的移动输入
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

