#ifndef RENDERER_H
#define RENDERER_H

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>

#include "gameData.h"
#include "sharedData.h"
//#include "gameBoard.h"

class Renderer {
    private:
        static const uint m_dim = 3; //TODO should maybe be all caps
        static const uint m_colorDim = 3; //TODO should maybe be all caps
    
        GLuint m_vertexArrayID;
        GLuint m_mapTextureID;
        GLuint m_mapTexture;
        GLuint m_mapUVModID;
        float m_mapUVMod[2];
        GLuint m_mapProgramID;
        GLuint m_mapVertexBuffer;
        float m_mapVertexData[m_dim * 4];
        
        //float * m_markerPositionData;
        GLuint m_markerPositionBuffer;
        
        GLuint m_markerProgramID;
        float m_markerVertexData[m_dim * 4];
        GLuint m_markerVertexBuffer;
        GLubyte m_markerColorData[m_colorDim * MapData::NUM_TERRITORIES];
        GLuint m_markerColorBuffer;
        
        GLuint m_textTexture;
        GLuint m_textTextureID;
        GLuint m_textVertexBufferID;
        GLuint m_textUVBufferID;
        GLuint m_textProgramID;
        
        GLuint m_cardProgramID;
        GLuint m_cardVertexBuffer;
        float m_cardVertexData[m_dim * 4];
        GLuint m_cardPositionBuffer;
        //float m_cardPositionData[]
        GLuint m_cardColorBuffer;
        GLubyte m_cardColorData[m_colorDim * 5];
        std::string m_cardText[5];
        
        GLuint m_plus2ProgramID;
        GLuint m_plus2VertexBuffer;
        float m_plus2VertexData[m_dim * 4];
        GLuint m_plus2PositionBuffer;
        //float m_plus2PositionData[]
        GLuint m_plus2ColorBuffer;
        GLubyte m_plus2ColorData[m_colorDim * 5];
        std::string m_plus2Text[5];
        
        //uint * m_territoryMarkers;
        //uint * m_troopCounts;
        //uint * m_activePlayerIndex;
        //GamePhase * m_gamePhase;
        
        GLuint m_buttonProgramID;
        float m_buttonVertexData[m_dim * 4];
        GLuint m_buttonVertexBuffer;
        //float * m_buttonModifierData;
        GLuint m_buttonModifierBuffer;
        GLubyte m_buttonColorData[m_colorDim * MapData::NUM_BUTTONS];
        GLuint m_buttonColorBuffer;
        
        char m_counterText[MapData::NUM_TERRITORIES][2];
        
        std::string m_gamePhaseIndicator;
        
        static const uint m_controlPanelTextLength = MapData::NUM_LABELS;
        std::string m_controlPanelText[MapData::NUM_LABELS];
        
        SharedData * m_sd;
    
    private:
        GLuint _loadProgram(const char * shaderPath, const char * fragPath);
        GLuint _loadBMP(const char * mapTexturePath);
        GLuint _loadDDS(const char * imagepath);
        void _drawMap();
        void _setCardColors();
        void _drawCards();
        void _setCardText();
        void _drawMarkers();
        void _printText2D(const char * text, uint length, float x, float y, float size);
        void _printText2D(std::vector<char> text, uint length, float x, float y, float size); 
        void _printText2D(std::string text, float x, float y, float size); 
        void _setCounterText();
        void _determineGamePhase();
        void _setTerritoryColors();
        void _setButtonColors();
        void _drawButtons();
        void _determineTurnNum();
        void _determineAttackerAndDefender();
        void _determineStartAndEndTerritories();
        void _determineNumNewUnits();
        void _determineAttackResult();
        void _controlPanelTextAppendDigits(uint n, uint i);
        void _setPlus2Colors();
        void _drawPlus2();
        void _setPlus2Text();
    
    public:
        //Renderer(GameBoard * gameBoard);
        Renderer(SharedData * sharedData);
        ~Renderer();
        void display();
};
    
#endif
