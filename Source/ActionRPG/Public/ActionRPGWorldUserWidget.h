
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionRPGWorldUserWidget.generated.h"

class UOverlay;
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
	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<UOverlay> ParentOverlay;
	
	//UI Equivalent of Actor::Tick -> runs Every frame to update position 
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual void NativeConstruct() override;
	
	bool bWasOnScreen = true;
	// A container widget used to move the content around the screen.
	// meta=(BindWidget) links this pointer to a widget named "ParentSizeBox" in the UMG Designer.
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> ParentSizeBox;
	
public:
	UFUNCTION(BlueprintCallable, Category= UI)
	static void AddToRootCanvasPanel(UUserWidget* InNewWidget);
	
	//the actor this widget is floating above
	UPROPERTY(BlueprintReadWrite,Category=UI, meta = (ExposeOnSpawn = true))
	AActor* AttachActor;
	
	//offset from the actor's origin(for a character Z = 90 )
	UPROPERTY(EditDefaultsOnly,Category=UI)
	FVector WorldOffset;
	
};
