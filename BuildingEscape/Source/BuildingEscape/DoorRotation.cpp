// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorRotation.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

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
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	CurrentRotation = Owner->GetActorRotation();
	
	UE_LOG(LogTemp, Warning, TEXT("Current door rotation is: %f"), CurrentRotation.Yaw);
	// ...
	
}

void UDoorRotation::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, 90.0f - DoorAngle, 0.0f));
}

void UDoorRotation::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, DoorAngle, 0.0f));
}


// Called every frame
void UDoorRotation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//Polling the trigger volume to check if trigger is stepped on:
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//Checking if it's time to close the door:
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelayTime)
		CloseDoor();
	// ...
}

