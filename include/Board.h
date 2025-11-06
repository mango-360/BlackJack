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

	void dealInitialCards();
	void animateInitialCards();
	void dealCardToPlayer();
	void dealCardToDealer();

	SDL_Texture* m_background;

	DrawableWithValue m_cards[52];

	Drawable m_cardBack;

	bool betStage;
	bool dealStage;

	Player m_player;

	Dealer m_dealer;

	vector<DrawableWithValue> m_playingCards;
};