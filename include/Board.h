#pragma once

#include <string>
#include "Engine.h"
#include "Presenter.h"

const int MAXCHIPS = 5;

class Board
{
public:
	Board();
	~Board();

	void init();
	void update();
	void draw();
	void destroy();
	void drawChips();
private:
	SDL_Texture* m_background;

	Chip m_chips[MAXCHIPS];
};