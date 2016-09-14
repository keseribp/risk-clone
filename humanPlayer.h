#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "player.h"

class HumanPlayer : public Player {
    private:
        std::vector<Card> m_cards; // unused cards in hand
        uint m_newUnits; // units to be placed
    
    public:
        HumanPlayer();
        ~HumanPlayer();
        void takeTurn();
        void setNumNewUnits(uint numUnits);
};

#endif
