#pragma once

#include "defines.h"
#include "TextField.h"
#include <string>

class Dealer {

public:
	Dealer();
	~Dealer();
	virtual void init();
	void addCard(DrawableWithValue card);
	void animateHand(int2 endPos);
	void drawHand();
	void drawPoints();
	void updatePoints();
	int getPoints();

	void subtractFirstCardPoints();

	vector<DrawableWithValue> m_hand;
	
	int2 m_cardStartPos;

	void calculatePoints();
protected:
	virtual void clearHand();
	void animateCardDeal(DrawableWithValue& card, int2 endPos);

	int m_points;
	int m_moveSpeed = 19;

	TextField m_pointsField;
};