#include "Asteroid.h"
#include "Game.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "Random.h"
#include "SphereCollision.h"
IMPL_ACTOR(Asteroid, Actor);


Asteroid::Asteroid(Game & game) : Actor(game)
{
	//Create a mesh component
	auto meshComponent = MeshComponent::Create(*this);
	auto& assetCache = game.GetAssetCache();
	auto mesh = assetCache.Load<Mesh>("Meshes/AsteroidMesh.itpmesh2");
	meshComponent->SetMesh(mesh);

	//Set Random rotation
	SetRotation(Quaternion(GetWorldTransform().GetZAxis(), Random::GetFloatRange(0.0f, Math::TwoPi)));

	//Set Movement
	auto move = MoveComponent::Create(*this, Component::PreTick);
	move->SetLinearSpeed(20.0f);
	move->SetLinearAxis(1.0f);
}