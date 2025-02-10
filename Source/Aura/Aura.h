// Copyright RickYan

#pragma once

#include "CoreMinimal.h"

// 定义自定义深度值的宏，用于在渲染中区分不同的对象
#define CUSTOM_DEPTH_RED 250   // 自定义深度红色通道值
#define CUSTOM_DEPTH_BLUE 251  // 自定义深度蓝色通道值
#define CUSTOM_DEPTH_TAN 252   // 自定义深度棕褐色通道值

// 定义碰撞通道的宏，用于在物理世界中区分不同的碰撞类型
#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1  // 定义投射物碰撞通道
#define ECC_Target ECollisionChannel::ECC_GameTraceChannel2      // 定义目标碰撞通道
#define ECC_ExcludePlayers ECollisionChannel::ECC_GameTraceChannel3 // 定义排除玩家碰撞通道
