#include "Ship.h"
#include "Game.h"
#include "Sound.h"
#include "Asteroid.h"
#include "Mesh.h"
#include "Checkpoint.h"


IMPL_ACTOR(Ship, Actor);

Ship::Ship(Game & game) : Actor(game)
{
	//Create a mesh component
	mMeshComponent = MeshComponent::Create(* this);
	auto& assetCache = game.GetAssetCache();
	auto mesh = assetCache.Load<Mesh>("Meshes/PlayerShip.itpmesh2");
	mMeshComponent->SetMesh(mesh);

	//Load the laser sound
	mAudio = AudioComponent::Create(*this);
	mEngineSound = assetCache.Load<Sound>("Sounds/ShipEngine.wav");
	mEngineCue = GetAudioComponent()->PlaySound(mEngineSound,true);
	mEngineCue.Pause();

	//Set Movement
	mInput = InputComponent::Create(*this, Component::PreTick);
	mInput->SetLinearSpeed(150.0f);
	mInput->SetYawSpeed(Math::TwoPi);
	
	//Set SphereCollision
	mColl = SphereCollision::Create(*this);
	mColl->RadiusFromMesh(mesh);
	mColl->SetScale(0.5f);
	mCollisionIsEnabled = false;

	SetScale(0.5f);

	//Set Camera
	mCamera = CameraComponent::Create(*this);
	mCamera->SetMoveComponent(mInput);
}

void Ship::BeginPlay()
{
	mInput->BindYawAxis("Yaw");
	mInput->BindPitchAxis("Pitch");
	mInput->BindLinearAxis("Move");

	mGame.GetInput().BindAction("Recenter", IE_Pressed, this, &Ship::OnRecenter);

	mCamera->SnapToIdeal();
}

InputComponentPtr Ship::GetInputComponent()
{
	return mInput;
}

AudioComponentPtr Ship::GetAudioComponent()
{
	return mAudio;
}

void Ship::OnRecenter(){
	mCamera->SnapToIdeal();
	SetRotation(Quaternion::Identity);
}

void Ship::Tick(float deltaTime)
{

	//Update the Texture and turn on/off the engine sound
	if (mInput->GetLinearAxis() != 0.0f)
	{
		mEngineCue.Resume();
	}
	else
	{
		mEngineCue.Pause();
	}

	Super::Tick(deltaTime);
}

void Ship::BeginTouch(Actor & other){
	//Handle checkpoint collection
	if (other.IsAlive() && IsA<Checkpoint>(other)){
		other.SetIsAlive(false);
		mGame.GetGameMode()->CollectCheckpoint();
	}
}