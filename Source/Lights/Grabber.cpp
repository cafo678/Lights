#include "Grabber.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();

	SetInputCmp();	
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(Location, Rotation); // Get Player Location and Rotation

	LineTraceEnd = Location + Rotation.Vector() * Reach; // EndPoint of the RayCast from the Player

	// Check that the pointer exists
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle -> GrabbedComponent) // If we have grabbed something
	{
		PhysicsHandle -> SetTargetLocation(LineTraceEnd); // Set the grabbed object location at the EndPoint of the RayCast from the Player
	}
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner() -> FindComponentByClass<UPhysicsHandleComponent>();

	// Check that the pointer exists
	if (!PhysicsHandle) 
	{ 
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandleComponent not found")); 
	}
}

void UGrabber::SetInputCmp()
{
	InputCmp = GetOwner() -> FindComponentByClass<UInputComponent>();

	// Check that the pointer exists
	if (!InputCmp) 
	{ 
		UE_LOG(LogTemp, Error, TEXT("InputComponent not found")); 
	}

	InputCmp -> BindAction("Grab", IE_Pressed, this, &UGrabber::Grab); 
	InputCmp -> BindAction("Grab", IE_Released, this, &UGrabber::Release);
}

void UGrabber::Grab()
{
	UPrimitiveComponent* ComponentToGrab = FindObjectToGrab().GetComponent(); // Get Component of the FHitResult

	if (FindObjectToGrab().GetActor()) // Get Actor of the FHitResult
	{
		// Check that the pointer exists
		if (!PhysicsHandle) { return; }

		// Grab Component of the FHitResult at his Location
		PhysicsHandle -> GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), ComponentToGrab->GetOwner()->GetActorRotation());
	}
}

void UGrabber::Release()
{
	// Check that the pointer exists
	if (!PhysicsHandle) { return; }
	PhysicsHandle -> ReleaseComponent(); // Release Component of the FHitResult
}

FHitResult UGrabber::FindObjectToGrab()
{
	GetWorld() -> GetFirstPlayerController() -> GetPlayerViewPoint(Location, Rotation); // Get Player Location and Rotation

	LineTraceEnd = Location + Rotation.Vector() * Reach; // EndPoint of the RayCast from the Player
	
	GetWorld() -> LineTraceSingleByObjectType // RayCast
	(
		Hit, // OUT Parameter
		Location, // Start Location of the RayCast
		LineTraceEnd, // End Location of the RayCast
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), // Search only for PhysicsBody type
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner()) // Not Explained, False doesn't watch to the proper form but the generic (cubotto, non sedia), Ignore ourself 
	);

	AActor* ActorHit = Hit.GetActor(); // Take the actor of the FHitResult

	if (Hit.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hitted object: %s"), *(Hit.GetActor() -> GetName()));
	}	
	
	return Hit;
}