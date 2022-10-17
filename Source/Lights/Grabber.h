#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"

class UPhysicsHandleComponent;
class UInputComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIGHTS_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	void Grab(); // Grab Object
	void Release(); // Release Object
	void FindPhysicsHandle(); // Find Physics Handle Component
	void SetInputCmp(); // Find and Set the Input Component
	FHitResult FindObjectToGrab();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Reach = 150.f; // Multiplier for the vector line in front of the player
	UPhysicsHandleComponent* PhysicsHandle = nullptr; // Component that handles objects
	UInputComponent* InputCmp = nullptr; // Component that takes inputs of the player
	FVector LineTraceEnd; // EndPoint of the RayCast from the Player
	FVector Location; // Location of the Player
	FRotator Rotation; // Rotation of the Player
	FHitResult Hit; // Object that the RayCast intersect
};
