#ifndef SPLASH_H
#define SPLASH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>

#include "gameData.h"

typedef unsigned int uint;

class Splash 
{
    private:
        static const uint m_dim = 3;
        
        GLuint m_vertexArrayID;
        GLuint m_splashProgramID;
        GLuint m_splashTexture;
        GLuint m_splashTextureID;
        GLuint m_splashVertexBuffer;
        float m_splashVertexData[m_dim * 4];

    private:
        GLuint _loadProgram(const char * shaderPath, const char * fragPath);
        GLuint _loadBMP(const char * imagepath);
        
    public:
        Splash();
        ~Splash();
        void display();
};

#endif
