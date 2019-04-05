// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/Public/DrawDebugHelpers.h"

#define OUTPUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Get player viewpoint:
	FVector PlayerViewpointLocation;						//Point based location of player (where the player is standing, positioned etc.)
	FRotator PlayerViewpointRotation;						//What direction in all 3 axis the player is looking at
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUTPUT PlayerViewpointLocation, OUTPUT PlayerViewpointRotation);				//Sets values in the func params
	UE_LOG(LogTemp, Warning, TEXT("Location: %s and Rotation: %s"), *PlayerViewpointLocation.ToString(), *PlayerViewpointRotation.ToString());
	
	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;

	//Draw a red line to show the direction the player is looking in:
	DrawDebugLine(
		GetWorld(),
		PlayerViewpointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.0f,
		0,
		10.0f
	);

	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	//Line-trace out to reach a distance:
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType
	(
		OUTPUT Hit,
		PlayerViewpointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	//Logging out what is being hit:
	AActor* ActorHit = Hit.GetActor();
	if(ActorHit)
		UE_LOG(LogTemp, Warning, TEXT("Object Hit: %s"), *ActorHit->GetName());
}

