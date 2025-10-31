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
	
	string tmp, backgroundImg, dealButtonImg;

	fstream stream;

	stream.open(CONFIG_FOLDER + configFile);

	stream >> tmp >> backgroundImg;
	stream >> tmp >> dealButtonImg >> m_dealButton.rect.x >> m_dealButton.rect.y >> m_dealButton.rect.w >> m_dealButton.rect.h;	

	stream.close();

	m_background = loadTexture(backgroundImg);
	m_dealButton.texture = loadTexture(dealButtonImg);

	for(int i = 0; i < MAXCHIPS; i++)
	{
		m_chips[i].texture = loadTexture("Chips\\Chip" + to_string(i + 1) + ".bmp");
		m_chips[i].rect = {50 + i * 250, 800, 100, 100 };
		double x = i + 1;
		m_chips[i].value =
				(215.0 / 24.0) * pow(x, 4)
				- (325.0 / 4.0) * pow(x, 3)
				+ (6445.0 / 24.0) * pow(x, 2)
				- (1465.0 / 4.0) * x
				+ 175.0;
	}
	for(int i = 0; i < 13; i ++)
	{
		m_cards[i].texture = loadTexture("Cards\\clubs_" + to_string(i + 2) + ".bmp");
		m_cards[i].rect = {i * 150, 0, 100, 140 };
		if (i == 9) 
			m_cards[i].value = 11;
		else if (i > 9)
			m_cards[i].value = 10;
		else
			m_cards[i].value = i + 2;
	}
	for(int i = 0; i < 13; i ++)
	{
		m_cards[i + 13].texture = loadTexture("Cards\\diamonds_" + to_string(i + 2) + ".bmp");
		m_cards[i + 13].rect = {i * 150, 200, 100, 140 };
		if (i == 9)
			m_cards[i + 13].value = 11;
		else if (i > 9)
			m_cards[i + 13].value = 10;
		else
			m_cards[i + 13].value = i + 2;
	}
	for(int i = 0; i < 13; i ++)
	{
		m_cards[i + 26].texture = loadTexture("Cards\\hearts_" + to_string(i + 2) + ".bmp");
		m_cards[i + 26].rect = {i * 150, 400, 100, 140 };
		if (i == 9)
			m_cards[i + 26].value = 11;
		else if (i > 9)
			m_cards[i + 26].value = 10;
		else
			m_cards[i + 26].value = i + 2;
	}
	for(int i = 0; i < 13; i ++)
	{
		m_cards[i + 39].texture = loadTexture("Cards\\spades_" + to_string(i + 2) + ".bmp");
		m_cards[i + 39].rect = {i * 150, 600, 100, 140 };
		if (i == 9)
			m_cards[i + 39].value = 11;
		else if (i > 9)
			m_cards[i + 39].value = 10;
		else
			m_cards[i + 39].value = i + 2;
	}
	betStage = true;

	m_playerMoneyField.init("p1money.txt");
	m_playerMoneyField.m_needToDrawBackground = false;
}

void Board::update()
{
	if (isMouseInRect(m_dealButton.rect) && InputManager::isMousePressed())
	{
		betStage = false;
		printf("Mouse is pressed on deal button\n");
	}
	if (betStage)
	{
		for (int i = 0; i < MAXCHIPS; i++)
		{
			if (isMouseInRect(m_chips[i].rect) && InputManager::isMousePressed)
			{
				m_player.bet(m_chips[i].value);
				SoundManager::playSound(X_PLACE);
			}
		}
	}
	if (!betStage) printf("Button pressed");

	m_playerMoneyField.update();
	m_playerMoneyField.setText("Money: $" + to_string(1324));
}

void Board::draw()
{
	drawObject(m_background);
	drawObject(m_dealButton);
	drawChips();
	//drawCards();

	m_playerMoneyField.draw();
}

void Board::destroy()
{
	SDL_DestroyTexture(m_background);
}

void Board::drawChips()
{
	for (int i = 0; i < MAXCHIPS; i++)
	{
		drawObject(m_chips[i]);
	}
}

void Board::drawCards()
{
	for (int i = 0; i < 52; i++)
	{
		drawObject(m_cards[i]);
		if (InputManager::isMousePressed() && isMouseInRect(m_cards[i].rect)) printf("Card value: %d\n", m_cards[i].value);
	}
}