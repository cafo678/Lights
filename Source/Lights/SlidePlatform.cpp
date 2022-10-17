#include "SlidePlatform.h"

void ASlidePlatform::BeginPlay()
{
    Super::BeginPlay();

    // Create a Dynamic Material (editable at runtime) based on the material of the actor
    DynamicMaterial = UMaterialInstanceDynamic::Create(FindComponentByClass<UStaticMeshComponent>()->GetMaterial(0), NULL);

    // Set the Dynamic Material to the actor
    FindComponentByClass<UStaticMeshComponent>()->SetMaterial(0, DynamicMaterial);

    FLinearColor Red = FLinearColor(1.f, 0.f, 0.f);
    FLinearColor Green = FLinearColor(0.f, 1.f, 0.f);
    FLinearColor Yellow = FLinearColor(1.f, 1.f, 0.f);
    FLinearColor Blue = FLinearColor(0.f, 0.f, 1.f);

    // Change the color of the material based on the color

    if (PlatformColor == EPlatformColor::Red)
    {
        DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Red);
    }

    else if (PlatformColor == EPlatformColor::Green)
    {
        DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Green);
    }

    else if (PlatformColor == EPlatformColor::Yellow)
    {
        DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Yellow);
    }

    else
    {
        DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Blue);
    }
}

