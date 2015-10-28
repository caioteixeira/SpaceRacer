#pragma once
#include "Actor.h"
#include "InputComponent.h"
#include "MeshComponent.h"
#include "AudioComponent.h"
#include "SphereCollision.h"
#include "CameraComponent.h"

class Ship : public Actor
{
	DECL_ACTOR(Ship, Actor);

public:
	Ship(Game & game);
	InputComponentPtr GetInputComponent();
	AudioComponentPtr GetAudioComponent();
	SphereCollisionPtr GetCollision() { return mColl; };
	void EnableCollision();
	void Tick(float deltaTime) override;
	void BeginPlay() override;
	void BeginTouch(Actor & other) override;
	void OnRespawnShip();
	void OnRecenter();
private:
	InputComponentPtr mInput;
	MeshComponentPtr mMeshComponent;
	AudioComponentPtr mAudio;
	CameraComponentPtr mCamera;
	TexturePtr mTexture1;
	TexturePtr mTexture2;
	SoundPtr mEngineSound;
	SoundCue mEngineCue;
	SphereCollisionPtr mColl;
	bool mCollisionIsEnabled;
};
DECL_PTR(Ship);