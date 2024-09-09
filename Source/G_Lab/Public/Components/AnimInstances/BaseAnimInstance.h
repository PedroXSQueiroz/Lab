// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

/****
* IKS
*****/
USTRUCT(BlueprintType)
struct FIKParams 
{
	GENERATED_BODY()

public:
	

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Weight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName WeightCurveName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float LockWeight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName LockWeightCurveName;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName WeightRotationCurveName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RotationWeight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D RotationFactor;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName RootBone;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName EffectorBone;

	UPROPERTY()
	FVector StartReferenceLocation;

	UPROPERTY()
	FVector StartTraceLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName StartTraceBoneReference;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool AddRelativeLocationFromReverseMask;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool AlignEffectorBoneToSurface;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector StartTraceMask{ FVector::Zero() };

	UPROPERTY(BlueprintReadWrite)
	FVector ReverseMaskStartTraceLocation{ FVector::Zero() };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector TraceDirection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TraceLength;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Padding;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TraceRadius;

	UPROPERTY(BlueprintReadOnly)
	FVector HitNormal;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector HittedTraceLocation;

	UPROPERTY(BlueprintReadOnly)
	FVector CurrentLockLocation;

	UPROPERTY(BlueprintReadOnly)
	FRotator EffectorAddtiveRotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator EffectorAddtiveRotationOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float EffectorRotationBoneLocalOffsetW;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector FinalIKLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator FinalIKRotation;

	UPROPERTY(BlueprintReadOnly)
	bool Hitted;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxLength;

};

USTRUCT(BlueprintType)
struct FIKData
{
	GENERATED_BODY()

public:

	FIKData() {};

	FIKData(
			float weight
		,	FVector hittedTraceLocation
		,	FVector reference
		,	FVector normal
		,	FVector location
		,	FRotator rotation = FRotator::ZeroRotator
		,	float rotationWeight = 0
	): 
		StartReferenceLocation(reference)
	,	HittedTraceLocation(hittedTraceLocation)
	,	Location(location)
	,	Weight(weight)
	,	Rotation(rotation)
	,	RotationWeight(rotationWeight)
	{};

	UPROPERTY()
	FVector StartReferenceLocation;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FVector HittedTraceLocation;

	UPROPERTY()
	FRotator Rotation;

	UPROPERTY()
	FVector Normal;

	UPROPERTY()
	float Weight;

	UPROPERTY()
	float RotationWeight;

};

USTRUCT(BlueprintType, Blueprintable)
struct FIKRoots
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName RootReference;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName RootName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> ChildIKs;

	UPROPERTY(BlueprintReadOnly)
	bool RootShouldDealocate;

	UPROPERTY(BlueprintReadOnly)
	FVector RootLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName RootIKWeightCurveName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RootIKWeight;

};


/************
* TRANSITIONS
*************/
UINTERFACE()
class UTransitionModifier : public UInterface
{
	GENERATED_BODY()
};

class G_LAB_API ITransitionModifier
{
	GENERATED_BODY()

public:

	virtual void Execute(UBaseAnimInstance* anim, FIKParams& currentParam, FTransitIKParams& transitParams) PURE_VIRTUAL(TEXT("NOT IMPLEMENTED YET"), return; );

};

UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class G_LAB_API UTransitionModifierAdditionalHeight : public UObject, public ITransitionModifier
{

	GENERATED_BODY()

public:

	virtual void Execute(UBaseAnimInstance* anim, FIKParams& currentParam, FTransitIKParams& transitParams) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName HeightCurve;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HeightScale{ 8 };
};

USTRUCT(BlueprintType, Blueprintable)
struct FTransitIKParams 
{
	GENERATED_BODY()

public:

	FTransitIKParams() {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName IKName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName WeightTransitionCurveName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TargetWeight;

	UPROPERTY(BlueprintReadOnly)
	FVector InitialLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (MustImplement = "TransitionModifier"))
	TArray<TSubclassOf<UObject>> Modifier;

	UPROPERTY()
	TArray<ITransitionModifier*> ModifierInstances;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName RootCurveName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName RootBoneReference;

	UPROPERTY()
	FVector TransitLockLocation;

	UPROPERTY()
	FVector InitialRootLocation;

	UPROPERTY()
	FRotator InitialRootRotation;

	UPROPERTY(BlueprintReadWrite)
	FVector CurrentRootLocation;

	UPROPERTY(BlueprintReadWrite)
	FRotator CurrrentRootRotation;

};

/**
 * 
 */
UCLASS()
class G_LAB_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	
	/*****
	* IKs
	*****/
	UFUNCTION(BlueprintCallable)
	FIKData GetIKData(const FIKParams& ikParams, bool& hitted);

	UFUNCTION(BlueprintCallable)
	TArray<FIKParams> UpdateIKs();

	void UpdateRoots();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings|IKs")
	TMap<FName, FIKParams> IKParams;

	UFUNCTION(BlueprintCallable)
	void UpdateReverseMaskStartTraceLocation(FName ikName, FVector newLocation);

	UFUNCTION(BlueprintCallable, BlueprintPure = true)
	TArray<FIKParams> GetIKParamsValues();

	UFUNCTION(BlueprintCallable)
	void SetStopping(bool flag);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|IKs")
	TArray<FIKRoots> IKRoots;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|IKs")
	bool DebugIKs { false };

	/***************
	* VELOCITY STATS
	****************/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Movement")
	float IdleMoveThreshold;
	
	UPROPERTY(BlueprintReadOnly)
	bool IsStopping;

	UPROPERTY(BlueprintReadOnly)
	bool IsDecelerating;

	UPROPERTY(BlueprintReadOnly)
	bool IsAccelerating;

	UFUNCTION(BlueprintCallable)
	void UpdateVelocityStats();

	UPROPERTY()
	FVector LastVelocity{ FVector::Zero() };

	/***********
	* TRANSITION
	************/
	UPROPERTY(BlueprintReadWrite)
	bool IsTransitioning;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool StoppingMovementAnimEnabled;

	UPROPERTY()
	TMap<FName, FTransitIKParams> IKTransitionInitialLocation;

	UFUNCTION(BlueprintCallable)
	virtual void SetInitialIKTransitions(TArray<FTransitIKParams> iksToTransit);

	UFUNCTION(BlueprintCallable)
	virtual void InterpolateIKTransition();

	UFUNCTION(BlueprintCallable)
	virtual void CleanIKTransitions();
	
	FVector GetRelativeIKLocation(FVector ikLocation);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Transition|Movement")
	bool MovingIdleTransitAnimEnabled;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Transition|IKs")
	bool DebugTransitionIKs{ false };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Transition|IKs")
	bool OverrideRootDuringTransition;

	UPROPERTY(BlueprintReadWrite)
	FVector OverrideRootDuringTranitionLocation;

	UPROPERTY(BlueprintReadWrite)
	FRotator OverrideRootDuringTranitionRotator;

};
