#include "gameBoard.h"
//TODO it would be really useful to just make a button class and panel so that the control panel could have buttons and then the buttons could be hidden and given states more easily
GameBoard::GameBoard(GLFWwindow * window, std::vector<Player *> * players) 
{
    srand(time(NULL));
    
    m_sd.window = window;//m_window = window;
    m_sd.players = players; //for now this is only 2 human players
    
    _initializeUIObjects();
    
    m_mouseDelay = 0;
    
    // game state variables
    m_sd.gamePhase = SETUP;
    m_sd.winner = -1;
    m_sd.activePlayerIndex = 0;
    m_cardUsageCounter = 0;
    m_originalStartTroopCount = 0;
    m_originalEndTroopCount = 0;
    m_plus2Selected = -1;
    m_noPlus2InSelected3 = false;
    m_sd.cardsAreLegalToTurnIn = false;
    m_conqueredTerritory = false;
    m_sd.territoriesHidden = false;
    m_sd.showCards = false;
    m_attackerSelected = false;
    m_defenderSelected = false;
    m_sd.attackingTerritory = NULL_TERRITORY;
    m_sd.defendingTerritory = NULL_TERRITORY;
    m_startSelected = false;
    m_endSelected = false;
    m_sd.startTerritory = NULL_TERRITORY;
    m_sd.endTerritory = NULL_TERRITORY;
    m_sd.numAttackingUnitsLost = 0;
    m_sd.numDefendingUnitsLost = 0;
    m_setupPhaseTerritoryHistory[0] = NULL_TERRITORY;
    m_setupPhaseTerritoryHistory[1] = NULL_TERRITORY;
    m_setupPhaseTerritoryHistory[2] = NULL_TERRITORY;
    
    for (uint i = 0; i < MapData::NUM_TERRITORIES; i++) 
    {
        m_sd.territoryMarkers[i] = 2;
        m_sd.troopCounts[i] = 1;
        m_sd.isHighlighted[i] = false;
    }
    
    m_sd.totalNumNewUnits = 0;
    
    m_numNewNeutralUnits = MapData::NUM_STARTING_UNITS_2P - MapData::NUM_TERRITORIES / 3;
    
    m_sd.numNewUnits = 2;//is 2 for setup phase //MapData::NUM_STARTING_UNITS_2P - MapData::NUM_TERRITORIES / 3;
    
    m_sd.turnNum = m_numNewNeutralUnits - 1; // initially do a countdown
    
    m_numNewUnitsPlaced = 0;
    m_neutralUnitPlaced = false;
    
    uint choiceArray[MapData::NUM_TERRITORIES];
    for (uint i = 0; i < MapData::NUM_TERRITORIES; i++)
        choiceArray[i] = i;
    
    _shuffleArray(choiceArray, MapData::NUM_TERRITORIES);
    
    for (uint i = 0; i < MapData::NUM_TERRITORIES / 3; i++)
        m_sd.territoryMarkers[choiceArray[i]] = 0;
    
    for (uint i = MapData::NUM_TERRITORIES / 3; i < 2 * MapData::NUM_TERRITORIES / 3; i++)
        m_sd.territoryMarkers[choiceArray[i]] = 1;
        
    // the final third are neutral territories
    
    // set number of new units (those left to be placed) for each player
    for (Player * player : *(m_sd.players)) 
        player->setNumNewUnits(m_sd.numNewUnits);
    
    for (int i = 0; i < MapData::NUM_TERRITORIES; i++) 
    {
        m_sd.markerPositionData[2 * i + 0] = static_cast<float>(MapData::TERRITORY_POSITIONS[i][0]) * (1.0f + X_EXTENT) / MapData::MAP_PIXEL_WIDTH - 1.0f;
        m_sd.markerPositionData[2 * i + 1] = -static_cast<float>(MapData::TERRITORY_POSITIONS[i][1]) * 2.0f / MapData::MAP_PIXEL_HEIGHT + 1.0f;
    }
    
    for (int i = 0; i < 5; i++) 
    {
        m_sd.cardPositionData[2 * i + 0] = -(1.0f + X_EXTENT) * 0.25f;
        m_sd.cardPositionData[2 * i + 1] = i * 2.0f / 5.0f - 1.0f + 2 * 0.125f;
        
        if (DEBUG) printf("%i, (%f, %f)\n", i, m_sd.cardPositionData[2 * i], m_sd.cardPositionData[2 * i + 1]);
        
        m_sd.plus2PositionData[2 * i + 0] = X_EXTENT / 2.0f;
        m_sd.plus2PositionData[2 * i + 1] = i * 2.0f / 5.0f - 1.0f + 2 * 0.125f;
    }
    
    for (uint i = 0; i < MapData::NUM_BUTTONS; i++)
    {
        m_sd.buttonModifierData[4 * i + 0] = MapData::BUTTON_CENTERS[i][0]; // x offset
        m_sd.buttonModifierData[4 * i + 1] = MapData::BUTTON_CENTERS[i][1]; // y offset
        m_sd.buttonModifierData[4 * i + 2] = MapData::BUTTON_WIDTHS[i]; // width multiplier
        m_sd.buttonModifierData[4 * i + 3] = MapData::LABEL_HEIGHT; // height multiplier
    }
    
    for (uint i = 0; i < MapData::NUM_BUTTONS; i++)
        m_sd.buttonStatus[i] = BUTTON_INACTIVE;
    m_sd.buttonStatus[7] = BUTTON_HIDDEN; // NOTE these were using LABEL indices instead of button indices
    m_sd.buttonStatus[9] = BUTTON_HIDDEN;
    
    // bring over and shuffle the game cards
    for (uint i = 0; i < MapData::NUM_CARDS; ++i)
        m_cardDeck.push_back(MapData::CARDS[i]);
    std::random_shuffle(m_cardDeck.begin(), m_cardDeck.end());
    
    for (uint i = 0; i < 5; i++)
    {
        m_sd.cardStatus[i] = BUTTON_HIDDEN;
        m_sd.plus2Status[i] = BUTTON_HIDDEN;
    }
    
    if (DEBUG) // testing case jump points to states of the game 
    {
        switch (1)
        {
            case 0: break;
            case 1:
                m_sd.turnNum++;
                m_sd.gamePhase = SETUP;
                m_sd.troopCounts[0] = 5;
                m_sd.troopCounts[1] = 2;
                m_sd.troopCounts[15] = 7;
                m_numNewNeutralUnits = 2;
                m_sd.numNewUnits = 2;
                m_sd.turnNum = m_numNewNeutralUnits - 1; // initially do a countdown
                for (Player * player : *(m_sd.players)) 
                    player->setNumNewUnits(m_sd.numNewUnits);
                //_calcNumNewUnits();
                break;
            case 2:
                m_sd.turnNum++;
                m_sd.gamePhase = ARMIES;
                m_sd.troopCounts[0] = 5;
                m_sd.troopCounts[1] = 2;
                m_sd.troopCounts[15] = 7;
                m_numNewNeutralUnits = 0;
                //m_sd.numNewUnits = 2 * 2;
                m_sd.turnNum = m_numNewNeutralUnits - 1; // initially do a countdown
                (*(m_sd.players))[m_sd.activePlayerIndex]->takeCard(m_cardDeck.back()); // draw from back, insert into front
                m_cardDeck.pop_back();
                (*(m_sd.players))[m_sd.activePlayerIndex]->takeCard(m_cardDeck.back()); // draw from back, insert into front
                m_cardDeck.pop_back();
                (*(m_sd.players))[m_sd.activePlayerIndex]->takeCard(m_cardDeck.back()); // draw from back, insert into front
                m_cardDeck.pop_back();
                (*(m_sd.players))[m_sd.activePlayerIndex]->takeCard(m_cardDeck.back()); // draw from back, insert into front
                m_cardDeck.pop_back();
                (*(m_sd.players))[m_sd.activePlayerIndex]->takeCard(m_cardDeck.back()); // draw from back, insert into front
                m_cardDeck.pop_back();
                for (Player * player : *(m_sd.players)) 
                    player->setNumNewUnits(m_sd.numNewUnits);
                _calcNumNewUnits();
                m_sd.totalNumNewUnits = m_sd.numNewUnits;
                break;
            case 3:
                m_sd.gamePhase = FORTIFY;
                m_sd.troopCounts[0] = 5;
                m_sd.troopCounts[1] = 2;
                m_sd.troopCounts[15] = 7;
                break;     
        }
    }
}

void GameBoard::_initializeUIObjects()
{
    
    m_sd.mainFrame.initialize(m_sd.window);
    
    m_sd.worldMap.initialize(m_sd.window, &m_sd.mainFrame);
    
    m_sd.controlPanel.initialize(m_sd.window, &m_sd.mainFrame);
    
    m_sd.labels[0].initialize(m_sd.window, &m_sd.controlPanel);
    m_sd.labels[1].initialize(m_sd.window, &m_sd.controlPanel);
    m_sd.labels[2].initialize(m_sd.window, &m_sd.controlPanel);
    m_sd.labels[3].initialize(m_sd.window, &m_sd.controlPanel);
    m_sd.labels[4].initialize(m_sd.window, &m_sd.controlPanel);
    m_sd.labels[5].initialize(m_sd.window, &m_sd.controlPanel);
    m_sd.labels[6].initialize(m_sd.window, &m_sd.controlPanel);
    m_sd.labels[7].initialize(m_sd.window, &m_sd.controlPanel);
    m_sd.labels[8].initialize(m_sd.window, &m_sd.controlPanel);
    m_sd.labels[9].initialize(m_sd.window, &m_sd.controlPanel);
    m_sd.labels[10].initialize(m_sd.window, &m_sd.controlPanel);
    
    if (DEBUG) printf("probe 4 GameBoard::_initializeUIObjects\n");    
    m_sd.labels[0].setLabel(MapData::LABELS[0]);//("RISK (green is neutral)");
    m_sd.labels[1].setLabel(MapData::LABELS[1]);//("Turn: ");
    m_sd.labels[2].setLabel(MapData::LABELS[2]);//("Player: ");
    m_sd.labels[3].setLabel(MapData::LABELS[3]);//("Phase: ");
    m_sd.labels[4].setLabel(MapData::LABELS[4]);//("Attacker: ");
    m_sd.labels[5].setLabel(MapData::LABELS[5]);//("Defender: ");
    m_sd.labels[6].setLabel(MapData::LABELS[6]);//("Attacker dice: ");
    m_sd.labels[7].setLabel(MapData::LABELS[7]);//("Defender dice: ");
    m_sd.labels[8].setLabel(MapData::LABELS[8]);//("Attack result: ");
    m_sd.labels[9].setLabel(MapData::LABELS[9]);//("New units: ");
    m_sd.labels[10].setLabel(MapData::LABELS[15]);//("Message: ");
    if (DEBUG) printf("probe 5 GameBoard::_initializeUIObjects\n");    
    m_sd.attackDiceGroup.initialize(m_sd.window, &m_sd.controlPanel, m_sd.attackDieButtons, 3);
    m_sd.defendDiceGroup.initialize(m_sd.window, &m_sd.controlPanel, m_sd.defendDieButtons, 2);
    
    m_sd.actionButtons[0].initialize(m_sd.window, &m_sd.controlPanel);
    m_sd.actionButtons[1].initialize(m_sd.window, &m_sd.controlPanel);
    m_sd.actionButtons[2].initialize(m_sd.window, &m_sd.controlPanel);
    m_sd.actionButtons[3].initialize(m_sd.window, &m_sd.controlPanel);
    m_sd.actionButtons[4].initialize(m_sd.window, &m_sd.controlPanel);
    if (DEBUG) printf("probe 6 GameBoard::_initializeUIObjects\n");    
    m_sd.actionButtons[0].setLabel(MapData::LABELS[10]);//("Use cards");
    m_sd.actionButtons[1].setLabel(MapData::LABELS[11]);//("View/Hide uints");
    m_sd.actionButtons[2].setLabel(MapData::LABELS[12]);//("Attack");
    m_sd.actionButtons[3].setLabel(MapData::LABELS[13]);//("View/Hide units");
    m_sd.actionButtons[4].setLabel(MapData::LABELS[14]);//("End phase");
    if (DEBUG) printf("probe 7 GameBoard::_initializeUIObjects\n");    
    m_sd.attackDieButtons[0].initialize(m_sd.window, &m_sd.attackDiceGroup);
    m_sd.attackDieButtons[1].initialize(m_sd.window, &m_sd.attackDiceGroup);
    m_sd.attackDieButtons[2].initialize(m_sd.window, &m_sd.attackDiceGroup);
    if (DEBUG) printf("probe 8 GameBoard::_initializeUIObjects\n");    
    m_sd.attackDieButtons[2].setLabel("3");
    m_sd.attackDieButtons[0].setLabel("1");
    m_sd.attackDieButtons[1].setLabel("2");
    //m_sd.attackDieButtons[2].setLabel("3");
    if (DEBUG) printf("probe 9 GameBoard::_initializeUIObjects\n");    
    m_sd.defendDieButtons[0].initialize(m_sd.window, &m_sd.defendDiceGroup);
    m_sd.defendDieButtons[1].initialize(m_sd.window, &m_sd.defendDiceGroup);
    m_sd.defendDieButtons[0].setLabel("1");
    m_sd.defendDieButtons[1].setLabel("2");
    if (DEBUG) printf("probe 10 GameBoard::_initializeUIObjects\n"); 
}

GameBoard::~GameBoard() {}

void GameBoard::update() 
{
    //float x, y;
    Territory i; 
    int b, c, p;
    m_mouseButtonStateLeft = glfwGetMouseButton(m_sd.window, GLFW_MOUSE_BUTTON_LEFT);
    m_mouseButtonStateRight = glfwGetMouseButton(m_sd.window, GLFW_MOUSE_BUTTON_RIGHT);
    glfwGetCursorPos(m_sd.window, &m_cursorX, &m_cursorY);
    m_cursorX = m_cursorX * 2.0f / WINDOW_X - 1.0f;
    m_cursorY = -m_cursorY * 2.0f / WINDOW_Y + 1.0f;
    
    b = _whichButtonIsCursorOver(); //TODO these should have clearer names (b,i,c,p)
    i = _whichMarkerWasClicked();
    c = _whichCardIsCursorOver();
    p = _whichPlus2IsCursorOver();
    
    // all button status code should be done here. should use variables set in the lower control structure to modify the buttons status and do no button modification in the lower level
    for (uint j = 0; j < MapData::NUM_BUTTONS; j++)
    { 
        m_sd.buttonStatus[j] = BUTTON_HIDDEN;
    } 
        
    for (uint j = 0; j < 5; j++) 
    {
        m_sd.cardStatus[j] = BUTTON_HIDDEN;
        m_sd.plus2Status[j] = BUTTON_HIDDEN;
    }
    
    //if (DEBUG) printf("mouse state left = %i\n", m_mouseButtonStateLeft);
    
    if (m_sd.showCards || m_sd.territoriesHidden) i = NULL_TERRITORY; // disallow territory selection when hidden
    if (!m_sd.showCards && m_sd.territoriesHidden) // disallow card an plus2 selection when hidden
    {
        c = -1;
        p = -1;
    }
         
    switch (m_sd.gamePhase)
    {
        case SETUP:
            break;
        case SETUP_FINAL:
            break;
        case ARMIES:
            if (m_sd.showCards && m_sd.selectedCards.size() != 3)
            {
                //m_sd.buttonStatus[5] = BUTTON_HIDDEN; // use cards
                m_sd.buttonStatus[6] = BUTTON_PRESSED; // view/hide cards
                //m_sd.buttonStatus[7] = BUTTON_HIDDEN; // attack
                //m_sd.buttonStatus[8] = BUTTON_HIDDEN; // view/hide units
                //m_sd.buttonStatus[9] = BUTTON_HIDDEN; // end phase
            }
            else if (m_sd.showCards && m_sd.selectedCards.size() == 3)
            {
                if (m_sd.cardsAreLegalToTurnIn && (m_plus2Selected > -1 || m_noPlus2InSelected3))
                {
                    m_sd.buttonStatus[5] = BUTTON_INACTIVE; // use cards
                }
                //else
                //    m_sd.buttonStatus[5] = BUTTON_HIDDEN; // use cards
                m_sd.buttonStatus[6] = BUTTON_PRESSED; // view/hide cards
                //m_sd.buttonStatus[7] = BUTTON_HIDDEN; // attack
                //m_sd.buttonStatus[8] = BUTTON_HIDDEN; // attack
                //m_sd.buttonStatus[9] = BUTTON_HIDDEN; // end phase
            }
            else if (m_sd.territoriesHidden)
            {
                //m_sd.buttonStatus[5] = BUTTON_HIDDEN; // use cards
                //m_sd.buttonStatus[6] = BUTTON_HIDDEN; // view/hide cards
                //m_sd.buttonStatus[7] = BUTTON_HIDDEN; // attack
                m_sd.buttonStatus[8] = BUTTON_PRESSED; // view/hide units
                //m_sd.buttonStatus[9] = BUTTON_HIDDEN; // end phase
            }
            else if (m_sd.numNewUnits > 0 || ((*(m_sd.players))[m_sd.activePlayerIndex]->getCards())->size() == 5)
            {
                //m_sd.buttonStatus[5] = BUTTON_HIDDEN; // use cards //TODO these should be tied directly into the appropriate label hides, etc via UIObjects
                m_sd.buttonStatus[6] = BUTTON_INACTIVE; // view/hide cards
                //m_sd.buttonStatus[7] = BUTTON_HIDDEN; // attack
                m_sd.buttonStatus[8] = BUTTON_INACTIVE; // view/hide units
                //m_sd.buttonStatus[9] = BUTTON_HIDDEN; // end phase
            }
            else //if (m_sd.numNewUnits == 0)
            {
                //m_sd.buttonStatus[5] = BUTTON_HIDDEN; // use cards
                m_sd.buttonStatus[6] = BUTTON_INACTIVE; // view/hide cards
                //m_sd.buttonStatus[7] = BUTTON_HIDDEN; // attack
                m_sd.buttonStatus[8] = BUTTON_INACTIVE; // view/hide units
                m_sd.buttonStatus[9] = BUTTON_INACTIVE; // end phase
            }
            if (m_sd.showCards)
            {
                uint s = ((*(m_sd.players))[m_sd.activePlayerIndex]->getCards())->size();
                if (s >= 1) 
                {
                    m_sd.cardStatus[0] = BUTTON_INACTIVE;
                    m_sd.plus2Status[0] = BUTTON_INACTIVE;
                }
                if (s >= 2) 
                {
                    m_sd.cardStatus[1] = BUTTON_INACTIVE;
                    m_sd.plus2Status[1] = BUTTON_INACTIVE;
                }
                if (s >= 3) 
                {
                    m_sd.cardStatus[2] = BUTTON_INACTIVE;
                    m_sd.plus2Status[2] = BUTTON_INACTIVE;
                }
                if (s >= 4) 
                {
                    m_sd.cardStatus[3] = BUTTON_INACTIVE;
                    m_sd.plus2Status[3] = BUTTON_INACTIVE;
                }
                if (s >= 5) 
                {
                    m_sd.cardStatus[4] = BUTTON_INACTIVE;
                    m_sd.plus2Status[4] = BUTTON_INACTIVE;
                }
            }
            break;
        case ATTACK:
            m_sd.buttonStatus[0] = BUTTON_INACTIVE;
            m_sd.buttonStatus[1] = BUTTON_INACTIVE;
            m_sd.buttonStatus[2] = BUTTON_INACTIVE;
            m_sd.buttonStatus[3] = BUTTON_INACTIVE;
            m_sd.buttonStatus[4] = BUTTON_INACTIVE;
            //m_sd.buttonStatus[5] = BUTTON_HIDDEN; // use cards
            m_sd.buttonStatus[6] = BUTTON_INACTIVE; // view/hide cards
            if (m_numAttackDice > 0 && m_numDefendDice > 0 && MapData::MAP[m_sd.attackingTerritory][m_sd.defendingTerritory])
            {
                m_sd.buttonStatus[7] = BUTTON_INACTIVE; // attack
            }
            m_sd.buttonStatus[8] = BUTTON_INACTIVE; // view/hide units
            m_sd.buttonStatus[9] = BUTTON_INACTIVE; // end phase
            if (m_numAttackDice == 1)
            { 
                m_sd.buttonStatus[0] = BUTTON_PRESSED;
            }
            else if (m_numAttackDice == 2)
            {
                m_sd.buttonStatus[1] = BUTTON_PRESSED;
            }
            else if (m_numAttackDice == 3)
            {
                m_sd.buttonStatus[2] = BUTTON_PRESSED;
            }
            if (m_numDefendDice == 1)
            { 
                m_sd.buttonStatus[3] = BUTTON_PRESSED;
            }
            else if (m_numDefendDice == 2)
            {
                m_sd.buttonStatus[4] = BUTTON_PRESSED;
            }
            break;
        case ATTACK_MOVE:
            m_sd.buttonStatus[0] = BUTTON_INACTIVE;
            m_sd.buttonStatus[1] = BUTTON_INACTIVE;
            m_sd.buttonStatus[2] = BUTTON_INACTIVE;
            m_sd.buttonStatus[3] = BUTTON_INACTIVE;
            m_sd.buttonStatus[4] = BUTTON_INACTIVE;
            // m_sd.buttonStatus[5] = BUTTON_HIDDEN; // use cards // use cards
            m_sd.buttonStatus[6] = BUTTON_INACTIVE; // view/hide cards
            m_sd.buttonStatus[7] = BUTTON_INACTIVE; // attack
            m_sd.buttonStatus[8] = BUTTON_INACTIVE; // view/hide units
            //m_sd.buttonStatus[9] = BUTTON_INACTIVE; // end phase
            break;
        case FORTIFY:
            m_sd.buttonStatus[6] = BUTTON_INACTIVE; // view/hide cards
            m_sd.buttonStatus[8] = BUTTON_INACTIVE; // view/hide units
            m_sd.buttonStatus[9] = BUTTON_INACTIVE; // end phase
            break;
        default:
        case END:
            break;
    }
    if (b > -1)
    {
        if (m_sd.buttonStatus[b] == BUTTON_INACTIVE) 
        {
            m_sd.buttonStatus[b] = BUTTON_HIGHLIGHTED;
        }
    }
    if (c > -1)
    {
        if (m_sd.cardStatus[c] == BUTTON_INACTIVE) 
        {
            m_sd.cardStatus[c] = BUTTON_HIGHLIGHTED;
        }   
    }
    if (p > -1)
    {
        if (m_sd.plus2Status[p] == BUTTON_INACTIVE && 
            m_sd.selectedCards.size() == 3 && 
            m_sd.cardsAreLegalToTurnIn) 
        {
            if (m_sd.selectedCards[0] == p || 
                m_sd.selectedCards[1] == p ||
                m_sd.selectedCards[2] == p)
            {
                m_sd.plus2Status[p] = BUTTON_HIGHLIGHTED;
            }
        }   
    }
    for (int cc : m_sd.selectedCards)
    {
        if (cc > -1) 
        {
            m_sd.cardStatus[cc] = BUTTON_PRESSED; //TODO likely don't need the if c >-1 anymore since we are no longer using an array
        }
    }
    //if (DEBUG) printf("before if: m_plus2Selected = %i\n", m_plus2Selected);
    //if (DEBUG) printf("before if: m_plus2Selected > -1 = %d\n", (m_plus2Selected > -1));
    if (m_plus2Selected > -1 &&
        m_sd.cardsAreLegalToTurnIn) 
    {    
        m_sd.plus2Status[m_plus2Selected] = BUTTON_PRESSED;
        //if (DEBUG) printf("m_sd.plus2Status[m_plus2Selected] = BUTTON_PRESSED\n");
        //if (DEBUG) printf("m_sd.plus2Status[m_plus2Selected] = %i\n", m_sd.plus2Status[m_plus2Selected]);
    }
    //if (DEBUG) printf("after if: m_plus2Selected = %i\n", m_plus2Selected);
    //if (DEBUG) 
    //{
    //    for (uint i = 0; i < 5; i++)   
    //        printf("display begin: m_sd.plus2Status[%i] = %i\n", i, m_sd.plus2Status[i]);
    //}
    if (m_leftMouseReleasedAfterPress && m_rightMouseReleasedAfterPress)
    { 
        /////////////////////////////////////////////////////////////
        if (m_sd.gamePhase == SETUP || m_sd.gamePhase == SETUP_FINAL)
        {
            if (m_mouseButtonStateLeft == GLFW_PRESS && m_mouseDelay == 0 && i > -1) 
	        {
	            m_mouseDelay = m_mouseDelayVal;
	            m_leftMouseReleasedAfterPress = false;
                if (m_sd.territoryMarkers[i] == m_sd.activePlayerIndex && 
                    //(*m_sd.m_players)[m_sd.activePlayerIndex]->getNumNewUnits() > 0 && 
                    m_numNewUnitsPlaced < 2) 
                {
                    // if the box is owned by the player and there is at least one new unit remaining
                    m_sd.troopCounts[i]++;
    //                (*m_sd.m_players)[m_sd.activePlayerIndex]->decrementNumNewUnits();
                    m_setupPhaseTerritoryHistory[m_numNewUnitsPlaced] = i; // 1st 2 slots are for the player owned territories
                    m_numNewUnitsPlaced++;
                    m_sd.numNewUnits--;
                    if (DEBUG) printf("decrement num new units for player %i\n", m_sd.activePlayerIndex);
                } 
                else if (m_sd.territoryMarkers[i] == 2 && 
                         //m_numNewNeutralUnits > 0 && 
                         !m_neutralUnitPlaced) 
                { // neutral player index is 2
                    m_sd.troopCounts[i]++;
                    m_setupPhaseTerritoryHistory[2] = i; // 3rd slot is for the netural territory placement
                    m_numNewNeutralUnits--;
                    m_neutralUnitPlaced = true;
                    if (DEBUG) printf("decrement num new units for neutral player\n");
                }
            }
            else if (m_mouseButtonStateRight == GLFW_PRESS && m_mouseDelay == 0 && i > -1) 
	        {
	            m_mouseDelay = m_mouseDelayVal;
	            m_rightMouseReleasedAfterPress = false;
                if (DEBUG) printf("mouse pressed in box at: %f, %f\n", m_cursorX, m_cursorY);
                if (m_sd.territoryMarkers[i] == m_sd.activePlayerIndex && 
                    //(*m_sd.m_players)[m_activePlayerIndex].getNumNewUnits() > 0 && 
                    m_numNewUnitsPlaced > 0 &&
                    (m_setupPhaseTerritoryHistory[0] == i || m_setupPhaseTerritoryHistory[1] == i)) 
                {
                    // if the box is owned by the player and there is at least one new unit remaining
                    m_sd.troopCounts[i]--;
    //                (*m_sd.m_players)[m_sd.activePlayerIndex]->incrementNumNewUnits();
                    m_numNewUnitsPlaced--;
                    m_sd.numNewUnits++;
                    if (m_setupPhaseTerritoryHistory[1] == i)
                    {
                        m_setupPhaseTerritoryHistory[1] = NULL_TERRITORY;
                    }
                    else //m_setupPhaseTerritoryHistory[0] = i
                    {
                        m_setupPhaseTerritoryHistory[0] = m_setupPhaseTerritoryHistory[1];
                        m_setupPhaseTerritoryHistory[1] = NULL_TERRITORY;
                    }
                    if (DEBUG) printf("decrement num new units for player %i\n", m_sd.activePlayerIndex);
                } 
                else if (m_sd.territoryMarkers[i] == 2 && 
                         //m_numNewNeutralUnits == 0 && 
                         m_neutralUnitPlaced && 
                         m_setupPhaseTerritoryHistory[2] == i) 
                { // neutral player index is 2
                    m_sd.troopCounts[i]--;
                    m_numNewNeutralUnits++;
                    m_setupPhaseTerritoryHistory[2] = NULL_TERRITORY;
                    m_neutralUnitPlaced = false;
                }
            }
            
            if (m_numNewUnitsPlaced == 2 && m_neutralUnitPlaced && 
                (glfwGetKey(m_sd.window, GLFW_KEY_N) == GLFW_PRESS || m_mouseButtonStateLeft == GLFW_PRESS && b == 7) &&
                m_mouseDelay == 0 &&
                m_sd.turnNum != 0 &&
                m_sd.gamePhase == SETUP) 
            {
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
                m_sd.activePlayerIndex = (m_sd.activePlayerIndex + 1) % 2;
                m_numNewUnitsPlaced = 0;
                m_setupPhaseTerritoryHistory[0] = NULL_TERRITORY;
                m_setupPhaseTerritoryHistory[1] = NULL_TERRITORY;
                m_setupPhaseTerritoryHistory[2] = NULL_TERRITORY;
                m_neutralUnitPlaced = false;
                m_sd.turnNum--;
                m_sd.numNewUnits = 2; // reset to num to be placed
                if (m_sd.turnNum == 0)
                    m_sd.gamePhase = SETUP_FINAL;
            }
            //if (DEBUG) printf("probe gameBoard 6\n");
            if (m_numNewNeutralUnits == 0 && m_sd.turnNum == 0 && m_numNewUnitsPlaced == 2 && m_neutralUnitPlaced && 
                (glfwGetKey(m_sd.window, GLFW_KEY_E) == GLFW_PRESS || m_mouseButtonStateLeft == GLFW_PRESS && b == 9) &&
                m_mouseDelay == 0 &&
                m_sd.gamePhase == SETUP_FINAL)
            {
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
                //if (DEBUG) printf("probe gameBoard 7\n");
                m_setupPhaseTerritoryHistory[0] = NULL_TERRITORY;
                m_setupPhaseTerritoryHistory[1] = NULL_TERRITORY;
                m_setupPhaseTerritoryHistory[2] = NULL_TERRITORY;
                m_sd.activePlayerIndex = (m_sd.activePlayerIndex + 1) % 2;
                m_sd.turnNum = 1;
                m_sd.gamePhase = ARMIES;
    //            m_sd.buttonStatus[7] = BUTTON_INACTIVE;
                _calcNumNewUnits();
                m_sd.totalNumNewUnits = m_sd.numNewUnits;
                //if (DEBUG) printf("game now at attack first turn\n");
            }
            
            if (m_sd.gamePhase == SETUP && m_numNewUnitsPlaced == 2 && m_neutralUnitPlaced) 
                m_sd.buttonStatus[7] = BUTTON_INACTIVE;
            if (m_sd.gamePhase == SETUP && m_numNewUnitsPlaced == 2 && m_neutralUnitPlaced && b == 7) 
                m_sd.buttonStatus[7] = BUTTON_HIGHLIGHTED;
            
            if (m_sd.gamePhase == SETUP_FINAL && m_numNewUnitsPlaced == 2 && m_neutralUnitPlaced) 
                m_sd.buttonStatus[9] = BUTTON_INACTIVE;
            if (m_sd.gamePhase == SETUP_FINAL && m_numNewUnitsPlaced == 2 && m_neutralUnitPlaced && b == 9) 
                m_sd.buttonStatus[9] = BUTTON_HIGHLIGHTED;
            
            for (uint ii = 0; ii < MapData::NUM_TERRITORIES; ii++) 
                m_sd.isHighlighted[ii] = false;
            if (m_setupPhaseTerritoryHistory[0] != NULL_TERRITORY) 
                m_sd.isHighlighted[m_setupPhaseTerritoryHistory[0]] = true;
            if (m_setupPhaseTerritoryHistory[1] != NULL_TERRITORY) 
                m_sd.isHighlighted[m_setupPhaseTerritoryHistory[1]] = true;
            if (m_setupPhaseTerritoryHistory[2] != NULL_TERRITORY) 
                m_sd.isHighlighted[m_setupPhaseTerritoryHistory[2]] = true;
            
            if (m_mouseDelay > 0) 
                m_mouseDelay--;
        }
        //////////////////////////////////
        else if (m_sd.gamePhase == ARMIES)
        {  
            //if (DEBUG) printf("armies probe 1\n");
            if (m_mouseButtonStateLeft == GLFW_PRESS && m_mouseDelay == 0 && i > -1 && !m_sd.territoriesHidden) 
	        {   
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
                if (m_sd.territoryMarkers[i] == m_sd.activePlayerIndex && m_sd.numNewUnits > 0) 
                {
                    m_placedUnits.push_back(i);
                    m_sd.numNewUnits--;
                    m_sd.troopCounts[i]++;
                    if (DEBUG) printf("armies pushed onto m_placedUnits\n");
                }
            }
            if (m_mouseButtonStateRight == GLFW_PRESS && m_mouseDelay == 0 && i > -1 && !m_sd.territoriesHidden) 
	        {
	            //if (DEBUG) printf("armies probe 4.1\n");
                m_mouseDelay = m_mouseDelayVal;
                m_rightMouseReleasedAfterPress = false;
                if (DEBUG) printf("numNewUnits = %i\n", m_sd.numNewUnits);
                if (DEBUG) printf("totalNumNewUnits = %i\n", m_sd.totalNumNewUnits);
                if (m_sd.territoryMarkers[i] == m_sd.activePlayerIndex && m_sd.numNewUnits < m_sd.totalNumNewUnits) 
                {
                    if (DEBUG) printf("territory num = %i\n", i);
                    for (uint ii = 0; ii < m_placedUnits.size(); ii++) 
                    {
                        if (DEBUG) printf("checking m_placedUnits[%i] = %i\n", ii, m_placedUnits[ii]);
                        if (i == m_placedUnits[ii])
                        {
                            m_placedUnits.erase(m_placedUnits.begin() + ii);
                            if (DEBUG) printf("undoing placement\n");
                            break;
                        }
                    }
                    m_sd.numNewUnits++;
                    m_sd.troopCounts[i]--;
                }
            }
            if (b == 8 && m_mouseButtonStateLeft == GLFW_PRESS && m_mouseDelay == 0 && !m_sd.showCards) 
            {
                // view/hide territories
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
                m_sd.territoriesHidden = !m_sd.territoriesHidden;
                if (m_sd.selectedCards.size() != 0)
                {
                    // reset the selected cards
                    m_sd.selectedCards.clear();
                }
            }
            if (b == 6 && m_mouseButtonStateLeft == GLFW_PRESS && m_mouseDelay == 0 && !m_sd.territoriesHidden) 
            {
                // view/hide cards
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
                m_sd.showCards = !m_sd.showCards;
                if (DEBUG) printf("probe into view/hide cards\n"); 
            }
            if (c > -1 && m_mouseButtonStateLeft == GLFW_PRESS && m_sd.showCards && m_mouseDelay == 0)
            {
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
                //select cards
                if (m_sd.selectedCards.size() < 3 && (m_sd.cardStatus[c] == BUTTON_INACTIVE || m_sd.cardStatus[c] == BUTTON_HIGHLIGHTED))
                {
                    //m_sd.selectedCards[m_numSelectedCards] = c;//static_cast<Territory>(c);
                    m_sd.selectedCards.push_back(c);
                    //m_numSelectedCards++;
                    m_sd.cardStatus[c] = BUTTON_PRESSED;
                    if (DEBUG) printf("button pressed.\n");
                }
                else if (m_sd.selectedCards.size() > 0 && m_sd.cardStatus[c] == BUTTON_PRESSED)
                {
                    //m_numSelectedCards--;
                    //m_sd.selectedCards[m_numSelectedCards] = -1;//static_cast<Territory>(c);
                    if (DEBUG) printf("before button unpressed.\n");
                    for (uint ii = 0; ii < m_sd.selectedCards.size(); ii++)
                        if (m_sd.selectedCards[ii] == c) m_sd.selectedCards.erase(m_sd.selectedCards.begin() + ii);
                    m_sd.cardStatus[c] = BUTTON_HIGHLIGHTED;
                    //if (DEBUG) printf("button unpressed.\n");
                    m_plus2Selected = -1;
                }
                else if (m_sd.selectedCards.size() == 3 && (m_sd.cardStatus[c] == BUTTON_INACTIVE || m_sd.cardStatus[c] == BUTTON_HIGHLIGHTED))
                {
                    m_sd.cardStatus[m_sd.selectedCards[0]] = BUTTON_INACTIVE;
                    m_sd.cardStatus[c] = BUTTON_PRESSED;
                    m_sd.selectedCards.erase(m_sd.selectedCards.begin());
                    m_sd.selectedCards.push_back(c);
                    if (m_sd.selectedCards[0] != m_plus2Selected && 
                        m_sd.selectedCards[1] != m_plus2Selected && 
                        m_sd.selectedCards[2] != m_plus2Selected) 
                    {
                        m_plus2Selected = -1;
                    }
                    //if (DEBUG) printf("button unpressed 2.\n");
                }
            }
            if (p > -1 && 
                m_mouseButtonStateLeft == GLFW_PRESS && 
                m_sd.showCards && 
                m_mouseDelay == 0 && 
                m_sd.selectedCards.size() == 3 &&
                m_sd.cardsAreLegalToTurnIn) // p needs to be associated with one of the cards that we already have highlighted
            {
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
                // you can only have one selected at a time and it must be owned by the active player
                if (m_sd.selectedCards[0] == p || m_sd.selectedCards[1] == p || m_sd.selectedCards[2] == p )
                {
                    m_plus2Selected = p;
                }
                if (DEBUG) printf("+2 button pressed. %i\n", m_plus2Selected);
                
            }
            if (b == 5 && m_mouseButtonStateLeft == GLFW_PRESS && m_sd.showCards && m_sd.selectedCards.size() == 3 && m_mouseDelay == 0)
            {
                // use cards
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
                std::vector<Card> * cards = (*(m_sd.players))[m_sd.activePlayerIndex]->getCards();
                //if ((*cards)[m_sd.selectedCards[0]].territory) 
                m_cardDeck.push_back((*cards)[m_sd.selectedCards[0]]);
                m_cardDeck.push_back((*cards)[m_sd.selectedCards[1]]);
                m_cardDeck.push_back((*cards)[m_sd.selectedCards[2]]);
                if (m_plus2Selected > -1 && !m_noPlus2InSelected3)
                {
                    m_sd.troopCounts[static_cast<uint>((*cards)[m_plus2Selected].territory)] += 2;
                }
                if ((*cards).size() == 3)
                {
                    (*cards).clear();
                }
                else if ((*cards).size() == 4)
                {
                    Card tmpCard;
                    for (uint i = 0; i < 4; i++)
                    {
                        if (i != m_sd.selectedCards[0] && i != m_sd.selectedCards[1] && i != m_sd.selectedCards[2])
                            tmpCard = (*cards)[i];
                    }
                    (*cards).clear();
                    (*cards).push_back(tmpCard);
                }
                else if ((*cards).size() == 5)
                {
                    std::vector<Card> tmpCard;
                    for (uint i = 0; i < 5; i++)
                    {
                        if (i != m_sd.selectedCards[0] && i != m_sd.selectedCards[1] && i != m_sd.selectedCards[2])
                            tmpCard.push_back((*cards)[i]);
                    }
                    (*cards).clear();
                    (*cards).push_back(tmpCard[0]);
                    (*cards).push_back(tmpCard[1]);
                }
                m_sd.selectedCards.clear();
                _calculateUnitsFromCards();
            }
            if ((glfwGetKey(m_sd.window, GLFW_KEY_E) == GLFW_PRESS || b == 9 && m_mouseButtonStateLeft == GLFW_PRESS) && 
                m_mouseDelay == 0 && 
                m_sd.numNewUnits == 0 && 
                !m_sd.territoriesHidden &&
                ((*(m_sd.players))[m_sd.activePlayerIndex]->getCards())->size() != 5) 
            {
                //if (DEBUG) printf("armies probe 5\n");
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
                if (DEBUG) printf("end armies phase\n");
                m_sd.totalNumNewUnits = 0;
                m_placedUnits.clear();
                m_sd.territoriesHidden = false;
                m_numAttackDice = 0;
                m_numDefendDice = 0;
                m_sd.gamePhase = ATTACK;
            }
            if (m_sd.selectedCards.size() == 3 && m_sd.showCards)
            {
                //if (DEBUG) printf("_areCardsLegalToTurnIn()\n"); 
                _areCardsLegalToTurnIn();
            }
            else
            {
                m_sd.cardsAreLegalToTurnIn = false;
                m_noPlus2InSelected3 = false;
            }
            for (uint ii = 0; ii < MapData::NUM_TERRITORIES; ii++)
                m_sd.isHighlighted[ii] = false;
            for (uint ii = 0; ii < m_placedUnits.size(); ii++) 
                m_sd.isHighlighted[m_placedUnits[ii]] = true;
            if (m_mouseDelay > 0) m_mouseDelay--;
            //if (DEBUG) printf("armies probe 3\n");
        }
        //////////////////////////////////
        else if (m_sd.gamePhase == ATTACK) 
        {
            if (m_mouseButtonStateLeft == GLFW_PRESS && m_mouseDelay == 0 && i > -1 && !m_sd.territoriesHidden) 
	        {
	            m_mouseDelay = m_mouseDelayVal;
	            m_leftMouseReleasedAfterPress = false;
                if ((m_sd.territoryMarkers[i] == m_sd.activePlayerIndex && !m_attackerSelected) ||
                    (i != m_sd.attackingTerritory && m_attackerSelected && m_sd.territoryMarkers[i] == m_sd.activePlayerIndex))
                {
                    m_attackerSelected = true;
                    m_sd.attackingTerritory = i;
                    if (m_sd.troopCounts[i] > 3)
                    {
                        m_numAttackDice = 3;
                        //m_sd.buttonStatus[2] = BUTTON_PRESSED;
                    } 
                    else if (m_sd.troopCounts[i] == 3)
                    {
                        m_numAttackDice = 2;
                        //m_sd.buttonStatus[1] = BUTTON_PRESSED;
                    }
                    else if (m_sd.troopCounts[i] == 2)
                    {
                        m_numAttackDice = 1;
                        //m_sd.buttonStatus[0] = BUTTON_PRESSED;//m_sd.buttonStatus[2] = BUTTON_INACTIVE;
                    }
                    else
                    {
                        m_numAttackDice = 0;
                    }
                    //printf("probe 1\n");
                } 
                else if ((m_sd.territoryMarkers[i] != m_sd.activePlayerIndex && !m_defenderSelected) ||
                         (i != m_sd.defendingTerritory && m_defenderSelected && m_sd.territoryMarkers[i] != m_sd.activePlayerIndex)) 
                {
                    m_defenderSelected = true;
                    m_sd.defendingTerritory = i;
                    if (m_sd.troopCounts[i] >= 2)
                    {
                        m_numDefendDice = 2;
                        //m_sd.buttonStatus[4] = BUTTON_PRESSED;
                    }
                    else
                    {
                        m_numDefendDice = 1;
                        //m_sd.buttonStatus[3] = BUTTON_PRESSED;
                    }
                    //printf("probe 2\n");
                } 
                else if (i == m_sd.attackingTerritory && m_attackerSelected) 
                {
                    m_attackerSelected = false;
                    m_sd.attackingTerritory = NULL_TERRITORY;
                    m_numAttackDice = 0;
                }
                else if (i == m_sd.defendingTerritory && m_defenderSelected) 
                {
                    m_defenderSelected = false;
                    m_sd.defendingTerritory = NULL_TERRITORY;
                    m_numDefendDice = 0;
                }
            }
            if (m_attackerSelected && m_defenderSelected && !m_sd.territoriesHidden) 
            {
                //printf("attacker and defender are both selected\n");
                //verfiy the selection is legal using the connectivity
                if (MapData::MAP[m_sd.attackingTerritory][m_sd.defendingTerritory] && m_sd.troopCounts[m_sd.attackingTerritory] > 1) 
                {
                    //if (DEBUG) printf("attack is legal. press A to attack.\n"); //for now we are just doing 1 v 1 dice rolls
                    if ((glfwGetKey(m_sd.window, GLFW_KEY_A) == GLFW_PRESS || b == 7 && m_mouseButtonStateLeft == GLFW_PRESS) && m_mouseDelay == 0) 
                    {
                        m_mouseDelay = m_mouseDelayVal;
                        m_leftMouseReleasedAfterPress = false;
                        // perform attack and resolve the values
                        if (DEBUG) printf("attacking.\n");
                        _determineAttackOutcome();
                        m_sd.troopCounts[m_sd.attackingTerritory] -= m_sd.numAttackingUnitsLost;
                        m_sd.troopCounts[m_sd.defendingTerritory] -= m_sd.numDefendingUnitsLost;
                        if (m_sd.troopCounts[m_sd.attackingTerritory] < 2)
                        {
                            m_attackerSelected = false;
                            m_sd.attackingTerritory = NULL_TERRITORY;
                            m_numAttackDice = 0;
                        }
                        else if (m_sd.troopCounts[m_sd.attackingTerritory] == 2 && m_numAttackDice >= 1) 
                        {
                            m_numAttackDice = 1;
                            //m_sd.buttonStatus[0] = BUTTON_PRESSED;
                        }
                        else if (m_sd.troopCounts[m_sd.attackingTerritory] == 3 && m_numAttackDice >= 2) 
                        {
                            m_numAttackDice = 2;
                            //m_sd.buttonStatus[1] = BUTTON_PRESSED;
                        }
                        else if (m_sd.troopCounts[m_sd.attackingTerritory] >= 4 && m_numAttackDice >= 3) 
                        {
                            m_numAttackDice = 3;
                            //m_sd.buttonStatus[2] = BUTTON_PRESSED;
                        }
                        if (m_sd.troopCounts[m_sd.defendingTerritory] < 1) 
                        {
                            if (DEBUG) printf("territory conquered.\n");
                            m_sd.territoryMarkers[m_sd.defendingTerritory] = m_sd.activePlayerIndex;
                            m_sd.troopCounts[m_sd.attackingTerritory] -= m_numAttackDice;
                            m_sd.troopCounts[m_sd.defendingTerritory] += m_numAttackDice;
                            m_defenderSelected = true;//false;
                            m_numDefendDice = 0;
                            m_conqueredTerritory = true;
                            m_sd.gamePhase = ATTACK_MOVE;
                        }
                        else if (m_sd.troopCounts[m_sd.defendingTerritory] == 1 &&
                                 m_numDefendDice >= 1) 
                        {
                            m_numDefendDice = 1;
                            //m_sd.buttonStatus[3] = BUTTON_PRESSED;
                        }
                        else if (m_sd.troopCounts[m_sd.defendingTerritory] >= 2 && 
                                 m_numDefendDice >= 2) 
                        {
                            m_numDefendDice = 2;
                            //m_sd.buttonStatus[4] = BUTTON_PRESSED;
                        }
                    }
                    if (m_mouseButtonStateLeft == GLFW_PRESS && 
                        m_mouseDelay == 0 && 
                        b == 0 && 
                        m_sd.troopCounts[m_sd.attackingTerritory] > 1)
	                {
	                    m_mouseDelay = m_mouseDelayVal;
	                    m_leftMouseReleasedAfterPress = false;
	                    m_numAttackDice = 1;
	                    //m_sd.buttonStatus[0] = BUTTON_PRESSED;
                        //m_sd.buttonStatus[1] = BUTTON_INACTIVE;
                        //m_sd.buttonStatus[2] = BUTTON_INACTIVE;
	                } 
	                else if (m_mouseButtonStateLeft == GLFW_PRESS && 
	                         m_mouseDelay == 0 && 
	                         b == 1 && 
	                         m_sd.troopCounts[m_sd.attackingTerritory] > 2) 
	                {
	                    m_mouseDelay = m_mouseDelayVal;
	                    m_leftMouseReleasedAfterPress = false;
	                    m_numAttackDice = 2;
	                    //m_sd.buttonStatus[0] = BUTTON_INACTIVE;
                        //m_sd.buttonStatus[1] = BUTTON_PRESSED;
                        //m_sd.buttonStatus[2] = BUTTON_INACTIVE;
	                }
	                else if (m_mouseButtonStateLeft == GLFW_PRESS && 
	                         m_mouseDelay == 0 && 
	                         b == 2 && 
	                         m_sd.troopCounts[m_sd.attackingTerritory] > 3) 
	                {
	                    m_mouseDelay = m_mouseDelayVal;
	                    m_leftMouseReleasedAfterPress = false;
	                    m_numAttackDice = 3;
	                    //m_sd.buttonStatus[0] = BUTTON_INACTIVE;
                        //m_sd.buttonStatus[1] = BUTTON_INACTIVE;
                        //m_sd.buttonStatus[2] = BUTTON_PRESSED;
	                }
	                else if (m_mouseButtonStateLeft == GLFW_PRESS && 
	                         m_mouseDelay == 0 && 
	                         b == 3 && 
	                         m_sd.troopCounts[m_sd.defendingTerritory] >= 1) 
	                {
	                    m_mouseDelay = m_mouseDelayVal;
	                    m_leftMouseReleasedAfterPress = false;
	                    m_numDefendDice = 1;
	                    //m_sd.buttonStatus[3] = BUTTON_PRESSED;
                        //m_sd.buttonStatus[4] = BUTTON_INACTIVE;
	                }
	                else if (m_mouseButtonStateLeft == GLFW_PRESS && 
	                         m_mouseDelay == 0 && 
	                         b == 4 && 
	                         m_sd.troopCounts[m_sd.defendingTerritory] >= 2) 
	                {
	                    m_mouseDelay = m_mouseDelayVal;
	                    m_leftMouseReleasedAfterPress = false;
	                    m_numDefendDice = 2;
	                    //m_sd.buttonStatus[3] = BUTTON_INACTIVE;
                        //m_sd.buttonStatus[4] = BUTTON_PRESSED;
	                }
                } 
                else if (MapData::MAP[m_sd.attackingTerritory][m_sd.defendingTerritory] && 
                         m_sd.troopCounts[m_sd.attackingTerritory] == 1) 
                {
                    if (DEBUG) printf("move is illegal due to not having enough attackign units\n");
                } 
                else 
                {
                    if (DEBUG) printf("move is illegal due to not being connected\n");
                }
            } 
            else if (m_attackerSelected && !m_defenderSelected && !m_sd.territoriesHidden) 
            {
                //printf("attacker selected\n");
                if (m_mouseButtonStateLeft == GLFW_PRESS && 
                    m_mouseDelay == 0 && 
                    b == 0 && 
                    m_sd.troopCounts[m_sd.attackingTerritory] > 1)
                {
                    m_mouseDelay = m_mouseDelayVal;
                    m_leftMouseReleasedAfterPress = false;
                    m_numAttackDice = 1;
                    //m_sd.buttonStatus[0] = BUTTON_PRESSED;
                } 
                else if (m_mouseButtonStateLeft == GLFW_PRESS && 
                         m_mouseDelay == 0 && 
                         b == 1 && 
                         m_sd.troopCounts[m_sd.attackingTerritory] > 2) 
                {
                    m_mouseDelay = m_mouseDelayVal;
                    m_leftMouseReleasedAfterPress = false;
                    m_numAttackDice = 2;
                    //m_sd.buttonStatus[1] = BUTTON_PRESSED;
                }
                else if (m_mouseButtonStateLeft == GLFW_PRESS && 
                         m_mouseDelay == 0 && 
                         b == 2 && 
                         m_sd.troopCounts[m_sd.attackingTerritory] > 3) 
                {
                    m_mouseDelay = m_mouseDelayVal;
                    m_leftMouseReleasedAfterPress = false;
                    m_numAttackDice = 3;
                    //m_sd.buttonStatus[2] = BUTTON_PRESSED;
                }
            } 
            else if (!m_attackerSelected && m_defenderSelected && !m_sd.territoriesHidden) 
            {
                //printf("defender selected\n");
                if (m_mouseButtonStateLeft == GLFW_PRESS && 
                    m_mouseDelay == 0 && 
                    b == 3 && 
                    m_sd.troopCounts[m_sd.defendingTerritory] >= 1) 
                {
                    m_mouseDelay = m_mouseDelayVal;
                    m_leftMouseReleasedAfterPress = false;
                    m_numDefendDice = 1;
                    //m_sd.buttonStatus[3] = BUTTON_PRESSED;
                }
                else if (m_mouseButtonStateLeft == GLFW_PRESS && 
                         m_mouseDelay == 0 && 
                         b == 4 && 
                         m_sd.troopCounts[m_sd.defendingTerritory] >= 2) 
                {
                    m_mouseDelay = m_mouseDelayVal;
                    m_leftMouseReleasedAfterPress = false;
                    m_numDefendDice = 2;
                    //m_sd.buttonStatus[4] = BUTTON_PRESSED;
                }
            } 
            if (b == 8 && m_mouseButtonStateLeft == GLFW_PRESS && m_mouseDelay == 0 && !m_sd.showCards) 
            {
                // view/hide territories
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
                m_sd.territoriesHidden = !m_sd.territoriesHidden;
            }
            if (b == 6 && m_mouseButtonStateLeft == GLFW_PRESS && m_mouseDelay == 0 && !m_sd.territoriesHidden) 
            {
                // view/hide territories
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
                m_sd.showCards = !m_sd.showCards;
            }
            if ((glfwGetKey(m_sd.window, GLFW_KEY_E) == GLFW_PRESS || b == 9 && m_mouseButtonStateLeft == GLFW_PRESS) && 
                m_mouseDelay == 0  && !m_sd.territoriesHidden) 
            {
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
                b = -1;
                if (DEBUG) printf("end attacking phase\n");
                m_attackerSelected = false;
                m_defenderSelected = false;
                m_sd.territoriesHidden = false;
                m_sd.showCards = false;
                m_sd.gamePhase = FORTIFY;
            }
            for (uint ii = 0; ii < MapData::NUM_TERRITORIES; ii++)
                m_sd.isHighlighted[ii] = false;
            if (m_attackerSelected) 
                m_sd.isHighlighted[m_sd.attackingTerritory] = true; 
            if (m_defenderSelected) 
                m_sd.isHighlighted[m_sd.defendingTerritory] = true;
            if (m_mouseDelay > 0) 
                m_mouseDelay--;
        }
        ///////////////////////////////////////
        else if (m_sd.gamePhase == ATTACK_MOVE)
        {
            if (m_mouseButtonStateLeft == GLFW_PRESS && 
                m_mouseDelay == 0 && 
                i == m_sd.attackingTerritory && 
                m_sd.troopCounts[m_sd.defendingTerritory] > m_numAttackDice && 
                !m_sd.territoriesHidden) 
            { // click on attacking territory (min is 1)
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
                m_sd.troopCounts[m_sd.attackingTerritory]++;
                m_sd.troopCounts[m_sd.defendingTerritory]--;
            }
            else if (m_mouseButtonStateLeft == GLFW_PRESS && 
                     m_mouseDelay == 0 && 
                     i == m_sd.defendingTerritory && 
                     m_sd.troopCounts[m_sd.attackingTerritory] > 1 && 
                     !m_sd.territoriesHidden)
            { // click on defending territory (min is num attack dice)
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
                m_sd.troopCounts[m_sd.attackingTerritory]--;
                m_sd.troopCounts[m_sd.defendingTerritory]++;
            }
            else if (((m_mouseButtonStateLeft == GLFW_PRESS && b == 7) || glfwGetKey(m_sd.window, GLFW_KEY_N) == GLFW_PRESS) && 
                     m_mouseDelay == 0 && !m_sd.territoriesHidden)
            {
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
                m_sd.attackingTerritory = m_sd.defendingTerritory; // carry on to the front of the battle
                m_sd.defendingTerritory = NULL_TERRITORY;
                m_defenderSelected = false;
                m_sd.territoriesHidden = false;
                m_sd.showCards = false;
                m_sd.gamePhase = ATTACK;
                if (m_sd.troopCounts[m_sd.attackingTerritory] > 3)
                {
                    m_numAttackDice = 3; 
                    //m_sd.buttonStatus[2] = BUTTON_PRESSED;
                    if (DEBUG) printf("setting default to BUTTON_PRESSED 3 dice\n");
                } 
                else if (m_sd.troopCounts[m_sd.attackingTerritory] == 3)
                {
                    m_numAttackDice = 2;
                    //m_sd.buttonStatus[1] = BUTTON_PRESSED;
                }
                else if (m_sd.troopCounts[m_sd.attackingTerritory] == 2)
                {
                    m_numAttackDice = 1;
                    //m_sd.buttonStatus[0] = BUTTON_PRESSED;
                }
                else
                {
                    m_attackerSelected = false;
                    m_sd.attackingTerritory = NULL_TERRITORY;
                    m_numAttackDice = 0;
                }
            }
            if (b == 8 && m_mouseButtonStateLeft == GLFW_PRESS && m_mouseDelay == 0 && !m_sd.showCards) 
            {
                // view/hide territories
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
                m_sd.territoriesHidden = !m_sd.territoriesHidden;
            }
            if (b == 6 && m_mouseButtonStateLeft == GLFW_PRESS && m_mouseDelay == 0 && !m_sd.territoriesHidden) 
            {
                // view/hide territories
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
                m_sd.showCards = !m_sd.showCards;
            }
            for (uint ii = 0; ii < MapData::NUM_TERRITORIES; ii++)
                m_sd.isHighlighted[ii] = false;
            if (m_attackerSelected) 
                m_sd.isHighlighted[m_sd.attackingTerritory] = true; 
            if (m_defenderSelected) 
                m_sd.isHighlighted[m_sd.defendingTerritory] = true;
            if (m_mouseDelay > 0) 
                m_mouseDelay--;
        }
        ///////////////////////////////////
        else if (m_sd.gamePhase == FORTIFY) 
        {
            // left click to select 2 adjacent territories
            // continue left clicking to bring units over to that territory out of the two
            // right click to deselect. goes back 1 at a time out fo the two or one selected territories. if moves were made, they are undone.
            if (m_mouseButtonStateLeft == GLFW_PRESS && 
                m_sd.territoryMarkers[i] == m_sd.activePlayerIndex &&
                m_mouseDelay == 0 && 
                i > -1 && 
                !m_sd.territoriesHidden)
	        {
	            m_mouseDelay = m_mouseDelayVal;
	            m_leftMouseReleasedAfterPress = false;
                if (//m_sd.territoryMarkers[i] == m_sd.activePlayerIndex && 
                    !m_startSelected && !m_endSelected) 
                {
                    m_startSelected = true;
                    m_sd.startTerritory = i;
                    m_sd.isHighlighted[m_sd.startTerritory] = true;
                    m_originalStartTroopCount = m_sd.troopCounts[m_sd.startTerritory];
                }
                else if (//m_sd.territoryMarkers[i] == m_sd.activePlayerIndex && 
                         m_sd.startTerritory != i &&
                         m_startSelected && !m_endSelected &&
                         MapData::MAP[m_sd.startTerritory][i])
                {
                    m_endSelected = true;
                    m_sd.endTerritory = i;
                    m_sd.isHighlighted[m_sd.endTerritory] = true;
                    m_originalEndTroopCount = m_sd.troopCounts[m_sd.endTerritory];
                }
                /*
                else if (m_sd.startTerritory == i && 
                         m_startSelected && !m_endSelected)
                {
                    //m_startSelected = false;
                    //m_sd.startTerritory = NULL_TERRITORY;
                }
                */
                else if (m_sd.startTerritory == i && 
                         m_startSelected && m_endSelected &&
                         MapData::MAP[m_sd.startTerritory][m_sd.endTerritory])
                {
                    //m_startSelected = false;
                    //m_sd.startTerritory = NULL_TERRITORY;
                    //m_endSelected = false;
                    //m_sd.endTerritory = NULL_TERRITORY;
                    if (m_sd.troopCounts[m_sd.endTerritory] > 1)
                    {
                        m_sd.troopCounts[m_sd.startTerritory]++;
                        m_sd.troopCounts[m_sd.endTerritory]--;
                    }
                }
                else if (m_sd.endTerritory == i && 
                         m_startSelected && m_endSelected &&
                         MapData::MAP[m_sd.startTerritory][m_sd.endTerritory])
                {
                    //m_endSelected = false;
                    //m_sd.endTerritory = NULL_TERRITORY;
                    if (m_sd.troopCounts[m_sd.startTerritory] > 1)
                    {
                        m_sd.troopCounts[m_sd.startTerritory]--;
                        m_sd.troopCounts[m_sd.endTerritory]++;
                    }
                }
                else //if (m_sd.startTerritory != i &&
                     //    m_sd.endTerritory != i && 
                     //    m_startSelected && m_endSelected)
                {
                    if (DEBUG) printf("deselecting and undoing\n");
                    m_sd.troopCounts[m_sd.startTerritory] = m_originalStartTroopCount;
                    m_sd.troopCounts[m_sd.endTerritory] = m_originalEndTroopCount;
                    m_startSelected = false;
                    m_sd.isHighlighted[m_sd.startTerritory] = false;
                    m_sd.startTerritory = NULL_TERRITORY;
                    m_originalStartTroopCount = 0;
                    m_endSelected = false;
                    m_sd.isHighlighted[m_sd.endTerritory] = false;
                    m_sd.endTerritory = NULL_TERRITORY;
                    m_originalEndTroopCount = 0;
                }
            }
            else if ((m_mouseButtonStateRight == GLFW_PRESS || (m_mouseButtonStateLeft == GLFW_PRESS && m_sd.territoryMarkers[i] != m_sd.activePlayerIndex)) && 
                     m_mouseDelay == 0 && 
                     i > -1 && 
                     !m_sd.territoriesHidden)
	        {
	            m_mouseDelay = m_mouseDelayVal;
	            m_rightMouseReleasedAfterPress = false;
                if (m_startSelected && !m_endSelected)
                {
                    m_startSelected = false;
                    m_sd.isHighlighted[m_sd.startTerritory] = false;
                    m_sd.endTerritory = NULL_TERRITORY;
                    m_originalStartTroopCount = 0;
                }
                else if (m_startSelected && m_endSelected)
                {
                    if (DEBUG) printf("undo move\n");
                    m_sd.troopCounts[m_sd.startTerritory] = m_originalStartTroopCount;
                    m_sd.troopCounts[m_sd.endTerritory] = m_originalEndTroopCount;
                    m_startSelected = false;
                    m_sd.isHighlighted[m_sd.startTerritory] = false;
                    m_sd.startTerritory = NULL_TERRITORY;
                    m_originalStartTroopCount = 0;
                    m_endSelected = false;
                    m_sd.isHighlighted[m_sd.endTerritory] = false;
                    m_sd.endTerritory = NULL_TERRITORY;
                    m_originalEndTroopCount = 0;
                }
            }
            if (b == 8 && m_mouseButtonStateLeft == GLFW_PRESS && m_mouseDelay == 0 && !m_sd.showCards) 
            {
                // view/hide territories
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
                m_sd.territoriesHidden = !m_sd.territoriesHidden;
            }
            if (b == 6 && m_mouseButtonStateLeft == GLFW_PRESS && m_mouseDelay == 0 && !m_sd.territoriesHidden) 
            {
                // view/hide territories
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
                m_sd.showCards = !m_sd.showCards;
            }
            if ((glfwGetKey(m_sd.window, GLFW_KEY_E) == GLFW_PRESS || b == 9 && m_mouseButtonStateLeft == GLFW_PRESS) && 
                     m_mouseDelay == 0 && !m_sd.territoriesHidden) 
            {
                m_mouseDelay = m_mouseDelayVal;
                m_leftMouseReleasedAfterPress = false;
            
                // make no move
                m_startSelected = false;
                m_endSelected = false;
                m_sd.isHighlighted[m_sd.startTerritory] = false;
                m_sd.isHighlighted[m_sd.endTerritory] = false;
                m_sd.startTerritory = NULL_TERRITORY;
                m_sd.endTerritory = NULL_TERRITORY;
                m_originalStartTroopCount = 0;
                m_originalEndTroopCount = 0;
                m_sd.territoriesHidden = false;
                m_sd.showCards = false;
                if (_gameIsOver())
                {
                    m_sd.gamePhase = END;
                }
                else
                {
                    if (m_conqueredTerritory)
                    {
                        (*(m_sd.players))[m_sd.activePlayerIndex]->takeCard(m_cardDeck.back()); // draw from back, insert into front
                        m_cardDeck.pop_back();
                    }
                    m_conqueredTerritory = false;
                    m_sd.gamePhase = ARMIES;
                    m_sd.totalNumNewUnits = 0;
                    m_sd.numNewUnits = 0;
                    m_numNewUnitsPlaced = 0;
                    m_sd.activePlayerIndex = (m_sd.activePlayerIndex + 1) % 2;
                    _calcNumNewUnits();
                }
                
                if (m_sd.activePlayerIndex == 0) 
                    m_sd.turnNum++;
            } 
            if (m_mouseDelay > 0) 
                m_mouseDelay--;
        }
        //////////////////////////////
        else if (m_sd.gamePhase = END)
        {
            if (DEBUG) printf("Player %i is the winner.\n", m_sd.winner);
        }
    //printf("after main if struct button status [0] = %i\n", static_cast<int>(m_sd.buttonStatus[0]));
    //printf("after main if struct button status [1] = %i\n", static_cast<int>(m_sd.buttonStatus[1]));
    //printf("after main if struct button status [2] = %i\n", static_cast<int>(m_sd.buttonStatus[2]));
    //if (DEBUG) printf("end of update: m_plus2Selected = %i\n", m_plus2Selected);
    //if (DEBUG) 
    //{
    //    for (uint i = 0; i < 5; i++)   
    //        printf("update end: m_sd.plus2Status[%i] = %i\n", i, m_sd.plus2Status[i]);
    //}
    }
    else
    {
        //if (DEBUG) printf("mouse button releases state.\n");
        // check to see if the mouse buttons were released
        //m_leftMouseReleasedAfterPress && m_rightMouseReleasedAfterPress
        if (m_mouseButtonStateRight == GLFW_RELEASE) m_rightMouseReleasedAfterPress = true;
        if (m_mouseButtonStateLeft == GLFW_RELEASE) m_leftMouseReleasedAfterPress = true;
    }
}

bool GameBoard::_gameIsOver()
{
    //check to see if the game is over
    // eventually all green will be taken out (likeli hood is very high) so we only consider when one color player is wiped out
    bool isAnyP0 = false;
    bool isAnyP1 = false;
    for (uint i = 0; i < MapData::NUM_TERRITORIES; i++)
    {
        isAnyP0 = isAnyP0 || (0 == m_sd.territoryMarkers[i]);
        isAnyP1 = isAnyP1 || (1 == m_sd.territoryMarkers[i]);
    }
    if (isAnyP0 && !isAnyP1) 
    {
        m_sd.winner = 0;
        return true;
    }
    else if (!isAnyP0 && isAnyP1)
    {
        m_sd.winner = 1;
        return true;
    }
    return false;
}

void GameBoard::_calculateUnitsFromCards()
{
    if (m_cardUsageCounter == 0)
    {
        m_sd.totalNumNewUnits += 4;
        m_sd.numNewUnits += 4;
    }
    else if (m_cardUsageCounter == 1)
    {
        m_sd.totalNumNewUnits += 6;
        m_sd.numNewUnits += 6;
    }
    else if (m_cardUsageCounter == 2)
    {
        m_sd.totalNumNewUnits += 8;
        m_sd.numNewUnits += 8;
    }
    else if (m_cardUsageCounter == 3)
    {
        m_sd.totalNumNewUnits += 10;
        m_sd.numNewUnits += 10;
    }
    else if (m_cardUsageCounter == 4)
    {
        m_sd.totalNumNewUnits += 12;
        m_sd.numNewUnits += 12;
    }
    else //if (m_cardUsageCounter <= 5)
    {
        uint tmp = 15 + pow(5, m_cardUsageCounter - 5);
        m_sd.totalNumNewUnits += tmp;
        m_sd.numNewUnits += tmp;
    }
    m_cardUsageCounter++;
}

void GameBoard::_determineAttackOutcome()
{
    m_sd.numAttackingUnitsLost = 0;
    m_sd.numDefendingUnitsLost = 0;
    
    if (m_numAttackDice == 0) //TODO error throwing or something else here?
    {
        printf("There are no attacking dice. Result is 0.");
        return;
    }
    if (m_numDefendDice == 0)
    {
        printf("There are no defending dice. Result is 0.");
        return;
    }
    
    std::vector<uint> attackDice;
    std::vector<uint> defendDice;
    
    struct {
        bool operator()(uint a, uint b)
        {   
            return a > b;
        }   
    } customGreat;
    
    // roll dice
    for (uint i = 0; i < m_numAttackDice; i++)
        attackDice.push_back(rand() % 6);
    std::sort(attackDice.begin(), attackDice.end(), customGreat);
    
    for (uint i = 0; i < m_numDefendDice; i++)
        defendDice.push_back(rand() % 6);
    std::sort(defendDice.begin(), defendDice.end(), customGreat);
    
    // compare results
    for (uint i = 0; i < std::min(m_numAttackDice, m_numDefendDice); i++) 
    {
        if (attackDice[i] > defendDice[i]) 
        {
            m_sd.numDefendingUnitsLost++;
        }
        else
        {
            m_sd.numAttackingUnitsLost++;
        }
    }
}

void GameBoard::_calcNumNewUnits() 
{
    m_sd.numNewUnits = 0;
    
    // for number of territories controlled
    // total territories controlled divided by 3 ignoring fractions with the min at 3
    uint territoriesControlled = 0;
    for (uint i = 0; i < MapData::NUM_TERRITORIES; i++)
    {
        if (m_sd.territoryMarkers[i] == m_sd.activePlayerIndex)
            territoriesControlled++;
    }
    m_sd.numNewUnits = floor(territoriesControlled / 3); //TODO verify that this is integer devision and leaves out the remainders
    if (m_sd.numNewUnits < 3) m_sd.numNewUnits = 3;
    if (DEBUG) printf("in _calcNumNewUnits; num territories controlled = %i\n", territoriesControlled);
    if (DEBUG) printf("in _calcNumNewUnits; m_sd.numNewUnits = %i\n", m_sd.numNewUnits);
    // for countries controlled
    // asia 7
    if (m_sd.territoryMarkers[6] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[7] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[8] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[9] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[10] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[11] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[12] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[13] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[14] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[15] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[16] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[17] == m_sd.activePlayerIndex)
    {
        m_sd.numNewUnits += 7;
    }
    // north america 5
    if (m_sd.territoryMarkers[29] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[30] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[31] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[32] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[33] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[34] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[35] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[36] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[37] == m_sd.activePlayerIndex)
    {
        m_sd.numNewUnits += 5;
    }
    // europe 5
    if (m_sd.territoryMarkers[22] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[23] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[24] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[25] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[26] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[27] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[28] == m_sd.activePlayerIndex)
    {
        m_sd.numNewUnits += 5;
    }
    // africa 3
    if (m_sd.territoryMarkers[0] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[1] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[2] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[3] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[4] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[5] == m_sd.activePlayerIndex)
    {
        m_sd.numNewUnits += 3;
    }
    // australia 2
    if (m_sd.territoryMarkers[18] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[19] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[20] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[21] == m_sd.activePlayerIndex)
    {
        m_sd.numNewUnits += 2;
    }
    // south america 2
    if (m_sd.territoryMarkers[38] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[39] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[40] == m_sd.activePlayerIndex &&
        m_sd.territoryMarkers[41] == m_sd.activePlayerIndex)
    {
        m_sd.numNewUnits += 2;
    }
    
    (*(m_sd.players))[m_sd.activePlayerIndex]->setNumNewUnits(m_sd.numNewUnits);
    if (DEBUG) printf("in _calcNumNewUnits; exiting; m_sd.numNewUnits = %i\n", m_sd.numNewUnits);
    // do RISK card armies elsewhere since they are not automatic at the beginning of a player's turn
    m_sd.totalNumNewUnits = m_sd.numNewUnits;
}

Territory GameBoard::_whichMarkerWasClicked()
{
    for (uint i = 0; i < MapData::NUM_TERRITORIES; i++) 
    {
        if (m_sd.markerPositionData[2 * i + 0] - MapData::TERRITORY_HALF_EXTENT < m_cursorX && 
            m_cursorX < m_sd.markerPositionData[2 * i + 0] + MapData::TERRITORY_HALF_EXTENT && 
            m_sd.markerPositionData[2 * i + 1] - MapData::TERRITORY_HALF_EXTENT < m_cursorY && 
            m_cursorY < m_sd.markerPositionData[2 * i + 1] + MapData::TERRITORY_HALF_EXTENT) 
        {
            return static_cast<Territory>(i);
        }
    }
    return NULL_TERRITORY;
}

int GameBoard::_whichButtonIsCursorOver()
{
    for (uint i = 0; i < MapData::NUM_BUTTONS; i++) 
    {
        if (m_sd.buttonModifierData[4 * i + 0] - MapData::BUTTON_HALF_WIDTHS[i] < m_cursorX && 
            m_cursorX < m_sd.buttonModifierData[4 * i + 0] + MapData::BUTTON_HALF_WIDTHS[i] && 
            m_sd.buttonModifierData[4 * i + 1] - MapData::BUTTON_HEIGHT < m_cursorY && 
            m_cursorY < m_sd.buttonModifierData[4 * i + 1] + MapData::BUTTON_HEIGHT) //TODO calc got screwed up so we use buttonheigh instead of half hieght but this shoudl be cleared up 
        {
            return i;
        }
    }
    return -1;
}

int GameBoard::_whichCardIsCursorOver()
{
    for (uint i = 0; i < 5; i++) 
    {
        if (m_sd.cardPositionData[2 * i + 0] - (1.0f + X_EXTENT) / 2.0f < m_cursorX && 
            m_cursorX < 0.0f /* m_sd.cardPositionData[2 * i + 0] + (1.0f + X_EXTENT) / 2.0f */ && 
            m_sd.cardPositionData[2 * i + 1] - 2.0f / 20.0f < m_cursorY && 
            m_cursorY < m_sd.cardPositionData[2 * i + 1] + 2.0f / 20.0f)
        {
            return i;
        }
    }
    return -1;
}

int GameBoard::_whichPlus2IsCursorOver()
{
    for (uint i = 0; i < 5; i++) 
    {
        if (m_sd.plus2PositionData[2 * i + 0] - X_EXTENT / 2.0f < m_cursorX && 
            m_cursorX < m_sd.plus2PositionData[2 * i + 0] + X_EXTENT / 2.0f && 
            m_sd.plus2PositionData[2 * i + 1] - 2.0f / 20.0f < m_cursorY && 
            m_cursorY < m_sd.plus2PositionData[2 * i + 1] + 2.0f / 20.0f)
        {
            return i;
        }
    }
    return -1;
}

void GameBoard::_areCardsLegalToTurnIn()
{
    if (m_sd.selectedCards.size() == 3) //TODO m_sd.selectedCards is not correct. these only give indices to the cards held by the player
    {
        std::vector<Card> * cards = (*(m_sd.players))[m_sd.activePlayerIndex]->getCards();
        if (((*cards)[m_sd.selectedCards[0]].numUnits == (*cards)[m_sd.selectedCards[1]].numUnits && (*cards)[m_sd.selectedCards[0]].numUnits == (*cards)[m_sd.selectedCards[2]].numUnits) ||
            ((*cards)[m_sd.selectedCards[0]].numUnits != (*cards)[m_sd.selectedCards[1]].numUnits && (*cards)[m_sd.selectedCards[1]].numUnits != (*cards)[m_sd.selectedCards[2]].numUnits && (*cards)[m_sd.selectedCards[2]].numUnits != (*cards)[m_sd.selectedCards[0]].numUnits))
        {
            m_sd.cardsAreLegalToTurnIn = true;
            if (m_sd.territoryMarkers[(*cards)[m_sd.selectedCards[0]].territory] != m_sd.activePlayerIndex && 
                m_sd.territoryMarkers[(*cards)[m_sd.selectedCards[1]].territory] != m_sd.activePlayerIndex && 
                m_sd.territoryMarkers[(*cards)[m_sd.selectedCards[2]].territory] != m_sd.activePlayerIndex)
            {
                m_noPlus2InSelected3 = true;
            }
            else
            {
                m_noPlus2InSelected3 = false;
            }
        }
        else
        {
            m_sd.cardsAreLegalToTurnIn = false;
            m_noPlus2InSelected3 = false;
        }
    }
    else
    {
        m_sd.cardsAreLegalToTurnIn = false;
        m_noPlus2InSelected3 = false;
    }
}

SharedData * GameBoard::getSharedData()
{
    return m_sdPtr;
}

template<typename T>
void GameBoard::_shuffleArray(T * array, uint length, uint iterations) 
{
    T swapVal;
    uint swap_i, swap_j;
    for (uint i = 0; i < iterations; i++) 
    {
        swap_i = rand() % length;
        swap_j = rand() % length;
        swapVal = array[swap_i];
        array[swap_i] = array[swap_j];
        array[swap_j] = swapVal;
    }
}

template<typename T>
void GameBoard::_shuffleArray(T * array, uint length) 
{
    _shuffleArray(array, length, 2 * length);
}


