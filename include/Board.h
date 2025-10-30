#pragma once

#include <string>
#include "Engine.h"
#include "Presenter.h"
#include "Player.h"

const int MAXCHIPS = 5;

class Board
{
public:
	Board();
	~Board();

	void init();
	void update();
	void draw();
	void destroy();
	void drawChips();
	void drawCards();
private:
	SDL_Texture* m_background;

	DrawableWithValue m_chips[MAXCHIPS];
	DrawableWithValue m_cards[52];

	Drawable m_dealButton;

	bool betStage;

	Player m_player;
};