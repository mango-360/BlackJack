#pragma once

#include <string>
#include <algorithm>
#include <random>

#include "Engine.h"
#include "Presenter.h"
#include "Player.h"

class Board
{
public:
	Board();
	~Board();

	void init();
	void update();
	void draw();
	void destroy();

	DrawableWithValue m_chips[MAXCHIPS];
private:
	void initChips();
	void initCards();
	void drawChips();
	void drawCards();
	void initPlayingCards();

	void dealCards();

	SDL_Texture* m_background;

	DrawableWithValue m_cards[52];

	Drawable m_cardBack;

	bool betStage;

	Player m_player;

	vector<DrawableWithValue> m_playingCards;
};