
#include "ActionRPGBlackholeProjectile.h"
#include "PhysicsEngine/RadialForceComponent.h"

AActionRPGBlackholeProjectile::AActionRPGBlackholeProjectile()
{
	RadialForceComponent=CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
	RadialForceComponent->SetupAttachment(RootComponent);
}
