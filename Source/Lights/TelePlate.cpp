#include "TelePlate.h"
#include "Components/WidgetComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Camera/CameraComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "CommandTextWidget.h"
#include "Components/TextBlock.h"


void ATelePlate::BeginPlay()
{
    Super::BeginPlay();

    PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

    EnableInput(GetWorld()->GetFirstPlayerController());
    UInputComponent* MyInputComp = this->InputComponent;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), RoofBlueprint, RoofList);

    if  (MyInputComp)
    {
        SetInputCmp(MyInputComp);
    }  
}

void ATelePlate::SetInputCmp(UInputComponent* MyInputComponent)
{
    MyInputComponent -> BindAction("NextPlate", IE_Pressed, this, &ATelePlate::NextPlate);
    MyInputComponent -> BindAction("PreviousPlate", IE_Pressed, this, &ATelePlate::PreviousPlate);
    MyInputComponent -> BindAction("SelectPlate", IE_Pressed, this, &ATelePlate::SelectPlate);
    MyInputComponent -> BindAction("ReturnToTelePlate", IE_Pressed, this, &ATelePlate::ReturnToTelePlate);
    MyInputComponent -> BindAction("QuitSelection", IE_Pressed, this, &ATelePlate::QuitSelection);
}

// Find all the plates that have been overlapped at least once
void ATelePlate::GetValidPlates(TArray<AActor*>& PlatesList)
{
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlate::StaticClass(), PlatesList);
    
    for (int32 i = 0 ; i < PlatesList.Num() ; i++)
    {
        APlate* Plate = Cast<APlate>(PlatesList[i]);
        
        if (!(Plate -> bUtilized))
        {
            PlatesList.RemoveAt(i);
            i-=1;         
        }
    }
}

void ATelePlate::SetupPlateSelection()
{
    // Get the command widget, save the original text for later, then edit it for the selection
    GetCommandWidget();
    OldWidgetText = CommandTextWidget->CommandText->GetText();
    CommandTextWidget->CommandText->SetText(FText::FromString("<- ->: Switch Plate\nENTER: Teleport to the selected Plate\n Q: Return to Game"));

    bCanInputOperate = true;
    bSelectionAvailable = true;
    ArrayIndex = 0;

    EnableInput(GetWorld()->GetFirstPlayerController());

    // Hide the roof for the selection
    for (AActor* Roof : RoofList)
    {
        Roof->SetActorHiddenInGame(true);
    }

    if  (PlayerPawn)
    {
        PlayerCamera = PlayerPawn->FindComponentByClass<UCameraComponent>();

        // Save the location of the camera component for later
        PlayerCameraLocation = PlayerCamera->GetRelativeLocation();
        PlayerCameraRotation = PlayerCamera->GetRelativeRotation();

        // Put the camera at the position for selection
        PlayerCamera->SetWorldLocationAndRotation(ChoosingLocation, ChoosingRotation);
        PlayerCamera->bUsePawnControlRotation = false;

        // Disable input and hides the Pawn for selection
        PlayerPawn->DisableInput(GetWorld()->GetFirstPlayerController());
        PlayerPawn->SetActorHiddenInGame(true);

        // Show the selection cursor for the first plate
        TrigList[ArrayIndex]->FindComponentByClass<UWidgetComponent>()->SetHiddenInGame(false);
    }
}

void ATelePlate::GetCommandWidget()
{
    TArray<UUserWidget*> WidgetArray;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), WidgetArray, UCommandTextWidget::StaticClass());
    CommandTextWidget = Cast<UCommandTextWidget>(WidgetArray[0]);
}

// Iterate on the array and show/hide the selection cursor of the plates
void ATelePlate::NextPlate()
{
    if  (bSelectionAvailable && bCanInputOperate)
    {
        TrigList[ArrayIndex]->FindComponentByClass<UWidgetComponent>()->SetHiddenInGame(true);
        
        if  (ArrayIndex == TrigList.Num() - 1)
        {
            ArrayIndex = 0;
        }
        
        else
        {
            ArrayIndex += 1;
            ArrayIndex = FMath::Clamp(ArrayIndex, 0, TrigList.Num() - 1);
        }
        
        TrigList[ArrayIndex]->FindComponentByClass<UWidgetComponent>()->SetHiddenInGame(false);
    }
}

// Iterate on the array and show/hide the selection cursor of the plates
void ATelePlate::PreviousPlate()
{
    if  (bSelectionAvailable && bCanInputOperate)
    {    
        TrigList[ArrayIndex]->FindComponentByClass<UWidgetComponent>()->SetHiddenInGame(true);
        
        if  (ArrayIndex == 0)
        {
            ArrayIndex = TrigList.Num() - 1; 
        }

        else
        {
            ArrayIndex -= 1;
            ArrayIndex = FMath::Clamp(ArrayIndex, 0, TrigList.Num() - 1);  
        }
        UE_LOG(LogTemp, Warning, TEXT("%i"), ArrayIndex);
        TrigList[ArrayIndex]->FindComponentByClass<UWidgetComponent>()->SetHiddenInGame(false);
    }
}

// If the player quit the selection, restore everything for gameplay (widget, roof, cursors, camera and pawn)
void ATelePlate::QuitSelection()
{
    if (bSelectionAvailable && bCanInputOperate)
    {
        CommandTextWidget->CommandText->SetText(OldWidgetText);

        for (AActor* Roof : RoofList)
        {
            Roof->SetActorHiddenInGame(false);
        }
        
        TrigList[ArrayIndex]->FindComponentByClass<UWidgetComponent>()->SetHiddenInGame(true);
        
        PlayerCamera->SetRelativeLocationAndRotation(PlayerCameraLocation, PlayerCameraRotation);
        PlayerCamera->bUsePawnControlRotation = true;
        
        PlayerPawn->EnableInput(GetWorld()->GetFirstPlayerController());
        PlayerPawn->SetActorHiddenInGame(false);

        DisableInput(GetWorld()->GetFirstPlayerController());

        bCanInputOperate = false;
    }
}

// When the player select the Plate to teleport restore everything for gameplay (widget, roof, cursors, camera and pawn) and teleport him
void ATelePlate::SelectPlate()
{    
    if (bSelectionAvailable && bCanInputOperate)
    {
        FString String = (OldWidgetText.ToString());
        String.Append("\nR: Return to the Teleport Plate");
        CommandTextWidget->CommandText->SetText(FText::FromString(String));
        
        TrigList[ArrayIndex]->FindComponentByClass<UWidgetComponent>()->SetHiddenInGame(true);

        for (AActor* Roof : RoofList)
        {
            Roof->SetActorHiddenInGame(false);
        }
        
        PlayerCamera->SetRelativeLocationAndRotation(PlayerCameraLocation, PlayerCameraRotation);
        PlayerCamera->bUsePawnControlRotation = true;

        PlayerPawn->SetActorLocation(TrigList[ArrayIndex]->GetActorLocation() + FVector(0.f, 0.f, 100.f));
        PlayerPawn->EnableInput(GetWorld()->GetFirstPlayerController());
        PlayerPawn->SetActorHiddenInGame(false);

        bSelectionAvailable = false;
    }
}

/* When the player wants to return to the TelePlate verify if he has grabbed an object than teleport him (and the object if there is one)
   and restore everything for gameplay (widget, roof, cursors, camera and pawn)*/
void ATelePlate::ReturnToTelePlate()
{
    if (!bSelectionAvailable && bCanInputOperate)
    {
        UPrimitiveComponent* GrabbedObject = PlayerPawn->FindComponentByClass<UPhysicsHandleComponent>()->GetGrabbedComponent();

        if (GrabbedObject)
        {
            GrabbedObject->SetSimulatePhysics(false);
            GrabbedObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            GrabbedObject->SetWorldLocation(GetActorLocation());
            GrabbedObject->SetSimulatePhysics(true);
            GrabbedObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        }

        CommandTextWidget->CommandText->SetText(OldWidgetText);
        PlayerPawn->SetActorLocation(GetActorLocation() + FVector(0.f, 0.f, 100.f));
        DisableInput(GetWorld()->GetFirstPlayerController());
        bCanInputOperate = false;
    }
}




