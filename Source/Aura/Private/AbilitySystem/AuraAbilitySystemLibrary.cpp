// Copyright RickYan


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "Game/AuraGameModeBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

// 函数：MakeWidgetControllerParams
// 功能：根据传入的世界上下文对象，填充FWidgetControllerParams结构体，并获取AAuraHUD对象。
// 参数：
//   - WorldContextObject: 世界上下文对象，用于获取玩家控制器。
//   - OutWCParams: 输出的FWidgetControllerParams结构体，包含玩家控制器、玩家状态、能力系统组件和属性集。
//   - OutAuraHUD: 输出的AAuraHUD对象。
// 返回值：如果成功获取并填充了参数，返回true；否则返回false。
bool UAuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHUD)
{
    // 获取玩家控制器
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    {
        // 获取AAuraHUD对象
        OutAuraHUD = Cast<AAuraHUD>(PC->GetHUD());
        if (OutAuraHUD)
        {
            // 获取玩家状态
            AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
            // 获取能力系统组件
            UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
            // 获取属性集
            UAttributeSet* AS = PS->GetAttributeSet();

            // 填充FWidgetControllerParams结构体
            OutWCParams.AttributeSet = AS;
            OutWCParams.AbilitySystemComponent = ASC;
            OutWCParams.PlayerState = PS;
            OutWCParams.PlayerController = PC;
            return true;
        }
    }
    return false;
}

// 函数：GetOverlayWidgetController
// 功能：获取Overlay Widget Controller。
// 参数：
//   - WorldContextObject: 世界上下文对象，用于获取玩家控制器。
// 返回值：如果成功获取Overlay Widget Controller，返回该对象；否则返回nullptr。
UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
    FWidgetControllerParams WCParams;
    AAuraHUD* AuraHUD = nullptr;
    // 调用MakeWidgetControllerParams填充参数
    if (MakeWidgetControllerParams(WorldContextObject, WCParams, AuraHUD))
    {
        // 返回Overlay Widget Controller
        return AuraHUD->GetOverlayWidgetController(WCParams);
    }
    return nullptr;
}

// 函数：GetAttributeMenuWidgetController
// 功能：获取Attribute Menu Widget Controller。
// 参数：
//   - WorldContextObject: 世界上下文对象，用于获取玩家控制器。
// 返回值：如果成功获取Attribute Menu Widget Controller，返回该对象；否则返回nullptr。
UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
    FWidgetControllerParams WCParams;
    AAuraHUD* AuraHUD = nullptr;
    // 调用MakeWidgetControllerParams填充参数
    if (MakeWidgetControllerParams(WorldContextObject, WCParams, AuraHUD))
    {
        // 返回Attribute Menu Widget Controller
        return AuraHUD->GetAttributeMenuWidgetController(WCParams);
    }
    return nullptr;
}

// 函数：GetSpellMenuWidgetController
// 功能：获取Spell Menu Widget Controller。
// 参数：
//   - WorldContextObject: 世界上下文对象，用于获取玩家控制器。
// 返回值：如果成功获取Spell Menu Widget Controller，返回该对象；否则返回nullptr。
USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
    FWidgetControllerParams WCParams;
    AAuraHUD* AuraHUD = nullptr;
    // 调用MakeWidgetControllerParams填充参数
    if (MakeWidgetControllerParams(WorldContextObject, WCParams, AuraHUD))
    {
        // 返回Spell Menu Widget Controller
        return AuraHUD->GetSpellMenuWidgetController(WCParams);
    }
    return nullptr;
}

// 函数：InitializeDefaultAttributes
// 功能：根据角色类别和等级，初始化默认属性。
// 参数：
//   - WorldContextObject: 世界上下文对象，用于获取角色类别信息。
//   - CharacterClass: 角色类别枚举，用于确定初始化的属性类型。
//   - Level: 角色等级，用于调整属性值。
//   - ASC: 能力系统组件，用于应用属性效果。
void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
    // 获取Avatar Actor
    AActor* AvatarActor = ASC->GetAvatarActor();

    // 获取角色类别信息
    UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
    // 获取角色类别的默认信息
    FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

    // 创建主要属性的Gameplay Effect上下文
    FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
    PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
    // 创建主要属性的Gameplay Effect Spec
    const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
    // 应用主要属性的Gameplay Effect
    ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

    // 创建次要属性的Gameplay Effect上下文
    FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
    SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
    // 创建次要属性的Gameplay Effect Spec
    const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
    // 应用次要属性的Gameplay Effect
    ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

    // 创建生命属性的Gameplay Effect上下文
    FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
    VitalAttributesContextHandle.AddSourceObject(AvatarActor);
    // 创建生命属性的Gameplay Effect Spec
    const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
    // 应用生命属性的Gameplay Effect
    ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

// 函数：InitializeDefaultAttributesFromSaveData
// 功能：从保存的游戏数据中初始化默认属性。
// 参数：
//   - WorldContextObject: 世界上下文对象，用于获取角色类别信息。
//   - ASC: 能力系统组件，用于应用属性效果。
//   - SaveGame: 保存的游戏数据，包含角色的属性值。
void UAuraAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ULoadScreenSaveGame* SaveGame)
{
    // 获取角色类别信息
    UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
    if (CharacterClassInfo == nullptr) return;

    // 获取游戏标签
    const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

    // 获取Avatar Actor
    const AActor* SourceAvatarActor = ASC->GetAvatarActor();

    // 创建Gameplay Effect上下文
    FGameplayEffectContextHandle EffectContexthandle = ASC->MakeEffectContext();
    EffectContexthandle.AddSourceObject(SourceAvatarActor);

    // 创建Gameplay Effect Spec
    const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->PrimaryAttributes_SetByCaller, 1.f, EffectContexthandle);

    // 设置主要属性的值
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Strength, SaveGame->Strength);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Intelligence, SaveGame->Intelligence);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Resilience, SaveGame->Resilience);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Attributes_Primary_Vigor, SaveGame->Vigor);

    // 应用主要属性的Gameplay Effect
    ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

    // 创建次要属性的Gameplay Effect上下文
    FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
    SecondaryAttributesContextHandle.AddSourceObject(SourceAvatarActor);
    // 创建次要属性的Gameplay Effect Spec
    const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes_Infinite, 1.f, SecondaryAttributesContextHandle);
    // 应用次要属性的Gameplay Effect
    ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

    // 创建生命属性的Gameplay Effect上下文
    FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
    VitalAttributesContextHandle.AddSourceObject(SourceAvatarActor);
    // 创建生命属性的Gameplay Effect Spec
    const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, 1.f, VitalAttributesContextHandle);
    // 应用生命属性的Gameplay Effect
    ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

// 函数：GiveStartupAbilities
// 功能：根据角色类别赋予初始能力。
// 参数：
//   - WorldContextObject: 世界上下文对象，用于获取角色类别信息。
//   - ASC: 能力系统组件，用于赋予能力。
//   - CharacterClass: 角色类别枚举，用于确定初始能力。
void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
    // 获取角色类别信息
    UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
    if (CharacterClassInfo == nullptr) return;

    // 赋予通用能力
    for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
        ASC->GiveAbility(AbilitySpec);
    }

    // 获取角色类别的默认信息
    const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
    // 赋予角色类别的初始能力
    for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
    {
        // 检查Avatar Actor是否实现了CombatInterface
        if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
        {
            // 根据角色等级赋予能力
            FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
            ASC->GiveAbility(AbilitySpec);
        }
    }
}

// 函数：GetXPRewardForClassAndLevel
// 功能：根据角色类别和等级获取经验奖励。
// 参数：
//   - WorldContextObject: 世界上下文对象，用于获取角色类别信息。
//   - CharacterClass: 角色类别枚举，用于确定经验奖励。
//   - CharacterLevel: 角色等级，用于计算经验奖励。
// 返回值：返回计算后的经验奖励值。
int32 UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
    // 获取角色类别信息
    UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
    if (CharacterClassInfo == nullptr) return 0;

    // 获取角色类别的默认信息
    const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
    // 根据等级获取经验奖励
    const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

    // 返回经验奖励值
    return static_cast<int32>(XPReward);
}

// 函数：SetIsRadialDamageEffectParam
// 功能：设置是否为范围伤害效果参数。
// 参数：
//   - DamageEffectParams: 伤害效果参数结构体，用于存储范围伤害的相关参数。
//   - bIsRadial: 是否为范围伤害。
//   - InnerRadius: 范围伤害的内半径。
//   - OuterRadius: 范围伤害的外半径。
//   - Origin: 范围伤害的原点。
void UAuraAbilitySystemLibrary::SetIsRadialDamageEffectParam(FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin)
{
    // 设置是否为范围伤害
    DamageEffectParams.bIsRadialDamage = bIsRadial;
    // 设置范围伤害的内半径
    DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
    // 设置范围伤害的外半径
    DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
    // 设置范围伤害的原点
    DamageEffectParams.RadialDamageOrigin = Origin;
}

// 函数：SetKnockbackDirection
// 功能：设置击退方向。
// 参数：
//   - DamageEffectParams: 伤害效果参数结构体，用于存储击退方向。
//   - KnockbackDirection: 击退方向向量。
//   - Magnitude: 击退力度。如果为0，则使用DamageEffectParams中的默认力度。
void UAuraAbilitySystemLibrary::SetKnockbackDirection(FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude)
{
    // 归一化击退方向
    KnockbackDirection.Normalize();
    // 如果力度为0，使用DamageEffectParams中的默认力度
    if (Magnitude == 0.f)
    {
        DamageEffectParams.KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
    }
    else
    {
        // 否则使用传入的力度
        DamageEffectParams.KnockbackForce = KnockbackDirection * Magnitude;
    }
}

// 函数：SetDeathImpulseDirection
// 功能：设置死亡冲击方向。
// 参数：
//   - DamageEffectParams: 伤害效果参数结构体，用于存储死亡冲击方向。
//   - ImpulseDirection: 死亡冲击方向向量。
//   - Magnitude: 冲击力度。如果为0，则使用DamageEffectParams中的默认力度。
void UAuraAbilitySystemLibrary::SetDeathImpulseDirection(FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude)
{
    // 归一化冲击方向
    ImpulseDirection.Normalize();
    // 如果力度为0，使用DamageEffectParams中的默认力度
    if (Magnitude == 0.f)
    {
        DamageEffectParams.DeathImpulse = ImpulseDirection * DamageEffectParams.DeathImpulseMagnitude;
    }
    else
    {
        // 否则使用传入的力度
        DamageEffectParams.DeathImpulse = ImpulseDirection * Magnitude;
    }
}

// 函数：SetTargetEffectParamsASC
// 功能：设置目标能力系统组件。
// 参数：
//   - DamageEffectParams: 伤害效果参数结构体，用于存储目标能力系统组件。
//   - InASC: 目标能力系统组件。
void UAuraAbilitySystemLibrary::SetTargetEffectParamsASC(FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC)
{
    DamageEffectParams.TargetAbilitySystemComponent = InASC;
}

// 函数：GetCharacterClassInfo
// 功能：获取角色类别信息。
// 参数：
//   - WorldContextObject: 世界上下文对象，用于获取游戏模式。
// 返回值：返回角色类别信息对象，如果获取失败则返回nullptr。
UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
    // 获取游戏模式
    const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
    if (AuraGameMode == nullptr) return nullptr;
    // 返回角色类别信息
    return AuraGameMode->CharacterClassInfo;
}

// 函数：GetAbilityInfo
// 功能：获取能力信息。
// 参数：
//   - WorldContextObject: 世界上下文对象，用于获取游戏模式。
// 返回值：返回能力信息对象，如果获取失败则返回nullptr。
UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
    // 获取游戏模式
    const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
    if (AuraGameMode == nullptr) return nullptr;
    // 返回能力信息
    return AuraGameMode->AbilityInfo;
}

// 函数：GetLootTiers
// 功能：获取战利品等级信息。
// 参数：
//   - WorldContextObject: 世界上下文对象，用于获取游戏模式。
// 返回值：返回战利品等级信息对象，如果获取失败则返回nullptr。
ULootTiers* UAuraAbilitySystemLibrary::GetLootTiers(const UObject* WorldContextObject)
{
    // 获取游戏模式
    const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
    if (AuraGameMode == nullptr) return nullptr;
    // 返回战利品等级信息
    return AuraGameMode->LootTiers;
}

// 函数：IsBlockedHit
// 功能：判断是否为被格挡的命中。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
// 返回值：如果命中被格挡，返回true；否则返回false。
bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
    // 转换为Aura游戏效果上下文
    if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return AuraEffectContext->IsBlockedHit();
    }
    return false;
}

// 函数：IsSuccessfulDebuff
// 功能：判断是否成功施加了减益效果。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
// 返回值：如果成功施加了减益效果，返回true；否则返回false。
bool UAuraAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
    // 转换为Aura游戏效果上下文
    if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return AuraEffectContext->IsSuccessfulDebuff();
    }
    return false;
}

// 函数：GetDebuffDamage
// 功能：获取减益效果的伤害值。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
// 返回值：返回减益效果的伤害值，如果获取失败则返回0。
float UAuraAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
    // 转换为Aura游戏效果上下文
    if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return AuraEffectContext->GetDebuffDamage();
    }
    return 0.f;
}

// 函数：GetDebuffDuration
// 功能：获取减益效果的持续时间。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
// 返回值：返回减益效果的持续时间，如果获取失败则返回0。
float UAuraAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
    // 转换为Aura游戏效果上下文
    if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return AuraEffectContext->GetDebuffDuration();
    }
    return 0.f;
}

// 函数：GetDebuffFrequency
// 功能：获取减益效果的触发频率。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
// 返回值：返回减益效果的触发频率，如果获取失败则返回0。
float UAuraAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
    // 转换为Aura游戏效果上下文
    if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return AuraEffectContext->GetDebuffFrequency();
    }
    return 0.f;
}

// 函数：GetDamageType
// 功能：获取伤害类型。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
// 返回值：返回伤害类型的标签，如果获取失败则返回无效标签。
FGameplayTag UAuraAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
    // 转换为Aura游戏效果上下文
    if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        if (AuraEffectContext->GetDamageType().IsValid())
        {
            return *AuraEffectContext->GetDamageType();
        }
    }
    return FGameplayTag();
}

// 函数：GetDeathImpulse
// 功能：获取死亡冲击向量。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
// 返回值：返回死亡冲击向量，如果获取失败则返回零向量。
FVector UAuraAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
    // 转换为Aura游戏效果上下文
    if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return AuraEffectContext->GetDeathImpulse();
    }
    return FVector::ZeroVector;
}

// 函数：GetKnockbackForce
// 功能：获取击退力向量。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
// 返回值：返回击退力向量，如果获取失败则返回零向量。
FVector UAuraAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
    // 转换为Aura游戏效果上下文
    if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return AuraEffectContext->GetKnockbackForce();
    }
    return FVector::ZeroVector;
}

// 函数：IsCriticalHit
// 功能：判断是否为暴击命中。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
// 返回值：如果是暴击命中，返回true；否则返回false。
bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
    // 转换为Aura游戏效果上下文
    if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return AuraEffectContext->IsCriticalHit();
    }
    return false;
}

// 函数：IsRadialDamage
// 功能：判断是否为范围伤害。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
// 返回值：如果是范围伤害，返回true；否则返回false。
bool UAuraAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
    // 转换为Aura游戏效果上下文
    if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return AuraEffectContext->IsRadialDamage();
    }
    return false;
}

// 函数：GetRadialDamageInnerRadius
// 功能：获取范围伤害的内半径。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
// 返回值：返回范围伤害的内半径，如果获取失败则返回0。
float UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
    // 转换为Aura游戏效果上下文
    if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return AuraEffectContext->GetRadialDamageInnerRadius();
    }
    return 0.f;
}

// 函数：GetRadialDamageOuterRadius
// 功能：获取范围伤害的外半径。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
// 返回值：返回范围伤害的外半径，如果获取失败则返回0。
float UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
    // 转换为Aura游戏效果上下文
    if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return AuraEffectContext->GetRadialDamageOuterRadius();
    }
    return 0.f;
}

// 函数：GetRadialDamageOrigin
// 功能：获取范围伤害的原点。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
// 返回值：返回范围伤害的原点，如果获取失败则返回零向量。
FVector UAuraAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
    // 转换为Aura游戏效果上下文
    if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return AuraEffectContext->GetRadialDamageOrigin();
    }
    return FVector::ZeroVector;
}

// 函数：SetIsBlockedHit
// 功能：设置是否为被格挡的命中。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
//   - bInIsBlockedHit: 是否为被格挡的命中。
void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
    // 转换为Aura游戏效果上下文
    if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        AuraEffectContext->SetIsBlockedHit(bInIsBlockedHit);
    }
}

// 函数：SetIsCriticalHit
// 功能：设置是否为暴击命中。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
//   - bInIsCriticalHit: 是否为暴击命中。
void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
    // 转换为Aura游戏效果上下文
    if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        AuraEffectContext->SetIsCriticalHit(bInIsCriticalHit);
    }
}

// 函数：SetIsSuccessfulDebuff
// 功能：设置是否成功施加了减益效果。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
//   - bInSuccessfulDebuff: 是否成功施加了减益效果。
void UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle, bool bInSuccessfulDebuff)
{
    // 转换为Aura游戏效果上下文
    if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        AuraEffectContext->SetIsSuccessfulDebuff(bInSuccessfulDebuff);
    }
}

// 函数：SetDebuffDamage
// 功能：设置减益效果的伤害值。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
//   - InDamage: 减益效果的伤害值。
void UAuraAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDamage)
{
    // 转换为Aura游戏效果上下文
    if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        AuraEffectContext->SetDebuffDamage(InDamage);
    }
}

// 函数：SetDebuffDuration
// 功能：设置减益效果的持续时间。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
//   - InDuration: 减益效果的持续时间。
void UAuraAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDuration)
{
    // 转换为Aura游戏效果上下文
    if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        AuraEffectContext->SetDebuffDuration(InDuration);
    }
}

// 函数：SetDebuffFrequency
// 功能：设置减益效果的触发频率。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
//   - InFrequency: 减益效果的触发频率。
void UAuraAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InFrequency)
{
    // 转换为Aura游戏效果上下文
    if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        AuraEffectContext->SetDebuffFrequency(InFrequency);
    }
}

// 函数：SetDamageType
// 功能：设置伤害类型。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
//   - InDamageType: 伤害类型的标签。
void UAuraAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
    // 转换为Aura游戏效果上下文
    if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
        AuraEffectContext->SetDamageType(DamageType);
    }
}

// 函数：SetDeathImpulse
// 功能：设置死亡冲击向量。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
//   - InImpulse: 死亡冲击向量。
void UAuraAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse)
{
    // 转换为Aura游戏效果上下文
    if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        AuraEffectContext->SetDeathImpulse(InImpulse);
    }
}

// 函数：SetKnockbackForce
// 功能：设置击退力向量。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
//   - InForce: 击退力向量。
void UAuraAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce)
{
    // 转换为Aura游戏效果上下文
    if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        AuraEffectContext->SetKnockbackForce(InForce);
    }
}

// 函数：SetIsRadialDamage
// 功能：设置是否为范围伤害。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
//   - bInIsRadialDamage: 是否为范围伤害。
void UAuraAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage)
{
    // 转换为Aura游戏效果上下文
    if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        AuraEffectContext->SetIsRadialDamage(bInIsRadialDamage);
    }
}

// 函数：SetRadialDamageInnerRadius
// 功能：设置范围伤害的内半径。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
//   - InInnerRadius: 范围伤害的内半径。
void UAuraAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius)
{
    // 转换为Aura游戏效果上下文
    if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        AuraEffectContext->SetRadialDamageInnerRadius(InInnerRadius);
    }
}

// 函数：SetRadialDamageOuterRadius
// 功能：设置范围伤害的外半径。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
//   - InOuterRadius: 范围伤害的外半径。
void UAuraAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius)
{
    // 转换为Aura游戏效果上下文
    if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        AuraEffectContext->SetRadialDamageOuterRadius(InOuterRadius);
    }
}

// 函数：SetRadialDamageOrigin
// 功能：设置范围伤害的原点。
// 参数：
//   - EffectContextHandle: 游戏效果上下文句柄。
//   - InOrigin: 范围伤害的原点。
void UAuraAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InOrigin)
{
    // 转换为Aura游戏效果上下文
    if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        AuraEffectContext->SetRadialDamageOrigin(InOrigin);
    }
}

// 函数：GetLivePlayersWithinRadius
// 功能：获取指定半径内的存活玩家。
// 参数：
//   - WorldContextObject: 世界上下文对象，用于获取世界。
//   - OutOverlappingActors: 输出的重叠演员数组。
//   - ActorsToIgnore: 需要忽略的演员数组。
//   - Radius: 搜索半径。
//   - SphereOrigin: 搜索的原点。
void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
    // 设置碰撞查询参数，忽略指定演员
    FCollisionQueryParams SphereParams;
    SphereParams.AddIgnoredActors(ActorsToIgnore);

    // 获取世界对象
    if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
    {
        TArray<FOverlapResult> Overlaps;
        // 执行球形重叠检测
        World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
        for (FOverlapResult& Overlap : Overlaps)
        {
            // 检查演员是否实现了CombatInterface且未死亡
            if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
            {
                // 将演员添加到输出数组
                OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
            }
        }
    }
}

// 函数：GetClosestTargets
// 功能：获取距离原点最近的指定数量的目标。
// 参数：
//   - MaxTargets: 最大目标数量。
//   - Actors: 输入的演员数组。
//   - OutClosestTargets: 输出的最近目标数组。
//   - Origin: 原点位置。
void UAuraAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{
    // 如果演员数量小于等于最大目标数量，直接返回所有演员
    if (Actors.Num() <= MaxTargets)
    {
        OutClosestTargets = Actors;
        return;
    }

    TArray<AActor*> ActorsToCheck = Actors;
    int32 NumTargetsFound = 0;

    // 循环查找最近的目标
    while (NumTargetsFound < MaxTargets)
    {
        if (ActorsToCheck.Num() == 0) break;
        double ClosestDistance = TNumericLimits<double>::Max();
        AActor* ClosestActor;
        for (AActor* PotentialTarget : ActorsToCheck)
        {
            // 计算距离
            const double Distance = (PotentialTarget->GetActorLocation() - Origin).Length();
            if (Distance < ClosestDistance)
            {
                ClosestDistance = Distance;
                ClosestActor = PotentialTarget;
            }
        }
        // 移除已找到的最近目标
        ActorsToCheck.Remove(ClosestActor);
        OutClosestTargets.AddUnique(ClosestActor);
        ++NumTargetsFound;
    }
}

// 函数：IsNotFriend
// 功能：判断两个演员是否为非友方。
// 参数：
//   - FirstActor: 第一个演员。
//   - SecondActor: 第二个演员。
// 返回值：如果两个演员是非友方，返回true；否则返回false。
bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
    // 判断两个演员是否都是玩家或都是敌人
    const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
    const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
    const bool bFriends = bBothArePlayers || bBothAreEnemies;
    return !bFriends;
}

// 函数：ApplyDamageEffect
// 功能：应用伤害效果。
// 参数：
//   - DamageEffectParams: 伤害效果参数结构体。
// 返回值：返回游戏效果上下文句柄。
FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
    // 获取游戏标签
    const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
    // 获取源Avatar Actor
    const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

    // 创建游戏效果上下文
    FGameplayEffectContextHandle EffectContexthandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
    EffectContexthandle.AddSourceObject(SourceAvatarActor);
    // 设置死亡冲击和击退力
    SetDeathImpulse(EffectContexthandle, DamageEffectParams.DeathImpulse);
    SetKnockbackForce(EffectContexthandle, DamageEffectParams.KnockbackForce);

    // 设置范围伤害相关参数
    SetIsRadialDamage(EffectContexthandle, DamageEffectParams.bIsRadialDamage);
    SetRadialDamageInnerRadius(EffectContexthandle, DamageEffectParams.RadialDamageInnerRadius);
    SetRadialDamageOuterRadius(EffectContexthandle, DamageEffectParams.RadialDamageOuterRadius);
    SetRadialDamageOrigin(EffectContexthandle, DamageEffectParams.RadialDamageOrigin);

    // 创建游戏效果Spec
    const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContexthandle);

    // 设置伤害类型和减益效果参数
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);

    // 应用游戏效果
    DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
    return EffectContexthandle;
}

// 函数：EvenlySpacedRotators
// 功能：生成均匀分布的旋转器数组。
// 参数：
//   - Forward: 前向向量。
//   - Axis: 旋转轴。
//   - Spread: 分布范围。
//   - NumRotators: 旋转器数量。
// 返回值：返回均匀分布的旋转器数组。
TArray<FRotator> UAuraAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
    TArray<FRotator> Rotators;

    // 计算左侧分布向量
    const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
    if (NumRotators > 1)
    {
        // 计算每个旋转器之间的角度增量
        const float DeltaSpread = Spread / (NumRotators - 1);
        for (int32 i = 0; i < NumRotators; i++)
        {
            // 计算方向向量并转换为旋转器
            const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
            Rotators.Add(Direction.Rotation());
        }
    }
    else
    {
        // 如果只有一个旋转器，直接使用前向向量
        Rotators.Add(Forward.Rotation());
    }
    return Rotators;
}

// 函数：EvenlyRotatedVectors
// 功能：生成均匀分布的向量数组。
// 参数：
//   - Forward: 前向向量。
//   - Axis: 旋转轴。
//   - Spread: 分布范围。
//   - NumVectors: 向量数量。
// 返回值：返回均匀分布的向量数组。
TArray<FVector> UAuraAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
    TArray<FVector> Vectors;

    // 计算左侧分布向量
    const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
    if (NumVectors > 1)
    {
        // 计算每个向量之间的角度增量
        const float DeltaSpread = Spread / (NumVectors - 1);
        for (int32 i = 0; i < NumVectors; i++)
        {
            // 计算方向向量
            const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
            Vectors.Add(Direction);
        }
    }
    else
    {
        // 如果只有一个向量，直接使用前向向量
        Vectors.Add(Forward);
    }
    return Vectors;
}