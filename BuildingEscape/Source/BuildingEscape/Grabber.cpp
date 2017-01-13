// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"))

		///Look for attached Physics Handle
		PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
		if(PhysicsHandle) {
			//Handle physics
		}else {
			UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
		}	
	
		///Look for input component
		InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
		if (InputComponent) {
			UE_LOG(LogTemp, Warning, TEXT("Input component found!"))
			///Bind input axis.
			InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
			InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Released);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("%s missing Input component"), *GetOwner()->GetName())
		}

}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//Get player view point this tick.
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	///UE_LOG(LogTemp, Warning, TEXT("Location: %s Rotation: %s"),*PlayerViewPointLocation.ToString(), * PlayerViewPointRotation.ToString())
	

	///Draw a red trace in the word to visualise.
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);

	///Setup Query Parameters.
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	///Line-trace (AKA ray-cast) out to reach distance.
	
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	HitedActor = Hit.GetActor();
	if(HitedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit:  %s "), *(HitedActor->GetName()))
	}



	///See what we hit
}


void UGrabber::Grab(){
	UE_LOG(LogTemp,Warning,TEXT("Grab button pressed."))
}

void UGrabber::Released(){
	UE_LOG(LogTemp, Warning, TEXT("Grab button released."))
}
