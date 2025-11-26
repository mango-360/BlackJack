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
	void drawResultStage();
	void destroy();
	void addMoney();
	void clearHand();
	int getMoney() { return m_money; }
	int getBet() { return m_bet; }

	Drawable m_dealButton;
	Drawable m_standButton;

private:
	void bet();
	void drawChipsBet();
	void removeChipBet();
	void clearBet();
	void allInBet();


	int m_money;
	int m_bet;

	pair<int,DrawableWithValue> m_ChipsBet[MAXCHIPS];

	TextField m_playerMoneyField;
	TextField m_playerBetField;
	
	Drawable m_allInButton;
	Drawable m_clearBetButton;
	Drawable m_hitButton;
};