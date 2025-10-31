#pragma once

#include "defines.h"

class Dealer {
	public:
	Dealer();
	~Dealer();
	void init();
	void addCard(DrawableWithValue card);
	void clearHand();
	void calculatePoints();

	int m_points;

	vector<DrawableWithValue> m_hand;
};