#pragma once

#include "defines.h"

class Dealer {

public:
	Dealer();
	~Dealer();
	virtual void init();
	void addCard(DrawableWithValue card);

protected:
	virtual void clearHand();
	void calculatePoints();

	int m_points;

	vector<DrawableWithValue> m_hand;
};