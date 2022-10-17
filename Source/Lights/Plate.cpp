#include "Plate.h"
#include "Components/WidgetComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/BoxComponent.h"


APlate::APlate()
{
    PrimaryActorTick.bCanEverTick = false;

    PointLight = CreateDefaultSubobject<UPointLightComponent> (FName("PointLightComponent"));
	PointLight->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    CollisionVolume = CreateDefaultSubobject<UBoxComponent> (FName("CollisionVolumeComponent"));
	CollisionVolume->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    SelectionCursor = CreateDefaultSubobject<UWidgetComponent> (FName("SelectionCursorComponent"));
    SelectionCursor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void APlate::BeginPlay()
{
    Super::BeginPlay();

	bUtilized = false;
	BaseColor = FindComponentByClass<UPointLightComponent>()->GetLightColor();
}

void APlate::ChangePlateColor()
{
    if (PointLight)
    {
        if (PointLight->GetLightColor() == BaseColor)
        {
            PointLight->SetLightColor(UsingColor);
        }
        
        else
        {
            PointLight->SetLightColor(BaseColor);
        }       
    }
}