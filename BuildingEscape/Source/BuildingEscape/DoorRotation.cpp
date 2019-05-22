// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorRotation.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

#define OUT

// Sets default values for this component's properties
UDoorRotation::UDoorRotation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...c
}


// Called when the game starts
void UDoorRotation::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	if (!Owner) { return; }
	CurrentRotation = Owner->GetActorRotation();

	if (!PressurePlate) 
	{ 
		UE_LOG(LogTemp, Error, TEXT("No pressure plate associated with %s"), *Owner->GetName()); 
		return;
	}
}

void UDoorRotation::OpenDoor()
{
	/*if (!Owner) { return; }
	Owner->SetActorRotation(FRotator(0.0f, CurrentRotation.Yaw - DoorAngle, 0.0f));*/
	OnOpenRequest.Broadcast();
}

void UDoorRotation::CloseDoor()
{
	/*if (!Owner) { return; }
	Owner->SetActorRotation(FRotator(0.0f, DoorAngle, 0.0f));*/
	OnCloseRequest.Broadcast();
}


// Called every frame
void UDoorRotation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//Polling the trigger volume to check if trigger is stepped on:
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//Checking if it's time to close the door:
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelayTime)
		CloseDoor();
	// ...
}

float UDoorRotation::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;

	TArray<AActor*> OverlappingActors;

	if (!PressurePlate) { return 0.0f; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const auto& Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		//UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());
	}

	return TotalMass;
}

