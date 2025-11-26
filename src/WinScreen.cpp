#include "WinScreen.h"
#include "World.h"

extern World world;

WinScreen::WinScreen()
{

}

WinScreen::~WinScreen()
{

}

void WinScreen::init()
{
	fstream stream;

	string tmp, WinScreenBackground;

	stream.open(CONFIG_FOLDER + "WinScreen.txt");

	stream >> tmp >> WinScreenBackground;
	stream >> tmp >> m_playAgainButton;

	stream.close();

	m_background = loadTexture(WinScreenBackground);
	m_playAgainButton.texture = loadTexture(m_playAgainButton.img);

	moneyWon = world.m_stateManager.m_game->m_board.moneyWon;
	moneyLost = world.m_stateManager.m_game->m_board.moneyLost;
	totalMoney = world.m_stateManager.m_game->m_board.totalMoney;
	roundsPlayed = world.m_stateManager.m_game->m_board.roundsPlayed;
	winRate = world.m_stateManager.m_game->m_board.winRate;

	m_moneyWonField.init("winScreenMoneyWon.txt");
	m_moneyWonField.setText(to_string(moneyWon));

	m_moneyLostField.init("winScreenMoneyLost.txt");
	m_moneyLostField.setText(to_string(moneyLost));

	m_totalMoneyField.init("winScreenTotalMoney.txt");
	m_totalMoneyField.setText( to_string(totalMoney));

	m_roundsPlayedField.init("winScreenRoundsPlayed.txt");
	m_roundsPlayedField.setText(to_string(roundsPlayed));

	m_winRateField.init("winScreenWinRate.txt");
	m_winRateField.setText(format("{:.2f}", winRate) + "%");
}

void WinScreen::run()
{
	drawObject(m_background);
	drawObject(m_playAgainButton);

	m_moneyWonField.draw();
	m_moneyLostField.draw();
	m_totalMoneyField.draw();
	m_roundsPlayedField.draw();
	m_winRateField.draw();

	if (InputManager::isMousePressed() && isMouseInRect(m_playAgainButton.rect))
	{
		world.m_stateManager.changeState(GAME_STATE::GAME);
	}
}

void WinScreen::destroy()
{
	SDL_DestroyTexture(m_background);
}