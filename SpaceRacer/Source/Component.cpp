#include "Component.h"
#include "Actor.h"
#include "Game.h";

IMPL_COMPONENT(Component, Object);

Component::Component(Actor& owner)
	:mOwner(owner)
{
}

Component::~Component()
{

	mOwner.GetGame().GetTime().ClearAllTimers(this);
}

void Component::Tick(float deltaTime)
{

}

void Component::Register()
{

}

void Component::Unregister()
{

}
