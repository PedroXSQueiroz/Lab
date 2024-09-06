// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Characters/Base.h"

#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>

// Sets default values
ABase::ABase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(this->InputMappings, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABase::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABase::Move);
	}
	
}

void ABase::Look(const FInputActionValue& value)
{
	FVector2D lookAddition = value.Get<FVector2D>();
	
	this->AddControllerPitchInput(lookAddition.Y);
	this->AddControllerYawInput(lookAddition.X);
}

void ABase::Move(const FInputActionValue& value)
{
	FVector inputDirection = value.Get<FVector>();
	FVector movementDirection = FRotator( 0, this->GetControlRotation().Yaw - 90, 0 ).RotateVector(inputDirection);

	this->AddMovementInput(FVector(movementDirection.X, movementDirection.Y, 0), inputDirection.Length());

}

