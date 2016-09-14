//TODO currently only set up to run for 2 human players. extend this in future iterations.
//TODO currently not resizable
//TODO currently programmed to fit a 4k display with 2x display magnification as compensation in the OS

//TODO defend -> defender/defending // same with attack // should be consistent with naming

//TODO some coding standards

//TODO could probably use a game state struct and then design a state diagram for easier game phase and state variable tracking

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <string>
#include <string.h>
#include <cstdio>

#include "generalData.h"
#include "gameBoard.h"
#include "splash.h"
#include "renderer.h"

int main(void) {
    // init gl and glew
    if(!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW.\n");
		return -1;
	}
    
    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    uint window_width = mode->width;
    uint window_height = mode->height;
    
    ///
    
    glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow * splashWindow;
    
	splashWindow = glfwCreateWindow(SPLASH_WINDOW_X, SPLASH_WINDOW_Y, "Map", NULL, NULL);
	if(splashWindow == NULL) {
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(splashWindow, (window_width - SPLASH_WINDOW_X) / 2.0f, (window_height - SPLASH_WINDOW_Y) / 2.0f);
	//glfwSetWindowPos(splashWindow, 10, 10);
	glfwMakeContextCurrent(splashWindow);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW.\n");
		glfwTerminate();
		return -1;
	}
    if (DEBUG) printf("glew init\n");
	glfwSetInputMode(splashWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(splashWindow, GLFW_STICKY_MOUSE_BUTTONS, 1);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
    
    // display a spalsh screen
    Splash splash;
    
    do 
    {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		splash.display();
		
		glfwSwapBuffers(splashWindow);
		
		glfwPollEvents();
	} 
	while (glfwGetKey(splashWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	       glfwGetKey(splashWindow, GLFW_KEY_ENTER) != GLFW_PRESS &&
		   glfwWindowShouldClose(splashWindow) == 0);
	   
    if (glfwGetKey(splashWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(splashWindow) != 0)
    {
        glfwTerminate();
        return 0;
    }

    //glfwTerminate();//glfwWindowShouldClose(splashWindow);
    //TODO should be do a delete for splashWindow or any glfw special function for that purpose?
    //splashWindow = nullptr;
    ///
    glfwDestroyWindow(splashWindow);

    glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow * window;
    
	window = glfwCreateWindow(WINDOW_X, WINDOW_Y, "Risk", NULL, NULL);
	if(window == NULL) {
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, (window_width - WINDOW_X) / 2.0f, (window_height - WINDOW_Y) / 2.0f);
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW.\n");
		glfwTerminate();
		return -1;
	}
    if (DEBUG) printf("glew init\n");
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
    
    // init game
    std::vector<Player *> players = {new HumanPlayer(), new HumanPlayer()};
    if (DEBUG) printf("main probe 1\n");
    GameBoard gameBoard(window, &players);
    if (DEBUG) printf("main probe 2\n");
    Renderer renderer(gameBoard.getSharedData());
    if (DEBUG) printf("main probe 3\n");
    // game loop
    do 
    {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		gameBoard.update();
	    	
		renderer.display();
		
		glfwSwapBuffers(window);
		
		glfwPollEvents();
	} 
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0);
    
	glfwTerminate();
    
    return 0;
}
