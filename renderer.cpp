#include "renderer.h"
//TODO could probably reduce the data transfers and memory usage by just using 2 spatial dimensions for all of these vertices
//Renderer::Renderer(GameBoard * gameBoard) 
Renderer::Renderer(SharedData * sharedData) 
{
    m_sd = sharedData;
    
    _determineGamePhase();
    
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);
    
    // map
    
    // map rendering programs
    m_mapProgramID = _loadProgram("mapVertexShader.glsl", "mapFragmentShader.glsl");
    
    // map texture
    m_mapTexture = _loadBMP(MapData::MAP_TEXTURE_PATH);
    m_mapTextureID = glGetUniformLocation(m_mapProgramID, "mapTextureSampler");
    
    // map vertices
    float mapVertexData[] = {
        -1.0f, -1.0f, 0.0f,
        X_EXTENT, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        X_EXTENT, 1.0f, 0.0f
    };
    //m_mapVertexData = new float[m_dim * 4];
    for (int i = 0; i < m_dim * 4; i++)
        m_mapVertexData[i] = mapVertexData[i];
    
    glGenBuffers(1, &m_mapVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_mapVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_dim * 4 * sizeof(float), m_mapVertexData, GL_STATIC_DRAW);
    
    m_mapUVModID = glGetUniformLocation(m_mapProgramID, "UVMod");
    m_mapUVMod[0] =  1.0f / (1.0f + X_EXTENT); // for x->u
    m_mapUVMod[1] =  1.0f / (1.0f + 1.0f);    // for y->v
    
    // territory color markers
    
    m_markerProgramID = _loadProgram("markerVertexShader.glsl", "markerFragmentShader.glsl");
    
    float markerVertexData[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };

    for (int i = 0; i < m_dim * 4; i++)
        m_markerVertexData[i] = markerVertexData[i] * MapData::TERRITORY_HALF_EXTENT; 

    glGenBuffers(1, &m_markerVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_markerVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_dim * 4 * sizeof(float), m_markerVertexData, GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_markerPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_markerPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, 2 * MapData::NUM_TERRITORIES * sizeof(float), m_sd->markerPositionData, GL_STATIC_DRAW);

    _setTerritoryColors();
    
    // text for territories

    // Initialize texture
	m_textTexture = _loadDDS(MapData::TEXT_TEXTURE_PATH);
	
	// Initialize VBO
	glGenBuffers(1, &m_textVertexBufferID);
	glGenBuffers(1, &m_textUVBufferID);

	// Initialize Shader
	m_textProgramID = _loadProgram("textVertexShader.glsl", "textFragmentShader.glsl");

	// Initialize uniforms' IDs
	m_textTextureID = glGetUniformLocation(m_textProgramID, "textTextureSampler");
	
	// text for the control panel
	for (uint i = 0; i < MapData::NUM_LABELS; i++)
	    m_controlPanelText[i] = MapData::LABELS[i];
	
	// buttons for control panel
	
	m_buttonProgramID = _loadProgram("buttonVertexShader.glsl", "buttonFragmentShader.glsl");

    float buttonVertexData[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };

    for (int i = 0; i < m_dim * 4; i++)
        m_buttonVertexData[i] = buttonVertexData[i]; 

    glGenBuffers(1, &m_buttonVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_buttonVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_dim * 4 * sizeof(float), m_buttonVertexData, GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_buttonModifierBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_buttonModifierBuffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * MapData::NUM_BUTTONS * sizeof(float), m_sd->buttonModifierData, GL_STATIC_DRAW);

    _setButtonColors();
    
    // card data for when they are shown in place of the map and markers
    
    m_cardProgramID = _loadProgram("cardVertexShader.glsl", "cardFragmentShader.glsl");
    printf("probe 1\n");
    float tmpX = X_EXTENT;
    float tmpY = 2.0f / 20.0f;
    float cardVertexData[m_dim * 4] = { // TODO this will need to be modified at some point to fit the grid structure that we intend to display
        -(1.0f + X_EXTENT) / 2.0f, -tmpY, 0.0f,
        (1.0f + X_EXTENT) / 2.0f, -tmpY, 0.0f,
        -(1.0f + X_EXTENT) / 2.0f,  tmpY, 0.0f,
        (1.0f + X_EXTENT) / 2.0f, tmpY, 0.0f
    };
    
    for (int i = 0; i < m_dim * 4; i++)
        m_cardVertexData[i] = cardVertexData[i]; 
        
    glGenBuffers(1, &m_cardVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_cardVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_dim * 4 * sizeof(float), m_cardVertexData, GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_cardPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_cardPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, 2 * 5 * sizeof(float), m_sd->cardPositionData, GL_STATIC_DRAW);
    
    _setCardColors();
    printf("probe 2\n");
    // plus 2 unit buttons
    
    m_plus2ProgramID = m_cardProgramID;//_loadProgram("cardVertexShader.glsl", "cardFragmentShader.glsl");
    
    float tmpXX = X_EXTENT;
    float tmpYY = 2.0f / 20.0f;
    float plus2VertexData[m_dim * 4] = { // TODO this will need to be modified at some point to fit the grid structure that we intend to display
        -X_EXTENT / 2.0f, -tmpYY, -0.01f,
        X_EXTENT / 2.0f, -tmpYY, -0.01f,
        -X_EXTENT / 2.0f,  tmpYY, -0.01f,
        X_EXTENT / 2.0f, tmpYY, -0.01f
    };
    
    for (int i = 0; i < m_dim * 4; i++)
        m_plus2VertexData[i] = plus2VertexData[i]; 
        
    glGenBuffers(1, &m_plus2VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_plus2VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_dim * 4 * sizeof(float), m_plus2VertexData, GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_plus2PositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_plus2PositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, 2 * 5 * sizeof(float), m_sd->plus2PositionData, GL_STATIC_DRAW);
    printf("probe 3\n");
    _setPlus2Colors();
    printf("probe 4\n");
}

Renderer::~Renderer() 
{
    //delete[] m_gamePhaseIndicator;
    //delete[] m_mapVertexData;
    //delete[] m_sd->markerPositionData;
    //delete[] m_markerColorData;
    //m_gamePhaseIndicator = nullptr;
    //m_mapVertexData = nullptr;
    //m_sd->markerPositionData = nullptr;
    //m_markerColorData = nullptr;
    glDeleteBuffers(1, &m_mapVertexBuffer);
    glDeleteBuffers(1, &m_markerVertexBuffer);
    glDeleteBuffers(1, &m_markerPositionBuffer);
    glDeleteBuffers(1, &m_markerColorBuffer);
    glDeleteBuffers(1, &m_textVertexBufferID);
    glDeleteBuffers(1, &m_textUVBufferID);
    glDeleteTextures(1, &m_mapTextureID);
    glDeleteTextures(1, &m_textTextureID);
    glDeleteProgram(m_mapProgramID);
    glDeleteProgram(m_markerProgramID);
    glDeleteProgram(m_textProgramID);
    
    glDeleteProgram(m_buttonProgramID);
    //delete[] m_buttonVertexData;
    //m_buttonVertexData = nullptr;
    glDeleteBuffers(1, &m_buttonVertexBuffer);
    //delete[] m_sd->buttonModifierData;
    //m_sd->buttonModifierData = nullptr;
    glDeleteBuffers(1, &m_buttonModifierBuffer);
    //delete[] m_buttonColorData;
    //m_buttonColorData = nullptr;
    glDeleteBuffers(1, &m_buttonColorBuffer);
    
    glDeleteVertexArrays(1, &m_vertexArrayID);
    
    glDeleteProgram(m_cardProgramID);
    glDeleteBuffers(1, &m_cardVertexBuffer);
    //m_cardVertexData
    glDeleteBuffers(1, &m_cardPositionBuffer);
    //m_cardPositionData
    glDeleteBuffers(1, &m_cardColorBuffer);
    //m_cardColorData
    
    //glDeleteProgram(m_plus2ProgramID); // plus2 program is actually a reuse of the card program, so we don't need to delete it again
    glDeleteBuffers(1, &m_plus2VertexBuffer);
    //m_cardVertexData
    glDeleteBuffers(1, &m_plus2PositionBuffer);
    //m_cardPositionData
    glDeleteBuffers(1, &m_plus2ColorBuffer);
    //m_cardColorData
}

void Renderer::display() 
{
    
    //if (DEBUG) 
    //{
    //    for (uint i = 0; i < 5; i++)   
    //        printf("display begin: m_sd->plus2Status[%i] = %i\n", i, m_sd->plus2Status[i]);
    //}

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //if (DEBUG) printf("display 1\n");
    if (!m_sd->showCards)
    {
        _drawMap();
    }
    else
    {
        _setCardColors();
        _drawCards();
        _setCardText();
        for (uint i = 0; i < 5; i++) 
        {
            _printText2D(
                m_cardText[i],
                //2, 
                m_sd->cardPositionData[2 * i + 0] - (1.0f + X_EXTENT) / 2.0f + 0.05f,// - m_cardText[i].length() / 8.0f,//;MapData::CARD_HALF_EXTENT,
                m_sd->cardPositionData[2 * i + 1] - (2.0f / 20.0f) / 2.0f + 0.0005f,// - (m_cardText[i].length() / 8.0f) / 2.0f,//;MapData::CARD_HALF_EXTENT / 2.0f, 
                1.0f / 16.0f
            );
        }
        
        _setPlus2Colors();
        _drawPlus2();
        _setPlus2Text();
        for (uint i = 0; i < 5; i++) 
        {
            _printText2D(
                m_plus2Text[i],
                //2, 
                m_sd->plus2PositionData[2 * i + 0] - X_EXTENT / 2.0f + 0.05f,// - m_cardText[i].length() / 8.0f,//;MapData::CARD_HALF_EXTENT,
                m_sd->plus2PositionData[2 * i + 1] - (2.0f / 20.0f) / 2.0f + 0.0005f,// - (m_cardText[i].length() / 8.0f) / 2.0f,//;MapData::CARD_HALF_EXTENT / 2.0f, 
                1.0f / 16.0f
            );
        }
    }
    
    
    for (uint i = 0; i < MapData::NUM_LABELS; i++)
        m_controlPanelText[i] = MapData::LABELS[i];
    
    // indicate which player holds the turn
    if (m_sd->activePlayerIndex == 0) 
        m_controlPanelText[2] = MapData::LABELS[2] + "Blue";
    else 
        m_controlPanelText[2] = MapData::LABELS[2] + "Red";
    
    switch (m_sd->gamePhase)
    {
        case SETUP:
            m_controlPanelText[12] = MapData::ALT_LABELS[4];
            break;
    
        case SETUP_FINAL:
            m_controlPanelText[14] = MapData::LABELS[14];
            break;
    
        case ARMIES:
            m_controlPanelText[10] = MapData::LABELS[10];
            m_controlPanelText[11] = MapData::LABELS[11];
            m_controlPanelText[13] = MapData::LABELS[13];
            m_controlPanelText[14] = MapData::LABELS[14];
            break;
    
        case ATTACK:
            _determineAttackerAndDefender();
            m_controlPanelText[12] = MapData::LABELS[12];
            m_controlPanelText[14] = MapData::LABELS[14];
            break;
            
        case ATTACK_MOVE:
            m_controlPanelText[12] = MapData::ALT_LABELS[3];
            break;
            
        case FORTIFY:
            _determineStartAndEndTerritories();
            m_controlPanelText[12] = MapData::ALT_LABELS[2];
            break;
            
        case END:
            m_controlPanelText[12] = MapData::ALT_LABELS[2];
            break;
    }
    
    for (uint i = 0; i < MapData::NUM_BUTTONS; i++)
        if (m_sd->buttonStatus[i] == BUTTON_HIDDEN) m_controlPanelText[MapData::BUTTON_2_LABEL[i]] = MapData::ALT_LABELS[6];
    
    if (!m_sd->territoriesHidden && !m_sd->showCards)
    {
        _setTerritoryColors();
        _drawMarkers();
        
        //if (DEBUG) printf("here\n");
        _setCounterText();
        for (uint i = 0; i < MapData::NUM_TERRITORIES; i++) 
        {
            _printText2D(
                m_counterText[i], 
                2, 
                m_sd->markerPositionData[2 * i + 0] - MapData::TERRITORY_HALF_EXTENT,
                m_sd->markerPositionData[2 * i + 1] - MapData::TERRITORY_HALF_EXTENT / 2.0f, 
                MapData::TERRITORY_HALF_EXTENT
            );
        }
    }
    
    _determineGamePhase();
    _determineTurnNum();
    
    _determineNumNewUnits();
    _determineAttackResult();
    
    _setButtonColors();
    _drawButtons();
    
    for (uint i = 0; i < MapData::NUM_LABELS; i++)
    {
        _printText2D(
            m_controlPanelText[i],
            //m_controlPanelText[i].length(),
            MapData::LABEL_CENTERS[i][0] - MapData::LABEL_CHAR_SIZE / 2.0f,//X_EXTENT + 0.05f,
            MapData::LABEL_CENTERS[i][1] - MapData::LABEL_CHAR_SIZE / 2.0f,//1.0f - static_cast<float>(3 * i + 3) / 24.0f,
            MapData::LABEL_HEIGHT
        );
    }
    //printf("after display button status [0] = %i\n", static_cast<int>(m_sd->buttonStatus[0]));
    //printf("after display button status [1] = %i\n", static_cast<int>(m_sd->buttonStatus[1]));
    //printf("after display button status [2] = %i\n", static_cast<int>(m_sd->buttonStatus[2]));
    //if (DEBUG) 
    //{
    //    for (uint i = 0; i < 5; i++)   
    //        printf("display end: m_sd->plus2Status[%i] = %i\n", i, m_sd->plus2Status[i]);
    //}
}

void Renderer::_setTerritoryColors() 
{
    for (int i = 0; i < MapData::NUM_TERRITORIES; i++) 
    {
        if (m_sd->territoryMarkers[i] == 0) 
        {
            if (m_sd->isHighlighted[i])
            {
                m_markerColorData[3 * i + 0] = MapData::LIGHT_BLUE[0];
                m_markerColorData[3 * i + 1] = MapData::LIGHT_BLUE[1];
                m_markerColorData[3 * i + 2] = MapData::LIGHT_BLUE[2];
            }
            else
            {
                m_markerColorData[3 * i + 0] = MapData::BLUE[0];
                m_markerColorData[3 * i + 1] = MapData::BLUE[1];
                m_markerColorData[3 * i + 2] = MapData::BLUE[2];
            }
        } 
        else if (m_sd->territoryMarkers[i] == 1) 
        {
            if (m_sd->isHighlighted[i])
            {
                m_markerColorData[3 * i + 0] = MapData::LIGHT_RED[0];
                m_markerColorData[3 * i + 1] = MapData::LIGHT_RED[1];
                m_markerColorData[3 * i + 2] = MapData::LIGHT_RED[2];
            }
            else
            {
                m_markerColorData[3 * i + 0] = MapData::RED[0];
                m_markerColorData[3 * i + 1] = MapData::RED[1];
                m_markerColorData[3 * i + 2] = MapData::RED[2];
            }
        } 
        else /* m_sd->territoryMarkers[i] == 2 */
        {
            if (m_sd->isHighlighted[i])
            {
                //if (DEBUG) printf("green is highlighted\n");
                m_markerColorData[3 * i + 0] = MapData::LIGHT_GREEN[0];
                m_markerColorData[3 * i + 1] = MapData::LIGHT_GREEN[1];
                m_markerColorData[3 * i + 2] = MapData::LIGHT_GREEN[2];
            }
            else
            {   
                //if (DEBUG) printf("green is not highlighted\n");
                m_markerColorData[3 * i + 0] = MapData::GREEN[0];
                m_markerColorData[3 * i + 1] = MapData::GREEN[1];
                m_markerColorData[3 * i + 2] = MapData::GREEN[2];
            }
        }
    }
    
    glGenBuffers(1, &m_markerColorBuffer); //TODO should this block be done here?
    glBindBuffer(GL_ARRAY_BUFFER, m_markerColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_colorDim * MapData::NUM_TERRITORIES * sizeof(GLubyte), m_markerColorData, GL_STREAM_DRAW);//TODO should not have things like this as static draw
}

void Renderer::_setButtonColors()
{
    for (int i = 0; i < MapData::NUM_BUTTONS; i++) 
    {
        if (m_sd->buttonStatus[i] == BUTTON_INACTIVE) 
        {
            m_buttonColorData[3 * i + 0] = MapData::BUTTON_INACTIVE[0];
            m_buttonColorData[3 * i + 1] = MapData::BUTTON_INACTIVE[1];
            m_buttonColorData[3 * i + 2] = MapData::BUTTON_INACTIVE[2];
        } 
        else if (m_sd->buttonStatus[i] == BUTTON_HIGHLIGHTED) 
        {
            m_buttonColorData[3 * i + 0] = MapData::BUTTON_HIGHLIGHTED[0];
            m_buttonColorData[3 * i + 1] = MapData::BUTTON_HIGHLIGHTED[1];
            m_buttonColorData[3 * i + 2] = MapData::BUTTON_HIGHLIGHTED[2];
        } 
        else if (m_sd->buttonStatus[i] == BUTTON_PRESSED)
        {
            m_buttonColorData[3 * i + 0] = MapData::BUTTON_PRESSED[0];
            m_buttonColorData[3 * i + 1] = MapData::BUTTON_PRESSED[1];
            m_buttonColorData[3 * i + 2] = MapData::BUTTON_PRESSED[2];
        }
        else //(m_sd->buttonStatus[i] == BUTTON_HIDDEN)
        {
            m_buttonColorData[3 * i + 0] = MapData::BUTTON_HIDDEN[0];
            m_buttonColorData[3 * i + 1] = MapData::BUTTON_HIDDEN[1];
            m_buttonColorData[3 * i + 2] = MapData::BUTTON_HIDDEN[2];
        }
    }
    
    //if (DEBUG) printf("in renderer button status = %i\n", static_cast<int>(m_sd->buttonStatus[6]));
    
    glGenBuffers(1, &m_buttonColorBuffer); //TODO should this block be done here?
    glBindBuffer(GL_ARRAY_BUFFER, m_buttonColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_colorDim * MapData::NUM_BUTTONS * sizeof(GLubyte), m_buttonColorData, GL_STREAM_DRAW);
}

void Renderer::_determineAttackResult()
{
    m_controlPanelText[8] = MapData::LABELS[8];
    
    // what color is attacking?
    if (m_sd->activePlayerIndex == 0)
    {
        //m_controlPanelText[8].push_back('B');
        m_controlPanelText[8] += "B";
    }
    else
    {
        //m_controlPanelText[8].push_back('R');
        m_controlPanelText[8] += "R";
    }
    //m_controlPanelText[8].push_back(' ');
    //m_controlPanelText[8].push_back('-');
    m_controlPanelText[8] += "-";
    
    // number attacking units lost
    _controlPanelTextAppendDigits(m_sd->numAttackingUnitsLost, 8);
    //m_controlPanelText[8].push_back(' ');
    m_controlPanelText[8] += " ";
    
    // what color is defending?
    if (m_sd->territoryMarkers[m_sd->defendingTerritory] == 0)
    {
        //m_controlPanelText[8].push_back('B');
        m_controlPanelText[8] += "B";
    }
    else if (m_sd->territoryMarkers[m_sd->defendingTerritory] == 1)
    {
        //m_controlPanelText[8].push_back('R');
        m_controlPanelText[8] += "R";
    }
    else
    {
        //m_controlPanelText[8].push_back('G');
        m_controlPanelText[8] += "G";
    }
    //m_controlPanelText[8].push_back(' ');
    //m_controlPanelText[8].push_back('-');
    m_controlPanelText[8] += "-";
    
    // number defending units lost
    _controlPanelTextAppendDigits(m_sd->numDefendingUnitsLost, 8);
}

void Renderer::_determineNumNewUnits()
{
    m_controlPanelText[9] = MapData::LABELS[9];
    _controlPanelTextAppendDigits(m_sd->numNewUnits, 9);
}

void Renderer::_determineTurnNum()
{
    if (m_sd->gamePhase == SETUP)
    {
        m_controlPanelText[1] = MapData::ALT_LABELS[5];
    }
    else
    {
        m_controlPanelText[1] = MapData::LABELS[1];
    }
    _controlPanelTextAppendDigits(m_sd->turnNum, 1);
}

void Renderer::_determineGamePhase() 
{
    switch (m_sd->gamePhase) 
    {
        case SETUP: m_gamePhaseIndicator = "Setup"; break;
        case ARMIES: m_gamePhaseIndicator = "Armies"; break;
        case ATTACK: m_gamePhaseIndicator = "Attack"; break;
        case FORTIFY: m_gamePhaseIndicator = "Fortify"; break;
        case END: m_gamePhaseIndicator = "End"; break;
    }
    std::string tmp0 = m_controlPanelText[3]; //TODO do we need tmp0 anymore?
    m_controlPanelText[3] = MapData::LABELS[3];// + m_gamePhaseIndicator;
    m_controlPanelText[3] += m_gamePhaseIndicator;
}

void Renderer::_determineAttackerAndDefender()
{
    m_controlPanelText[4] = MapData::LABELS[4];
    //m_controlPanelText[4].insert(m_controlPanelText[4].end(), MapData::TERRITORY_CHARS[m_sd->attackingTerritory].begin(), MapData::TERRITORY_CHARS[m_sd->attackingTerritory].end()); 
    if (m_sd->attackingTerritory != NULL_TERRITORY) m_controlPanelText[4] += MapData::TERRITORY_CHARS[m_sd->attackingTerritory];
    m_controlPanelText[5] = MapData::LABELS[5];
    //m_controlPanelText[5].insert(m_controlPanelText[5].end(), MapData::TERRITORY_CHARS[m_sd->defendingTerritory].begin(), MapData::TERRITORY_CHARS[m_sd->defendingTerritory].end());
    if (m_sd->defendingTerritory != NULL_TERRITORY) m_controlPanelText[5] += MapData::TERRITORY_CHARS[m_sd->defendingTerritory];
}

void Renderer::_determineStartAndEndTerritories()
{
    m_controlPanelText[4] = MapData::ALT_LABELS[0];
    //m_controlPanelText[4].insert(m_controlPanelText[4].end(), MapData::TERRITORY_CHARS[m_sd->startTerritory].begin(), MapData::TERRITORY_CHARS[m_sd->startTerritory].end()); 
    if (m_sd->startTerritory != NULL_TERRITORY) m_controlPanelText[4] += MapData::TERRITORY_CHARS[m_sd->startTerritory];
    m_controlPanelText[5] = MapData::ALT_LABELS[1];
    //m_controlPanelText[5].insert(m_controlPanelText[5].end(), MapData::TERRITORY_CHARS[m_sd->endTerritory].begin(), MapData::TERRITORY_CHARS[m_sd->endTerritory].end());
    if (m_sd->endTerritory != NULL_TERRITORY) m_controlPanelText[5] += MapData::TERRITORY_CHARS[m_sd->endTerritory];
}

void Renderer::_setCounterText() //TODO might change datastructure here to generalize with other similar function for digit to char
{
    //if (DEBUG) printf("set counter text\n");
    uint places[2];
    for (uint i = 0; i < MapData::NUM_TERRITORIES; i++) 
    {
        places[1] = m_sd->troopCounts[i] % 10;
        places[0] = (m_sd->troopCounts[i] - places[1]) / 10;
        
        for (uint j = 0; j < 2; j++) 
        {
            switch (places[j]) 
            {
                case 0: m_counterText[i][j] = '0'; break;
                case 1: m_counterText[i][j] = '1'; break;
                case 2: m_counterText[i][j] = '2'; break;
                case 3: m_counterText[i][j] = '3'; break;
                case 4: m_counterText[i][j] = '4'; break;
                case 5: m_counterText[i][j] = '5'; break;
                case 6: m_counterText[i][j] = '6'; break;
                case 7: m_counterText[i][j] = '7'; break;
                case 8: m_counterText[i][j] = '8'; break;
                case 9: m_counterText[i][j] = '9'; break;
            }
        }
    }
}

void Renderer::_controlPanelTextAppendDigits(uint n, uint i)
{
    uint places[2];
    places[1] = n % 10;
    places[0] = (n - places[1]) / 10;
    for (uint j = 0; j < 2; j++)
    {
        switch (places[j])
        {
            case 0: m_controlPanelText[i] += "0"; break;
            case 1: m_controlPanelText[i] += "1"; break;
            case 2: m_controlPanelText[i] += "2"; break;
            case 3: m_controlPanelText[i] += "3"; break;
            case 4: m_controlPanelText[i] += "4"; break;
            case 5: m_controlPanelText[i] += "5"; break;
            case 6: m_controlPanelText[i] += "6"; break;
            case 7: m_controlPanelText[i] += "7"; break;
            case 8: m_controlPanelText[i] += "8"; break;
            case 9: m_controlPanelText[i] += "9"; break;
        }
    }
}

GLuint Renderer::_loadProgram(const char * shaderPath, const char * fragPath) 
{
    // Create the shaders
	uint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	uint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file.
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(shaderPath, std::ios::in);
	if (vertexShaderStream.is_open()) 
	{
		std::string line = "";
		while (getline(vertexShaderStream, line))
			vertexShaderCode += "\n" + line;
		vertexShaderStream.close();
	} 
	else 
	{
		printf("Failed to open %s.\n", shaderPath);
		return 0;
	}

	// Read the Fragment Shader code from the file.
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(fragPath, std::ios::in);
	if (fragmentShaderStream.is_open()) 
	{
		std::string line = "";
		while (getline(fragmentShaderStream, line))
			fragmentShaderCode += "\n" + line;
		fragmentShaderStream.close();
	} 
	else 
	{
	    printf("Failed to open %s.\n", fragPath);
	    return 0;
	}

	int result = GL_FALSE;
	int infoLogLength; //TODO uint instead?

	// Compile Vertex Shader.
	printf("Compiling shader : %s\n", shaderPath);
	char const* vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader.
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) 
	{
		std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader.
	printf("Compiling shader : %s\n", fragPath);
	char const* fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader.
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) 
	{
		std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		printf("%s\n", &fragmentShaderErrorMessage[0]);
	}

	// Link the program.
	printf("Linking program\n");
	GLuint programID;
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Check the program.
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) 
	{
		std::vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);
	}

	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	
	return programID;
}

GLuint Renderer::_loadBMP(const char * imagepath) 
{
	printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (!file) 
	{
	    printf("%s could not be opened.\n", imagepath); 
	    getchar(); 
	    return 0;
    }

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if (fread(header, 1, 54, file) != 54)
	{ 
		printf("Not a correct BMP file\n");
		return 0;
	}
	
	// A BMP files always begins with "BM"
	if (header[0]!='B' || header[1]!='M')
	{
		printf("Not a correct BMP file\n");
		return 0;
	}
	
	// Make sure this is a 24bpp file
	if (*(int*)&(header[0x1E])!=0)         
	{
	    printf("Not a correct BMP file\n");    
	    return 0;
    }
	if (*(int*)&(header[0x1C])!=24)         
	{
	    printf("Not a correct BMP file\n");    
	    return 0;
    }

	// Read the information about the image
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

    
    if (DEBUG) printf("dataPos = %i\n", dataPos);
    if (DEBUG) printf("(imageSize, width, height) = (%i, %i, %i)\n", imageSize, width, height);
    if (DEBUG) printf("width * height = %i\n",width * height);
    uint paddingSize = (imageSize - 3 * width * height) / height;
    if (DEBUG) printf("paddingSize = %i\n", paddingSize);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54; // The BMP header is done that way

	// Create a buffer
	//data = new unsigned char [imageSize];
	data = new unsigned char [width * height * 3];
    unsigned char junk[paddingSize];

	// Read the actual data from the file into the buffer
	//fread(data,1,imageSize,file);
    uint extraHeader = dataPos - 54;
    if (extraHeader > 0) 
    {
        unsigned char junkHeader[extraHeader];
        fread(junkHeader, 1, extraHeader, file);
    }
    for (uint i = 0; i < height; i++) 
    {
        fread(data, i * width * 3, width * 3, file);
        fread(junk, 1, paddingSize, file);
    }

	// Everything is in memory now, the file wan be closed
	fclose (file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version
	delete [] data;

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	// ... nice trilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
}

void Renderer::_drawMap() 
{
    glUseProgram(m_mapProgramID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_mapTexture);
    glUniform1i(m_mapTextureID, 0);

    glUniform2f(m_mapUVModID, m_mapUVMod[0], m_mapUVMod[1]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_mapVertexBuffer);
    glVertexAttribPointer(
	    0,
	    m_dim,
	    GL_FLOAT,
	    GL_FALSE,
	    0,
	    (void*)0
    );

    glVertexAttribDivisor(0, 0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glDisableVertexAttribArray(0);
}

void Renderer::_setCardColors()
{ 
    // set all markers to hidden
    for (uint i = 0; i < 5; i++) // i is the territory index
    {
        m_cardColorData[3 * i + 0] = MapData::BUTTON_HIDDEN[0];
        m_cardColorData[3 * i + 1] = MapData::BUTTON_HIDDEN[1];
        m_cardColorData[3 * i + 2] = MapData::BUTTON_HIDDEN[2];
    }
    std::vector<Card> * cards = (*(m_sd->players))[m_sd->activePlayerIndex]->getCards();
    for (uint i = 0; i < cards->size(); i++) 
    {    
        //m_cardText[i] = MapData::TERRITORY_CHARS[(*cards)[i].territory] + " " + std::to_string((*cards)[i].numUnits);
        //if (DEBUG) printf("i = %i, m_sd->cardStatus[i] = %i\n", i, static_cast<int>(m_sd->cardStatus[i]));
        if (BUTTON_HIGHLIGHTED == m_sd->cardStatus[i])
        {
            m_cardColorData[3 * i + 0] = MapData::BUTTON_HIGHLIGHTED[0];
            m_cardColorData[3 * i + 1] = MapData::BUTTON_HIGHLIGHTED[1];
            m_cardColorData[3 * i + 2] = MapData::BUTTON_HIGHLIGHTED[2];
        }
        else if (BUTTON_PRESSED == m_sd->cardStatus[i])
        {
            m_cardColorData[3 * i + 0] = MapData::BUTTON_PRESSED[0];
            m_cardColorData[3 * i + 1] = MapData::BUTTON_PRESSED[1];
            m_cardColorData[3 * i + 2] = MapData::BUTTON_PRESSED[2];
        }
        else
        {
            m_cardColorData[3 * i + 0] = MapData::BUTTON_INACTIVE[0];
            m_cardColorData[3 * i + 1] = MapData::BUTTON_INACTIVE[1];
            m_cardColorData[3 * i + 2] = MapData::BUTTON_INACTIVE[2];
        }
    }
    glGenBuffers(1, &m_cardColorBuffer); //TODO should this block be done here?
    glBindBuffer(GL_ARRAY_BUFFER, m_cardColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_colorDim * 5 * sizeof(GLubyte), m_cardColorData, GL_STREAM_DRAW); //TODO should likely not be static draw
}

void Renderer::_setCardText()
{
    // print the territory name
    // print the number of units
    for (uint i = 0; i < 5; i++) 
        m_cardText[i] = " ";
    
    std::vector<Card> * cards = (*(m_sd->players))[m_sd->activePlayerIndex]->getCards();
    for (uint i = 0; i < cards->size(); i++) 
        m_cardText[i] = MapData::TERRITORY_CHARS[(*cards)[i].territory] + " " + std::to_string((*cards)[i].numUnits);
}

void Renderer::_drawCards()
{
    // rows will be continents
    // each row will have a sequence of square button markers that will either be blank or will have a card on it you own
    // there will be no left alignment, only set territory spaces
    // could use similar rendering code to the markers
    
    glUseProgram(m_cardProgramID);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_cardVertexBuffer);
    glVertexAttribPointer(
	    0,
	    m_dim,
	    GL_FLOAT,
	    GL_FALSE,
	    0,
	    (void*)0
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_cardPositionBuffer);
    glVertexAttribPointer(
	    1,
	    2,
	    GL_FLOAT,
	    GL_FALSE,
	    0,
	    (void*)0
    );

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_cardColorBuffer);
    glVertexAttribPointer(
	    2,
	    m_colorDim,
	    GL_UNSIGNED_BYTE,
	    GL_TRUE,
	    0,
	    (void*)0
    );

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 5);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void Renderer::_drawMarkers() 
{
    glUseProgram(m_markerProgramID);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_markerVertexBuffer);
    glVertexAttribPointer(
	    0,
	    m_dim,
	    GL_FLOAT,
	    GL_FALSE,
	    0,
	    (void*)0
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_markerPositionBuffer);
    glVertexAttribPointer(
	    1,
	    2,
	    GL_FLOAT,
	    GL_FALSE,
	    0,
	    (void*)0
    );

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_markerColorBuffer);
    glVertexAttribPointer(
	    2,
	    m_colorDim,
	    GL_UNSIGNED_BYTE,
	    GL_TRUE,
	    0,
	    (void*)0
    );

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, MapData::NUM_TERRITORIES);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void Renderer::_drawButtons() 
{
    glUseProgram(m_buttonProgramID);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_buttonVertexBuffer);
    glVertexAttribPointer(
	    0,
	    m_dim,
	    GL_FLOAT,
	    GL_FALSE,
	    0,
	    (void*)0
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_buttonModifierBuffer);
    glVertexAttribPointer(
	    1,
	    4,
	    GL_FLOAT,
	    GL_FALSE,
	    0,
	    (void*)0
    );

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_buttonColorBuffer);
    glVertexAttribPointer(
	    2,
	    m_colorDim,
	    GL_UNSIGNED_BYTE,
	    GL_TRUE,
	    0,
	    (void*)0
    );

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, MapData::NUM_BUTTONS);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void Renderer::_printText2D(std::string text, 
                            float x, 
                            float y, 
                            float size) 
{
	uint length = text.length();
	//if (DEBUG) printf("printText2D; length = %i\n", length);

	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
	for (unsigned int i=0; i<length; i++)
	{
		
		glm::vec2 vertex_up_left    = glm::vec2( x+i*size/2.0f     , y+size );
		glm::vec2 vertex_up_right   = glm::vec2( x+i*size/2.0f+size/2.0f, y+size );
		glm::vec2 vertex_down_right = glm::vec2( x+i*size/2.0f+size/2.0f, y      );
		glm::vec2 vertex_down_left  = glm::vec2( x+i*size/2.0f     , y      );

		vertices.push_back(vertex_up_left   );
		vertices.push_back(vertex_down_left );
		vertices.push_back(vertex_up_right  );

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character%16)/16.0f;
		float uv_y = (character/16)/16.0f;

		glm::vec2 uv_up_left    = glm::vec2( uv_x           , uv_y );
		glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/16.0f, uv_y );
		glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f, (uv_y + 1.0f/16.0f) );
		glm::vec2 uv_down_left  = glm::vec2( uv_x           , (uv_y + 1.0f/16.0f) );
		UVs.push_back(uv_up_left   );
		UVs.push_back(uv_down_left );
		UVs.push_back(uv_up_right  );

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, m_textVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_textUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_DYNAMIC_DRAW);

	// Bind shader
	glUseProgram(m_textProgramID);

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textTexture);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(m_textTextureID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_textVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_textUVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 0);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	//glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Renderer::_printText2D(const char * text, 
                            uint length, 
                            float x, 
                            float y, 
                            float size) 
{
	//uint length = strlen(text);
	//if (DEBUG) printf("printText2D; length = %i, %c, %c\n", length, text[0], text[1]);

	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
	for (unsigned int i=0; i<length; i++)
	{
		
		glm::vec2 vertex_up_left    = glm::vec2( x+i*size     , y+size );
		glm::vec2 vertex_up_right   = glm::vec2( x+i*size+size, y+size );
		glm::vec2 vertex_down_right = glm::vec2( x+i*size+size, y      );
		glm::vec2 vertex_down_left  = glm::vec2( x+i*size     , y      );

		vertices.push_back(vertex_up_left   );
		vertices.push_back(vertex_down_left );
		vertices.push_back(vertex_up_right  );

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character%16)/16.0f;
		float uv_y = (character/16)/16.0f;

		glm::vec2 uv_up_left    = glm::vec2( uv_x           , uv_y );
		glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/16.0f, uv_y );
		glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f, (uv_y + 1.0f/16.0f) );
		glm::vec2 uv_down_left  = glm::vec2( uv_x           , (uv_y + 1.0f/16.0f) );
		UVs.push_back(uv_up_left   );
		UVs.push_back(uv_down_left );
		UVs.push_back(uv_up_right  );

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, m_textVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_textUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_DYNAMIC_DRAW);

	// Bind shader
	glUseProgram(m_textProgramID);

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textTexture);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(m_textTextureID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_textVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_textUVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 0);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	//glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Renderer::_printText2D(std::vector<char> text, 
                            uint length, 
                            float x, 
                            float y, 
                            float size) 
{
	//uint length = strlen(text);
	//if (DEBUG) printf("printText2D; length = %i\n", length);

	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
	for (unsigned int i=0; i<length; i++)
	{
		
		glm::vec2 vertex_up_left    = glm::vec2( x+i*size/2.0f     , y+size );
		glm::vec2 vertex_up_right   = glm::vec2( x+i*size/2.0f+size/2.0f, y+size );
		glm::vec2 vertex_down_right = glm::vec2( x+i*size/2.0f+size/2.0f, y      );

		glm::vec2 vertex_down_left  = glm::vec2( x+i*size/2.0f     , y      );

		vertices.push_back(vertex_up_left   );
		vertices.push_back(vertex_down_left );
		vertices.push_back(vertex_up_right  );

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character%16)/16.0f;
		float uv_y = (character/16)/16.0f;

		glm::vec2 uv_up_left    = glm::vec2( uv_x           , uv_y );
		glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/16.0f, uv_y );
		glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f, (uv_y + 1.0f/16.0f) );
		glm::vec2 uv_down_left  = glm::vec2( uv_x           , (uv_y + 1.0f/16.0f) );
		UVs.push_back(uv_up_left   );
		UVs.push_back(uv_down_left );
		UVs.push_back(uv_up_right  );

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, m_textVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_textUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_DYNAMIC_DRAW);

	// Bind shader
	glUseProgram(m_textProgramID);

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textTexture);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(m_textTextureID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_textVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_textUVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 0);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	//glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

GLuint Renderer::_loadDDS(const char * imagepath) 
{
	unsigned char header[124];

	FILE *fp; 
 
	/* try to open the file */ 
	fp = fopen(imagepath, "rb"); 
	if (fp == NULL)
	{
		printf("%s could not be opened.\n", imagepath); getchar(); 
		return 0;
	}
   
	/* verify the type of file */ 
	char filecode[4]; 
	fread(filecode, 1, 4, fp); 
	if (strncmp(filecode, "DDS ", 4) != 0) 
	{ 
		fclose(fp); 
		return 0; 
	}
	
	/* get the surface desc */ 
	fread(&header, 124, 1, fp); 

	unsigned int height      = *(unsigned int*)&(header[8 ]);
	unsigned int width	     = *(unsigned int*)&(header[12]);
	unsigned int linearSize	 = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC      = *(unsigned int*)&(header[80]);

 
	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */ 
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize; 
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char)); 
	fread(buffer, 1, bufsize, fp); 
	/* close the file pointer */ 
	fclose(fp);

	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4; 
	unsigned int format;
	switch(fourCC) 
	{ 
	    case FOURCC_DXT1: 
		    format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; 
		    break; 
	    case FOURCC_DXT3: 
		    format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
		    break; 
	    case FOURCC_DXT5: 
		    format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
		    break; 
	    default: 
		    free(buffer); 
		    return 0; 
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);	
	
	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
	unsigned int offset = 0;

	/* load the mipmaps */ 
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) 
	{ 
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize; 
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,  
			0, size, buffer + offset); 
	 
		offset += size; 
		width  /= 2; 
		height /= 2; 

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if(width < 1) width = 1;
		if(height < 1) height = 1;
	} 

	free(buffer); 

	return textureID;
}

void Renderer::_setPlus2Colors()
{
    // set all markers to hidden
    for (uint i = 0; i < 5; i++) // i is the territory index
    {
        m_plus2ColorData[3 * i + 0] = MapData::BUTTON_HIDDEN[0];
        m_plus2ColorData[3 * i + 1] = MapData::BUTTON_HIDDEN[1];
        m_plus2ColorData[3 * i + 2] = MapData::BUTTON_HIDDEN[2];
    }
    //printf("probe 1 in\n");
    std::vector<Card> * cards = (*(m_sd->players))[m_sd->activePlayerIndex]->getCards();
    //printf("probe 2 in\n");
    for (uint i = 0; i < cards->size(); i++) 
    {    
        //m_cardText[i] = MapData::TERRITORY_CHARS[(*cards)[i].territory] + " " + std::to_string((*cards)[i].numUnits);
        //if (DEBUG) printf("i = %i, m_sd->cardStatus[i] = %i\n", i, static_cast<int>(m_sd->cardStatus[i]));
        if (m_sd->territoryMarkers[(*cards)[i].territory] == m_sd->activePlayerIndex)
        {
            if (BUTTON_HIGHLIGHTED == m_sd->plus2Status[i])
            {
                m_plus2ColorData[3 * i + 0] = MapData::BUTTON_HIGHLIGHTED[0];
                m_plus2ColorData[3 * i + 1] = MapData::BUTTON_HIGHLIGHTED[1];
                m_plus2ColorData[3 * i + 2] = MapData::BUTTON_HIGHLIGHTED[2];
            }
            else if (BUTTON_PRESSED == m_sd->plus2Status[i])
            {
                m_plus2ColorData[3 * i + 0] = MapData::BUTTON_PRESSED[0];
                m_plus2ColorData[3 * i + 1] = MapData::BUTTON_PRESSED[1];
                m_plus2ColorData[3 * i + 2] = MapData::BUTTON_PRESSED[2];
            }
            else
            {
                m_plus2ColorData[3 * i + 0] = MapData::BUTTON_INACTIVE[0];
                m_plus2ColorData[3 * i + 1] = MapData::BUTTON_INACTIVE[1];
                m_plus2ColorData[3 * i + 2] = MapData::BUTTON_INACTIVE[2];
            }
        }
        //if (DEBUG) printf("m_sd->plus2Status[%i] = %i\n", i, m_sd->plus2Status[i]);
    }
    //printf("probe 3 in\n");
    glGenBuffers(1, &m_plus2ColorBuffer); //TODO should this block be done here?
    glBindBuffer(GL_ARRAY_BUFFER, m_plus2ColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_colorDim * 5 * sizeof(GLubyte), m_plus2ColorData, GL_STREAM_DRAW); //TODO should likely not be static draw   
}

void Renderer::_drawPlus2()
{
    // rows will be continents
    // each row will have a sequence of square button markers that will either be blank or will have a card on it you own
    // there will be no left alignment, only set territory spaces
    // could use similar rendering code to the markers
    
    glUseProgram(m_plus2ProgramID);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_plus2VertexBuffer);
    glVertexAttribPointer(
	    0,
	    m_dim,
	    GL_FLOAT,
	    GL_FALSE,
	    0,
	    (void*)0
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_plus2PositionBuffer);
    glVertexAttribPointer(
	    1,
	    2,
	    GL_FLOAT,
	    GL_FALSE,
	    0,
	    (void*)0
    );

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_plus2ColorBuffer);
    glVertexAttribPointer(
	    2,
	    m_colorDim,
	    GL_UNSIGNED_BYTE,
	    GL_TRUE,
	    0,
	    (void*)0
    );

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 5);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);   
}

void Renderer::_setPlus2Text()
{
    // print the territory name
    // print the number of units
    for (uint i = 0; i < 5; i++) 
        m_plus2Text[i] = " ";
    
    std::vector<Card> * cards = (*(m_sd->players))[m_sd->activePlayerIndex]->getCards();
    for (uint i = 0; i < cards->size(); i++) 
    {
        if (m_sd->selectedCards.size() == 3 && m_sd->cardsAreLegalToTurnIn)//m_sd->buttonStatus[5] != BUTTON_HIDDEN) // this button status indicates that we have 3 cards selected
        {    
            if (m_sd->territoryMarkers[(*cards)[i].territory] == m_sd->activePlayerIndex) m_plus2Text[i] = "+2";
            //if (DEBUG) printf("+2\n");
        }
        else
        {
            if (m_sd->territoryMarkers[(*cards)[i].territory] == m_sd->activePlayerIndex) m_plus2Text[i] = "Owned";
            //if (DEBUG) printf("owned\n");
        }
    }
}
