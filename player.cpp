#include "player.h"

void Player::decrementNumNewUnits() 
{
    m_numNewUnits--;
}

void Player::incrementNumNewUnits() 
{
    m_numNewUnits++;
}

uint Player::getNumNewUnits() 
{
    return m_numNewUnits;
}

void Player::setNumNewUnits(uint numNewUnits)
{
    m_numNewUnits = numNewUnits;
}

void Player::takeCard(Card card)
{
    m_cards.push_back(card);
}

std::vector<Card> * Player::getCards()
{
    return m_cardsPtr;
}
