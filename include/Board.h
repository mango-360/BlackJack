#pragma once

#include <string>
#include <algorithm>
#include <random>

#include "Engine.h"
#include "Presenter.h"
#include "Player.h"
#include "TextField.h"

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
private:
	void initChips();
	void initCards();
	void drawChips();
	void drawCards();
	void initPlayingCards();

	void dealCards();

	SDL_Texture* m_background;

	DrawableWithValue m_chips[MAXCHIPS];
	DrawableWithValue m_cards[52];

	Drawable m_dealButton;
	Drawable m_cardBack;

	bool betStage;

	Player m_player;

	TextField m_playerMoneyField;
	TextField m_playerBetField;

	vector<DrawableWithValue> m_playingCards;
};