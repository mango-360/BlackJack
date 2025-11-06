#pragma once

#include "defines.h"
#include "TextField.h"
#include "Dealer.h"

class Player : public Dealer {
public:
	Player();
	~Player();

	void init(string configFile);
	void betUpdate();
	void dealUpdate();
	void betDraw();
	void dealDraw();
	void destroy();

	Drawable m_dealButton;

private:
	void bet();
	void clearHand();
	void addMoney();
	void drawChipsBet();
	void removeChipBet();
	void clearBet();
	void allInBet();


	int m_money;
	int m_bet;
	int m_points;


	pair<int,DrawableWithValue> m_ChipsBet[MAXCHIPS];

	TextField m_playerMoneyField;
	TextField m_playerBetField;
	
	Drawable m_allInButton;
	Drawable m_clearBetButton;
	Drawable m_hitButton;
	Drawable m_standButton;
};