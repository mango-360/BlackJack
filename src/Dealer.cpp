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
	m_cardStartPos = { 600, 50 };

	m_pointsField.init("dealerPoints.txt");
}

void Dealer::addCard(DrawableWithValue card)
{
	m_hand.push_back(card);
	calculatePoints();
}

void Dealer::animateCardDeal(DrawableWithValue& card, int2 endPos)
{
	if (card.isDealt || card.rect.x == endPos.x) return;

	float k = abs(endPos.x - card.rect.x) / abs(endPos.y - card.rect.y);
	float l = abs(endPos.y - card.rect.y) / abs(endPos.x - card.rect.x);

	int moveX = m_moveSpeed / sqrt(1 + l * l);
	int moveY = m_moveSpeed / sqrt(1 + k * k);

	if (m_moveSpeed >= abs(endPos.x - card.rect.x) || m_moveSpeed >= abs(endPos.y - card.rect.y))
	{
		card.rect.x = endPos.x;
		card.rect.y = endPos.y;

		card.isDealt = true;
		return;
	}

	card.rect.x += (endPos.x >= card.rect.x) ? moveX : -moveX;
	card.rect.y += (endPos.y >= card.rect.y) ? moveY : -moveY;
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
	for (const DrawableWithValue& card : m_hand)
	{
		Drawable tmp = card;

		drawObject(tmp);
		if (!card.isDealt) break; 
	}
}

void Dealer::drawPoints()
{
	m_pointsField.draw();	
}

void Dealer::updatePoints()
{
	m_pointsField.setText("Dealer Points: " + to_string(m_points));
	m_pointsField.update();
}

int Dealer::getPoints()
{
	return m_points;
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

void Dealer::subtractFirstCardPoints()
{
	m_points -= m_hand[0].value;
}
