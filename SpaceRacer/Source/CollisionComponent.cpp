#include "CollisionComponent.h"
#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"

IMPL_COMPONENT(CollisionComponent, Component);

CollisionComponent::CollisionComponent(Actor& owner)
	:Component(owner)
{

}

void CollisionComponent::Register()
{
	Super::Register();
	mOwner.GetGame().GetPhysWorld().AddComponent(ThisPtr());
}

void CollisionComponent::Unregister()
{
	Super::Register();
	mOwner.GetGame().GetPhysWorld().RemoveComponent(ThisPtr());
}

bool CollisionComponent::Intersects(std::shared_ptr<CollisionComponent> other)
{
	return false;
}
