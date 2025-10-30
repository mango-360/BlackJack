#pragma once

#include "defines.h"

class Player {
public:
	Player();
	~Player();

	void init();
	void update();
	void destroy();
	void addCard(DrawableWithValue card);
	void bet(int amount);
	void clearHand();
	void addMoney();
	void calculatePoints();

	int m_money;
	int m_bet;
	int m_points;

	vector<DrawableWithValue> m_hand;
};