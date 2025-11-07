#include "World.h"

extern World world;

Player::Player()
{
}

Player::~Player()
{
}

void Player::init(string configFile)
{
	string tmp, dealButtonImg, AllInImg, clearBetImg, hitButtonImg, standButtonImg;

	fstream stream;

	stream.open(CONFIG_FOLDER + configFile);

	stream >> tmp >> dealButtonImg >> m_dealButton.rect.x >> m_dealButton.rect.y >> m_dealButton.rect.w >> m_dealButton.rect.h;
	stream >> tmp >> AllInImg >> m_allInButton.rect.x >> m_allInButton.rect.y >> m_allInButton.rect.w >> m_allInButton.rect.h;
	stream >> tmp >> clearBetImg >> m_clearBetButton.rect.x >> m_clearBetButton.rect.y >> m_clearBetButton.rect.w >> m_clearBetButton.rect.h;
	stream >> tmp >> hitButtonImg >> m_hitButton.rect.x >> m_hitButton.rect.y >> m_hitButton.rect.w >> m_hitButton.rect.h;
	stream >> tmp >> standButtonImg >> m_standButton.rect.x >> m_standButton.rect.y >> m_standButton.rect.w >> m_standButton.rect.h;

	stream.close();

	m_dealButton.texture = loadTexture(PLAYER_FOLDER + dealButtonImg);
	m_allInButton.texture = loadTexture(PLAYER_FOLDER + AllInImg);
	m_clearBetButton.texture = loadTexture(PLAYER_FOLDER + clearBetImg);
	m_hitButton.texture = loadTexture(PLAYER_FOLDER + hitButtonImg);
	m_standButton.texture = loadTexture(PLAYER_FOLDER + standButtonImg);

	m_playerMoneyField.init("p1money.txt");
	m_playerMoneyField.m_needToDrawBackground = false;
	m_playerBetField.init("p1bet.txt");
	m_playerBetField.m_needToDrawBackground = false;


	m_money = 1000;
	m_bet = 0;

	for (int i = 0; i < MAXCHIPS; i++)
	{
		DrawableWithValue tmpChip = world.m_stateManager.m_game->m_board.m_chips[i];

		SDL_Rect tmpRect = { (Presenter::m_SCREEN_WIDTH - 380) / 2 + i * 70, (Presenter::m_SCREEN_HEIGHT - tmpChip.rect.h) / 2, tmpChip.rect.w, tmpChip.rect.h }; 

		m_ChipsBet[i] = { 0, tmpChip };
		m_ChipsBet[i].second.rect = tmpRect;
	}

	m_cardStartPos = { 600, 750 };

	m_pointsField.init("playerPoints.txt");
}

void Player::betUpdate()
{
	bet();
	removeChipBet();

	if(isMouseInRect(m_allInButton.rect) && InputManager::isMousePressed())
	{
		allInBet();
	}

	if(isMouseInRect(m_clearBetButton.rect) && InputManager::isMousePressed())
	{
		clearBet();
	}

	m_playerMoneyField.update();
	m_playerMoneyField.setText("Money: $" + to_string(m_money));
	m_playerBetField.update();
	m_playerBetField.setText("$" + to_string(m_bet));
}

void Player::betDraw()
{
	drawChipsBet();
	drawObject(m_dealButton);
	drawObject(m_allInButton);
	drawObject(m_clearBetButton);
	m_playerMoneyField.draw();
	m_playerBetField.draw();
}

void Player::dealUpdate()
{
	if(InputManager::isMousePressed() && isMouseInRect(m_hitButton.rect))
	{
		world.m_stateManager.m_game->m_board.dealCardToPlayer();
		SoundManager::playSound(X_PLACE);
	}

	m_pointsField.setText("Player: " + to_string(m_points));
	m_pointsField.update();
}

void Player::dealDraw()
{
	drawChipsBet();

	drawObject(m_hitButton);
	drawObject(m_standButton);

	m_playerMoneyField.draw();
	m_playerBetField.draw();
	m_pointsField.draw();
}

void Player::drawResultStage()
{
	drawChipsBet();

	m_playerMoneyField.draw();
	m_playerBetField.draw();
	m_pointsField.draw();
}

void Player::destroy()
{
	for(DrawableWithValue texture : m_hand)
	{
		SDL_DestroyTexture(texture.texture);
	}

	for(int i = 0; i < MAXCHIPS; i++)
	{
		SDL_DestroyTexture(m_ChipsBet[i].second.texture);
	}

	SDL_DestroyTexture(m_dealButton.texture);
	m_playerMoneyField.destroy();
	m_playerBetField.destroy();
}

void Player::bet()
{
	for (int i = 0; i < MAXCHIPS; i++)
	{
		DrawableWithValue chip = world.m_stateManager.m_game->m_board.m_chips[i];
		if (isMouseInRect(chip.rect) && InputManager::isMousePressed())
		{
			if(chip.value > m_money)
			{
				SoundManager::playSound(O_PLACE);
				return;
			}

			m_bet += chip.value;
			m_money -= chip.value;

			switch (chip.value)
			{
			case 5:
				m_ChipsBet[0].first++;
				break;

			case 10:
				m_ChipsBet[1].first++;
				break;

			case 25:
				m_ChipsBet[2].first++;
				break;

			case 100:
				m_ChipsBet[3].first++;
				break;

			case 500:
				m_ChipsBet[4].first++;
				break;

			default:
				printf("Unknown chip value in Player::bet\n");
				break;
			}

			SoundManager::playSound(X_PLACE);
		}
	}
	
}

void Player::clearHand()
{
	m_hand.clear();
	m_points = 0;
	m_bet = 0;
}

void Player::addMoney()
{
	m_money += m_bet * 2;
}

void Player::drawChipsBet()
{

	for (int i = 0; i < MAXCHIPS; i++)
	{
		Drawable tmpChipDrawable = m_ChipsBet[i].second;

		for(int j = 0; j < m_ChipsBet[i].first; j++)
		{
			tmpChipDrawable.rect.y -= 10;
			drawObject(tmpChipDrawable);
		}
	}
}

void Player::removeChipBet()
{
	for (int i = 4; i >= 0; i--)
	{
		if (m_ChipsBet[i].first <= 0) continue;

		SDL_Rect tmpRect = { m_ChipsBet[i].second.rect.x, m_ChipsBet[i].second.rect.y - m_ChipsBet[i].first * 10,
							 m_ChipsBet[i].second.rect.w, m_ChipsBet[i].second.rect.h + m_ChipsBet[i].first * 10 };

		if (InputManager::isMousePressed() && isMouseInRect(tmpRect))
		{
			m_ChipsBet[i].first --;
			
			m_bet -= m_ChipsBet[i].second.value;
			m_money += m_ChipsBet[i].second.value;
			break;
		}
	}
}

void Player::clearBet()
{
	for (int i = 0; i < MAXCHIPS; i++)
	{
		m_ChipsBet[i].first = 0;
	}

	m_money += m_bet;
	m_bet = 0;
}

void Player::allInBet()
{
	for (int i = MAXCHIPS - 1; i >= 0; i--)
	{
		while (m_money >= m_ChipsBet[i].second.value)
		{
			m_ChipsBet[i].first ++;
			m_bet += m_ChipsBet[i].second.value;
			m_money -= m_ChipsBet[i].second.value;
		}
	}
}
