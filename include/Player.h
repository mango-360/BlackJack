#pragma once

class Player {
public:
	Player();
	~Player();

	void init();
	void update();
	void destroy();

	int m_money;
	int m_bet;
};