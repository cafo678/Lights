#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "SlidePlatform.generated.h"

class UMaterialInstanceDynamic;

UENUM()
enum class EPlatformColor : uint8
{
	Red, 
	Green, 
	Yellow,
	Blue
};

UCLASS()
class LIGHTS_API ASlidePlatform : public AStaticMeshActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	EPlatformColor PlatformColor;

private:
	virtual void BeginPlay() override;

	UMaterialInstanceDynamic* DynamicMaterial = nullptr;	
};
