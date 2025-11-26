#include "Board.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "World.h"
#include "Player.h"

extern World world;

Board::Board()
{
}

Board::~Board()
{
}

void Board::init()
{
	string configFile = "board.txt";
	
	string tmp, backgroundImg, player1configFile;

	fstream stream;

	stream.open(CONFIG_FOLDER + configFile);

	stream >> tmp >> backgroundImg;
	stream >> tmp >> player1configFile;
	stream >> tmp >> m_youWin;
	stream >> tmp >> m_dealerWins;

	stream.close();

	m_background = loadTexture(backgroundImg);
	m_youWin.texture = loadTexture(m_youWin.img);
	m_dealerWins.texture = loadTexture(m_dealerWins.img);

	initChips();
	
	initCards();
	
	betStage = true;
	dealStage = false;
	dealerDealStage = false;
	resultStage = false;
	winStageDone = false;

	m_player.init(player1configFile);
	m_dealer.init();

	moneyWon = 0;
	moneyLost = 0;
	totalMoney = 0;
	roundsPlayed = 0;
	roundsWon = 0;

	winRate = 0.0f;
}

void Board::initChips()
{
	for (int i = 0; i < MAXCHIPS; i++)
	{
		m_chips[i].texture = loadTexture("Chips\\Chip" + to_string(i + 1) + ".bmp");
		m_chips[i].rect = { 50 + i * 250, 800, 100, 100 };
		double x = i + 1;
		m_chips[i].value =
			(215.0 / 24.0) * pow(x, 4)
			- (325.0 / 4.0) * pow(x, 3)
			+ (6445.0 / 24.0) * pow(x, 2)
			- (1465.0 / 4.0) * x
			+ 175.0;
	}
}

void Board::initCards()
{
	m_cardBack.texture = loadTexture("Cards\\back_dark.bmp");

	// Starting rectangle for all cards (off-screen start position)
	SDL_Rect startRect = { -200, -280, 200, 280 };

	for (int i = 0; i < 13; i++)
	{
		m_cards[i].texture = loadTexture("Cards\\clubs_" + to_string(i + 2) + ".bmp");
		m_cards[i].rect = startRect;
		if (i == 9)
			m_cards[i].value = 11;
		else if (i > 9)
			m_cards[i].value = 10;
		else
			m_cards[i].value = i + 2;
	}
	for (int i = 0; i < 13; i++)
	{
		m_cards[i + 13].texture = loadTexture("Cards\\diamonds_" + to_string(i + 2) + ".bmp");
		m_cards[i + 13].rect = startRect;
		if (i == 9)
			m_cards[i + 13].value = 11;
		else if (i > 9)
			m_cards[i + 13].value = 10;
		else
			m_cards[i + 13].value = i + 2;
	}
	for (int i = 0; i < 13; i++)
	{
		m_cards[i + 26].texture = loadTexture("Cards\\hearts_" + to_string(i + 2) + ".bmp");
		m_cards[i + 26].rect = startRect;
		if (i == 9)
			m_cards[i + 26].value = 11;
		else if (i > 9)
			m_cards[i + 26].value = 10;
		else
			m_cards[i + 26].value = i + 2;
	}
	for (int i = 0; i < 13; i++)
	{
		m_cards[i + 39].texture = loadTexture("Cards\\spades_" + to_string(i + 2) + ".bmp");
		m_cards[i + 39].rect = startRect;
		if (i == 9)
			m_cards[i + 39].value = 11;
		else if (i > 9)
			m_cards[i + 39].value = 10;
		else
			m_cards[i + 39].value = i + 2;
	}

	initPlayingCards();
}

void Board::update()
{
	if (betStage && (InputManager::isKeyPressed(SDL_SCANCODE_Q) || (m_player.getMoney() <= 0 && m_player.getBet() <= 0)))
	{
		totalMoney = m_player.getMoney();
		winRate = (roundsPlayed > 0) ? ((float)roundsWon / (float)roundsPlayed) * 100.0f : 0.0f;
		world.m_stateManager.changeState(WIN_SCREEN);
		return;
	}

	if (isMouseInRect(m_player.m_dealButton.rect) && InputManager::isMousePressed()) betStage = false;

	if (betStage)
	{
		m_player.betUpdate();
		return;
	}

	if (isMouseInRect(m_player.m_dealButton.rect) && InputManager::isMousePressed() && !dealStage)
	{
		if (!dealStage)
		{
			dealInitialCards();
			m_dealer.subtractFirstCardPoints();
		}

		dealStage = true;
	}

	if (InputManager::isMousePressed() && isMouseInRect(m_player.m_standButton.rect) && dealStage && m_player.m_allCardsDealt && m_dealer.m_allCardsDealt)
	{
		dealStage = false;
		dealerDealStage = true;
		m_dealer.m_hand[0].texture = m_dealer.m_hand[0].saveTexture;
		m_dealer.calculatePoints();
		m_dealer.updatePoints();
	}

	if (m_player.getPoints() >= 21 && dealStage && m_dealer.m_allCardsDealt && m_player.m_allCardsDealt)
	{
		dealStage = false;
		resultStage = true;
		m_dealer.m_hand[0].texture = m_dealer.m_hand[0].saveTexture;
		m_dealer.calculatePoints();
		m_dealer.updatePoints();
	}

	if (dealStage)
	{
		m_player.dealUpdate();
		animateInitialCards();
		m_dealer.updatePoints();

		if (m_dealer.m_hand[1].isDealt)
		{
			m_dealer.animateHand(m_dealer.m_cardStartPos);
			m_player.animateHand(m_player.m_cardStartPos);
		}
		return;
	}

	if (dealerDealStage)
	{
		if (m_dealer.getPoints() < 17 && m_dealer.getPoints() < m_player.getPoints())
		{
			dealCardToDealer();
			m_dealer.calculatePoints();
			m_dealer.updatePoints();
		}
		else if(m_dealer.m_allCardsDealt)
		{
			dealerDealStage = false;
			resultStage = true;
		}
		m_dealer.animateHand(m_dealer.m_cardStartPos);
		return;
	}

	if(resultStage)
	{
		roundsPlayed++;

		if ((m_player.getPoints() > m_dealer.getPoints() && m_player.getPoints() <= 21) || m_dealer.getPoints() > 21)
		{
			m_winner = 1;
			moneyWon += m_player.getBet();
			roundsWon++;
		}
		if ((m_player.getPoints() < m_dealer.getPoints() && m_dealer.getPoints() <= 21) || m_player.getPoints() > 21)
		{
			m_winner = 2;
			moneyLost += m_player.getBet();
		}
		if(m_player.getPoints() == m_dealer.getPoints())
		{
			m_winner = 0;
		}

		winStage();
	}
}

void Board::draw()
{
	drawObject(m_background);

	if (betStage)
	{
		drawChips();
		m_player.betDraw();
		return;
	}

	if (dealStage)
	{
		m_player.dealDraw();
		m_dealer.drawPoints();

		m_player.drawHand();
		m_dealer.drawHand();
		return;
	}

	if(dealerDealStage)
	{
		m_player.drawResultStage();
		m_dealer.drawPoints();

		m_player.drawHand();
		m_dealer.drawHand();
	}

	if(resultStage)
	{
		m_player.drawResultStage();
		m_dealer.drawPoints();

		m_player.drawHand();
		m_dealer.drawHand();
	}

	if(winStageDone)
	{
		int timer = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();

		m_player.drawResultStage();
		m_dealer.drawPoints();

		m_player.drawHand();
		m_dealer.drawHand();

		if (m_winner == 1)
		{
			drawObject(m_youWin);
		}
		if (m_winner == 2)
		{
			drawObject(m_dealerWins);
		}

		if (m_winner == 0)
		{
			drawObject(m_youWin);
			drawObject(m_dealerWins);
		}

		if(timer - now_seconds >= 3)
		{
			winStageDone = false;
			betStage = true;
			m_player.clearHand();
			m_dealer.clearHand();
		}
	}
}

void Board::destroy()
{
	SDL_DestroyTexture(m_background);

	m_player.destroy();
}

void Board::winStage()
{
	if (m_winner == 1)
	{
		m_player.addMoney();
		m_player.addMoney();
		cout << "Player wins!" << endl;
	}

	if (m_winner == 2)
	{
		cout << "Dealer wins!" << endl;
	}

	if (m_winner == 0)
	{
		m_player.addMoney();
		cout << "It's a tie!" << endl;
	}

	resultStage = false;
	winStageDone = true;

	now_seconds = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
}

void Board::drawChips()
{
	for (int i = 0; i < MAXCHIPS; i++)
	{
		drawObject(m_chips[i]);
	}
}

void Board::initPlayingCards()
{
	for(int j = 0; j < 5; j++)
	{
		for(int i = 0; i < 52; i++)
		{
			m_playingCards.push_back(m_cards[i]);
		}
	}

	random_device rd;
	mt19937 g(rd());

	shuffle(m_playingCards.begin(), m_playingCards.end(), g);
}

void Board::dealInitialCards()
{
	dealCardToPlayer();
	dealCardToDealer();

	m_dealer.m_hand[0].saveTexture = m_dealer.m_hand[0].texture;
	m_dealer.m_hand[0].texture = m_cardBack.texture;

	dealCardToPlayer();
	dealCardToDealer();
}

void Board::animateInitialCards()
{
	if (!m_player.m_hand[0].isDealt)
	{
		m_player.animateHand(m_player.m_cardStartPos);
		return;
	}
	if(!m_dealer.m_hand[0].isDealt)
	{
		m_dealer.animateHand(m_dealer.m_cardStartPos);
		return;
	}
	if(!m_player.m_hand[1].isDealt)
	{
		m_player.animateHand(m_player.m_cardStartPos);
		return;
	}
	if(!m_dealer.m_hand[1].isDealt)
	{
		m_dealer.animateHand(m_dealer.m_cardStartPos);
		return;
	}
}

void Board::dealCardToPlayer()
{
	m_player.addCard(m_playingCards.back());
	m_playingCards.pop_back();
	m_player.m_allCardsDealt = false;
}

void Board::dealCardToDealer()
{
	m_dealer.addCard(m_playingCards.back());
	m_playingCards.pop_back();
	m_dealer.m_allCardsDealt = false;
}
