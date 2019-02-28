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

	// ...
}


// Called when the game starts
void UDoorRotation::BeginPlay()
{
	Super::BeginPlay();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	// ...
	
}

void UDoorRotation::OpenDoor()
{
	AActor* Owner = GetOwner();
	FRotator NewRotation = FRotator(0.0f, 60.0f, 0.0f);
	Owner->SetActorRotation(NewRotation);
}


// Called every frame
void UDoorRotation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
		OpenDoor();
	// ...
}

