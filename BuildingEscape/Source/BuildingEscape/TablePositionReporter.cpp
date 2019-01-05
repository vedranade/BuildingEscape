// Fill out your copyright notice in the Description page of Project Settings.

#include "TablePositionReporter.h"
#include "GameFramework/Actor.h"


// Sets default values for this component's properties
UTablePositionReporter::UTablePositionReporter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTablePositionReporter::BeginPlay()
{
	Super::BeginPlay();

	
	UE_LOG(LogTemp, Warning, TEXT("Position reporter ongoing for Table"));
	// ...
	
}


// Called every frame
void UTablePositionReporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

