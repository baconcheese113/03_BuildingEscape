// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:

	// How far ahead of the player can we reach in cm
	int32 Reach = 100;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Raycast and grab what's in reach
	void Grab();
	// Release what's held
	void Release();
	//Find attached physics handle
	void FindPhysicsHandleComponent();
	//Setup assummed input component
	void SetupInputComponent();
	//Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
	
	//Return location of reach line start if true, end if false
	FVector GetReachLine(bool);
};
