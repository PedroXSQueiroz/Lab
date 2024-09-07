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
	FVector CurrentLockLocation;

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
			FVector reference
		,	FVector location
		,	float weight
	): 
		StartReferenceLocation(reference)
	,	Location(location)
	,	Weight(weight)
	{};

	UPROPERTY()
	FVector StartReferenceLocation;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	float Weight;
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

};
