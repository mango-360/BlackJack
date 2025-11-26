#pragma once

#include "defines.h"
#include "State.h"
#include "Presenter.h"
#include "InputManager.h"
#include "TextField.h"

class WinScreen : public State
{
public:
	WinScreen();
	~WinScreen();

	void init();
	void run();
	void destroy();

private:
	SDL_Texture* m_background;

	Drawable m_playAgainButton;

	TextField m_moneyWonField;
	TextField m_moneyLostField;
	TextField m_totalMoneyField;
	TextField m_roundsPlayedField;
	TextField m_winRateField;

	int m_winner;
	int moneyWon;
	int moneyLost;
	int totalMoney;
	int roundsPlayed;

	float winRate;
};