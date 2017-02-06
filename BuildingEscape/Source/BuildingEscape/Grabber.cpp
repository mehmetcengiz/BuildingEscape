#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

UGrabber::UGrabber(){
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay(){
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ){
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (!PhysicsHandle) { return; }
	if(PhysicsHandle->GrabbedComponent) {
		//Move the object that we're holding.
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

void UGrabber::FindPhysicsHandleComponent(){
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}
}

void UGrabber::SetupInputComponent(){
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Released);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing Input component"), *GetOwner()->GetName())
	}
}

void UGrabber::Grab(){
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if(ActorHit) {
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponent(
			ComponentToGrab, 
			NAME_None, //No bones needed.
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // Allow Rotation
			);
	}
}

void UGrabber::Released(){
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach(){


	///Setup Query Parameters.
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	///Line-trace (AKA ray-cast) out to reach distance.

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	HitedActor = HitResult.GetActor();
	if (HitedActor){
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit:  %s "), *(HitedActor->GetName()))
	}
	
	return HitResult;
}

FVector UGrabber::GetReachLineEnd() {
	FVector GetPlayerViewPointLocation;
	FRotator GetPlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT GetPlayerViewPointLocation,
		OUT GetPlayerViewPointRotation
	);
	return GetPlayerViewPointLocation;
}

FVector UGrabber::GetReachLineStart(){
	FVector GetPlayerViewPointLocation;
	FRotator GetPlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT GetPlayerViewPointLocation,
		OUT GetPlayerViewPointRotation
	);
	return (GetPlayerViewPointLocation + GetPlayerViewPointRotation.Vector() * Reach);
}
