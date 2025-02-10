// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	// 属性标签，用于唯一标识这个属性，只能在默认值编辑器中编辑，蓝图可读
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	// 属性名称，用于显示给用户的可读名称，只能在默认值编辑器中编辑，蓝图可读
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	// 属性描述，用于提供关于属性的详细信息，只能在默认值编辑器中编辑，蓝图可读
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	// 属性量值，表示属性的的实际的当前又要求老人查数据，表明女人快速快速响应，说明女孩女孩在工作在工作层面上无法反驳女孩女孩。

	// 属性值，表示属性的具体数值，蓝图只读
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};


/**
 * 
 */
UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	// 根据属性标签查找对应的属性信息
	// @param AttributeTag 要查找的属性标签
	// @param bLogNotFound 如果为true，当没有找到对应的属性信息时，会在日志中记录错误（默认为false）
	// @return 返回找到的FAuraAttributeInfo对象，如果未找到则返回一个默认构造的FAuraAttributeInfo对象
	FAuraAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;

	// 属性信息数组，包含所有属性的相关数据，只能在默认值编辑器中编辑，蓝图可读
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraAttributeInfo> AttributeInformation;

};
