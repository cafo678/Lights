#include "SlidePlate.h"
#include "Components/PointLightComponent.h"

void ASlidePlate::BeginPlay()
{
    Super::BeginPlay();

    PointLight = FindComponentByClass<UPointLightComponent>();

    FLinearColor Red = FLinearColor(1.f, 0.f, 0.f);
    FLinearColor Green = FLinearColor(0.f, 1.f, 0.f);
    FLinearColor Yellow = FLinearColor(1.f, 1.f, 0.f);
    FLinearColor Blue = FLinearColor(0.f, 0.f, 1.f);

    if (PlateColor == EPlateColor::Red)
    {
        PointLight->SetLightColor(Red);
        BaseColor = Red;
    }

    else if (PlateColor == EPlateColor::Green)
    {
        PointLight->SetLightColor(Green);
        BaseColor = Green;
    }

    else if (PlateColor == EPlateColor::Yellow)
    {
        PointLight->SetLightColor(Yellow);
        BaseColor = Yellow;
    }

    else
    {
        PointLight->SetLightColor(Blue);
        BaseColor = Blue;
    }
}

