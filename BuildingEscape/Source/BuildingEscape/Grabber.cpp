// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/Public/DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/PrimitiveComponent.h"

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
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector LineTraceEnd = GetLineReachEnd();
	if (PhysicsHandle->GrabbedComponent)
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
}

///Set attached input component, bind controls etc.:
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Input component found for %s"), *GetOwner()->GetName());

		///Binding controls:
		InputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component not found for %s"), *GetOwner()->GetName());
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
		UE_LOG(LogTemp, Error, TEXT("Physics handle not found"));
}
///Ray-cast and grab what is in reach:
void UGrabber::Grab()
{
	///LINE TRACE and see if we reach any actors with physics body collision channel set:
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent *ComponentToGrab = HitResult.GetComponent();
	AActor *ActorHit = HitResult.GetActor();

	///If we hit something, attached PhysicsHandle:
	if (ActorHit)
	{
		/*PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation()
		);*/

		/*PhysicsHandle->GrabComponent
		(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true
		);*/
		PhysicsHandle->GrabComponentAtLocationWithRotation
		(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector LineTraceEnd = GetLineReachEnd();

	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	//Line-trace out to reach a distance:
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType
	(
		OUTPUT Hit,
		GetLineReachStart(),
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	//Logging out what is being hit:
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
		UE_LOG(LogTemp, Warning, TEXT("Object Hit: %s"), *ActorHit->GetName());

	return Hit;
}

FVector UGrabber::GetLineReachStart()
{
	//Get player viewpoint:
	FVector PlayerViewpointLocation;						//Point based location of player (where the player is standing, positioned etc.)
	FRotator PlayerViewpointRotation;						//What direction in all 3 axis the player is looking at
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUTPUT PlayerViewpointLocation, OUTPUT PlayerViewpointRotation);				//Sets values in the func params

	return PlayerViewpointLocation;
}

FVector UGrabber::GetLineReachEnd()
{
	//Get player viewpoint:
	FVector PlayerViewpointLocation;						//Point based location of player (where the player is standing, positioned etc.)
	FRotator PlayerViewpointRotation;						//What direction in all 3 axis the player is looking at
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUTPUT PlayerViewpointLocation, OUTPUT PlayerViewpointRotation);				//Sets values in the func params

	return PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;
}