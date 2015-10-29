#pragma once
#include "Actor.h"
#include "FontComponent.h"

class HUD : public Actor {
	DECL_ACTOR(HUD, Actor);
public:
	HUD(Game & game);
	void Tick(float deltaTime) override;
	void BeginPlay() override;
	void SetGameover() { gameover = true; };
private:
	FontComponentPtr mTimerFontComponent;
	FontComponentPtr mScoreFontComponent;
	FontComponentPtr mGameOverFontComponent;
	bool gameover = false;
};

DECL_PTR(HUD);