#include "Dealer.h"
#include "Presenter.h"

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
	m_cardStartPos = { 400, 200 };
}

void Dealer::addCard(DrawableWithValue card)
{
	m_hand.push_back(card);
}

void Dealer::animateCardDeal(DrawableWithValue& card, int2 endPos)
{
	if (card.isDealt || card.rect.x == endPos.x) return;

	int2 startPos = { 0, 0 };
	int deltaX = endPos.x - startPos.x;
	int deltaY = endPos.y - startPos.y;

	int moveX = deltaX / m_moveSpeed;
	int moveY = deltaY / m_moveSpeed;

	if(abs(card.rect.x - endPos.x) <= moveX || abs(card.rect.y - endPos.y) <= moveY)
	{
		card.rect.x = endPos.x;
		card.rect.y = endPos.y;

		card.isDealt = true;
		return;
	}

	card.rect.x += moveX;
	card.rect.y += moveY;

	cout << "Moving with " << moveX << " " << moveY << endl;
}

void Dealer::animateHand(int2 endPos)
{
	for (int i = 0 ; i < m_hand.size(); i ++)
	{
		if (!m_hand[i].isDealt)
		{
			animateCardDeal(m_hand[i], { endPos.x + i * 50, endPos.y } );
			return;
		}
	}
}

void Dealer::drawHand()
{
	for (DrawableWithValue& card : m_hand)
	{
		drawObject(card);
	}
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

