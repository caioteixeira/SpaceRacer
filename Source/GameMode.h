#pragma once
#include <vector>
#include "AudioComponent.h"
#include "HUD.h"
#include "Asteroid.h"
#include "Ship.h"
#include "Checkpoint.h"
#include "AudioComponent.h"
#include "Arrow.h"

class GameMode : public Actor{
	DECL_ACTOR(GameMode, Actor);

public:
	GameMode(Game & game);
	void BeginPlay() override;
	void CollectCheckpoint();
	float GetTimer() { return mTimer; };
	int GetScore() { return mScore; };
	void Tick(float deltaTime) override;
private:
	//HUD
	HUDPtr mHUD;
	ArrowPtr mArrow;

	void AddTime() { mTimer += 10.0f; };
	void GameOver();

	float mTimer = 5.0f;
	int mScore = -100;

	ShipPtr mShip;
	void SpawnCheckpoint();
	AudioComponentPtr mAudio;
	SoundPtr mCollectSound;

	bool isRunning = true;
};
DECL_PTR(GameMode);