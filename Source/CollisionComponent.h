// CollisionComponent.h
// Base class for all collision components
// supported by the game

#pragma  once
#include "Component.h"

class CollisionComponent : public Component
{
	DECL_COMPONENT(CollisionComponent, Component);
public:
	CollisionComponent(Actor& owner);

	void Register() override;
	void Unregister() override;

	virtual bool Intersects(std::shared_ptr<CollisionComponent> other);
};

DECL_PTR(CollisionComponent);
