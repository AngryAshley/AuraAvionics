#ifndef LUTIL_H
#define LUTIL_H

#include "LOpenGL.h"
#include <stdio.h>
#include <string>

//Screen Constants
const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;
const int SCREEN_FPS = 60;


bool initGL();

void setLUtilPath(std::string data);

bool loadMedia();

void update();

void render();
/*
Pre Condition:
 -A valid OpenGL context
 -Active modelview matrix
Post Condition:
 -Renders the scene
Side Effects:
 -Clears the color buffer
 -Swaps the front/back buffer
*/

void handleKeys( unsigned char key, int x, int y );

#endif
