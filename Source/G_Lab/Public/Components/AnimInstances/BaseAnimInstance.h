// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

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
		,	FVector reference
		,	FVector normal
		,	FVector location
		,	FRotator rotation = FRotator::ZeroRotator
		,	float rotationWeight = 0
	): 
		StartReferenceLocation(reference)
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
};

USTRUCT()
struct FLeanBone 
{
	GENERATED_BODY()
	
public:
	
	FLeanBone() {};

	FLeanBone(
		FName name,
		FTransform transform
	):	Name(name)
	,	Transform(transform){};

	UPROPERTY()
	FName Name;

	UPROPERTY()
	FTransform Transform;
};

USTRUCT(BlueprintType, Blueprintable)
struct FLeanParams 
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Root;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Effector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRuntimeFloatCurve  LeanIntensityCurve;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator MaxAdditiveAngle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector MaxAdditiveDealocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Velocity {0.8};

	UPROPERTY()
	TArray<FLeanBone> BoneChain;

	UPROPERTY()
	FRotator PreviewLeanAngle;

	UPROPERTY()
	FRotator PreviewDiffLeanAngle;
};

USTRUCT(BlueprintType, Blueprintable)
struct FLean 
{
	GENERATED_BODY()

public:

	FLean() {};

	FLean(TArray<FLeanBone> bones)
	: Bones(bones)
	{};

	UPROPERTY()
	TArray<FLeanBone> Bones;
};

/**
 * 
 */
UCLASS()
class G_LAB_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure = true)
	FVector GetDesiredDirection();

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Movement")
	bool StoppingMovementAnimEnabled;

	UFUNCTION(BlueprintCallable)
	void SetStopping(bool flag);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|IKs")
	TArray<FIKRoots> IKRoots;

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

	UPROPERTY()
	TMap<FName, FTransitIKParams> IKTransitionInitialLocation;

	UFUNCTION(BlueprintCallable)
	virtual void SetInitialIKTransitions(TArray<FTransitIKParams> iksToTransit);

	UFUNCTION(BlueprintCallable)
	virtual void InterpolateIKTransition();

	UFUNCTION(BlueprintCallable)
	virtual void CleanIKTransitions();
	
	FVector GetRelativeIKLocation(FVector ikLocation);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Movement")
	bool MovingIdleTransitAnimEnabled;

	/*****
	* LEAN
	******/

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Lean")
	TArray<FLeanParams> LeanParans;

	UFUNCTION(BlueprintCallable)
	bool SetupLean();

	UFUNCTION(BlueprintCallable)
	void UpdateLean();

	UFUNCTION(BlueprintCallable, BlueprintPure = true)
	TArray<FLean> GetLeans();

};
