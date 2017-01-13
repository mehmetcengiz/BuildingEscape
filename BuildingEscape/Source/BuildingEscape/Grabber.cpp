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
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();


}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//TODO if the physics handle is attached
		//Move the object that we're holding.




	///See what we hit
}

void UGrabber::FindPhysicsHandleComponent(){
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}
}

void UGrabber::SetupInputComponent(){
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

void UGrabber::Grab(){
	UE_LOG(LogTemp, Warning, TEXT("Grab button pressed."))
		//TODO attach physics handle
		//Try and reach any actors with physics body collision channel set
		GetFirstPhysicsBodyInReach();

	//If we hit something then attach a physics handle
}

void UGrabber::Released(){
	UE_LOG(LogTemp, Warning, TEXT("Grab button released."))
	//TODO Release physics handle.
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach(){



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
	if (HitedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit:  %s "), *(HitedActor->GetName()))
	}
	
	return Hit;
}
