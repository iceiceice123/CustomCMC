// Fill out your copyright notice in the Description page of Project Settings.


#include "CMCCharacter.h"

// Sets default values
ACMCCharacter::ACMCCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCMCCharacterMovementComponent>(CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UcmcCharacterMovementComponent = Cast<class UCMCCharacterMovementComponent>(GetCharacterMovement());
}

// Called when the game starts or when spawned
void ACMCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACMCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACMCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

