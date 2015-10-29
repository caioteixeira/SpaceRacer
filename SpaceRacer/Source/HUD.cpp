#include "HUD.h"
#include "Game.h"
#include "Font.h"
#include <string>
#include <sstream>
#include <iomanip>

IMPL_ACTOR(HUD, Actor);

HUD::HUD(Game & game) : Actor(game){
}

void HUD::BeginPlay(){
	auto & assetCache = mGame.GetAssetCache();
	
	//Timer
	auto timer = Actor::SpawnAttached(*this);
	mTimerFontComponent = FontComponent::Create(*timer);
	timer->SetPosition(Vector3(0.0f, 300.0f, 0.0f));

	//Score
	auto score = Actor::SpawnAttached(*this);
	mScoreFontComponent = FontComponent::Create(*score);
	score->SetPosition(Vector3(-300.0f, 300.0f, 0.0f));

	//Gameover
	auto gameover = Actor::SpawnAttached(*this);
	mGameOverFontComponent = FontComponent::Create(*gameover);
	gameover->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

	auto font = assetCache.Load<Font>("Fonts/Carlito-Regular.ttf");
	mTimerFontComponent->SetFont(font);
	mScoreFontComponent->SetFont(font);
	mGameOverFontComponent->SetFont(font);

	
}


void HUD::Tick(float deltaTime){
	float timer = mGame.GetGameMode()->GetTimer();
	int score = mGame.GetGameMode()->GetScore();

	std::string out = "Timer: ";
	std::ostringstream stream;

	//Timer
	stream << std::fixed << std::setprecision(2) << timer;
	out += stream.str();

	mTimerFontComponent->SetText(out, timer > 5.0f?Color::Green:Color::Red, 30);
	mTimerFontComponent->SetAlignment(FontComponent::AlignCenter);

	//Score
	stream = std::ostringstream();
	out = "Score: ";
	stream << score;
	out += stream.str();

	mScoreFontComponent->SetText(out, Color::Blue, 30);
	mScoreFontComponent->SetAlignment(FontComponent::AlignLeft);

	//GameOver
	if (gameover){
		out = "Gameover :(";
		mGameOverFontComponent->SetText(out, Color::Red, 40);
		mGameOverFontComponent->SetAlignment(FontComponent::AlignCenter);
	}
}

