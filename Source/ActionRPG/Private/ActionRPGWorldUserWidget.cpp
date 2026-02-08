#include "ActionRPGWorldUserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CanvasPanel.h"


void UActionRPGWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	// 1. Cleanup Check: If the enemy died or the actor was destroyed, remove this health bar
	if (!IsValid(AttachActor))
	{
		RemoveFromParent();
		UE_LOG(LogTemp,Warning,TEXT("AttachedActor no longer valid,removing Health widget"))
		return;
	}
	
	FVector2D ScreenPosition;
	bool bIsOnScreen =UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(),AttachActor->GetActorLocation()+WorldOffset,ScreenPosition); 
	// 2. Projection: Math to convert 3D World Space (Vector) -> 2D Screen Space (Vector2D)
	if (bIsOnScreen)
	{
		//dPI Scaling: UI coordinates are not 1:1 with pixels due to resolution scaling.
		//divide by the viewport scale to get the correct UMG position.
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition /= Scale;
		// 4. Apply the new position to the container widget
	//	ParentOverlay->SetRenderTranslation(ScreenPosition);
	}
	
	if (bWasOnScreen != bIsOnScreen)
	{
	//	ParentOverlay->SetVisibility(bIsOnScreen ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
	bWasOnScreen = bIsOnScreen;
}

void UActionRPGWorldUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UCanvasPanelSlot*CanvasPanelSlot = static_cast<UCanvasPanelSlot*>(Slot);
	if (CanvasPanelSlot)
	{
		CanvasPanelSlot->SetAlignment(FVector2D(0.5f,0.5f));
		CanvasPanelSlot->SetAutoSize(true);
	}
}

void UActionRPGWorldUserWidget::AddToRootCanvasPanel(UUserWidget* InNewWidget)
{
	// Grab the 'main hud' which will have a Canvas Panel to use.
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(InNewWidget, Widgets, UUserWidget::StaticClass(), true);
	// Expect only one main hud, other menus etc. might mess with this and requires slight change (eg. adding a pause menu to the root)
	//check(Widgets.Num() == 1)

	UUserWidget* MainHUD = Widgets[0];
	UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(MainHUD->GetRootWidget());
	CanvasPanel->AddChild(InNewWidget);
}
