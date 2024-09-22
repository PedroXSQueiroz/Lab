// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/ToggableCache.h"

void FAnimNode_ToggableCache::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	this->BasePose.Initialize(Context);
}

#pragma optimize("", off)
void FAnimNode_ToggableCache::Update_AnyThread(const FAnimationUpdateContext& Context)
{
	UE_LOG(LogTemp, Log, TEXT("IS CACHING: %s"), this->ShouldCache? *FString("YES") : *FString("NO"))
	
	if (this->ShouldCache)
	{
		this->BasePose.Update(Context);
	}
}
#pragma optimize("", on)

void FAnimNode_ToggableCache::Evaluate_AnyThread(FPoseContext& Output)
{
	this->BasePose.Evaluate(Output);
}

FString UAnimGraphNode_ToggableCache::GetNodeCategory() const
{
	return FString("Anim Node Category");
}
