#pragma once

#include "CoreMinimal.h"
#include "Plate.h"
#include "TelePlate.generated.h"

class UCameraComponent;
class UCommandTextWidget;

UCLASS()
class LIGHTS_API ATelePlate : public APlate
{
	GENERATED_BODY()

private:

	void NextPlate();
	void PreviousPlate();
	void SelectPlate();
	void ReturnToTelePlate();
	void QuitSelection();

	void SetInputCmp(UInputComponent* MyInputComponent);

	void GetCommandWidget();

	int32 ArrayIndex = 0;

	bool bCanInputOperate = false;
	bool bSelectionAvailable = true;

	APawn* PlayerPawn = nullptr;
	
	FVector ChoosingLocation = FVector(1000.f, 1200.f, 4500.f);
	FRotator ChoosingRotation = FRotator(-90.f, 90.f, 0.f);

	FVector PlayerCameraLocation;
	FRotator PlayerCameraRotation;

	FLinearColor SelectionColor = FLinearColor(1.f, 0.f, 0.8f);

	UCameraComponent* PlayerCamera = nullptr;

	TArray<AActor*> RoofList;

protected:

	virtual void BeginPlay() override;

	UFUNCTION (BlueprintCallable, Category = "Custom")
	void GetValidPlates(UPARAM(ref) TArray<AActor*>& PlatesList);

	UFUNCTION (BlueprintCallable, Category = "Custom")
	void SetupPlateSelection();
	
	UPROPERTY(BlueprintReadWrite, Category = "Custom")
	TArray<AActor*> TrigList;

	UPROPERTY(BlueprintReadOnly, Category = "Custom")
	FText OldWidgetText;

	UPROPERTY(BlueprintReadOnly, Category = "Custom")
	UCommandTextWidget* CommandTextWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Custom")
	TSubclassOf<AStaticMeshActor> RoofBlueprint; 
};
