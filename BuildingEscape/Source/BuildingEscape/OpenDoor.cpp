// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
			
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the Trigger Volume
	if (GetTotalMassOfActorsOnPlate() > 50.f) {
		OpenDoor(true);
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//Check if it's time to close door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
		OpenDoor(false);
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	float TotalMass = 0.f;
	
	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (const auto* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		//TotalMass += Actor->GetRootPrimitiveComponent()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("TotalMass is %f for %s."), Actor->GetRootPrimitiveComponent()->GetMass(), *Actor->GetName());
		
	}
	return TotalMass;
}

// Move the door open if true, closed if false
void UOpenDoor::OpenDoor(bool Open) {
	//My implementation
	AActor* Owner = GetOwner();

	FRotator NewRotation = FRotator(0.f, (Open ? OpenAngle : 50.f), 0.f);
	Owner->SetActorRotation(NewRotation);
}
