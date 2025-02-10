// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AuraInputConfig.h"
#include "AuraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	// 模板函数，用于绑定能力动作到输入配置
	// 参数InputConfig是指向UAuraInputConfig的指针，包含了输入配置信息
	// 参数Object是指向UserClass的指针，表示要绑定动作的对象
	// 参数PressedFunc是按下动作时调用的函数，类型为PressedFuncType
	// 参数ReleasedFunc是释放动作时调用的函数，类型为ReleasedFuncType
	// 参数HeldFunc是持续按住动作时调用的函数，类型为HeldFuncType
	// 该函数将输入配置中的能力动作绑定到指定的对象和函数上，以便在相应的输入事件发生时调用这些函数
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
		void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);

};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig); // 确保InputConfig不为空

	// 遍历InputConfig中的AbilityInputActions数组
	for (const FAuraInputAction& Action : InputConfig->AbilityInputActions)
	{
		// 检查InputAction和InputTag是否有效
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			// 如果提供了PressedFunc函数，则绑定按下动作
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			// 如果提供了ReleasedFunc函数，则绑定释放动作
			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
            
			// 如果提供了HeldFunc函数，则绑定持续按住动作
			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}

