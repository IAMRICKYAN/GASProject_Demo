// Copyright RickYan

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

UCLASS()
class AURA_API AMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	AMagicCircle();
	virtual void Tick(float DeltaTime) override;

	// 魔法圈贴花组件，用于在场景中显示魔法圈的视觉效果
	// 该组件对所有人可见，但只能在编辑器中通过蓝图进行修改
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> MagicCircleDecal;
	
protected:
	virtual void BeginPlay() override;


	
};
