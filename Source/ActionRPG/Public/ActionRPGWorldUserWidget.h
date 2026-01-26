
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionRPGWorldUserWidget.generated.h"

class USizeBox;
/**
 * this widget class meant to be attached to a specific actor in the world.
 * it will manually update its screen position in Tick to follow that actor
 */
UCLASS()
class ACTIONRPG_API UActionRPGWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	//UI Equivalent of Actor::Tick -> runs Every frame to update position 
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	// A container widget used to move the content around the screen.
	// meta=(BindWidget) links this pointer to a widget named "ParentSizeBox" in the UMG Designer.
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> ParentSizeBox;
	
public:
	//the actor this widget is floating above
	UPROPERTY(BlueprintReadOnly,Category=UI)
	AActor* AttachActor;
	
	//offset from the actor's origin(for a character Z = 90 )
	UPROPERTY(EditDefaultsOnly,Category=UI)
	FVector WorldOffset;
	
};
