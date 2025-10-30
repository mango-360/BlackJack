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
	
	string tmp, backgroundImg;

	fstream stream;

	stream.open(CONFIG_FOLDER + configFile);

	stream >> tmp >> backgroundImg;

	stream.close();

	m_background = loadTexture(backgroundImg);

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
}

void Board::update()
{
}

void Board::draw()
{
	drawObject(m_background);
	drawChips();

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