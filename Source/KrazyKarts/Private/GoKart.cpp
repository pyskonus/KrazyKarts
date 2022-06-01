// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKart.h"

#include "Components/InputComponent.h"

// Sets default values
AGoKart::AGoKart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGoKart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CalculateVelocity(DeltaTime);

	ApplyRotation(DeltaTime);
	
	const FVector Translation = Velocity * DeltaTime * 100;
	FHitResult Hit;
	AddActorWorldOffset(Translation, true, &Hit);
	if (Hit.IsValidBlockingHit())	{ Velocity = FVector::ZeroVector; }
}

// Called to bind functionality to input
void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::Server_MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGoKart::Server_MoveRight);
}

void AGoKart::CalculateVelocity(const float DeltaTime)
{
	FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle;

	Force += GetResistance();

	const FVector Acceleration = Force / Mass;

	Velocity += Acceleration * DeltaTime;
}

void AGoKart::ApplyRotation(const float DeltaTime)
{
	const float DotProduct = FVector::DotProduct(GetActorForwardVector(), Velocity);
	const float RotationAngle = FMath::Sign(DotProduct) * FMath::Sqrt(FMath::Abs(DotProduct)) * DeltaTime * SteeringThrow / MinTurningRadius;
	const FQuat RotationDelta(GetActorUpVector(), RotationAngle);
	Velocity = RotationDelta.RotateVector(Velocity);
	AddActorWorldRotation(RotationDelta);
}


void AGoKart::Server_MoveForward_Implementation(const float Value)
{
	Throttle = Value;
}

bool AGoKart::Server_MoveForward_Validate(const float Value)
{
	return FMath::Abs(Value) <= 1;
}

void AGoKart::Server_MoveRight_Implementation(const float Value)
{
	SteeringThrow = Value;
}

bool AGoKart::Server_MoveRight_Validate(const float Value)
{
	return FMath::Abs(Value) <= 1;
}

FVector AGoKart::GetResistance() const
{
	return -Velocity.GetSafeNormal() * (Velocity.SizeSquared() * DragCoefficient + FMath::Sqrt(Velocity.Size()) * RollingFrictionCoeff * Mass);
}
