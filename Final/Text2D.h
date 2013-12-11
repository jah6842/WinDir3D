#ifndef _TEXT2D_H
#define _TEXT2D_H

#include "GLFW\glfw3.h"

void initText2D(const char * texturePath);
void setWindowSizeText2D(GLFWwindow* window);
void printText2D(const char * text, int x, int y, int size);
void cleanupText2D();

#endif // _TEXT2D_H