#pragma once

#include "defines.h"

class Dealer {

public:
	Dealer();
	~Dealer();
	virtual void init();
	void addCard(DrawableWithValue card);
	void animateHand(int2 endPos);
	void drawHand();

	vector<DrawableWithValue> m_hand;

	int2 m_cardStartPos;

protected:
	virtual void clearHand();
	void calculatePoints();
	void animateCardDeal(DrawableWithValue& card, int2 endPos);

	int m_points;
	int m_moveSpeed = 5;
};