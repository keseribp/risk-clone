#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <vector>
#include <random>
#include <cstring>
#include <ctime>

#include "generalData.h"
#include "gameData.h"
#include "sharedData.h"
#include "player.h"
#include "humanPlayer.h"

//#include "uiObject.h"
//#include "frame.h"

//#include "computerPlayer.h"

class GameBoard 
{
    private:
        SharedData m_sd;
        SharedData * m_sdPtr = & m_sd; // TODO do we really need this?
        
        std::vector<Card> m_cardDeck; // deck of cards drawn by players
        //std::vector<Player *> * m_players; // array of players set upon initialization. set in turn order //TODO need to put more actions through these isntead of in the gameBoard.update()
        
        std::vector<Territory> m_placedUnits; // for keeping track of where we placed units during the ARMIES phase so we can right click undo and do highlighting
        
        uint m_numNewNeutralUnits;
        uint m_numNewUnitsPlaced;
        bool m_neutralUnitPlaced;
        
        bool m_attackerSelected;
        bool m_defenderSelected;
        
        bool m_startSelected;
        bool m_endSelected;
        
        int m_mouseDelay; //TODO rename to actionDelay
        uint m_mouseDelayVal = 40;
        
        double m_cursorX;
        double m_cursorY;
        int m_mouseButtonStateRight;
        int m_mouseButtonStateLeft;
        
        bool m_leftMouseReleasedAfterPress;
        bool m_rightMouseReleasedAfterPress;
        
        float * m_markerPositionData;
        
        uint m_numAttackDice;
        uint m_numDefendDice;
        
        bool m_conqueredTerritory;
        
        uint m_originalStartTroopCount = 0;
        uint m_originalEndTroopCount = 0;
        
        uint m_setupPhaseTerritoryHistory[3]; // 0, 1 as own territories, 2 as the neutral. MapData::NUM_TERRITORIES as the clear state
        
        uint m_cardUsageCounter;
        
        int m_plus2Selected;
        bool m_noPlus2InSelected3;
        
        //uint m_numSelectedCards;
        
        //bool m_cardsAreLegalToTurnIn;
        //std::vector<int> m_selectedCards;
        
    private:
        template<typename T> void _shuffleArray(T * array, uint length, uint iterations);
        template<typename T> void _shuffleArray(T * array, uint length);
        Territory _whichMarkerWasClicked();
        int _whichButtonIsCursorOver();
        int _whichCardIsCursorOver();
        int _whichPlus2IsCursorOver();
        void _calcNumNewUnits();
        void _determineAttackOutcome();
        void _initializeUIObjects();
        bool _gameIsOver();
        void _calculateUnitsFromCards();
        void _areCardsLegalToTurnIn();
        
    public:
        GameBoard(GLFWwindow * window, std::vector<Player *> * players);
        ~GameBoard();
        void update();
        SharedData * getSharedData();
};

#endif
