#include "Dealer.h"

Dealer::Dealer()
{
}

Dealer::~Dealer()
{
}

void Dealer::init()
{
	m_points = 0;
	m_hand.clear();
}

void Dealer::addCard(DrawableWithValue card)
{
	m_hand.push_back(card);
}

void Dealer::clearHand()
{
	m_hand.clear();
	m_points = 0;
}

void Dealer::calculatePoints()
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

