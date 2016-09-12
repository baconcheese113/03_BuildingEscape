// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::Grab() {

	//Try and reach any actors with physics body collision channel set
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();
	// If we hit something then attach a physics handle
	if (ActorHit) {
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, //no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			false
		);
	}
}
void UGrabber::Release() {
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	// Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle found on %s."), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Ray-cast out to reach distance
	FHitResult Hit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());


	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLine(true),
		GetReachLine(false),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	); 
	if (Hit.GetActor())
		UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *Hit.GetActor()->GetName());
	return Hit;
}

// Get reach line start if true, end if false
FVector UGrabber::GetReachLine(bool Start)
{
	// Get player view point
	FVector ViewPoint;
	FRotator Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT ViewPoint, OUT Rotation);
	//return linetrace start
	if (Start)
		return ViewPoint;
	//return linetrace end
	return ViewPoint + Rotation.Vector() * Reach;
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(GetReachLine(false));
	}
}
