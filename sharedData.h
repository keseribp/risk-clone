#ifndef SHAREDDATA_H
#define SHAREDDATA_H

#include "gameData.h"
#include "player.h"
#include "uiObject.h"
#include "frame.h"
#include "canvas.h"
#include "panel.h"
#include "button.h"
#include "label.h"
#include "radioGroup.h"

typedef unsigned int uint;

struct SharedData
{
    // data
    std::vector<Player *> * players;
    uint territoryMarkers[MapData::NUM_TERRITORIES];
    uint troopCounts[MapData::NUM_TERRITORIES];
    uint activePlayerIndex; 
    GamePhase gamePhase;
    uint turnNum;
    Territory attackingTerritory;
    Territory defendingTerritory;    
    Territory startTerritory;
    Territory endTerritory;   
    uint numNewUnits;
    uint numAttackingUnitsLost;
    uint numDefendingUnitsLost;
    bool isHighlighted[MapData::NUM_TERRITORIES];
    ButtonStatus buttonStatus[MapData::NUM_BUTTONS];
    ButtonStatus cardStatus[5];
    ButtonStatus plus2Status[5];
    float buttonModifierData[4 * MapData::NUM_BUTTONS];
    float markerPositionData[2 * MapData::NUM_TERRITORIES];
    float cardPositionData[2 * 5];
    float plus2PositionData[2 * 5];
    uint totalNumNewUnits;
    int winner;
    bool territoriesHidden;
    bool showCards;
    Territory highlightedCard;
    //int selectedCards[3];
    bool cardsAreLegalToTurnIn;
    std::vector<int> selectedCards;
    
    // GUI
    GLFWwindow * window;
    
    Frame mainFrame;
    Canvas worldMap;
    Panel controlPanel;
    Button actionButtons[5];
    Label labels[11];
    Button attackDieButtons[3];
    Button defendDieButtons[2];
    RadioGroup attackDiceGroup;
    RadioGroup defendDiceGroup;
    
    //UIObject territoryButtons[];
    
};

#endif
