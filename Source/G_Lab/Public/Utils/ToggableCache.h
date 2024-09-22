// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnimGraphNode_Base.h"
#include "Kismet2/BlueprintEditorUtils.h"

#include "ToggableCache.generated.h"

USTRUCT(BlueprintType)
struct FAnimNode_ToggableCache : public FAnimNode_Base
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links)
	FPoseLink BasePose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links, meta = (PinShownByDefault))
	bool ShouldCache{true};

	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	//virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) override;
	virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
	virtual void Evaluate_AnyThread(FPoseContext& Output) override;
};

/**
 * 
 */
UCLASS()
class UAnimGraphNode_ToggableCache : public UAnimGraphNode_Base
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = Settings)
	FAnimNode_ToggableCache Node;

	virtual FString GetNodeCategory() const override;
};
