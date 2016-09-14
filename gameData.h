#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <GL/glew.h>
#include <string.h>
#include <string>

#include "generalData.h"

typedef unsigned int uint;

enum Continent 
{
    AFRICA = 0,
    ASIA = 1,
    AUSTRALIA = 2,
    EUROPE = 3,
    NORTH_AMERICA = 4,
    SOUTH_AMERICA = 5
};

enum Territory 
{
    NORTH_AFRICA = 0,
    EGYPT = 1,
    CONGO = 2,
    EAST_AFRICA = 3,
    SOUTH_AFRICA = 4,
    MADAGASCAR = 5,
    
    URAL = 6,
    SIBERIA = 7,
    YAKUTSK = 8,
    KAMCHATKA = 9,
    IRKUTSK = 10,
    AFGHANISTAN = 11,
    CHINA = 12,
    MONGOLIA = 13,
    JAPAN = 14,
    MIDDLE_EAST = 15,
    INDIA = 16,
    SIAM = 17,
    
    INDONESIA = 18,
    NEW_GUINEA = 19,
    WESTERN_AUSTRALIA = 20,
    EASTERN_AUSTRALIA = 21,
    
    ICELAND = 22,
    SCANDINAVIA = 23,
    UKRAINE = 24,
    GREAT_BRITAIN = 25,
    NORTHERN_EUROPE = 26,
    WESTERN_EUROPE = 27,
    SOUTHERN_EUROPE = 28,
    
    ALASKA = 29,
    NORTH_WEST_TERRITORY = 30,
    GREENLAND = 31,
    ALBERTA = 32,
    ONTARIO = 33,
    QUEBEC = 34,
    WESTERN_UNITED_STATES = 35,
    EASTERN_UNITED_STATES = 36,
    CENTRAL_AMERICA = 37,
    
    VENEZUELA = 38,
    PERU = 39,
    BRAZIL = 40,
    ARGENTINA = 41,
    
    NULL_TERRITORY = -1 // also to be interpreted as Wild when used in a card struct
};

enum GamePhase 
{
    SETUP = 0,
    ARMIES = 1, // getting and placing new armies
    ATTACK = 2, // attacking
    FORTIFY = 3, // fortifying
    END = 4,     // game ended
    ATTACK_MOVE = 5, // after conquering a territory, move additional units to fronline
    SETUP_FINAL = 6 // the final setup turn (typicall the last one by player red before the start of the first armies phase)
};

enum ButtonStatus 
{ //TODO deprecated
    BUTTON_INACTIVE = 0,
    BUTTON_HIGHLIGHTED = 1,
    BUTTON_PRESSED = 2,
    BUTTON_HIDDEN = 3
};

struct Card 
{
    Territory territory;
    uint numUnits;
};

namespace MapData 
{
    static const uint NUM_CONTINENTS = 6; // based on Territory enum
    static const uint NUM_TERRITORIES = 42; // based on Territory enum
    static const bool MAP[NUM_TERRITORIES][NUM_TERRITORIES] = {
        {true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, true, false}, //NORTH_AFRICA = 0,
        
        {true, true, false, true, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false}, //EGYPT = 1,
        
        {true, false, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //CONGO = 2,
        
        {true, true, true, true, true, true, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //EAST_AFRICA = 3,
        
        {false, false, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //SOUTH_AFRICA = 4,
        
        {false, false, false, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //MADAGASCAR = 5,
        
        {false, false, false, false, false, false, true, true, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //URAL = 6,
        
        {false, false, false, false, false, false, true, true, true, false, true, false, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //SIBERIA = 7,
        
        {false, false, false, false, false, false, false, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //YAKUTSK = 8,
        
        {false, false, false, false, false, false, false, false, true, true, true, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false}, //KAMCHATKA = 9,
        
        {false, false, false, false, false, false, false, true, true, true, true, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //IRKUTSK = 10,
        
        {false, false, false, false, false, false, true, false, false, false, false, true, true, false, false, true, true, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //AFGHANISTAN = 11,
        
        {false, false, false, false, false, false, true, true, false, false, false, true, true, true, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //CHINA = 12,
        
        {false, false, false, false, false, false, false, true, false, true, true, false, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //MONGOLIA = 13,
        
        {false, false, false, false, false, false, false, false, false, true, false, false, false, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //JAPAN = 14,
        
        {false, true, false, true, false, false, false, false, false, false, false, true, false, false, false, true, true, false, false, false, false, false, false, false, true, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false}, //MIDDLE_EAST = 15,
        
        {false, false, false, false, false, false, false, false, false, false, false, true, true, false, false, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //INDIA = 16,
        
        {false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //SIAM = 17,
        
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //INDONESIA = 18,
        
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //NEW_GUINEA = 19,
        
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //WESTERN_AUSTRALIA = 20,
        
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //EASTERN_AUSTRALIA = 21,
        
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, false, true, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false}, //ICELAND = 22,
        
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //SCANINAVIA = 23,
        
        {false, false, false, false, false, false, true, false, false, false, false, true, false, false, false, true, false, false, false, false, false, false, false, true, true, false, true, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false}, //UKRAINE = 24,
        
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, false, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false}, //GREAT_BRITAIN = 25,
        
        {false, false,false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false}, //NORTHERN_EUROPE = 26,
        
        {true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false}, //WESTERN_EUROPE = 27,
        
        {true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, true, false, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false}, //SOUTHERN_EUROPE = 28,
        
        {false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, false, true, false, false, false, false, false, false, false, false, false}, //ALASKA = 29,
        
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, false, false, false, false, false, false, false, false}, //NORTH_WEST_TERRITORY = 30,
        
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, false, false, true, true, false, true, true, false, false, false, false, false, false, false}, //GREENLAND = 31,
        
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, false, true, true, false, true, false, false, false, false, false, false}, //ALBERTA = 32,
        
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, false, false, false, false, false}, //ONTARIO = 33,
        
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, true, true, false, true, false, false, false, false, false}, //QUEBEC = 34,
        
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, false, true, true, true, false, false, false, false}, //WESTERN_UNITED_STATES = 35,
        
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, false, false, false, false}, //EASTERN_UNITED_STATES = 36,
        
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, false, false, false}, //CENTRAL_AMERICA = 37,
        
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, false}, //VENEZUELA = 38,
        
        {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true}, //PERU = 39,
        
        {true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true}, //BRAZIL = 40,
        
        {false, false, false, false, false, false, false,false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true} //ARGENTINA = 41
    };
    
    static const uint TERRITORY_POSITIONS[NUM_TERRITORIES][2] = { // in texture/image coordinates
        {457, 415},//NORTH_AFRICA = 0,
        {541, 388},//EGYPT = 1,
        {536, 511},//CONGO = 2,
        {577, 451},//EAST_AFRICA = 3,
        {544, 603},//SOUTH_AFRICA = 4,
        {640, 606},//MADAGASCAR = 5,
        
        {696, 157},//URAL = 6,
        {751, 104},//SIBERIA = 7,
        {831, 74},//YAKUTSK = 8,
        {914, 80},//KAMCHATKA = 9,
        {815, 163},//IRKUTSK = 10,
        {679, 252},//AFGHANISTAN = 11,
        {804, 300},//CHINA = 12,
        {830, 231},//MONGOLIA = 13,
        {936, 239},//JAPAN = 14,
        {618, 353},//MIDDLE_EAST = 15,
        {739, 355},//INDIA = 16,
        {820, 388},//SIAM = 17,
        
        {835, 492},//INDONESIA = 18,
        {926, 470},//NEW_GUINEA = 19,
        {868, 604},//WESTERN_AUSTRALIA = 20,
        {973, 611},//EASTERN_AUSTRALIA = 21,
        
        {425, 129},//ICELAND = 22,
        {506, 118},//SCANDINAVIA = 23,
        {604, 154},//UKRAINE = 24,
        {398, 208},//GREAT_BRITAIN = 25,
        {495, 222},//NORTHERN_EUROPE = 26,
        {421, 318},//WESTERN_EUROPE = 27,
        {507, 297},//SOUTHERN_EUROPE = 28,
        
        {52, 101},//ALASKA = 29,
        {165, 101},//NORTH_WEST_TERRITORY = 30,
        {344, 61},//GREENLAND = 31,
        {140, 154},//ALBERTA = 32,
        {210, 167},//ONTARIO = 33,
        {277, 167},//QUEBEC = 34,
        {143, 233},//WESTERN_UNITED_STATES = 35,
        {219, 259},//EASTERN_UNITED_STATES = 36,
        {150, 307},//CENTRAL_AMERICA = 37,
        
        {214, 380},//VENEZUELA = 38,
        {233, 473},//PERU = 39,
        {308, 455},//BRAZIL = 40,
        {254, 549}//ARGENTINA = 41
    };
    
    static const std::string TERRITORY_CHARS[NUM_TERRITORIES + 1] = {
        "North Africa",//{'N','O','R','T','H',' ','A','F','R','I','C','A'},
        "Egypt",//{'E','G','Y','P','T'},
        "Congo",//{'C','O','N','G','O'},
        "East Africa",//{'E','A','S','T',' ','A','F','R','I','C','A'},
        "South Africa",//{'S','O','U','T','H',' ','A','F','R','I','C','A'},
        "Madagascar",//{'M','A','D','A','G','A','S','C','A','R'},
        
        "Ural",//{'U','R','A','L'},
        "Siberia",//{'S','I','B','E','R','I','A'},
        "Yakutsk",//{'Y','A','K','U','T','S','K'},
        "Kamchatka",//{'K','A','M','C','H','A','T','K','A'},
        "Irkutsk",//{'I','R','K','U','T','S','K'},
        "Afghanistan",//{'A','F','G','H','A','N','I','S','T','A','N'},
        "China",//{'C','H','I','N','A'},
        "Mongolia",//{'M','O','N','G','O','L','I','A'},
        "Japan",//{'J','A','P','A','N'},
        "Middle East",//{'M','I','D','D','L','E',' ','E','A','S','T'},
        "India",//{'I','N','D','I','A'},
        "Siam",//{'S','I','A','M'},
        
        "Indonesia",//{'I','N','D','O','N','E','S','I','A'},
        "New Guinea",//{'N','E','W',' ','G','U','I','N','E','A'},
        "Western Australia",//{'W','E','S','T','E','R','N',' ','A','U','S','T','R','A','L','I','A'},
        "Eastern Australia",//{'E','A','S','T','E','R','N',' ','A','U','S','T','R','A','L','I','A'},
        
        "Iceland",//{'I','C','E','L','A','N','D'},
        "Scandinavia",//{'S','C','A','N','D','I','N','A','V','I','A'},
        "Ukraine",//{'U','K','R','A','I','N','E'},
        "Great Britain",//{'G','R','E','A','T',' ','B','R','I','T','A','I','N'},
        "Northern Europe",//{'N','O','R','T','H','E','R','N',' ','E','U','R','O','P','E'},
        "Western Europe",//{'W','E','S','T','E','R','N',' ','E','U','R','O','P','E'},
        "Southern Europe",//{'S','O','U','T','H','E','R','N',' ','E','U','R','O','P','E'},
        
        "Alaska",//{'A','L','A','S','K','A'},
        "North West Territory",//{'N','O','R','T','H',' ','W','E','S','T',' ','T','E','R','R','I','T','O','R','Y'},
        "Greenland",//{'G','R','E','E','N','L','A','N','D'},
        "Alberta",//{'A','L','B','E','R','T','A'},
        "Ontario",//{'O','N','T','A','R','I','O'},
        "Quebec",//{'Q','U','E','B','E','C'},
        "Western United States",//{'W','E','S','T','E','R','N',' ','U','N','I','T','E','D',' ','S','T','A','T','E','S'},
        "Eastern United States",//{'E','A','S','T','E','R','N',' ','U','N','I','T','E','D',' ','S','T','A','T','E','S'},
        "Central America",//{'C','E','N','T','R','A','L',' ','A','M','E','R','I','C','A'},
        
        "Venezuela",//{'V','E','N','E','Z','U','E','L','A'},
        "Peru",//{'P','E','R','U'},
        "Brazil",//{'B','R','A','Z','I','L'},
        "Argentina",//{'A','R','G','E','N','T','I','N','A'},
        " " // clear case
    };
    
    static const uint NUM_LABELS = 16;
    
    static const float LABEL_CHAR_SIZE = 1.0f / 24.0f;
    static const float LABEL_HEIGHT = LABEL_CHAR_SIZE;
    
    static const std::string LABELS[NUM_LABELS] = {
        "RISK (green is neutral)", //0
	    "Turn: ", //1
	    "Player: ", //2
	    "Phase: ", //3
	    "Attacker: ", //4
	    "Defender: ", //5
	    "Attacker dice: 1 2 3", //6 // buttons 0, 1, 2
	    "Defender dice: 1 2", //7 // buttons 3, 4
	    "Attack result: ", //8
	    "New units: ", //9
	    "Use cards", //10 // button 5
	    "View/Hide cards", //11 // button 6
	    "Attack", //12 // button 7
	    "View/Hide units", //13 // button 8
	    "End phase", //14 // button 9
	    "Message: " //15
    };
    
    static const std::string ALT_LABELS[7] = {
        "Start: ", //0 // alt for 4
	    "End: ", //1 // alt for 5
	    "Move", //2 // alt for 12
	    "Done", //3
	    "Next", //4 // alt for 12
	    "Turns remaining: ", //5 // alt for 1
	    {" "} //6 hidden label
    };
    
    static const float LABEL_CENTERS[NUM_LABELS][2] = {
        {X_EXTENT + 0.05f, -1.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f},
        {X_EXTENT + 0.05f, -2.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f},
        {X_EXTENT + 0.05f, -3.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f},
        {X_EXTENT + 0.05f, -4.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f},
        {X_EXTENT + 0.05f, -5.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f},
        {X_EXTENT + 0.05f, -6.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f},
        {X_EXTENT + 0.05f, -7.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f},
        {X_EXTENT + 0.05f, -8.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f},
        {X_EXTENT + 0.05f, -9.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f},
        {X_EXTENT + 0.05f, -10.0f * 2.0f / static_cast<float>(NUM_LABELS +2) - LABEL_HEIGHT / 2.0f + 1.0f},
        {X_EXTENT + 0.05f, -11.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f},
        {X_EXTENT + 0.05f, -12.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f},
        {X_EXTENT + 0.05f, -13.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f},
        {X_EXTENT + 0.05f, -14.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f},
        {X_EXTENT + 0.05f, -15.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f},
        {X_EXTENT + 0.05f, -16.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f}
    };
    
    static const uint NUM_BUTTONS = 10;
    
    static const float BUTTON_HEIGHT = LABEL_HEIGHT;
    static const float BUTTON_HALF_HEIGHT = LABEL_HEIGHT / 2.0f;
    
    static const float BUTTON_WIDTHS[NUM_BUTTONS] = {
        LABEL_CHAR_SIZE / 4.0f, //6  //0
        LABEL_CHAR_SIZE / 4.0f, //6  //1
        LABEL_CHAR_SIZE / 4.0f, //6  //2
        LABEL_CHAR_SIZE / 4.0f, //7  //3
        LABEL_CHAR_SIZE / 4.0f, //7  //4
        LABEL_CHAR_SIZE * LABELS[10].size() / 4.0f, //10  //5
        LABEL_CHAR_SIZE * LABELS[11].size() / 4.0f, //11  //6
        LABEL_CHAR_SIZE * LABELS[12].size() / 4.0f, //12  //7
        LABEL_CHAR_SIZE * LABELS[13].size() / 4.0f, //13  //8
        LABEL_CHAR_SIZE * LABELS[14].size() / 4.0f  //14  //9
    }; 
    
    static const float BUTTON_HALF_WIDTHS[NUM_BUTTONS] = { //TODO something happend with the calc here so it go tuned instead
        BUTTON_WIDTHS[0],// / 2.0f, //6
        BUTTON_WIDTHS[1],// / 2.0f, //6
        BUTTON_WIDTHS[2],// / 2.0f, //6
        BUTTON_WIDTHS[3],// / 2.0f, //7
        BUTTON_WIDTHS[4],// / 2.0f, //7
        BUTTON_WIDTHS[5],// / (2.0f * 1.32f), //10
        BUTTON_WIDTHS[6],// / (2.0f * 1.15f), //11
        BUTTON_WIDTHS[7],// / (2.0f * 1.5f), //12
        BUTTON_WIDTHS[8],// / (2.0f * 1.15f), //13
        BUTTON_WIDTHS[9]//{X_EXTENT + 0.05f, 16.0f * static_cast<float>(MAP_WINDOW_Y) / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f} //15
    }; 
    
    static const float BUTTON_CENTERS[NUM_BUTTONS][2] = { //TODO something happend with the calc here so it go tuned instead
        {X_EXTENT + 1.15f * 0.05f + LABEL_CHAR_SIZE * LABELS[6].size() / 2.0f - 3.0f * LABEL_CHAR_SIZE, -7.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f}, //6
        {X_EXTENT + 1.15f * 0.05f + LABEL_CHAR_SIZE * LABELS[6].size() / 2.0f - 2.0f * LABEL_CHAR_SIZE, -7.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f}, //6
        {X_EXTENT + 1.15f * 0.05f + LABEL_CHAR_SIZE * LABELS[6].size() / 2.0f - 1.0f * LABEL_CHAR_SIZE, -7.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f}, //6
        {X_EXTENT + 1.15f * 0.05f + LABEL_CHAR_SIZE * LABELS[7].size() / 2.0f - 2.0f * LABEL_CHAR_SIZE, -8.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f}, //7
        {X_EXTENT + 1.15f * 0.05f + LABEL_CHAR_SIZE * LABELS[7].size() / 2.0f - 1.0f * LABEL_CHAR_SIZE, -8.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f}, //7
        {X_EXTENT + 0.05f + BUTTON_WIDTHS[5] / 1.32f, -11.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f}, //10
        {X_EXTENT + 0.05f + BUTTON_WIDTHS[6] / 1.15f, -12.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f}, //11
        {X_EXTENT + 0.05f + BUTTON_WIDTHS[7] / 1.5f, -13.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f}, //12
        {X_EXTENT + 0.05f + BUTTON_WIDTHS[8] / 1.15f, -14.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f}, //13
        {X_EXTENT + 0.05f + BUTTON_WIDTHS[9] / 1.32f, -15.0f * 2.0f / static_cast<float>(NUM_LABELS + 2) - LABEL_HEIGHT / 2.0f + 1.0f} //14
    }; 
    
    static const uint BUTTON_2_LABEL[NUM_BUTTONS] = {
        6,
        6,
        6,
        7,
        7,
        10,
        11,
        12,
        13,
        14
    };
    
    static const char * SPLASH_TEXTURE_PATH = "splashTexture.bmp";
    static const char * MAP_TEXTURE_PATH = "mapTexture.bmp";
    static const char * TEXT_TEXTURE_PATH = "Holstein.DDS";
    
    static const uint MAP_PIXEL_WIDTH = 1020; //TODO should this be read in?
    static const uint MAP_PIXEL_HEIGHT = 675; //TODO should this be read in?
    static const float MAP_PIXEL_HALF_WIDTH = static_cast<float>(MAP_PIXEL_WIDTH) / 2.0f;
    static const float MAP_PIXEL_HALF_HEIGHT = static_cast<float>(MAP_PIXEL_HEIGHT) / 2.0f;
    
    static const uint NUM_STARTING_UNITS_2P = 40;
    
    static const float TERRITORY_HALF_EXTENT = 1.0f / (2.0f * 16.0f);
    static const float CARD_HALF_EXTENT = 1.0f / (8.0f);
    
    static const GLubyte BLUE[3] = {0, 193, 232};
    static const GLubyte RED[3] = {232, 39, 0};
    static const GLubyte GREEN[3] = {77, 232, 0};
    static const GLubyte PURPLE[3] = {155, 0, 232};
    
    static const GLubyte LIGHT_BLUE[3] = {50 + 0, 40 + 193, 20 + 232};
    static const GLubyte LIGHT_RED[3] = {22 + 232, 60 + 39, 60 + 0};
    static const GLubyte LIGHT_GREEN[3] = {50 + 77, 20 + 232, 50 + 0};
    static const GLubyte LIGHT_PURPLE[3] = {50 + 155, 50 + 0, 20 + 232};
    
    static const GLubyte BUTTON_INACTIVE[3] = {75, 75, 75};
    static const GLubyte BUTTON_HIGHLIGHTED[3] = {155, 0, 232};
    static const GLubyte BUTTON_PRESSED[3] = {232, 39, 0};   
    static const GLubyte BUTTON_HIDDEN[3] = {0, 0, 0};
    
    static const uint NUM_CARDS = NUM_TERRITORIES + 2;
    static const Card CARDS[NUM_CARDS] = {
        {AFGHANISTAN, 1},//Afghanistan – Infantry
        {ALASKA, 1},//Alaska –Infantry
        {ALBERTA, 1},//Alberta – Infantry
        {ARGENTINA, 1},//Argentina – Infantry
        {BRAZIL, 10},//Brazil- Artillery
        {CENTRAL_AMERICA, 5},//Central America – Calvary
        {CHINA, 5},//China – Calvary
        {CONGO, 5},//Congo –Calvary
        {EAST_AFRICA, 10},//East Africa – Artillery
        {EASTERN_AUSTRALIA, 1},//Eastern Australia – Infantry
        {EASTERN_UNITED_STATES, 10},//Eastern United States – Artillery
        {EGYPT, 1},//Egypt – Infantry
        {GREAT_BRITAIN, 5},//Great Britain – Calvary
        {GREENLAND, 5},//Greenland –Calvary
        {ICELAND, 1},//Iceland – Infantry
        {INDIA, 1},//India –Infantry
        {INDONESIA, 5},//Indonesia –Calvary
        {IRKUTSK, 1},//Irkutsk –Infantry
        {JAPAN, 1},//Japan- Infantry
        {KAMCHATKA, 5},//Kamchatka – Calvary
        {MADAGASCAR, 1},//Madagascar – Infantry
        {MIDDLE_EAST, 10},//Middle East – Artillery
        {MONGOLIA, 10},//Mongolia – Artillery
        {NEW_GUINEA, 5},//New Guinea – Calvary
        {NORTH_AFRICA, 1},//North Africa – Infantry
        {NORTHERN_EUROPE, 5},//Northern Europe – Calvary
        {NORTH_WEST_TERRITORY, 10},//Northwest Territories – Artillery
        {ONTARIO, 5},//Ontario –Calvary
        {PERU, 5},//Peru – Calvary
        {QUEBEC, 10},//Quebec – Artillery
        {SCANDINAVIA, 10},//Scandinavia – Artillery
        {SIAM, 10},//Siam – Artillery
        {SIBERIA, 10},//Siberia – Artillery
        {SOUTH_AFRICA, 10},//South Africa – Artillery
        {SOUTHERN_EUROPE, 5},//Southern Europe – Calvary
        {UKRAINE, 10},//Ukraine – Artillery
        {URAL, 5},//Ural – Calvary
        {VENEZUELA, 5},//Venezuela – Artillery
        {WESTERN_AUSTRALIA, 10},//Western Australia – Artillery
        {WESTERN_EUROPE, 1},//Western Europe – Infantry
        {WESTERN_UNITED_STATES, 1},//Western United States – Infantry
        {NULL_TERRITORY, 0},//Wild Card (x2)
        {NULL_TERRITORY, 0},
        {YAKUTSK, 5}//Yakutsk – Calvary 
    };
    
    static const float CARD_POSITIONS[NUM_CARDS][2] = {
        {0, 0},//NORTH_AFRICA = 0,
        {1, 0},//EGYPT = 1,
        {2, 0},//CONGO = 2,
        {3, 0},//EAST_AFRICA = 3,
        {4, 0},//SOUTH_AFRICA = 4,
        {5, 0},//MADAGASCAR = 5,        
        {6, 0},//URAL = 6,
        {7, 0},//SIBERIA = 7,
        {8, 0},//YAKUTSK = 8,
        {9, 0},//KAMCHATKA = 9,
        {10, 0},//IRKUTSK = 10,
        
        {0, 1},//AFGHANISTAN = 11,
        {1, 1},//CHINA = 12,
        {2, 1},//MONGOLIA = 13,
        {3, 1},//JAPAN = 14,
        {4, 1},//MIDDLE_EAST = 15,
        {5, 1},//INDIA = 16,
        {6, 1},//SIAM = 17,
        {7, 1},//INDONESIA = 18,
        {8, 1},//NEW_GUINEA = 19,
        {9, 1},//WESTERN_AUSTRALIA = 20,
        {10,1},//EASTERN_AUSTRALIA = 21,
        
        {0, 2},//ICELAND = 22,
        {1, 2},//SCANDINAVIA = 23,
        {2, 2},//UKRAINE = 24,
        {3, 2},//GREAT_BRITAIN = 25,
        {4, 2},//NORTHERN_EUROPE = 26,
        {5, 2},//WESTERN_EUROPE = 27,
        {6, 2},//SOUTHERN_EUROPE = 28,
        {7, 2},//ALASKA = 29,
        {8, 2},//NORTH_WEST_TERRITORY = 30,
        {9, 2},//GREENLAND = 31,
        {10, 2},//ALBERTA = 32,
        
        {0, 3},//ONTARIO = 33,
        {1, 3},//QUEBEC = 34,
        {2, 3},//WESTERN_UNITED_STATES = 35,
        {3, 3},//EASTERN_UNITED_STATES = 36,
        {4, 3},//CENTRAL_AMERICA = 37,
        {5, 3},//VENEZUELA = 38,
        {6, 3},//PERU = 39,
        {7, 3},//BRAZIL = 40,
        {8, 3},//ARGENTINA = 41
        {9, 3},//WILD
        {10, 3}//WILD
    };
    
    static const float CARD_HEIGHT = LABEL_HEIGHT;
    static const float CARD_HALF_HEIGHT = LABEL_HEIGHT / 2.0f;
    
    static const float CARD_COL_WIDTHS[4] = {
        12 + 3, //pick out the longest per col
        17 + 3,
        20 + 3,
        21 + 3
    };
    
    static const float CARD_WIDTHS[NUM_CARDS] = {
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3]
    };
    
    static const float CARD_HALF_WIDTHS[NUM_CARDS] = { //TODO something happend with the calc here so it go tuned instead
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        CARD_COL_WIDTHS[0],
        
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        CARD_COL_WIDTHS[1],
        
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        CARD_COL_WIDTHS[2],
        
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3],
        CARD_COL_WIDTHS[3]
    }; 
};

#endif
