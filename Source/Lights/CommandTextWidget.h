#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommandTextWidget.generated.h"

class UTextBlock;

UCLASS()
class LIGHTS_API UCommandTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// Text of the Command Widget
	UPROPERTY (BlueprintReadWrite, Category = "Custom")
	UTextBlock* CommandText = nullptr; 
};
