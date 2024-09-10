// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AnimInstances/BaseAnimInstance.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Algo/Reverse.h"

#pragma optimize("", off)
FVector UBaseAnimInstance::GetDesiredDirection()
{
    ACharacter* player = Cast<ACharacter>( this->GetOwningActor() );
    
    if (!player) 
    {
        return FVector::Zero();
    }

    return player->GetLastMovementInputVector();
}

FIKData UBaseAnimInstance::GetIKData(const FIKParams& ikParams, bool& hitted)
{
    
    /************
    * GET WEIGHTS
    *************/
    bool getWeightByCurve = ikParams.WeightCurveName.IsValid()
                        &&  !ikParams.WeightCurveName.IsNone()
                        &&  ikParams.WeightCurveName.GetStringLength() > 0;
    float currentWeight = getWeightByCurve ?
            this->GetCurveValue(ikParams.WeightCurveName) 
        :   ikParams.Weight;

    UE_LOG(LogTemp, Log, TEXT("WEIGHT:%.2f"), currentWeight);
    
    bool getLockWeightByCurve = ikParams.LockWeightCurveName.IsValid() 
                            &&  !ikParams.LockWeightCurveName.IsNone()
                            &&  ikParams.LockWeightCurveName.GetStringLength() > 0;
    float currentLockWeight = getLockWeightByCurve ?
            this->GetCurveValue(ikParams.LockWeightCurveName)
        :   ikParams.LockWeight;


    /**********************
    * CALCULATE START TRACE
    ***********************/
    FVector startTrace = FVector::Zero();
    FVector startReference = FVector::Zero();
    if (ikParams.StartTraceBoneReference.IsValid() && ikParams.StartTraceBoneReference.GetStringLength() > 0) 
    {
        startReference = this->GetOwningComponent()->GetSocketLocation(ikParams.StartTraceBoneReference) * ikParams.StartTraceMask;
        startTrace = FVector(startReference);

        if (ikParams.AddRelativeLocationFromReverseMask) 
        {
            FVector reverseMask = FVector(1) - ikParams.StartTraceMask;
            startTrace += reverseMask * ikParams.ReverseMaskStartTraceLocation;
        }
    }
    else 
    {
        startTrace = ikParams.StartTraceLocation;
    }

    /*****************
    * FIND IK LOCATION
    ******************/
    FHitResult traceResult;
    FCollisionQueryParams params;
    
    hitted = this->GetWorld()->SweepSingleByChannel(
        traceResult,
        startTrace,
        startTrace + ( ikParams.TraceDirection * ikParams.TraceLength ),
        FQuat::Identity,
        ECollisionChannel::ECC_Visibility,
        FCollisionShape::MakeSphere(ikParams.TraceRadius),
        params
    );

    if (hitted) 
    {
        FVector ikLocation = FMath::Lerp( 
                traceResult.ImpactPoint + ((ikParams.TraceDirection * -1) * ikParams.Padding)
            ,   ikParams.CurrentLockLocation
            ,   currentLockWeight
        );

        if (!ikParams.AlignEffectorBoneToSurface) 
        {
            return FIKData(
                    currentWeight
                ,   startReference
                ,   traceResult.Normal
                ,   ikLocation
            );
        }

        float asideAlignment = UKismetMathLibrary::DegAtan2(traceResult.Normal.Y, traceResult.Normal.Z);
        float forwardAlignment = UKismetMathLibrary::DegAtan2(traceResult.Normal.X, traceResult.Normal.Z) * -1;

        FRotator effectorBoneAdditiveRotation = FRotator(
                forwardAlignment + ikParams.EffectorAddtiveRotationOffset.Pitch
            ,   0
            ,   asideAlignment + ikParams.EffectorAddtiveRotationOffset.Roll);

        float rotationWeight = this->GetCurveValue(ikParams.WeightRotationCurveName);
     
        return FIKData(
                currentWeight
            ,   startReference
            ,   traceResult.Normal
            ,   ikLocation
            ,   effectorBoneAdditiveRotation
            ,   rotationWeight
        );
    }

    return FIKData();
}
#pragma optimize("", on)

#pragma optimize("", off)
TArray<FIKParams> UBaseAnimInstance::UpdateIKs()
{
    ACharacter* character = Cast<ACharacter>( this->GetOwningActor() );

    if (!character) 
    {
        return TArray<FIKParams>();
    }
    
    TArray<FName> iks;
    this->IKParams.GetKeys(iks);

    for (FName currentIk : iks) 
    {
        bool hitted = false;
        
        FIKData ik = this->GetIKData(this->IKParams[currentIk], hitted);
        this->IKParams[currentIk].StartReferenceLocation = ik.StartReferenceLocation;
        this->IKParams[currentIk].CurrentLockLocation = ik.Location;
        this->IKParams[currentIk].HitNormal = ik.Normal;
        this->IKParams[currentIk].EffectorAddtiveRotation = ik.Rotation;
        this->IKParams[currentIk].Hitted = hitted;
        this->IKParams[currentIk].Weight = ik.Weight;
        this->IKParams[currentIk].RotationWeight = ik.RotationWeight;
        this->IKParams[currentIk].FinalIKLocation = this->GetRelativeIKLocation(
            this->IKParams[currentIk].CurrentLockLocation
        );
    }

    this->UpdateRoots();


    if (this->IsTransitioning) 
    {
        this->InterpolateIKTransition();
    }

    return this->GetIKParamsValues();

}

void UBaseAnimInstance::UpdateRoots()
{
    USkeletalMeshComponent* body = this->GetOwningComponent();

    for (FIKRoots& currentRoot : this->IKRoots)
    {
        currentRoot.RootShouldDealocate = false;

        FVector rootLocation = body->GetSocketLocation(currentRoot.RootReference);

        float excedingDealocation = 0;
        FVector directionDealocation = FVector::Zero();

        for (FName childIK : currentRoot.ChildIKs)
        {
            FVector ikDealocation = this->IKParams[childIK].CurrentLockLocation - rootLocation;

            float currentExcedingDealocation = ikDealocation.Length() - this->IKParams[childIK].MaxLength;

            if (currentExcedingDealocation > 0 && currentExcedingDealocation > excedingDealocation)
            {
                excedingDealocation = currentExcedingDealocation;
                directionDealocation = this->IKParams[childIK].TraceDirection;
                currentRoot.RootShouldDealocate = true;
            }
        }

        if (currentRoot.RootShouldDealocate)
        {
            //TODO: IMPLEMENT OBTAIN OF WEIGHT WITHOUT CURVES

            float rootIKWeight = this->GetCurveValue(currentRoot.RootIKWeightCurveName);
            FVector additionalRootDealocation = (directionDealocation * excedingDealocation * rootIKWeight);

            currentRoot.RootLocation = additionalRootDealocation;

            DrawDebugSphere(
                this->GetWorld(),
                currentRoot.RootLocation + rootLocation,
                12,
                12,
                FColor::Purple
            );
        }

        FVector greaterDealocation = FVector::Zero();
    }
}

#pragma optimize("", on)
void UBaseAnimInstance::UpdateVelocityStats()
{
    FVector currrentVelocity    = this->GetOwningActor()->GetVelocity();
    FVector horizontalVelocity  = FVector(currrentVelocity.X, currrentVelocity.Y, 0);
    float currentAcceleration   = horizontalVelocity.Length() - this->LastVelocity.Length();

    this->IsDecelerating        = currentAcceleration < ( this->IdleMoveThreshold * -1 );
    this->IsAccelerating        = currentAcceleration > this->IdleMoveThreshold;

    if (!this->IsStopping && this->IsDecelerating)
    {
        this->IsStopping = this->GetOwningActor()->GetVelocity().Length() < this->IdleMoveThreshold;
    }

    if (this->IsAccelerating)
    {
        this->IsStopping = false;
    }

    this->LastVelocity = horizontalVelocity;
}

void UBaseAnimInstance::UpdateReverseMaskStartTraceLocation(FName ikName,FVector newLocation)
{
    this->IKParams[ikName].ReverseMaskStartTraceLocation = newLocation;
}

TArray<FIKParams> UBaseAnimInstance::GetIKParamsValues()
{
    TArray<FName> keys;
    TArray<FIKParams> values;
    
    this->IKParams.GetKeys(keys);

    for (FName key : keys) 
    {
        values.Add(this->IKParams[key]);
    }
    
    return values;
}

void UBaseAnimInstance::SetStopping(bool flag)
{
    this->IsStopping = flag;
}

void UBaseAnimInstance::SetInitialIKTransitions(TArray<FTransitIKParams> iksToTransit)
{
    for (FTransitIKParams currentIK : iksToTransit)
    {
        if (this->IKParams.Contains(currentIK.IKName)) 
        {
            currentIK.InitialLocation = this->IKParams[currentIK.IKName].CurrentLockLocation;
            
            this->IKTransitionInitialLocation.Add(
                    currentIK.IKName
                ,   currentIK
            );
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("IK settings: %s not found"), *currentIK.IKName.ToString());
        }
        
    }
}

#pragma optimize("", off)
void UBaseAnimInstance::InterpolateIKTransition()
{
    TArray<FName> iks;

    this->IKTransitionInitialLocation.GetKeys(iks);
    
    for (FName ik : iks) 
    {
        
        FTransitIKParams currentTransit = this->IKTransitionInitialLocation[ik];
        FVector currentInitialLocation = currentTransit.InitialLocation;

        float interpWeight = 0;
        if (
            currentTransit.WeightTransitionCurveName.IsValid()
            && !currentTransit.WeightTransitionCurveName.IsNone()
        ) {
            this->GetCurveValue(currentTransit.WeightTransitionCurveName, interpWeight);
        }
        else 
        {
            interpWeight = currentTransit.TargetWeight;
        }

        FVector transitingLocation = FMath::Lerp(currentInitialLocation, this->IKParams[ik].StartReferenceLocation, interpWeight);
        FVector startTrace = FVector(transitingLocation);

        if (this->IKParams[ik].AddRelativeLocationFromReverseMask)
        {
            FVector reverseMask = FVector(1) - this->IKParams[ik].StartTraceMask;
            startTrace += reverseMask * this->IKParams[ik].ReverseMaskStartTraceLocation;
        }

        FHitResult traceResult;
        bool hitted = this->GetWorld()->SweepSingleByChannel(
            traceResult,
            startTrace,
            startTrace + (this->IKParams[ik].TraceDirection * this->IKParams[ik].TraceLength),
            FQuat::Identity,
            ECollisionChannel::ECC_Visibility,
            FCollisionShape::MakeSphere(this->IKParams[ik].TraceRadius)
        );

        
        if (hitted) 
        {
            transitingLocation = traceResult.ImpactPoint + ((this->IKParams[ik].TraceDirection * -1) * this->IKParams[ik].Padding);
            this->IKParams[ik].FinalIKLocation = this->GetRelativeIKLocation(transitingLocation);
        }
        else
        {
            this->IKParams[ik].FinalIKLocation = this->GetRelativeIKLocation(transitingLocation);
        }
        
        this->IKParams[ik].CurrentLockLocation = transitingLocation;
    }
}
#pragma optimize("", on)

void UBaseAnimInstance::CleanIKTransitions()
{
    this->IKTransitionInitialLocation.Empty();
}

FVector UBaseAnimInstance::GetRelativeIKLocation(FVector ikLocation)
{
    ACharacter* character = Cast<ACharacter>(this->GetOwningActor());

    if (!character)
    {
        return FVector();
    }
    
    return FTransform(
        character->GetMesh()->GetComponentQuat(),
        character->GetMesh()->GetComponentLocation()
    ).InverseTransformPosition(ikLocation);
}

bool UBaseAnimInstance::SetupLean()
{
    
    for (FLeanParams& lean : this->LeanParans)
    {
        TArray<FName> chain { lean.Effector };
        FName currentBone = FName("");

        do 
        {
            currentBone = this->GetOwningComponent()->GetParentBone(chain.Last());

            if (!currentBone.IsNone()) 
            {
                chain.Add(currentBone);
            }

        }while(
                !currentBone.IsEqual(lean.Root) 
            &&  !currentBone.IsNone()
        );

        if (currentBone.IsEqual(lean.Root)) 
        {
            chain.Add(lean.Root);
            Algo::Reverse(chain);
            lean.BoneChain = chain;

            return true;
        }

        UE_LOG(LogTemp, Log, TEXT("ROOT AND EFFECTOR LOOKS TO NOT BE IN THE SAME SEQUENCE"));

    }

    return false;

}
