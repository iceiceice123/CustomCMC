// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CMCCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMCMC_API UCMCCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class  FSavedMove_CMC : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;
	public:
		virtual void Clear() override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, const FVector& NewAccel,
			FNetworkPredictionData_Client_Character& ClientData) override;
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter,
			float MaxDelta) const override;
		virtual void PrepMoveFor(ACharacter* C) override;
		virtual uint8 GetCompressedFlags() const override;
		uint8 Saved_bWantsToSprint: 1;
	};

	class FNetworkPredictionData_Client_CMC : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_CMC(const UCMCCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;
		
		virtual FSavedMovePtr AllocateNewMove() override;
	};

	UPROPERTY(EditDefaultsOnly) float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly) float Walk_MaxWalkSpeed;

	bool Safe_bWantsToSprint;

public:
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
public:
	UFUNCTION(BlueprintCallable)
	void SprintePressed();

	UFUNCTION(BlueprintCallable)
	void SprintReleased();
};
