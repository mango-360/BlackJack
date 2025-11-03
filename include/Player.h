#pragma once

#include "defines.h"
#include "TextField.h"
#include "Dealer.h"

class Player : public Dealer {
public:
	Player();
	~Player();

	void init(string configFile);
	void update();
	void draw();
	void destroy();

	Drawable m_dealButton;

private:
	void bet();
	void clearHand();
	void addMoney();
	void drawChipsBet();
	void removeChipBet();
	void resetChipsBet();


	int m_money;
	int m_bet;
	int m_points;

	vector<DrawableWithValue> m_hand;

	pair<int,DrawableWithValue> m_ChipsBet[MAXCHIPS];

	TextField m_playerMoneyField;
	TextField m_playerBetField;
};