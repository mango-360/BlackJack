#pragma once

#include "defines.h"

class Dealer {
	public:
	Dealer();
	~Dealer();
	void init();
	void update();
	void destroy();

	int m_points;

	vector<DrawableWithValue> m_hand;
};