#pragma once
#include "Actor.h"
#include "MeshComponent.h"

class Arrow : public Actor {
	DECL_ACTOR(Arrow, Actor);
public:
	Arrow(Game & game);
	void SetShipPointer(ActorPtr ship) { mShip = ship; };
	void SetCheckpointPosition(Vector3 pos) { mCheckpointPos = pos; };
	void BeginPlay() override;
	void Tick(float deltaTime) override;
private:
	MeshComponentPtr mMeshComponent;
	ActorPtr mShip = nullptr;
	Vector3 mCheckpointPos = Vector3::UnitX;
	void UpdatePosition();
};

DECL_PTR(Arrow);