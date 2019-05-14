// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/Public/DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

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
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Physics handle not found"));
	}

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Input component found for %s"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component not found for %s"), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Get player viewpoint:
	FVector PlayerViewpointLocation;						//Point based location of player (where the player is standing, positioned etc.)
	FRotator PlayerViewpointRotation;						//What direction in all 3 axis the player is looking at
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUTPUT PlayerViewpointLocation, OUTPUT PlayerViewpointRotation);				//Sets values in the func params
	
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

///Ray-cast and grab what is in reach:
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
}


void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release pressed"));
}

