// Component.h
// Base class for all Components

#pragma once
#include <memory>
#include "Object.h"

// Forward declaration
class Actor;

class Component : public Object
{
public:
	enum UpdateType
	{
		PreTick,
		PostTick
	};
private:
	DECL_COMPONENT(Component, Object);
public:
	Component(Actor& owner);
	virtual ~Component();

	virtual void Tick(float deltaTime);

	virtual void Register();
	virtual void Unregister();

	Actor& GetOwner() { return mOwner; }
protected:
	Actor& mOwner;
};

DECL_PTR(Component);
