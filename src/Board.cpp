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

	stream.close();

	m_background = loadTexture(backgroundImg);

	initChips();
	
	initCards();
	
	betStage = true;
	dealStage = false;

	m_player.init(player1configFile);
	m_dealer.init();
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

	for (int i = 0; i < 13; i++)
	{
		m_cards[i].texture = loadTexture("Cards\\clubs_" + to_string(i + 2) + ".bmp");
		m_cards[i].rect = { 0, 0, 100, 140 };
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
		m_cards[i + 13].rect = { 0, 0, 100, 140 };
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
		m_cards[i + 26].rect = { 0, 0, 100, 140 };
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
		m_cards[i + 39].rect = { 0, 0, 100, 140 };
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
		}

		dealStage = true;
	}

	if (dealStage)
	{
		m_player.dealUpdate();
		animateInitialCards();
		return;
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

		m_player.drawHand();
		m_dealer.drawHand();
	}
}

void Board::destroy()
{
	SDL_DestroyTexture(m_background);

	m_player.destroy();
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
}

void Board::dealCardToDealer()
{
	m_dealer.addCard(m_playingCards.back());
	m_playingCards.pop_back();

}
