#include "splash.h"

Splash::Splash()
{
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);
    
    // map
    
    // map rendering programs
    m_splashProgramID = _loadProgram("splashVertexShader.glsl", "splashFragmentShader.glsl");
    
    // map texture
    m_splashTexture = _loadBMP(MapData::SPLASH_TEXTURE_PATH);
    m_splashTextureID = glGetUniformLocation(m_splashProgramID, "splashTextureSampler");
    
    // map vertices
    float splashVertexData[] = {
        -1.0f, -1.0f, 0.0f,
        1.0, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0, 1.0f, 0.0f
    };
    //m_mapVertexData = new float[m_dim * 4];
    for (int i = 0; i < m_dim * 4; i++)
        m_splashVertexData[i] = splashVertexData[i];
        
    glGenBuffers(1, &m_splashVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_splashVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_dim * 4 * sizeof(float), m_splashVertexData, GL_STATIC_DRAW);
    
    //m_mapUVModID = glGetUniformLocation(m_splashProgramID, "UVMod");
    //m_mapUVMod[0] =  1.0f; // for x->u
    //m_mapUVMod[1] =  1.0f;    // for y->v
}

Splash::~Splash() 
{
    glDeleteBuffers(1, &m_splashVertexBuffer);
    glDeleteTextures(1, &m_splashTextureID);
    glDeleteProgram(m_splashProgramID);
    glDeleteVertexArrays(1, &m_vertexArrayID);
}

void Splash::display() 
{
    glUseProgram(m_splashProgramID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_splashTexture);
    glUniform1i(m_splashTextureID, 0);

    //glUniform2f(m_splashUVModID, m_splashUVMod[0], m_splashUVMod[1]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_splashVertexBuffer);
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

//TODO below here are duplicates from renderer.h that should be static in that class

GLuint Splash::_loadProgram(const char * shaderPath, const char * fragPath) 
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

GLuint Splash::_loadBMP(const char * imagepath) 
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
