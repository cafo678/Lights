#pragma once

#include "CoreMinimal.h"
#include "Plate.h"
#include "SlidePlate.generated.h"

UENUM()
enum class EPlateColor : uint8
{
	Red, 
	Green, 
	Yellow,
	Blue
};

UCLASS()
class LIGHTS_API ASlidePlate : public APlate
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	EPlateColor PlateColor;

private:

	virtual void BeginPlay() override;

	UPointLightComponent* PointLight = nullptr;	
};
