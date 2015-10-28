#include "Arrow.h"
#include "Game.h"

IMPL_ACTOR(Arrow, Actor);

Arrow::Arrow(Game & game) : Actor(game){
	//Create a mesh component
	mMeshComponent = MeshComponent::Create(*this);
	auto & assetCache = game.GetAssetCache();
	auto mesh = assetCache.Load<Mesh>("Meshes/Arrow.itpmesh2");
	mMeshComponent->SetMesh(mesh);
}

void Arrow::BeginPlay(){
	UpdatePosition();
}
void Arrow::Tick(float deltaTime){
	UpdatePosition();
}

void Arrow::UpdatePosition(){

	//Position
	Vector3 unprojectVec = mGame.GetRenderer().Unproject(Vector3(0.0f, 150.0f, 0.6f));

	//Rotation
	auto shipPos = mShip ? mShip->GetPosition() : Vector3(0, 0, 0);
	auto facing = mCheckpointPos - shipPos;
	facing.Normalize();
	float angle = acos(Dot(Vector3::UnitX, facing));
	Vector3 axis = Cross(Vector3::UnitX, facing);
	Quaternion quat;
	if (axis.Length() < 0.01f){
		quat = Quaternion::Identity;
	}
	else {
		axis.Normalize();
		quat = Quaternion(axis, angle);
	}

	SetRotation(quat);
	SetPosition(unprojectVec);
}

