#include "GameMode.h"
#include "Game.h"
#include <vector>
#include "Random.h"


IMPL_ACTOR(GameMode, Actor);

GameMode::GameMode(Game & game) : Actor(game){
}


void GameMode::BeginPlay(){
	//Player
	mShip = Ship::Spawn(mGame);
	mShip->SetRotation(Quaternion(mShip->GetWorldTransform().GetZAxis(), Math::PiOver2));

	//HUD
	mHUD = HUD::Spawn(mGame);
	mArrow = Arrow::Spawn(mGame);
	mArrow->SetShipPointer(mShip);

	//Asteroids
	const int NUM_ASTEROIDS = 500;

	Vector3 minVec(-5000.0f, -5000.0f, -5000.0f);
	Vector3 maxVec(5000.f, 5000.f, 5000.0f);
	for (int i = 0; i < NUM_ASTEROIDS; i++)
	{
		auto ast = Asteroid::Spawn(mGame);
		ast->SetPosition(Random::GetVector(minVec, maxVec));
		ast->SetScale(0.1f);
	}

	//Audio
	mAudio = AudioComponent::Create(*this);
	mCollectSound = mGame.GetAssetCache().Load<Sound>("Sounds/Checkpoint.wav");

	/*Spawn Initial Checkpoint*/
	SpawnCheckpoint();

}

/*Handle Checkpoint collection*/
void GameMode::CollectCheckpoint(){
	/*Collection audio*/
	mAudio->PlaySound(mCollectSound);


	/*Spawn new checkpoint*/
	SpawnCheckpoint();
}

/*Set GameOver*/
void GameMode::GameOver(){
	isRunning = false;
	SetIsPaused(true);
	mShip->SetIsPaused(true);
	mHUD->SetGameover();
}

/*Spawns a Checkpoint actor in a random location near the ship*/
void GameMode::SpawnCheckpoint(){
	mScore += 100;
	float distanceFactor = 1000.0f + (mScore);
	Vector3 shipPos = mShip->GetWorldTransform().GetTranslation();
	Vector3 minVec = Vector3(shipPos.x - distanceFactor, shipPos.y - distanceFactor, shipPos.z - distanceFactor);
	Vector3 maxVec = Vector3(shipPos.x + distanceFactor, shipPos.y + distanceFactor, shipPos.z + distanceFactor);
	auto checkpoint = Checkpoint::Spawn(mGame);

	Vector3 pos = Random::GetVector(minVec, maxVec);
	checkpoint->SetPosition(pos);

	mArrow->SetCheckpointPosition(pos);

	AddTime();
}

void GameMode::Tick(float deltaTime){
	if (mTimer <= 0.0f){
		mTimer = 0.0f;
		GameOver();
	}

	if (isRunning){
		//Update timer
		mTimer -= deltaTime;
	}
}
