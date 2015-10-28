#pragma once
#include "Actor.h"
#include "MeshComponent.h"
#include "SphereCollision.h"

class Checkpoint : public Actor {
	DECL_ACTOR(Checkpoint, Actor);
public:
	Checkpoint(Game & game);
	SphereCollisionPtr GetCollision() { return mColl; };
	void BeginTouch(Actor & other) override;
private:
	MeshComponentPtr mMeshComponent;
	SphereCollisionPtr mColl;
};

DECL_PTR(Checkpoint);