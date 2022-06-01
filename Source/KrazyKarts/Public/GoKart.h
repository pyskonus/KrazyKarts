// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

UCLASS()
class KRAZYKARTS_API AGoKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGoKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere)
	float Mass = 200;

	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;

	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 3;

	UPROPERTY(EditAnywhere)
	float TurningInertia = 3;
	
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;
	
	UPROPERTY(EditAnywhere)
	float RollingFrictionCoeff = 1;
	
	FVector Velocity;
	float Throttle;
	float SteeringThrow;

	// forward movement
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveForward(const float Value);
	void CalculateVelocity(const float DeltaTime);
	// turning
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveRight(const float Value);
	void ApplyRotation(const float DeltaTime);
	// resistance
	FVector GetResistance() const;
};
