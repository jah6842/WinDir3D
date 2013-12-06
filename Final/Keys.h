#ifndef _KEYS_H
#define _KEYS_H

#include "GLFW\glfw3.h"
#include <iostream>

class Keys {
public:
	static bool A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
		UP,DOWN,LEFT,RIGHT,
		LSHIFT,RSHIFT,SHIFT, SPACE;
	static void Input(int key, int scancode, int action, int mods);
};

#endif // _KEYS_H
