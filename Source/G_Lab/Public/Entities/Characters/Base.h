// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Base.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS(Blueprintable, BlueprintType)
class G_LAB_API ABase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Settings")
	UInputMappingContext* InputMappings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Settings")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Settings")
	UInputAction* MoveAction;

	UFUNCTION()
	void Look(const FInputActionValue& value);

	UFUNCTION()
	void Move(const FInputActionValue& value);

};
