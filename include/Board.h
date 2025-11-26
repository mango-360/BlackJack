#pragma once

#include <string>
#include <algorithm>
#include <random>

#include "Engine.h"
#include "Presenter.h"
#include "Player.h"
#include <chrono>

using namespace chrono;

class Board
{
public:
	Board();
	~Board();

	void init();
	void update();
	void draw();
	void destroy();
	void dealCardToPlayer();
	void dealCardToDealer();

	DrawableWithValue m_chips[MAXCHIPS];

	long long now_seconds;

	int moneyWon;
	int moneyLost;
	int totalMoney;
	int roundsPlayed;
	int roundsWon;

	float winRate;
private:
	void initChips();
	void initCards();
	void drawChips();
	void initPlayingCards();

	void dealInitialCards();
	void animateInitialCards();
	void winStage();

	SDL_Texture* m_background;

	DrawableWithValue m_cards[52];

	Drawable m_cardBack;
	Drawable m_youWin;
	Drawable m_dealerWins;

	bool betStage;
	bool dealStage;
	bool dealerDealStage;
	bool resultStage;
	bool winStageDone;

	Player m_player;

	Dealer m_dealer;

	vector<DrawableWithValue> m_playingCards;

	int m_winner;
};