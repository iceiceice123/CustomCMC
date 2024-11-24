// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMovement/CMCCharacterMovementComponent.h"

#include "GameFramework/Character.h"

void UCMCCharacterMovementComponent::FSavedMove_CMC::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWantsToSprint = 0;
}

void UCMCCharacterMovementComponent::FSavedMove_CMC::SetMoveFor(ACharacter* C, float InDeltaTime,
	const FVector& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UCMCCharacterMovementComponent* CharacterMovementComponent = Cast<UCMCCharacterMovementComponent>(C->GetCharacterMovement());
	Saved_bWantsToSprint = CharacterMovementComponent->Safe_bWantsToSprint;
}

//can we comabine lastmove and newmove, it can save bandwith
bool UCMCCharacterMovementComponent::FSavedMove_CMC::CanCombineWith(const FSavedMovePtr& NewMove,
	ACharacter* InCharacter, float MaxDelta) const
{
	FSavedMove_CMC* NewSafeMoveData = static_cast<FSavedMove_CMC*>(NewMove.Get());
	if (Saved_bWantsToSprint != NewSafeMoveData->Saved_bWantsToSprint)
	{
		return false;
	}
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UCMCCharacterMovementComponent::FSavedMove_CMC::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);

	UCMCCharacterMovementComponent* CharacterMovementComponent = Cast<UCMCCharacterMovementComponent>(C->GetCharacterMovement());

	CharacterMovementComponent->Safe_bWantsToSprint = Saved_bWantsToSprint;
}

uint8 UCMCCharacterMovementComponent::FSavedMove_CMC::GetCompressedFlags() const
{
	uint8 Result = FSavedMove_Character::GetCompressedFlags();

	if(Saved_bWantsToSprint) Result |= FLAG_Custom_0;
	return  Result;
}

UCMCCharacterMovementComponent::FNetworkPredictionData_Client_CMC::FNetworkPredictionData_Client_CMC(
	const UCMCCharacterMovementComponent& ClientMovement): Super(ClientMovement)
{
	
}

FSavedMovePtr UCMCCharacterMovementComponent::FNetworkPredictionData_Client_CMC::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_CMC());
}

FNetworkPredictionData_Client* UCMCCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr);

	if (ClientPredictionData == nullptr)
	{
		UCMCCharacterMovementComponent* MutableThis = const_cast<UCMCCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_CMC(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UCMCCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UCMCCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (MovementMode == MOVE_Walking)
	{
		if(Safe_bWantsToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
		}
		else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
	}
}

void UCMCCharacterMovementComponent::SprintePressed()
{
	Safe_bWantsToSprint = true;
}

void UCMCCharacterMovementComponent::SprintReleased()
{
	Safe_bWantsToSprint = false;
}
