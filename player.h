#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "gameData.h"

typedef unsigned int uint;

class Player {
    protected:
        std::vector<Card> m_cards; // unused cards in hand
        uint m_numNewUnits; // units to be placed
    
    private:
        std::vector<Card> * m_cardsPtr = &m_cards;
    
    
    public:
        //virtual Player() = 0;
        //virtual ~Player() = 0;
        virtual void takeTurn() = 0;
        void setNumNewUnits(uint numNewUnits);
        void decrementNumNewUnits();
        void incrementNumNewUnits();
        uint getNumNewUnits();
        void takeCard(Card card);
        std::vector<Card> * getCards();
};

#endif
