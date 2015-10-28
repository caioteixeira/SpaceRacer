#include "Checkpoint.h"
#include "Game.h"

IMPL_ACTOR(Checkpoint, Actor);

Checkpoint::Checkpoint(Game & game) : Actor(game){
	//Create a mesh component
	mMeshComponent = MeshComponent::Create(*this);
	auto & assetCache = game.GetAssetCache();
	auto mesh = assetCache.Load<Mesh>("Meshes/Checkpoint.itpmesh2");
	mMeshComponent->SetMesh(mesh);

	//Set collision
	mColl = SphereCollision::Create(*this);
	mColl->RadiusFromMesh(mesh);
	mColl->SetScale(0.75f);
}

void Checkpoint::BeginTouch(Actor & other){

}


