#include "ActionRPGWorldUserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"


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
	// 2. Projection: Math to convert 3D World Space (Vector) -> 2D Screen Space (Vector2D)
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(),AttachActor->GetActorLocation()+WorldOffset,ScreenPosition))
	{
		//dPI Scaling: UI coordinates are not 1:1 with pixels due to resolution scaling.
		//divide by the viewport scale to get the correct UMG position.
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition /= Scale;
		// 4. Apply the new position to the container widget
		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}
