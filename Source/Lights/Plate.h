#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Plate.generated.h"

class UPointLightComponent;
class UBoxComponent;
class UWidgetComponent;

UCLASS()
class LIGHTS_API APlate : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void ChangePlateColor();

	// If the trigger has been overlapped at least once
	UPROPERTY(BlueprintReadWrite, Category = "Custom")
	bool bUtilized;

protected:

	APlate();

	virtual void BeginPlay();

	UPROPERTY(BlueprintReadWrite, Category = "Custom")
	FLinearColor BaseColor;

	UPROPERTY(BlueprintReadWrite, Category = "Custom")
	FLinearColor UsingColor = FLinearColor(0.f, 1.0f, 1.0f);

private:

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* PointLight = nullptr;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionVolume = nullptr;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* SelectionCursor = nullptr;
	
};
