#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::init()
{
	m_money = 1000;
	m_bet = 0;
}

void Player::update()
{

}

void Player::destroy()
{
}

void Player::addCard(DrawableWithValue card)
{
	m_hand.push_back(card);
}

void Player::bet(int amount)
{
	m_bet += amount;
	m_money -= amount;
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

void Player::calculatePoints()
{
	m_points = 0;
	int aceCount = 0;
	for (auto& card : m_hand)
	{
		m_points += card.value;
		if (card.value == 11)
			aceCount++;
	}
	while (m_points > 21 && aceCount > 0)
	{
		m_points -= 10;
		aceCount--;
	}
}

